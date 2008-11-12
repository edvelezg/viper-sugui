/*
 * File:   SqrtIntervalBitmapIndex.h
 * Author: lemire
 *
 * Created on September 3, 2008, 9:02 PM
 */

#ifndef _INTERVALBITMAPINDEX_H
#define	_INTERVALBITMAPINDEX_H


#include "IntervalBitmapWriter.h"
#include "LogicalOperations.h"



/////////////////////////////////////////////////////
//This is the classical scheme
////////////////////////////////////////////////////



class RangeBitmapIndex {
public:
	RangeBitmapIndex(const string & indexfile);
    virtual ~RangeBitmapIndex();

    void process(const OrthogonalQuery & oq, vector<ulong> & result, int hint = HINT_NONE);

    uint sizeOfHeader() {
        return startofdata;
    }

    enum {
        version = RangeBitmapWriter<SparseCompressedDataBlock<> >::version
    };


    //vector<stringuintmultimap > & getStringToBitmap() {return mCode;}
    //const vector<stringuintmultimap > & getStringToBitmap() const {return mCode;}
    AttributeValueMap avm;


    void readHeader(ifstream & in);
    fstream::pos_type startofdata; // where does the data start (and header ends)
    fstream::pos_type locationoffirstunreadblock; // location of the second block
    string mFilename;
    uint mCompressionMode;
    uint mNumberOfBits;
    uint getNumberOfBits() const { return mNumberOfBits;}

private:
    template <class uword>
    void rangeQuery_templated(const OrthogonalQuery & oq, vector<ulong> & result, const int hint);


    static void computeRangeQuerySet(const map<uint,set<pair<string,string> > >  range_query, AttributeValueMap & avm, map<uint,IntervalQuerySet> & ciset)  {
    	const bool debug (false);
        for (map<uint, set<pair<string,string> > >::const_iterator i = range_query.begin(); i != range_query.end(); ++i) {
            for(set<pair<string,string> >::iterator k = i->second.begin(); k!= i->second.end(); ++k) {
                  if(k->first>k->second) {
                      cerr <<" in lexicographical order, "<<k->first<<" comes after "<< k->second<<endl;
                      cerr<<" So your query is not a valid range query."<<endl;
                      throw invalidrangequery_exception();
                  }
                  const pair<uint,uint> inter(avm.getIndex(i->first, k->first),avm.getIndex(i->first, k->second));
                  if(debug)cout<<"Have a query from "<<inter.first<<" to "<<inter.second<<endl;
                  if(debug)cout<<"range of bitmap is from  "<<avm.getFirstIndex(i->first)<<" to "<<avm.getLastIndex(i->first)<<endl;
                  if(avm.getFirstIndex(i->first) == inter.first) {
                	  if(debug)cout<<" we insert bitmap "<<inter.second<<endl;
                	  ciset[i->first].bitmaps.insert(inter.second);
                  } else {
                	  if(debug)cout<<" AND "<<inter.second<<" NOT"<<inter.first-1 << endl;
                	  ciset[i->first].andnots.insert(pair<uint,uint>(inter.second,inter.first-1));
                  }
              }
        }
    }

};

RangeBitmapIndex::RangeBitmapIndex(const string & filename) : avm(filename), startofdata(0),
locationoffirstunreadblock(0),  mFilename(filename),
mCompressionMode(0), mNumberOfBits(0) {
    ifstream in(mFilename.c_str());
    if (!in) {
        cerr << "I failed to open file " << filename << endl;
    }
    readHeader(in);
    startofdata = in.tellg();
    if (!in) {
        cerr << "I failed to read the header of the file " << filename << endl;
    }
    locationoffirstunreadblock = in.tellg();
    in.close();
}

RangeBitmapIndex::~RangeBitmapIndex() {
}

void RangeBitmapIndex::readHeader(ifstream & in) {
    uint cookie(0);
    in.read(reinterpret_cast<char *> (& cookie), sizeof (cookie));
    if (cookie != BitmapWriter<SparseCompressedDataBlock<> >::cookie) {
        cerr << "index file cookie mismatch" << endl;
        throw cookiemismatch_exception("bad cookie");
    }
    if (!in) {
        throw cookiemismatch_exception("trouble while reading cookie");
    }
    uint currentversion(0);
    // we always start out with the file format version number
    in.read(reinterpret_cast<char *> (& currentversion), sizeof (currentversion));
    if (currentversion != version) {
        cerr << "version mismatch, expect trouble" << endl;
        throw versionmismatch_exception("bad file version");
    }
    in.read(reinterpret_cast<char *> (& mCompressionMode), sizeof (mCompressionMode));
    in.read(reinterpret_cast<char *> (& mNumberOfBits), sizeof (mNumberOfBits));
}






