#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include <sys/stat.h>
#include "DataBlock.h"
#include "BitmapWriter.h"
#include "CompressedDataBlock.h"
#include "BitmapIndex.h"
#include "projectionindex.h"
#include "compressedprojectionindex.h"
#include "IntervalBitmapWriter.h"
#include "querylanguage.h"

using namespace std;


void createProjectionIndex(const string & indexfile, uint BlockSize,const string & csvfilename, const string & delimiter, const string & projectiontype) {
    if(projectiontype == "uncompressed") {
    	ProjectionIndexWriter piw(indexfile,BlockSize);
    	piw.indexCSVFile(csvfilename,false,delimiter);
    } else if(projectiontype == "8") {
        CompressedProjectionIndexWriter<RLE<uint,unsigned char> > piw(indexfile,BlockSize);
    	piw.indexCSVFile(csvfilename,false,delimiter);
    } else if(projectiontype == "16") {
    	CompressedProjectionIndexWriter<RLE<uint,unsigned short> > piw(indexfile,BlockSize);
    	piw.indexCSVFile(csvfilename,false,delimiter);
    } else if(projectiontype == "32") {
    	CompressedProjectionIndexWriter<RLE<uint,uint> > piw(indexfile,BlockSize);
    	piw.indexCSVFile(csvfilename,false,delimiter);
    } else if(projectiontype == "Kaser32") {
    	CompressedProjectionIndexWriter<KaserArray<uint,uint32> > piw(indexfile,BlockSize);
    	piw.indexCSVFile(csvfilename,false,delimiter);
    } else if(projectiontype == "Kaser64") {
    	CompressedProjectionIndexWriter<KaserArray<uint,uint64> > piw(indexfile,BlockSize);
    	piw.indexCSVFile(csvfilename,false,delimiter);
    } else {
    	cerr<<"Don't know projection type '"<<projectiontype<<"'"<<endl;
    	throw "bug";

    }
}


template <class DB>
void createIntervalIndex(const string & indexfile, uint BlockSize,const string & csvfilename, const string & delimiter = ",", const string intervaltype="range") {
    if (intervaltype=="sqrt") {
    	SqrtIntervalBitmapWriter<DB> piw(indexfile,BlockSize);
        piw.indexCSVFile(csvfilename,false,delimiter);
    }else if (intervaltype=="interval")
    	throw "not implemented yet!";
    else if (intervaltype=="range") {
    	RangeBitmapWriter<DB> piw(indexfile,BlockSize);
        piw.indexCSVFile(csvfilename,false,delimiter);
    }    else {
    	cerr<<intervaltype<<endl;
    	throw "flag not supported";
    }
}

/**
 * as the name suggests, this creates a bitmap index from a CSV data file.
 */

template <class DB>
void createIndex(string indexfile, bool verbose, uint BlockSize, string csvfilename, int k, bool forcek, BitmapAllocation & ba, string delimiter = ",") {
    //clock_t start,finish;
    //start = clock();
    KofNBitmapBudget K(k, !forcek);
    BitmapWriter<DB> bw(indexfile, BlockSize, K, ba);
    bw.indexCSVFile(csvfilename, verbose, delimiter);
    //finish = clock();
    //cout << " the bitmap index file is in  " << indexfile << endl;
    //cout << "took "<<static_cast<double>(finish - start) / CLOCKS_PER_SEC << " seconds  "<<endl;
}

void materializeHistogram(string csvfilename, string delimiter = ",") {
    histogramtype mHistograms;
    HistogramLoader::loadHistogram(csvfilename, mHistograms, delimiter, '#', false);
}

