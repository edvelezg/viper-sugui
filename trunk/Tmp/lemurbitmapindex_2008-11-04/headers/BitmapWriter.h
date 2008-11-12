#ifndef BITMAPWRITER_H_
#define BITMAPWRITER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <map>
#include "CSVParser.h"
#include "DataBlock.h"
#include "Util.h"
#include "BitmapAllocation.h"
#include "AttributeValueMap.h"

using namespace std;

class HistogramLoader {
public:
    static string getHistoFilename(string csvfilename);
    static void loadHistogram(const string filename, histogramtype & mHistograms, const string delimiter = ",", const char commentmarker = '#', const bool verbose = false);
};

template <class DB>
class BitmapWriter {
public:
    /**
     * codingK is for supporting k-of-N encoding.
     */
    BitmapWriter(const string & indexfile, uint uncompressedblocksize = defaultUncompressedBlockSize, KofNBitmapBudget& bb = g1ofNBitmapBudget, BitmapAllocation & ba = gStandardBitmapAllocation);

    void indexCSVFile(string filename, bool verbose = false, string delimiter = ",", char commentmarker = '#');


    virtual ~BitmapWriter();

    enum {
        version = 2, cookie = 0xAAAA, verboseindexing = false
    };

    /*histogramtype& getHistogram() {
            return mHistograms;
    }*/

    void setBitmapBudget(KofNBitmapBudget & bb) {
        mBB = & bb;
    }


private:
    BitmapWriter(const BitmapWriter & x);
    BitmapWriter & operator=(const BitmapWriter & x);
    
    //histogramtype mHistograms;
    void writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap);
    /*void writeIndex(CSVParser& csv) {
            StandardBitmapAllocation sba;
            writeIndex(csv,sba);
    }*/
    void writeIndex(string filename, bool verbose, string delimiter = ",", char commentmarker = '#'); //, BitmapAllocation & ba
    string mIndexFile;
    uint blocksize;
    //int mCodingK;
    KofNBitmapBudget * mBB;
    BitmapAllocation & mBA;
};

template <class DB>
BitmapWriter<DB>::BitmapWriter(const string & indexfile, uint uncompressedblocksize, KofNBitmapBudget& bb, BitmapAllocation & ba) :
mIndexFile(indexfile), blocksize(uncompressedblocksize), mBB(&bb), mBA(ba) {
    std::ios::sync_with_stdio(false);

}

template <class DB>
BitmapWriter<DB>::~BitmapWriter() {
}

template <class DB>
void BitmapWriter<DB>::indexCSVFile(string filename, bool verbose, string delimiter, char commentmarker) {
    writeIndex(filename, verbose, delimiter, commentmarker);
}

template <class DB>
void BitmapWriter<DB>::writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap) {
    const uint mycookie = cookie;
    out.write(reinterpret_cast<const char *> (&mycookie), sizeof (mycookie));
    const uint currentversion = version;
    // we always start out with the file format version number
    out.write(reinterpret_cast<const char *> (& currentversion), sizeof (currentversion));
    const uint compressionmode = DB::compressiontype;
    out.write(reinterpret_cast<const char *> (& compressionmode), sizeof (compressionmode));
    const uint nb = DB::getNumberOfBits();
    out.write(reinterpret_cast<const char *> (& nb), sizeof (nb));
}

template <class DB>
void BitmapWriter<DB>::writeIndex(string filename, bool verbose, string delimiter, char commentmarker) {
    //BitmapAllocation &  ba  = mBA;

    ofstream out(mIndexFile.c_str(), ios::out | ios::binary);
    if (!out) {
        cerr << "I can't open the index file \"" << mIndexFile << "\", expect trouble." << endl;
        throw cantindexfiletowrite_exception("can't open bitmap file to write to");
    }
    //assert(out.tellp()==0);
    //vector<uint> Ns = StandardBitmapAllocation::numbersOfBitmaps(mCodingK,mHistograms,false);
    histogramtype mHistograms;
    HistogramLoader::loadHistogram(filename, mHistograms, delimiter, commentmarker, verbose);
    vector<stringuintmultimap > tobitmap;
    mBA.computeBitmapAllocation(*mBB, mHistograms, tobitmap);
    mHistograms.clear();
    ostream::pos_type beforeheader = out.tellp();
    writeHeader(out, tobitmap);
    AttributeValueMap::writeToDisk(tobitmap, mIndexFile);
    ostream::pos_type afterheader = out.tellp();
    if (verboseindexing) cout << "header written" << endl;
    if (!out) {
        cerr << " I did not manage to write the header of the bitmap index in \"" << mIndexFile << "\". Aborting. " << endl;
        throw cantwritebitmapindexheader_exception("bad ostream after header written");
    }
    if (verbose) cout << "size of the bitmap header = " << (afterheader - beforeheader) << " bytes" << endl;
    // first, we have to decide where each attribute value goes
    // this is easy because we do something straight forward, without compression
    // next, we process the data and write it out
    uint numberofbitmaps = BitmapAllocation::numberOfBitmaps(tobitmap); //0;
    DB db(numberofbitmaps, blocksize);
    if (verboseindexing) cout << "DB created" << endl;
    ifstream in2(filename.c_str());
    if (!in2) {
        cerr << " could not reopen CSV file:" << filename << endl;
        throw cantopencsv_exception("could not reopen");
    }
    CSVParser csv(in2, delimiter, commentmarker);
    while (csv.hasNext()) {
        //cout << "inside loop"<<endl;
        if (db.full()) {
            db.emptyToDisk(out, verbose);
            if (!out) {
                cerr << " I did not manage to write one of the datablocks in  \"" << mIndexFile << "\". Aborting. " << endl;
                throw couldnotwritedatablock_exception("bad ostream after datablock written");
            }
        }
        //csv.next();
        const vector <string> & currentdata = csv.next();
        if (tobitmap.size() != currentdata.size()) continue;
        db.newRow();
        if (currentdata.size() != tobitmap.size()) {
            cerr << "This histogram is corrupted, size mismatch. Try deleting the .histo file." << endl;
            in2.close();
            out.close();
            throw corruptedhistogram_exception("size mismatch");
        }
        for (uint k = 0; k < currentdata.size(); ++k) {
            //multimap<string,uint>::iterator p;
            stringuintmultimap::const_iterator p = tobitmap[k].find(currentdata[k]);
            const uint nbr = tobitmap[k].count(currentdata[k]);
            if (nbr == 0) {
                cerr << "This histogram is corrupted, can't find value " << currentdata[k] << ". Try deleting the .histo file." << endl;
                in2.close();
                out.close();
                throw corruptedhistogram_exception(currentdata[k]);
            }
            for (uint j = 0; j < nbr; ++j) {
                db.set(p->second);
                //cout << ""
                ++p;
            }
        }
        //++nbrrows;
    }
    in2.close();
    if (db.getNumberOfRows() == 0)
        cerr << "I didn't find any row to index!" << endl;
    db.emptyToDisk(out, verbose);
    if (!out) {
        cerr << " I did not manage to write the last datablocks in  \"" << mIndexFile << "\". Aborting. " << endl;
        throw couldnotwritedatablock_exception("bad ostream after last datablock written");
    }
    //cout << "wrote "<<out.tellp()<< "bytes"<<endl;
    out.close();
}

#endif /*BITMAPWRITER_H_*/
