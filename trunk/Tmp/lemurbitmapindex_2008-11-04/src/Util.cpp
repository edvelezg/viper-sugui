#include <assert.h>
#include "Util.h"


int mkdir_parentsonly(const char *path) {
        //cout<<" called with "<<path<<endl;
        char * shorter_path = strdup(path);
        char * p = strrchr(shorter_path, '/');// find the last occurance of '/'
        if(p == 0 || p == path) { 
                  free(shorter_path); 
                  return 0;// nothing to do
        }
        p[0] = 0;
        //cout<<" making dir "<<shorter_path<<endl;
        int errnumero = mkdir(shorter_path,0777);
        if ( errnumero < 0 && errno != EEXIST) {
          if ((errno > 0 ) && (errno != ENOENT))  {// the path contains a file, no hope of solving this
                        cerr << "The path "<< path <<" could not be created, errno "<< errno<< ". No hope of creating it. " << endl;
                        free(shorter_path);
                        return -1;
          }
          int returnvalue = mkdir_parentsonly(shorter_path);
          if(returnvalue == 0)
                  returnvalue = mkdir(shorter_path,0777);
          free(shorter_path);
          return returnvalue;
        } else {
                  free(shorter_path);
                  return 0;//success!
        }
}

void loadVectorMapFromTabSeparatedText(vector<map<string,uint> > & answer, istream & in) {
	answer.clear();// to make sure
	map<string,uint> latestmap;
	answer.push_back(latestmap);
	string line;
	while(getline(in,line)) {
		if(line.size() == 0) {
			map<string,uint> latestmap;
			answer.push_back(latestmap);			
		} else {
			const uint split = line.find_first_of('\t', 0);
			if(split == static_cast<uint>(string::npos))
				throw nottabseparatedtext_exception(line);
			const string attributeval =  line.substr(0, split);
			const uint val = atol(line.substr(split+1, line.size()).c_str());
			answer[answer.size()-1][attributeval] = val;
		}
	}
	if(answer[answer.size()-1].size() == 0)
		answer.pop_back();
}

void saveVectorMapAsTabSeparatedText(vector<map<string,uint> > & answer, ostream & out) {
	for(vector<map<string,uint> >::const_iterator i = answer.begin(); 
	  i!= answer.end(); ++i) {
		const map<string,uint> & myhisto = *i;
		for( map<string,uint>::const_iterator iter = myhisto.begin(); 
		iter != myhisto.end(); iter++ ) {
			out << iter->first << "\t" << iter->second << endl;
	   }
	   out << endl;
	}
}








