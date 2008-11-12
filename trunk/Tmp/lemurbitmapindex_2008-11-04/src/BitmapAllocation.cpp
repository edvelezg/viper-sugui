#include "BitmapAllocation.h"

uint BitmapAllocation::numberOfBitmaps(const vector<stringuintmultimap > & m) {
	// a safe way to do it is just to go through and find the largest uint
	uint size = 0;
	for(uint k = 0 ; k < m.size() ; ++k ) {
		  const stringuintmultimap & current = m[k];
		  for(stringuintmultimap::const_iterator iter = current.begin(); iter != current.end(); iter++ ) {
			  if(iter->second > size) size = iter->second;
		  }
		
	}
	++size;
	return size;
}

/**
 * given a histogram, returns the bitmap allocation. Here codingK
 * is the 1 for unary bitmaps, 2 for 2-of-N bitmaps and so on.
 */
/*bitmapalloctype StandardBitmapAllocation::computeBitmapAllocation(int codingK, const histogramtype&  histogram) {
	bitmapalloctype tobitmap(histogram.size());
	uint bitsUsedPrevDims(0);
	for(uint k = 0 ; k < histogram.size() ; ++k ) {
	  //uint counter(0);
	  const map<string,uint> thishisto = histogram[k];
	  const uint numValues=thishisto.size();
	  uint bitsReq = numBitsRequired(numValues,codingK); // try 2-of- coding
	  if(mAllowReduceK) {
		  codingK = softReductionOfCodingK(bitsReq, codingK, numValues);
		  bitsReq = numBitsRequired(numValues,codingK);
	  }
	  StandardBitmapAllocation::standardBitmapLayout(thishisto.begin(), thishisto.end(), tobitmap[k], bitsReq,codingK, bitsUsedPrevDims);
	  bitsUsedPrevDims += bitsReq;
	}
	return tobitmap;
}*/



void AlphabeticalBitmapAllocation::computeBitmapAllocation(KofNBitmapBudget & bb, const histogramtype&  histogram, bitmapalloctype & tobitmap) {
	tobitmap.resize(histogram.size());
	uint bitsUsedPrevDims(0);
	for(uint k = 0 ; k < histogram.size() ; ++k ) {
	  //uint counter(0);
	  const stringuintmap & thishisto = histogram[k];
	  const uint numValues=thishisto.size();
	  uint bitsReq = bb.howManyBitmaps(numValues);
	  uint codingK = bb.getK(numValues,bitsReq);
	  BitmapAllocation::addToKofNBitmapLayout(thishisto.begin(), thishisto.end(), tobitmap[k], bitsReq,codingK, bitsUsedPrevDims);
	  bitsUsedPrevDims += bitsReq;
	}
	//return tobitmap;
}

void WeightSortedBitmapAllocation::computeBitmapAllocation(KofNBitmapBudget & bb, const histogramtype&  histogram, bitmapalloctype & tobitmap) {
	tobitmap.resize(histogram.size());
	uint bitsUsedPrevDims(0);
	for(uint k = 0 ; k < histogram.size() ; ++k ) {
	  //uint counter(0);
	  const stringuintmap & thishisto = histogram[k];
	  const uint numValues=thishisto.size();
	  uint bitsReq = bb.howManyBitmaps(numValues);
	  uint codingK = bb.getK(numValues,bitsReq);
	  vector<PairComparePrimarilyOnLast<string,uint> > sortedbyweight;
	  try {
	    for(stringuintmap::const_iterator i = thishisto.begin(); i!= thishisto.end();++i)
	    	sortedbyweight.push_back(PairComparePrimarilyOnLast<string,uint>(*i));
	    sort(sortedbyweight.begin(), sortedbyweight.end());
	  } catch (std::bad_alloc& e) {
		  cerr<<" not enough memory for sorting the attribute values by weight on dim. "<<k<<"! trying to continue..."<<endl;
		  BitmapAllocation::addToKofNBitmapLayout(thishisto.begin(), thishisto.end(), tobitmap[k], bitsReq,codingK, bitsUsedPrevDims);
		  bitsUsedPrevDims += bitsReq;
		  continue;
	  }
	  BitmapAllocation::addToKofNBitmapLayout(sortedbyweight.begin(), sortedbyweight.end(), tobitmap[k], bitsReq,codingK, bitsUsedPrevDims);
	  bitsUsedPrevDims += bitsReq;
	}
	//return tobitmap;
}
/*
struct  graycompareless : public binary_function<vector<uint>, vector<uint>, bool> {
bool operator()(const vector<uint> & a, const vector<uint> & b) {
	bool normalorder = true;
	const uint mins = a.size() > b.size() ? b.size() : a.size();
	 for(uint p = 0 ; p < mins; ++p) {
		  if(a[p] > b[p])
		      return normalorder ? true : false;
		  if(a[p] < b[p])
		      return normalorder ? false : true;
		  normalorder = not(normalorder);
	  }
	  if(a.size() > b.size() ) return normalorder ? false : true;
	  if(a.size() < b.size() )return normalorder ? true : false;
	  return false; 	
}
};*/


