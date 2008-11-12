/*
 * File:   projectionindex.h
 * Author: lemire
 *
 * The goal of these classes is to implement a simple
 * external-memory projection index.
 * Created on August 1, 2008, 11:33 AM
 */

#ifndef _PROJECTIONINDEX_H
#define	_PROJECTIONINDEX_H

#include "BitmapWriter.h"
#include "querylanguage.h"

class ProjectionIndexDataBlock {
public:

    ProjectionIndexDataBlock(uint uncompressedblocksize = defaultUncompressedBlockSize, uint numberofcolumns = 0) :
    blocksize(uncompressedblocksize), columns(numberofcolumns), mNumberOfRows(0) {
    }
    virtual ~ProjectionIndexDataBlock() {}

    uint getNumberOfRows() {
        return mNumberOfRows;
    }

    void process(const OrthogonalQuery & oq, AttributeValueMap & avm, const uint startingRowID, vector<ulong> & result) {
        if (oq.isEqualityQuery()) {// for speed we specialize!
            map<uint, set<uint> > queryset;
            oq.computeContainmentQuerySet(avm, queryset);
            if (queryset.size() == 1) { // specialization for speed
                const uint dim = queryset.begin()->first;
                const set<uint> val = queryset.begin()->second;
                for (uint row = 0; row < mNumberOfRows; ++row) {
                    if (val.find(columns[dim][row]) != val.end()) result.push_back(row + startingRowID);
                }
                return;
            }
            for (uint row = 0; row < mNumberOfRows; ++row) {
                //cout << row<<endl;
                bool ok(true);
                for (map<uint, set<uint> >::iterator i = queryset.begin(); i != queryset.end(); ++i) {
                    if (i->second.find(columns[i->first][row]) == i->second.end()) {
                        ok = false;
                        break;
                    }
                }
                if (ok) result.push_back(row + startingRowID);
            }
        } else {
            map<uint, set<uint> > queryset;
            map<uint, set<pair<uint, uint> > > rangequeryset;
            oq.computeContainmentQuerySet(avm, queryset);
            oq.computeRangeQuerySet(avm, rangequeryset);
            for (uint row = 0; row < mNumberOfRows; ++row) {
                //cout << row<<endl;
                bool ok(true);
                if (queryset.size() > 0) {
                    throw unsupportedoperation_exception("I don't want to have to think about a mix of range and equality queries");
                }
                for (map<uint, set<pair<uint, uint> > >::iterator i = rangequeryset.begin(); i != rangequeryset.end(); ++i) {
                    /*cout<<"at row"<<row<<" and column "<<i->first
                            <<" checking that value encoded as "
                            << columns[i->first][row]<<" is between "
                            <<i->second.first<<" and "
                            <<i->second.second<<endl;*/
                    bool localok(false);
                    for(set<pair<uint, uint> >::iterator j = i->second.begin(); j != i->second.end(); ++j) {
                        if ((columns[i->first][row] >= j->first) and (columns[i->first][row] <= j->second)) {
                            localok = true;
                            break;
                        }
                    }
                    if(!localok) {
                        ok = false;
                        break;
                    }
                }
                if (ok) result.push_back(row + startingRowID);
            }

        }
    }


    void add(vector<uint> & row) {
        for (uint k = 0; k < columns.size(); ++k)
            columns[k].push_back(row[k]);
        ++mNumberOfRows;
    }

    bool full() {
        return blocksize <= mNumberOfRows * columns.size() * sizeof (uint);
    }

    void emptyToDisk(ostream & out) {
        if(!out) cerr<<"output stream is no good"<<endl;
        if (getNumberOfRows() == 0) return;
        uint dims = columns.size();
        uint mcookie(cookie);
        //cout<<"writing "<<mcookie<<" at "<<out.tellp()<<endl;
        out.write(reinterpret_cast<const char *> (&mcookie), sizeof (mcookie));
        //cout<<"there are  "<<dims<<" dims "<<endl;
        out.write(reinterpret_cast<const char *> (&dims), sizeof (dims));
        //cout<<"there are  "<<mNumberOfRows<<" rows "<<endl;
        out.write(reinterpret_cast<const char *> (&mNumberOfRows), sizeof (mNumberOfRows));
        for (uint k = 0; k < columns.size(); ++k) {
            //cout<<"writing a column from "<<static_cast<uint>(out.tellp())<<" to "<<(static_cast<uint>(out.tellp())+ sizeof (uint) * columns[k].size())<<endl;
            //cout<<"column "<<k<<" at "<<static_cast<uint>(out.tellp())<<endl;
            out.write(reinterpret_cast<const char *> (&columns[k][0]), sizeof (uint) * columns[k].size());
            //for(uint j = 0;j<columns[k].size();++j)cout<<columns[k][j]<<endl;
            if(!out) cerr<<"output stream is no good after writing column "<<k<<endl;
        }
    }

    enum {
        cookie = 0xBBCC
    };

