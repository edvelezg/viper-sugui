/*
 * RLEUtil.h
 *
 *  Created on: Sep 29, 2008
 *      Author: lemire
 */

#ifndef RLEUTIL_H_
#define RLEUTIL_H_

#include<vector>
using namespace std;




template <class valuetype, class countertype>
class RLEArrayWriter {
public:
	RLEArrayWriter() : lastvalue(0), currentcounter(0), container() {}
	RLEArrayWriter(const RLEArrayWriter & o) : lastvalue(o.lastvalue), currentcounter(o.currentcounter), container(o.container) {}

	void add(valuetype newval) {
		//cout <<"receiving value " <<newval<<" last value was "<<lastvalue<<" currentcounter="<<static_cast<uint>(currentcounter)<<" / "<<static_cast<uint>(largestcount)<<endl;
		if((newval == lastvalue) and (currentcounter>0)) {
			///cout<<"I have a repetition!"<<endl;
			if(currentcounter == 1) {
				++currentcounter;
				appendValue();
			} else if(currentcounter == largestcount) {
				flush();
				++currentcounter;
				appendValue();
			} else {
				++currentcounter;
			}
		} else {
			//cout<<"this is a new value, last value was "<<lastvalue<<endl;
			flush();
			++currentcounter;
			lastvalue = newval;
			appendValue();
		}
	}

	void write(ostream & out) {
	  	out.write(reinterpret_cast<const char *> (&container[0]),  size());
	}

	static const countertype largestcount = ~static_cast<countertype>(0);

	/**
	 * Write any counter value you were holding on to
	 */
	void flush() {
			if(currentcounter>1)
				writecountervalue();
			currentcounter = 0;
	}
	uint size() const {return container.size();}

	valuetype lastvalue;
	countertype currentcounter;
	vector<char> container;

private:
	RLEArrayWriter & operator=(const RLEArrayWriter<valuetype,countertype> & c);
	void appendValue() {
		//cout <<"appending value " <<lastvalue<<endl;
		const uint s = container.size();
		container.resize(s+sizeof(valuetype));
		memcpy(&container[s],&lastvalue,sizeof(valuetype));
	}
	void writecountervalue() {
		//cout <<"appending counter " <<currentcounter<<endl;
		const uint s = container.size();
		container.resize(s+sizeof(countertype));
		memcpy(&container[s],&currentcounter,sizeof(countertype));
	}

};

class RLERowArray {
public:
	RLERowArray() : ranges() {}
	void swap(RLERowArray & o) {ranges.swap(o.ranges);}

	void add(uint startingrow, uint endingrow){
		if(ranges.size()>0) {
			if(ranges[ranges.size()-1].second> startingrow) throw "should not happen";
			if(ranges[ranges.size()-1].second == startingrow) {
				// merging
				ranges[ranges.size()-1].second=endingrow;
				return;
			}
		}
		ranges.push_back(pair<uint,uint>(startingrow, endingrow));
	}

	void appendRowIDs(vector<ulong> & out, const uint offset = 0) {
		for(vector<pair<uint,uint> >::const_iterator i = ranges.begin(); i!= ranges.end(); ++i)
			for(uint k = i->first; k<= i->second; ++k) out.push_back(offset+k);
	}

	void intersection(RLERowArray & other, RLERowArray & out) {
		vector<pair<uint,uint> >::const_iterator i =ranges.begin();
		vector<pair<uint,uint> >::const_iterator j =other.ranges.begin();
		while( j!=other.ranges.end()) {
			while((i!= ranges.end()) and (i->second <= j->second) ) {
				if(i->second >=j->first) {
					out.add(max(i->first,j->first),i->second);
				}
				++i;
			}
			if(i == ranges.end()) break;
			while(( j!=other.ranges.end()) and (j->second <= i->second)  ) {
				if(j->second >=i->first) {
					out.add(max(j->first,i->first),j->second);
				}
				++j;
			}
		}
	}
	vector<pair<uint,uint> > ranges;

private:
	RLERowArray & operator=(const RLERowArray & o);
	uint max(uint x, uint y) {
		return x > y ? x : y;
	}
};

template <class valuetype, class countertype>
class RLEArrayReader {
public:
	typedef RLERowArray RowArray;

	RLEArrayReader(const vector<char> & c) : container(c), pos(0),value(0),value2(0),count(0), mNext(false),row(0){
		init();
	}


	RLEArrayReader(const RLEArrayReader<valuetype,countertype> & c) : container(c.container), pos(c.pos),value(c.value),value2(c.value2),count(c.count), mNext(c.mNext),row(c.row){
		//container.swap(c.container);
		//init();
	}


	void init() {
		if(pos<container.size()) {
			memcpy(&value2,&container[pos],sizeof(valuetype));
			pos+=sizeof(valuetype);
			mNext = true;
		} else
			mNext =false;
	}

	void read(istream & in, const uint length) {
		container.resize(length);
		in.read(reinterpret_cast<char *> (&container[0]), container.size());
		init();
	}

	RLEArrayReader() : container(), pos(0),value(0),value2(0),count(0), mNext(false),row(0) {

	}

	template <class filterclass>
	void filter(const filterclass & fc, RLERowArray &out) {
		while(hasNext()) {
			next();
			if(fc(value))
				out.add(row,row+count-1);
		}
	}


	//RLEArrayReader(const RLEArrayReader<valuetype,countertype> & o) : container(o.container), pos(o.pos),value(o.value),value2(o.value2),count(o.count), mNext(o.mNext),row(o.row){}

	bool hasNext() const {
		return mNext;
		//return(pos<=container.size());
	}

	/**
	 * at the end of the call, the value is in value, and the number of times
	 * it has been repeated is in count.
	 */
	void next() {
		row += count;
		value=value2;
		if(pos<container.size()) {
		  memcpy(&value2,&container[pos],sizeof(valuetype));
		  pos+=sizeof(valuetype);
		  if(value2==value) {
			  // we need to read a counter
			  memcpy(&count,&container[pos],sizeof(countertype));
			  pos+=sizeof(countertype);
			  if(pos<container.size()) {
				memcpy(&value2,&container[pos],sizeof(valuetype));
				pos+=sizeof(valuetype);
			    mNext = true;
			  } else
				mNext = false;
		  } else {
			  count = 1;
			  mNext=true;
		  }
		} else {
			count=1;
			mNext = false;
		}
	}
	vector<char> container;
	uint pos;
	valuetype value,value2;
	countertype count;
	bool mNext;
	uint row;

private:
	RLEArrayReader & operator=(const RLEArrayReader<valuetype,countertype> & c);
};




template <class valuetype, class countertype>
class RLE {
public:
	typedef RLEArrayWriter<valuetype,countertype> ArrayWriter;
	typedef RLEArrayReader<valuetype,countertype> ArrayReader;
	static const uint bitsforcounters = sizeof(countertype)*8;

};





#endif /* RLEUTIL_H_ */
