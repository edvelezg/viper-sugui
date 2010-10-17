#include <iostream>

using namespace std;

int main (int argc, char *argv[])
{
	string name = "hello world";
	cout << "Hello World" << endl;
	for (int j = 0; j < name.length(); ++j)
	{
		cout << "name.at(" << j << " ): " <<  name.at(j) << endl;
	}
	return 0;
}

