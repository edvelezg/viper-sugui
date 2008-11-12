/*
 * compressedprojectionindex.h
 *
 *  Created on: Sep 26, 2008
 *      Author: lemire
 */

#ifndef COMPRESSEDPROJECTIONINDEX_H_
#define COMPRESSEDPROJECTIONINDEX_H_
#include "Util.h"
#include "RLEUtil.h"
#include "BitmapWriter.h"
#include "querylanguage.h"
#include "CompressedKaserBitmap.h"
#include "queryfilter.h"


/*********************************************
 * template CompressedProjectionIndexDataBlock
 *********************************************/

template <class ArrayType>
class CompressedProjectionIndexDataBlock {
public:

	CompressedProjectionIndexDataBlock(uint uncompressedblocksize = defaultUncompressedBlockSize, uint numberofcolumns = 0) :
    blocksize(uncompressedblocksize), columnwriters(numberofcolumns),columnreaders(), mNumberOfRows(0), currentsize(0) {
    }
    virtual ~CompressedProjectionIndexDataBlock() {}

    uint getNumberOfRows() {
        return mNumberOfRows;
    }

    void process(const OrthogonalQuery & oq, AttributeValueMap & avm, const uint startingRowID, vector<ulong> & result) {
    	if (oq.isEqualityQuery()) {// for speed we specialize!
    		map<uint, set<uint> > queryset;
    		oq.computeContainmentQuerySet(avm, queryset);
    		typename ArrayType::ArrayReader::RowArray answer;//RLEArrayReader<uint, countertype>
    		map<uint, set<uint> >::iterator i = queryset.begin();
    		{
    			const set<uint> val = i->second;
    			const uint dim = i->first;
    			typename ArrayType::ArrayReader & reader = columnreaders[dim];//RLEArrayReader<uint, countertype>
    			if(val.size()==1)
    				reader.filter(IsEqualToOneValue(*val.begin()),answer);
    			else
    				reader.filter(IsEqualToAtLeastOneValue(val),answer);
    		}
    		++i;
    		for (; i != queryset.end(); ++i) {
    			//vector<uint> tanswer;
    			typename ArrayType::ArrayReader & treader = columnreaders[i->first];//RLEArrayReader<uint, countertype>
    			const set<uint> val = i->second;
    			//RLERowArray
    			typename ArrayType::ArrayReader::RowArray tanswer;//RLEArrayReader<uint, countertype>
    			if(val.size()==1)
    				treader.filter(IsEqualToOneValue(*val.begin()),tanswer);
    			else
    				treader.filter(IsEqualToAtLeastOneValue(val),tanswer);
    			//RLERowArray
    			typename ArrayType::ArrayReader::RowArray tmp;//RLEArrayReader<uint, countertype>
    			answer.intersection(tanswer,tmp);
    			tmp.swap(answer);
    		}
    		answer.appendRowIDs(result,startingRowID);
    	} else {
    		map<uint, set<uint> > queryset;
    		map<uint, set<pair<uint, uint> > > rangequeryset;
    		oq.computeContainmentQuerySet(avm, queryset);
    		oq.computeRangeQuerySet(avm, rangequeryset);
    		if (queryset.size() > 0) {
    			throw unsupportedoperation_exception("I don't want to have to think about a mix of range and equality queries");
    		}
    		//RLERowArray
    		typename ArrayType::ArrayReader::RowArray answer;//RLEArrayReader<uint, countertype>
    		map<uint, set<pair<uint, uint> > >::iterator i = rangequeryset.begin();
    		{
    			const uint dim = i->first;
    			const set<pair<uint, uint> > val = i->second;
    			typename ArrayType::ArrayReader & reader = columnreaders[dim];//RLEArrayReader<uint, countertype>
    			if(val.size()==1)
    				reader.filter(BelongsToRange(*val.begin()),answer);
    			else
    				reader.filter(BelongsToAtLeastOneRange(val),answer);
    		}
    		++i;
    		for (; i != rangequeryset.end(); ++i) {
    			typename ArrayType::ArrayReader & treader = columnreaders[i->first];//RLEArrayReader<uint, countertype>
    			const set<pair<uint, uint> > val = i->second;
    			//RLERowArray
    			typename ArrayType::ArrayReader::RowArray tanswer;//RLEArrayReader<uint, countertype>
    			if(val.size()==1)
    				treader.filter(BelongsToRange(*val.begin()),tanswer);
    			else
    				treader.filter(BelongsToAtLeastOneRange(val),tanswer);
    			//RLERowArray
    			typename ArrayType::ArrayReader::RowArray tmp;//RLEArrayReader<uint, countertype>
    			answer.intersection(tanswer,tmp);
    			answer.swap(tmp);
    		}
    		answer.appendRowIDs(result,startingRowID);
    	}
    }


