#include "CSVFile.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

void CSVFile::readfile()
{
    string line;
    ifstream myfile ("example.csv");
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
            getline (myfile,line);
            cout << line << endl;
        }
        myfile.close();
    }

    else cout << "Unable to open file"; 
}

