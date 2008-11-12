#ifndef DATABLOCK_H_
#define DATABLOCK_H_
#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <cassert>
//#include <assert.h>
#include "constants.h"
#include "Util.h"

using namespace std;


/**
 * After wasting an incredible amount of time, I gave up on finding a
 * sensible dynamic bitset implementation. All you have is the Boost
 * version, but I think these guys make a point of making their code
 * difficult to parse and difficult to modify.
 *
 * This is a helper class to DataBlock (see below).
 *  (Though it could standalone somewhat.)
 */

template <class uword=uword32>
class BoolArray {
	public:
		// how many bits?
		BoolArray(const uint n, const uword initval= 0):buffer(n / wordinbits + (n % wordinbits == 0 ? 0 : 1),initval),sizeinbits(n){	}
		BoolArray():buffer(),sizeinbits(0){}
		BoolArray(const BoolArray & ba) : buffer(ba.buffer),sizeinbits(ba.sizeinbits){}
		void read(istream & in){
			//uint sizeinbits;
			sizeinbits = 0;
			in.read(reinterpret_cast<char *>(&sizeinbits), sizeof(sizeinbits));
			if(verboseIO) cout << "loading "<< sizeinbits << " bits" <<endl;
			buffer.resize(sizeinbits / wordinbits + (sizeinbits % wordinbits == 0 ? 0 : 1));
			if(verboseIO) cout << "using "<< buffer.size() << " words" <<endl;
			in.read(reinterpret_cast<char *>(&buffer[0]),buffer.size()*sizeof(uword));
		}
		void readBuffer(istream & in,const uint size){
			//uint sizeinbits;
			//in.read(reinterpret_cast<char *>(&sizeinbits), sizeof(sizeinbits));
			//if(verboseIO) cout << "loading "<< sizeinbits << " bits" <<endl;
			buffer.resize(size);
			//if(verboseIO) cout << "using "<< buffer.size() << " words" <<endl;
			in.read(reinterpret_cast<char *>(&buffer[0]),buffer.size()*sizeof(uword));
			sizeinbits = size*sizeof(uword)*8;
		}

		void setSizeInBits(const uint sizeib) {
			sizeinbits = sizeib;
		}


		void write(ostream & out) {
			write(out,sizeinbits);
		}

		void write(ostream & out, const uint numberofbits) const {
			if(verboseIO) cout << "dumping "<< numberofbits << " bits" <<endl;
	   		const uint size = numberofbits/wordinbits + (numberofbits%wordinbits == 0 ? 0: 1);
	   		if(verboseIO) cout << "using "<< size << " words" <<endl;
	   		out.write(reinterpret_cast<const char *>(&numberofbits), sizeof(numberofbits));
	   		out.write(reinterpret_cast<const char *>(&buffer[0]),size*sizeof(uword));
		}

		void writeBuffer(ostream & out, const uint numberofbits) const {
					if(verboseIO) cout << "dumping "<< numberofbits << " bits" <<endl;
			   		const uint size = numberofbits/wordinbits + (numberofbits%wordinbits == 0 ? 0: 1);
			   		if(verboseIO) cout << "using "<< size << " words" <<endl;
			   		//out.write(reinterpret_cast<const char *>(&numberofbits), sizeof(numberofbits));
			   		out.write(reinterpret_cast<const char *>(&buffer[0]),size*sizeof(uword));
		}

		uint sizeOnDisk() const {
			uint size = sizeinbits/wordinbits + (sizeinbits%wordinbits == 0 ? 0: 1);
			return sizeof(sizeinbits) + size*sizeof(uword);
		}


		BoolArray& operator=(const BoolArray & x) {
			this->buffer = x.buffer;
			return *this;
		}

		bool operator==(const BoolArray & x) const {
			if(sizeinbits != x.sizeinbits) return false;
			assert(buffer.size() == x.buffer.size());
			for(uint k = 0; k < buffer.size(); ++k)
			if(buffer[k] != x.buffer[k]) return false;
			return true;
		}

		bool operator!=(const BoolArray & x) const {
			return ! operator==(x);
		}

		void setWord(const uint pos, const uword val) {
			assert(pos < buffer.size());
			buffer[pos] = val;
		}

		void add(const uword val){
			if(sizeinbits % wordinbits != 0) throw invalid_argument("you probably didn't want to do this");
			sizeinbits += wordinbits;
			buffer.push_back(val);
		}

		uword getWord(const uint pos) const {
			assert(pos < buffer.size());
			return buffer[pos];
		}

