#include <vector>
#include <fstream>
// class std::vector<uint32_t>;
// class std::fstream;

using namespace std;

class BinFile
{
    public:
        BinFile();
        virtual ~BinFile();
        void readfile();
        void readfile_ntohl();
        int readhdr();
        int writefile();
        

    private:
        int magic;
        int version;
        uint32_t numcols;
        vector< vector<uint32_t> > matrix;
        std::ifstream ifile;
        std::ofstream ofile;
};
