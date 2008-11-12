
#include <cstdlib>
#include <ctype.h>
#include <sys/stat.h>
#include <fstream>
#include <vector>
#include <map>
#include "CSVParser.h"
#include "BitmapWriter.h"
#include "CompressedDataBlock.h"


using namespace std;

void help() {
	cout << "This sorts the column, but putting the highest cardinality column first."<<endl;
	cout<<" You can reverse the sort order with the -r flag."<<endl;
	cout <<"You must provide at least a filename, plus optionally a delimiter using using the -d flag"<<endl;
	cout <<"We write to standard output."<<endl;
}

int main(int argc, char ** argv) {
	std::ios::sync_with_stdio (false);
	string delimiter = ",";
	if(argc == 1) {
			cout <<" You must provide at least a filename"<<endl;
			help();
			return -1;
	}
	int c;
	bool invertedsort (false);
    while ((c = getopt (argc, argv, "d:r")) != -1) {
      switch (c)
        {
        case 'd':
          delimiter = string(optarg);
          break;
        case 'r':
        	invertedsort = true;
          break;
        default:
          abort ();
        }
    }
    if(argc - optind <1) {
    	help();
    	return -2;
    }
    string filename(argv[optind]);
	histogramtype  mHistograms;
	ifstream in(filename.c_str());
	if(!in) {
		cerr <<"I can't open "<<filename<<" aborting!"<<endl;
		return -3;
	}
	HistogramLoader::loadHistogram(filename, mHistograms,delimiter);
	if(mHistograms.size() ==0) {
		cerr <<"empty file. aborting"<<endl;
		in.close();
		return -4;
	}
	vector<pair<uint,uint> > vector_cardinality_columnindex;
	for(uint k = 0; k<mHistograms.size();++k)
		vector_cardinality_columnindex.push_back(pair<uint,uint>(mHistograms[k].size(),k));
	if(!invertedsort)
		sort(vector_cardinality_columnindex.begin(),vector_cardinality_columnindex.end(), greater<pair<uint,uint> >());
	else 
		sort(vector_cardinality_columnindex.begin(),vector_cardinality_columnindex.end());
	CSVParser parser(in,delimiter);
	vector<string> buffer(mHistograms.size());
	while(parser.hasNext()) {
		const vector<string> & line = parser.next();
		if(mHistograms.size() != line.size()) {
			cerr <<" I have got a line with a different number of columns than what I expected, bug?"<<endl;
			break;
		} 
		for(uint k = 0; k < line.size() ; ++k ) {
			buffer[k] = line[vector_cardinality_columnindex[k].second];
		}
		cout <<buffer[0];
		for(uint k = 1; k < line.size(); ++k) {
			cout <<delimiter<<buffer[k];
		}
		cout <<endl;
	}
	in.close();
} 
