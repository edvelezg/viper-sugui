#ifndef BITMAPALLOCATION_H_
#define BITMAPALLOCATION_H_

#include <vector>
#include <map>
#include <functional>
#include <numeric>
#include <map>  
#include <cmath>  
#include <string>
#include <algorithm>
#include <assert.h>
//#include <tr1/unordered_map>
#include "Util.h"
#include "bitmapexceptions.h"

using namespace std;
typedef vector<stringuintmultimap > bitmapalloctype ;
typedef vector<stringuintmap > histogramtype;


/**********************
 * Bitmap Budget is a way to tell the software how many bitmaps
 * to allocate.
 *********************/
/*class BitmapBudget {
public:
	BitmapBudget() {}
	virtual ~BitmapBudget(){}
	virtual uint howManyBitmaps(const uint numItems) = 0;
	virtual string name() = 0;
};*/
/// this is the generic solution:
class KofNBitmapBudget  { /*: public BitmapBudget*/
public:
	KofNBitmapBudget(uint k, bool smart): mK(k), mSmart(smart) {}
	~KofNBitmapBudget(){}
	uint howManyBitmaps(const uint numItems);
	inline string name() {return "kofn";}
	inline uint getK(const uint numValues,const uint bitsReq) {
		if(mSmart) return guessK(numValues,bitsReq);
		else return mK;
	}

	/*
	 *  returns suitable M for k-of-M coding
	 */
	static int numBitsRequired(const  int numCodes, const int k);
	static uint guessK(const uint numValues,const uint bitsReq);
private:
	uint mK;
	bool mSmart;
};


static KofNBitmapBudget g1ofNBitmapBudget = KofNBitmapBudget(1,true);

static KofNBitmapBudget g2ofNBitmapBudget = KofNBitmapBudget(2,true);


///this is when you really know what you are doing,
// provided as an example, really:
/*class PredefinedBitmapBudget  : public BitmapBudget{
public:
	PredefinedBitmapBudget(vector<uint> & Ns):mNs(Ns) {}
	~PredefinedBitmapBudget(){}
	inline uint howManyBitmaps(const uint numItems) {
		return mNs[dimIndex];
	}
	inline string name() {return "predefined";}
private:
	vector<uint> mNs;
};
*/







/*********************
 * Bitmap allocation provides algorithms to match attribute
 * values with bitmaps
 *********************/

class BitmapAllocation {
public: 
	BitmapAllocation(){}
	virtual ~BitmapAllocation() {};
	virtual void  computeBitmapAllocation(KofNBitmapBudget & bb, const histogramtype & histogram, bitmapalloctype & out)  = 0;
	virtual string name() =0;

	/**
	 * return the number of bitmaps we will need to implement this bitmap allocation.
	 */
	static uint numberOfBitmaps(const vector<stringuintmultimap > &);
	template<class InputIterator>//stringuintmap::const_iterator
	static void addToKofNBitmapLayout(const InputIterator &  begin, const InputIterator &  end, stringuintmultimap & layout, const uint L, const uint codingK, const uint bitsUsedPrevDims);
	/**
	 * Something contributed by Owen to support k-of-M encoding
	 */
	//static vector<int> codeKofM(const int seqNo, const int k, const int M);

};



//BitmapAllocation::addToKofNBitmapLayout
class AlphabeticalBitmapAllocation : public BitmapAllocation{

public: 	
	AlphabeticalBitmapAllocation() {}
	
