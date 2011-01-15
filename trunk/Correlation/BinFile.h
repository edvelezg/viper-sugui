#include <vector>
#include <map>
#include <fstream>
// class std::vector<uint32_t>;
// class std::fstream;
#define MAGIC       0x76
#define VERSION     1

using namespace std;

typedef vector < map<uint32_t, uint32_t> >  map_vector;
typedef map< vector<uint32_t>, uint32_t > pair_map;

class BinFile
{
    public:
        BinFile(const string &);
        virtual ~BinFile();
        void readhdr();
        void readfile();
        uint32_t card_at(uint32_t);
        uint32_t joint_card();
        double calc_dan_corr();

    private:
        int magic;
        int version;
        uint32_t numcols;
        vector< vector<uint32_t> > matrix;
        std::ifstream ifile;
        map_vector histo;
        pair_map joint;
};
