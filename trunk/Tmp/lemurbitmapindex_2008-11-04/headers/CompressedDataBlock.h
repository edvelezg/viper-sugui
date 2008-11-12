#ifndef COMPRESSEDDATABLOCK_H_
#define COMPRESSEDDATABLOCK_H_



/***
 * WAH by Wu et al. is a nice running length encoding.
 * The core idea is that you should manipulate words, not bits
 * when compressing/uncompressing.
 *
 * With Wu et al.'s approach, if you words have length w, then
 * you use pseudo-words of w-1 bits. With what we implemented here,
 * we work directly with words, in a way similar to BBC.
 *
 * This variant is "enhanced" because, provably, in the worst case, you have
 * zero compression (you only add one word per 2**15 or 2**31 words),
 *  whereeas with WAH,
 *  in the worst case, you get an
 * expansion of the data by w/(w-1) which represents 1.6\% if you have 64~bits words.
 *
 */

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "bitmapexceptions.h"
#include "DataBlock.h"
#include "Util.h"
#include "constants.h"
using namespace std;


/**
 * This is used below in the Enhanced Word Aligned  Hybrid (EWAH)
 */
template <class uword=uword32>
class RunningLengthWord {
	public:
		RunningLengthWord (uword & data) : mydata(data) {
			// these should disappear when the compiler goes over them (?)
			if(sizeof(ushort)!=2) cerr << "bad ushort" << endl;
			if(sizeof(uword)<2) cerr << "bad uword" << endl;
			assert( (largestliteralcount<<(1+runninglengthbits))>>(1+runninglengthbits) ==  largestliteralcount);
			assert(static_cast<uword>(~(1+ (largestliteralcount<<(1+runninglengthbits)) + (largestrunninglengthcount<<1)))==static_cast<uword>(0));
			//assert(largestliteralcount == 0x7FFFUL);
			//assert(2UL<<runninglengthbits-1 == static_cast<uword>(0xFFFFUL));
			//assert(2UL<<(runninglengthbits+1) - 1 == static_cast<uword>(0x1FFFFUL));
		}

		RunningLengthWord(const RunningLengthWord & rlw) : mydata(rlw.mydata) {}

		RunningLengthWord& operator=(const RunningLengthWord & rlw) {
			mydata = rlw.mydata;
			return *this;
		}


		bool getRunningBit() const {
			return mydata & static_cast<uword>(1);
		}
		// static call for people who hate objects
		static inline bool getRunningBit(uword  data) {
			return data & static_cast<uword>(1) ;
		}
		/**
		 * how many words should be filled by the running bit (see previous method)
		 */
		uword getRunningLength() const {
			return (mydata >> 1) & largestrunninglengthcount ;
		}
		// static call for people who hate objects
		static inline uword getRunningLength(uword data) {
			return (data >> 1) & largestrunninglengthcount ;
		}
		/**
		 * followed by how many literal words?
		 */
		uword getNumberOfLiteralWords() const {
			return mydata >> (1+runninglengthbits);
		}

		uint size() const {
			return getRunningLength() + getNumberOfLiteralWords();
		}
		// static call for people who hate objects
		static inline uword getNumberOfLiteralWords(uword data) {
			return data >> (1+runninglengthbits);
		}

		void setRunningBit(bool b) {
			if(b) mydata |= static_cast<uword>(1); else mydata &= ~static_cast<uword>(1);
		}
		// static call for people who hate objects
		static inline void setRunningBit(uword & data, bool b) {
			if(b) data |= static_cast<uword>(1); else data &= ~static_cast<uword>(1);
		}
		void discardFirstWords(uint x) {
			assert(x<= size());
			const uword rl ( getRunningLength() );
			if(rl >= x) {
				setRunningLength(rl - x);
				return;
			}
			x -= rl;
			setRunningLength(0);
			// potentially unsafe...
			setNumberOfLiteralWords(getNumberOfLiteralWords() - x);
		}
		void setRunningLength(uword l) {
			mydata |= shiftedlargestrunninglengthcount;
			mydata &= ((static_cast<uword>(l)) << 1) + notshiftedlargestrunninglengthcount;
		}
		// static call for people who hate objects
		static inline void setRunningLength(uword & data, uword l) {
			data |= shiftedlargestrunninglengthcount;
			data &= ((static_cast<uword>(l)) << 1) + notshiftedlargestrunninglengthcount;
		}

		void setNumberOfLiteralWords(uword l) {
			mydata |= notrunninglengthplusrunningbit;
			mydata &= ((static_cast<uword>(l)) << (runninglengthbits +1) ) +runninglengthplusrunningbit;
		}
		// static call for people who hate objects
		static inline void setNumberOfLiteralWords(uword & data, uword l) {
			data |= notrunninglengthplusrunningbit;
			data &= ((static_cast<uword>(l)) << (runninglengthbits +1) ) +runninglengthplusrunningbit;
		}
		static const uint runninglengthbits = sizeof(uword)*4;//16;
		static const uint literalbits = sizeof(uword)*8 - 1 - runninglengthbits;
		static const uword largestliteralcount = (static_cast<uword>(1)<<literalbits) - 1;
		static const uword largestrunninglengthcount = (static_cast<uword>(1)<<runninglengthbits)-1;//static_cast<uword>(0xFFFFUL) ;
		static const uword shiftedlargestrunninglengthcount = largestrunninglengthcount<<1;
		static const uword notshiftedlargestrunninglengthcount = ~shiftedlargestrunninglengthcount;
		static const uword runninglengthplusrunningbit = (static_cast<uword>(1)<<(runninglengthbits+1)) - 1;//static_cast<uword>(0x1FFFFUL);
		static const uword notrunninglengthplusrunningbit =~runninglengthplusrunningbit;
		static const uword notlargestrunninglengthcount =~largestrunninglengthcount;

		uword & mydata;
	private:
};


template <class uword=uword32>
class ConstRunningLengthWord {
	public:
		ConstRunningLengthWord (const uword & data) : mydata(data) {
			// these should disappear when the compiler goes over them (?)
			if(sizeof(ushort)!=2) cerr << "bad ushort" << endl;
			if(sizeof(uword)<2) cerr << "bad uword" << endl;
		}

		ConstRunningLengthWord(const ConstRunningLengthWord & rlw) : mydata(rlw.mydata) {}

		bool getRunningBit() const {
			return mydata & static_cast<uword>(1);
		}

		/**
	 	* how many words should be filled by the running bit (see previous method)
	 	*/
		uword getRunningLength() const {
			return (mydata >> 1) & RunningLengthWord<uword>::largestrunninglengthcount ;
		}

		/**
		 * followed by how many literal words?
		 */
		uword getNumberOfLiteralWords() const {
			return mydata >> (1+RunningLengthWord<uword>::runninglengthbits);
		}

		uint size() const {
			return getRunningLength() + getNumberOfLiteralWords();
		}

		const uword & mydata;
	private:
};

template <class uword=uword32>
class BufferedRunningLengthWord {
	public:
		BufferedRunningLengthWord (const uword & data) : RunningBit(data & static_cast<uword>(1)),
		RunningLength((data >> 1) & RunningLengthWord<uword>::largestrunninglengthcount),
		NumberOfLiteralWords(data >> (1+RunningLengthWord<uword>::runninglengthbits))
		{
		}
		BufferedRunningLengthWord (const RunningLengthWord<uword> & p) : RunningBit(p.mydata & static_cast<uword>(1)),
		RunningLength((p.mydata >> 1) & RunningLengthWord<uword>::largestrunninglengthcount),
		NumberOfLiteralWords(p.mydata >> (1+RunningLengthWord<uword>::runninglengthbits))
		{
		}

		void read(const uword & data) {
			RunningBit = data & static_cast<uword>(1);
			RunningLength = (data >> 1) & RunningLengthWord<uword>::largestrunninglengthcount;
			NumberOfLiteralWords = data >> (1+RunningLengthWord<uword>::runninglengthbits);
		}
		bool getRunningBit() const {
			return RunningBit;
		}
		void discardFirstWords(uint x) {
					assert(x<= size());
					if(RunningLength >= x) {
						RunningLength -= x;
						return;
					}
					x -= RunningLength;
					RunningLength = 0;
					// potentially unsafe...
					NumberOfLiteralWords -= x;
		}
		/**
	 	* how many words should be filled by the running bit (see previous method)
	 	*/
		uword getRunningLength() const {
			return RunningLength ;
		}

		/**
		 * followed by how many literal words?
		 */
		uword getNumberOfLiteralWords() const {
			return NumberOfLiteralWords;
		}

		uint size() const {
			return RunningLength + NumberOfLiteralWords;
		}
		bool RunningBit;
		uword RunningLength;
		uword NumberOfLiteralWords;

};
template <class uword>
class EWAHBoolArray;

template <class uword>
class EWAHBoolArraySparseIterator;

template <class uword=uword32>
class EWAHBoolArrayIterator {
	public:
		bool hasNext()  const {
			//cout << "call to hasNext"<<endl;
			return pointer < myparent.size();
		}

		uword next() {
			//cout << "call to next pointer is on "<< pointer << " compressedwords "<< compressedwords<< " literawords "<< literalwords << endl;
			uword returnvalue;
			if(compressedwords < rl) {
				++compressedwords;
		    		if(b)
		    			returnvalue = notzero;
		    		else
		    			returnvalue =  zero;
		  	} else {
		  		assert (literalwords < lw) ;
		  		++literalwords;
		  		++pointer;
		  		assert(pointer <myparent.size());
		  		returnvalue =  myparent[pointer];
		  	}
		  	if((compressedwords == rl) && (literalwords == lw)) {
		  		++pointer;
		  		if(pointer < myparent.size()) readNewRunningLengthWord();
		  	}
		  	return returnvalue;
		}

		EWAHBoolArrayIterator(const EWAHBoolArrayIterator<uword> & other):pointer(other.pointer),
		//rlw(other.rlw),
		myparent(other.myparent),
		compressedwords(other.compressedwords),
		literalwords(other.literalwords),
		rl(other.rl),
		lw(other.lw),
		b(other.b){}

