/**
 * Up until July 2008, the bitmap index library kept the maps from attribute values
 * to bitmap positions in internal memory. The AttributeValueMap uses external-memory
 * b-trees to do the same job.
 */


#ifndef ATTRIBUTEVALUEMAP_H_
#define ATTRIBUTEVALUEMAP_H_

#include <villa.h> // QDBM, see http://qdbm.sourceforge.net/, please download and install!, 
// Daniel: QDBM has a C++ API, but villa is the C API which should do fine
// We use villa because it has sorted keys access (b-tree).
// under macos type "fink install qdbm14"

#include "Util.h"

class BTree {
	public:

		BTree(const string & FileName): mFileName(FileName), mVilla(NULL) {
			open();
		}
		
		BTree() : mFileName(), mVilla(NULL) {
			
		}
		
		BTree & operator=(const BTree & o) {
			mFileName = o.mFileName;
			mVilla  = o.mVilla;
			return *this;
		}
		
		BTree(const BTree & o) : mFileName(o.mFileName), mVilla(o.mVilla) {
			
		}
		
	    int getNumberOfRecords() const {return vlrnum(mVilla);}
	    
	    uint64 getSizeOfBTreeOnDisk() const { 
	    	struct stat my_stat;
	    	if(stat(mFileName.c_str(), &my_stat)!=0) return 0;
	    	if ((my_stat.st_size) > 0)
	    		return my_stat.st_size;
	    	else return 0;
	    }
	    
	    virtual ~BTree() {}

	    void close(){
            if(mVilla) {
               if(!vlclose(mVilla)){
                 cerr << __FILE__ << ":"<< __LINE__ <<" I failed to close " << mFileName << endl;       
                 cerr << dperrmsg(dpecode)<<endl;
                 // here, the user cannot do much, so I'm not throwing an exception. Hope for the best!
               }
            }
            mVilla = NULL;
         }
	    
	    
	    
	    /*
	    void goTo(const char * key, const uint keysize) {
	    	if(!vlcurjump (mVilla,key, keysize,VL_JFORWARD)) {
	    		cerr << " No record found in the database for key "<<key <<endl;
	    		//size = 0;
	    		throw missingattributevalue_exception("Missing attribute value");
	    	}
	    }*/
	    
	    void goToStart()  const {
                if(mVilla == NULL) throw accesstoclosedbtree_exception("goToStart");
	    	if(!vlcurfirst(mVilla)) {
	    		cerr << " No record found in the database"<<endl;
	    		//size = 0;
	    		throw missingattributevalue_exception("Missing attribute value");
	    	}	    	
	    }
	    
	    void goToEnd()  const {
                if(mVilla == NULL) throw accesstoclosedbtree_exception("goToEnd");
	    	if(!vlcurlast(mVilla)) {
	    		cerr << " No record found in the database"<<endl;
	    		//size = 0;
	    		throw missingattributevalue_exception("Missing attribute value");
	    	}	    	
	    }
	    void getCurrentKey(string & out)  const {
	    	if(mVilla == NULL) throw accesstoclosedbtree_exception("getCurrentKey");
                int size;
	    	char * ans = vlcurkey(mVilla,&size);
	    	out = ans;
	    	free(ans);
	    }
	    void getCurrentValues(vector<uint> & out)  const {
                        if(mVilla == NULL) throw accesstoclosedbtree_exception("getCurrentValues");
			int size(0);
			uint * answer = reinterpret_cast<uint *>(vlcurval(mVilla, &size));
			for(uint k = 0; k<static_cast<uint>(size)/sizeof(uint);++k ) {
				out.push_back(answer[k]);
			}
			free(answer);
	    }
	    uint getCurrentValue()  const {
			int size(0);
                        if(mVilla == NULL) throw accesstoclosedbtree_exception("getCurrentValue");
                        char * tofree (  vlcurval(mVilla, &size) );
			uint * answer = reinterpret_cast<uint *>(tofree);
                        if( size != sizeof(uint) )  cerr <<"ops! I am querying one value, and there may be several??? BUG!"<<endl;
                        //return *answer;// oddly, it won't let me free tofree, so what am I going to do?
                        uint out = answer[0];
                        //cout <<" about to delete answer in getCurrentValue..."<<endl;
			free(tofree);
                        //cout <<" about to delete answer in getCurrentValue...ok"<<endl;
                        return out;
	    }	 
            void getValues(const char * key, const int keysize, set<uint> & out) const {
                int size(0);
                if(mVilla == NULL) throw accesstoclosedbtree_exception("getValues");
                uint * answer = reinterpret_cast<uint *>(vlget(mVilla, key,keysize, &size));
                if(answer == NULL) {
	    		cerr << " No record found in the database for key '"<<key <<"'"<<endl;
	    		throw missingattributevalue_exception("Missing attribute value");
                }
		for(uint k = 0; k<static_cast<uint>(size)/sizeof(uint);++k ) {
                    out.insert(answer[k]);
		}
		free(answer);
            }
            void getValues(const char * startkey, const int startkeysize,const char * endkey, const int endkeysize, set<uint> & out) const {
                int size(0);
                if(mVilla == NULL) throw accesstoclosedbtree_exception("getValues");
                if(!vlcurjump(mVilla, startkey, startkeysize, VL_JFORWARD)) return;
	    	char * ans = vlcurkey(mVilla,&size);
                while ( (endkeysize != size) or ( strcmp(ans,endkey)!=0) ) {
                    out.insert(getCurrentValue());
                    if(!vlcurnext(mVilla)) break;
                    free(ans);
                    ans = vlcurkey(mVilla,&size);
                }
                while ( (endkeysize == size) and ( strcmp(ans,endkey)==0) ) {
                    out.insert(getCurrentValue());
                    if(!vlcurnext(mVilla)) break;
                    free(ans);
                    ans = vlcurkey(mVilla,&size);
                }
                free(ans);
            }            
            