static bool graycompareless(const vector<uint> & a, const vector<uint> & b) {
	bool normalorder = true;
	const uint mins = a.size() > b.size() ? b.size() : a.size();
	 for(uint p = 0 ; p < mins; ++p) {
		  if(a[p] > b[p])
		      return normalorder ? true : false;
		  if(a[p] < b[p])
		      return normalorder ? false : true;
		  normalorder = not(normalorder);
	  }
	  if(a.size() > b.size() ) return normalorder ? false : true;
	  if(a.size() < b.size() )return normalorder ? true : false;
	  return false; 	
}

static bool graycomparegreater(const vector<uint> & a, const vector<uint> & b) {
	bool normalorder = true;
	const uint mins = a.size() > b.size() ? b.size() : a.size();
	 for(uint p = 0 ; p < mins; ++p) {
		  if(a[p] > b[p])
		      return normalorder ? false : true;
		  if(a[p] < b[p])
		      return normalorder ? true : false;
		  normalorder = not(normalorder);
	  }
	  if(a.size() > b.size() ) return normalorder ? true : false;
	  if(a.size() < b.size() )return normalorder ? false : true;
	  return false; 	
}


void GrayCodeBitmapAllocation::computeBitmapAllocation(KofNBitmapBudget & bb, const histogramtype&  histogram, bitmapalloctype & tobitmap) {
	tobitmap.resize(histogram.size());
	uint bitsUsedPrevDims(0);
	for(uint k = 0 ; k < histogram.size() ; ++k ) {
	  //uint counter(0);
	  const stringuintmap & thishisto = histogram[k];
	  const uint numValues=thishisto.size();
	  uint bitsReq = bb.howManyBitmaps(numValues);
	  uint codingK = bb.getK(numValues,bitsReq);
	  BitmapAllocation::addToKofNBitmapLayout(thishisto.begin(), thishisto.end(), tobitmap[k], bitsReq,codingK, bitsUsedPrevDims);
	  bitsUsedPrevDims += bitsReq;
	  try {
		    vector<vector<uint> > sortedbygray(histogram[k].size(),vector<uint>(codingK,0));
		    uint valcounter(0);
		    stringuintmap::const_iterator i = thishisto.begin();
		    for(; i!= thishisto.end();++i) {
		    	uint counter = 0; 
		    	//cout <<i->first<<endl;
		    	const stringuintmultimap & bitalloc = tobitmap[k];
		    	stringuintmultimap::const_iterator j = bitalloc.find(i->first);
		    	stringuintmultimap::const_iterator jend = bitalloc.upper_bound(i->first);
		    	for(; j!=jend;++j) {
		    		sortedbygray[valcounter][counter] = j->second;
		    		++counter;
		    	}
		    	sort(sortedbygray[valcounter].begin(),sortedbygray[valcounter].end());
		    	++valcounter;
		    }
		    tobitmap[k].clear();
		    sort(sortedbygray.begin(),sortedbygray.end(), graycompareless);
		    uint j (0);
		    stringuintmap::const_iterator i2 = thishisto.begin();
		    for(; i2 != thishisto.end();++i2) {
		    	const string & myval = i2->first;
		    	vector<uint> & pos = sortedbygray[j];
		    	for(vector<uint>::iterator w = pos.begin();w!=pos.end();++w)
		    		tobitmap[k].insert(pair<string,uint>(myval,*w));
		    	++j;
		    }
		    //cout<<"done"<<endl;
	  }catch (std::bad_alloc& e) {
		  cerr<<" not enough memory for gray code sorting of the bitmap allocation on dim. "<<k<<"! trying to continue..."<<endl;
		  continue;
	  }
	}
	//return tobitmap;
}

void GrayAllCodeBitmapAllocation::generateAllGrayCodes(const uint bitsReq, const uint codingK, vector<vector<uint> > & sortedbygray, bool reverse = false) {
  const uint totalnumberofcodes = choose(bitsReq,codingK);
  sortedbygray.clear();
  sortedbygray.resize(totalnumberofcodes,vector<uint>(codingK,0));
  vector<uint> positionOfOnes(codingK,0);
  // initialize
  for(uint i = 1; i < positionOfOnes.size(); ++i)
 	positionOfOnes[i] = i;		    
  for(uint i = 0 ; i < totalnumberofcodes;++i) {
	sortedbygray[i]=positionOfOnes;
	uint toincrement = positionOfOnes.size() - 1;
	while(( positionOfOnes[toincrement] == bitsReq - (positionOfOnes.size()  - toincrement) ) and ( toincrement > 0)) {
		--toincrement;
	}
	++positionOfOnes[toincrement];
	for(uint i = toincrement + 1; i < positionOfOnes.size(); ++i)
		positionOfOnes[i] = positionOfOnes[toincrement] + i - toincrement;
  }
  sort(sortedbygray.begin(),sortedbygray.end(), reverse ? graycompareless : graycomparegreater);
}