		/**
		 * set to true (whether it was already set to true or not)
		 *
		 * TODO this is an expensive (random access) API, you really ought to
		 * prepare a new word and then append it.
		 */
		void set(const uint pos) {
			//assert(pos/wordinbits < buffer.size());
			//cout <<"b pos= "<<pos<< "pos % wordinbits= "<<(pos% wordinbits) <<" "  << buffer[pos/wordinbits] << endl;
			buffer[pos/wordinbits] |= ( static_cast<uword>(1) << (pos % wordinbits) ) ;
			//cout <<"a pos= "<<pos<< "pos % wordinbits= "<<(pos% wordinbits) <<" "  << buffer[pos/wordinbits] << endl;
			//assert(get(pos));
		}//(buffer[pos/wordinbits] & ( 1 << (pos % wordinbits) )) != 0;

		/**
		 * set to false (whether it was already set to false or not)
		 *
		 * TODO this is an expensive (random access) API, you really ought to
		 * prepare a new word and then append it.
		 */
		void unset(const uint pos) {
			//assert(pos/wordinbits < buffer.size());
			buffer[pos/wordinbits] |= ~( static_cast<uword>(1) << (pos % wordinbits) ) ;
	    	//assert(!get(pos));
		}

		/**
		 * true of false? (set or unset)
		 *
		 * TODO this is an expensive (random access) API, you really ought to
		 * proceed word-by-word
		 */
		bool get(const ulong pos) const {
			assert(pos/wordinbits < buffer.size());
			//cout << buffer[pos/wordinbits]<< " "<<(buffer[pos/wordinbits] & ( static_cast<uword>(1) << (pos % wordinbits) ))<<endl;
			return (buffer[pos/wordinbits] & ( static_cast<uword>(1) << (pos % wordinbits) )) != 0;
		}

		/**
		 * set all bits to 0
		 */
		void reset() {
			memset(&buffer[0],0,sizeof(uword)*buffer.size());
			sizeinbits = 0;
		}

		//inline uint size() {return buffer.size() * sizeof(uint);}
		uint sizeInBits() const {return sizeinbits; }
		~BoolArray() {}

		void logicaland(const BoolArray & ba, BoolArray & out);
		void logicalor(const BoolArray & ba, BoolArray & out);

		void appendRowIDs(vector<ulong> & answer, const uint offset = 0) const;


		inline void printout(ostream &o = cout) {
			for(uint k = 0; k < sizeinbits; ++k)
				o << get(k) << " ";
			o << endl;
		}

		void append(const BoolArray & a);

		enum { wordinbits =  sizeof(uword) * 8, verboseIO=false};
  	private:
		vector<uword>  buffer;
		uint sizeinbits;
};

/**
 * This is used as a buffer to write (and read?) data to disk.
 * Ultimately, this should be implemented very efficiently...
 *
 *
 * Usage: To create the following bitmap
 *
 *  0100
 *  0001
 *  1000
 *
 * Just do
 *
 * DataBlock db(4);
 * db.newRow();
 * db.set(1);
 * db.newRow();
 * db.set(4);
 * db.newRow();
 * db.set(0);
 *
 * Then you can save the result with an emptyToDisk call.
 *
 * This is a helper class for BitmapWriter (though it could standalone somewhat).
 */

template <class uword=uword32>
class DataBlock{
	public:
		/**
		* blocksize is how large the data block is allowed to get (in bytes).
		* (WARNING: Currently, this is not quite true. It could be that the code will
		* use more or less than the allocated memory budget. In most realistic cases,
		* this is a good estimate of the max.  memory used though.)
		* The object is allowed to use that much memory even if no data has been entered.
		* Note that because derived classes may implement compression, it is not
		* immediately obvious how many rows a given blocksize can cope with.
		*/
		DataBlock(int numberofbitmaps, int blocksize);// = 33554432

    static inline string name() {return "DataBlock";}
		/**
		* default constructor, for convenience.
		*/
		DataBlock();
		/**
		* have we reached or exceeded the blocksize in memory usage?
		*/
		inline bool full();

		/**
		* (used to write new data)
		* Passing a new row to the data block (may result is some memory
		* allocation)
		*/
		inline void newRow();

		/**
		* (used to write new data)
		* set the current bit of the corresponding bitmap to 1 (default is 0)
		*/
		inline void set(int whichbitmap);


		/**
		* write data to disk and empty the buffer
		*/
		void emptyToDisk(ostream& out, bool verbose = false);

