#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include "BitmapIndex.h"

using namespace std;


int main(int argc, char ** argv) {
	if(argc != 2) {
		cout <<" You must provide  an index file"<<endl;
		return -1;
	}
	string indexfile(argv[1]);
	const BitmapIndex bi(indexfile);
	//const vector<stringuintmultimap > & v = bi.getStringToBitmap();
	for(uint d = 0; d< bi.avm.size();++d) {
		bi.avm.dumpMap(d);/*
		const stringuintmultimap & m = v[d];
		for(stringuintmultimap::const_iterator i = m.begin(); i!= m.end(); ++i) {
			const string & s = i->first;
			uint c = m.count(s);
			//cout<<s<<" count " << c<<endl;
			cout << i->first<<" ";
			vector<uint> pos;
			for(uint k = 0; k < c - 1;++k) {
				//if (i->first != s) 
				//cerr<<i->first<<" bug "<<s<<endl;
				pos.push_back((i++)->second);
			}
			pos.push_back(i->second);
			sort(pos.begin(),pos.end());
			for(uint k = 0; k < c;++k)
				cout <<pos[k]<<" ";
			cout << endl;
		}
		cout << endl;*/
	}
}


 
