#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include "BitmapIndex.h"
#include "querylanguage.h"
using namespace std;

/**
 * Query an unindexed CSV file.
 */

int main(int params, char ** args) {
    //std::ios::sync_with_stdio (false);
    if (params  <= 3) {
        cout << "You must give me a CSV file, a column number and an attribute value " << endl;
        cout << "Usage: ./querybitmap somefile 1 Montreal 2 Dog" << endl;
        cout << "Usage (verbose): ./querybitmap -v somefile 1 Montreal 2 Dog" << endl;
        return -1;
    }
    string delimiter(",");
    bool verbose(false);
    bool justmatching(false);
    OrthogonalQuery query;
    string csvfile(args[1]);
    for (int k = 2; k < params - 1; ++k) {
        if (strcmp(args[k], "-v") == 0)
            verbose = true;
        else if (strcmp(args[k], "-j") == 0) {
            justmatching = true;
        } else {
            query.process(args,k,params);
        }
    }
    if (verbose) cout << "going through CSV file... " << endl;
    clock_t start, finish;
    start = clock();
    vector<ulong> result;
    //if (!query.isEqualityQuery()) throw "Support for range queries has not been coded";
    CSVParser::query(csvfile, query, result, delimiter);
    finish = clock();
    double quering = static_cast<double> (finish - start) / CLOCKS_PER_SEC;
    if (verbose) {
        cout << "query in " << quering << " seconds  " << endl;
        cout << "matching lines: " << endl;
        printout(result);
    }
    if(justmatching)
        printout(result);
    if ((!verbose) and (!justmatching)) {
        //vector<float> answer;
        //answer.push_back(static_cast<float>(result.size()));
        //answer.push_back(loading);
        //answer.push_back(quering);
        cout << result.size() << " " << quering << endl;
        //printline(answer);
    }
}
