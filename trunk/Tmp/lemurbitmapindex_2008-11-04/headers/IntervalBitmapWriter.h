/*
 * File:   SqrtIntervalBitmapWriter.h
 * Author: lemire
 *
 * Created on September 3, 2008, 1:11 PM
 */


/**
 *This is the Chan-Ionnadis interval coding scheme
 *for range queries and various variants.
 */
#ifndef _INTERVALBITMAPWRITER_H
#define	_INTERVALBITMAPWRITER_H

#include "BitmapWriter.h"




/////////////////////////////////////////////////////
// This is the classical range encoding
////////////////////////////////////////////////////



template <class DB>
class RangeBitmapWriter {
public:
    RangeBitmapWriter(const string & indexfile, uint uncompressedblocksize = defaultUncompressedBlockSize);

    void indexCSVFile(string filename, bool verbose = false, string delimiter = ",", char commentmarker = '#');


    virtual ~RangeBitmapWriter();

    enum {
        version = 2, cookie = BitmapWriter<CompressedDataBlock<> >::cookie
    };



private:
	RangeBitmapWriter(const RangeBitmapWriter & x);
	RangeBitmapWriter & operator=(const RangeBitmapWriter & x);

    void writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap);

    void writeIndex(string filename, bool verbose, string delimiter = ",", char commentmarker = '#'); //, BitmapAllocation & ba
    string mIndexFile;
    uint blocksize;
};

template <class DB>
RangeBitmapWriter<DB>::RangeBitmapWriter(const string & indexfile, uint uncompressedblocksize) :
mIndexFile(indexfile), blocksize(uncompressedblocksize) {
    std::ios::sync_with_stdio(false);

}

template <class DB>
RangeBitmapWriter<DB>::~RangeBitmapWriter() {
}

template <class DB>
void RangeBitmapWriter<DB>::indexCSVFile(string filename, bool verbose, string delimiter, char commentmarker) {
    writeIndex(filename, verbose, delimiter, commentmarker);
}

template <class DB>
void RangeBitmapWriter<DB>::writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap) {
    const uint mycookie = cookie;
    out.write(reinterpret_cast<const char *> (&mycookie), sizeof (mycookie));
    const uint currentversion = version;
    // we always start out with the file format version number
    out.write(reinterpret_cast<const char *> (& currentversion), sizeof (currentversion));
    const uint compressionmode = DB::compressiontype;
    out.write(reinterpret_cast<const char *> (& compressionmode), sizeof (compressionmode));
    const uint nb = DB::getNumberOfBits();
    out.write(reinterpret_cast<const char *> (& nb), sizeof (nb));
}