int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    /*if ((params < 3) or ((params < 4) and (strcmp(args[1],"-c")==0))){
            cout << "You must provide two parameters, a CSV file and the name of an output index file, " << endl;
            cout <<"you may prefix these two by the -c flag for compression."<<endl;
            cout <<"Example: ./indexcsv -c testdata.csv testindex.bin"<<endl;
            return -1;
    }*/
    clock_t start, finish;
    uint64 BlockSize = defaultUncompressedBlockSize;
    int k = 1;
    string delimiter(",");
    string fname;
    bool compression(true);
    bool verbose(false);
    bool sparsemode(false);
    AlphabeticalBitmapAllocation aba;
    WeightSortedBitmapAllocation wsba;
    GrayCodeBitmapAllocation gcba;
    GrayAllCodeBitmapAllocation gacba;
    WeightSortedGrayCodeBitmapAllocation wsgcba;
    WeightSortedGrayAllCodeBitmapAllocation wsgacba;
    GrayCodeBitmapAllocation gcbar(true);
    GrayAllCodeBitmapAllocation gacbar(true);
    WeightSortedGrayCodeBitmapAllocation wsgcbar(true);
    WeightSortedGrayAllCodeBitmapAllocation wsgacbar(true);
    WeightSortedGrayAllCodeBitmapAllocation wsgacbarr(true, true);
    BitmapAllocation * ba = &aba;
    vector<int> omittedcolumns;
    string csvfilename, indexfile;
    int c;
    bool projection(false);
    bool interval(false);
    uint bits = 32;
    bool forcek(false);
    bool justhisto(false);
    string intervaltype, projectiontype;
    //AdaptiveBitmapAllocation aba;
    while ((c = getopt(argc, argv, "i:joa:vcsK:k:d:b:B:p:")) != -1) {
        switch (c) {
            case 'c':
                cerr << "Compression flag is obselete. As of July 2008, support for uncompressed bitmap indexes has been terminated." << endl;
                compression = true;
                break;
            case 's':
                sparsemode = true;
                break;
            case 'p':
                projection = true;
                if (strcmp(optarg, "uncompressed") == 0)
                    projectiontype="uncompressed";
                else if (strcmp(optarg, "8") == 0)
                	projectiontype="8";
                else if (strcmp(optarg, "16") == 0)
                	projectiontype="16";
                else if (strcmp(optarg, "32") == 0)
                	projectiontype="32";
                else if (strcmp(optarg, "Kaser32") == 0)
                	projectiontype="Kaser32";
                else if (strcmp(optarg, "Kaser64") == 0)
                	projectiontype="Kaser64";
                else cerr << "Unrecognized projection index type: " << optarg << " try 'uncompressed', '8', '16' or '32'"<<endl;
                break;
                break;
            case 'i':
                interval = true;
                if (strcmp(optarg, "sqrt") == 0)
                    intervaltype="sqrt";
                else if (strcmp(optarg, "interval") == 0)
                	intervaltype="interval";
                else if (strcmp(optarg, "range") == 0)
                	intervaltype="range";
                else cerr << "Unrecognized interval indexing strategy: " << optarg << " try 'range'"<<endl;
                break;
            case 'k':
                k = strtol(optarg, NULL, 10);
                break;
            case 'j':
                justhisto = true;
                break;
            case 'B':
                bits = strtol(optarg, NULL, 10);
                break;
            case 'K':
                k = strtol(optarg, NULL, 10);
                forcek = true;
                break;
                /*case 'e':
                extrabits = strtol(optarg);
                aba.setExtraBitmaps(extrabits);
                break;*/
            case 'a':
                if (strcmp(optarg, "alpha") == 0)
                    ba = &aba;
                else if (strcmp(optarg, "weight") == 0)
                    ba = &wsba;
                else if (strcmp(optarg, "gray") == 0)
                    ba = &gcba;
                else if (strcmp(optarg, "grayall") == 0)
                    ba = &gacba;
                else if (strcmp(optarg, "weightgray") == 0)
                    ba = &wsgcba;
                else if (strcmp(optarg, "weightgrayall") == 0)
                    ba = &wsgacba;
                else if (strcmp(optarg, "grayreverse") == 0)
                    ba = &gcbar;
                else if (strcmp(optarg, "grayallreverse") == 0)
                    ba = &gacbar;
                else if (strcmp(optarg, "weightgrayreverse") == 0)
                    ba = &wsgcbar;
                else if (strcmp(optarg, "weightgrayallreverse") == 0)
                    ba = &wsgacbar;
                else if (strcmp(optarg, "weightgrayallreversereverse") == 0)
                    ba = &wsgacbarr;
                else cerr << "Unrecognized bitmap allocation strategy: " << optarg << endl;
                break;
            case 'v':
                verbose = true;
                break;
            case 'd':
                delimiter = string(optarg);
                if (verbose == true) cout << "delimiter is " << delimiter << endl;
                break;
            case 'b':
                BlockSize = strtoll(optarg, NULL, 10);
                if (verbose == true) cout << "block size is now " << BlockSize << endl;
                break;
                /* This was a mistake...now use a separate conversion
                   program (cvt_index.rb) on the (uncompressed) index file...
              case 'p':
                fname = string(optarg);
                printing = true;
                if (verbose==true) cout << " dump file name is " << fname << endl;
                break;
                 */
            default:
                abort();
        }
    }
    /*if((extrabits>0) && (ba == &aba)) {
        if (verbose==true) cout <<"using "<<extrabits<< " extra bitmaps"<<endl;
    }*/
    if ((!justhisto and(argc - optind < 2)) or(justhisto and(argc - optind < 1))) {
        cout << "You must provide two parameters, a CSV file and the name of an output " << endl;
        cout << "index file, " << endl;
        //cout <<"you may prefix these two by the -c flag for compression."<<endl;
        cout << "you may prefix these two by the -v flag for verbose mode." << endl;
        cout << "to construct a projection index instead of a bitmap index, use the -p flag" << endl;
        cout << "Example: ./indexcsv  testdata.csv testindex.bin  (compression and unary encoding)" << endl;
        cout << "Example: ./indexcsv  -k 2 testdata.csv testindex.bin (compression and 2-of-N encoding)" << endl;
        cout << "Example: ./indexcsv  -k 2 -d \"|\" testdata.csv testindex.bin (compression and 2-of-N encoding, caracter delimiter is |)" << endl;
        cout << "Example: ./indexcsv  -k 3 -a testdata.csv testindex.bin (compression, adaptive and 3-of-N encoding)" << endl;
        cout << "Example: ./indexcsv -v  -k 3 -a testdata.csv testindex.bin (verbose mode, compression, adaptive and 3-of-N encoding)" << endl;
        cout << " For sparse bitmap indexes, try using the '-s' (sparse) flag." << endl;
        cout << " You can also specify the data block size with the -b flag." << endl;
        cout << " Replace the k flag by the K flag to force k-of-N encoding even when it is inefficient." << endl;
        cout << " by default, we have -b " << static_cast<uint>(defaultUncompressedBlockSize) << "." << endl;
        cout << "You also have an 'a' flag that can be used as -a alpha, -a gray, -a grayall, -a weight, -a weightgray to control how the bitmaps are allocated. It is probably only useful with the -k flag for values 2,3,4, the gray option should work well over sorted CSV files." << endl;
        cout << "The j flag will just materialize the histogram (without creating the index)." << endl;
        cout << "To switch to 16-bit or 64-bit words, use -B 16 or -B 64."<<endl;
        return -1;
    }
    if (verbose == true) cout << "k = " << k << endl;
    /*if(( k == 1) and (ba == &aba)) {
        cerr <<"[SEVERE WARNING] it makes no sense to turn on adaptive encoding and to leave k=1."<<endl;
    }*/
    /*if(( k > 4 ) and (ba == &aba)) {
        cerr <<"[SEVERE WARNING] adaptive encoding may not work for k>3."<<endl;
    }*/
    if (!compression && sparsemode) {
        cerr << "ignore the sparse flag. Try turning on compression (-c)." << endl;
    }
    csvfilename = argv[optind];
    if (!justhisto) indexfile = argv[optind + 1];
    struct stat csv_stat;
    if (stat(csvfilename.c_str(), &csv_stat) != 0) {
        cerr << "csv file " << csvfilename << " was not found" << endl;
        return -1;
    } else if (verbose == true) cout << "csv file size = " << (csv_stat.st_size / 1000000.0) << " MB" << endl;
    if (verbose == true) cout << "Please wait while I index CSV file " << csvfilename << endl;
    try {
        start = clock();
        if (justhisto) {
            materializeHistogram(csvfilename, delimiter);
            finish = clock();
            if (verbose == true) {
                cout << "the bitmap index file is in  " << indexfile << endl;
                cout << "took " << static_cast<double> (finish - start) / CLOCKS_PER_SEC << " seconds  " << endl;
            } else cout << static_cast<double> (finish - start) / CLOCKS_PER_SEC << " ";
        } else {
            if(interval)
                if (bits == 16)
                	if (sparsemode)
                		createIntervalIndex<SparseCompressedDataBlock<uword16> >(indexfile,BlockSize,csvfilename,delimiter,intervaltype);
                	else
                		createIntervalIndex<CompressedDataBlock<uword16> >(indexfile,BlockSize,csvfilename,delimiter,intervaltype);
                else if (bits == 32)
                	if (sparsemode)
                	createIntervalIndex<SparseCompressedDataBlock<uword32> >(indexfile,BlockSize,csvfilename,delimiter,intervaltype);
                	else
                		createIntervalIndex<CompressedDataBlock<uword32> >(indexfile,BlockSize,csvfilename,delimiter,intervaltype);
                else if (bits == 64)
                	if (sparsemode)
                    createIntervalIndex<SparseCompressedDataBlock<uword64> >(indexfile,BlockSize,csvfilename,delimiter,intervaltype);
                	else
                		createIntervalIndex<CompressedDataBlock<uword64> >(indexfile,BlockSize,csvfilename,delimiter,intervaltype);
                else
                    cerr << "unrecognized number of bits : " << bits << endl;

            else if(projection)
                createProjectionIndex(indexfile,BlockSize,csvfilename,delimiter,projectiontype);
            else if (bits == 16)
                if (sparsemode)
                    createIndex<SparseCompressedDataBlock<uword16> >(indexfile, verbose, BlockSize, csvfilename, k, forcek, *ba, delimiter);
                else
                    createIndex<CompressedDataBlock<uword16> >(indexfile, verbose, BlockSize, csvfilename, k, forcek, *ba, delimiter);
            else if (bits == 32)
                if (sparsemode)
                    createIndex<SparseCompressedDataBlock<uword32> >(indexfile, verbose, BlockSize, csvfilename, k, forcek, *ba, delimiter);
                else
                    createIndex<CompressedDataBlock<uword32> >(indexfile, verbose, BlockSize, csvfilename, k, forcek, *ba, delimiter);
            else if (bits == 64)
                if (sparsemode)
                    createIndex<SparseCompressedDataBlock<uword64> >(indexfile, verbose, BlockSize, csvfilename, k, forcek, *ba, delimiter);
                else
                    createIndex<CompressedDataBlock<uword64> >(indexfile, verbose, BlockSize, csvfilename, k, forcek, *ba, delimiter);
            else
                cerr << "unrecognized number of bits : " << bits << endl;
            finish = clock();
            if (verbose == true) {
                cout << "the bitmap index file is in  " << indexfile << endl;
                cout << "took " << static_cast<double> (finish - start) / CLOCKS_PER_SEC << " seconds  " << endl;
            } else cout << static_cast<double> (finish - start) / CLOCKS_PER_SEC << " ";
        }
        struct stat my_stat;
        my_stat.st_size = 0;
        if (!justhisto and(stat(indexfile.c_str(), &my_stat) != 0))
            cerr << "index file " << indexfile << " was not created" << endl;
        else {
            if (verbose == true) cout << "index  file size = " << (my_stat.st_size / 1000000.0) << " MB" << endl;
            else cout << (my_stat.st_size / 1000000.0) << endl;
        }
        return 0;
    } catch (problemtoolarge_exception pte) {
        cerr << " You have activated adaptive bitmap indexes, but the problem space is too large, increase k or set it equal to 2" << endl;
        return -1;
    }
}
