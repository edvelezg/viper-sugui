
#include "CSVParser.h"


int main(int argc, char ** argv) {
	std::ios::sync_with_stdio (false);
	string delimiter = ",";
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
	CSVParser parser(cin,delimiter);
	while(parser.hasNext()) {
		const vector<string> & line = parser.next();
		if(line.size()==1) continue;
		cout <<line[1];
		for(uint k = 3; k < line.size(); k+=2) {
			cout<<delimiter<<line[k];
		}
		cout <<endl;
	}
}
