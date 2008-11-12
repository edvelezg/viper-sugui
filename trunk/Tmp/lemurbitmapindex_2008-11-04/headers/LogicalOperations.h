/*
 * File:   LogicalOperations.h
 * Author: lemire
 *
 * Created on August 20, 2008, 12:03 PM
 */

#ifndef _LOGICALOPERATIONS_H
#define	_LOGICALOPERATIONS_H


#include "Util.h"
#include "CompressedDataBlock.h"


/*
References:


Optimizing Queries On Compressed Bitmaps
http://www.vldb.org/conf/2000/P329.pdf

Query optimization for selections using bitmaps
http://portal.acm.org/citation.cfm?id=304181.304202

Optimizing bitmap indices with efficient compression  (see last section of the paper)
http://portal.acm.org/citation.cfm?id=1132863.1132864

*/
enum {
    HINT_NONE, HINT_SPARSE
};



class IntervalQuerySet {
public:
    IntervalQuerySet() : bitmaps(), notbitmaps(), andnots(), ands() {}

    set<uint> bitmaps;
    set<uint> notbitmaps;
    set<pair<uint,uint> > andnots;
    set<pair<uint,uint> > ands;

    void dumpBitmaps(set<uint> &out) {
        out.insert(bitmaps.begin(), bitmaps.end());
        out.insert(notbitmaps.begin(), notbitmaps.end());
        for(set<pair<uint,uint> >::iterator i = andnots.begin(); i != andnots.end(); ++i) {
            out.insert(i->first);
            out.insert(i->second);
        }
        for(set<pair<uint,uint> >::iterator i = ands.begin(); i != ands.end(); ++i) {
            out.insert(i->first);
            out.insert(i->second);
        }
    }

    void describe() {
    	cout<<bitmaps.size()<<" bitmaps"<<endl;
    	cout<<notbitmaps.size()<<" notbitmaps"<<endl;
    	cout<<andnots.size()<<" andnots bitmaps"<<endl;
    	cout<<ands.size()<<" ands"<<endl;
    }

    uint size() const {
        return bitmaps.size() + notbitmaps.size() +2* andnots.size() + 2* ands.size();
    }

    uint containsAnds() const {
        return  andnots.size() + ands.size() > 0;
    }
    template <class uword>
    void process(map<uint, EWAHBoolArray<uword> > & membuffer, EWAHBoolArray<uword> & container);

    template <class uword>
    void processAnds(map<uint, EWAHBoolArray<uword> > & membuffer, EWAHBoolArray<uword> & container) {
        EWAHBoolArray<uword> tmpcontainer,tmpcontainer2;
        container.reset();
        const bool debug(false);
        for(set<pair<uint,uint> >::iterator i = ands.begin(); i!= ands.end(); ++i) {
        	if(debug)cout<<"computing AND betwee "<<i->first<<" and "<<i->second<<endl;
            EWAHBoolArray<uword> & bitmap1(membuffer[i->first]);
            EWAHBoolArray<uword> & bitmap2(membuffer[i->second]);
            bitmap1.rawlogicaland(bitmap2,tmpcontainer);
            if(container.sizeInBits() == 0)
                    container.swap(tmpcontainer);
            else {
                container.rawlogicalor(tmpcontainer,tmpcontainer2);
                tmpcontainer2.swap(container);
            }
        }
        for(set<pair<uint,uint> >::iterator i = andnots.begin(); i!= andnots.end(); ++i) {
            EWAHBoolArray<uword> & bitmap1(membuffer[i->first]);
            EWAHBoolArray<uword> & bitmap2(membuffer[i->second]);
            bitmap2.logicalnot(tmpcontainer2);
            bitmap1.rawlogicaland(tmpcontainer2,tmpcontainer);
            if(container.sizeInBits() == 0)
                    container.swap(tmpcontainer);
            else {
                container.rawlogicalor(tmpcontainer,tmpcontainer2);
                tmpcontainer2.swap(container);
            }
        }
    }

};




/**
 *This is the special case used by equality queries.
 */
