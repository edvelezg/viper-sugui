#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include "BitmapIndex.h"

/**
 * Returns the total number of dirty words **per dimension** in a bitmap index
 * along with other data.
 */

typedef unsigned long long int uint64;

using namespace std;

template <class uword>
void printDirtyWordsPerDimension(const BitmapIndex & bi) {
	//const vector<multimap<string,uint> > & vmsu = bi.getStringToBitmap();
	//map<uint,uint> frombittodimension;
	//vector<uint> maxindexbitmap(bi.avm.size(),0);
	vector<uint> cardinalityperdimension(bi.avm.size(),0);
	for(uint k=0;k<bi.avm.size();++k) {
		cardinalityperdimension[k] = bi.avm.getNumberOfAttributeValues(k);
	}
	vector<uint64> dirtyperdimension(bi.avm.size(),0);
	vector<uint64> cleanperdimension(bi.avm.size(),0);
	vector<uint64> compressedsizeperdimension(bi.avm.size(),0);
	vector<uint64> uncompressedsizeperdimension(bi.avm.size(),0);
	vector<uint64> bitmapsperdimension(bi.avm.size(),0);
	vector<uint64> markersperdimension(bi.avm.size(),0);
        ifstream in(bi.mFilename.c_str());
        in.seekg(bi.locationoffirstunreadblock);
	BlockIterator<uword> bli (in);
	bool firstblock = true;
	while(bli.hasNext()) {
		const ReadCompressedDataBlock<uword> & n = bli.next();
		const map<uint,EWAHBoolArray<uword> > & b =  n.getBuffer();
		uint d  = 0;
                pair<uint,uint> currentdimrange = bi.avm.getRangeOfBitmapIndexes(d);
		for(uint k = 0; k< b.size();++k) {//
			typename map<uint,EWAHBoolArray<uword> >::const_iterator cur  = b.find(k);
			if(cur == b.end()) {
				cerr << "Can't find bitmap "<< k << endl;
				continue;
			}
			const EWAHBoolArray<uword> & ba = cur->second;
			//uint d = frombittodimension[k];
			//uint d = 0;
			//while(maxindexbitmap[d]<k )++d;
                        while(k>currentdimrange.second) {
                            ++d;
                            currentdimrange = bi.avm.getRangeOfBitmapIndexes(d);
                        }
			//for(uint k = 0)
			assert(d < bi.avm.size());
			BitmapStatistics p = ba.computeStatistics();
			//cout<<p.getNumberOfDirtyWords()<<" "<<p.getNumberOfCleanWords()<<endl;
			dirtyperdimension[d] += p.getNumberOfDirtyWords();
			cleanperdimension[d] += p.getNumberOfCleanWords();
			compressedsizeperdimension[d] += p.getCompressedSize();
			uncompressedsizeperdimension[d] += p.getUncompressedSize();
			markersperdimension[d] += p.getNumberOfMarkers();
			if(firstblock) ++bitmapsperdimension[d];
		}
		firstblock = false;
	}
	cerr <<"# dirtywords cleanwords distinctnumberofattributevalues numberofbitmapsonthisdimension compressedsizeinwords uncompressedsizeinwords cleanwordscompressed"<<endl;
	for(uint k =0; k<bi.avm.size();++k) {
		cout <<dirtyperdimension[k]<< " "<<cleanperdimension[k]<< " "<< cardinalityperdimension[k]
		<< " "<< bitmapsperdimension[k]
		<< " "<< compressedsizeperdimension[k]
		<< " "<< uncompressedsizeperdimension[k]
		<< " "<< markersperdimension[k]
		<<endl;
	}
}

int main(int params, char ** args) {
	std::ios::sync_with_stdio (false);
	if (params <= 1) {
		cout << "You must give me an index file"<<endl;
		return -1;
	}
	string indexfile(args[1]);
        const BitmapIndex bi(indexfile);
	try {
            if(bi.getNumberOfBits() == 16)
                printDirtyWordsPerDimension<uword16>(bi);
            else if(bi.getNumberOfBits() == 32)
                printDirtyWordsPerDimension<uword32>(bi);
            else if(bi.getNumberOfBits() == 64)
                printDirtyWordsPerDimension<uword64>(bi);
            else cerr <<"don't know about "<<bi.getNumberOfBits()<<"-bit words"<<endl;
	} catch(badcompressiontype_exception be) {
		  cout <<"only works with compressed index for now"<<endl;
	} catch(cookiemismatch_exception ce) {
		cout << " a cookie mismatch means you probably do not have an index file."<<endl;  
	} catch(versionmismatch_exception ve) {
		cout << " a version mismatch means you must regenerate the index file."<<endl;
	}


}