		/**
		* return the value of the given bitmap at the given row.
		* (This method is potentially unsafe: you have to know
		* whether this row and bitmap exists, no checks are made.)
		*/
		bool getBit(int row, int bitmap);

		inline BoolArray<uword> & getBitmap(int bitmap) {
			return membuffer.at(bitmap);
		}

		/**
		* do the logical and of the
		* columns indexes contained in the query set.
		*/
		BoolArray<uword> logicaland(const vector<uint> & queryset);

		/**
		* return true if no data was entered.
		*/
		bool empty();

		virtual ~DataBlock();

		/**
		* would not be very useful if we couldn't retrieve the data, right?
		*/
		void readAllFromDisk(istream & in);

		/*
		* read selectively only some bitmaps
		*/
		void readFromDisk(istream & in,vector<uint> & whichbitmaps);
		bool haveBeenLoaded(const vector<uint> & queryset) ;

		int getNumberOfRows() {return RowCounter+1;}
		int getNumberOfBitmaps() {return mNumberOfBitmaps;}
                void printout( ostream &o=cout);

		enum{versionID=2,Cookie=0xABCD,compressiontype=UNCOMPRESS,verboseread=false,verbosewrite=false,vverboseread=false,vverbosewrite=false, asserts=true};
		vector<BoolArray<uword> > & getBuffer() {return membuffer;}
	private:
		int mNumberOfBitmaps, mBlockSize;
		vector<BoolArray<uword> > membuffer;
		int RowCounter;
};


template <class uword>
bool DataBlock<uword>::full() {
	/*if(membuffer[0].sizeInBits() <= (uint) RowCounter + 1) {
		cout << "I am full with "<< membuffer[0].sizeInBits()<< endl;
	}*/
	return membuffer[0].sizeInBits() <= static_cast<uint>( RowCounter ) + 1;
}


template <class uword>
void DataBlock<uword>::newRow(){
	++RowCounter;
}


template <class uword>
void DataBlock<uword>::set(int whichbitmap){
	assert(whichbitmap >= 0);
	assert(whichbitmap < mNumberOfBitmaps);
	assert(RowCounter >= 0);
	membuffer[whichbitmap].set(RowCounter);
}


/***
 * Rest of this file used to be in DataBlock.cpp before we had to
 * use template to enable switching between 64 bit and 32 bits
 */


template <class uword>
void BoolArray<uword>::append(const BoolArray & a) {
			if(sizeinbits % wordinbits == 0) {
				buffer.insert(buffer.end(),a.buffer.begin(),a.buffer.end());
			} else {
				// we have to work harder?
				throw invalid_argument("I am a lazy bum, please make sure this never happens");
			}
			sizeinbits += a.sizeinbits;
}

template <class uword>
void BoolArray<uword>::appendRowIDs(vector<ulong> & answer, const uint offset) const {
	for(ulong k = 0; k < sizeinbits; ++k) {
		//if(get(k)) cout<<k<<endl;
		if(get(k)) answer.push_back(k+offset);
	}
}


template <class uword>
void BoolArray<uword>::logicaland(const BoolArray & ba, BoolArray & out) {
	assert(buffer.size() == ba.buffer.size());
	out.buffer.resize(buffer.size());
	out.sizeinbits=sizeinbits;
	for(uint k = 0; k < buffer.size(); ++k)
		out.buffer[k]= buffer[k]&ba.buffer[k];
}

template <class uword>
void BoolArray<uword>::logicalor(const BoolArray & ba, BoolArray & out) {
	assert(buffer.size() == ba.buffer.size());
	out.buffer.resize(buffer.size());
	out.sizeinbits=sizeinbits;
	for(uint k = 0; k < buffer.size(); ++k) {
		out.buffer[k]= buffer[k] | ba.buffer[k];
		//cout<<buffer[k]<<" | "<<ba.buffer[k]<<" = "<< out.buffer[k]<<endl;

	}
}


template <class uword>
DataBlock<uword>::DataBlock(int numberofbitmaps, int blocksize) :
	mNumberOfBitmaps(numberofbitmaps), mBlockSize(blocksize),
	membuffer(mNumberOfBitmaps, BoolArray<uword>(mBlockSize/mNumberOfBitmaps*8)),
	RowCounter(-1)
{
	//cout << "I was given a budget of "<<mBlockSize<<"bytes"<<endl;
	//cout << "I splitted it in columns having "<<(mBlockSize*8/mNumberOfBitmaps)<<"bits"<<endl;
   for(uint k = 0; k < membuffer.size();++k) {
	   assert(membuffer[k].sizeInBits() == static_cast<uint>(mBlockSize/mNumberOfBitmaps *8));
   }
}

