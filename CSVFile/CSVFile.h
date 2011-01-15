#ifndef __CSVFILE_H_INCL__
#define __CSVFILE_H_INCL__

#include <vector>
#include <map>
#include <string>

using namespace std;

typedef vector < map<string, uint32_t> >  map_vector;
typedef map< vector<string>, uint32_t > pair_map;

/**
 * TODO: Add class description
 * 
 * @author   Beatriz
 */
class CSVFile
{
public:
    // Constructor
    CSVFile();
    void readfile();
    vector<string> CSVFile::split( const string&, const string& );
    virtual ~CSVFile();

private:
    map_vector histo;
    pair_map joint;
};

// Constructor implementation
inline CSVFile::CSVFile()
{
}

// Destructor implementation
inline CSVFile::~CSVFile()
{
}

#endif // __CSVFILE_H_INCL__
