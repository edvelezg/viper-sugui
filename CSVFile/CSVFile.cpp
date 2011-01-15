#include "CSVFile.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <iterator>

using namespace std;

void CSVFile::readfile()
{
    string line;
    uint32_t numcols = 2;
    ifstream myfile ("example.csv");

    if (myfile.is_open())
    {
        getline (myfile,line);
        vector<string> row( split(line, ",") );
        numcols = row.size();
        // 
        for (size_t i = 0; i < numcols; ++i)
        {
            map<string, uint32_t> a_map;
            histo.push_back(a_map);
            histo[i][row[i]]++;
        }
        joint[row]++;
        // cout << row[0] << " " << row[1] << endl;
        
        while ( myfile.good() )
        {
            getline (myfile,line);
            vector<string> row( split(line, ",") );
            
            for (size_t i = 0; i < numcols; ++i)
            {
                if(!row[i].empty())
                {
                    histo[i][row[i]]++;
                }
            }
            // cout << row[0] << " " << row[1] << endl;
            joint[row]++;
        }
        
        for(size_t i = 0; i < histo.size(); ++i)
        {
            cout << "cardinality of column " << i << " is " << histo[i].size() << endl;
        }
        cout << "joint cardinality is: " << joint.size() << endl;
        
        myfile.close();
    }
    else cout << "Unable to open file"; 
}

inline vector<string> CSVFile::split( const string& s, const string& f ) {
    vector<string> temp;
    if ( f.empty() )
    {
        temp.push_back( s );
        return temp;
    }
    typedef string::const_iterator iter;
    const iter::difference_type f_size( distance( f.begin(), f.end() ) );
    iter i( s.begin() );
    for ( iter pos; ( pos = search( i , s.end(), f.begin(), f.end() ) ) != s.end(); )
    {
        temp.push_back( string( i, pos ) );
        advance( pos, f_size );
        i = pos;
    }
    temp.push_back( string( i, s.end() ) );
    return temp;
}
