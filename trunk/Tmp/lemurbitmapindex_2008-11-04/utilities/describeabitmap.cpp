#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include "BitmapIndex.h"

using namespace std;

template <class uword>
int describe(const BitmapIndex & bi, uint bitmapnumber) {
        ifstream in(bi.mFilename.c_str());
        in.seekg(bi.locationoffirstunreadblock);
    	BlockIterator<uword> bli (in);
	while(bli.hasNext()) {
		const ReadCompressedDataBlock<uword> & n = bli.next();
                typedef EWAHBoolArray<uword> ew; 
		const map<uint,ew > & b =  n.getBuffer();
		if(b.find(bitmapnumber) == b.end()) {
			cerr << "can't find this bitmap: "<<bitmapnumber<<endl;
			return -2;
		}
		
		EWAHBoolArrayRawIterator<uword> i (b.find(bitmapnumber)->second);
		while(i.hasNext()) {
			i.next();
			cout <<" Clean word ("<<i.rlw.getRunningBit() <<") repeated "<< i.rlw.getRunningLength()<< " times followed by "<< i.rlw.getNumberOfLiteralWords()<< " dirty words"<<endl; 
			for(uint j = 0; j<i.rlw.getNumberOfLiteralWords();++j) {
				const uword & w = i.dirtyWords()[j];
				for(uint k = 0; k <sizeof(uword)*8;++k) {
					if(w & (1<<k)) cout <<"1"; else cout << "0";
				}
				cout <<endl;
			}
		}
		if(bli.hasNext()) cout <<"==end of data block=="<<endl; 
	}
        return 0;

}
/**
 * This utility describes in a readable manner a single bitmap out of
 * an index.
 */
int main(int argc, char ** argv) {
cout<<"As of June 18th 2008, this program may output garbage (under Linux?)."<<endl;
if(argc != 3) {
		cout <<" You must provide the number of an index file, and a bitmap number from 0 to L-1"<<endl;
		return -1;
	}
	string indexfile(argv[1]);
	uint bitmapnumber = strtoll(argv[2],NULL,10);
	const BitmapIndex bi(indexfile);
        if(bi.getNumberOfBits() == 16)
            return describe<uword16>(bi,bitmapnumber);
        else if(bi.getNumberOfBits() == 32)
            return describe<uword32>(bi,bitmapnumber);
        else if (bi.getNumberOfBits() == 64)
            return describe<uword64>(bi,bitmapnumber);
        else cerr<<"Don't know about "<<bi.getNumberOfBits()<<"-bit words"<<endl;
}