		static const uword zero = 0;
		static const uword notzero=~zero;
private:
	EWAHBoolArrayIterator(const vector<uword> & parent) ;
	void readNewRunningLengthWord() ;
	friend class EWAHBoolArray<uword>;
	friend class EWAHBoolArraySparseIterator<uword>;
	uint pointer;
	//RunningLengthWord rlw;
	const vector<uword>  & myparent;
	uword compressedwords;
	uword literalwords;
	uword rl, lw;
	bool b;
};

template <class uword=uword32>
class EWAHBoolArraySparseIterator {
	public:
		bool hasNext() const {
			//cout << "call to hasNext"<<endl;
			return i.hasNext();
		}

		uint position() const {
			return mPosition;
		}

		uword next() {
			//cout << "call to next pointer is on "<< pointer << " compressedwords "<< compressedwords<< " literawords "<< literalwords << endl;
			uword returnvalue;
			if(i.compressedwords < i.rl) {
		    		if(i.b) {
		    			++mPosition;
		    			++i.compressedwords;
		    			returnvalue = EWAHBoolArrayIterator<uword>::notzero;
		    		} else {
		    			mPosition += i.rl;
		    			i.compressedwords = i.rl;
		    			returnvalue = EWAHBoolArrayIterator<uword>::zero;//next();
		    		}
		  	} else {
		  		assert  (i.literalwords < i.lw);
		  		++i.literalwords;
		  		++i.pointer;
		  		++mPosition;
		  		assert(i.pointer <i.myparent.size());
		  		returnvalue =  i.myparent[i.pointer];
		  	}
		  	if((i.compressedwords == i.rl) && (i.literalwords == i.lw)) {
		  		++i.pointer;
		  		if(i.pointer < i.myparent.size()) i.readNewRunningLengthWord();
		  	}
		  	return returnvalue;
		}

		EWAHBoolArraySparseIterator(const EWAHBoolArraySparseIterator<uword> & other):i(other.i),mPosition(other.mPosition) {}

private:
	EWAHBoolArraySparseIterator(const vector<uword> & parent) : i(parent), mPosition(0){}
	EWAHBoolArrayIterator<uword> i;
	uint mPosition;
	friend class EWAHBoolArray<uword>;
};



class BitmapStatistics {
public:
 BitmapStatistics() : totalliteral(0), totalcompressed(0), runningwordmarker(0) {}
	uint getCompressedSize() {return totalliteral+ runningwordmarker;}
	uint getUncompressedSize() {return totalliteral+ totalcompressed;}
	uint getNumberOfDirtyWords() {return totalliteral;}
	uint getNumberOfCleanWords() {return totalcompressed;}
	uint getNumberOfMarkers() {return runningwordmarker;}
	uint totalliteral;
	uint totalcompressed;
	uint runningwordmarker;
};


template <class uword>
class EWAHBoolArrayRawIterator;
/**
 * This is my variant of WAH. This is where compression
 * happens.
 */

template <class uword=uword32>
class EWAHBoolArray {
	public:
		EWAHBoolArray(): buffer(1,0), sizeinbits(0), lastRLW(0) {
		}


		void sparselogicaland(const EWAHBoolArray &a, EWAHBoolArray &out) const;
		//void  logicaland(const EWAHBoolArray &a, EWAHBoolArray &out)  const;
		void rawlogicaland(const  EWAHBoolArray &a, EWAHBoolArray &container) const;

		void intersection(const  EWAHBoolArray &a, EWAHBoolArray &container) const {
			rawlogicaland(a,container);
		}



		void rawlogicalor(const  EWAHBoolArray &a, EWAHBoolArray &container) const;
		void reset() {
			buffer.clear();
			buffer.push_back(0);
			sizeinbits = 0;
			lastRLW = 0;
		}

		/**
	 	* convenience method.
	 	*
		// returns the number of words added (storage cost increase)
	 	*/
		inline uint add(const uword  newdata, const uint bitsthatmatter = 8*sizeof(uword));
		inline void printout(ostream &o = cout) {
			toBoolArray().printout(o);
		}
		void debugprintout() const;

		inline uint sizeInBits() const{
			return sizeinbits;
		}
		inline void setSizeInBits(const uint size) {
			sizeinbits = size;
		}
		inline uint sizeInBytes() const{
			return buffer.size()*sizeof(uword);
		}



		// same as addEmptyWord, but you can do several in one shot!
		// returns the number of words added (storage cost increase)
		uint addStreamOfEmptyWords(const bool v, const uint number);
		uint addStreamOfDirtyWords(const uword * v, const uint number);

		// make sure the size of the array is totalbits bits by padding with zeroes.
		// returns the number of words added (storage cost increase)
		inline uint padWithZeroes(const uint totalbits);
		uint64 sizeOnDisk() const;
		inline void write(ostream & out, const bool savesizeinbits=true) const;
		inline void writeBuffer(ostream & out) const;
		inline uint bufferSize() const {return buffer.size();}


		inline void read(istream & in, const bool savesizeinbits=true);
		inline void readBuffer(istream & in, const uint buffersize);

		bool operator==(const EWAHBoolArray & x) const;

		bool operator!=(const EWAHBoolArray & x) const;

		bool operator==(const BoolArray<uword> & x) const;

		bool operator!=(const BoolArray<uword> & x) const;

		EWAHBoolArrayIterator<uword> uncompress() const ;
		EWAHBoolArraySparseIterator<uword> sparse_uncompress() const ;

		EWAHBoolArrayRawIterator<uword> raw_iterator() const ;

		void iterator_sanity_check();

		/*
	 	* implementing append requires some thought.
	 	*/
		void append(const EWAHBoolArray & x);

		/**
		 * For research purposes. This computes the number of
		 * dirty words and the number of compressed words.
		 */
		BitmapStatistics computeStatistics() const;

		BoolArray<uword> toBoolArray() const;
		//void toRowIDs(vector<ulong> & answer) const;
		void appendRowIDs(vector<ulong> & out, const uint offset = 0) const;
		ulong numberOfOnes();
		void swap(EWAHBoolArray & x);
		const vector<uword> & getBuffer() const {return buffer; };
		enum { wordinbits =  sizeof(uword) * 8, sanitychecks = false};


                /**
                 *Please don't copy your bitmaps!
                 **/
		EWAHBoolArray(const EWAHBoolArray& other) :
		buffer(other.buffer),
		sizeinbits(other.sizeinbits),
		lastRLW(other.lastRLW){
                    assert(buffer.size()<=1);// performance assert!
                    //if(buffer.size()>1) {cerr<<buffer.size()<<endl;throw "xxx";}
                }
                // please, never hard-copy this object. Use the swap method if you must.
		EWAHBoolArray & operator=(const EWAHBoolArray & x) {
			//cerr<<"performance warning: you are doing a hardcopy of EWAHBoolArray!"<<endl;
			//inline void addEmptyWordStaticCalls(bool v);
			buffer = x.buffer;
			sizeinbits = x.sizeinbits;
			lastRLW = x.lastRLW;
                        assert(buffer.size()<=1);// performance assert!
                        //if(buffer.size()>1) {cerr<<buffer.size()<<endl;throw "xxx";}
			return *this;
		}

                /**
                 *if you don't care to copy the bitmap (performance-wise), use this!
                 */
                void expensive_copy(const EWAHBoolArray & x) {
                        buffer = x.buffer;
			sizeinbits = x.sizeinbits;
			lastRLW = x.lastRLW;
                }

                void logicalnot(EWAHBoolArray & x) const;
	private:



		// private because does not increment the size in bits
		// returns the number of words added (storage cost increase)
		inline uint addLiteralWord(const uword  newdata) ;

		// private because does not increment the size in bits
		// returns the number of words added (storage cost increase)
		uint addEmptyWord(const bool v);
		// this second version "might" be faster if you hate OOP.
		// in my tests, it turned out to be slower!
		// private because does not increment the size in bits
		//inline void addEmptyWordStaticCalls(bool v);

		vector<uword> buffer;
		uint sizeinbits;
		//RunningLengthWord  lastRunningLengthWord;
		uint lastRLW;
		//friend class EWAHBoolArrayIterator;
};

/* a slow but convenient function */
template <class EWAH>
uint countRowIDs(const EWAH &ba) {
	vector<ulong>  a;
	ba.appendRowIDs(a);
	return a.size();
}

template <class uword>
void EWAHBoolArray<uword>::logicalnot(EWAHBoolArray & x) const {
    x.reset();
    EWAHBoolArrayRawIterator<uword> i = this->raw_iterator();
    while(i.hasNext()) {
        BufferedRunningLengthWord<uword> & rlw = i.next();
        x.addStreamOfEmptyWords(! rlw.getRunningBit(), rlw.getRunningLength());
	const uword * dw = i.dirtyWords();
        for(uint k = 0 ; k <rlw.getNumberOfLiteralWords();++k) {
          x.addLiteralWord(~ dw[k]);
        }
    }
    x.sizeinbits = this->sizeinbits;
}


template <class uword>
uint EWAHBoolArray<uword>::add(const uword  newdata, const uint bitsthatmatter) {
	//cout << " ==========got this data "<< newdata<< endl;
	//printout();
	sizeinbits += bitsthatmatter;
	if(newdata == 0) {
		return addEmptyWord(0);
	} else if (newdata == static_cast<uword>(~0)) {
		return addEmptyWord(1);
	} else {
		return addLiteralWord(newdata);
	}
}


template <class uword>
inline void EWAHBoolArray<uword>::writeBuffer(ostream & out) const {
	out.write(reinterpret_cast<const char *>(& buffer[0]),sizeof(uword)*buffer.size());
}


template <class uword>
inline void EWAHBoolArray<uword>::readBuffer(istream & in, const uint buffersize) {
	buffer.resize(buffersize);
	in.read(reinterpret_cast<char *>(&buffer[0]),sizeof(uword)*buffersize);
}


template <class uword>
void EWAHBoolArray<uword>::write(ostream & out, const bool savesizeinbits) const {
		if(savesizeinbits)out.write(reinterpret_cast<const char *>( & sizeinbits), sizeof(sizeinbits));
		const uint buffersize = buffer.size();
		out.write(reinterpret_cast<const char *>(& buffersize),sizeof(buffersize));
		out.write(reinterpret_cast<const char *>(& buffer[0]),sizeof(uword)*buffersize);
}