    void readFromDisk(istream & in, set<uint> & querysetfrombtree) {
        //cout <<"calling ProjectionIndexDataBlock.readFromDisk with "<<in.tellg()<<endl;
        uint dim(0), mcookie(0);
        in.read(reinterpret_cast<char *> (&mcookie), sizeof (mcookie));
        if(!in) return;
        if (mcookie != cookie) {
            cerr<<" I was expecting datablock cookie "<<static_cast<uint>(cookie)<<" but got "<<mcookie<< " instead at "<< in.tellg()<<". Bug?"<<endl;
            throw cookiemismatch_exception("bad cookie");
        }
        in.read(reinterpret_cast<char *> (&dim), sizeof (dim));
        in.read(reinterpret_cast<char *> (&mNumberOfRows), sizeof (mNumberOfRows));
        columns.resize(dim);
        fstream::pos_type sizeofacolumn = mNumberOfRows * sizeof (uint);
        fstream::pos_type current = in.tellg();
        for(set<uint>::const_iterator i =  querysetfrombtree.begin(); i!=querysetfrombtree.end(); ++i) {
            in.seekg(*i * sizeofacolumn + current);
            vector<uint> & currentcol (columns[*i]);
            currentcol.resize(mNumberOfRows);
            //for(uint k = 0; k< mNumberOfRows;++k) cout<<currentcol[k]<<endl;
            //cout << "loading column "<<i->first<< " and allocating "<<mNumberOfRows<<endl;
            //cout<<"is input good ? "<<in.good()<<endl;
            //cout<<" reading from "<<static_cast<uint>(in.tellg())<<" to "<<(static_cast<uint>(in.tellg())+sizeof (uint) * mNumberOfRows)<<endl;
            //cout<<"column "<<i->first<<" at "<<static_cast<uint>(in.tellg())<<endl;
            in.read(reinterpret_cast<char *> (&currentcol[0]), sizeof (uint) * currentcol.size());
            if(!in) cerr<<"problem reading column "<<*i<<endl;
            //cout<<"is input good ? "<<in.good()<<endl;
            //for(uint k = 0; k< mNumberOfRows;++k) cout<<currentcol[k]<<endl;
        }
        in.seekg(columns.size()*sizeofacolumn + current - 1);
    }

    uint blocksize;
    vector<vector<uint> > columns;
    uint mNumberOfRows;
};

class ProjectionIndexWriter {
public:

    ProjectionIndexWriter(const string & indexfile, uint uncompressedblocksize = defaultUncompressedBlockSize)
    : mIndexFile(indexfile), blocksize(uncompressedblocksize) {
    }
    void indexCSVFile(const string &  filename, bool verbose = false, const string &  delimiter = ",", char commentmarker = '#');
    string mIndexFile;
    uint blocksize;
    void writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap);

    enum {
        cookie = 0xAFFA, version = 1, compressiontype = 0
    };

};

void ProjectionIndexWriter::writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap) {
    const uint mycookie = cookie;
    out.write(reinterpret_cast<const char *> (&mycookie), sizeof (mycookie));
    const uint currentversion = version;
    // we always start out with the file format version number
    out.write(reinterpret_cast<const char *> (& currentversion), sizeof (currentversion));
    const uint compressionmode = compressiontype;
    out.write(reinterpret_cast<const char *> (& compressionmode), sizeof (compressionmode));
    //cout <<"header brings me to "<<out.tellp()<<endl;
}

void ProjectionIndexWriter::indexCSVFile(const string &  filename, bool verbose, const string & delimiter, char commentmarker) {
    ofstream out(mIndexFile.c_str(), ios::out | ios::binary);
    //cout <<" I am going to write to "<<mIndexFile<<endl;
    if (!out) {
        cerr << "I can't open the index file \"" << mIndexFile << "\", expect trouble." << endl;
        throw cantindexfiletowrite_exception("can't open bitmap file to write to");
    }
    histogramtype mHistograms;
    HistogramLoader::loadHistogram(filename, mHistograms, delimiter, commentmarker, verbose);
    AlphabeticalBitmapAllocation aba;
    vector<stringuintmultimap > tobitmap;
    aba.computeBitmapAllocation(g1ofNBitmapBudget, mHistograms, tobitmap);
    mHistograms.clear();
    ostream::pos_type beforeheader = out.tellp();
    writeHeader(out, tobitmap);
    AttributeValueMap::writeToDisk(tobitmap, mIndexFile);
    ostream::pos_type afterheader = out.tellp();
    if (!out) {
        cerr << " I did not manage to write the header of the bitmap index in \"" << mIndexFile << "\". Aborting. " << endl;
        throw cantwritebitmapindexheader_exception("bad ostream after header written");
    }
    if (verbose) cout << "size of the header = " << (afterheader - beforeheader) << " bytes" << endl;
    // first, we have to decide where each attribute value goes
    // this is easy because we do something straight forward, without compression
    // next, we process the data and write it out
    //uint numberofbitmaps = BitmapAllocation::numberOfBitmaps(tobitmap); //0;
    //DB db(numberofbitmaps, blocksize);
    //if (verboseindexing) cout << "DB created" << endl;
    ifstream in2(filename.c_str());
    if (!in2) {
        cerr << " could not reopen CSV file:" << filename << endl;
        throw cantopencsv_exception("could not reopen");
    }
    vector<uint> values(tobitmap.size());
    ProjectionIndexDataBlock db(blocksize, tobitmap.size());
    CSVParser csv(in2, delimiter, commentmarker);
    while (csv.hasNext()) {
        //cout << "inside loop"<<endl;
        if (db.full()) {
            db.emptyToDisk(out);
            if (!out) {
                cerr << " I did not manage to write one of the datablocks in  \"" << mIndexFile << "\". Aborting. " << endl;
                throw couldnotwritedatablock_exception("bad ostream after datablock written");
            }
        }
        //csv.next();
        const vector <string> & currentdata = csv.next();
        if (tobitmap.size() != currentdata.size()) continue;
        for (uint k = 0; k < currentdata.size(); ++k) {
            stringuintmultimap::const_iterator p = tobitmap[k].find(currentdata[k]);
            if (p == tobitmap[k].end()) throw corruptedhistogram_exception(currentdata[k]);
            values[k] = p->second;
        }
        db.add(values);
        //++nbrrows;
    }
    in2.close();
    if (db.getNumberOfRows() == 0)
        cerr << "I didn't find any row to index!" << endl;
    db.emptyToDisk(out);
    if (!out) {
        cerr << " I did not manage to write the last datablocks in  \"" << mIndexFile << "\". Aborting. " << endl;
        throw couldnotwritedatablock_exception("bad ostream after last datablock written");
    }
    //cout << "wrote "<<out.tellp()<< "bytes"<<endl;
    out.close();

};





















