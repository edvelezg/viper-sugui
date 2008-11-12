/***
 * Unit testing
 *
 * Run this to check out whether the code works!
 */

#include <fstream>
#include "BitmapWriter.h"
#include "BitmapIndex.h"
#include "CompressedDataBlock.h"
#include "CSVParser.h"
#include "Util.h"
#include "RLEUtil.h"
using namespace std;

static string testfailed = "---\ntest failed.\n\n\n\n\n\n";

bool testTabSeparatedText() {
    vector<map<string, uint> > x;
    map<string, uint> x1;
    map<string, uint> x2;
    map<string, uint> x3;
    x1["blabla"] = 142;
    x1["bla bla"] = 12;
    x2["zblabla"] = 142;
    x2["zbla bla"] = 12;
    x3["ablabla"] = 1142;
    x3["abla bla"] = 112;
    x.push_back(x1);
    x.push_back(x2);
    x.push_back(x3);
    string indexfile("testingTabSeparatedText.txt");
    ::unlink(indexfile.c_str());
    ofstream out(indexfile.c_str());
    saveVectorMapAsTabSeparatedText(x, out);
    out.close();
    ifstream in(indexfile.c_str());
    vector<map<string, uint> > y;
    loadVectorMapFromTabSeparatedText(y, in);
    in.close();
    if (x[0] != y[0]) {
        cout << testfailed << endl;
        return false;
    }
    if (x[1] != y[1]) {
        cout << testfailed << endl;
        return false;
    }
    if (x[2] != y[2]) {
        cout << testfailed << endl;
        return false;
    }
    if (x.size() != y.size()) {
        cout << testfailed << endl;
        return false;
    }
    ::unlink(indexfile.c_str());
    return true;
}

template<class uword>
bool testRunningLengthWord() {
    cout << "[testing RunningLengthWord]" << endl;
    bool isOk(true);
    uword somenumber(0xABCD);
    RunningLengthWord<uword> rlw(somenumber);
    rlw.setRunningBit(true);
    if (rlw.getRunningBit() != true) {
        cout << "failed to set the running bit " << sizeof (uword) << endl;
        isOk = false;
    }
    for (uword myrl = 0; myrl <= RunningLengthWord<uword>::largestrunninglengthcount; myrl+=RunningLengthWord<uword>::largestrunninglengthcount/10) {
        rlw.setRunningLength(myrl);
        if (rlw.getRunningBit() != true) {
            cout << "failed to set the running bit (2) " << sizeof (uword) << endl;
            isOk = false;
        }
        if (rlw.getRunningLength() != myrl) {
            cout << "failed to set the running length " << sizeof (uword) << endl;
            isOk = false;
        }
    }
    rlw.setRunningLength(12);
    for (uword mylw = 0; mylw <= RunningLengthWord<uword>::largestliteralcount; mylw+=RunningLengthWord<uword>::largestliteralcount/10) {
        rlw.setNumberOfLiteralWords(mylw);
        if (rlw.getRunningBit() != true) {
            cout << "failed to set the running bit (3) " << sizeof (uword) << endl;
            isOk = false;
        }
        if (rlw.getRunningLength() != 12) {
            cout << "failed to set the running length (2) " << sizeof (uword) << endl;
            isOk = false;
        }
        if (rlw.getNumberOfLiteralWords() != mylw) {
            cout << "failed to set the LiteralWords " <<mylw << " "<<sizeof (uword)<<" "<<rlw.getNumberOfLiteralWords() << endl;
            isOk = false;
        }
    }
    rlw.setNumberOfLiteralWords(43);
    rlw.setRunningBit(false);
    if (rlw.getRunningBit() != false) {
        cout << "failed to set the running bit (4) " << sizeof (uword) << endl;
        isOk = false;
    }
    if (rlw.getRunningLength() != 12) {
        cout << "failed to set the running length (3) " << sizeof (uword) << endl;
        isOk = false;
    }
    if (rlw.getNumberOfLiteralWords() != 43) {
        cout << "failed to set the LiteralWords (2) " << sizeof (uword) << endl;
        isOk = false;
    }
    if (!isOk) cout << testfailed << endl;
    return isOk;
}

