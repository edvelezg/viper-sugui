#include "BinFile.h"
#include <iostream>
#include <iterator>
#include <iomanip>
#include <string>

const static int BUF_SIZE = 4;                    // 4 * num of ints

using namespace std;

BinFile::BinFile(const string &filename)
{
    magic   = 0x76;
    version = 1;
    numcols = 0;
    ifile.open(filename.c_str(), ios::in | ios::binary);
}


BinFile::~BinFile()
{
}


void BinFile::readhdr()
{
    uint32_t hdr_buf[3];

    ifile.read(reinterpret_cast<char*>(hdr_buf), sizeof(hdr_buf));

    magic   = hdr_buf[0];
    version = hdr_buf[1];
    numcols = hdr_buf[2];

    if(!(magic == MAGIC) || (version != VERSION))
    {
        cout << "MAGIC number or VERSION mismatch." << endl;
    }
    cout << "found " << numcols << " columns." << endl;
}

void BinFile::readfile()
{
    readhdr();
    if( numcols < 1 )
    {
        cout << "No columns found." << endl;
        return;
    }

    // Initialize map
    for(size_t i = 0; i < numcols; ++i)
    {
        map<uint32_t, uint32_t> a_map;
        histo.push_back(a_map);
    }

    vector<uint32_t> buf(numcols);

    if(ifile.is_open())
    {
        while (ifile.read(reinterpret_cast<char*>(&buf[0]), sizeof(uint32_t)*numcols))
        {
            for(size_t i = 0; i < numcols; ++i)
            {
                histo[i][buf[i]]++;
            }
            // pair<uint32_t, uint32_t> mypair(buf[0], buf[1]);
            joint[buf]++;
        }
    }    
}

uint32_t BinFile::card_at(uint32_t column)
{
    return histo[column].size();
}

uint32_t BinFile::joint_card()
{
    return joint.size();
}

double BinFile::calc_dan_corr()
{
    // ifile.clear();
    // ifile.seekg(0, ios::beg);
    double prod_card = 1.0;
    for (map_vector::const_iterator i = histo.begin(); i!= histo.end(); ++i)
    {
        const map<uint32_t, uint32_t> a_histo = *i;
        prod_card *= a_histo.size();
    }
    cout << "prod_card: "  << prod_card << endl;
    
    double joint_card = joint.size();
    double nmtr  = 1.0;
    double dnmtr = 1.0;
    for (map_vector::const_iterator i = histo.begin(); i!= histo.end(); ++i)
    {
        const map<uint32_t, uint32_t> a_histo = *i;        
        nmtr *= joint_card - a_histo.size();
        dnmtr *= prod_card - a_histo.size();
        cout << "nmtr: "<< nmtr << endl;
        cout << "dnmtr: "<< dnmtr << endl;
    }
    
    return 1.0-(nmtr/dnmtr);
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