    void add(vector<uint> & row) {
    	currentsize=0;
        for (uint k = 0; k < row.size(); ++k) {
        	columnwriters[k].add(row[k]);
            currentsize+=columnwriters[k].size();
        }
        ++mNumberOfRows;
    }

    bool full() {
        return blocksize <= currentsize;
    }

    void emptyToDisk(ostream & out) {
    	const bool debug(false);
        if(!out) cerr<<"output stream is no good"<<endl;
        if (getNumberOfRows() == 0) return;
        uint dims = columnwriters.size();
        uint mcookie(cookie);
        if(debug) cout<<"writing "<<mcookie<<" at "<<out.tellp()<<endl;
        out.write(reinterpret_cast<const char *> (&mcookie), sizeof (mcookie));
        if(debug) cout<<"there are  "<<dims<<" dims "<<endl;
        out.write(reinterpret_cast<const char *> (&dims), sizeof (dims));
        //cout<<"there are  "<<mNumberOfRows<<" rows "<<endl;
        out.write(reinterpret_cast<const char *> (&mNumberOfRows), sizeof (mNumberOfRows));
        uint pos(0);
        for (uint k = 0; k < columnwriters.size(); ++k) {
        	//const uint s (columnwriters.size());
        	if(debug) cout<<"writing "<<pos<<endl;
        	out.write(reinterpret_cast<const char *> (&pos), sizeof (pos));
        	columnwriters[k].flush();
        	if(debug) cout<<k<<" "<<columnwriters[k].size()<<endl;
        	pos += columnwriters[k].size();
        }
        out.write(reinterpret_cast<const char *> (&pos), sizeof (pos));
        if(debug) cout<<"writing "<<pos<<endl;
        if(debug) cout<<"writing arrays at "<<out.tellp()<<endl;
        for (uint k = 0; k < columnwriters.size(); ++k) {
        	//const uint predictednextpos(static_cast<uint>(out.tellp())+  columnwriters[k].size());
            //cout<<"writing a column from "<<static_cast<uint>(out.tellp())<<" to "<<(static_cast<uint>(out.tellp())+  columnwriters[k].size())<<endl;
            //cout<<"column "<<k<<" at "<<static_cast<uint>(out.tellp())<<endl;
        	columnwriters[k].write(out);//out.write(reinterpret_cast<const char *> (&columnwriters[k].container[0]),  columnwriters[k].size());
            //for(uint j = 0;j<columns[k].size();++j)cout<<columns[k][j]<<endl;
        	/*if(static_cast<uint>(out.tellp()) != predictednextpos) {
        		cerr<<"I was expected to be at post "<<static_cast<uint>(out.tellp())<<" but I am at "<<predictednextpos<<" bug?"<<endl;
        		throw "stop";
        	}*/

            if(!out) cerr<<"output stream is no good after writing column "<<k<<endl;
        }
        if(debug) cout<<"finished with block at "<<out.tellp()<<endl;
    }

    enum {
        cookie = 0xDBCC
    };

