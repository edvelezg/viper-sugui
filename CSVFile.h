#ifndef __CSVFILE_H_INCL__
#define __CSVFILE_H_INCL__

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

    // Destructor
    virtual ~CSVFile();

    // Copy constructor
    // TODO: Uncomment the copy constructor when you need it.
    //CSVFile(const CSVFile& src);

    // Assignment operator
    // TODO: Uncomment the assignment operator when you need it.
    //CSVFile& operator=(const CSVFile& src);
};

// Constructor implementation
inline CSVFile::CSVFile()
{
}

// Destructor implementation
inline CSVFile::~CSVFile()
{
}

// TODO: Uncomment the copy constructor when you need it.
//inline CSVFile::CSVFile(const CSVFile& src)
//{
//   // TODO: copy
//}

// TODO: Uncomment the assignment operator when you need it.
//inline CSVFile& CSVFile::operator=(const CSVFile& rhs)
//{
//   if (this == &rhs) {
//      return *this;
//   }
//
//   // TODO: assignment
//
//   return *this;
//}

#endif // __CSVFILE_H_INCL__