void GrayAllCodeBitmapAllocation::computeBitmapAllocation(KofNBitmapBudget & bb, const histogramtype&  histogram,bitmapalloctype & tobitmap) {
	tobitmap.resize(histogram.size());
	uint bitsUsedPrevDims(0);
	for(uint k = 0 ; k < histogram.size() ; ++k ) {
	  //uint counter(0);
	  const stringuintmap & thishisto = histogram[k];
	  const uint numValues=thishisto.size();
	  uint bitsReq = bb.howManyBitmaps(numValues);
	  uint codingK = bb.getK(numValues,bitsReq);
	  try {
		  vector<vector<uint> > sortedbygray;
		  generateAllGrayCodes(bitsReq, codingK, sortedbygray,reverse);
		  tobitmap[k].clear();
		  uint j (0);
		  stringuintmap::const_iterator i2 = thishisto.begin();
		  for(; i2 != thishisto.end();++i2) {
		    	const string & myval = i2->first;
		    	vector<uint> & pos = sortedbygray[j];
		    	for(vector<uint>::iterator w = pos.begin();w!=pos.end();++w)
		    		tobitmap[k].insert(pair<string,uint>(myval,*w+bitsUsedPrevDims));
		    	++j;
		  }
	  } catch (std::bad_alloc& e) {
		  cerr<<" not enough memory for gray code sorting of the bitmap allocation on dim. "<<k<<"! trying to continue..."<<endl;
		  BitmapAllocation::addToKofNBitmapLayout(thishisto.begin(), thishisto.end(), tobitmap[k], bitsReq,codingK, bitsUsedPrevDims);
	  }
	  bitsUsedPrevDims += bitsReq;
	}
	//return tobitmap;
}


void WeightSortedGrayCodeBitmapAllocation::computeBitmapAllocation(KofNBitmapBudget & bb, const histogramtype&  histogram, bitmapalloctype & tobitmap) {
	tobitmap.resize(histogram.size());
	uint bitsUsedPrevDims(0);
	for(uint k = 0 ; k < histogram.size() ; ++k ) {
	  //uint counter(0);
	  const stringuintmap & thishisto = histogram[k];
	  const uint numValues=thishisto.size();
	  uint bitsReq = bb.howManyBitmaps(numValues);
	  uint codingK = bb.getK(numValues,bitsReq);
	  BitmapAllocation::addToKofNBitmapLayout(thishisto.begin(), thishisto.end(), tobitmap[k], bitsReq,codingK, bitsUsedPrevDims);
	  bitsUsedPrevDims += bitsReq;
	  try {
		    vector<vector<uint> > sortedbygray(histogram[k].size(),vector<uint>(codingK,0));
		    uint valcounter(0);
		    stringuintmap::const_iterator i = thishisto.begin();
		    for(; i!= thishisto.end();++i) {
		    	uint counter = 0; 
		    	//cout <<i->first<<endl;
		    	const stringuintmultimap & bitalloc = tobitmap[k];
		    	stringuintmultimap::const_iterator j = bitalloc.find(i->first);
		    	stringuintmultimap::const_iterator jend = bitalloc.upper_bound(i->first);
		    	for(; j!=jend;++j) {
		    		sortedbygray[valcounter][counter] = j->second;
		    		++counter;
		    	}
		    	sort(sortedbygray[valcounter].begin(),sortedbygray[valcounter].end());
		    	++valcounter;
		    }
		    sort(sortedbygray.begin(),sortedbygray.end(), reverse ? graycompareless : graycomparegreater);//);
		    vector<PairComparePrimarilyOnLast<string,uint> > sortedbyweight;
		    for(stringuintmap::const_iterator i = thishisto.begin(); i!= thishisto.end();++i)
		      	sortedbyweight.push_back(PairComparePrimarilyOnLast<string,uint>(*i));
		    sort(sortedbyweight.begin(), sortedbyweight.end());
		    uint j (0);
		    vector<PairComparePrimarilyOnLast<string,uint> >::const_iterator i2 = sortedbyweight.begin();
		    tobitmap[k].clear();
		    for(; i2 != sortedbyweight.end();++i2) {
		    	const string & myval = i2->first;
		    	vector<uint> & pos = sortedbygray[j];
		    	///////////////////////////
		    	if(verbose_mai2008) {
		    		cout<<"value "<<myval<<" [freq="<<i2->second<<"] gets bitmaps ";
		    		for(vector<uint>::iterator w = pos.begin();w!=pos.end();++w)
		    			cout << *w<<" ";
		    		cout<<endl;
		    	}
		    	//////////////////////////
		    	for(vector<uint>::iterator w = pos.begin();w!=pos.end();++w)
		    		tobitmap[k].insert(pair<string,uint>(myval,*w));
		    	++j;
		    }
		    //cout<<"done"<<endl;
	  }catch (std::bad_alloc& e) {
		  cerr<<" not enough memory for gray code sorting of the bitmap allocation on dim. "<<k<<"! trying to continue..."<<endl;
		  continue;
	  }
	}
	//return tobitmap;
}

