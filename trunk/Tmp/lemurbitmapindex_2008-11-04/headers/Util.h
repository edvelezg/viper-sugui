#ifndef UTIL_H_
#define UTIL_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include "bitmapexceptions.h"
using namespace std;


//using namespace std::tr1;
typedef multimap<string,uint> stringuintmultimap;
//typedef unordered_multimap<string,uint> stringuintmultimap;
typedef map<string,uint> stringuintmap;

typedef unsigned int uint;
typedef unsigned int uint32;
typedef unsigned long long int uint64;
typedef unsigned long  ulong;

typedef unsigned short ushort;
// forcing 32-bit words on all machines
typedef unsigned short  uword16;
typedef unsigned int  uword32;
typedef unsigned long long  uword64;
//typedef unsigned long long  uword;

//assert(sizeof(ushort)==2);
//assert(sizeof(uword)>=4);
/**
 * Util is really meant for common functions, such as computing the factorial
 * or trimming strings.
 * Daniel moved functions having to do with allocating bitmaps to
 * BitmapAllocation.h
 */


inline uint min(uint x, uint y) {
	return x > y ? y : x;
}


inline uint max(uint x, uint y) {
	return x > y ? x : y;
}

template <class uword>
inline string toBinaryString(const uword w) {
  stringstream s;
 for(uint k = 0; k <sizeof(uword)*8;++k) {
	if(w & (1<<k)) s <<"1"; else s << "0";
 }
 string ans;
 s >>ans;
 return ans;
}

/**
 * usual pair object, but with <�redefined.
 * 20 May 2008: ofk refines to break ties using the first component
 */
template <class U, class V>
class PairComparePrimarilyOnLast {
public:
	PairComparePrimarilyOnLast(U &u , V & v) : first(u), second(v) {}
	PairComparePrimarilyOnLast(const PairComparePrimarilyOnLast &other): first(other.first), second(other.second) {}
	PairComparePrimarilyOnLast(const pair<U,V> &other): first(other.first), second(other.second) {}
	U first;
	V second;
	bool operator<(const PairComparePrimarilyOnLast& other) const {
          if (second != other.second)
            return second < other.second;
          else// break ties
            return first < other.first;}

	bool operator>(const PairComparePrimarilyOnLast& other) const {
          if (second != other.second)
            return second > other.second;
          else// break ties
            return first > other.first;}
	PairComparePrimarilyOnLast& operator=(const PairComparePrimarilyOnLast& other) {first= other.first;second = other.second;return *this;}
};

/**
 * Compute n choose k
 */
//static inline uint  choose(uint n, uint k);

void loadVectorMapFromTabSeparatedText(vector<map<string,uint> > & answer, istream & in);
void saveVectorMapAsTabSeparatedText(vector<map<string,uint> > & answer, ostream & out);
/*
 * dump on the screen
 */
template<class T>
void printout(vector<T> x) {
	for(typename vector<T>::iterator i = x.begin(); i != x.end();++i)
		cout << *i << endl;
}

template<class X>
X absolute(X x) {
	return x <0? -x : x;
}


template<class T>
void printline(vector<T> x) {
	for(typename vector<T>::iterator i = x.begin(); i != x.end();++i)
		cout << *i << " ";
		cout <<endl;
}


/**
 * See http://www.codeproject.com/vcpp/stl/stdstringtrim.asp
 * by Rodrigo C. Dias
 */
inline void trim(string& str) {
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
}



/**
 * Computes n choose k
 */
static inline uint  choose(const uint n, const uint k) {
  if (k < 0 || k > n) return 0;
  if (k==0 || n==k) return 1;
  if (k==1 || n==k-1) return n;

  long num=1, denom=1;
  for (uint i=0; i < k; ++i) {
    num *= (n-i);
    denom *= (i+1);
  }

  return  num / denom;
 // assert(ans >= 0);
  //  cout << n << " choose " << k << " is " << ans << endl;
 // return static_cast<uint>(ans);
}


// create all paths, but assume the path is a file
int mkdir_parentsonly(const char *path);
#endif /*UTIL_H_*/