template <class uword>
void logicaland(map<uint, EWAHBoolArray<uword> > & membuffer, const vector<uint> & queryset, EWAHBoolArray<uword> & container, const int hint = HINT_NONE) {
    if (queryset.size() < 2)
        throw noneedforlogicaloperation_exception("please, do not try to AND a bitmap with itself");
    EWAHBoolArray<uword> tmpcontainer;
    if (hint == HINT_SPARSE) {
        membuffer[queryset[0]].sparselogicaland(membuffer[queryset[1]], container);
        for (uint k = 2; k < queryset.size(); ++k) {
            container.sparselogicaland(membuffer[queryset[k]], tmpcontainer);
            container.swap(tmpcontainer);
        }
    } else {
        membuffer[queryset[0]].rawlogicaland(membuffer[queryset[1]], container);
        for (uint k = 2; k < queryset.size(); ++k) {
            container.rawlogicaland(membuffer[queryset[k]], tmpcontainer);
            container.swap(tmpcontainer);
        }
    }
};
/*
template <class uword>
void logicaland(const vector<EWAHBoolArray<uword> > & queryset, EWAHBoolArray<uword> & container, const int hint = HINT_NONE) {
    if (queryset.size() < 2)
        throw noneedforlogicaloperation_exception("please, do not try to AND a bitmap with itself");
    EWAHBoolArray<uword> tmpcontainer;
    if (hint == HINT_SPARSE) {
        queryset[0].sparselogicaland(queryset[1], container);
        for (uint k = 2; k < queryset.size(); ++k) {
            container.sparselogicaland(queryset[k], tmpcontainer);
            container.swap(tmpcontainer);
        }
    } else {
        queryset[0].rawlogicaland(queryset[1], container);
        for (uint k = 2; k < queryset.size(); ++k) {
            container.rawlogicaland(queryset[k], tmpcontainer);
            container.swap(tmpcontainer);
        }
    }
};*/


namespace __logicalor {

/**
 *
 */
/*
template <class uword>
void logicalor(map<uint, EWAHBoolArray<uword> > & membuffer, const vector<uint> & queryset, EWAHBoolArray<uword> & container, const int hint = HINT_NONE) {
    if (queryset.size() < 2)
        if(queryset.size()==1) {
            membuffer[queryset[0]].swap(container);
            return;
        } else throw noneedforlogicaloperation_exception("nothing to do");
    EWAHBoolArray<uword> tmpcontainer;
    membuffer[queryset[0]].rawlogicalor(membuffer[queryset[1]], container);
    for (uint k = 2; k < queryset.size(); ++k) {
            container.rawlogicalor(membuffer[queryset[k]], tmpcontainer);
            container.swap(tmpcontainer);
    }
};*/
template <class uword>
void  logicalor(map<uint, EWAHBoolArray<uword> > & membuffer, const set<uint> & queryset, EWAHBoolArray<uword> & container, const int hint = HINT_NONE) {
	const bool debug(false);
	if(debug) {
         	for(set<uint>::iterator it = queryset.begin(); it!=queryset.end();++it)
         		cout <<" OR ( "<<*it<<" ) ";
             cout<<endl;
    }
	if (queryset.size() < 2)
        if(queryset.size()==1) {
            membuffer[*queryset.begin()].swap(container);
            return;
        } else throw noneedforlogicaloperation_exception("nothing to do");
	EWAHBoolArray<uword> tmpcontainer;
	set<uint>::iterator i (queryset.begin());
	const uint firstthing = *i;
	const uint secondthing = *(++i);
    //if(debug)cout<<" or of "<<*i<< " ("<< countRowIDs(membuffer[*i])<< ") and ";
    membuffer[firstthing].rawlogicalor(membuffer[secondthing], container);
    if(debug) {
    	cout<<firstthing<<" ("<< countRowIDs(membuffer[firstthing])<< ")"<<endl;
    	cout<<secondthing<<" ("<< countRowIDs(membuffer[secondthing])<< ")"<<endl;
    	cout<<"total ("<< countRowIDs(container)<< ")"<<endl;
    	BoolArray<uword> xxxcontainer;
    	membuffer[firstthing].toBoolArray().logicalor(membuffer[secondthing].toBoolArray(), xxxcontainer);
    	cout<<"slow total ("<< countRowIDs(xxxcontainer)<< ")"<<endl;
    }
    //cout<<*i<<" ("<< countRowIDs(membuffer[*i])<< ")"<<endl;
    //cout<<"count is now "<<countRowIDs(*container)<<endl;

    while(++i!=queryset.end()) {
    	if(debug)cout<<" or  "<<*i<<endl;
            container.rawlogicalor(membuffer[*i], tmpcontainer);
            container.swap(tmpcontainer);
    }
};

/*
template <class uword>
void  logicalor(map<uint, EWAHBoolArray<uword> > & membuffer, IntervalQuerySet & queryset, EWAHBoolArray<uword> & container, const int hint = HINT_NONE) {
    const bool debug(false);
    if(debug) cout<<"[debug]Êprocessing a logicalor query with a IntervalQuerySet"<<endl;
	if (queryset.size() < 2)
        if(queryset.size()==1) {
        	if(queryset.bitmaps.size()==1)
        		membuffer[*queryset.bitmaps.begin()].swap(container);
        	else
        		membuffer[*queryset.notbitmaps.begin()].logicalnot(container);
            return;
        } else throw noneedforlogicaloperation_exception("nothing to do");
    EWAHBoolArray<uword> tmpcontainer;
    if(!queryset.containsAnds()) {
    	if(debug) cout<<"[debug]ÊI have a routine collection of bitmaps"<<endl;
    	// we just have a set of bitmaps to OR together
        logicalor(membuffer,queryset.bitmaps,container,hint);
        return;
    }
    if(queryset.bitmaps.size()==0) {
    	if(debug) cout<<"[debug]Êcalling queryset.processAnds"<<endl;
    	queryset.processAnds(membuffer,container);
        return;
    }
    else if(queryset.bitmaps.size()==1) {
    	if(debug) cout<<"[debug]Êcalling queryset.processAnds and OR with 1 bitmap"<<endl;
    	EWAHBoolArray<uword> tmpcontainer;
        queryset.processAnds(membuffer,tmpcontainer);
        membuffer[*queryset.bitmaps.begin()].rawlogicalor(tmpcontainer,container);
    } else {
    	if(debug) cout<<"[debug]Êcalling queryset.processAnds and OR with several bitmaps"<<endl;
        EWAHBoolArray<uword> tmpcontainer2;
        logicalor(membuffer,queryset.bitmaps,&tmpcontainer2,hint);
        EWAHBoolArray<uword> tmpcontainer;
        queryset.processAnds(membuffer,tmpcontainer);
        tmpcontainer2.rawlogicalor(tmpcontainer,container);
    }
};
*/


class interval_to_values_iterator {
public:
    interval_to_values_iterator(set<pair<uint,uint> > & in): parent(in), i(parent.begin()),k(i->first) {
        assert(i->first<=i->second);
    }