void WeightSortedGrayAllCodeBitmapAllocation::computeBitmapAllocation(KofNBitmapBudget & bb, const histogramtype&  histogram, bitmapalloctype & tobitmap) {
	tobitmap.resize(histogram.size());
	uint bitsUsedPrevDims(0);
	for(uint k = 0 ; k < histogram.size() ; ++k ) {
	  //uint counter(0);
	  const stringuintmap & thishisto = histogram[k];
	  const uint numValues=thishisto.size();
	  uint bitsReq = bb.howManyBitmaps(numValues);
	  uint codingK = bb.getK(numValues,bitsReq);
	  try {
		    vector<vector<uint> > sortedbygray;
		    GrayAllCodeBitmapAllocation::generateAllGrayCodes(bitsReq, codingK, sortedbygray,reverse);
		    vector<PairComparePrimarilyOnLast<string,uint> > sortedbyweight;
		    for(stringuintmap::const_iterator i = thishisto.begin(); i!= thishisto.end();++i)
		      	sortedbyweight.push_back(PairComparePrimarilyOnLast<string,uint>(*i));
		    //descendingweightorder ? greater<PairComparePrimarilyOnLast<string,uint> >() :
		    if(descendingweightorder)
		      sort(sortedbyweight.begin(), sortedbyweight.end(), greater<PairComparePrimarilyOnLast<string,uint> >() );
		    else
		      sort(sortedbyweight.begin(), sortedbyweight.end(), less<PairComparePrimarilyOnLast<string,uint> >() );
		    uint j (0);
		    vector<PairComparePrimarilyOnLast<string,uint> >::const_iterator i2 = sortedbyweight.begin();
		    tobitmap[k].clear();
		    for(; i2 != sortedbyweight.end();++i2) {
		    	const string & myval = i2->first;
		    	vector<uint> & pos = sortedbygray[j];
		    	for(vector<uint>::iterator w = pos.begin();w!=pos.end();++w)
		    		tobitmap[k].insert(pair<string,uint>(myval,*w+bitsUsedPrevDims));
		    	++j;
		    }
		    //cout<<"done"<<endl;
	  }catch (std::bad_alloc& e) {
		  cerr<<" not enough memory for gray code sorting of the bitmap allocation on dim. "<<k<<"! trying to continue..."<<endl;
		  BitmapAllocation::addToKofNBitmapLayout(thishisto.begin(), thishisto.end(), tobitmap[k], bitsReq,codingK, bitsUsedPrevDims);
	  }
	  bitsUsedPrevDims += bitsReq;
	}
	//return tobitmap;
}



uint KofNBitmapBudget::guessK(const uint numValues,const uint bitsReq) {
	for(uint k = 1; k <9; ++k) {
		if(static_cast<uint>(numBitsRequired(numValues,k)) <= bitsReq)
			return k;
	}
	throw notenoughbitmaps_exception("try adding more bitmaps");
}

/*
uint StandardBitmapAllocation::softReductionOfCodingK(uint bits, const uint maxcodingk, uint numValues) {
	uint codingk (maxcodingk);
	while(((uint) choose(bits,codingk-1)>=numValues) and (codingk>=2)) --codingk;
	//if(codingk <maxcodingk)
	//	cout <<"I was asked to use "<< maxcodingk<<"-of-N encoding, but I am switching to " <<codingk<<"-of-N encoding."<<endl;
	return codingk;
}*/

