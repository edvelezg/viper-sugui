#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include "BitmapIndex.h"

using namespace std;

/**
 * to dump a lot of possibly verbose data about a bitmap index.
 */

typedef unsigned long long int uint64;

template <class uword>
void displayBitmapStats( const BitmapIndex & bi) {
        ifstream in(bi.mFilename.c_str());
        in.seekg(bi.locationoffirstunreadblock);
	BlockIterator<uword> bli (in);
	vector<uint> bitmapsizes;
	while(bli.hasNext()) {
		const ReadCompressedDataBlock<uword> & n = bli.next();
		const map<uint,EWAHBoolArray<uword> > & b =  n.getBuffer();
		bitmapsizes.resize(b.size(),0);
		for(uint k = 0; k< b.size();++k) {
			//if(dirty) cout<<"bitmap #"<<k << " of datablock #"<<db<<endl;
			typename map<uint,EWAHBoolArray<uword> >::const_iterator cur  = b.find(k);
			if(cur == b.end()) {
				cerr << "Can't find bitmap "<< k << endl;
				continue;
			}
			const EWAHBoolArray<uword> & ba = cur->second;
			BitmapStatistics p = ba.computeStatistics();
			bitmapsizes[k]+=p.getCompressedSize();
		}
	}
	for(uint k = 0; k <bitmapsizes.size();++k)
		cout<<bitmapsizes[k]<<endl;
}

template <class uword>
void analyzeBitmap(const BitmapIndex & bi, bool allocation, bool dirty, bool transitions) {
	//const vector<multimap<string,uint> > & vmsu = bi.getStringToBitmap();
	if(allocation) cout <<"Dumping the map from attribute values to bitmap numbers"<<endl;
	for(uint k=0;k<bi.avm.size();++k) {
		if(allocation) cout <<"dimension #"<<k<<endl;
		bi.avm.dumpMap(k);
		//cout <<"dimension #"<<k<<" has "<<dimensionset.size()<< " attribute values, using "<< bitmapset.size()<< " bitmaps"<<endl;
	}
	if(allocation) cout<<endl<<" Next I am going to tell about how the bitmaps are compressed."<<endl;
        ifstream in(bi.mFilename.c_str());
        in.seekg(bi.locationoffirstunreadblock);
	BlockIterator<uword> bli (in);
	int db = 0;
	uint64 overalldirty(0), overallclean(0), overallruns(0);
        vector<int> prev_values(0);
	while(bli.hasNext()) {
		if(dirty) cout<<endl<<endl << "=========datablock #"<<++db<<endl;
		const ReadCompressedDataBlock<uword> & n = bli.next();
		const map<uint,EWAHBoolArray<uword> > & b =  n.getBuffer();
		if (prev_values.size() == 0)
			for (uint ii=0; ii < b.size(); ++ii) prev_values.push_back(-1);
		for(uint k = 0; k< b.size();++k) {
			if(dirty) cout<<"bitmap #"<<k << " of datablock #"<<db<<endl;
			typename map<uint,EWAHBoolArray<uword> >::const_iterator cur  = b.find(k);
			if(cur == b.end()) {
				cerr << "Can't find bitmap "<< k << endl;
				continue;
			}
			const EWAHBoolArray<uword> & ba = cur->second;
			BitmapStatistics p = ba.computeStatistics();
                        if (transitions) {
                          int prevBit = prev_values[k];
                          EWAHBoolArrayIterator<uword> it = ba.uncompress();
                          // assume the least significant bit corresp to first row
                          // this needs to be verified.
                          while (it.hasNext()) {
                            uword w = it.next();
                            int bitsInWord = sizeof(uword)*8;
                            for (int bitpos =0; bitpos < bitsInWord; ++bitpos) {
                              int thisBit = w & 0x1;
                              // cout << ctr++ << " " << thisBit << endl; // temp temp
                              if( prevBit == -1) prevBit = thisBit;
                              if (thisBit != prevBit) {
                                ++overallruns;
                                prevBit = thisBit;
                              }
                              w >>= 1;
                            }
                          }
                          prev_values[k] = prevBit;  // set up for next block
                        }
			if(dirty) cout << " dirty words = "<<p.getNumberOfDirtyWords()<<" compressed (all 1s or all 0s) words = "<<p.getNumberOfCleanWords()<< " clean words after compression = "<<p.getNumberOfMarkers() <<" bitmap compressed size = " << p.getCompressedSize()  << " bitmap uncompressed size = " << p.getUncompressedSize() << endl;
			overalldirty += p.getNumberOfDirtyWords();
			overallclean += p.getNumberOfCleanWords();
		}
	}
	cout <<"total number of dirty words: "<<overalldirty<< ", total number of clean words: "<<overallclean<<endl;
        if (transitions) cout << "total number of transitions " << overallruns << endl;
	if(dirty or allocation) cout <<endl<<"If you didn't get all of the output, try to pipe it to a file!"<<endl;
}


void displayhelp() {
	cout <<" the a flag displays the allocation of the bitmaps"<<endl;
	cout <<" the d flag displays the number of dirty words in a verbose way"<<endl;
}

int main(int argc, char ** argv) {
	std::ios::sync_with_stdio (false);
	bool allocation (false), dirty(false);
        bool transitions(false);
        bool bitmapdisplay(false);
	int c;
    while ((c = getopt (argc, argv, "hdatb")) != -1)
      switch (c)
        {
        case 'h':
          displayhelp();
          break;
        case 'a':
          allocation=true;
          break;
        case 'd':
          dirty=true;
          break;
        case 't':
          transitions=true;
          break;
        case 'b' :
          bitmapdisplay = true;
          break;
        default:
          abort ();
        }
    if(argc - optind < 1 ){
		cout << "You must give me an index file"<<endl;
		return -1;
    }
	string indexfile(argv[optind]);
	/*if(sizeof(ulong)>4)
		cout <<"you have a 64-bit machine, expecting a 64-bit index..."<<endl;
	else
		cout <<"you have a 32-bit machine, expecting a 32-bit index..."<<endl;*/
	try {
          const BitmapIndex bi(indexfile);

          if(bitmapdisplay) {
        	  if(bi.getNumberOfBits() == 16)
                      displayBitmapStats<uword16>(bi);
                  else if(bi.getNumberOfBits() == 32)
                      displayBitmapStats<uword32>(bi);
                  else if (bi.getNumberOfBits() == 64)
                      displayBitmapStats<uword64>(bi);
                  else cerr <<"don't know about "<<bi.getNumberOfBits()<<"-bit words"<<endl;
          }    else {
        	  if(bi.getNumberOfBits() == 16)
                      analyzeBitmap<uword16>(bi, allocation, dirty, transitions);
                  else if(bi.getNumberOfBits() == 32)
                      analyzeBitmap<uword32>(bi, allocation, dirty, transitions);
                  else if (bi.getNumberOfBits() == 64)
                      analyzeBitmap<uword64>(bi, allocation, dirty, transitions);
                  else cerr <<"don't know about "<<bi.getNumberOfBits()<<"-bit words"<<endl;
          }
	} catch(badcompressiontype_exception be) {
		  cout <<"only works with compressed index for now"<<endl;
	} catch(cookiemismatch_exception ce) {
		cout << " a cookie mismatch means you probably do not have an index file."<<endl;
	} catch(versionmismatch_exception ve) {
		cout << " a version mismatch means you must regenerate the index file."<<endl;
	}
}
