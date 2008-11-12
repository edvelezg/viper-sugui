#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include "BitmapIndex.h"

/**
 * Returns the total number of dirty words in a bitmap index.
 */

typedef unsigned long long int uint64;

using namespace std;

template <class uword>
void printDirtyWords(const BitmapIndex& bi) {
        ifstream in(bi.mFilename.c_str());
        in.seekg(bi.locationoffirstunreadblock);
	BlockIterator<uword> bli (in);
	uint64 count = 0;
	while(bli.hasNext()) {
		const ReadCompressedDataBlock<uword> & n = bli.next();
		const map<uint,EWAHBoolArray<uword> > & b =  n.getBuffer();
		for(uint k = 0; k< b.size();++k) {
			typename map<uint,EWAHBoolArray<uword> >::const_iterator cur  = b.find(k);
			if(cur == b.end()) {
				cerr << "Can't find bitmap "<< k << endl;
			} else {
				BitmapStatistics p = cur->second.computeStatistics();
				count += p.getNumberOfDirtyWords();
			}
		}
	}
	cout <<count <<endl;
}

int main(int params, char ** args) {
	std::ios::sync_with_stdio (false);
	if (params <= 1) {
		cout << "You must give me an index file"<<endl;
		return -1;
	}
	string indexfile(args[1]);
	try {
            const BitmapIndex bi(indexfile);
            if(bi.getNumberOfBits() == 16)
                printDirtyWords<uword16>(bi);
            else if (bi.getNumberOfBits() == 32)
                printDirtyWords<uword32>(bi);
            else if (bi.getNumberOfBits() == 64)
                printDirtyWords<uword64>(bi);
            else cerr << "Don't know what about "<<bi.getNumberOfBits()<<"-bit words"<<endl;
	} catch(badcompressiontype_exception be) {
		  cout <<"only works with compressed index for now"<<endl;
	} catch(cookiemismatch_exception ce) {
		cout << " a cookie mismatch means you probably do not have an index file."<<endl;  
	} catch(versionmismatch_exception ve) {
		cout << " a version mismatch means you must regenerate the index file."<<endl;
	}


}
