#include <fstream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctype.h>
#include <sys/stat.h>
#include "CSVParser.h"
using namespace std;

/** 
 * this can be used to remove or extract a single column from a CSV file.
 */

int main(int argc, char **argv) {
	std::ios::sync_with_stdio (false);
	set<int> omittedcolumn;
	vector<int> includedcolumn;
	string delimiter(",");
	int c;
    while ((c = getopt (argc, argv, "o:i:d:")) != -1)
      switch (c)
        {
        case 'o':
          omittedcolumn.insert(strtol(optarg,NULL,10));
          break;
        case 'i':
          includedcolumn.push_back(strtol(optarg,NULL,10));
          break;
        case 'd':
          delimiter = string(optarg);
          break;
        default:
          abort ();
        }
    if((argc - optind < 1) or (max(omittedcolumn.size(),includedcolumn.size()) == 0)  ){
		cerr << "You must provide a source file name and an omitted (or included) column" << endl;
		cerr <<"Example: ./csvextractor -o  0 testdata.csv testdata2.csv "<<endl;
		cerr <<"Example: ./csvextractor -i  0 testdata.csv testdata2.csv "<<endl;
		cerr <<"You can repeat the -i or -o flags several times, but you cannot mix them. "<<endl;
		cerr <<"The order in which the -i flags appear matter (\"-i 0 -i 2\" and \"-i 2 -i 0\" return different files). "<<endl;
		return -1;    	
    }
	string csvfilename = argv[optind];
	ostream * out = NULL;
	if(argc - optind > 1) {
	  string csvfilename2 = argv[optind+1];
	  out = new ofstream (csvfilename2.c_str());
	  cout <<"target:"<<csvfilename2<<endl;
	} else {
	  out = &cout;
	}
	if(out != & cout) {
		cout <<"source:"<<csvfilename<<endl;
		if(omittedcolumn.size()!=0) {
			for (set<int>::iterator i = omittedcolumn.begin() ; i!= omittedcolumn.end(); ++i)
				cout <<"omitted column:"<<*i<<endl;
		}
		if(includedcolumn.size()!=0) {
			for (vector<int>::iterator i = includedcolumn.begin() ; i!= includedcolumn.end(); ++i)
				cout <<"included column:"<<*i<<endl;
		}
	}
	ifstream in(csvfilename.c_str());
	CSVParser p(in,delimiter);
    int counter  =0;
    while(p.hasNext()) {
    	++counter;
    	vector<string> line = p.next();
    	if(omittedcolumn.size()!=0) {
    	  bool emptyline = true;
    	  for(uint k = 0; k < line.size();++k)
    		  if(omittedcolumn.find(k)== omittedcolumn.end()) {
    			  if(!emptyline) *out <<delimiter;
    			  emptyline = false;
    			  *out <<line[k];
    		  }
    	} else if(includedcolumn.size()!=0) {
    	  bool emptyline = true;
    	  for(vector<int>::iterator i = includedcolumn.begin(); i != includedcolumn.end(); ++i) {
   			  if(!emptyline) *out <<delimiter;
    		  emptyline = false;
    		  *out <<line[*i];
    	  }
    	} else {
    		cerr << "bug! report me!"<<endl;
    	}
    	*out<<endl;
    }
    if(out != & cout) cout<<"processed "<<counter<<" lines"<<endl;
    in.close();
    if(out != & cout) {
    	static_cast<ofstream *>(out)->close();
    	delete out;
    }
    return 0;
}
