/*
 * queryfilter.h
 *
 *  Created on: Oct 10, 2008
 *      Author: lemire
 */

#ifndef QUERYFILTER_H_
#define QUERYFILTER_H_

#include "Util.h"
#include "FastLogicalAndOperations.h"

class IsEqualToOneValue {
public:

	enum {TWOBYTWO, TWOBYTWOREVERSE,BIGAND};
	IsEqualToOneValue(const uint v): testvalue(v) {}
	bool operator()  (uint value) const {
		return testvalue == value;
	}
	template <class uword>
	void operator() (vector <EWAHBoolArray<uword> > & bitmaps,EWAHBoolArray<uword>&out, const int hint = /*BIGAND*/TWOBYTWOREVERSE) const  {
		if(hint == TWOBYTWOREVERSE)
			__aggregate_two_by_two_reverse(bitmaps,out);
		else if(hint == TWOBYTWOREVERSE)
			__aggregate_two_by_two(bitmaps,out);
		else if(hint == BIGAND) {
			vector<bool> b;
			for(uint k = 0; k<bitmaps.size();++k)
				b.push_back((testvalue & (1<<k)) !=0);
			BigAnd<uword> ba(bitmaps,b,out);
			ba.process();
		}
	}

	template <class uword>
	void __aggregate_two_by_two(vector <EWAHBoolArray<uword> > & bitmaps,EWAHBoolArray<uword>&out) const {
		uint k = 0;
		//cout<<"k="<<k<<endl;
		//bitmaps[k].debugprintout();
		if((testvalue & (1<<k))!=0) {
			bitmaps[k].swap(out);
		} else {
			bitmaps[k].logicalnot(out);
		}
		EWAHBoolArray<uword> tmp,tmp2;
		++k;
		for(; k<bitmaps.size(); ++k) {
			//cout<<"k="<<k<<endl;
			//bitmaps[k].debugprintout();
			if((testvalue & (1<<k))!=0) {
				bitmaps[k].rawlogicaland(out,tmp);
				out.swap(tmp);
			} else {
				bitmaps[k].logicalnot(tmp2);
				tmp2.rawlogicaland(out,tmp);
				out.swap(tmp);
			}
		}
	}

	template <class uword>
	void __aggregate_two_by_two_reverse(vector <EWAHBoolArray<uword> > & bitmaps,EWAHBoolArray<uword>&out) const {
			int k = bitmaps.size()-1;
			//cout<<"k="<<k<<endl;
			//bitmaps[k].debugprintout();
			if((testvalue & (1<<k))!=0) {
				bitmaps[k].swap(out);
			} else {
				bitmaps[k].logicalnot(out);
			}
			EWAHBoolArray<uword> tmp,tmp2;
			--k;
			for(; k>=0; --k) {
				//cout<<"k="<<k<<endl;
				//bitmaps[k].debugprintout();
				if((testvalue & (1<<k))!=0) {
					bitmaps[k].rawlogicaland(out,tmp);
					out.swap(tmp);
				} else {
					bitmaps[k].logicalnot(tmp2);
					tmp2.rawlogicaland(out,tmp);
					out.swap(tmp);
				}
			}
	}
	uint  testvalue;
};

class IsEqualToAtLeastOneValue {
public:
	IsEqualToAtLeastOneValue(const set<uint> & v): values(v) {}
	bool operator() (uint value) const  {
		return values.find(value) != values.end();
	}
	template <class uword>
	void operator() (vector <EWAHBoolArray<uword> > & bitmaps,EWAHBoolArray<uword>&out) const  {
		cerr<<"not yet implemented"<<endl;
		throw "bailing out";
	}

	set<uint>  values;
};

class BelongsToRange {
public:
	BelongsToRange(const pair<uint, uint> & r): ranges(r) {}
	bool operator()  (uint value)  const {
		return ((value>= ranges.first) and (value<= ranges.second));
	}
	template <class uword>
	void operator() (vector <EWAHBoolArray<uword> > & bitmaps,EWAHBoolArray<uword>&out) const  {
		cerr<<"not yet implemented"<<endl;
		throw "bailing out";
	}
	pair<uint, uint>  ranges;
};

class BelongsToAtLeastOneRange {
public:
	BelongsToAtLeastOneRange(const set<pair<uint, uint> > & r) : ranges(r) {}
	bool operator()  (uint value) const{
   		for(set<pair<uint, uint> >::iterator pi= ranges.begin(); pi!=ranges.end();++pi)
        			if( (pi->first<=value)
        					and (value<=pi->second))
        				return true;
   		return false;
	}
	template <class uword>
	void operator() (vector <EWAHBoolArray<uword> > & bitmaps,EWAHBoolArray<uword>&out) const  {
		cerr<<"not yet implemented"<<endl;
		throw "bailing out";
	}
	set<pair<uint, uint> > ranges;
};


#endif /* QUERYFILTER_H_ */
