
#include <cstdlib>
#include <ctype.h>
#include <sys/stat.h>
#include <fstream>
#include <vector>
#include "CSVParser.h"
#include "BitmapWriter.h"
#include "CompressedDataBlock.h"

using namespace std;

void help() {
	cout <<"This program will insert columns in CSV matching the frequency of each item."<<endl;
	cout<<"For example, if tulip occurs 100 times, each occurence of tulip will be replaced by 100,tulip"<<endl;
	cout <<"You must provide at least a filename, plus optionally a delimiter using using the -d flag"<<endl;
	cout <<"We write to standard output."<<endl;
}

char * toHex(unsigned int num, char * out) {
	sprintf (out, "%04X", num);
	return out;
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
    while ((c = getopt (argc, argv, "d:")) != -1) {
      switch (c)
        {
        case 'd':
          delimiter = string(optarg);
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
	CSVParser parser(in,delimiter);
	char buffer[16];
	while(parser.hasNext()) {
		const vector<string> & line = parser.next();
		if(mHistograms.size() != line.size()) {
			cerr <<" I have got a line with a different number of columns than what I expected, bug?"<<endl;
			break;
		} 
		cout <<toHex(mHistograms[0][line[0]],buffer)<<delimiter<<line[0];
		for(uint k = 1; k < line.size(); ++k) {
			cout <<delimiter<<toHex(mHistograms[k][line[k]],buffer)<<delimiter<<line[k];
		}
		cout <<endl;
	}
	in.close();
}
