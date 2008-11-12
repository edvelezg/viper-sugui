/*
 * File:   querylanguage.h
 * Author: lemire
 *
 * Created on August 20, 2008, 11:27 AM
 */

#ifndef _QUERYLANGUAGE_H
#define	_QUERYLANGUAGE_H

#include <set>
#include <map>

#include "AttributeValueMap.h"
#include "CompressedDataBlock.h"

using namespace std;







class cantparsequery_exception {
public:
	cantparsequery_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;};
private:
      string message;
};

/**
 *Our goal is not to reproduce SQL or create a new language.
 *This is just a class to help us parse simple command-line queries.
 *
 *basically, for each dimension you are allowed to specify one value
 *or one range of values. That is all.
 */
class OrthogonalQuery {
public:
    OrthogonalQuery() : equality_query(), range_query() {}
    map<uint,set<string> >  equality_query;
    map<uint,set<pair<string,string> > >  range_query;

    void process(char ** args, int & k, const int maxargs) {
        if(k>= maxargs) {throw cantparsequery_exception ("should never happen");}
        char * key = args[k];
         if((key[0] == 'r') or (key[0] == 'R')) {
            // we have a range specification
            uint dim = strtol(key+1, NULL, 10);
            if(k+2>= maxargs) {throw cantparsequery_exception ("trouble parsing your query!");}
            char * begin = args[k+1];
            char * end = args[k+2];
            set<pair<string,string> > & s = range_query[dim];
            s.insert(pair<string,string>(begin,end));
            k+=2;
        } else {
            // we have an equality query
            if(k+1>= maxargs) {throw cantparsequery_exception ("trouble parsing your query!");}
            uint dim = strtol(key, NULL, 10);
            char * value = args[k+1];
            set<string> & s = equality_query[dim];
            s.insert(value);
            ++k;
        }
    }

    bool isEqualityQuery() const {
        return range_query.size() == 0;
    }

    /**
     * for equality queries (bitmaps!)
     */
    void computeEqualityQuerySet(AttributeValueMap & avm, vector<uint> & querysetfrombtree) const {
        for (map<uint, set<string> >::const_iterator i = equality_query.begin(); i != equality_query.end(); ++i) {
            const uint numberofstring = i->second.size();
            if(numberofstring > 1 ) {
                cerr<<"for dim "<<i->first<<" you have "<<numberofstring<<" possible values. "<<endl;
                for(set<string>::const_iterator k = i->second.begin(); k!= i->second.end(); ++k)
                    cerr<< *k<<endl;
                cerr<<"that's not a valid equality query."<<endl;
                throw unsupportedoperation_exception("your index does not yet support containment queries");
            }
            for(set<string>::const_iterator k = i->second.begin(); k!= i->second.end(); ++k)
                avm.getBitmapsIndices(i->first, *k, querysetfrombtree);
        }
    }


    /**
     * for fancy equality queries (have OR on each dimension..., so not really equality queries!).
     */
    template <class container>
    void computeContainmentQuerySet(AttributeValueMap & avm, map<uint,container> & querysetfrombtree) const {
        for (map<uint, set<string> >::const_iterator i = equality_query.begin(); i != equality_query.end(); ++i) {
            for(set<string>::const_iterator k = i->second.begin(); k!= i->second.end(); ++k)
                avm.getBitmapsIndices(i->first, *k, querysetfrombtree[i->first]);
        }
    }

    void whichColumns(set<uint> & out) const {
        for(map<uint,set<string> >::const_iterator i = equality_query.begin(); i != equality_query.end(); ++i)
            out.insert(i->first);
        for(map<uint,set<pair<string,string> > >::const_iterator i = range_query.begin(); i != range_query.end(); ++i)
            out.insert(i->first);
    }



    void computeRangeQuerySet(AttributeValueMap & avm, map<uint,set< pair<uint,uint> > > & querysetfrombtree) const {
        for (map<uint, set<pair<string,string> > >::const_iterator i = range_query.begin(); i != range_query.end(); ++i) {
            for(set<pair<string,string> >::const_iterator k = i->second.begin(); k!= i->second.end(); ++k) {
                  if(k->first>k->second) {
                      cerr <<" in lexicographical order, "<<k->first<<" comes after "<< k->second<<endl;
                      cerr<<" So your query is not a valid range query."<<endl;
                      throw invalidrangequery_exception();
                  }
                  const pair<uint,uint> ans(avm.getIndex(i->first, k->first),avm.getIndex(i->first, k->second));
                  assert(ans.first<=ans.second);
                  querysetfrombtree[i->first].insert(ans);
            }
        }
    }




    /**
     *Chan-Ionnadis
     *
     *
     */

    void randomizeQuery(AttributeValueMap & avm) {
       if(!isEqualityQuery()) throw unsupportedoperation_exception("randomize non-equality not supported yet");
       for (map<uint, set<string> >::iterator it = equality_query.begin(); it != equality_query.end(); ++it) {
           //const uint size = it->second.size();
           it->second.clear();
           string rands;
           avm.getRandomAttributeValue(it->first, rands );
           it->second.insert(rands);
       }
    }

    /**
     * for range/generic queries
     */
    /*void computeQuerySet(AttributeValueMap & avm, map<uint,vector<uint> > & queryset) const  {
        for (map<uint, set<string> >::const_iterator i = equality_query.begin(); i != equality_query.end(); ++i) {
            for(set<string>::const_iterator k = i->second.begin(); k!= i->second.end(); ++k)
                avm.getBitmapsIndices(i->first, *k, queryset[i->first]);
            if(queryset[i->first].size()==0) queryset.erase(i->first);
        }
        for (map<uint, set<pair<string,string> > >::const_iterator i = range_query.begin(); i != range_query.end(); ++i) {
            for(set<pair<string,string> >::const_iterator k = i->second.begin(); k!= i->second.end(); ++k)
                avm.getBitmapsIndices(i->first, k->first,k->second, queryset[i->first]);
            if(queryset[i->first].size()==0) queryset.erase(i->first);
        }
    } */




};

#endif	/* _QUERYLANGUAGE_H */