    uint getValue() const {
        return k;
    }

    bool next() {
        if(k<i->second) {++k;return true;}
        if(++i !=parent.end()) { assert(i->first<=i->second);k=i->first; return true;}
        return false;
    }

    set<pair<uint,uint> > & parent;
    set< pair<uint,uint> >::iterator i;
    uint k;
private:
    interval_to_values_iterator();
    interval_to_values_iterator & operator=(const interval_to_values_iterator & o);
};


template <class uword>
void logicalor(map<uint, EWAHBoolArray<uword> > & membuffer, set<pair<uint,uint> > & queryset, EWAHBoolArray<uword> & container, const int hint = HINT_NONE) {
    EWAHBoolArray<uword> tmpcontainer;
    //bool debug(true);
    //cout<<"logicalor over a set of "<< queryset.size()<<" pairs..."<<endl;
    if(queryset.size()==0) throw noneedforlogicaloperation_exception("nothing to do");
    interval_to_values_iterator i(queryset);
    EWAHBoolArray<uword> & first (membuffer[i.getValue()]);
    if (! i.next()) {
        container.swap(first);
        return;
    }
    assert(first.sizeInBits()>0);
    assert(membuffer[i.getValue()].sizeInBits()>0);
    //throw noneedforlogicaloperation_exception("please, do not try to OR a bitmap with itself");
    first.rawlogicalor(membuffer[i.getValue()], container);
    while(i.next()) {
            assert(container.sizeInBits()>0);
            assert(membuffer[i.getValue()].sizeInBits()>0);
            container.rawlogicalor(membuffer[i.getValue()], tmpcontainer);
            container.swap(tmpcontainer);
    }
}

}