template <class uword>
void EWAHBoolArray<uword>::read(istream & in, const bool savesizeinbits) {
		if(savesizeinbits) in.read(reinterpret_cast<char *>(&sizeinbits), sizeof(sizeinbits));
		else sizeinbits = 0;
		uint buffersize(0);
		in.read(reinterpret_cast<char *>(&buffersize), sizeof(buffersize));
		buffer.resize(buffersize);
		in.read(reinterpret_cast<char *>(&buffer[0]),sizeof(uword)*buffersize);
		if(sanitychecks) iterator_sanity_check() ;
}


template <class uword>
uint EWAHBoolArray<uword>::addLiteralWord(const uword  newdata) {
	RunningLengthWord<uword> lastRunningLengthWord(buffer[lastRLW]);
	uword numbersofar = lastRunningLengthWord.getNumberOfLiteralWords();
	if(numbersofar >= RunningLengthWord<uword>::largestliteralcount) {//0x7FFF) {
		buffer.push_back(0);
		lastRLW = buffer.size() - 1;
		RunningLengthWord<uword> lastRunningLengthWord2(buffer[lastRLW]);
		lastRunningLengthWord2.setNumberOfLiteralWords(1);
		buffer.push_back(newdata);
		return 2;
	}
	lastRunningLengthWord.setNumberOfLiteralWords(numbersofar + 1);
	assert(lastRunningLengthWord.getNumberOfLiteralWords()==numbersofar + 1);
	buffer.push_back(newdata);
	return 1;
}




template <class uword>
uint EWAHBoolArray<uword>::padWithZeroes(const uint totalbits) {
	assert(totalbits >= sizeinbits);
	uint missingbits = totalbits - sizeinbits;
	uint wordsadded = addStreamOfEmptyWords(0, missingbits/wordinbits + ((missingbits % wordinbits != 0) ? 1 : 0));
	assert(sizeinbits >= totalbits);
	assert(sizeinbits  <= totalbits + wordinbits);
	sizeinbits = totalbits;
	return wordsadded;
}



/**
 * This is a low-level iterator.
 */

template <class uword=uword32>
class EWAHBoolArrayRawIterator {
	public:
		EWAHBoolArrayRawIterator(const EWAHBoolArray<uword> & p) : pointer(0),
		myparent(&p.getBuffer()), rlw((*myparent)[pointer]) { //RunningLength(0), NumberOfLiteralWords(0), Bit(0) {
			  //ConstRunningLengthWord rlw(myparent[pointer]);
			  //RunningLength = rlw.getRunningLength();
			  //NumberOfLiteralWords = rlw.getNumberOfLiteralWords();
			  //Bit = rlw.getRunningBit();
			  if(verbose) {
				  cout<<"created a new raw iterator over buffer of size  "<<myparent->size()<<endl;
			  }
		}
		EWAHBoolArrayRawIterator(const EWAHBoolArrayRawIterator & o) : pointer(o.pointer),
		myparent(o.myparent), rlw(o.rlw) {}


		bool hasNext() const {
			if(verbose)cout<<"call to hasNext, pointer is at "<<pointer<< ", parent.size()= "<<myparent->size()<<endl;
			return pointer < myparent->size();
		}

		BufferedRunningLengthWord<uword> & next() {
			  assert(pointer < myparent->size());
			  //copyofmarker = (*myparent)[pointer];
			  rlw.read( (*myparent)[pointer]);
			  //ConstRunningLengthWord rlw(myparent[pointer]);
			  //RunningLength = rlw.getRunningLength();
			  //NumberOfLiteralWords = rlw.getNumberOfLiteralWords();
			  //Bit = rlw.getRunningBit();
			  pointer += rlw.getNumberOfLiteralWords() + 1;
			  if(verbose)cout<<"call to next, pointer moves to "<<pointer<< ", parent.size()= "<<myparent->size()<<endl;
			  return rlw;
		}

		const uword * dirtyWords()  const {
			assert(pointer>0);
			assert(pointer>=rlw.getNumberOfLiteralWords());
			return & ((*myparent)[pointer-rlw.getNumberOfLiteralWords()]);
			//return & myparent[pointer-NumberOfLiteralWords];//];
		}

		EWAHBoolArrayRawIterator & operator=(const EWAHBoolArrayRawIterator & other) {
			pointer = other.pointer;
			myparent=other.myparent;
			//copyofmarker=other.copyofmarker;
			rlw=other.rlw;
			return *this;
		}

		enum{verbose=false};
	uint pointer;
	const vector<uword>  * myparent;
	//uword copyofmarker;
	BufferedRunningLengthWord<uword> rlw;
	//uint RunningLength;
	//uint NumberOfLiteralWords;
	//bool Bit;
private:

	EWAHBoolArrayRawIterator();
};





template <class uword>
class SparseCompressedDataBlock;




template <class uword=uword32>
class CompressedDataBlock {
	public:
		CompressedDataBlock(int numberofbitmaps, uint64 blocksize);// = 67108864//64 meg uncompressed of data
//		CompressedDataBlock();
		~CompressedDataBlock();
		static uint getNumberOfBits() {return sizeof(uword)*8;}
		inline void newRow();
		inline void set(int whichbitmap);
		inline void flushLatestRow();
		static inline string name() {return "CompressedDataBlock";}
		void emptyToDisk(ostream& out, bool verbose = false);
		bool empty();

		CompressedDataBlock & operator=(const CompressedDataBlock & other);

		/**
		 * return true if the memory budget is exceeded. Notice that
		 * you may still have slightly gone over budget and this could
		 * return "false". The amount over budget is number of bitmaps x length
		 * of a word (it is a small amount unless you have thousands of bitmaps).
		 */
		bool full() {
			if((getNumberOfRows() % (8*sizeof(uword)) != 0 ) or (getNumberOfRows() == 0))  return false;
			return predictedSizeOnDisk() >= mBlockSize;
			// logically, this should return "true" for the first time
			// when we have a multiple of "8*sizeof(uword)"
			//uint64 wordsperbitmap = getNumberOfRows() / (8*sizeof(uword));
			//return  mNumberOfBitmaps*wordsperbitmap* sizeof(uword) >= mBlockSize *expectedCompressionRate;
			}
		int getNumberOfRows() {return RowCounter;}

		uint64 predictedSizeOnDisk() {
			return (mStorageUsage*sizeof(uword)
			+ mNumberOfBitmaps*(sizeof(uword))
			+sizeof(uint)*3+sizeof(uint64)+sizeof(int)
			+sizeof(uint)*(mNumberOfBitmaps+1));
		}
		int getNumberOfBitmaps() {return mNumberOfBitmaps;}
		enum{versionID=4,Cookie=0xABCA,compressiontype=EWAH,expectedCompressionRate=8,verboseBlockWrites=false};
		vector<EWAHBoolArray<uword> > & getBuffer() {return membuffer;}
		const vector<EWAHBoolArray<uword> > & getBuffer() const {return membuffer;}
	private:
		CompressedDataBlock(const CompressedDataBlock & other);
		int mNumberOfBitmaps;
		uint64 mBlockSize;
		uint64 mStorageUsage;
		vector<EWAHBoolArray<uword> > membuffer;
		vector<uword> latestRow;
		int RowCounter;
        friend class SparseCompressedDataBlock<uword>;
                // disabling for now: const static int memsetThreshold = 1000; // tuneable
                //bool usingMemset;
};


template <class uword>
void CompressedDataBlock<uword>::newRow() {
	// the first time you add a row beyond the current word, flush the current word
	if (( (RowCounter % (sizeof(uword)*8)) == 0) and (RowCounter != 0) ){
		for(uint k = 0; k < latestRow.size(); ++k) {
			mStorageUsage += membuffer[k].add(latestRow[k]);
			assert(membuffer[k].sizeInBits() == static_cast<uint>(RowCounter));
		}
        memset(& latestRow[0],0,latestRow.size()*sizeof(uword));
	}
	++RowCounter;
}


template <class uword>
void CompressedDataBlock<uword>::set(int whichbitmap) {
	// the "-1" comes from the following logic: if there is one row,
	// then bit 0 should be changed, and so on.
	latestRow[whichbitmap] |= static_cast<uword>(1)<< ((RowCounter - 1) %(sizeof(uword)*8)) ;
    //    if (! usingMemset) dirtyIndices.push_back(whichbitmap);
}

template <class uword>
void CompressedDataBlock<uword>::flushLatestRow() {
	// this is a bit messy. Suppose you have entered 32 bits.
	// because newRow only flushes at row 33, you'd have to flush
	// 32 bits if there 32 rows entered.
	if(RowCounter == 0) return;
	int leftover = (RowCounter - 1)  % (sizeof(uword)*8) + 1;
  //cout << "leftover at RowCOunter "<< RowCounter << " is " << leftover<< " mNumberOfBitmaps= "<<mNumberOfBitmaps<<endl;

  assert(static_cast<uint>(mNumberOfBitmaps) == membuffer.size());
  assert(static_cast<uint>(mNumberOfBitmaps) == latestRow.size());
	//if(leftover >0) {
		for(int k = 0 ; k < mNumberOfBitmaps; ++k) {
			mStorageUsage += membuffer[k].add(latestRow[k],leftover);
		 assert(membuffer[k].sizeInBits()  == static_cast<uint>(RowCounter));
  //cout << "at RowCOunter "<< RowCounter << " have  membuffer[k].sizeInBits() = " << membuffer[k].sizeInBits()<<endl;
		}
		//RowCounter = -1;
        memset(& latestRow[0],0,latestRow.size()*sizeof(uword));
	//}
	RowCounter = 0;
}

/**
 * This is a wrapper object around CompressedDataBlock that should
 * work especially well when the number of bitmaps is large and we
 * have many zeroes.
 */

