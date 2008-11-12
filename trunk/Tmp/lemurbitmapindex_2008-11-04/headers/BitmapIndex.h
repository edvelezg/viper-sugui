#ifndef BITMAPINDEX_H_
#define BITMAPINDEX_H_

#include <fstream>
#include <map>
#include <vector>
#include <set>
#include "BitmapWriter.h"
#include "DataBlock.h"
#include "CompressedDataBlock.h"
#include "BitmapAllocation.h"
#include "bitmapexceptions.h"
#include "LogicalOperations.h"
#include "querylanguage.h"
using namespace std;
/**
 * Once you have created an index file (using class BitmapWriter),
 * you can then use this class to query some data.
 *
 *
 *
 * How it works:
 *  - you point it at a file
 *  - it then reads the header, building b-trees that serve later
 *  to answer queries (mostly, it maps attribute values to bitmaps)
 */

template <class uword>
class BlockIterator;

class BitmapIndex {
public:
    BitmapIndex(const string & indexfile/*,  bool bufferfirstblock = true*/);
    virtual ~BitmapIndex();

    // the integers being return point to rows
    // the query object is a map describing the desired rows...
    // for example, if you want the first column to have value "cat"
    // and the second column to have value "dog", you have to pass
    // a map satisfying
    // map[0] = "cat";map[1] = "dog"
   // void equalityQuery(const map<uint, string> & query, vector<ulong> & result, const bool sparse = false);
    void process(const OrthogonalQuery & oq, vector<ulong> & result, int hint = HINT_NONE);


    // return in byte the size of the preloaded data block

    uint sizeOfPreloadedBlock() {
        return locationoffirstunreadblock - startofdata;
    }
    // return in byte the size of the head

    uint sizeOfHeader() {
        return startofdata;
    }

    enum {
        version = BitmapWriter<uint>::version
    };

    AttributeValueMap avm;


    //void computeQuerySet(const map<uint, string> & query, vector<uint> & querysetfrombtree);
    void readHeader(ifstream & in);
    //vector<stringuintmultimap > mCode;
    fstream::pos_type startofdata; // where does the data start (and header ends)
    fstream::pos_type locationoffirstunreadblock; // location of the second block
    //DB FirstBlock; // the first block!
    string mFilename;
    //bool FirstBlockEnabled;
    //friend class BlockIterator<DB>;
    uint mCompressionMode;
    uint mNumberOfBits;
    uint getNumberOfBits() const { return mNumberOfBits;}
    void dumpText(const string &fname);


private:
    template <class uword>
    void equalityQuery_templated(const OrthogonalQuery & oq, vector<ulong> & result, const int hint); //const map<uint, string> & query, vector<ulong> & result, const bool sparse = false);
    template <class uword>
    void rangeQuery_templated(const OrthogonalQuery & oq, vector<ulong> & result, const int hint);

    template <class uword>
    void dumpText_templated(const string &fname);

};

BitmapIndex::BitmapIndex(const string & filename/*, bool bufferfirstblock*/) : avm(filename), startofdata(0),
locationoffirstunreadblock(0), /*FirstBlock(),*/ mFilename(filename), /*, FirstBlockEnabled(bufferfirstblock)*/
mCompressionMode(0), mNumberOfBits(0) {
    ifstream in(mFilename.c_str());
    if (!in) {
        cerr << "I failed to open file " << filename << endl;
    }
    readHeader(in);
    startofdata = in.tellg();
    if (!in) {
        cerr << "I failed to read the header of the file " << filename << endl;
    }
    locationoffirstunreadblock = in.tellg();
    in.close();
}

BitmapIndex::~BitmapIndex() {
}

void BitmapIndex::readHeader(ifstream & in) {
    //cout << "reading header..." << endl;
    uint cookie(0);
    in.read(reinterpret_cast<char *> (& cookie), sizeof (cookie));
    if (cookie != BitmapWriter<SparseCompressedDataBlock<> >::cookie) {
        cerr << "index file cookie mismatch" << endl;
        throw cookiemismatch_exception("bad cookie");
    }
    if (!in) {
        throw cookiemismatch_exception("trouble while reading cookie");
    }
    uint currentversion(0);
    // we always start out with the file format version number
    in.read(reinterpret_cast<char *> (& currentversion), sizeof (currentversion));
    if (currentversion != version) {
        cerr << "version mismatch, expect trouble" << endl;
        throw versionmismatch_exception("bad file version");
    }
    in.read(reinterpret_cast<char *> (& mCompressionMode), sizeof (mCompressionMode));
    in.read(reinterpret_cast<char *> (& mNumberOfBits), sizeof (mNumberOfBits));
}


// dump the index in an uncompressed text format

template <class uword>
void BitmapIndex::dumpText_templated(const string & fname) {
    ofstream out(fname.c_str());
    ifstream in(mFilename.c_str());

    in.seekg(locationoffirstunreadblock); // does this assume first block buffered? // Daniel: no, it does not
    /*if (FirstBlockEnabled) {
      FirstBlock.printout(out);
    }*/
    while (in) {
        ReadCompressedDataBlock<uword> db;
        db.readAllFromDisk(in);
        if (!in) break;
        db.printout(out);
    }
    in.close();
    out.close();
}

