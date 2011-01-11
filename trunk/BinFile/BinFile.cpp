#include "BinFile.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <vector>

const static int BUF_SIZE = 4;                    // 4 * num of ints

// using std::ios_base;
using namespace std;

//Constructor for the C++ tutorial
BinFile::BinFile()
{
    magic   = 0x76;
    version = 1;
    numcols = 0;
    file = new std::ifstream("somefile.bin", ios_base::in | ios_base::binary);
// mData    = new std::vector<int>;
// bufWidth = 10;
}


BinFile::~BinFile()
{
// delete mData;
}


int BinFile::writefile()
{
    std::vector <uint32_t> mData;

    ofstream fout("InvDat.dat", ios::out | ios::binary);
    if (!fout)
    {
        cout << "Cannot open file.\n";
        return 1;
    }

    mData.push_back(magic);
    mData.push_back(version);
    mData.push_back(numcols);
    mData.push_back(300);
    mData.push_back(301);
    mData.push_back(392);

    for (unsigned int i = 0; i < mData.size(); i++)
        fout.write( reinterpret_cast<char*>(&mData[i]), sizeof(uint32_t) );
// fout.write((const char *) &mData[i], sizeof(uint32_t));

// fout.close();

    if (!fout.good())
    {
        cout << "A file error occurred.";
        return 1;
    }
    return 0;
}

int BinFile::readhdr()
{
	return 0;
}

void BinFile::readfile()
{
	uint32_t hdr_buf[3];
    file->read(reinterpret_cast<char*>(hdr_buf), sizeof(hdr_buf));
    magic   = hdr_buf[0];
    version = hdr_buf[1];
    numcols = hdr_buf[2];
	
    cout << ntohl(numcols) << endl;

    std::vector<uint32_t> buf(numcols);

    if(file->is_open())
    {
// while (file->read(reinterpret_cast<char*>(&buf[0]), sizeof(uint32_t)))
        while (file->read(reinterpret_cast<char*>(&buf[0]), sizeof(buf)))
        {
// uint32_t *num = reinterpret_cast<uint32_t* >(&buf[0]);
            std::cout << left << setw(10) << ntohl(buf[0]) << left << setw(10) << ntohl(buf[1]) << endl;
        }
        std::cout << endl;
    }

    file->close();
}


//     string filename = "InvDat.dat";
//     // std::vector <int> mData(bufWidth*4);
//     std::ifstream file(filename.c_str(), ios_base::in | ios_base::binary);
//
// // ios_base::fmtflags flags = cout.flags( );
//
//     uint32_t width = 14;
//     if(file.is_open())
//     {
//         while (file.read(reinterpret_cast<uint32_t*>(mData), bufWidth*4))
//         {
//             uint32_t *num = reinterpret_cast<uint32_t* >(mData);
//             for (uint32_t i = 0; i < bufWidth; ++i)
//             {
//                 std::cout << left << setw(width) << num[i] << " ";
//             }
//             std::cout << std::endl;
//         }
//         file.close();
//     }