template <class uword=uword32>
class SparseCompressedDataBlock {
public:
	SparseCompressedDataBlock(int numberofbitmaps, int blocksize) : parent(numberofbitmaps, blocksize),
	dirtyIndices() {	}
	static uint getNumberOfBits() {return sizeof(uword)*8;}
	inline void newRow();
	inline void set(int whichbitmap);
	inline void emptyToDisk(ostream& out, bool verbose = false) {
		flushLatestZeroWords(); parent.emptyToDisk(out, verbose);
	}
  static inline string name() {return "SparseCompressedDataBlock";}
	inline bool full() {return parent.full();};
	inline int getNumberOfRows() {return parent.getNumberOfRows();};
	inline int getNumberOfBitmaps() {return parent.getNumberOfBitmaps();}
	inline bool empty() {return parent.empty();}

  vector<EWAHBoolArray<uword> > & getBuffer() {return parent.getBuffer();}
  const vector<EWAHBoolArray<uword> > & getBuffer() const {return parent.getBuffer();}
	enum{compressiontype=EWAH};
private:

	//inline void flushZeroWords(uint whichbitmap);
	inline void flushLatestZeroWords();
	CompressedDataBlock<uword>  parent;
    std::set<uint> dirtyIndices;
    ///vector<uint> lastdirty;
};





template <class uword>
void SparseCompressedDataBlock<uword>::set(int whichbitmap) {
	parent.set(whichbitmap);
	dirtyIndices.insert(whichbitmap);
}



template <class uword>
void SparseCompressedDataBlock<uword>::newRow() {
	//cout <<"newRow from "<<parent.RowCounter<<endl;
	// the first time you add a row beyond the current word, flush the current word
	if (( (parent.RowCounter % (sizeof(uword)*8)) == 0) and (parent.RowCounter != 0)) {
		//cout <<"flushing at "<<parent.RowCounter<<endl;
		for(std::set<uint>::iterator i = dirtyIndices.begin(); i != dirtyIndices.end(); ++i) {
			assert(parent.membuffer[*i].sizeInBits() <= static_cast<uint>(parent.RowCounter)- sizeof(uword)*8);
			const uint missingbits = static_cast<uint>(parent.RowCounter)- sizeof(uword)*8 - parent.membuffer[*i].sizeInBits();
			assert(missingbits % (sizeof(uword)*8) == 0);
			//flushZeroWords(*i);
			parent.mStorageUsage += parent.membuffer[*i].addStreamOfEmptyWords(0,missingbits / (sizeof(uword)*8));
			assert(parent.membuffer[*i].sizeInBits() == parent.RowCounter - sizeof(uword)*8);
			parent.mStorageUsage += parent.membuffer[*i].add(parent.latestRow[*i]);
			assert(parent.membuffer[*i].sizeInBits() == static_cast<uint>(parent.RowCounter));
			parent.latestRow[*i] = 0; // replaces the memset
		}
		dirtyIndices.clear();
	}
	++parent.RowCounter;
}

/*
void SparseCompressedDataBlock::flushZeroWords(uint whichbitmap) {
	//assert(parent.RowCounter % (sizeof(uword)*8) == 0);
	const uint currentWordIndex = parent.RowCounter / (sizeof(uword)*8);
	const int streamofzeroes = currentWordIndex - parent.membuffer[whichbitmap].sizeInBits() / (sizeof(uword)*8) - 1 ;
	//cout << parent.RowCounter <<" "<< <<" "<< streamofzeroes<<endl;
	if(streamofzeroes > 0) {
		//uint before = parent.membuffer[whichbitmap].sizeInBits();
		parent.mStorageUsage += parent.membuffer[whichbitmap].addStreamOfEmptyWords(0, streamofzeroes);
		//uint after = parent.membuffer[whichbitmap].sizeInBits();
		//assert((after - before)/(8*sizeof(uword)) == streamofzeroes);
	}
	//assert(parent.membuffer[whichbitmap].sizeInBits() == parent.RowCounter - 8*);
	//lastdirty[whichbitmap]  = currentWordIndex;
}*/


template <class uword>
void SparseCompressedDataBlock<uword>::flushLatestZeroWords() {
	//cout <<"flushLatestZeroWords "<<parent.RowCounter<<endl;
	// this is a bit messy. Suppose you have entered 32 bits.
	// because newRow only flushes at row 33, you'd have to flush
	// 32 bits if there 32 rows entered.
	if(parent.RowCounter == 0) return;
	int leftover = (parent.RowCounter - 1)  % (sizeof(uword)*8) + 1;
	for(std::set<uint>::iterator i = dirtyIndices.begin(); i != dirtyIndices.end(); ++i) {
		assert(parent.membuffer[*i].sizeInBits() <= static_cast<uint>(parent.RowCounter- leftover));
		const uint missingbits = parent.RowCounter- leftover - parent.membuffer[*i].sizeInBits();
		assert(missingbits % (sizeof(uword)*8) == 0);
		//flushZeroWords(*i);
		parent.mStorageUsage += parent.membuffer[*i].addStreamOfEmptyWords(0,missingbits / (sizeof(uword)*8));
		assert(parent.membuffer[*i].sizeInBits() == static_cast<uint>(parent.RowCounter - leftover));
		parent.mStorageUsage += parent.membuffer[*i].add(parent.latestRow[*i],leftover);
		assert(parent.membuffer[*i].sizeInBits() == static_cast<uint>(parent.RowCounter));
	}
	dirtyIndices.clear();
	memset(& parent.latestRow[0],0,parent.latestRow.size()*sizeof(uword));
	for(int k = 0 ; k < parent.mNumberOfBitmaps; ++k) {
		//flushZeroWords(k);
		//parent.membuffer[k].add(latestRow[k],leftover);
		parent.mStorageUsage += parent.membuffer[k].padWithZeroes(parent.RowCounter);
		assert(parent.membuffer[k].sizeInBits()  == static_cast<uint>(parent.RowCounter));
		//cout << parent.membuffer[k].sizeInBits() << " ";
	}
	//cout<<endl;
    //memset(& latestRow[0],0,latestRow.size()*sizeof(uword));
	parent.RowCounter = 0;
}



template <class uword=uword32>
class ReadCompressedDataBlock {
public:
	ReadCompressedDataBlock();
	ReadCompressedDataBlock & operator=(const ReadCompressedDataBlock & other);
	~ReadCompressedDataBlock();
	void readAllFromDisk(istream & in);
        template <class container>
	void readFromDisk(istream & in, container &whichbitmaps);
	//bool haveBeenLoaded(const vector<uint> & queryset) ;
	/*void logicaland(const vector<uint> & queryset, EWAHBoolArray<uword> & container);
	void sparselogicaland(const vector<uint> & queryset, EWAHBoolArray<uword> & container);
	void rawlogicaland(const vector<uint> & queryset, EWAHBoolArray<uword> & container);*/
	int getNumberOfRows() {return RowCounter;}
	int getNumberOfBitmaps() {return mNumberOfBitmaps;}
	const EWAHBoolArray<uword> & getBitmap(uint b) const {
		typename map<uint,EWAHBoolArray<uword> >::const_iterator i = membuffer.find(b);
		if(i == membuffer.end()) {
			cerr<<"asking for bitmap "<<b<<endl;
			cerr<<"here are the bitmaps available:"<<endl;
			for(typename map<uint,EWAHBoolArray<uword> >::const_iterator j=membuffer.begin();j!=membuffer.end();++j)
				cerr<<j->first<<endl;
			throw missingbitmap_exception("could not find requested bitmap");
		}
		return i->second;}
	map<uint,EWAHBoolArray<uword> > & getBuffer() {return membuffer;}
	const map<uint,EWAHBoolArray<uword> > & getBuffer() const {return membuffer;}
	void printout(ostream &o=cout);
	enum {compressiontype=CompressedDataBlock<uword>::compressiontype};

private:
	int mNumberOfBitmaps;
	int RowCounter;
	map<uint,EWAHBoolArray<uword> > membuffer;
};


/****
 * The rest of this file used to be in CompressedDataBlock.cpp before we
 * had to go template-happy to enable switching between 64 bits and 32 bits.
 */




template <class uword>
EWAHBoolArrayIterator<uword> EWAHBoolArray<uword>::uncompress() const {
		return EWAHBoolArrayIterator<uword>(buffer);
}

template <class uword>
EWAHBoolArrayRawIterator<uword> EWAHBoolArray<uword>::raw_iterator() const {
		return EWAHBoolArrayRawIterator<uword>(*this);
}


template <class uword>
EWAHBoolArraySparseIterator<uword> EWAHBoolArray<uword>::sparse_uncompress() const {
		return EWAHBoolArraySparseIterator<uword>(buffer);
}

template <class uword>
bool EWAHBoolArray<uword>::operator==(const EWAHBoolArray & x) const {
			if(sizeinbits != x.sizeinbits) return false;
			if(buffer.size() != x.buffer.size()) return false;
			for(uint k = 0; k < buffer.size(); ++k)
				if(buffer[k] != x.buffer[k]) return false;
			return true;
}

template <class uword>
void EWAHBoolArray<uword>::swap(EWAHBoolArray & x) {
	buffer.swap(x.buffer);
	uint tmp = x.sizeinbits;
	x.sizeinbits = sizeinbits;
	sizeinbits = tmp;
	tmp = x.lastRLW;
	x.lastRLW = lastRLW;
	lastRLW = tmp;
}

template <class uword>
void EWAHBoolArray<uword>::append(const EWAHBoolArray & x) {
	if(sizeinbits % wordinbits == 0) {
		// hoping for the best?
		sizeinbits += x.sizeinbits;
		ConstRunningLengthWord<uword> lRLW(buffer[lastRLW]);
		if( (lRLW.getRunningLength() == 0) && (lRLW.getNumberOfLiteralWords() == 0)) {
			// it could be that the running length word is empty, in such a case,
			// we want to get rid of it!
			assert(lastRLW == buffer.size()-1);
			lastRLW = x.lastRLW + buffer.size()  - 1;
			buffer.resize(buffer.size()-1);
			buffer.insert(buffer.end(),x.buffer.begin(),x.buffer.end());
		} else {
		  lastRLW = x.lastRLW + buffer.size();
		  buffer.insert(buffer.end(),x.buffer.begin(),x.buffer.end());
		}
	} else {
		throw invalid_argument("this should really not happen!");
	}
}

