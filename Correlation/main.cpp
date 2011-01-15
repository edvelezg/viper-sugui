#include "BinFile.h"
#include <iostream>

using namespace std;

int main (int argc, char const *argv[])
{
    BinFile bf("out.dat");    
    bf.readfile();
    cout << bf.card_at(0) << endl;
    cout << bf.card_at(1) << endl;
    cout << bf.joint_card() << endl;
    cout << bf.calc_dan_corr() << endl;

    return 0;
}