    void readFromDisk(istream & in, set<uint> & querysetfrombtree) {
        const bool debug(false);
    	//cout <<"calling ProjectionIndexDataBlock.readFromDisk with "<<in.tellg()<<endl;
        uint dim(0), mcookie(0);
        if(debug) cout<<"reading cookie at "<<in.tellg()<<endl;
        in.read(reinterpret_cast<char *> (&mcookie), sizeof (mcookie));
        if(!in) return;
        if (mcookie != cookie) {
            cerr<<" I was expecting datablock cookie "<<static_cast<uint>(cookie)<<" but got "<<mcookie<< " instead at "<< in.tellg()<<". Bug?"<<endl;
            throw cookiemismatch_exception("bad cookie");
        }
        in.read(reinterpret_cast<char *> (&dim), sizeof (dim));
        if(debug) cout<<"dim =  "<<dim<<endl;
        in.read(reinterpret_cast<char *> (&mNumberOfRows), sizeof (mNumberOfRows));
        //columnreaders.resize(dim);
        map<uint,uint> pos;
        fstream::pos_type current = in.tellg();
        for(set<uint>::const_iterator i =  querysetfrombtree.begin(); i!=querysetfrombtree.end(); ++i) {
        	in.seekg(current +static_cast<fstream::pos_type>(*i * sizeof(uint)));
        	in.read(reinterpret_cast<char *> (&pos[*i]), sizeof (pos[*i]));
        	if(querysetfrombtree.find(*i+1)==querysetfrombtree.end())
        		in.read(reinterpret_cast<char *> (&pos[*i+1]), sizeof (pos[*i+1]));
        }
        //if(in.tellg()<=current + static_cast<fstream::pos_type>(dim * sizeof(uint))) {
        	in.seekg(current + static_cast<fstream::pos_type>(dim * sizeof(uint)));
        	in.read(reinterpret_cast<char *> (&pos[dim+1]), sizeof (pos[dim+1]));
        //} else in.seekg(current + static_cast<fstream::pos_type>((dim + 1) * sizeof(uint)));
        if(debug) for(map<uint,uint>::iterator z = pos.begin(); z!=pos.end();++z )
        	cout<<"read pos "<<z->first<<" "<<z->second<<endl;
        current = in.tellg();
        //cout<<"reading arrays at "<<in.tellg()<<endl;
        if(debug) cout<<"number of arrays to read: "<<querysetfrombtree.size()<<endl;
        for(set<uint>::const_iterator i =  querysetfrombtree.begin(); i!=querysetfrombtree.end(); ++i) {
        	if(debug) cout<<"reading array "<<*i<<endl;
        	in.seekg(pos[*i] + current);
        	columnreaders[*i].read(in,pos[*i+1]-pos[*i]);
            if(!in) cerr<<"problem reading column "<<*i<<endl;
        }
        if(debug) cout<<"done reading arrays at "<<in.tellg()<<" "<<pos.size() <<" "<< pos[dim+1]<<endl;
        in.seekg(current+static_cast<fstream::pos_type>(pos[dim+1]));
        if(debug) cout<<"ending at "<<in.tellg()<<endl;
    }

    uint blocksize;

    vector<typename ArrayType::ArrayWriter> columnwriters;//RLEArrayWriter<uint, countertype>
    map<uint,typename ArrayType::ArrayReader> columnreaders;//RLEArrayReader<uint, countertype>
    uint mNumberOfRows, currentsize;
};


/*********************************************
 * template CompressedProjectionIndexWriter
 *********************************************/


template <class ArrayType>
class CompressedProjectionIndexWriter {
public:

	CompressedProjectionIndexWriter(const string & indexfile, uint uncompressedblocksize = defaultUncompressedBlockSize)
    : mIndexFile(indexfile), blocksize(uncompressedblocksize) {
    }
    void indexCSVFile(const string &  filename, bool verbose = false, const string &  delimiter = ",", char commentmarker = '#');
    string mIndexFile;
    uint blocksize;
    void writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap);

    enum {
        cookie = 0xCFFA, version = 1, compressiontype = 1
    };

};

template <class ArrayType>
void CompressedProjectionIndexWriter<ArrayType>::writeHeader(ofstream & out, vector<stringuintmultimap >& tobitmap) {
    const uint mycookie = cookie;
    out.write(reinterpret_cast<const char *> (&mycookie), sizeof (mycookie));
    const uint currentversion = version;
    // we always start out with the file format version number
    out.write(reinterpret_cast<const char *> (& currentversion), sizeof (currentversion));
    const uint compressionmode = compressiontype;
    out.write(reinterpret_cast<const char *> (& compressionmode), sizeof (compressionmode));
    //const uint bitsforcounters = sizeof(countertype)*8;
    uint bfc(ArrayType::bitsforcounters);
    out.write(reinterpret_cast<const char *> (& bfc), sizeof (bfc));
    //cout<<"bitsforcounters="<<bfc<<endl;
    //cout <<"header brings me to "<<out.tellp()<<endl;
}