template <class uword>
EWAHBoolArrayIterator<uword>::EWAHBoolArrayIterator(const vector<uword> & parent) :
	pointer(0), //rlw(parent[0]),
	myparent(parent),
	compressedwords(0), literalwords(0), rl(0), lw(0), b(0){
	//cout <<" call to freaking private constructor"<<endl;
	if(pointer <myparent.size()) readNewRunningLengthWord();
}


template <class uword>
void EWAHBoolArrayIterator<uword>::readNewRunningLengthWord() {
	//cout << "readNewRunningLengthWord"<<endl;
	  literalwords = 0;
	  compressedwords = 0;
	  ConstRunningLengthWord<uword> rlw(myparent[pointer]);
	  rl = rlw.getRunningLength();
	  lw = rlw.getNumberOfLiteralWords();
	  //cout << "running length = "<< rl << " literals = "<< lw << endl;
	  b = rlw.getRunningBit();
	  if((rl == 0) && (lw == 0)) {
		  if(pointer < myparent.size() -1) {
			  //cout<<"I have "<<myparent.size()<<" words"<<endl;
			  //cout<<"at "<<pointer<<" I have an empty rlw!!!"<<endl;
			  //cout<<"this should not happen, but I will try to recover... this may crash."<<endl;
			  /**
			   * there is a small bug, or inefficiency during the creation of the index.
			   * We should never get here in the, but in rare instances, it happens.
			   * The following code seems to allow us to recover:
			   */
			  ++pointer;
			  readNewRunningLengthWord();
		  } else {
		    assert(pointer >= myparent.size()-1);
		    pointer = myparent.size();
		    assert(! hasNext());
		  }
	  }
}

template <class uword>
BoolArray<uword> EWAHBoolArray<uword>::toBoolArray() const {
	BoolArray<uword> ans(sizeinbits);
	EWAHBoolArrayIterator<uword> i = uncompress();
	int counter = 0;
	while(i.hasNext()) {
		ans.setWord(counter++,i.next());
	}
	return ans;
}

template <class uword>
ulong EWAHBoolArray<uword>::numberOfOnes() {
	ulong c (0);
	EWAHBoolArraySparseIterator<uword> i = sparse_uncompress();
	//EWAHBoolArrayIterator i = uncompress();
	//uword counter (0);
	while(i.hasNext()) {
		const uword currentword = i.next();
		//const ulong counter = (i.position()  - 1 )* sizeof(uword)* 8;
		for(int k = 0; k < wordinbits; ++k) {
			if ( (currentword & (static_cast<uword>(1) << k)) != 0)
				++c;
			//++counter;
		}
	}
	return c;

}


template <class uword>
void EWAHBoolArray<uword>::appendRowIDs(vector<ulong> & out, const uint offset) const {
	EWAHBoolArraySparseIterator<uword> i = sparse_uncompress();
	while(i.hasNext()) {
		const uword currentword = i.next();
		const ulong counter = (i.position()  - 1 )* sizeof(uword)* 8;
		const uint bitsavailable = min(wordinbits,sizeinbits-counter);
		for(uint k = 0; k < bitsavailable; ++k) {
			if ( (currentword & (static_cast<uword>(1) << k)) != 0)
				out.push_back(counter + k + offset);
		}
	}
}

template <class uword>
bool EWAHBoolArray<uword>::operator!=(const EWAHBoolArray<uword> & x) const {
	return !(*this == x);
}

template <class uword>
bool EWAHBoolArray<uword>::operator==(const BoolArray<uword> & x) const {
	// could be more efficient
	return (this->toBoolArray() == x);
}

template <class uword>
bool EWAHBoolArray<uword>::operator!=(const BoolArray<uword> & x) const {
	// could be more efficient
	return (this->toBoolArray() != x);
}


template <class uword>
uint EWAHBoolArray<uword>::addStreamOfEmptyWords(const bool v, const uint number) {
	if(number == 0) return 0;
	RunningLengthWord<uword> lastRunningLengthWord(buffer[lastRLW]);
	const bool noliteralword = (lastRunningLengthWord.getNumberOfLiteralWords() == 0);
	//firts, if the last running length word is empty, we align it
	// this
	const uword runlen = lastRunningLengthWord.getRunningLength();
	if( ( noliteralword ) && ( runlen == 0 )) {
		lastRunningLengthWord.setRunningBit(v);
	}
	uint wordsadded (0);
	if( ( noliteralword ) && (lastRunningLengthWord.getRunningBit() == v) && (runlen < RunningLengthWord<uword>::largestrunninglengthcount) ) {
		// that's the easy case, we are just continuing
		uint whatwecanadd = min(number, RunningLengthWord<uword>::largestrunninglengthcount-runlen);
		lastRunningLengthWord.setRunningLength(runlen+whatwecanadd);
		sizeinbits += whatwecanadd*wordinbits;
		if(number - whatwecanadd> 0 ) wordsadded += addStreamOfEmptyWords(v, number - whatwecanadd);
	} else {
		buffer.push_back(0);
		++wordsadded;
		lastRLW = buffer.size() - 1;
		RunningLengthWord<uword> lastRunningLengthWord2(buffer[lastRLW]);
		uint whatwecanadd = min(number, RunningLengthWord<uword>::largestrunninglengthcount);
		lastRunningLengthWord2.setRunningBit(v);
		lastRunningLengthWord2.setRunningLength(whatwecanadd);
		sizeinbits += whatwecanadd*wordinbits;
		if(number - whatwecanadd> 0 ) wordsadded += addStreamOfEmptyWords(v, number - whatwecanadd);
	}
	return wordsadded;
}


template <class uword>
uint EWAHBoolArray<uword>::addStreamOfDirtyWords(const uword * v, const uint number) {
	if(number == 0) return 0;
	RunningLengthWord<uword>  lastRunningLengthWord(buffer[lastRLW]);
	const uword NumberOfLiteralWords = lastRunningLengthWord.getNumberOfLiteralWords();
        assert(RunningLengthWord<uword>::largestliteralcount >= NumberOfLiteralWords);
        //cout<<" number of literal words is "<<NumberOfLiteralWords<<" number to add is "<<number<<" max count is "<<RunningLengthWord<uword>::largestliteralcount<<endl;
	const uint whatwecanadd = min(number, RunningLengthWord<uword>::largestliteralcount - NumberOfLiteralWords);//0x7FFF-NumberOfLiteralWords);
        assert(NumberOfLiteralWords+whatwecanadd>=NumberOfLiteralWords);
        assert(NumberOfLiteralWords+whatwecanadd<=RunningLengthWord<uword>::largestliteralcount);
        lastRunningLengthWord.setNumberOfLiteralWords(NumberOfLiteralWords+whatwecanadd);
        assert(lastRunningLengthWord.getNumberOfLiteralWords()==NumberOfLiteralWords+whatwecanadd);
	const uint leftovernumber = number -whatwecanadd;
	// add the dirty words...
	// this could be done more efficiently using memcopy....
	const uint oldsize (buffer.size());
	buffer.resize(oldsize+whatwecanadd);
	memcpy(&buffer[oldsize],v,whatwecanadd*sizeof(uword));
	/*for(uint k =0; k<whatwecanadd; ++k)
		buffer.push_back(v[k]);*/
	uint wordsadded(whatwecanadd);
	if(leftovernumber>0) {
		//add
		buffer.push_back(0);
                lastRLW=buffer.size() - 1;
		++wordsadded;
                //cout<<"I have got "<<leftovernumber<< " left to write out of a total of "<<number<<endl;
		wordsadded+=addStreamOfDirtyWords(v+whatwecanadd, leftovernumber);
	}
        assert(wordsadded >= number);
	return wordsadded;
}



template <class uword>
uint EWAHBoolArray<uword>::addEmptyWord(const bool v) {
	//cout << "adding empty word:"<<v<<endl;
	RunningLengthWord<uword> lastRunningLengthWord(buffer[lastRLW]);
	const bool noliteralword = (lastRunningLengthWord.getNumberOfLiteralWords() == 0);
	//firts, if the last running length word is empty, we align it
	// this
	uword runlen = lastRunningLengthWord.getRunningLength();
	if( ( noliteralword ) && ( runlen == 0 )) {
		lastRunningLengthWord.setRunningBit(v);
		assert(lastRunningLengthWord.getRunningBit() == v);
	}
	if( ( noliteralword ) && (lastRunningLengthWord.getRunningBit() == v) && (runlen < RunningLengthWord<uword>::largestrunninglengthcount) ) {
		lastRunningLengthWord.setRunningLength(runlen+1);
                //cerr<<lastRunningLengthWord.getRunningLength()<<" "<<runlen+1<<" "<<runlen<<endl;
		assert(lastRunningLengthWord.getRunningLength() == runlen+1);
		return 0;
	} else {
		// we have to start anew
		//cout << " [EmptyWord]  starting a new rlw"<<endl;
		//printout();
		buffer.push_back(0);
		lastRLW = buffer.size() - 1;
		RunningLengthWord<uword> lastRunningLengthWord2(buffer[lastRLW]);
		assert(lastRunningLengthWord2.getRunningLength()==0);
		assert(lastRunningLengthWord2.getRunningBit()==0);
		assert(lastRunningLengthWord2.getNumberOfLiteralWords()==0);
		lastRunningLengthWord2.setRunningBit(v);
		assert(lastRunningLengthWord2.getRunningBit() == v);
		lastRunningLengthWord2.setRunningLength(1);
		assert(lastRunningLengthWord2.getRunningLength() == 1);
		assert(lastRunningLengthWord2.getNumberOfLiteralWords()==0);
		return 1;
	}
}

/*
template <class uword>
void EWAHBoolArray<uword>::logicaland(const  EWAHBoolArray &a, EWAHBoolArray &container) const {
	container.reset();// answer;
	assert(sizeInBits() == a.sizeInBits());
	EWAHBoolArrayIterator<uword> i = a.uncompress();
	EWAHBoolArrayIterator<uword> j = uncompress();
	while(i.hasNext()) {
		assert(j.hasNext());
		const uword x = j.next();
		const uword y = i.next();
		container.add(x & y);
	}
	container.setSizeInBits(sizeInBits());
}*/