template <class uword>
bool testBoolArray() {
    cout << "[testing BoolArray]" << endl;
    bool isOk(true);
    BoolArray<uword> ba(132);
    ba.set(2);
    ba.set(7);
    ba.set(42);
    if (!ba.get(2)) {
        cout << "expected a 1 bit at position 2" << endl;
        isOk = false;
    }
    if (!ba.get(7)) {
        cout << "expected a 1 bit at position 7" << endl;
        isOk = false;
    }
    if (!ba.get(42)) {
        cout << "expected a 1 bit at position 42" << endl;
        isOk = false;
    }
    for (int k = 0; k < 132; ++k)
        if ((k != 2) and(k != 7) and(k != 42))
            if (ba.get(k)) {
                cout << "expected a 0 bit at position " << k << endl;
                isOk = false;
            }
    string indexfile("testingboolarray.bin");
    ::unlink(indexfile.c_str());
    ofstream out(indexfile.c_str(), ios::out | ios::binary);
    ba.write(out);
    out.close();
    ifstream in(indexfile.c_str());
    BoolArray<uword> bain;
    bain.read(in);
    if (!bain.get(2)) {
        isOk = false;
        cout << "expected a 1 bit at position 2 (after reload)" << endl;
    }
    if (!bain.get(7)) {
        cout << "expected a 1 bit at position 7 (after reload)" << endl;
        isOk = false;
    }
    if (!bain.get(42)) {
        cout << "expected a 1 bit at position 42 (after reload)" << endl;
        isOk = false;
    }

    for (int k = 0; k < 132; ++k)
        if ((k != 2) and(k != 7) and(k != 42))
            if (bain.get(k)) {
                cout << "expected a 0 bit at position " << k << endl;
                isOk = false;
            }
    /*for(int k = 0; k < 45; ++k)
            cout << bain.get(k) << " ";
    cout << endl;*/
    in.close();
    if (isOk) ::unlink(indexfile.c_str());
    if (!isOk) cout << "testfailed." << endl;
    return isOk;
}

template <class uword>
bool testEWAHBoolArrayAppend() {
    cout << "[testing EWAHBoolArrayAppend]" << endl;
    bool isOk(true);
    uword zero = 0;
    uword specialval = 1UL + (1UL << 4)+(static_cast<uword>(1) << (sizeof(uword)*8-1));
    uword notzero = ~zero;
    EWAHBoolArray<uword> myarray1;
    BoolArray<uword> ba1;
    myarray1.add(zero);
    ba1.add(zero);
    myarray1.add(zero);
    ba1.add(zero);
    myarray1.add(zero);
    ba1.add(zero);
    myarray1.add(specialval);
    ba1.add(specialval);
    myarray1.add(specialval);
    ba1.add(specialval);
    myarray1.add(notzero);
    ba1.add(notzero);
    myarray1.add(zero);
    ba1.add(zero);
    EWAHBoolArray<uword> myarray2;
    BoolArray<uword> ba2;
    myarray2.add(notzero);
    ba2.add(notzero);
    myarray2.add(zero);
    ba2.add(zero);
    myarray2.add(notzero);
    ba2.add(notzero);
    myarray2.add(specialval);
    ba2.add(specialval);
    myarray2.add(specialval);
    ba2.add(specialval);
    myarray2.add(notzero);
    ba2.add(notzero);
    BoolArray<uword> aggregate1(ba1);
    BoolArray<uword> aggregate2(ba2);
    aggregate1.append(ba2);
    aggregate2.append(ba1);
    EWAHBoolArray<uword> caggregate1;
    caggregate1.append(myarray1);
    EWAHBoolArray<uword> caggregate2;
    caggregate2.append(myarray2);
    caggregate1.append(myarray2);
    caggregate2.append(myarray1);
    if (caggregate1 != aggregate1) {
        cout << "aggregate 1 failed" << endl;
        isOk = false;
    }
    if (caggregate2 != aggregate2) {
        cout << "aggregate 2 failed" << endl;
        isOk = false;
    }
    if (!isOk) cout << testfailed << endl;
    return isOk;
}

