#include "BinFile.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <vector>
#include <netinet/in.h>

const static int BUF_SIZE = 4;                    // 4 * num of ints

using namespace std;

BinFile::BinFile(const string &inname, const string &outname)
{
    magic   = 0x76;
    version = 1;
    numcols = 0;
    ifile.open(inname.c_str(), ios::in | ios::binary);
    ofile.open(outname.c_str(), ios::out | ios::binary);
}

BinFile::~BinFile()
{
}


int BinFile::writefile()
{
    vector<uint32_t> a_row(2);
    a_row[0] = 1;
    a_row[1] = 2;
    matrix.push_back(a_row);
    a_row[0] = 3;
    a_row[1] = 2;
    matrix.push_back(a_row);
    a_row[0] = 5;
    a_row[1] = 4;
    matrix.push_back(a_row);
    a_row[0] = 7;
    a_row[1] = 4;
    matrix.push_back(a_row);
    a_row[0] = 9;
    a_row[1] = 6;
    matrix.push_back(a_row);

    if (!ofile)
    {
        cout << "Cannot open file.\n";
        return 1;
    }

    uint32_t hdr_buf[3];

    hdr_buf[0] = magic;
    hdr_buf[1] = version;
    hdr_buf[2] = numcols;

    ofile.write(reinterpret_cast<char*>(hdr_buf), sizeof(hdr_buf));

    cout << matrix.size() << endl;
    for(size_t i = 0; i < matrix.size(); ++i)
    {
        ofile.write( reinterpret_cast<char*>(&matrix[i][0]), sizeof(matrix[i][0])*matrix[i].size() );
        // for(size_t j = 0; j < numcols; ++j)
        // {
        //     std::cout << left << setw(10) << matrix[i][j];
        //       
        //     // ofile.write( reinterpret_cast<char*>(&matrix[i][j]), sizeof(uint32_t) );
        // }
        // cout << endl;
    }

    if (!ofile.good())
    {
        cout << "A file error occurred.";
        return 1;
    }
    ofile.close();

    return 0;
}


int BinFile::readhdr()
{
    return 0;
}


void BinFile::readfile_ntohl()
{
    uint32_t hdr_buf[3];

    ifile.read(reinterpret_cast<char*>(hdr_buf), sizeof(hdr_buf));

    magic   = ntohl(hdr_buf[0]);
    version = ntohl(hdr_buf[1]);
    numcols = ntohl(hdr_buf[2]);

    vector<uint32_t> buf(numcols);
    if(ifile.is_open())
    {
        while (ifile.read(reinterpret_cast<char*>(&buf[0]), sizeof(buf)))
        {
            for(size_t i = 0; i < numcols; ++i)
            {
                buf[i] = ntohl(buf[i]);
                cout << left << setw(10) << buf[i];
            }
            matrix.push_back(buf);
            cout << endl;
        }
    }
    ifile.close();
}

void BinFile::readfile()
{
    uint32_t hdr_buf[3];

    ifile.read(reinterpret_cast<char*>(hdr_buf), sizeof(hdr_buf));

    magic   = hdr_buf[0];
    version = hdr_buf[1];
    numcols = hdr_buf[2];
    cout << "magic: " << magic << endl;
    cout << "version: " << version << endl;
    cout << "numcols: " << numcols << endl;

    vector<uint32_t> buf(numcols);
    if(ifile.is_open())
    {
        while (ifile.read(reinterpret_cast<char*>(&buf[0]), sizeof(uint32_t)*numcols))
        {
            for(size_t i = 0; i < numcols; ++i)
            {
                std::cout << left << setw(10) << buf[i];
            }
            matrix.push_back(buf);
            cout << endl;
        }
    }
    ifile.close();
}

// vector< vector<uint32_t> > matrix;
// 
// for(size_t i = 0; i < matrix.size(); ++i) 
//   ofile.write( (char*)&matrix[i][0], sizeof(matrix[i][0])*matrix[i].size() );

//     string ifilename = "InvDat.dat";
//     // std::vector <int> mData(bufWidth*4);
//     std::ifstream ifile(ifilename.c_str(), ios_base::in | ios_base::binary);
//
// // ios_base::fmtflags flags = cout.flags( );
//
//     uint32_t width = 14;
//     if(ifile.is_open())
//     {
//         while (ifile.read(reinterpret_cast<uint32_t*>(mData), bufWidth*4))
//         {
//             uint32_t *num = reinterpret_cast<uint32_t* >(mData);
//             for (uint32_t i = 0; i < bufWidth; ++i)
//             {
//                 std::cout << left << setw(width) << num[i] << " ";
//             }
//             std::cout << std::endl;
//         }
//         ifile.close();
//     }