template <class uword>
void EWAHBoolArray<uword>::sparselogicaland(const  EWAHBoolArray &a, EWAHBoolArray &container) const {
	//EWAHBoolArray answer;
	container.reset();
	assert(sizeInBits() == a.sizeInBits());
	/**
	 * This could possibly be faster if we go around
	 * the uncompress calls.
	 */
	EWAHBoolArraySparseIterator<uword> i = a.sparse_uncompress();
	EWAHBoolArraySparseIterator<uword> j = sparse_uncompress();
	uint pos (0);
	uword x,y;
	bool ibehindj,jbehindi;
	while(i.hasNext() and j.hasNext()) {
			x = i.next();
			y = j.next();
			ibehindj = i.position() < j.position();
			jbehindi = j.position() < i.position();
			while (( ibehindj and i.hasNext()) or (jbehindi and j.hasNext())) {
				if(ibehindj) x = i.next();
				else if(jbehindi) y = j.next();
				ibehindj = i.position() < j.position();
				jbehindi = j.position() < i.position();
			}
			uint nextnonzero = min(i.position(), j.position());
			if(nextnonzero > pos + 1) {
				container.addStreamOfEmptyWords(0, nextnonzero-pos-1);
				pos += nextnonzero-pos-1;
			}
			if(i.position() == j.position()) {
				container.add(x & y);
				++pos;
			}
	}
	container.setSizeInBits(sizeInBits());
	//return answer;
}



template <class uword>
void EWAHBoolArray<uword>::rawlogicalor(const  EWAHBoolArray &a, EWAHBoolArray &container) const {
	container.reset();
        //if(sizeInBits() != a.sizeInBits()) cerr<<sizeInBits()<<" "<<a.sizeInBits()<<endl;
	assert(sizeInBits() == a.sizeInBits());
	EWAHBoolArrayRawIterator<uword> i = a.raw_iterator();
	EWAHBoolArrayRawIterator<uword> j = raw_iterator();
	if(!(i.hasNext() and j.hasNext())) {// hopefully this never happens...
		container.setSizeInBits(sizeInBits());
		return;
	}
	// at this point, this should be safe:
	BufferedRunningLengthWord<uword> & rlwi = i.next();
	BufferedRunningLengthWord<uword> & rlwj = j.next();
	//RunningLength;
	while (true) {
		bool i_is_prey (rlwi.size()<rlwj.size());
		BufferedRunningLengthWord<uword> & prey ( i_is_prey ? rlwi: rlwj);
		BufferedRunningLengthWord<uword> & predator (i_is_prey ? rlwj: rlwi);
		if(prey.getRunningBit() == 0) {
                    // we have a stream of 0x00
			const uint predatorrl (predator.getRunningLength());
			const uint preyrl (prey.getRunningLength());
			const uint tobediscarded = (predatorrl >= preyrl) ?  preyrl : predatorrl;
			container.addStreamOfEmptyWords(predator.getRunningBit(), tobediscarded);
			const uword * dw_predator (i_is_prey ? j.dirtyWords(): i.dirtyWords());
			container.addStreamOfDirtyWords(dw_predator, preyrl - tobediscarded);
			predator.discardFirstWords(preyrl);
			prey.discardFirstWords(preyrl);
			//prey.discardFirstWords(prey.getRunningLength());
		} else {
                        // we have a stream of 1x11
			const uint preyrl (prey.getRunningLength());
			predator.discardFirstWords(preyrl);
			prey.discardFirstWords(preyrl);
			container.addStreamOfEmptyWords(1, preyrl);
		}
		const uint predatorrl (predator.getRunningLength());
		if(predatorrl>0){
		if(predator.getRunningBit() == 0) {
			const uint nbre_dirty_prey(prey.getNumberOfLiteralWords());
			const uword * dw_prey (i_is_prey ? i.dirtyWords(): j.dirtyWords());
			const uint tobediscarded = (predatorrl >= nbre_dirty_prey) ? nbre_dirty_prey : predatorrl;
			//nbre_dirty_prey -= tobediscarded;
			container.addStreamOfDirtyWords(dw_prey, tobediscarded);
			//dw_prey += tobediscarded;
			predator.discardFirstWords(tobediscarded);
			prey.discardFirstWords(tobediscarded);
                } else {
                    	const uint nbre_dirty_prey(prey.getNumberOfLiteralWords());
			//const uword * dw_prey (i_is_prey ? i.dirtyWords(): j.dirtyWords());
			const uint tobediscarded = (predatorrl >= nbre_dirty_prey) ? nbre_dirty_prey : predatorrl;
			//nbre_dirty_prey -= tobediscarded;
			//dw_prey += tobediscarded;
			predator.discardFirstWords(tobediscarded);
			prey.discardFirstWords(tobediscarded);
			container.addStreamOfEmptyWords(1, tobediscarded);
		}
		}
		assert(prey.getRunningLength() ==0);
		// all that is left to do now is to AND the dirty words
		uint nbre_dirty_prey(prey.getNumberOfLiteralWords());
		if(nbre_dirty_prey > 0) {
			assert(predator.getRunningLength() ==0);
			const uword * dw_prey (i_is_prey ? i.dirtyWords(): j.dirtyWords());
			const uword * dw_predator (i_is_prey ? j.dirtyWords(): i.dirtyWords());
			for(uint k = 0; k< nbre_dirty_prey;++k) {
				container.add(dw_prey[k] | dw_predator[k]);
			}
			predator.discardFirstWords(nbre_dirty_prey);
			//prey.discardFirstWords(nbre_dirty_prey);
		}
		if( i_is_prey ) {
			//assert(i.rlw.size() ==0);
			if(!i.hasNext()) break;
			rlwi = i.next();
		} else {
			//assert(j.rlw.size() ==0);
			if(!j.hasNext()) break;
			rlwj = j.next();
		}
	}
	container.setSizeInBits(sizeInBits());
}


template <class uword>
void EWAHBoolArray<uword>::rawlogicaland(const  EWAHBoolArray &a, EWAHBoolArray &container) const {
	container.reset();
	assert(sizeInBits() == a.sizeInBits());
	EWAHBoolArrayRawIterator<uword> i = a.raw_iterator();
	EWAHBoolArrayRawIterator<uword> j = raw_iterator();
	if(!(i.hasNext() and j.hasNext())) {// hopefully this never happens...
		container.setSizeInBits(sizeInBits());
		return;
	}
	// at this point, this should be safe:
	BufferedRunningLengthWord<uword> & rlwi = i.next();
	BufferedRunningLengthWord<uword> & rlwj = j.next();
	//RunningLength;
	while (true) {
		bool i_is_prey (rlwi.size()<rlwj.size());
		BufferedRunningLengthWord<uword> & prey ( i_is_prey ? rlwi: rlwj);
		BufferedRunningLengthWord<uword> & predator (i_is_prey ? rlwj: rlwi);
		if(prey.getRunningBit() == 0) {
			const uint preyrl (prey.getRunningLength());
			predator.discardFirstWords(preyrl);
			prey.discardFirstWords(preyrl);
			container.addStreamOfEmptyWords(0, preyrl);
			//prey.discardFirstWords(prey.getRunningLength());
		} else {
			// we have a stream of 1x11
			const uint predatorrl (predator.getRunningLength());
			const uint preyrl (prey.getRunningLength());
			const uint tobediscarded = (predatorrl >= preyrl) ?  preyrl : predatorrl;
			container.addStreamOfEmptyWords(predator.getRunningBit(), tobediscarded);
			const uword * dw_predator (i_is_prey ? j.dirtyWords(): i.dirtyWords());
			container.addStreamOfDirtyWords(dw_predator, preyrl - tobediscarded);
			predator.discardFirstWords(preyrl);
			prey.discardFirstWords(preyrl);
		}
		const uint predatorrl (predator.getRunningLength());
		if(predatorrl>0){
		if(predator.getRunningBit() == 0) {
			const uint nbre_dirty_prey(prey.getNumberOfLiteralWords());
			//const uword * dw_prey (i_is_prey ? i.dirtyWords(): j.dirtyWords());
			const uint tobediscarded = (predatorrl >= nbre_dirty_prey) ? nbre_dirty_prey : predatorrl;
			//nbre_dirty_prey -= tobediscarded;
			//dw_prey += tobediscarded;
			predator.discardFirstWords(tobediscarded);
			prey.discardFirstWords(tobediscarded);
			container.addStreamOfEmptyWords(0, tobediscarded);
		} else {
			const uint nbre_dirty_prey(prey.getNumberOfLiteralWords());
			const uword * dw_prey (i_is_prey ? i.dirtyWords(): j.dirtyWords());
			const uint tobediscarded = (predatorrl >= nbre_dirty_prey) ? nbre_dirty_prey : predatorrl;
			//nbre_dirty_prey -= tobediscarded;
			container.addStreamOfDirtyWords(dw_prey, tobediscarded);
			//dw_prey += tobediscarded;
			predator.discardFirstWords(tobediscarded);
			prey.discardFirstWords(tobediscarded);
		}
		}
		assert(prey.getRunningLength() ==0);
		// all that is left to do now is to AND the dirty words
		uint nbre_dirty_prey(prey.getNumberOfLiteralWords());
		if(nbre_dirty_prey > 0) {
			assert(predator.getRunningLength() ==0);
			const uword * dw_prey (i_is_prey ? i.dirtyWords(): j.dirtyWords());
			const uword * dw_predator (i_is_prey ? j.dirtyWords(): i.dirtyWords());
			for(uint k = 0; k< nbre_dirty_prey;++k) {
				container.add(dw_prey[k] & dw_predator[k]);
			}
			predator.discardFirstWords(nbre_dirty_prey);
			//prey.discardFirstWords(nbre_dirty_prey);
		}
		if( i_is_prey ) {
			//assert(i.rlw.size() ==0);
			if(!i.hasNext()) break;
			rlwi = i.next();
		} else {
			//assert(j.rlw.size() ==0);
			if(!j.hasNext()) break;
			rlwj = j.next();
		}
	}
	container.setSizeInBits(sizeInBits());
}