template<class uword>
bool testEWAHBoolArray() {
    cout << "[testing EWAHBoolArray]" << endl;
    bool isOk(true);
    EWAHBoolArray<uword> myarray;
    BoolArray<uword> ba(10 * sizeof (uword) * 8);
    uword zero = 0;
    uword notzero = ~zero;
    myarray.add(zero);
    ba.setWord(0, zero);
    myarray.add(zero);
    ba.setWord(1, zero);
    myarray.add(zero);
    ba.setWord(2, zero);
    //myarray.printout();
    uword specialval = 1UL + (1UL << 4)+(static_cast<uword>(1) << (sizeof(uword)*8-1));
    //cout << " SPECIAL VAL *************"<<specialval<<endl;
    myarray.add(specialval);
    ba.setWord(3, specialval);
    //myarray.printout();
    //cout << " not zero *************"<<endl;
    myarray.add(notzero);
    //myarray.printout();
    ba.setWord(4, notzero);
    myarray.add(notzero);
    ba.setWord(5, notzero);
    myarray.add(notzero);
    ba.setWord(6, notzero);
    myarray.add(notzero);
    ba.setWord(7, notzero);
    //myarray.printout();
    //cout << " SPECIAL VAL *************"<<specialval<<endl;
    myarray.add(specialval);
    ba.setWord(8, specialval);
    myarray.add(zero);
    ba.setWord(9, zero);
    if (myarray.sizeInBits() != 10 * sizeof (uword) * 8) {
        cout << "expected " << 10 * sizeof (uword) * 8 << " bits but found " << myarray.sizeInBits() << endl;
        isOk = false;
    }
    //myarray.printout();
    string indexfile("testingewahboolarray.bin");
    ::unlink(indexfile.c_str());
    ofstream out(indexfile.c_str(), ios::out | ios::binary);
    myarray.write(out);
    out.close();
    EWAHBoolArray<uword> lmyarray;
    ifstream in(indexfile.c_str());
    lmyarray.read(in);
    in.close();
    //cout << "checking equality..."<<endl;
    if (!(myarray == lmyarray)) {
        cout << "bad news, they are not equal" << endl;
        cout << "size in bits: " << myarray.sizeInBits() << " vs. " << lmyarray.sizeInBits() << endl;
        isOk = false;
    }
    //cout << "checking equality...ok"<<endl;
    EWAHBoolArrayIterator<uword> i = myarray.uncompress();
    EWAHBoolArrayIterator<uword> j = lmyarray.uncompress();
    uint k = 0;
    while (i.hasNext()) {
        if (!j.hasNext()) {
            cout<<"the two arrays don't have the same size?"<<endl;
            isOk = false;
            break;
        }
        uword val = i.next();
        uword val2 = j.next();
        uword valref = ba.getWord(k++);
        if (val != valref) {
            cout<<"the two arrays differ from uncompressed array at "<<k<<" "<< val<< " "<< val2<<" " <<valref << endl;
            isOk = false;
        }
        if (val != val2) {
            cout<<"the two arrays differ at "<<k<<" "<< val<< " "<< val2<<" " <<valref << endl;
            isOk = false;
        }
    }
    if (isOk) ::unlink(indexfile.c_str());
    if (!isOk) cout << testfailed << endl;
    return isOk;
}



template<class uword>
bool testEWAHBoolArrayLogical() {
    cout << "[testing EWAHBoolArrayLogical]" << endl;
    bool isOk(true);
    EWAHBoolArray<uword> myarray1;
    EWAHBoolArray<uword> myarray2;
    const uint N=15;
    uword allones = static_cast<uword>(~0L);
    uword x1[N] ={1,54,24,145,0,0,0,allones,allones,allones,43,0,0,0,1};
    uword x2[N] ={allones,0,0,0,0,0,0,0,allones,allones,allones,0,4,0,0};
    uword xand[N];
    uword xxor[N];
    for(uint k = 0; k < N; ++k) {
        myarray1.add(x1[k]);
        myarray2.add(x2[k]);
        xand[k] = x1[k] & x2[k];
        xxor[k] = x1[k] | x2[k];
    }
    EWAHBoolArray<uword> myand;
    EWAHBoolArray<uword> mysparseand;
    EWAHBoolArray<uword> myor;
    myarray1.rawlogicaland(myarray2,myand);
    myarray1.sparselogicaland(myarray2,mysparseand);
    myarray1.rawlogicalor(myarray2,myor);
    EWAHBoolArrayIterator<uword> i = myand.uncompress();
    EWAHBoolArrayIterator<uword> ii = mysparseand.uncompress();
    EWAHBoolArrayIterator<uword> j = myor.uncompress();
    EWAHBoolArrayIterator<uword> it1 = myarray1.uncompress();
    EWAHBoolArrayIterator<uword> it2 = myarray2.uncompress();
    for(uint k = 0; k <N;++k) {
        const uword m1 = it1.next();
        const uword m2 = it2.next();
        if(!i.hasNext()) {
            cout<<"type 1 error"<<endl;
            isOk=false; break;
        }
        if(!ii.hasNext()) {
            cout<<"type 2 error"<<endl;
            isOk=false; break;
        }
        if(!j.hasNext()) {
            cout<<"type 3 error"<<endl;
            isOk=false; break;
        }
        if(i.next()!= xand[k]){
            cout<<"type 4 error"<<endl;
            isOk=false; break;
        }
        if(ii.next()!= xand[k]){
            cout<<"type 5 error"<<endl;
            isOk=false; break;
        }
        const uword jor = j.next();
        if(jor!= xxor[k]){
            cout<<m1<<" or "<< m2<<" = "<< xxor[k] << " but got "<<jor <<endl;
            cout<<"type 6 error OR"<<endl;
            isOk=false; break;
        }
    }
    if (!isOk) cout << testfailed << endl;
    return isOk;
}

