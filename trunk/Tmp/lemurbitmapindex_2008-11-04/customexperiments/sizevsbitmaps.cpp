/*
 * One fundamental issue once you allow
 * non-unary encodings and hybrid encodings is
 * that you must pick the right number of bitmap
 * indexes.
 * 
 * If you have too few bitmap indexes, then you 
 * will have a very dense (lots of zero) bitmap
 * index and it will compress poorly. If it
 * is too large, it will be too sparse and, again,
 * it will compress poorly.
 * 
 * There is a sweet spot in the density of the
 * bitmap index and this piece of code
 * is meant to find it.
 * 
 * author: Daniel Lemire
 * date: started on Oct. 4th 2007
 */
#include <cstdlib>
#include <ctype.h>
#include <sys/stat.h>
#include <fstream>
#include "BitmapWriter.h"
#include "BitmapIndex.h"
#include "CompressedDataBlock.h"
#include "CSVParser.h"
using namespace std;

void displayhelp() {
	cout << "please specify a CSV file."<<endl;
}

/*
void classicbenchmark(string csvfilename) {
	string indexfile(csvfilename);
	indexfile.insert(indexfile.size(), ".temporarybinfilecassical");
	BitmapWriter<CompressedDataBlock> bw(indexfile,defaultUncompressedBlockSize,g1ofNBitmapBudget);
	//bw.readHistogram(csvfilename);
	::unlink(indexfile.c_str());
	for(uint k = 1; k<5;++k) {
		KofNBitmapBudget g2ofkBitmapBudget(k);
		clock_t start,finish;
		start = clock();
		bw.setBitmapBudget(g2ofkBitmapBudget);
		bw.indexCSVFile(csvfilename);
		finish = clock();
		struct stat my_stat;
		if(stat(indexfile.c_str(), &my_stat) != 0) { 
			cerr <<"index file "<<indexfile<<" was not created"<<endl;
		}
		cout<<"# k= "<<k <<" "<< (my_stat.st_size/1000000.0)<<", "<<static_cast<double>(finish - start) / CLOCKS_PER_SEC<<endl;
		::unlink(indexfile.c_str());
	}
}*/

/*
void benchmark(string csvfilename) {
	// first, we figure out the geometry of the 
	// CSV file
	// computing the histogram:
	string indexfile(csvfilename);
	indexfile.insert(indexfile.size(), ".temporarybinfile");
	BitmapWriter<CompressedDataBlock> bw(indexfile,defaultUncompressedBlockSize,g1ofNBitmapBudget, gAdaptiveBitmapAllocation );
	//bw.readHistogram(csvfilename);
	vector<map<string,uint> > histogram ;//= bw.getHistogram();
	{ifstream in(csvfilename.c_str());
	if (!in) {
		cerr << " could not open CSV file:"<< csvfilename <<endl;
	}
	CSVParser csvhisto(in);
	csvhisto.readHistogram(histogram);
	in.close();
	}
	if(histogram.size() != 1) {
		cout <<"For now, this code assumes that your CSV has only one column."<<endl;
		cout <<"Use ./cvsextractor -i if needed"<<endl;
		return;
	}
	map<string,uint> columnhisto = histogram[0];
	uint n = columnhisto.size();
	uint maxL= n;
	uint minL = KofNBitmapBudget::numBitsRequired(n, 3);
	cout << "# I will vary the number of bitmaps between "<<minL<<" and "<<maxL<<endl;
	uint increment = 1;
	//if(maxL-minL > 20)
	//	increment = 10;
	cout <<"# output format is  L, size(MB), time (s)"<<endl;
	for(uint L = minL; L<=maxL;L+=increment) {
		if((L-minL)% 20 == 0) cerr<<"done "<<(L-minL)<<endl;
		clock_t start,finish;
		start = clock();
		vector<uint> budgets(1,L);
		PredefinedBitmapBudget pbb(budgets);
		bw.setBitmapBudget(pbb);
		bw.indexCSVFile(csvfilename);
		finish = clock();
		struct stat my_stat;
		if(stat(indexfile.c_str(), &my_stat) != 0) { 
			cerr <<"index file "<<indexfile<<" was not created"<<endl;
		}
		cout<<L <<", "<< (my_stat.st_size/1000000.0)<<", "<<static_cast<double>(finish - start) / CLOCKS_PER_SEC<<endl;
		::unlink(indexfile.c_str());
	}
}
*/

int main(int argc, char **argv) {
	if (argc <= 1) {
		displayhelp();
		return -1;
	}
	string inputfile(argv[1]);
    if(inputfile.size()==0) {
    	displayhelp();
    	return -1;
    }
    cout << "# using CSV file "<<inputfile<<endl;
    cout << "# I am hoping that your CSV file has a single column. "<<endl;
  //  classicbenchmark(inputfile); 
  //  benchmark(inputfile);
}