template<class uword>
void RangeBitmapIndex::rangeQuery_templated(const OrthogonalQuery & oq, vector<ulong> & result, const int hint) {
    map<uint,IntervalQuerySet> queryset;
    const bool debug(false);
    if(debug) cout<<"[debug] computing queryset..."<<endl;
    RangeBitmapIndex::computeRangeQuerySet(oq.range_query,this->avm,queryset);
    if(debug) queryset.begin()->second.describe();
    ifstream in(mFilename.c_str());
    in.seekg(locationoffirstunreadblock);
    EWAHBoolArray<uword> answers;
    EWAHBoolArray<uword> container;
    set<uint> totalqueryset;
    if(debug) cout<<"[debug] computing totalqueryset..."<<endl;
    for(map<uint,IntervalQuerySet >::iterator i = queryset.begin(); i!=queryset.end();++i)
        i->second.dumpBitmaps(totalqueryset);
    if(debug) cout<<"[debug]  totalqueryset.size() = "<< totalqueryset.size()<<endl;
    while (in) {
        ReadCompressedDataBlock<uword> db;
        db.readFromDisk(in, totalqueryset);
        if (!in) break;
        if (totalqueryset.size() == 1) {
            answers.append(db.getBitmap(*totalqueryset.begin()));
        } else {
            orthogonalquery(db.getBuffer(),queryset,container,hint);
            if (answers.sizeInBits() == 0)
                answers.swap(container);
            else
                answers.append(container);
        }
    }
    in.close();
    answers.appendRowIDs(result);
}


//void equalityQuery(const map<uint, string> & query, vector<ulong> & result, const bool sparse = false);

void RangeBitmapIndex::process(const OrthogonalQuery & oq, vector<ulong> & result, int hint) {
    if (oq.isEqualityQuery()) {
        cerr<<"interval bitmap indexes do not support equality queries directly, use the range query syntax."<<endl;
        throw unsupportedoperation_exception("no support for equality queries");
    } else {
        if (mNumberOfBits == 32)
            rangeQuery_templated<uword32 > (oq, result, hint);
        else if (mNumberOfBits == 64)
            rangeQuery_templated<uword64 > (oq, result, hint);
        else if (mNumberOfBits == 16)
            rangeQuery_templated<uword16 > (oq, result, hint);
        else
            throw unsupportedwordsize_exception("I do not supported the requested word size");
    }
}



/////////////////////////////////////////////////////
// Next is sqrt-encoding (Daniel's accidental invention)
////////////////////////////////////////////////////




class SqrtIntervalBitmapIndex {
public:
    SqrtIntervalBitmapIndex(const string & indexfile);
    virtual ~SqrtIntervalBitmapIndex();

    // the integers being return point to rows
    // the query object is a map describing the desired rows...
    // for example, if you want the first column to have value "cat"
    // and the second column to have value "dog", you have to pass
    // a map satisfying
    // map[0] = "cat";map[1] = "dog"
   // void equalityQuery(const map<uint, string> & query, vector<ulong> & result, const bool sparse = false);
    void process(const OrthogonalQuery & oq, vector<ulong> & result, int hint = HINT_NONE);

    uint sizeOfHeader() {
        return startofdata;
    }

    enum {
        version = SqrtIntervalBitmapWriter<SparseCompressedDataBlock<> >::version
    };


    //vector<stringuintmultimap > & getStringToBitmap() {return mCode;}
    //const vector<stringuintmultimap > & getStringToBitmap() const {return mCode;}
    AttributeValueMap avm;


    void readHeader(ifstream & in);
    fstream::pos_type startofdata; // where does the data start (and header ends)
    fstream::pos_type locationoffirstunreadblock; // location of the second block
    string mFilename;
    uint mCompressionMode;
    uint mNumberOfBits;
    uint getNumberOfBits() const { return mNumberOfBits;}
    vector<uint> intervalwidths;

private:
    template <class uword>
    void rangeQuery_templated(const OrthogonalQuery & oq, vector<ulong> & result, const int hint);