//void StandardBitmapAllocation::standardBitmapLayout(const map<string,uint> & thishisto, multimap<string,uint> & layout, uint L, uint bitsUsedPrevDims) {
//	  map<string,uint>::const_iterator iter;   
//	  for( iter = thishisto.begin(); iter != thishisto.end(); iter++ ) {
//int codingK
/*
bitmapalloctype AdaptiveBitmapAllocation::computeBitmapAllocation(KofNBitmapBudget & bb, const histogramtype & histogram) {
	//if(codingK == 1) {
		// there won't be any optimization to do, so let us not waste any time.
		//StandardBitmapAllocation sba(true);
		//return sba.computeBitmapAllocation(codingK,histogram);
	//}
	bitmapalloctype tobitmap(histogram.size());
	uint bitsUsedPrevDims(0);
	for(uint k = 0 ; k < histogram.size() ; ++k ) {
		const stringuintmap & thishisto = histogram[k];
		const uint numValues = thishisto.size();
		const int bitsReq = bb.howManyBitmaps(numValues);//StandardBitmapAllocation::numBitsRequired(numValues,codingK);
		optimalBitmapLayout(thishisto, tobitmap[k], bitsReq,bitsUsedPrevDims ); 
		bitsUsedPrevDims += bitsReq;
	}
	return tobitmap;
}*/
/*template<class InputIterator>//stringuintmap::const_iterator
void BitmapAllocation::addToKofNBitmapLayout(const InputIterator &  begin, const InputIterator &  end, stringuintmultimap & layout, const uint L, const uint codingK, const uint bitsUsedPrevDims) {
	InputIterator iter;
	uint counter(0);
	if(codingK > 1) {
	  uint seqNo;
	  int k, M;
	  for( iter = begin; iter != end; iter++ ) {
	    //vector<int> onesPositions = codeKofM(counter++, codingK, L);
	    //uint 
	    seqNo = counter++;
	    //int 
	    k = codingK;
	    //int 
	    M = L;
	    for (; M > 0;  --M)  {
	        // the first M-1 \choose k of the M-choose-k codes start with 0
	    	const uint  chmodM1k= choose(M-1,k);
	        if (seqNo >= chmodM1k) {
	        	layout.insert(pair<string,uint>(iter->first,bitsUsedPrevDims + M-1)) ;
	            seqNo-=chmodM1k; k--;
	            if(k==0) break;
	        }
	        
	    }
	    //counter++;
	    //assert(onesPositions.size() == static_cast<uint>(codingK));
	    //for (uint i=0; i < onesPositions.size(); ++i)
	     // layout.insert(pair<string,uint>(iter->first,bitsUsedPrevDims + onesPositions[i])) ;
	  }
	} else // k= 1
		  for( iter = begin; iter != end; iter++ ) 
		    layout.insert(pair<string,uint>(iter->first,bitsUsedPrevDims + counter++));
		  
}*/
template<class InputIterator>//stringuintmap::const_iterator
void BitmapAllocation::addToKofNBitmapLayout(const InputIterator &  begin,
		const InputIterator &  end, 
		stringuintmultimap & layout, 
		const uint L, 
		const uint codingK, const uint bitsUsedPrevDims) {
	vector<uint> positionOfOnes(codingK,0);
	// initialize
	for(uint i = 1; i < positionOfOnes.size(); ++i)
		positionOfOnes[i] = i;
	InputIterator iter;
	for(iter = begin; iter != end; iter++) {
		//cout <<"for "<<iter->first<<endl;
		//for(uint i = 0; i < positionOfOnes.size(); ++i)
		//	cout<<positionOfOnes[i]<<" ";
		//cout<<endl;
		for(uint i = 0; i < positionOfOnes.size(); ++i)
			layout.insert(pair<string,uint>(iter->first,
					bitsUsedPrevDims + positionOfOnes[i]));
		// next, we increment the counter
		uint toincrement = positionOfOnes.size() - 1;
		while(( positionOfOnes[toincrement] == L - (positionOfOnes.size()  - toincrement) ) and ( toincrement > 0)) {
			--toincrement;
		}
		++positionOfOnes[toincrement];
		for(uint i = toincrement + 1; i < positionOfOnes.size(); ++i)
			positionOfOnes[i] = positionOfOnes[toincrement] + i - toincrement;
	}
}
/*
void  AdaptiveBitmapAllocation::optimalBitmapLayout(const stringuintmap & thishisto, stringuintmultimap & layout, uint L, uint startingbitindex) {
	//
	 // TODO: I need to document this code a lot better
	 // 
	 // What we do is that we seek the best solution of the form
	 // 
	 // 1-of-N1, 2-of-N2, ...
	 // 
	 // with the constraint that N1+N2+...= L
	 // 
	 // The objective function is the number of 1s.
	 //
	const uint n = thishisto.size();// number of items
	
	 // now we are going to work out from frequency using dynamic programming
	// the goal is to minimize the number of ones
	 // 
	 // We build a matrix that requires maxk * (L+1) * (n+1) storage. This is
	 // at most n (n+1)**2 / 2. If the cardinality is too large, then we have
	 // a problem! However, choosing an appropriately large k shoudl fix 
	 // the problem. However, it is not practical to use a large value of k.
	 // 
	 // Note that if the storage is O(L**2 n), the computational cost is O(L**3 n) (or more).
	 // 
	//uint maxk = 2;
	//cout <<"# using optimal tripartition encoding
	vector<uint> Ns = optimalTripartition(thishisto,L);
	//try {
	//	Ns = dynamicProgramming(thishisto, maxk,L);
	//} catch(notenoughbitmaps_exception neb) {
	//	maxk = L/2;
	//	Ns = dynamicProgramming(thishisto, maxk,L);
	//}
	// Ok, so now N contains our solution, but just to make sure...
	//assert((uint) accumulate(Ns.begin(),Ns.end(),0) == n);
	// at this point in time, the vector Ns contains the solution I was looking for!
	// now I need to actually build my solution.... gah!
	//multimap<string,uint> layout;
	{
		uint bitsUsedPrevEncoding (0);
		stringuintmap::const_iterator ibegin = thishisto.begin();
		stringuintmap::const_iterator iend = ibegin;
		uint codedsofar(0);
		uint codingK(1);
		vector<uint>::const_iterator Nsiter;
		for(Nsiter = Ns.begin(); Nsiter != Ns.end() ; ++Nsiter) {
			const  uint numberwewillcode = min(n-codedsofar,choose(*Nsiter,codingK));
			for(uint k = 0; k < numberwewillcode; ++k ) ++iend;
			//cout <<"numberwewillcode = "<<numberwewillcode<<" layout.size()="<<layout.size()<<endl;
			StandardBitmapAllocation::standardBitmapLayout(ibegin, iend, layout, *Nsiter, codingK, bitsUsedPrevEncoding + startingbitindex);
			ibegin = iend;
			bitsUsedPrevEncoding += *Nsiter;
			codedsofar += numberwewillcode;
			++codingK;
	  }
	  assert(codedsofar == n);
	  assert(ibegin == thishisto.end());
	  assert(iend == thishisto.end());
	}
	//return layout;
}*/


/**
 * this will use the bitmaps with 1-of-N encoding and 2-of-N encoding and 3-of-N encoding, only.
 * No 4-of-N encoding, 5-of-N encoding and so on.
 * It is an easier problem than the general case
 */