            void getValues(const char * key, const int keysize, vector<uint> & out) const {
                int size(0);
                if(mVilla == NULL) throw accesstoclosedbtree_exception("getValues");
                uint * answer = reinterpret_cast<uint *>(vlget(mVilla, key,keysize, &size));
                if(answer == NULL) {
	    		cerr << " No record found in the database for key '"<<key <<"'"<<endl;
	    		throw missingattributevalue_exception("Missing attribute value");
                }
		for(uint k = 0; k<static_cast<uint>(size)/sizeof(uint);++k ) {
                    out.push_back(answer[k]);
		}
		free(answer);
            }
            void getValues(const char * startkey, const int startkeysize,const char * endkey, const int endkeysize, vector<uint> & out) const {
                int size(0);
                if(mVilla == NULL) throw accesstoclosedbtree_exception("getValues");
                if(!vlcurjump(mVilla, startkey, startkeysize, VL_JFORWARD)) return;
	    	char * ans = vlcurkey(mVilla,&size);
                while ( (endkeysize != size) or ( strcmp(ans,endkey)!=0) ) {
                    out.push_back(getCurrentValue());
                    if(!vlcurnext(mVilla)) break;
                    free(ans);
                    ans = vlcurkey(mVilla,&size);
                }
                free(ans);
            }            
            
            uint getValue(const char * key, const int keysize) const {
                int size(0);
                if(mVilla == NULL) throw accesstoclosedbtree_exception("getValue");
                uint * answer = reinterpret_cast<uint *>(vlget(mVilla, key,keysize, &size));
                if(answer == NULL) {
	    		cerr << " No record found in the database for key '"<<key<<"'" <<endl;
	    		throw missingattributevalue_exception("Missing attribute value");
                }
                if(static_cast<uint>(size) != sizeof(uint)) {
                    cerr <<"Somehow the software expected one mapping, but I have several. Did you try to use k-of-N encoding when it is not supported?"<<endl;
                    throw unexectedencoding_exception("did not find exactly one mapping");
                }
                uint out = answer[0];
                free(answer);
                return out;
            }
            
	    bool readNext() const {
	    	return vlcurnext(mVilla);
	    }
	    
	    inline char * getKeyAt(const uint pos, int & size) const {
                if(mVilla == NULL) throw accesstoclosedbtree_exception("getKeyAt");
	    	if(!vlcurfirst(mVilla)) {
	    		cerr << " No record found in the database"<<endl;
	    		size = 0;
	    		throw missingattributevalue_exception("Missing attribute value");
	    	}
	    	uint counter = 0;
	    	do {
	    		if(counter++ == pos) {
	    			return vlcurkey(mVilla, &size);
	    		}
	    	} while(vlcurnext(mVilla));
	    	cerr <<" no ith record, bug?"<<endl;
	    	throw missingattributevalue_exception("Missing attribute value");
	    }
	    	    // return true if the value was added
	    inline bool add(const char * key, const uint keysize, const char * value, const uint valuesize) {
                	  if(mVilla == NULL) throw accesstoclosedbtree_exception("add");
	        	  if (! vlput(mVilla, key, keysize, value, valuesize, VL_DCAT)) {
	        	        cerr  << __FILE__ << ":"<< __LINE__ << " I didn't manage to store the data in "<< mFileName <<": " <<dperrmsg(dpecode)<< endl;
	        	        return false;
	        	  }
	        	  return true;
	    }
	        
private:

    
    void open() throw(ifstream::failure){
    	  close();
    	  mkdir_parentsonly(mFileName.c_str());//
    	  if( ! (mVilla = vlopen(mFileName.c_str(), VL_OWRITER | VL_OCREAT ,VL_CMPLEX))) {//| VL_OZCOMP//VL_OCREAT//|VL_OZCOMP
    	    cerr  << __FILE__ << ":"<< __LINE__ << "I failed to open " << mFileName << endl;
    	    cerr << " The file might be corrupted: not a QDBM format. "<< endl;
    	    cerr << " You may have to delete it. "<< endl;
    	    stringstream strs;
    	    strs << " Failed to open file "<< mFileName<< ", error might be QDBM related. I'm throwing an ifstream::failure!";
    	    strs << dperrmsg(dpecode);
    	    //string errormsg;
    	    //strs >>  errormsg ; 
    	    throw ifstream::failure(strs.str());
    	  }
    	  //  vlsettuning(mVilla,14096,0,0,0);// vlsettuning seems to make no difference
    	}