template <class uword>
DataBlock<uword>::DataBlock() :
	mNumberOfBitmaps(0), mBlockSize(0), membuffer(0),
	RowCounter(0) {
}

template <class uword>
DataBlock<uword>::~DataBlock()
{
}



template <class uword>
bool DataBlock<uword>::getBit(int row, int bitmap) {
	assert(row <= RowCounter);
	assert(bitmap < mNumberOfBitmaps);
	return membuffer.at(bitmap).get(row);
}

template <class uword>
bool DataBlock<uword>::empty() {
  return RowCounter == -1;
}

template <class uword>
void DataBlock<uword>::readAllFromDisk(istream & in) {
	//cout<<"reading an uncompressed datablock from disk..."<<endl;
	vector<uint> whichbitmaps;
	for(uint k = 0; k <static_cast<uint>(mNumberOfBitmaps); ++k)
		whichbitmaps.push_back(k);
	readFromDisk(in,whichbitmaps);
}

template <class uword>
void DataBlock<uword>::readFromDisk(istream & in, vector<uint> & whichbitmaps) {
	//cout<<"reading the following bitmaps from disk..."<<endl;
	//::printout(whichbitmaps);
	uint cookie(0), version(0);
	in.read(reinterpret_cast<char *>(&cookie),sizeof(cookie));
	if(cookie != DataBlock::Cookie) {
		//cerr << "bad cookie, aborting DataBlock read" << endl;
		return;
	}
	if(!in) return;
	in.read(reinterpret_cast<char *>(&version),sizeof(version));
	if(version != DataBlock::versionID) {
		cerr << "version mismatch, this may fail" << endl;
	}
	if(!in) return;
	mBlockSize = 0;
	in.read(reinterpret_cast<char *>(&mBlockSize),sizeof(mBlockSize));
	if(!in) return;
	if(verboseread) cout << "[read]  mBlockSize = " << mBlockSize << endl;
	mNumberOfBitmaps = 0;
	in.read(reinterpret_cast<char *>(&mNumberOfBitmaps),sizeof(mNumberOfBitmaps));
	if(!in) return;
	if(mNumberOfBitmaps == 0) {
		cout <<"no bitmaps?"<<endl;
		throw failedtoreaddatablockheader_exception("In CompressedDataBlock::readFromDisk, no bitmaps reported in data block! ");
	}
	if(whichbitmaps.size() == 0)
		for(uint k = 0; k <static_cast<uint>(mNumberOfBitmaps); ++k)
			whichbitmaps.push_back(k);
	if(verboseread) cout << "[read]  mNumberOfBitmaps = " << mNumberOfBitmaps << endl;
	membuffer = vector<BoolArray<uword> >(mNumberOfBitmaps, BoolArray<uword>(mBlockSize/mNumberOfBitmaps));
	uint numberofrows;
	in.read(reinterpret_cast<char *>(&numberofrows),sizeof(numberofrows));
	if(verboseread) cout << "[read]  numberofrows = " << numberofrows << endl;
	//vector<ostream::pos_type> locations(mNumberOfBitmaps+1,0);
	//in.read(reinterpret_cast<char *>(& locations[0]), sizeof(ostream::pos_type)*(mNumberOfBitmaps+1));
	RowCounter = numberofrows - 1;
	ostream::pos_type startpoint = in.tellg();
	ostream::pos_type sizeofabitmap =   sizeof(uword) * (numberofrows / (sizeof(uword)*8) + (numberofrows % (sizeof(uword)*8) == 0 ? 0 : 1));
	for(vector<uint>::iterator k = whichbitmaps.begin(); k!= whichbitmaps.end();++k) {
	  //for(int k = 0; k < mNumberOfBitmaps; ++k) {
      // for this implementation, the size of bitmaps is constant
	  // due to the lack of compression
		//cout << "loading bitmap number "<<*k<<endl;
		//if(locations[*k] != in.tellg())
			in.seekg(startpoint+(*k)*sizeofabitmap);
		//assert(locations[*k] == in.tellg());
		membuffer[*k].readBuffer(in,sizeofabitmap/sizeof(uword));
		//assert(membuffer[*k].sizeOnDisk() == sizeofabitmap);
		assert(in.tellg() == startpoint+((*k) + 1)*sizeofabitmap);
		if(vverboseread) cout<<"[read]  "<<membuffer[*k].sizeInBits()<<" "<<RowCounter<<endl;
		//cout <<RowCounter<< " "<<membuffer[*k].sizeInBits()<<endl;
		membuffer[*k].setSizeInBits(static_cast<uint>( RowCounter) + 1);
		assert(membuffer[*k].sizeInBits() == static_cast<uint>( RowCounter) + 1);
	}
	in.seekg(startpoint+mNumberOfBitmaps*sizeofabitmap);//locations[mNumberOfBitmaps]
}