/*vector<uint> AdaptiveBitmapAllocation::optimalTripartition(const stringuintmap & thishisto, const uint L) {
  const uint n = thishisto.size();// number of items
  // next line should translate as a proposition in a paper
  if ( (L < n) and  (choose(L,2)<n) and (choose(L,3)<n) )
    throw notenoughbitmaps_exception("try adding more bitmaps, I don't have enough for tripartition encoding!");
  vector<uint> freqs = sortedFrequencies(thishisto);
  assert(freqs.size() == n);
  if(n > 1)
	  assert(accumulate(freqs.begin(), freqs.begin()+2,0.0)== freqs[0]+freqs[1]);
  uint bestL1 = 0;
  uint bestL2 = 0;
  double lowestcost = 1e30;
  if(L >= n) {// no time to waste
	  bestL1 = L;
	  bestL2 = 0;
	  lowestcost = accumulate(freqs.begin(), freqs.end(),0.0);
  } else {
    for(uint L1 = 0; L1 <= L; ++L1) {
	  // assume we have 1-of-L1 encoding
	  // we need to figure out a best choice for L2 and L3
	  // but that's an easy problem, just maximize L2
	  const uint nleftover = n - L1;
	  const uint Lleftover = L-L1;
	  if ( ( choose(Lleftover,2) < nleftover) and ( choose(Lleftover,3) < nleftover) )// no solution possible
	    continue;
	  uint L2;
	  // first of all, can we just use 2-of-N?
	  if(choose(Lleftover,2) >= nleftover) {
		  L2 = Lleftover;
	  } else {
		  // find the largest value of L_2 that will do!
		  assert(choose(Lleftover,3)>=nleftover);
		  for(L2=0; L2<= Lleftover;++L2)
			  if(choose(L2,2)+choose(Lleftover-L2,3) < nleftover) 
				  break;
		  assert(L2>0);
		  --L2;
	  }
	  const uint L3 = L - L1 - L2;
	  const uint coded1ofN1 = min(L1,n);
	  //cout <<"n="<<n<<" coded1ofN1="<<coded1ofN1<<endl;
	  assert(coded1ofN1<=n);
	  const uint coded2ofN2 = min(choose(L2,2),n-coded1ofN1);
	  //cout <<"n="<<n<<" coded1ofN1="<<coded1ofN1<<" coded2ofN2="<<coded2ofN2<<endl;
	  assert(coded1ofN1+coded2ofN2<=n);
	  const uint coded3ofN3 = min(choose(L3,3),n-coded1ofN1-coded2ofN2);
	  //cout <<"n="<<n<<" coded1ofN1="<<coded1ofN1<<" coded2ofN2="<<coded2ofN2<<" coded3ofN3="<<coded3ofN3<<endl;
	  //cout <<"coded1ofN1+coded2ofN2+coded3ofN3="<<coded1ofN1+coded2ofN2+coded3ofN3<<endl;
	  //cout <<"n="<<n<<endl;
	  //cout <<"coded1ofN1+coded2ofN2+coded3ofN3-n "<<(coded1ofN1+coded2ofN2+coded3ofN3-n)<<endl;
	  //cout <<"coded1ofN1+coded2ofN2+coded3ofN3==n "<<(coded1ofN1+coded2ofN2+coded3ofN3==n)<<endl;
	  assert(coded1ofN1+coded2ofN2+coded3ofN3==n);

	  const double cost = accumulate(freqs.begin(), freqs.begin()+coded1ofN1,0.0) 
	  + 2 * accumulate(freqs.begin()+coded1ofN1, freqs.begin()+coded1ofN1+coded2ofN2,0.0)
	  + 3 * accumulate(freqs.begin()+coded1ofN1+coded2ofN2, freqs.begin()+coded1ofN1+coded2ofN2+coded3ofN3,0.0);
	  if(cost < lowestcost) {
		  lowestcost = cost;
		  bestL1 = L1;
		  bestL2 = L2;
	  }
    }
  }
  if(lowestcost >= 1e30)
	  throw toomanybits_exception("please, give me more bitmaps");
  const uint bestL3 = L - bestL1 - bestL2;
  const uint coded1ofN1 = min(bestL1,n);
  assert(coded1ofN1<=n);
  const uint coded2ofN2 = min(choose(bestL2,2),n-coded1ofN1);
  assert(coded1ofN1+coded2ofN2<=n);
  const uint coded3ofN3 = min(choose(bestL3,3),n-coded1ofN1-coded2ofN2);
  assert(coded1ofN1+coded2ofN2+coded3ofN3==n);
  vector<uint> Ns(3,0);
  Ns[0] = KofNBitmapBudget::numBitsRequired(coded1ofN1, 1);
  Ns[1] = KofNBitmapBudget::numBitsRequired(coded2ofN2, 2);
  Ns[2] = KofNBitmapBudget::numBitsRequired(coded3ofN3, 3);
  assert(Ns[0] + Ns[1] + Ns[2] <= L);
  return Ns;
}*/



/**
 * this will use the bitmaps with 1-of-N encoding and 2-of-N encoding, only. No 3-of-N encoding.
 * It turns out that it simplifies the problem quite a bit.
 */
