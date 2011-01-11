#include <vector>
using namespace std;

class BinFile
{
    public:
        BinFile ();
        virtual ~BinFile ();
		void readfile();
		int writefile();
		
    private:
		uint32_t numcols;
		int magic;
		int version;
		std::vector <int> *mData;
};
