#ifndef GRAYCODESORTING_H_
#define GRAYCODESORTING_H_

#include <sys/stat.h> 
#include <sys/types.h>
#include <errno.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <sstream>
#include <villa.h> // QDBM, see http://qdbm.sourceforge.net/, please download and install!, 
// Daniel: QDBM has a C++ API, but villa is the C API which should do fine
// We use villa because it has sorted keys access (b-tree).
// under macos type "fink install qdbm14"
//
/// PLEASE try to make sure that your version of QDBM had ZLIB compression enabled at build time.
//
//#include <gmp.h> // you need the GNU MP library. Under MacOS, type "fink install gmp"
//#include <gmpxx.h>
#include "CSVParser.h"
#include "BitmapAllocation.h"
#include "BitmapWriter.h"
#include "CompressedDataBlock.h"
//#include "BigUnsigned.hh" // part of the bigint C++ library
//#include "BigIntegerUtils.hh"

using namespace std;
typedef unsigned int uint;



//VLCFUNC VL_CMPINTARRAY = vlintarraycompare;

/*
static uint comparisoncounter = 0;
static inline int vlintarraycompare(const char *aptr, int asiz, const char *bptr, int bsiz){
  //assert(aptr && asiz >= 0 && bptr &&  asiz == bsiz);
  const int * aintptr = reinterpret_cast<const int *>( aptr);
  const int * bintptr = reinterpret_cast<const int *>( bptr);
  ++comparisoncounter;
  for(uint i = 0; i < asiz/sizeof(int); i++) {
    if(aintptr[i] != bintptr[i]) 
            return aintptr[i] - bintptr[i];
  }
  return 0;
}
*/



class FileUtil {// always better to not put things in global namespace...
  public:
   static int mkdir_parentsonly(const char *path);
   static uint64 getFileSize(const char* fileName);
};

class GrayCodeCSVSorter {
	public:

	    enum sortingtype {gray=1, lexi=2};
		GrayCodeCSVSorter(string FileName, sortingtype t, bool reverseorder, string CSVFileName,  string delimiter = ",", char  commentmarker= '#',  	KofNBitmapBudget& bb = g1ofNBitmapBudget, BitmapAllocation & ba = gStandardBitmapAllocation );
		void /* GrayCodeCSVSorter::*/ dumpSortedRows(string file);
	    int getNumberOfRecords() {return vlrnum(mVilla);}
	    uint64 getSizeOfBTreeOnDisk() { return FileUtil::getFileSize(mFileName.c_str());}
	    virtual ~GrayCodeCSVSorter() {close();}

	    void close();
	    enum {sparsemode = true};

private:

    void sync();
    
    // return true if the value was added
    inline bool add(const char * key, const uint keysize, const char * value, const uint valuesize){
    	  if (! vlput(mVilla, key, keysize, value, valuesize, VL_DDUP)) {
    	        cerr  << __FILE__ << ":"<< __LINE__ << " I didn't manage to store the data in "<< mFileName <<": " <<dperrmsg(dpecode)<< endl;
    	        return false;
    	  }
    	  return true;
    }
    
    void open() throw(ifstream::failure);
    
	//inline void toBitmapRowFromValues(const vector<string> & fact, vector<ulong> & bits);
	//inline void toGrayCode(BigUnsigned  & graycoderank);

	//inline void set(vector<uint> & bits,const uint bitindex);
    string mCSVFileName;
	string mFileName; // B-tree file
    VILLA *mVilla;
    
    //vector<stringuintmultimap > tobitmap;// = mBA.computeBitmapAllocation(*mBB,mHistograms);
    uint numberofbitmaps;
    sortingtype  type;
    bool mReverseorder;
};
/*
static const BigUnsigned bu1 = 1;	



static void toGrayCodeRank(BigUnsigned & bits) {
	uint shift = 1;
	BigUnsigned shf (bits >> shift);
	while (! shf.isZero()) {
		bits ^= shf;
	    shift <<= 1;
	    shf = bits >> shift;
	}
}

static void toGrayCodeRankFromLocationOfOnes(const uint * ones, int size, BigUnsigned & bu) {
	for (int k = 0 ; k < size; ++k)
		bu += bu1 << ones[k];
	toGrayCodeRank(bu);
}

static void toGrayCode(BigUnsigned  & graycoderank) {
	graycoderank = graycoderank^(graycoderank>>1);
}
*/

/*
static int vlSLOWgraycompare(const char *aptr, int asiz, const char *bptr, int bsiz){
	  //cout <<"call to compare..." <<endl;
	  const uint * ones1 = reinterpret_cast<const uint *>( aptr);
	  const uint * ones2 = reinterpret_cast<const uint *>( bptr);
	  BigUnsigned  bu1;
	  BigUnsigned  bu2;
	  toGrayCodeRankFromLocationOfOnes(ones1, asiz/sizeof(uint), bu1);
	  toGrayCodeRankFromLocationOfOnes(ones2, bsiz/sizeof(uint), bu2);
	  //cout <<"call to compare...ok" <<endl;
	  if(bu1 == bu2) return 0;
	  if( bu1 > bu2 ) return 1;
	  return -1;
	  //return 0;
	
}*//*
static int vlgraycompare(const char *aptr, int asiz, const char *bptr, int bsiz){
	  //cout <<"call to compare..." <<endl;
	  const ulong * data1 = reinterpret_cast<const ulong *>( aptr);
	  const ulong * data2 = reinterpret_cast<const ulong *>( bptr);
	  BigUnsigned  bu1 (easyDataToBI<ulong>(data1, asiz/sizeof(uword), BigInteger::positive));
	  BigUnsigned  bu2 (easyDataToBI<ulong>(data2, bsiz/sizeof(uword), BigInteger::positive));
	  if(bu1 == bu2) return 0;
	  if( bu1 > bu2 ) return 1;
	  return -1;
}*/

