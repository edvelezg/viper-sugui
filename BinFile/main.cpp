#include "BinFile.h"
#include <iostream>

using namespace std;

int main (int argc, char const *argv[])
{
    BinFile bt;    
    // bt.readfile();
	bt.readfile();
    bt.writefile();

	// bt.setX(1);
	// bt.setY(3);
	//     cout << bt.Add() << endl;
    return 0;
}