bool testCSV() {
    cout << "[testing CSV]" << endl;
    bool isOk(true);
    string filename("testdata.csv");
    uint expectedfactsperrow = 3;
    int expectednumberofrows = 45;
    int actualnumberofrows(0);
    ifstream in(filename.c_str());
    CSVParser csv(in);
    while (csv.hasNext()) {
        const vector<string> & line = csv.next();
        if (line.size() != expectedfactsperrow) {
            isOk = false;
            cout << "mismatch in the number of facts" << endl;
        }
        /**for(uint k = 0; k < line.size(); ++k)
                cout << line[k] << " ";
        cout << endl;**/
        ++actualnumberofrows;
    }
    if (actualnumberofrows != expectednumberofrows) {
        isOk = false;
        cout << "[csv]mismatch in the number of rows" << actualnumberofrows << " " << expectednumberofrows << endl;
    }
    in.close();
    if (!isOk) cout << testfailed << endl;
    return isOk;
}

template <class uword>
bool testDataBlock() {
    cout << "[testing DataBlock]" << endl;
    string indexfile("testingdatablock.bin");
    ::unlink(indexfile.c_str());
    int numberofbitmaps = 4;
    int numberofrows = 3;
    DataBlock<uword> db(4, 8);
    db.newRow();
    db.set(1);
    db.newRow();
    db.set(3);
    db.set(2);
    db.newRow();
    db.set(0);
    ofstream out(indexfile.c_str(), ios::out | ios::binary);
    bool isOk(true);
    if (db.getNumberOfRows() != numberofrows) isOk = false;
    if (db.getNumberOfBitmaps() != numberofbitmaps) isOk = false;
    if (!db.getBit(0, 1)) isOk = false;
    if (db.getBit(0, 0)) isOk = false;
    if (db.getBit(0, 2)) isOk = false;
    if (!db.getBit(1, 3)) isOk = false;
    if (db.getBit(1, 0)) isOk = false;
    if (!db.getBit(1, 2)) isOk = false;
    if (!db.getBit(2, 0)) isOk = false;
    //cout <<" saved : " << db.getNumberOfRows() <<" rows" <<endl;
    db.emptyToDisk(out);
    out.close();
    if (!db.empty()) {
        cout << "Empty to disk does not empty?" << endl;
        isOk = false;
    }
    ifstream in(indexfile.c_str());
    DataBlock<uword> dbin;
    dbin.readAllFromDisk(in);
    in.close();
    //cout <<" read : " << dbin.getNumberOfRows() <<" rows" <<endl;
    if (!dbin.getBit(0, 1)) isOk = false;
    if (!dbin.getBit(1, 3)) isOk = false;
    if (!dbin.getBit(1, 2)) isOk = false;
    if (!dbin.getBit(2, 0)) isOk = false;
    if (dbin.getNumberOfRows() != numberofrows) {
        cout << "mismatch in the number of rows : " << dbin.getNumberOfRows() << " " << numberofrows << endl;
        isOk = false;
    }
    if (dbin.getNumberOfBitmaps() != numberofbitmaps) {
        cout << "mismatch in the number of bitmaps : " << dbin.getNumberOfBitmaps() << " " << numberofbitmaps << endl;
        isOk = false;
    }
    if (isOk) ::unlink(indexfile.c_str());
    if (!isOk) cout << testfailed << endl;
    return isOk;
}