/*vector<uint> AdaptiveBitmapAllocation::optimalBipartition(const map<string,uint> & thishisto, const uint L) {
	const uint n = thishisto.size();// number of items
	// first check that we have enough bitmaps
	if(static_cast<uint>(choose(L,2))<n)
		throw notenoughbitmaps_exception("try adding more bitmaps, I don't have enough for 2-of-N encoding!");
	vector<uint> freqs = sortedFrequencies(thishisto);
	double lowestcost = 1e30;
	int bestN1 = -1;
	for(uint N1 = 0; N1 <= L ;++N1) {
		const uint N2 = L-N1;
		const uint coded1ofN1 = min(N1,n);
		const uint coded2ofN2 = min(choose(N2,2),n-coded1ofN1);
		if(coded1ofN1 + coded2ofN2 < n) break;
		assert(freqs.begin()+coded1ofN1+coded2ofN2 == freqs.end());
		// next line can be made faster with prefix sums!
		const double cost = accumulate(freqs.begin(), freqs.begin()+coded1ofN1,0.0) + 2 * accumulate(freqs.begin()+coded1ofN1, freqs.begin()+coded1ofN1+coded2ofN2,0.0);
		if(cost < lowestcost) {
			lowestcost = cost;
			bestN1 = N1;
		}
	}
	vector<uint> Ns(2,0);
	Ns[0] = bestN1;
	Ns[1] = StandardBitmapAllocation::numBitsRequired(n-min(bestN1,n), 2);//L - bestN1;
	//cout << Ns[0]<<" "<<Ns[1]<<endl;
	return Ns;
}*/
/*
vector<uint> AdaptiveBitmapAllocation::sortedFrequencies(const stringuintmap & thishisto) {
	//const uint n = thishisto.size();// number of items
	multimap<int,string,greater<int> > frequency;
	stringuintmap::const_iterator i;
	for( i = thishisto.begin(); i != thishisto.end(); i++ ) {
		frequency.insert(pair<int,string>(i->second, i->first));
	}
	assert(thishisto.size() == frequency.size());
	//assert(n == frequency.size());
	// for convenience, we shall recopy the frequencies in an array
	vector<uint> freqs;
	//vector<string> names;
	multimap<int,string>::const_iterator j;
	for(j = frequency.begin(); j != frequency.end();++j) {
		freqs.push_back(j->first);
		//names.push_back(j->second);
	}
	return freqs;	
}*/
/*
vector<uint> AdaptiveBitmapAllocation::dynamicProgramming(const stringuintmap & thishisto, const uint maxk, const uint L) {
	if(maxk <= 3) return optimalTripartition(thishisto, L);
	const uint n = thishisto.size();// number of items
	if(maxk * (n+1)* (L+1) >= 100000) 
		throw problemtoolarge_exception ("too large for in-memory optimization, try increasing k");
	vector<uint> freqs = sortedFrequencies(thishisto);
	// we must make sure they are sorted in inverse order
	//for(uint t = 0; t<freqs.size()-1;++t)
	//	assert(freqs[t]>=freqs[t+1]);
	const vector<double> initializeddoublevector(L+1,1e30) ;
	const vector<vector<double> > initializeddoublematrix(n+1,  initializeddoublevector );
	vector<vector<vector<double> > > M(  maxk,   initializeddoublematrix );// used to store the cost (defined as the number of ones)
	const vector<int> initializedintvector(L+1,-1);
	const vector<vector<int> > initializedintmatrix(n+1,  initializedintvector);
	vector<vector<vector<int> > > Msol(maxk,initializedintmatrix);
	{// BEGIN DYN. PROG.
		//uint makk = L/2; // this should be formalized, but it makes not sense
		// to consider k-of-N codes for k>N/2, right?
		//
		// we are allowed to use k-of-N1, (k+1)-of-N2, ...
		for(uint k = maxk; k > 0; --k) {
			// finding best solution for items m to n
			for(uint m = 0; m <= n;++m) {
				for(uint bitbudget = 0; bitbudget <= L;++bitbudget) {
					if(k == maxk) {// special case
						Msol[maxk - k][m][bitbudget] = bitbudget;
						//if(verbose) cout << "Msol["<< (maxk - k) << " ]["<<m<<"]["<<bitbudget<<"] = "<<bitbudget<<endl;
						const uint numberwecancode = choose(bitbudget,k);
						if(n-m > numberwecancode) {
							//M[maxk - k][m][bitbudget] = UINT_MAX;
						} else {
							//if(verbose) cout << "M["<< (maxk - k) << " ]["<<m<<"]["<<bitbudget<<"] = "<<(k * accumulate(freqs.begin()+m, freqs.end(),0))<<endl;
							M[maxk - k][m][bitbudget] = k * accumulate(freqs.begin()+m, freqs.end(),0) ;
						}
					} else {// generic case
				          for(uint N = 0 ; N <=bitbudget; ++N) {//using k-of-N coding from m to c
				        	  const uint numberwecancode = choose(N,k);
				        	  const uint numberweshallcode = min(numberwecancode,n-m);
				        	  const uint probak = accumulate(freqs.begin()+m, freqs.begin()+m+numberweshallcode,0);
				        	  const double Mrest = M.at(maxk - k - 1).at(m+numberweshallcode).at(bitbudget-N);
				        	  const double cost = probak * k + Mrest;
				        	  if(M[maxk - k][m][bitbudget] > cost) {
				        		  M[maxk - k][m][bitbudget] = cost;
				        		  Msol[maxk - k][m][bitbudget] = N;
				        	  }//if(M[maxk - k][m][bitbudget] > cost)
					      }//for(uint N = 0 ; N <=bitbudget; ++N)
					}//if(k == maxk)
				}//for(uint bitbudget = 0; bitbudget <= L;++bitbudget)
			}//for(uint m = 0; m <= n;++n) 
		}//for(uint k = maxk; k > 0; --k) 
	}//END DYN. PROG.
	vector<uint> Ns;
	{// BEGIN POST-PROCESS DYN. PROG.
		uint k (1);
		uint m (0);
		uint bitbudget (L);
		//if(verbose) cout << "bitbudget left = " << bitbudget << " items left = "<< (n-m) << " k = " << k << endl; 
		uint N = Msol.at(maxk-k).at(m).at(bitbudget);
		//if(verbose) cout <<" N =  "<< N <<endl;
		//const uint bestcost = M.at(maxk-k).at(m).at(bitbudget);
		//if(sverbose) cout <<" Number of ones will be "<< bestcost <<endl;
		M.clear(); // we no longer need M!
		Ns.push_back(N);
		while(maxk-k>0) { 
		      const uint numberwecancode = choose(N,k);
		      const uint numberweshallcode = min(numberwecancode,n-m);
		      m += numberweshallcode;
		      if(n-m == 0) break;
		      bitbudget -=  N;
		      ++k;
		      //if(verbose) cout << "bitbudget left = " << bitbudget << " items left = "<< (n-m) << " k = " << k << " maxk = "<< maxk << endl; 
		      N = Msol.at(maxk-k).at(m).at(bitbudget);
		      //if(verbose) cout <<" N =  "<< N <<endl;
		      Ns.push_back(N);
		}//while(maxk-k>0)
	}// END POST-PROCESS DYN. PROG.
	return Ns;
}*/



