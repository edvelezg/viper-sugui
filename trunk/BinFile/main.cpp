#include "BinFile.h"
#include <iostream>

using namespace std;

int main (int argc, char const *argv[])
{
    BinFile bf("out.bin", "out.bin");
    bf.writefile();
    bf.readfile();

// bt.setX(1);
// bt.setY(3);
//     cout << bt.Add() << endl;
    return 0;
}