    static void computeRangeQuerySet(const map<uint,set<pair<string,string> > >  range_query, AttributeValueMap & avm, const vector<uint> & intervalwidths, map<uint,IntervalQuerySet> & ciset)  {
    	const bool debug (false);
        for (map<uint, set<pair<string,string> > >::const_iterator i = range_query.begin(); i != range_query.end(); ++i) {
            for(set<pair<string,string> >::iterator k = i->second.begin(); k!= i->second.end(); ++k) {
                  if(k->first>k->second) {
                      cerr <<" in lexicographical order, "<<k->first<<" comes after "<< k->second<<endl;
                      cerr<<" So your query is not a valid range query."<<endl;
                      throw invalidrangequery_exception();
                  }
                  if(debug) cout<<"[debug] dim "<<i->first<<endl;
                  if(debug) cout<<"[debug] dim bitmaps from "<<avm.getFirstIndex(i->first)<<" to "<<avm.getLastIndex(i->first) <<endl;
                  if(debug) cout <<"[debug] interval widths is  intervalwidths["<<i->first<<"] = "<<intervalwidths[i->first]<<endl;
                  if(debug) cout <<"[debug] query from "<<k->first<<" to "<< k->second<<endl;
                  const pair<uint,uint> inter(avm.getIndex(i->first, k->first),avm.getIndex(i->first, k->second));
                  if(debug) cout <<"[debug] mapped to from "<<inter.first<<" to "<< inter.second<<endl;
                  if(inter.second-inter.first+1 < intervalwidths[i->first]) {
                	  /**
                	   *  we have a small interval
                	   */
                      if(inter.second  == avm.getLastIndex(i->first)) {
                    	  /*if(debug) cout <<"[debug] adding an ands from  "<<inter.first<<" to "<< inter.second-intervalwidths[i->first]<<endl;
                    	  if(inter.second <  avm.getFirstIndex(i->first)+intervalwidths[i->first])
                    	    throw "mysterious bug";
                    	  //ciset[i->first].ands.insert(pair<uint,uint>(inter.first,inter.second-intervalwidths[i->first]) );*/
                    	  if(debug) cout <<" going from "<<inter.first<<" to the end, so just have to check one bitmap"<<endl;
                    	  ciset[i->first].bitmaps.insert(inter.first);
                      } else  {
                    	  if(debug) cout <<"[debug] adding an andnotss from  "<<inter.first<<" to "<< inter.second<<endl;
                    	  if(debug) cout <<"[debug] (  "<<inter.first<<" ) AND  NOT "<< (inter.second + 1)<<endl;
                    	  ciset[i->first].andnots.insert(pair<uint,uint>(inter.first,inter.second + 1) );
                      }
                  } else {
                	/**
                	 * We have a large interval.
                	 */
                	if(debug) cout <<"must go from  "<<inter.first<<" to "<< inter.second<<endl;
                    uint b(inter.first),e(b+intervalwidths[i->first]);
                    if(debug) cout <<"starting a bitmap "<<b<<endl;
                    for(; e <= inter.second;b=e,e=b+intervalwidths[i->first]) {
                      if(debug) cout <<"[debug] adding bitmap  "<<b<<endl;
                      ciset[i->first].bitmaps.insert(b);
                    }
                    // if we got lucky, then we are done
                    if((e==inter.second+1) or (avm.getLastIndex(i->first) == inter.second)) {
                      if(debug) cout <<"[debug] (lucky) adding bitmap  "<<b<<endl;
                      ciset[i->first].bitmaps.insert(b);
                      continue;
                    }
                    if(debug) cout <<"[debug] adding bitmap  "<<(inter.second - intervalwidths[i->first]+1)<<endl;
                    ciset[i->first].bitmaps.insert(inter.second - intervalwidths[i->first]+1);
                    if(debug) {
                    	for(set<uint>::iterator it = ciset[i->first].bitmaps.begin(); it!=ciset[i->first].bitmaps.end();++it)
                    		cout <<" OR ( "<<*it<<" ) ";
                        cout<<endl;
                    }
                 }
            }
        }
    }

};