	/**
	 * given a histogram, returns the bitmap allocation. Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	void  computeBitmapAllocation(KofNBitmapBudget& bb, const histogramtype & histogram, bitmapalloctype & out) ;
	/**
	 * Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	//static vector<uint> numbersOfBitmaps(int codingK, const histogramtype&  histogram, bool AllowReduceK);
	string name() {return "AlphabeticalBitmapAllocation";}


};

//BitmapAllocation::addToKofNBitmapLayout
class WeightSortedBitmapAllocation : public BitmapAllocation{

public: 	
	WeightSortedBitmapAllocation() {}
	
	/**
	 * given a histogram, returns the bitmap allocation. Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	void  computeBitmapAllocation(KofNBitmapBudget& bb, const histogramtype & histogram, bitmapalloctype & out) ;
	/**
	 * Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	//static vector<uint> numbersOfBitmaps(int codingK, const histogramtype&  histogram, bool AllowReduceK);
	string name() {return "WeightSortedBitmapAllocation";}


};


//BitmapAllocation::addToKofNBitmapLayout
class GrayCodeBitmapAllocation : public BitmapAllocation{

public: 	
	GrayCodeBitmapAllocation(bool r = false) : reverse(r)  {}
	
	/**
	 * given a histogram, returns the bitmap allocation. Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	void  computeBitmapAllocation(KofNBitmapBudget& bb, const histogramtype & histogram, bitmapalloctype & out) ;
	/**
	 * Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	//static vector<uint> numbersOfBitmaps(int codingK, const histogramtype&  histogram, bool AllowReduceK);
	string name() {return "GrayCodeBitmapAllocation";}
	bool reverse;

};



//BitmapAllocation::addToKofNBitmapLayout
class GrayAllCodeBitmapAllocation : public BitmapAllocation{

public: 	
	GrayAllCodeBitmapAllocation(bool r = false) : reverse(r) {}
	
	static void generateAllGrayCodes(const uint bitsReq, const uint codingK, vector<vector<uint> > & sortedbygray, bool reverse);
	


    
	
	/**
	 * given a histogram, returns the bitmap allocation. Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	void  computeBitmapAllocation(KofNBitmapBudget& bb, const histogramtype & histogram, bitmapalloctype & out) ;
	/**
	 * Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	//static vector<uint> numbersOfBitmaps(int codingK, const histogramtype&  histogram, bool AllowReduceK);
	string name() {return "GrayAllCodeBitmapAllocation";}
	bool reverse;

};
//BitmapAllocation::addToKofNBitmapLayout
class WeightSortedGrayCodeBitmapAllocation : public BitmapAllocation{

public: 	
	WeightSortedGrayCodeBitmapAllocation(bool r = false) : reverse(r){}
	
	/**
	 * given a histogram, returns the bitmap allocation. Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	void  computeBitmapAllocation(KofNBitmapBudget& bb, const histogramtype & histogram, bitmapalloctype & out) ;
	/**
	 * Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	//static vector<uint> numbersOfBitmaps(int codingK, const histogramtype&  histogram, bool AllowReduceK);
	string name() {return "WeightSortedGrayCodeBitmapAllocation";}
    enum {verbose_mai2008=false};
    bool reverse;
};

class WeightSortedGrayAllCodeBitmapAllocation : public BitmapAllocation{

public: 	
	WeightSortedGrayAllCodeBitmapAllocation(bool r = false, bool sortindescendingweightorder = false) : reverse(r),descendingweightorder(sortindescendingweightorder) {}
	
	/**
	 * given a histogram, returns the bitmap allocation. Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	void  computeBitmapAllocation(KofNBitmapBudget& bb, const histogramtype & histogram, bitmapalloctype & out) ;
	/**
	 * Here codingK
	 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
	 */
	//static vector<uint> numbersOfBitmaps(int codingK, const histogramtype&  histogram, bool AllowReduceK);
	string name() {return "WeightSortedGrayAllCodeBitmapAllocation";}
	bool reverse,descendingweightorder;
};


/***
 * The standard bitmap allocation does not really attempt to make
 * use of the histogram, it only uses the number of distinct attribute
 * values in each dimension.
 * 
 * As long as k (as in k-of-N encoding) is small and the number of attribute
 * values is large, then there will be far more 1s than 0s in the
 * bitmaps. Intuitively, we can simply try to minimize (further) the 
 * number of ones by mixing different encodings (1-of-N, 2-of-N,...) so that
 * more frequent items are coded using unary codes (1-of-N) whereas
 * less frequent items are coding using more redondant codes (3-of-N,  for example).
 * 
 * We try to make it so that the number of bitmaps stays the same whether you
 * use the AdaptiveBitmapAllocation or the StandardBitmapAllocation (for
 * a fixed value of codingK).  
 * 
 */
/*class AdaptiveBitmapAllocation : public BitmapAllocation{

public: 	
	// we may allow some extra bitmaps to ensure that optimization can kick in
	AdaptiveBitmapAllocation() {}//uint extrabitmaps = 0 ): mExtraBitmaps(extrabitmaps) {}
	string name() {return "AdaptiveBitmapAllocation";}
	bitmapalloctype  computeBitmapAllocation(KofNBitmapBudget& bb, const histogramtype & histogram) ;
	
	//void setExtraBitmaps(uint extrabitmaps) {mExtraBitmaps = extrabitmaps;}
private: 
	// L is the number of bitmaps
	static void optimalBitmapLayout(const stringuintmap & thishisto, stringuintmultimap & layout, const uint L, const uint startingbitindex);
	static vector<uint> dynamicProgramming(const stringuintmap & thishisto, const uint maxk, const uint L);
	static vector<uint> sortedFrequencies(const stringuintmap & thishisto);
	static vector<uint> optimalTripartition(const stringuintmap & thishisto, const uint L);

	//uint mExtraBitmaps;
};*/



static AlphabeticalBitmapAllocation gStandardBitmapAllocation;
//static AdaptiveBitmapAllocation gAdaptiveBitmapAllocation;






#endif /*BITMAPALLOCATION_H_*/