template <class CDB, class uword>
bool testCompressedDataBlock(uint blocksize) {
    cout << "[testing DataBlock with Compression] " << blocksize << " -- " << CDB::name() << endl;
    string indexfile("testingdatablock.bin");
    ::unlink(indexfile.c_str());
    int numberofbitmaps = 4;
    int numberofrows = 3;
    CDB db(4, blocksize);
    db.newRow();
    db.set(1);
    db.newRow();
    db.set(3);
    db.set(2);
    db.newRow();
    db.set(0);

    ofstream out(indexfile.c_str(), ios::out | ios::binary);
    bool isOk(true);
    if (db.getNumberOfRows() != numberofrows) {
        cout << "you should have " << numberofrows << " rows, but I see  " << db.getNumberOfRows() << endl;
        isOk = false;
    }
    if (db.getNumberOfBitmaps() != numberofbitmaps) {
        cout << "you should have " << numberofbitmaps << " rows, but I see  " << db.getNumberOfBitmaps() << endl;
        isOk = false;
    }
    /*if(!db.getBit(0,1)) isOk=false;
    if(db.getBit(0,0)) isOk=false;
    if(db.getBit(0,2)) isOk=false;
    if(!db.getBit(1,3)) isOk=false;
    if(db.getBit(1,0)) isOk=false;
    if(!db.getBit(1,2)) isOk=false;
    if(!db.getBit(2,0)) isOk=false;*/
    //cout <<" saved : " << db.getNumberOfRows() <<" rows" <<endl;
    db.emptyToDisk(out);
    out.close();
    if (!db.empty()) {
        cout << "Empty to disk does not empty?" << endl;
        isOk = false;
    }
    ifstream in(indexfile.c_str());
    // next line is not a mistake, you do not want to read a SparseCompressedDataBlock.
    ReadCompressedDataBlock<uword> dbin;
    dbin.readAllFromDisk(in);
    in.close();
    //cout <<" read : " << dbin.getNumberOfRows() <<" rows" <<endl;
    /*for(int row = 0; row < numberofrows; ++row) {
            for(int j = 0; j < numberofbitmaps; ++j)
                    cout << dbin.get(row,j)<< " ";
            cout << endl;
    }*/
    /*if(!dbin.getBit(0,1)) isOk=false;
    if(!dbin.getBit(1,3)) isOk=false;
    if(!dbin.getBit(1,2)) isOk=false;
    if(!dbin.getBit(2,0)) isOk=false;*/
    if (dbin.getNumberOfRows() != numberofrows) {
        cout << "Expected " << numberofrows << " but got " << dbin.getNumberOfRows() << " instead" << endl;
        isOk = false;
    }
    if (dbin.getNumberOfBitmaps() != numberofbitmaps) isOk = false;
    if (isOk) ::unlink(indexfile.c_str());
    if (!isOk) cout << testfailed << endl;
    return isOk;
}

template <class CDB, class uword>
bool testLargeCompressedDataBlock(uint blocksize) {
    cout << "[testing Large DataBlock with Compression]" << endl;
    string indexfile("testingdatablock.bin");
    bool isOk(true);
    ::unlink(indexfile.c_str());
    int numberofbitmaps = 2;
    CDB db(numberofbitmaps, blocksize);
    cout << "using a data block of type: " << db.name() << endl;
    ofstream out(indexfile.c_str(), ios::out | ios::binary);
    for (int k = 0; k < 100; ++k) {
        if (db.full()) {
            cout << "(writing) filled after " << db.getNumberOfRows() << endl;
            db.emptyToDisk(out);
            cout << "(writing) end of block at " << out.tellp() << endl;
        }
        db.newRow();
        db.set(0);
    }
    for (int k = 0; k < 100; ++k) {
        if (db.full()) {
            cout << "(writing) filled after " << db.getNumberOfRows() << " rows" << endl;
            db.emptyToDisk(out);
            cout << "(writing) end of block at " << out.tellp() << endl;
        }
        db.newRow();
        db.set(1);
    }
    cout << "(writing) terminate with " << db.getNumberOfRows() << " rows" << endl;
    if (db.getBuffer().size() != static_cast<uint> (numberofbitmaps)) {
        cout << "(end) wrong number of bitmaps: " << db.getBuffer().size() << endl;
        isOk = false;
    }
    db.emptyToDisk(out);
    cout << "out.tellp() " << out.tellp() << endl;
    out.close();
    // next, I will read on disk!
    ifstream in(indexfile.c_str());
    int counter = 0;
    while (in) {
        //cout<<"calling up a new block"<<endl;
        // next line is not a mistake, you do not want to read a SparseCompressedDataBlock.
        ReadCompressedDataBlock<uword> db;
        db.readAllFromDisk(in);
        cout << "(reading)  end of block at " << in.tellg() << endl;
        cout << "(reading) number of rows = " << db.getNumberOfRows() << " rows" << endl;
        if (!in) break;
        if (db.getNumberOfBitmaps() != numberofbitmaps) {
            cout << "wrong number of bitmaps: " << db.getNumberOfBitmaps() << endl;
            isOk = false;
        }
        map<uint, EWAHBoolArray<uword> > & x = db.getBuffer();
        cout << x.size() << endl;
        vector<BoolArray<uword> > y;
        for (uint k = 0; k < x.size(); ++k) {
            typename map<uint, EWAHBoolArray<uword> >::iterator cur = x.find(k);
            if (cur == x.end()) {
                isOk = false;
                cout << "Can't find bitmap " << k << endl;
            } else {
                BoolArray<uword> ba = cur->second.toBoolArray();
                y.push_back(ba);
            }
        }
        assert(y.size() > 0);
        bool errorInThisBlock(false);
        for (uint i = 0; i < y[0].sizeInBits(); ++i) {
            //cout <<y[0].get(i)<< " "<<y[1].get(i)<<endl;
            if (counter < 100) {
                if ((!y[0].get(i)) or(y[1].get(i))) {
                    isOk = false;
                    errorInThisBlock = true;
                }
            }else {
                if ((y[0].get(i)) or(!y[1].get(i))) {
                    isOk = false;
                    errorInThisBlock = true;
                }
            }
            ++counter;
        }
        if (errorInThisBlock) cout << "bad data in this block" << endl;
    }
    if (counter != 200) {
        cout << "I have entered 200 bits and got : " << counter << "bits out " << endl;
        isOk = false;
    }
    if (isOk) ::unlink(indexfile.c_str());
    if (!isOk) cout << testfailed << endl;
    return isOk;
}