void BitmapIndex::dumpText(const string & fname) {
    if (mNumberOfBits == 32)
        dumpText_templated<uword32 > (fname);
    else if (mNumberOfBits == 64)
        dumpText_templated<uword64 > (fname);
    else if (mNumberOfBits == 16)
        dumpText_templated<uword16 > (fname);
    else
        throw unsupportedwordsize_exception("I do not supported the requested word size");
}
/*
void BitmapIndex::computeQuerySet(const map<uint, string> & query, vector<uint> & querysetfrombtree) {
    for (map<uint, string>::const_iterator i = query.begin(); i != query.end(); ++i) {
        avm.getBitmapsIndices(i->first, i->second, querysetfrombtree);
    }
}*/


template<class uword>
void BitmapIndex::rangeQuery_templated(const OrthogonalQuery & oq, vector<ulong> & result, const int hint) {
    map<uint,set< pair<uint,uint> > >  querysetfrombtree;
    oq.computeRangeQuerySet(this->avm, querysetfrombtree);
    ifstream in(mFilename.c_str());
    bool debug(false);
    in.seekg(locationoffirstunreadblock);
    EWAHBoolArray<uword> answers;
    EWAHBoolArray<uword> container;
    set<uint> queryset;
    for(map<uint,set< pair<uint,uint> > >::iterator i = querysetfrombtree.begin(); i!=querysetfrombtree.end();++i)
        fromIntervalsToBitmaps(i->second, queryset);
    while (in) {
        ReadCompressedDataBlock<uword> db;
        db.readFromDisk(in, queryset);
        if (!in) break;
        if (queryset.size() == 1) {
            answers.append(db.getBitmap(*queryset.begin()));
        } else {
            if(debug) cout<<"I have "<<queryset.size()<<" bitmaps loaded"<<endl;
            sloworthogonalquery(db.getBuffer(),querysetfrombtree,container,hint);
            if (answers.sizeInBits() == 0)
                answers.swap(container);
            else
                answers.append(container);
        }
    }
    in.close();
    answers.appendRowIDs(result);
}

template<class uword>
void BitmapIndex::equalityQuery_templated(const OrthogonalQuery & oq, vector<ulong> & result, const int hint) {
    vector<uint>  queryset;
    oq.computeEqualityQuerySet(this->avm, queryset);
    ifstream in(mFilename.c_str());
    in.seekg(locationoffirstunreadblock);
    EWAHBoolArray<uword> answers;
    EWAHBoolArray<uword> container;
    //int hint = sparse ? HINT_SPARSE : HINT_NONE;
    while (in) {
        ReadCompressedDataBlock<uword> db;
        db.readFromDisk(in, queryset);
        if (!in) break;
        if (queryset.size() == 1) {
            answers.append(db.getBitmap(*queryset.begin()));
        } else {
            logicaland(db.getBuffer(),queryset,container,hint);
            if (answers.sizeInBits() == 0)
                answers.swap(container);
            else
                answers.append(container);
        }
    }
    in.close();
    answers.appendRowIDs(result);
}

//void equalityQuery(const map<uint, string> & query, vector<ulong> & result, const bool sparse = false);

void BitmapIndex::process(const OrthogonalQuery & oq, vector<ulong> & result, int hint) {
    if (oq.isEqualityQuery()) {
        if (mNumberOfBits == 32)
            equalityQuery_templated<uword32 > (oq, result, hint);
        else if (mNumberOfBits == 64)
            equalityQuery_templated<uword64 > (oq, result, hint);
        else if (mNumberOfBits == 16)
            equalityQuery_templated<uword16 > (oq, result, hint);
        else
            throw unsupportedwordsize_exception("I do not supported the requested word size");
    } else {
        if (mNumberOfBits == 32)
            rangeQuery_templated<uword32 > (oq, result, hint);
        else if (mNumberOfBits == 64)
            rangeQuery_templated<uword64 > (oq, result, hint);
        else if (mNumberOfBits == 16)
            rangeQuery_templated<uword16 > (oq, result, hint);
        else
            throw unsupportedwordsize_exception("I do not supported the requested word size");
    }
}
/*
void BitmapIndex::equalityQuery(const map<uint, string> & query, vector<ulong> & result, const bool sparse) {
    if (mNumberOfBits == 32)
        equalityQuery_templated<uword32 > (query, result, sparse);
    else if (mNumberOfBits == 64)
        equalityQuery_templated<uword64 > (query, result, sparse);
    else if (mNumberOfBits == 16)
        equalityQuery_templated<uword16 > (query, result, sparse);
    else
        throw unsupportedwordsize_exception("I do not supported the requested word size");
}*/

template <class uword>
class BlockIterator {
public:

    /*BlockIterator(const BitmapIndex & bi) : parent(bi), in(bi.mFilename.c_str()), datablock() {
        in.seekg(parent.locationoffirstunreadblock);
        if (sizeof (uword)*8 != bi.mNumberOfBits)
            throw wordsizemismatch_exception("small internal bug, probably");
    }

    BlockIterator(const BlockIterator & bli) : parent(bli.parent), in(bli.parent.mFilename.c_str()), datablock() {
        in.seekg(parent.locationoffirstunreadblock);
    }*/

    BlockIterator(ifstream & source) : in(source), datablock() {
    }

    ~BlockIterator() {
        in.close();
    }

    bool hasNext() {
        datablock.readAllFromDisk(in);
        return in;
    }

    const ReadCompressedDataBlock<uword> & next() {
        return datablock;
    }
private:
    //const BitmapIndex & parent;
    ifstream & in;
    ReadCompressedDataBlock<uword> datablock;
    //bool initialized;

};


#endif /*BITMAPINDEX_H_*/
