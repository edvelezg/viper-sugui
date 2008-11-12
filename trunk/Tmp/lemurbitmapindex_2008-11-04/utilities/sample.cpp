/**
* This cpp file is meant to create samples from an input file.
* 
* @author: kamel
*/
#include <fstream>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <map>
#include <iostream>
#include <time.h>

using namespace std;
typedef unsigned int uint;

/**
 * Little program to extract, randomly, lines from a data file.
 */


int main(int params, char ** args)  {
	std::ios::sync_with_stdio (false);
	float p(1.0);//default sampling percentage 
	uint n(0);//number of lines of the sample 
	map<int,bool> randomnumber;
	string samplefile ("sample.out");//default output
	int r=4567;  //default rand seed
	

	cout << "This program generates samples from an input data file" << endl;

	if (params <= 3) {
		cerr << "You must provide some parameters " << endl;
		cout << "Usage: ./sample [-n 1000|-p 0.01, -o outputfile, -r seed] inputfile" << endl;
		return -1;
	}

	
	string datafilename (args[params-1]); //Data file name
	cout << "Data file name:  " << datafilename.c_str() << endl;
	
	ifstream filein (datafilename.c_str()); //Opening

	if (!filein) {
		cout << "Can't open the damned file. " <<endl;
		return -10;
	}
	clock_t start,finish;
	start = clock();
	//Reading and checking the program parareters
	for(int i = 1; i < params - 1; ++i) {
		if(strcmp(args[i],"-n") == 0){
			n = strtol(args[++i],NULL,10);
			cout << "Number of lines in the sample you are generating: " << n <<endl;
		}
		if(strcmp(args[i],"-p") == 0){
			p = static_cast<float> (atof(args[++i]));
			cout << "Percentage of sampling: " << p << "%" << endl;
			n=0;
		}
		if(strcmp(args[i],"-o") == 0){
			samplefile = args[++i];
			cout << "Name of the output file: " << samplefile.c_str() << endl;
		}
		if(strcmp(args[i],"-r") == 0){
                        r= strtol(args[++i],NULL,10);
                        cout << "Random seed: " << r <<endl;
		}
	}
	srand(r);
	uint N = count(istreambuf_iterator<char>(filein),istreambuf_iterator<char>(),'\n' );//counting the number of lines
	filein.close();
	 
	cout << "Number of records: "<< N << endl;
	uint ec = n;
	if (n==0) ec = static_cast<uint>((p*N/100));//if -n is not set

	while (randomnumber.size() < ec){
		int randomvalue = 1 + static_cast<int> (static_cast<double> (rand()) / (static_cast<double>(RAND_MAX) + 1) * N);
		randomnumber.insert(make_pair(randomvalue, true));
	}
	ofstream fileout (samplefile.c_str());
	if (!fileout){
		cerr << "Cannot create the output file" << endl;
		return 11;
	}
	filein.open (datafilename.c_str(), ifstream::in);//open again fileib
	string line;
	uint linenumber(0);
	getline (filein,line);//attribute names
	fileout << line << endl;
	while (filein){
		getline (filein,line);
		++linenumber;
		map<int,bool>::iterator it = randomnumber.find(linenumber); 
		if ( it != randomnumber.end())
		{
			//cout << it->first << " "<< line << endl;
			fileout << line<< endl;
			randomnumber.erase(it);
		}
		if (randomnumber.size() == 0) break;
		//{ cout << "going to break at line "<< linenumber << endl; break;}
	}
	filein.close();
	fileout.close();
	finish = clock();
	cout << "Size of the generated sample is: " << ec << endl;
	cout << "Time used to read data: " << static_cast<double> (finish - start) / CLOCKS_PER_SEC << " seconds or " << (finish - start)<< " ticks "<<endl;
	return 0;
}