template <class ArrayType>
void CompressedProjectionIndexWriter<ArrayType>::indexCSVFile(const string &  filename, bool verbose, const string & delimiter, char commentmarker) {
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
    CompressedProjectionIndexDataBlock<ArrayType> db(blocksize, tobitmap.size());
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













/*********************************************
 * template CompressedProjectionIndexReader
 *********************************************/





class CompressedProjectionIndexReader {
public:

    CompressedProjectionIndexReader(const string & indexfile)
    : locationoffirstunreadblock(0), mIndexFile(indexfile), avm(indexfile), bitsforcounters(0),IOonly(false) {
        ifstream in(indexfile.c_str());
        readHeader(in);
        in.close();
    }
    fstream::pos_type locationoffirstunreadblock;
    string mIndexFile;
    void readHeader(ifstream & out, vector<stringuintmultimap >& tobitmap);

    //enum {
    //	  cookie = 0xCFFA, version = 1, compressiontype = 1
    //};

    void readHeader(ifstream & in);
    void process(const OrthogonalQuery & query, vector<ulong> & result);
    //void computeQuerySet(const map<uint, string> & query, map<uint,uint> & querysetfrombtree);

    AttributeValueMap avm;
    uint bitsforcounters;
    bool IOonly;
private:
	template <class ArrayType>
	void templated_process(const OrthogonalQuery & query, vector<ulong> & result);
};












void CompressedProjectionIndexReader::readHeader(ifstream & in) {
    //cout << "reading header..." << endl;
    uint cookie(0);
    in.read(reinterpret_cast<char *> (& cookie), sizeof (cookie));
    if (cookie != CompressedProjectionIndexWriter<uint>::cookie) {
        cerr <<"the file has cookie "<<cookie<<" but I am expecting cookie "<< static_cast<uint>(ProjectionIndexWriter::cookie)<<endl;
        throw cookiemismatch_exception("bad cookie");
    }
    uint currentversion;
    in.read(reinterpret_cast<char *> (& currentversion), sizeof (currentversion));
    if (currentversion != CompressedProjectionIndexWriter<uint>::version) {
        cerr <<"file says it has version "<<currentversion<<" whereas this software has version "<<static_cast<uint>(ProjectionIndexWriter::version)<<endl;
        throw versionmismatch_exception("bad file version");
    }
    uint compressionmode;
    in.read(reinterpret_cast<char *> (& compressionmode), sizeof (compressionmode));
    in.read(reinterpret_cast<char *> (& bitsforcounters), sizeof (bitsforcounters));
    //cout<<"bitsforcounters="<<bitsforcounters<<endl;
    locationoffirstunreadblock = in.tellg();
    //cout<<"header finished at "<<locationoffirstunreadblock<<endl;
}
void CompressedProjectionIndexReader::process(const OrthogonalQuery & query, vector<ulong> & result) {
	if(bitsforcounters==8)
		templated_process<RLE<uint,unsigned char> >(query,result);
	else if (bitsforcounters==16)
		templated_process<RLE<uint,unsigned short> >(query,result);
	else if (bitsforcounters==32)
		templated_process<RLE<uint,uint32> >(query,result);
	else if(bitsforcounters == 512 + 32)
		templated_process<KaserArray<uint,uint32> >(query,result);
	else if(bitsforcounters == 512 + 64)
		templated_process<KaserArray<uint,uint64> >(query,result);
	else {
		cout<<"Don't know what to do with "<<bitsforcounters<<"-bit counters. Corrupted file?"<<endl;
		throw "bug";
	}
}
template <class ArrayType>
void CompressedProjectionIndexReader::templated_process(const OrthogonalQuery & query, vector<ulong> & result) {
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
        CompressedProjectionIndexDataBlock<ArrayType> db;
        //cout <<" reading block..."<<endl;
        db.readFromDisk(in, whichcolumns);
        if (!in) {
            //cout<<"file NOT good after readingblock..."<<endl;
            break;
        }
        //cout <<" reading block...done!"<<endl;
        if(!IOonly) db.process(query,avm,currentRowID, result);
        //cout <<" equality query, ok!..."<<endl;
        currentRowID += db.getNumberOfRows();
        //cout <<" row id is now "<<currentRowID<<endl;
    }
    in.close();
}





#endif /* COMPRESSEDPROJECTIONINDEX_H_ */