class ProjectionIndexReader {
public:

    ProjectionIndexReader(const string & indexfile)
    : locationoffirstunreadblock(0), mIndexFile(indexfile), avm(indexfile),IOonly(false) {
        ifstream in(indexfile.c_str());
        readHeader(in);
        in.close();
    }
    fstream::pos_type locationoffirstunreadblock;
    string mIndexFile;
    void readHeader(ifstream & out, vector<stringuintmultimap >& tobitmap);

   // enum {
    //    cookie = 0xAFFA, version = 1, compressiontype = 0
    //};

    void readHeader(ifstream & in);
    void process(const OrthogonalQuery & query, vector<ulong> & result);
    //void computeQuerySet(const map<uint, string> & query, map<uint,uint> & querysetfrombtree);

    AttributeValueMap avm;
    bool IOonly;
};

/*
void ProjectionIndexReader::computeQuerySet(const map<uint, string> & query,  map<uint,uint> & querysetfrombtree) {
    for (map<uint, string>::const_iterator i = query.begin(); i != query.end(); ++i) {
        querysetfrombtree[i->first] = avm.getIndex(i->first, i->second);
    }
}*/











void ProjectionIndexReader::readHeader(ifstream & in) {
    //cout << "reading header..." << endl;
    uint cookie(0);
    in.read(reinterpret_cast<char *> (& cookie), sizeof (cookie));
    if (cookie != ProjectionIndexWriter::cookie) {
        //cerr <<"the file has cookie "<<cookie<<" but I am expecting cookie "<< static_cast<uint>(ProjectionIndexWriter::cookie)<<endl;
        throw cookiemismatch_exception("bad cookie");
    }
    uint currentversion;
    in.read(reinterpret_cast<char *> (& currentversion), sizeof (currentversion));
    if (currentversion != ProjectionIndexWriter::version) {
        cerr <<"file says it has version "<<currentversion<<" whereas this software has version "<<static_cast<uint>(ProjectionIndexWriter::version)<<endl;
        throw versionmismatch_exception("bad file version");
    }
    uint compressionmode;
    in.read(reinterpret_cast<char *> (& compressionmode), sizeof (compressionmode));
    locationoffirstunreadblock = in.tellg();
    //cout<<"header finished at "<<locationoffirstunreadblock<<endl;
}

void ProjectionIndexReader::process(const OrthogonalQuery & query, vector<ulong> & result) {
    /*map<uint,uint> querysetfrombtree;
    try {
        computeQuerySet(query, querysetfrombtree);
    } catch (missingattributevalue_exception ia) {
        return; // vector<ulong>();
    }*/
    set<uint> whichcolumns;
    query.whichColumns(whichcolumns);
    ifstream in(mIndexFile.c_str());
    in.seekg(locationoffirstunreadblock);
    //cout <<"moved to "<<locationoffirstunreadblock<<endl;
    ulong currentRowID=0;
    while (in) {
        ProjectionIndexDataBlock db;
        //cout <<" reading block..."<<endl;
        db.readFromDisk(in, whichcolumns);
        if (!in) {
            //cout<<"file NOT good after readingblock..."<<endl;
            break;
        }
        //cout <<" reading block...done!"<<endl;
        if(!IOonly)db.process(query,avm,currentRowID, result);
        //cout <<" equality query, ok!..."<<endl;
        currentRowID += db.getNumberOfRows();
        //cout <<" row id is now "<<currentRowID<<endl;
    }
    in.close();
}


#endif	/* _PROJECTIONINDEX_H */

