/*
 * FastLogicalAndOperations.h
 *
 *  Created on: Oct 17, 2008
 *      Author: daniel
 */

#ifndef FASTLOGICALANDOPERATIONS_H_
#define FASTLOGICALANDOPERATIONS_H_
#include <vector>
#include "CompressedDataBlock.h"

using namespace std;

template <class uword>
class BigAnd {
public:
	enum {careful=true};
	BigAnd(vector<EWAHBoolArray<uword> > & its, vector<bool> & sign,
			EWAHBoolArray<uword> & out) : largestrunofzeroes(0),
			iterators(), container(out),msign(sign),sib(its.begin()->sizeInBits())  {
				cout<<"number of rows = "<<its.begin()->sizeInBits()<<endl;
		vector<bool>::iterator si = msign.begin();
		for(typename vector<EWAHBoolArray<uword> >::iterator i = its.begin();
		          i!= its.end(); ++i,++si) {
			EWAHBoolArrayRawIterator<uword> it (i->raw_iterator());
			BufferedRunningLengthWord<uword> & j (it.next());
			if (j.getRunningBit() == !(*si))
				if(largestrunofzeroes <j.getRunningLength())
					largestrunofzeroes= j.getRunningLength();
			iterators.push_back(it);
		}
	}

	bool eatZeroWords() {
		//container.add
		cout<<"adding "<<largestrunofzeroes<<" zeroes"<<endl;
		container.addStreamOfEmptyWords(0, largestrunofzeroes);
		// append the number of 0x00 to the solution
		// next, clear out counters
		// updating all along largestrunofzeroes
		return discardWordsAndCheckForEmptyRLW(largestrunofzeroes);
	}

	bool discardWordsAndCheckForEmptyRLW(const uint wordstodiscard) {
		cout<<"wordstodiscard = "<<wordstodiscard<<endl;
		/*if(careful) if(largestrunofzeroes < wordstodiscard) {
			cout<<"largestrunofzeroes = "<<largestrunofzeroes<<endl;
			cout<<"wordstodiscard = "<<wordstodiscard<<endl;
			throw "";}*/
		largestrunofzeroes -= wordstodiscard;
		vector<bool>::iterator si = msign.begin();
		for(typename vector<EWAHBoolArrayRawIterator<uword> >::iterator i = iterators.begin();
		          i!= iterators.end(); ++i, ++si) {
			uint discard(0);//wordstodiscard
			while(discard < wordstodiscard) {
				if(i->rlw.size() > wordstodiscard) {
					i->rlw.discardFirstWords(wordstodiscard);
					break;
				} else {
					discard += i->rlw.size();
					if(!i->hasNext())
						return false;
					i->next();
				}
			}
			if(i->rlw.getRunningBit() == !(*si))
				if(largestrunofzeroes <i->rlw.getRunningLength())
					largestrunofzeroes= i->rlw.getRunningLength();
		}
		return true;
	}

	bool eatDirtyWords() {
		const uint shortestblock(findShortestBlock());
		vector<uword> data(shortestblock,~static_cast<uword>(0));
		vector<bool>::iterator si = msign.begin();
		for(typename vector<EWAHBoolArrayRawIterator<uword> >::iterator i = iterators.begin();
		          i!= iterators.end(); ++i,++si) {
			if(*si) __logicaland(data,*i);
			else __logicalandnot(data,*i);
		}
		for(typename vector<uword>::iterator j = data.begin(); j!=data.end();++j)
			container.add(*j);
		return discardWordsAndCheckForEmptyRLW(shortestblock);

	}

	void __logicaland(vector<uword> & data, EWAHBoolArrayRawIterator<uword> & brlw) {
		// first skip any 1x11 words
		//
		if(careful and (brlw.rlw.getRunningLength()>0)) if(!brlw.rlw.getRunningBit()) throw "";
		if(careful) if(brlw.rlw.size()<data.size()) throw "";
		const uint rl1= brlw.rlw.getRunningLength();
		for(uint k = 0; k + rl1 < data.size(); ++k)
			data[k+rl1] |= brlw.dirtyWords()[k];
	}

	void __logicalandnot(vector<uword> & data, EWAHBoolArrayRawIterator<uword> & brlw) {
		// first skip any 1x11 words
		//
		if(careful and (brlw.rlw.getRunningLength()>0)) if(brlw.rlw.getRunningBit()) throw "";
		const uint rl1= brlw.rlw.getRunningLength();
		for(uint k = 0; k + rl1 < data.size(); ++k)
			data[k+rl1] |=  ! ( brlw.dirtyWords()[k] );
	}
	uint findShortestBlock() {
		uint ans(iterators.begin()->rlw.size());
		for(typename vector<EWAHBoolArrayRawIterator<uword> >::iterator i = iterators.begin();
		          i!= iterators.end(); ++i) {
			if(ans> i->rlw.size())
				ans = i->rlw.size();
		}
		if(careful)for(typename vector<EWAHBoolArrayRawIterator<uword> >::iterator i = iterators.begin();
			i!= iterators.end(); ++i) {
				if(ans> i->rlw.size()) throw "";
		}
		return ans;
	}

	void process() {
		while(largestrunofzeroes>0) {
			cout<<"largestrunofzeroes = "<<largestrunofzeroes<<endl;
			if(!eatZeroWords()) return;
		}
		while(largestrunofzeroes==0) {
			cout<<"largestrunofzeroes = "<<largestrunofzeroes<<endl;
			if(!eatDirtyWords()) return;
		}
		container.setSizeInBits(sib);
	}


	uint largestrunofzeroes;
	vector<EWAHBoolArrayRawIterator<uword> >  iterators;
	EWAHBoolArray<uword> & container;
	vector<bool> & msign;
	uint sib;
	//static const uword zero = 0;
	//static const uword notzero=~zero;
};

#endif /* FASTLOGICALANDOPERATIONS_H_ */