template <class DB>
void RangeBitmapWriter<DB>::writeIndex(string filename, bool verbose, string delimiter, char commentmarker) {
    ofstream out(mIndexFile.c_str(), ios::out | ios::binary);
    if (!out) {
        cerr << "I can't open the index file \"" << mIndexFile << "\", expect trouble." << endl;
        throw cantindexfiletowrite_exception("can't open bitmap file to write to");
    }
    histogramtype mHistograms;
    HistogramLoader::loadHistogram(filename, mHistograms, delimiter, commentmarker, verbose);
    vector<stringuintmultimap > tobitmap;
    gStandardBitmapAllocation.computeBitmapAllocation(g1ofNBitmapBudget, mHistograms,tobitmap);
    mHistograms.clear();
    vector<uint> widths;
    ostream::pos_type beforeheader = out.tellp();
    writeHeader(out, tobitmap);
    AttributeValueMap::writeToDisk(tobitmap, mIndexFile, "Interval");
    ostream::pos_type afterheader = out.tellp();
    if (!out) {
        cerr << " I did not manage to write the header of the bitmap index in \"" << mIndexFile << "\". Aborting. " << endl;
        throw cantwritebitmapindexheader_exception("bad ostream after header written");
    }
    uint numberofbitmaps = BitmapAllocation::numberOfBitmaps(tobitmap); //0;
    DB db(numberofbitmaps, blocksize);
    ifstream in2(filename.c_str());
    if (!in2) {
        cerr << " could not reopen CSV file:" << filename << endl;
        throw cantopencsv_exception("could not reopen");
    }
    //cout <<" setting up a CI bitmap"<<endl;
    CSVParser csv(in2, delimiter, commentmarker);
    while (csv.hasNext()) {
        //cout << "inside loop"<<endl;
        if (db.full()) {
            db.emptyToDisk(out, verbose);
            if (!out) {
                cerr << " I did not manage to write one of the datablocks in  \"" << mIndexFile << "\". Aborting. " << endl;
                throw couldnotwritedatablock_exception("bad ostream after datablock written");
            }
        }
        //csv.next();
        const vector <string> & currentdata = csv.next();
        if (tobitmap.size() != currentdata.size()) continue;
        db.newRow();
        if (currentdata.size() != tobitmap.size()) {
            cerr << "This histogram is corrupted, size mismatch. Try deleting the .histo file." << endl;
            in2.close();
            out.close();
            throw corruptedhistogram_exception("size mismatch");
        }
        for (uint k = 0; k < currentdata.size(); ++k) {
            //multimap<string,uint>::iterator p;
            stringuintmultimap::const_iterator p = tobitmap[k].find(currentdata[k]);
            const uint nbr = tobitmap[k].count(currentdata[k]);
            if(nbr!=1) {
            	if (nbr == 0) {
                  cerr << "This histogram is corrupted, can't find value " << currentdata[k] << ". Try deleting the .histo file." << endl;
                  in2.close();
                  out.close();
                  throw corruptedhistogram_exception(currentdata[k]);
                } else {
                    cerr << "This histogram is corrupted, value " << currentdata[k] << " appears "<<nbr<<" times. Try deleting the .histo file." << endl;
                    in2.close();
                    out.close();
                    throw corruptedhistogram_exception(currentdata[k]);
                }
            }
            for(;p!=tobitmap[k].end();++p) {
            	db.set(p->second);
            }
        }
        //++nbrrows;
    }
    in2.close();
    if (db.getNumberOfRows() == 0)
        cerr << "I didn't find any row to index!" << endl;
    db.emptyToDisk(out, verbose);
    if (!out) {
        cerr << " I did not manage to write the last datablocks in  \"" << mIndexFile << "\". Aborting. " << endl;
        throw couldnotwritedatablock_exception("bad ostream after last datablock written");
    }
    out.close();
}






/////////////////////////////////////////////////////
// Next is sqrt-encoding (Daniel's accidental invention)
////////////////////////////////////////////////////




inline uint intervalsqrt(uint numberofvalues) {
    return lround(sqrt(static_cast<double>(numberofvalues)));
}


inline void computeSqrtIntervalWidths(const vector<stringuintmultimap > & tobitmap, vector<uint> & out) {
    out.resize(tobitmap.size());
    for(uint k = 0; k< tobitmap.size(); ++k)
        out[k] = intervalsqrt(tobitmap[k].size());
}

inline void computeSqrtIntervalWidths(const AttributeValueMap & avm, vector<uint> & out) {
    out.resize(avm.size());
    for(uint k = 0; k< avm.size(); ++k)
        out[k] = intervalsqrt(avm.getNumberOfAttributeValues(k));
}



template <class DB>
class SqrtIntervalBitmapWriter {
public:
    SqrtIntervalBitmapWriter(const string & indexfile, uint uncompressedblocksize = defaultUncompressedBlockSize);

    void indexCSVFile(string filename, bool verbose = false, string delimiter = ",", char commentmarker = '#');


    virtual ~SqrtIntervalBitmapWriter();

    enum {
        version = 2, cookie = BitmapWriter<CompressedDataBlock<> >::cookie
    };


private:
    SqrtIntervalBitmapWriter(const SqrtIntervalBitmapWriter & x);
    SqrtIntervalBitmapWriter & operator=(const SqrtIntervalBitmapWriter & x);

    void writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap);

    void writeIndex(string filename, bool verbose, string delimiter = ",", char commentmarker = '#'); //, BitmapAllocation & ba
    string mIndexFile;
    uint blocksize;
};

template <class DB>
SqrtIntervalBitmapWriter<DB>::SqrtIntervalBitmapWriter(const string & indexfile, uint uncompressedblocksize) :
mIndexFile(indexfile), blocksize(uncompressedblocksize) {
    std::ios::sync_with_stdio(false);

}

template <class DB>
SqrtIntervalBitmapWriter<DB>::~SqrtIntervalBitmapWriter() {
}

template <class DB>
void SqrtIntervalBitmapWriter<DB>::indexCSVFile(string filename, bool verbose, string delimiter, char commentmarker) {
    writeIndex(filename, verbose, delimiter, commentmarker);
}