    string mFileName; // B-tree file
    VILLA *mVilla;
    
};



class AttributeValueMap {
public:
	AttributeValueMap(const string & filename): mFileName(filename), mBTrees(0){
		readMetadataFile();
		vector<string> names(mBTrees.size());
		AttributeValueMap::getAllFileNames(filename, names);
		for(uint k = 0; k < mBTrees.size();++k)
			mBTrees[k] = BTree(names[k]);

	}
	
	~AttributeValueMap() {
		for(uint k = 0; k < mBTrees.size();++k)
			mBTrees[k].close();
	}
        
        uint getFirstIndex(const uint dim) {
		if(dim >= mBTrees.size()) {
			cerr <<"Warning: could not find  column " <<dim <<" in the btrees"<<endl;
			throw dimensiondoesnotexist_exception("Missing dimension");
		}   
                mBTrees[dim].goToStart();
                return mBTrees[dim].getCurrentValue();
        }        
        uint getLastIndex(const uint dim) {
		if(dim >= mBTrees.size()) {
			cerr <<"Warning: could not find  column " <<dim <<" in the btrees"<<endl;
			throw dimensiondoesnotexist_exception("Missing dimension");
		}
                mBTrees[dim].goToEnd();
                return mBTrees[dim].getCurrentValue();
        }        
                
        template <class container>
	void getBitmapsIndices(const uint dim, const string & attributevalue, container & out) {//vector<uint>
		if(dim >= mBTrees.size()) {
			cerr <<"Warning: could not find  column " <<dim <<" in the btrees"<<endl;
			throw dimensiondoesnotexist_exception("Missing dimension");
		}
		mBTrees[dim].getValues(attributevalue.c_str(), attributevalue.size(),out);
		//mBTrees[dim].getCurrentValues(out);
	}
        template <class container>
	void getBitmapsIndices(const uint dim, const string & startattributevalue,const string & endattributevalue, container & out) {//vector<uint>
		if(dim >= mBTrees.size()) {
			cerr <<"Warning: could not find  column " <<dim <<" in the btrees"<<endl;
			throw dimensiondoesnotexist_exception("Missing dimension");
		}
                mBTrees[dim].getValues(startattributevalue.c_str(), startattributevalue.size(),endattributevalue.c_str(), endattributevalue.size(),out);
		//mBTrees[dim].getCurrentValues(out);
	}
	uint getIndex(const uint dim, const string & attributevalue) {
		if(dim >= mBTrees.size()) {
			cerr <<"Warning: could not find  column " <<dim <<" in the btrees"<<endl;
			throw dimensiondoesnotexist_exception("Missing dimension");
		}
		return mBTrees[dim].getValue(attributevalue.c_str(), attributevalue.size());
		//return mBTrees[dim].getCurrentValue();
	}	
        
	uint getNumberOfAttributeValues(const uint dim) const {
		if(dim >= mBTrees.size()) {
			cerr <<"Warning: could not find  column " <<dim <<" in the btrees"<<endl;
			throw dimensiondoesnotexist_exception("Missing dimension");
		}
		return mBTrees[dim].getNumberOfRecords();
	}
	
	void getRandomAttributeValue(const uint dim, string & out) {
		int which = static_cast<int> (static_cast<double> (rand()) / RAND_MAX * getNumberOfAttributeValues(dim));
		getAttributeValueAt(dim, which, out);
	}
        
