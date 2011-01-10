#include "BinFile.h"

//Constructor for the C++ tutorial
BinFile::BinFile()
{
    x = 0;
    y = 0;
}

BinFile::~BinFile()
{
}

int BinFile::Add()
{
    return x+y;
}

void BinFile::setX(int x)
{
    this->x = x;
}

void BinFile::setY(int y)
{
    this->y = y;
}
