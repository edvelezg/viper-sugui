#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "projectionindex.h"
#include "compressedprojectionindex.h"

using namespace std;
bool verbose(false);
bool justsolution(false);

//template<class DB>
template<class IndexReader>
void queryProjection(string indexfile, vector<OrthogonalQuery > queries, vector<vector<ulong> >& ans, uint repeat, bool doRand, bool IOonly) {
    double loading(0);
    double quering(0);
    //cout << "inside query projection..."<<endl;

    if (verbose == true) cout << "loading index file... " << endl;
    clock_t start, finish, startRand, finishRand;
    start = clock();

    //cout << "calling pir..."<<endl;
    IndexReader bi(indexfile);
    bi.IOonly = IOonly;
    //cout << "calling pir...ok"<<endl;

    finish = clock();
    loading = static_cast<double> (finish - start) / CLOCKS_PER_SEC;
    if (verbose == true) cout << "done in " << loading << " seconds  " << endl;
    //vector<vector<ulong> > ans;
    for (uint k = 0; k < queries.size(); ++k) {
        OrthogonalQuery & query = queries[k];
        vector<ulong> result;
        double randomAttr(0);
        long totalResults(0);

        ans.push_back(result);
        start = clock();
        for (uint ti = 0; ti < repeat; ++ti) {
            // compensate for randomization times
            if (doRand) {
                startRand = clock();
                /*
                for (map<uint, string>::iterator it = query.begin(); it != query.end(); ++it) {
                    int d = it->first;
                    //it->second =
                    bi.avm.getRandomAttributeValue(d, it->second); // can be  expensive; compensate for its time.
                    if (verbose)
                        cout << it->second << endl;
                }*/
                query.randomizeQuery(bi.avm);
                finishRand = clock();
                randomAttr += static_cast<double> (finishRand - startRand) / CLOCKS_PER_SEC;
            }
            ans[ans.size() - 1].clear();
            bi.process(query, ans[ans.size() - 1]);
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
        cout << "Usage: ./queryprojection somefile 1 Montreal 2 Dog" << endl;
        cout << "Example: ./queryprojection  testindex.bin  1 Montreal" << endl;
        cout << "If you want to output just the matching lines, use the -j flag." << endl;
        cout << "Without the verbose flag, the command returns the number of matching lines, " << endl;
        cout << "and  the time necessary to process the index." << endl;
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
    vector<OrthogonalQuery> query(1);

    //vector<ulong> answer;
    vector<ulong> c;

    uint repeat = 1;
    bool randomized = false;
    bool IOonly = false;
    // set up random seed (could override with command line if desired)
    struct timeval tv;
    gettimeofday(&tv, 0);
    srand(static_cast<int> (tv.tv_usec));

    for (int k = 2; k < params - 1; ++k) {
        if (strcmp(args[k], "-r") == 0)
            repeat = strtol(args[++k], NULL, 10);
        else if (strcmp(args[k], "-v") == 0)
            verbose = true;
        else if (strcmp(args[k], "-R") == 0) {
            randomized = true;
            repeat = strtol(args[++k], NULL, 10);
        } else if (strcmp(args[k], "-j") == 0) {
            justsolution = true;
        } else if(strcmp(args[k],"-i") == 0) {
        	IOonly = true;
        } else if (strcmp(args[k], "/") == 0) {
            query.resize(query.size() + 1);
        } else {
            query[query.size()-1].process(args,k,params);
        }
    }
    try {
        vector<vector<ulong> > out;
        try{
          queryProjection<ProjectionIndexReader>(indexfile, query, out, repeat, randomized,IOonly);
        } catch (cookiemismatch_exception ce) {
        	try{
        	queryProjection<CompressedProjectionIndexReader>(indexfile, query, out, repeat, randomized,IOonly);
        	} catch (cookiemismatch_exception ce) {
        		cerr<<"cookie mismatch (tried both compressed and uncompressed)"<<endl;
        	}
        }
    } catch (versionmismatch_exception ve) {
        cout << " a version mismatch means you must regenerate the index file." << endl;
    }
}