template <class uword>
BitmapStatistics EWAHBoolArray<uword>::computeStatistics() const {
			//uint totalcompressed(0), totalliteral(0);
			BitmapStatistics bs;
			EWAHBoolArrayIterator<uword> i = uncompress();
			while(i.hasNext()) {
				if((i.compressedwords == 0) && (i.literalwords == 0))
					++bs.runningwordmarker;
				if(i.compressedwords < i.rl) {
					++i.compressedwords;
					++bs.totalcompressed;
			  	} else {
			  		assert (i.literalwords < i.lw) ;
			  		++i.literalwords;
			  		++i.pointer;
			  		++bs.totalliteral;
			  	}
			  	if((i.compressedwords == i.rl) && (i.literalwords == i.lw)) {
			  		++i.pointer;
			  		if(i.pointer < i.myparent.size()) i.readNewRunningLengthWord();
			  	}
			}
			return bs;
}

template <class uword>
void EWAHBoolArray<uword>::iterator_sanity_check() {
	EWAHBoolArrayIterator<uword> i = uncompress();
	uint64 counter(0);
	while(i.hasNext()) {
		i.next();
		counter += 8*sizeof(uword);
	}
	if(counter<sizeinbits) {
		cerr <<"the uncompressed bitmap has size at most "<<counter<<" bits, yet it is supposed to be of size "<<sizeinbits<<endl;
	}
	if(counter - 8*sizeof(uword)>=sizeinbits) {
		cerr <<"the uncompressed bitmap has size at most "<<counter<<" bits and over "<< (counter - 8*sizeof(uword))<<", yet it is supposed to be of size "<<sizeinbits<<endl;
	}
	assert(counter>=sizeinbits);
	assert(counter - 8*sizeof(uword)<sizeinbits );
}

template <class uword>
void EWAHBoolArray<uword>::debugprintout() const {
			cout << "==printing out EWAHBoolArray=="<<endl;
			cout <<"Number of compressed words: "<< buffer.size()<< endl;
			uint pointer = 0;
			while(pointer <buffer.size()) {
				ConstRunningLengthWord<uword> rlw(buffer[pointer]);
				bool b = rlw.getRunningBit() ;
				uword rl =  rlw.getRunningLength() ;
				uword lw = rlw.getNumberOfLiteralWords();
				cout << "pointer = "<<pointer<<" running bit="<<b<<" running length="<<rl<<" lit. words="<<lw<<endl;
				for(uint j = 0; j < lw ;++j) {
				  const uword & w = buffer[pointer+j+1];
				  cout<<toBinaryString(w)<<endl;;
				}
				pointer += lw + 1;
			}
			cout << "==END=="<<endl;
}

template <class uword>
uint64 EWAHBoolArray<uword>::sizeOnDisk() const {
	return sizeof(sizeinbits)+sizeof(uint)+sizeof(uword)*static_cast<uint64>(buffer.size());
}
/*CompressedDataBlock::CompressedDataBlock():
	mNumberOfBitmaps(0),
	mBlockSize(0),
	membuffer(mNumberOfBitmaps),
	latestRow(mNumberOfBitmaps,0),
	RowCounter(-1)
{
}*/

template <class uword>
CompressedDataBlock<uword>::CompressedDataBlock(int numberofbitmaps, uint64 blocksize):
	mNumberOfBitmaps(numberofbitmaps),
	mBlockSize(blocksize),
	mStorageUsage(0),
	membuffer(mNumberOfBitmaps),
	latestRow(mNumberOfBitmaps,0),
	RowCounter(0)//,
        //dirtyIndices()//,
        // usingMemset( numberofbitmaps < memsetThreshold),
        //usingMemset(1)
{
}


template <class uword>
ReadCompressedDataBlock<uword>::ReadCompressedDataBlock():
	mNumberOfBitmaps(0),
	RowCounter(0),
	membuffer()
	//,
        //dirtyIndices()//,
        // usingMemset( numberofbitmaps < memsetThreshold),
        //usingMemset(1)
{
}


/*
CompressedDataBlock::CompressedDataBlock():
	mNumberOfBitmaps(0),
	mBlockSize(0),
	membuffer(0),
	latestRow(0,0),
	RowCounter(0)//,
        //dirtyIndices()//,
        // usingMemset( numberofbitmaps < memsetThreshold),
        //usingMemset(1)
{
}*/


template <class uword>
CompressedDataBlock<uword>::CompressedDataBlock(const CompressedDataBlock & other):
	mNumberOfBitmaps(other.mNumberOfBitmaps),
	mBlockSize(other.mBlockSize),
	mStorageUsage(other.mStorageUsage),
	membuffer(other.membuffer),
	latestRow(other.latestRow),
	RowCounter(other.RowCounter)//,
        //dirtyIndices(other.dirtyIndices)//,
        //usingMemset(other.usingMemset)
{
}

template <class uword>
CompressedDataBlock<uword> & CompressedDataBlock<uword>::operator=(const CompressedDataBlock & other) {
	mNumberOfBitmaps = other.mNumberOfBitmaps;
	mBlockSize = other.mBlockSize;
	mStorageUsage = other.mStorageUsage;
	membuffer = other.membuffer;
	latestRow = other.latestRow;
	RowCounter = other.RowCounter;
        //dirtyIndices = other.dirtyIndices;
        //usingMemset = other.usingMemset;
	return *this;
}

template <class uword>
ReadCompressedDataBlock<uword> & ReadCompressedDataBlock<uword>::operator=(const ReadCompressedDataBlock & other) {
	mNumberOfBitmaps = other.mNumberOfBitmaps;
	RowCounter = other.RowCounter;
	membuffer = other.membuffer;
	return *this;
}



template <class uword>
CompressedDataBlock<uword>::~CompressedDataBlock()
{
}

template <class uword>
ReadCompressedDataBlock<uword>::~ReadCompressedDataBlock()
{
}

template <class uword>
void ReadCompressedDataBlock<uword>::readAllFromDisk(istream & in) {
	vector<uint> whichbitmaps;
	//for(uint k = 0; k <(uint)mNumberOfBitmaps; ++k)
	//	whichbitmaps.push_back(k);
        whichbitmaps.resize(static_cast<uint> (mNumberOfBitmaps));
        for (uint k = 0; k <static_cast<uint> (mNumberOfBitmaps); ++k)
            whichbitmaps[k]=k;
        readFromDisk(in,whichbitmaps);
}

template <class uword>
template <class container>
void ReadCompressedDataBlock<uword>::readFromDisk(istream & in, container & whichbitmaps) {
    uint cookie(0), version(0);
    in.read(reinterpret_cast<char *> (&cookie), sizeof (cookie));
    if (!in) return;
    if (cookie != CompressedDataBlock<uword>::Cookie) {
        return;
    }
    in.read(reinterpret_cast<char *> (&version), sizeof (version));
    if ((version != CompressedDataBlock<uword>::versionID) and(version != 1)) {
        cerr << "version mismatch, this may fail" << endl;
    }
    uint64 BlockSize(0);
    in.read(reinterpret_cast<char *> (&BlockSize), sizeof (BlockSize));
    mNumberOfBitmaps = 0;
    in.read(reinterpret_cast<char *> (&mNumberOfBitmaps), sizeof (mNumberOfBitmaps));
    if (mNumberOfBitmaps == 0) {
        cout << "no bitmaps?" << endl;
        throw failedtoreaddatablockheader_exception("In CompressedDataBlock::readFromDisk, no bitmaps reported in data block! ");
    }
    if (whichbitmaps.size() == 0) {
        //whichbitmaps.resize(static_cast<uint> (mNumberOfBitmaps));
        for (uint k = 0; k <static_cast<uint> (mNumberOfBitmaps); ++k)
            whichbitmaps.insert(whichbitmaps.end(), k);
    }
    uint numberofrows(0);
    in.read(reinterpret_cast<char *> (&numberofrows), sizeof (numberofrows));
    RowCounter = numberofrows;
    if (!in) return;
    ostream::pos_type beginning = in.tellg();
    map<uint, uint> locations;
    if (whichbitmaps.size() == static_cast<uint> (mNumberOfBitmaps)) {
        vector<uint> tmp(mNumberOfBitmaps + 1);
        in.read(reinterpret_cast<char *> (& tmp[0]), sizeof (uint)*(mNumberOfBitmaps + 1));
        for (uint k = 0; k < static_cast<uint> (mNumberOfBitmaps + 1); ++k)
            locations[k] = tmp[k];
    } else {
        uint pos(0);
        for (typename container::iterator k = whichbitmaps.begin(); k != whichbitmaps.end(); ++k) {
            in.seekg(beginning + static_cast<ostream::pos_type> ((*k) * sizeof (uint)));
            pos = 0;
            in.read(reinterpret_cast<char *> (&pos), sizeof (pos));
            locations[*k] = pos;
            in.read(reinterpret_cast<char *> (&pos), sizeof (pos));
            locations[(*k) + 1] = pos;
        }
        in.seekg(beginning + static_cast<ostream::pos_type> (mNumberOfBitmaps * sizeof (uint)));
        in.read(reinterpret_cast<char *> (&pos), sizeof (pos));
        locations[mNumberOfBitmaps] = pos;
    }
    if (!in) {
        throw failedtoreaddatablockheader_exception("In CompressedDataBlock::readFromDisk, failed to read the datablock header... ");
    }
    for (typename container::iterator k = whichbitmaps.begin(); k != whichbitmaps.end(); ++k) {
        //for(int k = 0; k < mNumberOfBitmaps; ++k) {
        if (static_cast<ostream::pos_type> (locations[*k]) + beginning != in.tellg()) {
            in.seekg(static_cast<ostream::pos_type> (locations[*k]) + beginning);
            if (!in) {
                cerr << "In CompressedDataBlock::readFromDisk, failed to seek to bitmap #" << *k << " at " << (locations[*k] + beginning) << endl;
                return;
            }
        }
        assert(static_cast<ostream::pos_type> (locations[*k]) + beginning == in.tellg());
        //membuffer[*k].read(in,false);
        //cout <<Ê"bitmap "<<*k <<Ê" starts at "<<locations[k]<<Êendl;
        assert(locations.find((*k) + 1) != locations.end());
        assert(locations.find(*k) != locations.end());
        assert((locations[(*k) + 1] - locations[*k]) % sizeof (uword) == 0);
        const uint buffersize = (locations[(*k) + 1] - locations[*k]) / sizeof (uword);
        membuffer[*k].readBuffer(in, buffersize);
        membuffer[*k].setSizeInBits(numberofrows);
        assert(membuffer[*k].sizeInBits() == numberofrows);
    }
    if (!in) {
        throw failedtoreadbitmap_exception("In CompressedDataBlock::readFromDisk, failed to read the bitmaps... ");
    }
    in.seekg(locations[mNumberOfBitmaps] + beginning);
    if (!in) {
        cerr << "In CompressedDataBlock::readFromDisk, failed to seek to the end of the datablock... " << endl;
    }
}