uint KofNBitmapBudget::howManyBitmaps(const uint numItems) {
	/**
	 * this whole thing is a freaking hack,  but I am trying to tame it.
	 * TODO: replace this hack by something less icky.
	 */
	  if(numItems == 0) return 0;
	  if(mK>4)
		  cerr<<"I don't support k-of-N encoding above 4-of-N. Adapting."<<endl;
	  //assert(mK < 9); // for numerical stability?
	  uint effectivek = mK < 5 ? mK : 4;
	  if(mSmart) {
	  if(numItems<= 5) // forN=5, nothing else but 1-of-N makes sense 
		  effectivek = 1;
	  else if(numItems<= 21) // for N=10, nothing else but 1-of-N or 2-of-N makes sense 
		  effectivek = effectivek <= 2? effectivek : 2;
	  else if(numItems<=85)
	  	  effectivek = effectivek <= 3? effectivek : 3;
	  }
	  double rootofnumCodes = pow(numItems,1.0/effectivek);
	  assert(rootofnumCodes >= 0.0000001);// numerical stability
	  int maxM = static_cast<int>(ceil(rootofnumCodes *effectivek + 1));
	  int minM = static_cast<int>(floor(rootofnumCodes* effectivek / exp(1)));
	  assert(maxM>=minM); // sanity check
	  for (int trialM = minM; trialM <= maxM; ++trialM) {
	    if (static_cast<uint>(choose(trialM, effectivek)) >= numItems) {
	    	if(mSmart) {
	    		uint guessedk = KofNBitmapBudget::guessK(numItems,trialM);
		    	if(guessedk != mK)  
		    		cerr<<"You requested "<<mK<<"-of-N, but a dimension has "<<numItems<<" att. values, switching to "<<guessedk<<"-of-"<<trialM<<" for this dimension"<<endl;
	    	}
	    	//if(guessedk != effectivek)
	    	//	cerr<<"guessedk = "<<guessedk<<", effectivek = "<<effectivek<<endl;
	    	return trialM;
	    }
	  }
	  cerr << "need too many bits" << endl;
	  cerr << "your program may not live through a horrible, horrible death" << endl;
	  cerr << "I hope this code is never used in an airplane" << endl;
	  throw toomanybits_exception("need too many bits");
	
}
/*
 *  returns suitable M for k-of-M coding
 * 
 * Update: Daniel derived some inequalities (see bitmapallocationmath.tex)
 */
//c^{1/k} k +1 \geq m\geq c^{1/k} k / e
int KofNBitmapBudget::numBitsRequired(const  int numCodes, const int k) {
  if(numCodes == 0) return 0;
  // should use a binary search or solve analytically.
  assert(k < 9); // for numerical stability?
  double rootofnumCodes = pow(numCodes,1.0/k);
  assert(rootofnumCodes >= 0.0000001);// numerical stability
  int maxM = static_cast<int>(ceil(rootofnumCodes *k + 1));
  int minM = static_cast<int>(floor(rootofnumCodes* k / exp(1)));
  assert(maxM>=minM); // sanity check
  for (int trialM = minM; trialM <= maxM; ++trialM) {
    if (choose(trialM, k) >= static_cast<uint>(numCodes)) return trialM;
  }
  cerr << "need too many bits" << endl;
  cerr << "your program may not live through a horrible, horrible death" << endl;
  cerr << "I hope this code is never used in an airplane" << endl;
  throw toomanybits_exception("need too many bits");
}
/**
 * Something contributed by Owen to support k-of-M encoding
 */
/*vector<int> BitmapAllocation::codeKofM(int seqNo, int k, int M)
{
  vector<int> ans;
  //  cout << " express " << seqNo << " as a " << k << " of " << M << endl;
  
  // expensive assertion.
  assert( M >= numBitsRequired( seqNo+1, k));

  // converted from a tail recursion
  for (; M > 0;  --M)  {
    // the first M-1 \choose k of the M-choose-k codes start with 0
	const uint  chmodM1k= choose(M-1,k);
    if (static_cast<uint>(seqNo) < chmodM1k) {
      // cout << "0"; 
      // tail call: codeKofM(seqNo, k, M-1);
    } else {
      // then come the next batch of codes, those with 1.
      ans.push_back(M-1);  // bit position of a 1.
      // cout << "1";
      // we next account for all the codes starting with 0 that we skipped
      // tail call: codeKofM(seqNo-choose(M-1,k), k-1, M-1);
      seqNo-=chmodM1k; k--;
    }
  }
  return ans;
}*/