template <class uword>
void DataBlock<uword>::printout(ostream &o) {
  // or maybe mNumberofBitmaps
	for(uint k = 0; k < membuffer.size(); ++k)
		membuffer[k].printout(o);
}

template <class uword>
void DataBlock<uword>::emptyToDisk(ostream& out, bool verbose){
	// first, we check if any data was entered
	if(empty()) return;
	//cout <<"flushing a new db to disk, number of rows "<<getNumberOfRows()<<endl;
	// save a cookie (for tracking, debugging purposes)
	uint cookie = DataBlock::Cookie;
	out.write(reinterpret_cast<char *>(&cookie),sizeof(cookie));
	// version number, for debugging purposes...
	uint version = DataBlock::versionID;
	out.write(reinterpret_cast<char *>(&version),sizeof(version));
	// save the memory budget (useful for debugging?)
	out.write(reinterpret_cast<char *>(&mBlockSize),sizeof(mBlockSize));
	// save the number of bitmaps
	out.write(reinterpret_cast<char *>(&mNumberOfBitmaps),sizeof(mNumberOfBitmaps));
	// save the number of rows
	uint numberofrows = RowCounter + 1;
	out.write(reinterpret_cast<char *>(& numberofrows), sizeof(numberofrows));
	// next we save the location on disk of the bitmaps
	//ostream::pos_type locations_pos = out.tellp();
	//vector<uint> locations(mNumberOfBitmaps+1,0);
	//out.write(reinterpret_cast<char *>(& locations[0]), sizeof(uint)*(mNumberOfBitmaps+1));
	// for each bitmap, save how many bytes have been used
	// then dump the data
	for(int k = 0; k < mNumberOfBitmaps; ++k) {
      // for this implementation, the size of bitmaps is constant
	  // due to the lack of compression
	  assert(numberofrows >= 1);
	  //locations[k] = out.tellp();
	  membuffer[k].writeBuffer(out,numberofrows);
	  if(vverbosewrite) cout<<"[write]  "<<membuffer[k].sizeInBits()<<" "<<RowCounter<<endl;
	  membuffer[k].reset();
	}
	//locations[mNumberOfBitmaps] = out.tellp();
	if(!out) cerr<<"something went wrong while saving the bitmaps"<<endl;
	//out.seekp(locations_pos);
	//out.write(reinterpret_cast<char *>(& locations[0]), sizeof(ostream::pos_type)*(mNumberOfBitmaps+1));
	//out.seekp(locations[mNumberOfBitmaps]);
	if(!out) cerr<<"I didn't manage to go back in the stream to write the positions!"<<endl;
	//for(uint k = 0; k < membuffer.size(); ++k) membuffer[k].reset();
	RowCounter = -1;
}

template <class uword>
bool DataBlock<uword>::haveBeenLoaded(const vector<uint> & queryset) {
	if(queryset.size() == 0) return true;
	uint expectedsize = membuffer[queryset[0]].sizeInBits();
	if(expectedsize == 0)
		return false;
	for(uint k = 1; k < queryset.size(); ++k) {
			if(membuffer[queryset[k]].sizeInBits() != expectedsize)
				return false;
	}
	return true;
}


template <class uword>
BoolArray<uword>  DataBlock<uword>::logicaland(const vector<uint> & queryset) {
	//cout << "call to logical and"<<queryset.size()<<endl;
	//::printout(queryset);
	if(queryset.size() == 0) {
		cout << "warning: did you mean to send an empty query?" << endl;
		return BoolArray<uword>();
	}
	assert(haveBeenLoaded(queryset));
	if(queryset.size() == 1)// no need to do any "and"
		return membuffer[queryset[0]];
	BoolArray<uword> answer(membuffer[queryset[0]]);
    for(uint k = 1; k < queryset.size(); ++k) {
		answer.logicaland(membuffer[queryset[k]]);
	}
    /*for(uint k = 0; k < membuffer[queryset[0]].sizeInBits(); ++k)
    	cout << membuffer[queryset[0]].get(k) << " ";
    cout << endl;*/
	return answer;
}


#endif /*DATABLOCK_H_*/