template <class container>
void fromIntervalsToBitmaps(set<pair<uint,uint> > & in, container & out) {
     for(set< pair<uint,uint> >::iterator i = in.begin(); i!=in.end();++i) {
        for(uint k = i->first; k<=i->second;++k)
            out.insert(out.end(),k);
     }
}

/**
 * This is the more general case
 *( ... OR ....) AND (.... OR ....) AND (.... OR ....)
 *it should be sufficient to answer most orthogonal range queries if
 *the bitmap index uses unary encoding or if we have some sensible
 *range-query encoding such as interval coding (Chan-Ionnadis)
 */
template <class uword>
void sloworthogonalquery(map<uint, EWAHBoolArray<uword> > & membuffer,map<uint,set< pair<uint,uint> > > & queryset, EWAHBoolArray<uword> & container, const int hint = HINT_NONE) {
    //vector<EWAHBoolArray<uword> > containers;
	bool debug(false);
	if(debug) cout<<"call to an orthogonalquery"<<endl;
    map<uint,set< pair<uint,uint> > >::iterator p = queryset.begin();
    //if(p->second.size() > 1)
    __logicalor::logicalor(membuffer, p->second, container, hint);
    ++p;
    //else
    //    container = membuffer[*(p->second.begin())];
    EWAHBoolArray<uword> tmpcontainer;
    EWAHBoolArray<uword> localcontainer;
    for (; p!= queryset.end();++p) {
        //if(p->second.size()> 1)
    	__logicalor::logicalor(membuffer, p->second, localcontainer, hint);
        //else
        //   localcontainer = membuffer[*(p->second.begin())];
        if (hint == HINT_SPARSE) {
          localcontainer.sparselogicaland(container, tmpcontainer);
        } else {
          localcontainer.rawlogicaland(container, tmpcontainer);
        }
        container.swap(tmpcontainer);
    }
}

template <class cont, class uword>
void orthogonalquery(map<uint, EWAHBoolArray<uword> > & membuffer,  map<uint,cont> & queryset, EWAHBoolArray<uword> & container, const int hint = HINT_NONE) {
    //vector<EWAHBoolArray<uword> > containers;
	bool debug(false);
	if(debug) cout<<"call to an orthogonalquery"<<endl;
    typename map<uint,cont>::iterator p = queryset.begin();
    //if(p->second.size() > 1)
    p->second.process(membuffer,container);
    //__logicalor::logicalor(membuffer, p->second, container, hint);
    ++p;
    //else
    //    container = membuffer[*(p->second.begin())];
    EWAHBoolArray<uword> tmpcontainer;
    EWAHBoolArray<uword> localcontainer;
    for (; p!= queryset.end();++p) {
        //if(p->second.size()> 1)
    	p->second.process(membuffer,localcontainer);
    	//__logicalor::logicalor(membuffer, p->second, localcontainer, hint);
        //else
        //   localcontainer = membuffer[*(p->second.begin())];
        if (hint == HINT_SPARSE) {
          localcontainer.sparselogicaland(container, tmpcontainer);
        } else {
          localcontainer.rawlogicaland(container, tmpcontainer);
        }
        container.swap(tmpcontainer);
    }
}



template <class uword>
void IntervalQuerySet::process(map<uint, EWAHBoolArray<uword> > & membuffer, EWAHBoolArray<uword> & container) {
    if(containsAnds())
    	processAnds(membuffer,container);
	if(bitmaps.size()>0) {
		EWAHBoolArray<uword>  tmpcontainer;
		__logicalor::logicalor(membuffer,bitmaps,tmpcontainer);
		if(container.sizeInBits()>0) {
			EWAHBoolArray<uword>  tmpcontainer2;
			container.rawlogicalor(tmpcontainer,tmpcontainer2);
			tmpcontainer2.swap(container);
		} else
			tmpcontainer.swap(container);
	}
	if(notbitmaps.size()>0) {
		EWAHBoolArray<uword>  tmpcontainer;
		__logicalor::logicalor(membuffer,bitmaps,tmpcontainer);
		if(container.sizeInBits()>0) {
			EWAHBoolArray<uword>  tmpcontainer2;
			container.rawlogicalor(tmpcontainer,tmpcontainer2);
			container.swap(tmpcontainer2);
		} else
			tmpcontainer.swap(container);
	}
}




#endif	/* _LOGICALOPERATIONS_H */