SqrtIntervalBitmapIndex::SqrtIntervalBitmapIndex(const string & filename) : avm(filename), startofdata(0),
locationoffirstunreadblock(0),  mFilename(filename),
mCompressionMode(0), mNumberOfBits(0) {
    ifstream in(mFilename.c_str());
    if (!in) {
        cerr << "I failed to open file " << filename << endl;
    }
    readHeader(in);
    startofdata = in.tellg();
    if (!in) {
        cerr << "I failed to read the header of the file " << filename << endl;
    }
    locationoffirstunreadblock = in.tellg();
    in.close();
    computeSqrtIntervalWidths(this->avm, intervalwidths);
}

SqrtIntervalBitmapIndex::~SqrtIntervalBitmapIndex() {
}

void SqrtIntervalBitmapIndex::readHeader(ifstream & in) {
    uint cookie(0);
    in.read(reinterpret_cast<char *> (& cookie), sizeof (cookie));
    if (cookie != BitmapWriter<SparseCompressedDataBlock<> >::cookie) {
        cerr << "index file cookie mismatch" << endl;
        throw cookiemismatch_exception("bad cookie");
    }
    if (!in) {
        throw cookiemismatch_exception("trouble while reading cookie");
    }
    uint currentversion(0);
    // we always start out with the file format version number
    in.read(reinterpret_cast<char *> (& currentversion), sizeof (currentversion));
    if (currentversion != version) {
        cerr << "version mismatch, expect trouble" << endl;
        throw versionmismatch_exception("bad file version");
    }
    in.read(reinterpret_cast<char *> (& mCompressionMode), sizeof (mCompressionMode));
    in.read(reinterpret_cast<char *> (& mNumberOfBits), sizeof (mNumberOfBits));
}






template<class uword>
void SqrtIntervalBitmapIndex::rangeQuery_templated(const OrthogonalQuery & oq, vector<ulong> & result, const int hint) {
    map<uint,IntervalQuerySet> queryset;
    //map<uint,set<pair<uint,uint> > >   andnot;
    const bool debug(false);
    if(debug) cout<<"[debug] computing queryset..."<<endl;
    SqrtIntervalBitmapIndex::computeRangeQuerySet(oq.range_query,this->avm,intervalwidths ,queryset);
    if(debug) queryset.begin()->second.describe();
    ifstream in(mFilename.c_str());
    in.seekg(locationoffirstunreadblock);
    EWAHBoolArray<uword> answers;
    EWAHBoolArray<uword> container;
    set<uint> totalqueryset;
    if(debug) cout<<"[debug] computing totalqueryset..."<<endl;
    for(map<uint,IntervalQuerySet >::iterator i = queryset.begin(); i!=queryset.end();++i)
        i->second.dumpBitmaps(totalqueryset);
    if(debug) cout<<"[debug]  totalqueryset.size() = "<< totalqueryset.size()<<endl;
    while (in) {
        ReadCompressedDataBlock<uword> db;
        db.readFromDisk(in, totalqueryset);
        if (!in) break;
        if (totalqueryset.size() == 1) {
            answers.append(db.getBitmap(*totalqueryset.begin()));
        } else {
            orthogonalquery(db.getBuffer(),queryset,container,hint);
            if (answers.sizeInBits() == 0)
                answers.swap(container);
            else
                answers.append(container);
        }
    }
    in.close();
    answers.appendRowIDs(result);
}


//void equalityQuery(const map<uint, string> & query, vector<ulong> & result, const bool sparse = false);

void SqrtIntervalBitmapIndex::process(const OrthogonalQuery & oq, vector<ulong> & result, int hint) {
    if (oq.isEqualityQuery()) {
        cerr<<"interval bitmap indexes do not support equality queries directly, use the range query syntax."<<endl;
        throw unsupportedoperation_exception("no support for equality queries");
    } else {
        if (mNumberOfBits == 32)
            rangeQuery_templated<uword32 > (oq, result, hint);
        else if (mNumberOfBits == 64)
            rangeQuery_templated<uword64 > (oq, result, hint);
        else if (mNumberOfBits == 16)
            rangeQuery_templated<uword16 > (oq, result, hint);
        else
            throw unsupportedwordsize_exception("I do not supported the requested word size");
    }
}




#endif	/* _INTERVALBITMAPINDEX_H */