/*
template<class uword>
bool testBitmapWriter(int kofn = 1, bool bufferfirstblock = true) {
    cout << "[testing BitmapWriter]" << endl;
    bool isOk(true);
    string csvfilename("testdata.csv");
    string indexfile("bitmapindex.bin");
    ::unlink(indexfile.c_str());
    KofNBitmapBudget KofN(kofn, false);
    BitmapWriter<DataBlock<uword> > bw(indexfile, 2000, KofN);
    cout << " {indexing the CVS file}" << endl;
    bw.indexCSVFile(csvfilename);
    cout << " {opening the index file} " << bufferfirstblock << endl;
    BitmapIndex<DataBlock> bi(indexfile, bufferfirstblock);
    // we seek all of the people from Montreal
    map<uint, string> query;
    query[1] = "Montreal";
    query[0] = "Daniel";
    cout << " {CVS query} " << endl;
    vector<ulong> trueresult;
    CSVParser::query(csvfilename, query, trueresult);
    cout << " {Indexed equality query} " << endl;
    vector<ulong> result;
    bi.equalityQuery(query, result, false);
    //printout(result);
    if (result != trueresult) {
        isOk = false;
        cout << " I was expecting: " << endl;
        printout(trueresult);
        cout << " I got:" << endl;
        printout(result);
    }
    if (!isOk) cout << testfailed << endl;
    if (isOk) ::unlink(indexfile.c_str());
    return isOk;
}*/

/*
bool testAdaptiveBitmapWriter(int kofn = 1, bool bufferfirstblock = true) {
        cout << "[testing BitmapWriter]" << endl;
        bool isOk(true);
        string csvfilename("testdata.csv");
        string indexfile("bitmapindex.bin");
        ::unlink(indexfile.c_str());
        KofNBitmapBudget KofN(kofn,false);
        BitmapWriter<DataBlock> bw(indexfile,2000,KofN,gAdaptiveBitmapAllocation);
        cout <<" {indexing the CVS file}" <<endl;
        bw.indexCSVFile(csvfilename);
        cout <<" {opening the index file} "<< bufferfirstblock<<endl;
        BitmapIndex<DataBlock> bi(indexfile,bufferfirstblock);
        // we seek all of the people from Montreal
        map<uint, string> query;
        query[1]="Montreal";
        cout <<" {CVS query} "<<endl;
        vector<ulong> trueresult;
        CSVParser::query(csvfilename,query,trueresult);
        cout <<" {Indexed equality query} "<<endl;
        vector<ulong> result;
        bi.equalityQuery(query,result,false);
        //printout(result);
        if(result != trueresult) {
                isOk=false;
                cout<< " I was expecting: "<<endl;
                printout(trueresult);
                cout<< " I got:" <<endl;
                printout(result);
        }
        if(!isOk) cout << testfailed << endl;
        if(isOk) ::unlink(indexfile.c_str());
        return isOk;
}*/

template<class uword>
bool testCompressedBitmapWriter(int kofn = 1, bool sparse = false) {
    cout << "[testing BitmapWriter with compression]" << endl;
    bool isOk(true);
    string csvfilename("testdata.csv");
    string indexfile("compressedbitmapindex.bin");
    ::unlink(indexfile.c_str());
    KofNBitmapBudget KofN(kofn, false);
    BitmapWriter<CompressedDataBlock<uword> > bw(indexfile, defaultUncompressedBlockSize, KofN);
    cout << " {indexing the CVS file}" << endl;
    bw.indexCSVFile(csvfilename);
    cout << " {opening the index file}" << endl;
    BitmapIndex bi(indexfile);
    // we seek all of the people from Montreal
    map<uint, set<string> > query;
    set<string> s1; s1.insert("Montreal");
    set<string> s0; s0.insert("Daniel");
    query.insert(pair<uint,set<string> >(1, s1));
    query.insert(pair<uint,set<string> >(0, s0));
    vector<ulong> result;
    OrthogonalQuery oq;
    oq.equality_query = query;
    cout << " {CVS query} " << endl;
    vector<ulong> trueresult;
    CSVParser::query(csvfilename, oq, trueresult);
    cout << " {Indexed equality query} " << endl;
    cout << "sparseness:" << sparse << endl;
    bi.process(oq, result, sparse);
    //printout(result);
    if (result != trueresult) {
        isOk = false;
        cout << " I was expecting: " << endl;
        printout(trueresult);
        cout << " I got:" << endl;
        printout(result);
    }
    if (!isOk) cout << testfailed << endl;
    if (isOk) ::unlink(indexfile.c_str());
    return isOk;
}

