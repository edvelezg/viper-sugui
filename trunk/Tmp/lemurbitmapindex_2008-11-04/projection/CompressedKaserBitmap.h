/*
 * CompressedKaserBitmap.h
 *
 *  Created on: Sep 26, 2008
 *      Author: lemire
 */

#ifndef COMPRESSEDKASERBITMAP_H_
#define COMPRESSEDKASERBITMAP_H_

#include "CompressedDataBlock.h"

/**
 * Owen's idea is to build a bitmap index with the minimal number of bitmaps. For range
 * queries, this might be very good... maybe?
 */



/*********************
 *  This code is probably impossible to parse if you do not see how it can be used with
 *  the code in compressedprojectionindex.h.
 */

template <class valuetype, class uword>
class KaserArray {
public:
	typedef KaserArray<valuetype,uword> ArrayWriter;
	typedef KaserArray<valuetype,uword> ArrayReader;
	static const uint bitsforcounters = sizeof(uword)*8 + 512;// just a code to recognize the format
	static const uint numberofbitmaps = sizeof(valuetype)*8;
	KaserArray() : bitmaps(numberofbitmaps), lastvalues(numberofbitmaps,0), msize(0),count(0) {
		for(typename vector<EWAHBoolArray<uword> >::iterator i = bitmaps.begin(); i!= bitmaps.end(); ++i)
			msize+=i->sizeInBytes();
	}
	KaserArray(const KaserArray & o) : bitmaps(o.bitmaps), lastvalues(o.lastvalues), msize(o.msize), count(o.count) {}

	void add(valuetype newval) {
		for(uint k = 0; k<numberofbitmaps;++k)
			if((newval & (static_cast<valuetype>(1)<<k)) > 0)
				lastvalues[k] |= static_cast<uword>(1) << (count % (sizeof(uword)*8));
		++count;
		if(count % (sizeof(uword)*8) == 0 ) {
			flush();
		}
	}

	void flush() {
		for(uint k = 0; k<numberofbitmaps;++k) {
			//if(slowsize()!= msize){cerr<<"problem1 : "<<slowsize()<<" "<<msize<<endl;throw "abort";}
			//if(count % (sizeof(uword)*8) == 0 )
			//cout<<"bits that count "<<count - bitmaps[k].sizeInBits()<< " "<<count << " "<<bitmaps[k].sizeInBits()<<endl;
			//cout<<k<<" "<<lastvalues[k]<<endl;
			  msize+=sizeof(uword)*bitmaps[k].add(lastvalues[k],count - bitmaps[k].sizeInBits() );
			//if(slowsize()!= msize){cerr<<"problem2 : "<<slowsize()<<" "<<msize<< " "<< lastvalues[k] << " "<< (sizeof(uword)*8  - (count % (sizeof(uword)*8))) <<endl;throw "abort";}
		}
		memset(& lastvalues[0], 0, lastvalues.size()*sizeof(uword));
	}
	// size on disk in bytes
	uint size() const {
		//if(slowsize()!= msize){cerr<<"problem : "<<slowsize()<<" "<<msize<<endl;throw "abort";}
		return msize + 2*sizeof(uint)*bitmaps.size();
	}

	// for debugging purposes
	uint slowsize() const {
		uint longs(0);
		for(typename vector<EWAHBoolArray<uword> >::const_iterator i = bitmaps.begin(); i!= bitmaps.end(); ++i)
			longs +=i->sizeInBytes();
		return longs;
	}

	void write(ostream & out) {
		for(typename vector<EWAHBoolArray<uword> >::iterator i = bitmaps.begin(); i!= bitmaps.end(); ++i) {
			//cerr<<i->sizeInBytes()<<" at "<<out.tellp()<<endl;
			i->write(out,true);
		}
	}
	void read(istream & in,uint s) {
			msize = 0;
			for(typename vector<EWAHBoolArray<uword> >::iterator i = bitmaps.begin(); i!= bitmaps.end(); ++i) {
				i->read(in,true);
				//cerr<<i->sizeInBytes()<<" at"<<in.tellg() <<endl;
				msize += i->sizeInBytes();
			}
			//if(s!= msize) cerr<<"mismatch on the reported size, should be "<<s<<" but loaded "<<msize<<" bytes"<<endl;
			count = bitmaps.begin()->sizeInBits();
	}

	template <class filterclass>
	void filter(const filterclass & fc, EWAHBoolArray<uword> &out) {
		fc(bitmaps,out);
	}

	typedef EWAHBoolArray<uword> RowArray;

	vector<EWAHBoolArray<uword> > bitmaps;
	vector<uword> lastvalues;
	uint msize;
	uint count;


private:
	KaserArray & operator=(const KaserArray<valuetype,uword> & c);

};

#endif /* COMPRESSEDKASERBITMAP_H_ */
