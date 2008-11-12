#include "CSVParser.h"
#include "querylanguage.h"

CSVParser::CSVParser(istream & in, const string delimiter, const char  commentmarker) : 
	line(), mDelimiter(delimiter), mDelimiterPlusSpace(delimiter),mCommentMarker( commentmarker),mIn(in),
	currentData()
{
	mDelimiterPlusSpace.append(" ");
	//std::ios::sync_with_stdio (false);
}

CSVParser::~CSVParser()
{
}

/**
 * find matching rows
 */
void CSVParser::query(const string & filename, /*const map<uint,set<string> >& query*/OrthogonalQuery & oq, vector<ulong>& matchinglines,const string & delimiter, const char  commentmarker) {
	ifstream in (filename.c_str());
	if(!in ) cerr << "can't open the file "<<filename<<endl;
	CSVParser csvparser(in,delimiter, commentmarker);
	//vector<ulong> matchinglines;
	ulong counter = 0;
        if(oq.isEqualityQuery()) {
	while(csvparser.hasNext()) {
		const vector <string> & currentdata = csvparser.next();
		bool match(true);
		for(map<uint,set<string> >::const_iterator i =  oq.equality_query.begin(); i != oq.equality_query.end(); ++i) {
			if( i->second.find(currentdata[i->first]) == i->second.end()) {
				match = false;
				break;
			}
		}
		if(match) matchinglines.push_back(counter);
		++counter;
	}
        } else {
	while(csvparser.hasNext()) {
		const vector <string> & currentdata = csvparser.next();
		bool match(true);
		for(map<uint,set<pair<string,string> > >::const_iterator i =  oq.range_query.begin(); i != oq.range_query.end(); ++i) {
                    bool localmatch(false);
                    for(set<pair<string,string> >::iterator j =  i->second.begin(); j!=i->second.end();++j) {
                        if((j->first <= currentdata[i->first]) and (j->second >= currentdata[i->first])) {
                            localmatch = true;
                            break;
                        }
                    }
                    if(!localmatch) {
                        match=false;
                        break;
                    }
		}
		if(match) matchinglines.push_back(counter);
		++counter;
	}            
        }
	in.close();
	//return matchinglines;
}


void  CSVParser::readHistogram(vector<map<string,uint> > & answer) {
   // we initialize the data structure...
   assert(answer.size()==0);
   if(hasNext()) {
	  const vector<string> & currentdata = next();
	  for(vector<string>::const_iterator i = currentdata.begin(); i != currentdata.end(); ++i) {
		  map<string,uint> m;
		  m[*i] = 1;
		  answer.push_back(m);
	  }
  }
  uint64 linecounter = 1;
  bool warned = false;
  while(hasNext()) {
	   ++linecounter;
	   const vector <string> & currentdata = next();
	   if(answer.size() != currentdata.size()) {
		   if(! warned) {
			   cerr <<"Expecting "<<answer.size()<<" elements on line "<<linecounter<< " but got "<<currentdata.size()<< " instead, I will start skipping lines"<<endl;
			   warned = true;
		   }  
	   } else for(uint k = 0; k < answer.size(); ++k) { 
		  //assert(answer.size() == currentdata.size());
		  answer[k][currentdata[k]]++;
	   }
  }
}