template<class valuetype, class countertype>
bool testRLE() {
	bool isOk(true);
	const uint N=30;
	unsigned char s [N] ={2,2,1,4,1,5,88,88,88,88,88,88,88,88, 108,32,32,24,24,24,24,24,24,24,32,32,32,32,14,15};//"lalalaaaaa bfdfd  fffffds fdsfdsi09u0no99999fd";
	RLEArrayWriter<valuetype,countertype> w;
	for(uint k=0; k<N;++k) {
		w.add(static_cast<unsigned char>(s[k]));
	}
	w.flush();
	uint pos(0);
	RLEArrayReader<valuetype,countertype> r(w.container);
	while(r.hasNext()) {
		r.next();
		//cout<<" I see "<<r.value<<" repeated "<<r.count<<" times"<<endl;
		for(uint i = 0; i<r.count; ++i) {
			//cout<<r.value<<" "<<static_cast<unsigned int>(s[pos])<<endl;
			if(r.value !=  static_cast<unsigned char>(s[pos])) {
				isOk=false;
			}
			++pos;
		}
	}
	if(!(pos == N)) isOk=false;
    if (!isOk) cout << testfailed << endl;
    return isOk;
}


template<class valuetype, class countertype>
bool testLargeRLE() {
	bool isOk(true);
	const uint N=100000;
	vector<unsigned char> s;
	for(uint k =0; k<N;++k)
		s.push_back(0);
	RLEArrayWriter<valuetype,countertype> w;
	for(uint k=0; k<N;++k) {
		w.add(static_cast<unsigned char>(s[k]));
	}
	w.flush();
	uint pos(0);
	RLEArrayReader<valuetype,countertype> r(w.container);
	while(r.hasNext()) {
		r.next();
		//cout<<" I see "<<r.value<<" repeated "<<r.count<<" times"<<endl;
		for(uint i = 0; i<r.count; ++i) {
			//cout<<r.value<<" "<<static_cast<unsigned int>(s[pos])<<endl;
			if(r.value !=  static_cast<unsigned char>(s[pos])) {
				isOk=false;
			}
			++pos;
		}
	}
	if(!(pos == N)) isOk=false;
    if (!isOk) cout << testfailed << endl;
    return isOk;
}

void tellmeaboutmachine() {
    cout << "number of bytes in ostream::pos_type = " << sizeof (ostream::pos_type) << endl;
    if (sizeof (ulong) > 4)
        cout << "you have a 64-bit machine" << endl;
    else
        cout << "you have a 32-bit machine" << endl;
#if  __LITTLE_ENDIAN__
    cout << "you have little endian machine" << endl;
#endif
#if  __BIG_ENDIAN__
    cout << "you have a big endian machine" << endl;
#endif
    if (__CHAR_BIT__ != 8)
        cout << "on your machine, chars don't have 8bits???" << endl;
}

