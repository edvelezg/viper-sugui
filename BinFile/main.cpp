#include "BinFile.h"
#include <iostream>

using namespace std;

int main (int argc, char const *argv[])
{
    BinFile bt;
	bt.writefile();
	bt.readfile();
	// bt.setX(1);
	// bt.setY(3);
	//     cout << bt.Add() << endl;
    return 0;
}
