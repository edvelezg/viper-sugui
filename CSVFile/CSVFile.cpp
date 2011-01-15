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
    uint32_t numcols = 0;
    ifstream myfile ("example.csv");

    if (myfile.is_open())
    {
        getline (myfile,line);
        vector<string> row( split(line, ",") );
        numcols = row.size();
        cout << numcols << endl;

        for (size_t i = 0; i < numcols; ++i)
        {
            map<string, uint32_t> a_map;
            histo.push_back(a_map);
        }

        while ( myfile.good() )
        {
            for (size_t i = 0; i < numcols; ++i)
            {
                histo[i][row[i]]++;
            }
            joint[row]++;
            getline (myfile,line);
            vector<string> row( split(line, ",") );
        }

        for (map_vector::const_iterator i = histo.begin(); i!= histo.end(); ++i)
        {
            const map<string, uint32_t> a_histo = *i;
            cout << a_histo.size();
        }

        myfile.close();
    }
    else cout << "Unable to open file"; 

    for (map_vector::const_iterator i = histo.begin(); i!= histo.end(); ++i)
    {
        const map<string, uint32_t> a_histo = *i;
        cout << a_histo.size();
    }
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