        /**
         *This is probably very slow! Don't use during query processing.
         */
        pair<uint,uint> getRangeOfBitmapIndexes(const uint dim) const {
		if(dim >= mBTrees.size()) {
			cerr <<"Warning: could not find  column " <<dim <<" in the btrees"<<endl;
			throw dimensiondoesnotexist_exception("Missing dimension");
		}
                mBTrees[dim].goToStart();
                vector<uint> pos;
                pair<uint,uint> ans(UINT_MAX,0);
		do {
			string  key;
			mBTrees[dim].getCurrentKey(key);
                        pos.clear();
			mBTrees[dim].getCurrentValues(pos);
                        const uint localmax = *max_element(pos.begin(),pos.end());
                        const uint localmin = *min_element(pos.begin(),pos.end());
                        if(localmax > ans.second) ans.second = localmax;
                        if(localmin < ans.first) ans.first = localmin;
		} while(mBTrees[dim].readNext());
            return ans;
        }
	
	void dumpMap(const uint dim)  const {
		if(dim >= mBTrees.size()) {
			cerr <<"Warning: could not find  column " <<dim <<" in the btrees"<<endl;
			throw dimensiondoesnotexist_exception("Missing dimension");
		}
		mBTrees[dim].goToStart();
		do {
			string  key;
			mBTrees[dim].getCurrentKey(key);
			vector<uint> pos;
			mBTrees[dim].getCurrentValues(pos);
			cout<<" key -> ";
			for(uint k = 0;k<pos.size();++k)
				cout<<pos[k]<<" ";
			cout <<endl;
		} while(mBTrees[dim].readNext());
	}
	
	void getAttributeValueAt(const uint dim, const uint i, string & out) {
		if(dim >= mBTrees.size()) {
			cerr <<"Warning: could not find  column " <<dim <<" in the btrees"<<endl;
			throw dimensiondoesnotexist_exception("Missing dimension");
		}
		int size;
		char * at = mBTrees[dim].getKeyAt(i,size);
		out =  at;
		free(at);
	}
	static void writeToDisk(const vector<stringuintmultimap >& tobitmap, const string & filename, const string & comment = "") {
		vector<string> names(tobitmap.size());
		AttributeValueMap::getAllFileNames(filename, names);
		for(uint k = 0; k< tobitmap.size();++k) {
			::unlink(names[k].c_str());
			BTree bt(names[k]);
			const stringuintmultimap & thismultimap(tobitmap[k]);
			for(stringuintmultimap::const_iterator  iter = thismultimap.begin(); iter != thismultimap.end(); iter++ ) {
				const string & attributevalue = iter->first;
				const uint numberofchars = attributevalue.size();
				const uint pos = iter->second;
				//cout<<"for dim "<<k<<" adding pos "<<pos<<" to attribute value "<<attributevalue<<endl;
				bt.add(attributevalue.c_str(),numberofchars,reinterpret_cast<const char *>(& pos),sizeof(pos));	
			}			
			bt.close();
		}
		AttributeValueMap::createMetadataFile(filename,tobitmap.size(), comment);
	}
	
	static void getAllFileNames(const string & filename, vector<string> & out) {
		for(uint d = 0; d<out.size();++d) {
			stringstream ss;
			ss << filename;
			ss << ".dim";
			ss << d;
			out[d]= ss.str();
		}
	}
	
	static void createMetadataFile(const string & filename, const uint numberofdims, const string & comment) {
		stringstream ss;
		ss << filename;
		ss<<".dim";
		ofstream out(ss.str().c_str());
		if(!out) {
			cerr<<"Can't open file "<<ss.str()<< " to create/replace it"<<endl;
		} else {
			out<<"QDBMVilla\n"<<endl;
			out <<numberofdims<< " dimensions\n"<<endl;
                        out <<comment<<endl;
			out.close();
		}
	}
	void readMetadataFile() {
		stringstream ss;
		ss << mFileName;
		ss<<".dim";
		ifstream in(ss.str().c_str());
		if(!in) {
			cerr<<"Can't open file "<<ss.str()<<endl;
                        throw missingmetadatafile_exception();
		} else {
			string s;
			in >> s;
			if(s.compare("QDBMVilla")!=0) {
				cerr<<"I was expecting "<<s<<" to be QDBMVilla, check file "<<ss.str()<<endl;
				
			} else {
				uint NumberOfDimensions;
				in >> NumberOfDimensions;
				mBTrees.resize(NumberOfDimensions);
			}
			in.close();
		}
	}

	uint size() const { return mBTrees.size();}
private:
	AttributeValueMap(const AttributeValueMap & o);
	AttributeValueMap & operator=(const AttributeValueMap & o);
	string mFileName;
	vector<BTree> mBTrees;
	
};


#endif /*ATTRIBUTEVALUEMAP_H_*/