int main(void) {
    cout << "Welcome to the Lemur Bitmap Index Testing Tool." << endl;
    int failures = 0;
    if (!testTabSeparatedText())++failures;
    if (!testRunningLengthWord<uword16 > ())++failures;
    if (!testRunningLengthWord<uword32 > ())++failures;
    if (!testRunningLengthWord<uword64 > ())++failures;
    if (!testCSV())++failures;
    if (!testBoolArray<uword16 > ())++failures;
    if (!testBoolArray<uword32 > ())++failures;
    if (!testBoolArray<uword64 > ())++failures;
    if (!testDataBlock<uword16 > ())++failures;
    if (!testDataBlock<uword32 > ())++failures;
    if (!testDataBlock<uword64 > ())++failures;
    //if (!testBitmapWriter())++failures;
    //if (!testBitmapWriter(2))++failures;
    //f (!testBitmapWriter(3))++failures;
    //if (!testBitmapWriter(1, false))++failures;
    //if (!testBitmapWriter(2, false))++failures;
    //if (!testBitmapWriter(3, false))++failures;
    if (!testEWAHBoolArray<uword16 > ())++failures;
    if (!testEWAHBoolArray<uword32 > ())++failures;
    if (!testEWAHBoolArray<uword64 > ())++failures;

    if (!testEWAHBoolArrayLogical<uword16 > ())++failures;
    if (!testEWAHBoolArrayLogical<uword32 > ())++failures;
    if (!testEWAHBoolArrayLogical<uword64 > ())++failures;

    if (!testEWAHBoolArrayAppend<uword16 > ())++failures;
    if (!testEWAHBoolArrayAppend<uword32 > ())++failures;
    if (!testEWAHBoolArrayAppend<uword64 > ())++failures;
    for (uint k = 1; k < 66; ++k) {
        if (!testCompressedDataBlock<CompressedDataBlock<uword16>, uword16 > (k))++failures;
        if (!testCompressedDataBlock<CompressedDataBlock<uword32>, uword32 > (k))++failures;
        if (!testCompressedDataBlock<CompressedDataBlock<uword64>, uword64 > (k))++failures;
    }
    for (uint k = 100; k < 7000; k += 100) {
        if (!testCompressedDataBlock<CompressedDataBlock<uword16>, uword16 > (k))++failures;
        if (!testCompressedDataBlock<CompressedDataBlock<uword32>, uword32 > (k))++failures;
        if (!testCompressedDataBlock<CompressedDataBlock<uword64>, uword64 > (k))++failures;
    }
    for (uint k = 1; k < 66; ++k) {
        if (!testCompressedDataBlock<SparseCompressedDataBlock<uword16>, uword16 > (k))++failures;
        if (!testCompressedDataBlock<SparseCompressedDataBlock<uword32>, uword32 > (k))++failures;
        if (!testCompressedDataBlock<SparseCompressedDataBlock<uword64>, uword64 > (k))++failures;
    }
    for (uint k = 100; k < 7000; k += 100) {
        if (!testCompressedDataBlock<SparseCompressedDataBlock<uword16>, uword16 > (k))++failures;
        if (!testCompressedDataBlock<SparseCompressedDataBlock<uword32>, uword32 > (k))++failures;
        if (!testCompressedDataBlock<SparseCompressedDataBlock<uword64>, uword64 > (k))++failures;
    }
    for (uint k = 1; k < 66; ++k) {
        if (!testLargeCompressedDataBlock<CompressedDataBlock<uword16>, uword16 > (k))++failures;
        if (!testLargeCompressedDataBlock<CompressedDataBlock<uword32>, uword32 > (k))++failures;
        if (!testLargeCompressedDataBlock<CompressedDataBlock<uword64>, uword64 > (k))++failures;
        if (!testLargeCompressedDataBlock<SparseCompressedDataBlock<uword16>, uword16 > (k))++failures;
        if (!testLargeCompressedDataBlock<SparseCompressedDataBlock<uword32>, uword32 > (k))++failures;
        if (!testLargeCompressedDataBlock<SparseCompressedDataBlock<uword64>, uword64 > (k))++failures;
    }

    for (uint k = 1000; k < 16000; k += 1000) {
        if (!testLargeCompressedDataBlock<CompressedDataBlock<uword16>, uword16 > (k))++failures;
        if (!testLargeCompressedDataBlock<CompressedDataBlock<uword32>, uword32 > (k))++failures;
        if (!testLargeCompressedDataBlock<CompressedDataBlock<uword64>, uword64 > (k))++failures;
        if (!testLargeCompressedDataBlock<SparseCompressedDataBlock<uword16>, uword16 > (k))++failures;
        if (!testLargeCompressedDataBlock<SparseCompressedDataBlock<uword32>, uword32 > (k))++failures;
        if (!testLargeCompressedDataBlock<SparseCompressedDataBlock<uword64>, uword64 > (k))++failures;
    }
    for (uint k = 1; k < 5; ++k) {
        if (!testCompressedBitmapWriter<uword16 > (k, false))++failures;
        if (!testCompressedBitmapWriter<uword16 > (k, true))++failures;
        if (!testCompressedBitmapWriter<uword32 > (k, false))++failures;
        if (!testCompressedBitmapWriter<uword32 > (k, true))++failures;
        if (!testCompressedBitmapWriter<uword64 > (k, false))++failures;
        if (!testCompressedBitmapWriter<uword64 > (k, true))++failures;
    }
    if(! testRLE<uint,unsigned char>() ) ++ failures;
    if(! testRLE<uint,unsigned short>() ) ++ failures;
    if(! testRLE<uint,unsigned int>() ) ++ failures;
    if(! testLargeRLE<uint,unsigned char>() ) ++ failures;
    if(! testLargeRLE<uint,unsigned short>() ) ++ failures;
    if(! testLargeRLE<uint,unsigned int>() ) ++ failures;
    tellmeaboutmachine();
    if (failures == 0) {
        cout << "Your code is ok." << endl;
        return EXIT_SUCCESS;
    }
    cout << "Got " << failures << " failed tests!" << endl;
    return EXIT_FAILURE;
}
