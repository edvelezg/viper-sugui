#ifndef CSVPARSER_H_
#define CSVPARSER_H_

#include <string>
//#include <istream>
#include <fstream>
#include <vector>
#include <map>
#include <assert.h>
#include "Util.h"
#include "querylanguage.h"

using namespace std;

/**
 * Parses coma-separeted values
 * 
 * TODO: should have support for escape characters and quote characters
 */
class CSVParser{
	public:
		CSVParser(istream & in, const string delimiter = ",", const char  commentmarker= '#');
		virtual ~CSVParser();
		inline bool hasNext(){
			//if(!mIn) cout <<"can't read"<<endl;
			//cout << "hasNext called"<<endl;
			while(getline(mIn,line)) {
				if(line.size() == 0) continue;
				if(line.at(0) == mCommentMarker) continue;
				tokenize(line);
			    return true;
			}
			return false;
		}
		inline const vector<string> & next()  const {
			return currentData;
		}
	
		/**
		 * parse the entire file, building a dimension-by-dimension
		 * histogram.
		 */
		void  readHistogram(vector<map<string,uint> > & answer);
		
		/**
		 * find matching rows
		 */
		static void query(const string & filename, /*const map<uint,set<string> >& query*/OrthogonalQuery &oq, vector<ulong>& matchinglines,const string & delimiter = ",", const char  commentmarker= '#');
		string line;
	private:
		inline void tokenize(const string& str);
		string mDelimiter;
		string mDelimiterPlusSpace;
		char mCommentMarker;
		istream & mIn;
		vector<string> currentData;

};




void CSVParser::tokenize(const string& str){
		currentData.clear();
    	string::size_type lastPos = str.find_first_not_of(mDelimiterPlusSpace, 0);
    	string::size_type pos     = str.find_first_of(mDelimiter, lastPos);
		string::size_type pos_w = str.find_last_not_of(' ',pos);
    	while (string::npos != pos || string::npos != lastPos){
    		string token(str,lastPos, pos == string::npos ? pos_w + 1 - lastPos: pos_w -lastPos);
    		currentData.push_back(token);
        	lastPos = str.find_first_not_of(mDelimiterPlusSpace, pos);
        	pos = str.find_first_of(mDelimiter, lastPos);
        	pos_w = str.find_last_not_of(' ',pos);
    	}
        /*
for(uint k = 0; k <currentData.size();++k)
cout <<"'"<<currentData[k]<<"' ";
cout <<endl; */
}

#endif /*CSVPARSER_H_*/
