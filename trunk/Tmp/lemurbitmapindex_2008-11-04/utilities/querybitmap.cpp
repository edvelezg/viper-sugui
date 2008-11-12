#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "BitmapIndex.h"
#include "querylanguage.h"

/**
 * as the name suggests, the goal of this program is to query a bitmap
 * index. The syntax is described in the ../doc/ HTML files.
 * Basically, it only supports equality queries.
 */

using namespace std;
bool verbose(false);
bool justsolution(false);

//template<class DB>

void queryBitmap(string indexfile, vector<OrthogonalQuery> & queries/*vector<map<uint, string> > queries*/, vector<vector<ulong> >& ans, int hint, uint repeat, bool doRand) {
    double loading(0);
    double quering(0);

    if (verbose == true) cout << "loading index file... " << endl;
    clock_t start, finish, startRand, finishRand;
    start = clock();
    BitmapIndex bi(indexfile);
    finish = clock();
    loading = static_cast<double> (finish - start) / CLOCKS_PER_SEC;
    if (verbose == true) cout << "done in " << loading << " seconds  " << endl;
    if (verbose) cout << "Size of header (in bytes) = " << bi.sizeOfHeader() << endl;

    if (verbose == true) cout << " size in bytes of the preloaded data block = " << bi.sizeOfPreloadedBlock() << endl;
    //vector<vector<ulong> > ans;
    for (uint k = 0; k < queries.size(); ++k) {
        //map<uint, string> & query = queries[k].equality_query;
        vector<ulong> result;
        double randomAttr(0);
        long totalResults(0);

        ans.push_back(result);
        start = clock();
        for (uint ti = 0; ti < repeat; ++ti) {
            // compensate for randomization times
            if (doRand) {
                startRand = clock();
                queries[k].randomizeQuery(bi.avm);
                /*for (map<uint, string>::iterator it = query.begin(); it != query.end(); ++it) {
                    int d = it->first;
                    //it->second = 
                    bi.randomAttribute(d, it->second); // can be  expensive; compensate for its time.
                    if (verbose)
                        cout << it->second << endl;
                }*/
                finishRand = clock();
                randomAttr += static_cast<double> (finishRand - startRand) / CLOCKS_PER_SEC;
            }
            ans[ans.size() - 1].clear();
            bi.process(queries[k], ans[ans.size() - 1], hint);
            totalResults += ans[ans.size() - 1].size(); // compute an average (harmless for nonrandom)
        }
        //for(int k = 0; k < 10000;++k) bi.equalityQuery(query,sparse);
        finish = clock();
        quering = static_cast<double> (finish - start) / CLOCKS_PER_SEC;
        // adjust for the randomization, if any
        quering -= randomAttr;
        //cout << justsolution << endl;
        if (verbose == true) {
            if (doRand && (repeat > 1)) cout << "only the last randomize result shown" << endl;
            cout << "query in " << quering << " seconds  " << endl;
            cout << "matching lines: " << endl; // only the last query
            printout(ans[ans.size() - 1]);
        } else if (justsolution) {
            printout(ans[ans.size() - 1]);
        } else {
            //vector<float> answer;
            //answer.push_back(static_cast<float>(ans[ans.size()-1].size()));
            //answer.push_back(loading);
            //answer.push_back(quering);
            cout << (totalResults / repeat) << " " << quering << endl;
            //printline(answer);
        }
    }
}

int main(int params, char ** args) {
    std::ios::sync_with_stdio(false);
    if (params <= 3) {
        cout << "You must give me an index file, a column number and an attribute value " << endl;
        cout << "Usage: ./querybitmap somefile 1 Montreal 2 Dog" << endl;
        cout << "You may use the -c flag for compression before the column numbers and attribute values" << endl;
        cout << "Example: ./querybitmap  testindex.bin -c 1 Montreal" << endl;
        cout << "Example: ./querybitmap  testindex.bin -s -c 1 Montreal  (for sparse bitmap indexes)" << endl;
        cout << "If you want to output just the matching lines, use the -j flag." << endl;
        cout << "Without the verbose flag, the command returns the number of matching lines, " << endl;
        cout << "and  the time necessary to process the bitmaps." << endl;
        cout << "You can enter several queries by separating them with \"/\"" << endl;
        cout << "For timing purposes, you can also use the '-r' flag followed by a number, to repeat the query several times." << endl;
        cout << "Or you can use the -R flag, followed by a number, to repeat random queries on dimension (ignores given attribute, which must still be given)" << endl;
        return -1;
    }
    /*if (verbose==true){
    if(sizeof(ulong)>4)
                    cout <<"you have a 64-bit machine, expecting a 64-bit index..."<<endl;
            else 
                    cout <<"you have a 32-bit machine, expecting a 32-bit index..."<<endl;
    }*/
    string indexfile(args[1]);
    //vector<map<uint, string> > query(1);
    vector<OrthogonalQuery> query(1);
    bool compression(true);
    //vector<ulong> answer;
    vector<ulong> c;
    bool sparse = false;
    uint repeat = 1;
    bool randomized = false;

    // set up random seed (could override with command line if desired)
    struct timeval tv;
    gettimeofday(&tv, 0);
    srand(static_cast<int> (tv.tv_usec));

    for (int k = 2; k < params - 1; ++k) {
        if (strcmp(args[k], "-c") == 0) {
            compression = true;
            cerr << " The compression flag is obselete. As of July 2008, support for uncompressed bitmaps has been removed." << endl;
        } else if (strcmp(args[k], "-s") == 0)
            sparse = true;
        else if (strcmp(args[k], "-r") == 0)
            repeat = strtol(args[++k], NULL, 10);
        else if (strcmp(args[k], "-v") == 0)
            verbose = true;
        else if (strcmp(args[k], "-R") == 0) {
            randomized = true;
            repeat = strtol(args[++k], NULL, 10);
        } else if (strcmp(args[k], "-j") == 0) {
            justsolution = true;
        } else if (strcmp(args[k], "/") == 0) {
            query.resize(query.size() + 1);
        } else {
            query[query.size()-1].process(args,k,params);
            /*
            ulong column = strtol(args[k], NULL, 10);
            if (verbose == true) cout << "column " << column << " value " << args[k + 1] << endl;
            map<uint, string>::iterator x = query[query.size() - 1].find(column);
            if (x != query[query.size() - 1].end()) {
                cerr << "Warning: you entered the same dimension (" << strtol(args[k], NULL, 10) << ") at least twice" << endl;
            }
            query[query.size() - 1][column] = string(args[k + 1]);
            ++k;*/
        }
    }
    try {
        vector<vector<ulong> > out;
        queryBitmap(indexfile, query, out, sparse ? HINT_SPARSE : HINT_NONE, repeat, randomized);
    } catch (cookiemismatch_exception ce) {
        cout << " a cookie mismatch means you probably do not have an index file." << endl;
    } catch (versionmismatch_exception ve) {
        cout << " a version mismatch means you must regenerate the index file." << endl;
    }
}