template <class DB>
void SqrtIntervalBitmapWriter<DB>::writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap) {
    const uint mycookie = cookie;
    out.write(reinterpret_cast<const char *> (&mycookie), sizeof (mycookie));
    const uint currentversion = version;
    // we always start out with the file format version number
    out.write(reinterpret_cast<const char *> (& currentversion), sizeof (currentversion));
    const uint compressionmode = DB::compressiontype;
    out.write(reinterpret_cast<const char *> (& compressionmode), sizeof (compressionmode));
    const uint nb = DB::getNumberOfBits();
    out.write(reinterpret_cast<const char *> (& nb), sizeof (nb));
}

template <class DB>
void SqrtIntervalBitmapWriter<DB>::writeIndex(string filename, bool verbose, string delimiter, char commentmarker) {
    ofstream out(mIndexFile.c_str(), ios::out | ios::binary);
    if (!out) {
        cerr << "I can't open the index file \"" << mIndexFile << "\", expect trouble." << endl;
        throw cantindexfiletowrite_exception("can't open bitmap file to write to");
    }
    histogramtype mHistograms;
    HistogramLoader::loadHistogram(filename, mHistograms, delimiter, commentmarker, verbose);
    vector<stringuintmultimap > tobitmap;
    gStandardBitmapAllocation.computeBitmapAllocation(g1ofNBitmapBudget, mHistograms,tobitmap);
    mHistograms.clear();
    vector<uint> widths;
    computeSqrtIntervalWidths(tobitmap, widths);
    ostream::pos_type beforeheader = out.tellp();
    writeHeader(out, tobitmap);
    AttributeValueMap::writeToDisk(tobitmap, mIndexFile, "Interval");
    ostream::pos_type afterheader = out.tellp();
    if (!out) {
        cerr << " I did not manage to write the header of the bitmap index in \"" << mIndexFile << "\". Aborting. " << endl;
        throw cantwritebitmapindexheader_exception("bad ostream after header written");
    }
    uint numberofbitmaps = BitmapAllocation::numberOfBitmaps(tobitmap); //0;
    DB db(numberofbitmaps, blocksize);
    ifstream in2(filename.c_str());
    if (!in2) {
        cerr << " could not reopen CSV file:" << filename << endl;
        throw cantopencsv_exception("could not reopen");
    }
    //cout <<" setting up a CI bitmap"<<endl;
    CSVParser csv(in2, delimiter, commentmarker);
    while (csv.hasNext()) {
        //cout << "inside loop"<<endl;
        if (db.full()) {
            db.emptyToDisk(out, verbose);
            if (!out) {
                cerr << " I did not manage to write one of the datablocks in  \"" << mIndexFile << "\". Aborting. " << endl;
                throw couldnotwritedatablock_exception("bad ostream after datablock written");
            }
        }
        //csv.next();
        const vector <string> & currentdata = csv.next();
        if (tobitmap.size() != currentdata.size()) continue;
        db.newRow();
        if (currentdata.size() != tobitmap.size()) {
            cerr << "This histogram is corrupted, size mismatch. Try deleting the .histo file." << endl;
            in2.close();
            out.close();
            throw corruptedhistogram_exception("size mismatch");
        }
        for (uint k = 0; k < currentdata.size(); ++k) {
            //multimap<string,uint>::iterator p;
            stringuintmultimap::const_iterator p = tobitmap[k].find(currentdata[k]);
            const uint nbr = tobitmap[k].count(currentdata[k]);
            if(nbr!=1) {
            	if (nbr == 0) {
                  cerr << "This histogram is corrupted, can't find value " << currentdata[k] << ". Try deleting the .histo file." << endl;
                  in2.close();
                  out.close();
                  throw corruptedhistogram_exception(currentdata[k]);
                } else {
                    cerr << "This histogram is corrupted, value " << currentdata[k] << " appears "<<nbr<<" times. Try deleting the .histo file." << endl;
                    in2.close();
                    out.close();
                    throw corruptedhistogram_exception(currentdata[k]);
                }
            }
            for(uint j = 0; j < widths[k];++j) {
                db.set(p->second);
                if(p == tobitmap[k].begin()) break;
                --p;
            }
        }
        //++nbrrows;
    }
    in2.close();
    if (db.getNumberOfRows() == 0)
        cerr << "I didn't find any row to index!" << endl;
    db.emptyToDisk(out, verbose);
    if (!out) {
        cerr << " I did not manage to write the last datablocks in  \"" << mIndexFile << "\". Aborting. " << endl;
        throw couldnotwritedatablock_exception("bad ostream after last datablock written");
    }
    out.close();
}


#endif	/* _INTERVALBITMAPWRITER_H */