template <class uword>
void CompressedDataBlock<uword>::emptyToDisk(ostream& out, bool verbose) {
	assert(mNumberOfBitmaps>0);
	flushLatestRow();
	ostream::pos_type beforeheader = out.tellp();
	uint numberofrows = membuffer[0].sizeInBits();
	if(RowCounter != 0)
		throw forgottoflushlastrow_exception("In emptyToDisk, it appears you did not call flushLatestRow while it was needed");
	if(numberofrows == 0) return;
	//cout <<"flushing a new db to disk, number of rows "<<getNumberOfRows()<<endl;
	// save a cookie (for tracking, debugging purposes)
	uint cookie = Cookie;
	if(!out) cerr<<"the output stream provided to CompressedDataBlock::emptyToDisk is not writeable."<<endl;
	out.write(reinterpret_cast<char *>(&cookie),sizeof(cookie));
	// version number, for debugging purposes...
	uint version = versionID;
	out.write(reinterpret_cast<char *>(&version),sizeof(version));
	// save the memory budget (useful for debugging?)
	out.write(reinterpret_cast<char *>(&mBlockSize),sizeof(mBlockSize));
	// save the number of bitmaps
	out.write(reinterpret_cast<char *>(&mNumberOfBitmaps),sizeof(mNumberOfBitmaps));
	// save the number of rows
	out.write(reinterpret_cast<char *>(& numberofrows), sizeof(numberofrows));
	if(!out) cerr <<"the stream is in bad shape after writing the header of the datablock"<<endl;
	//cout<<"number of rows saved= "<<numberofrows<<endl;
	// for each bitmap, save how many bytes have been used
	// then dump the data
	// next we save the location on disk of the bitmaps
	//vector<uint> locations(mNumberOfBitmaps+1,0);
	//out.write(reinterpret_cast<char *>(& locations[0]), sizeof(uint)*(mNumberOfBitmaps+1));
	ostream::pos_type locations_pos = out.tellp();
	{uint location =  (mNumberOfBitmaps+1)*sizeof(uint);
	for(int k = 0; k < mNumberOfBitmaps; ++k) {
		out.write(reinterpret_cast<const char *>(& location), sizeof(location));
		location += membuffer[k].bufferSize()*sizeof(uword);
	}
	out.write(reinterpret_cast<const char *>(& location), sizeof(location));}
	if(!out)
		cerr <<"the stream is in bad shape before I even being writing the bitmaps"<<endl;
	uint headersize = static_cast<uint>(out.tellp()-  beforeheader);
	if(verbose) cout << "CompressedDatablock header uses "<<(out.tellp()-  beforeheader) << " bytes ("<<mNumberOfBitmaps<<" bitmaps)"<<endl ;
	//location = (mNumberOfBitmaps+1)*sizeof(uint);
	for(int k = 0; k < mNumberOfBitmaps; ++k) {
	     assert(numberofrows >= 1);
	     //cout <<membuffer[k].sizeInBits() << " " << numberofrows<<endl;
	     assert(membuffer[k].sizeInBits() == numberofrows);/* {
	    	 cerr << "This is odd. You are supposed to have "<<numberofrows<< " bits per bitmap, "<<endl;
	    	 cerr <<"but bitmap "<<k<< " has "<<membuffer[k].sizeInBits()<< " bits! "<<endl;
	     }*/
	     //assert (membuffer[k].sizeInBits() == numberofrows);
	     //locations[k] = static_cast<uint>(out.tellp() - locations_pos);
	     //cout <<Ê"wrote bitmap "<<k <<Ê" at location "<<locations[k]<<Êendl;
	     membuffer[k].writeBuffer(out);//,false);
	     //location += membuffer[k].bufferSize()*sizeof(uword);
	     //assert(static_cast<ostream::pos_type>(location) + locations_pos == out.tellp());
	     if(!out) {
	       cerr <<"the stream is in bad shape after writing bitmap #"<<k<<endl;
	       cout << "size of the current datablock header: "<<headersize <<endl ;
	       cout << "bytes before the current datablock: "<<beforeheader <<endl ;
	       cout << "size of the current datablock so far: "<<(out.tellp()-  beforeheader) << " bytes ("<<mNumberOfBitmaps<<" bitmaps)"<<endl ;
	       cerr <<"expected size of the bitmap on disk: "<<membuffer[k].sizeOnDisk()<< " bytes"<<endl;
	       cerr << "aborting!"<<endl;
	       break;
	     }
	     membuffer[k].reset();
	     assert(membuffer[k].sizeInBits() == 0);
	}
	//locations[mNumberOfBitmaps] = static_cast<uint>(out.tellp() - locations_pos);
	if(verboseBlockWrites) {
		cout << " data block using "<< (out.tellp() - locations_pos) << " on disk" <<endl;
		cout << " mNumberOfBitmaps =  "<< mNumberOfBitmaps  <<endl;
		cout << " mBlockSize =  "<< mBlockSize <<endl;
	}
	if(!out) cerr<<"something went wrong while saving the bitmaps"<<endl;
	if(verbose) cout << "CompressedDatablock  uses "<<(out.tellp()-  beforeheader) << " bytes in total, header overhead is "<< (headersize*100.0/(out.tellp()-beforeheader))<<" %"<<endl ;
	if(verbose) cout <<" predicted size is " <<(mStorageUsage*sizeof(uword) + mNumberOfBitmaps*(sizeof(uword))+sizeof(uint)*3+sizeof(uint64)+sizeof(int)+sizeof(uint)*(mNumberOfBitmaps+1))<<endl;
	if(verbose) cout <<" predicted header size is " <<(sizeof(uint)*3+sizeof(uint64)+sizeof(int)+sizeof(uint)*(mNumberOfBitmaps+1))<<endl;
	//if(verbose) cout <<" mStorageUsage " <<mStorageUsage<<endl;
	//if(verbose) cout << "CompressedDatablock  estimated datablock overhead "<< ((headersize+2*mNumberOfBitmaps*sizeof(uint))*100.0/(out.tellp()-beforeheader))<<" %"<<endl ;
	if(verbose) if((headersize*100.0/(out.tellp()-beforeheader))>5)
		cout << "The header overhead is too high. Increase the data block size."<<endl;
	//out.seekp(locations_pos);
	//out.write(reinterpret_cast<char *>(& locations[0]), sizeof(uint)*(mNumberOfBitmaps+1));
	//out.seekp(locations[mNumberOfBitmaps] + locations_pos);
	if(!out) cerr<<"I didn't manage to go back in the stream to write the positions!"<<endl;
	//for(uint k = 0; k < membuffer.size(); ++k) membuffer[k].reset();
	assert(RowCounter == 0);
	mStorageUsage = 0;
	assert(empty());
	assert(!full());
}


template <class uword>
bool CompressedDataBlock<uword>::empty() {
	if(mNumberOfBitmaps == 0) return true;
	if(RowCounter > 0)
		return false;
	//for(int k = 0 ; k < mNumberOfBitmaps; ++k)
	//	if(membuffer[0].sizeInBits() > 0) return false;
	return true;
}

/*
bool CompressedDataBlock::haveBeenLoaded(const vector<uint> & queryset) {
	if(queryset.size() == 0) return true;
	uint expectedsize = membuffer[queryset[0]].sizeInBits();
	if(expectedsize == 0)
		return false;
	for(uint k = 1; k < queryset.size(); ++k) {
			if(membuffer[queryset[k]].sizeInBits() != expectedsize)
				return false;
	}
	return true;
}*/

template <class uword>
void ReadCompressedDataBlock<uword>::printout(ostream &o) {
	for(typename map<uint,EWAHBoolArray<uword> >::iterator i = membuffer.begin(); i != membuffer.end(); ++i)
		i->second.printout(o);
}
/*
template <class uword>
void ReadCompressedDataBlock<uword>::logicaland(const vector<uint> & queryset, EWAHBoolArray<uword> & container) {
	if(queryset.size() == 0) {
		cout << "warning: did you mean to send an empty query?" << endl;
		container.reset();
		return;
	}
	container =  membuffer[queryset[0]];
	EWAHBoolArray<uword>  tmpcontainer;
    for(uint k = 1; k < queryset.size(); ++k) {
    	container.logicaland(membuffer[queryset[k]],tmpcontainer);
    	container.swap(tmpcontainer);
    }
}

template <class uword>
void ReadCompressedDataBlock<uword>::sparselogicaland(const vector<uint> & queryset, EWAHBoolArray<uword> & container) {
	if(queryset.size()<2)
		throw noneedforlogicaloperation_exception("please, do not try to AND a bitmap with itself");
	membuffer[queryset[0]].sparselogicaland(membuffer[queryset[1]],container);
	EWAHBoolArray<uword>  tmpcontainer;
    for(uint k = 2; k < queryset.size(); ++k) {
    	container.sparselogicaland(membuffer[queryset[k]],tmpcontainer);
    	container.swap(tmpcontainer);
    }
}

template <class uword>
void ReadCompressedDataBlock<uword>::rawlogicaland(const vector<uint> & queryset, EWAHBoolArray<uword> & container) {
	if(queryset.size()<2)
		throw noneedforlogicaloperation_exception("please, do not try to AND a bitmap with itself");
	EWAHBoolArray<uword>  tmpcontainer;
	membuffer[queryset[0]].rawlogicaland(membuffer[queryset[1]],container);
    for(uint k = 2; k < queryset.size(); ++k) {
    	container.rawlogicaland(membuffer[queryset[k]],tmpcontainer);
    	container.swap(tmpcontainer);
    }
}*/



#endif /*COMPRESSEDDATABLOCK_H_*/