static int vlsparsegraycompare(const char *aptr, int asiz, const char *bptr, int bsiz){
	  //cout <<"call to compare..." <<endl;
	  const uint * data1 = reinterpret_cast<const uint *>( aptr);
	  const uint * data2 = reinterpret_cast<const uint *>( bptr);
	  bool normalorder = true;
	  const uint mins = asiz/sizeof(uint) > bsiz/sizeof(uint) ? bsiz/sizeof(uint) : asiz/sizeof(uint);
	  for(uint p = 0 ; p < mins; ++p) {
		  if(data1[p] > data2[p])
		      return normalorder ? 1 : -1;
		  if(data1[p] < data2[p])
		      return normalorder ? -1 : 1;
		  normalorder = not(normalorder);
	  }
	  if(asiz > bsiz ) return normalorder ? -1 : 1;
	  if(asiz < bsiz )return normalorder ? 1 : -1;
	  //if(asiz == bsiz) : 
	  return 0; 
}

static int vlsparsegraycompare_reverse(const char *aptr, int asiz, const char *bptr, int bsiz){
	  //cout <<"call to compare..." <<endl;
	  const uint * data1 = reinterpret_cast<const uint *>( aptr);
	  const uint * data2 = reinterpret_cast<const uint *>( bptr);
	  bool normalorder = true;
	  const uint mins = asiz/sizeof(uint) > bsiz/sizeof(uint) ? bsiz/sizeof(uint) : asiz/sizeof(uint);
	  for(uint p = 0 ; p < mins; ++p) {
		  if(data1[p] > data2[p])
		      return normalorder ? -1 : 1;
		  if(data1[p] < data2[p])
		      return normalorder ? 1 : -1;
		  normalorder = not(normalorder);
	  }
	  if(asiz > bsiz ) return normalorder ? 1 : -1;
	  if(asiz < bsiz )return normalorder ? -1 : 1;
	  //if(asiz == bsiz) : 
	  return 0; 
}

static int vlsparselexicompare(const char *aptr, int asiz, const char *bptr, int bsiz){
	  //cout <<"call to compare..." <<endl;
	  const uint * data1 = reinterpret_cast<const uint *>( aptr);
	  const uint * data2 = reinterpret_cast<const uint *>( bptr);
	  const uint mins = asiz/sizeof(uint) > bsiz/sizeof(uint) ? bsiz/sizeof(uint) : asiz/sizeof(uint);
	  for(uint p = 0 ; p < mins; ++p) {
		  if(data1[p] > data2[p])
		      return  1 ;
		  if(data1[p] < data2[p])
		      return -1;
	  }
	  if(asiz > bsiz ) return  -1;
	  if(asiz < bsiz )return  1;
	  //if(asiz == bsiz) : 
	  return 0; 
}

static int vlsparselexicompare_reverse(const char *aptr, int asiz, const char *bptr, int bsiz){
	  //cout <<"call to compare..." <<endl;
	  const uint * data1 = reinterpret_cast<const uint *>( aptr);
	  const uint * data2 = reinterpret_cast<const uint *>( bptr);
	  const uint mins = asiz/sizeof(uint) > bsiz/sizeof(uint) ? bsiz/sizeof(uint) : asiz/sizeof(uint);
	  for(uint p = 0 ; p < mins; ++p) {
		  if(data1[p] > data2[p])
		      return  -1 ;
		  if(data1[p] < data2[p])
		      return 1;
	  }
	  if(asiz > bsiz ) return  1;
	  if(asiz < bsiz )return  -1;
	  //if(asiz == bsiz) : 
	  return 0; 
}

static void toBitmapPositionsFromValues(const vector<string> & fact, const vector<stringuintmultimap > & tobitmap, 
		vector<uint> & pos) {
	if(tobitmap.size() != fact.size() ) {
		cerr << "bad histogram, size mismatch, please delete the .histo file."<<endl;
		throw corruptedhistogram_exception ("bad histogram."); 
	}
	for (uint k = 0; k < fact.size(); ++k) {
		stringuintmultimap::const_iterator p = tobitmap[k].find(fact[k]);
		const uint nbr = tobitmap[k].count(fact[k]);
		if(nbr == 0 ) {
			cerr << "bad histogram, please delete the .histo file."<<endl;
			throw corruptedhistogram_exception ("bad histogram."); 
		}
		for (uint j = 0; j < nbr; ++j) {
			pos.push_back(p->second);
			++p;
		}
	}
	sort(pos.begin(),pos.end());
}
/*
static void toBigUnsignedFromValues(const vector<string> & fact, const vector<stringuintmultimap > & tobitmap, 
		BigUnsigned & pos) {
	for (uint k = 0; k < fact.size(); ++k) {
		stringuintmultimap::const_iterator p = tobitmap[k].find(fact[k]);
		const uint nbr = tobitmap[k].count(fact[k]);
		for (uint j = 0; j < nbr; ++j) {
			pos += bu1 <<(p->second);
			++p;
		}
	}
}*/
/*
void GrayCodeCSVSorter::set(vector<uint> & bits, const uint bitindex) {
	const uint bitsinulong = 8 * sizeof(ulong);
	bits[bitindex / bitsinulong] |= 1UL << (bitindex % bitsinulong));
}
*/
#endif /*GRAYCODESORTING_H_*/
