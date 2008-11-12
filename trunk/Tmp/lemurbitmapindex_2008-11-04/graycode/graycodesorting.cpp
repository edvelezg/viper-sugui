
#include "graycodesorting.h"


//returns the size of an existing file in bytes
// WARNING: 0 if there is an error or if file doesn't exist
uint64 FileUtil::getFileSize(const char* fileName)
{
  //if(fileExists	(fileName)) {
    struct stat my_stat;
    //assumes file exists (probably not the best strategy)// now ok?
    if(stat(fileName, &my_stat)!=0) return 0;
    if ((my_stat.st_size) > 0)
      return my_stat.st_size;
    else return 0;
  //} else {
    //      return 0 ;// that's evil, but user probably doesn't use this method to check whether the file exists or not 
  //}
}
// create all paths, but assume the path is a file
int FileUtil::mkdir_parentsonly(const char *path) {
        //cout<<" called with "<<path<<endl;
        char * shorter_path = strdup(path);
        char * p = strrchr(shorter_path, '/');// find the last occurance of '/'
        if(p == 0 || p == path) { 
                  free(shorter_path); 
                  return 0;// nothing to do
        }
        p[0] = 0;
        //cout<<" making dir "<<shorter_path<<endl;
        int errnumero = mkdir(shorter_path,0777);
        if ( errnumero < 0 && errno != EEXIST) {
          if ((errno > 0 ) && (errno != ENOENT))  {// the path contains a file, no hope of solving this
                        cerr << "The path "<< path <<" could not be created, errno "<< errno<< ". No hope of creating it. " << endl;
                        free(shorter_path);
                        return -1;
          }
          int returnvalue = mkdir_parentsonly(shorter_path);
          if(returnvalue == 0)
                  returnvalue = mkdir(shorter_path,0777);
          free(shorter_path);
          return returnvalue;
        } else {
                  free(shorter_path);
                  return 0;//success!
        }
}

GrayCodeCSVSorter::GrayCodeCSVSorter(string FileName, sortingtype t, bool reverseorder, string CSVFileName,  string delimiter, char  commentmarker,  	KofNBitmapBudget& bb , BitmapAllocation & ba) 
: mCSVFileName(CSVFileName), mFileName(FileName), mVilla(NULL),  numberofbitmaps(0), type(t),mReverseorder(reverseorder) {
	ifstream in(mCSVFileName.c_str());
	if(!in) {
		cerr <<" I failed to open the CSV file :"<<mCSVFileName<<endl;
		return;
	}
	cout <<"loading histogram for file "<<mCSVFileName<< endl;
	histogramtype mHistograms;
	HistogramLoader::loadHistogram(mCSVFileName, mHistograms, delimiter, commentmarker, false);
	cout <<"constructing bitmap allocation table "<< endl;
	vector<stringuintmultimap > tobitmap;
	ba.computeBitmapAllocation(bb,mHistograms,tobitmap);
	mHistograms.clear();
	numberofbitmaps = BitmapAllocation::numberOfBitmaps(tobitmap);
	cout <<"There shall be "<<numberofbitmaps << " bitmaps"<<endl;
	open();
	CSVParser parser(in, delimiter , commentmarker);
	uint numberofrows = 0;
	vector<uint>  pos;
	while(parser.hasNext()) {
		++ numberofrows;
		//if(numberofrows % 1000 == 0) cout << "processed "<<numberofrows<< " rows"<<endl;
		const vector<string> & current = parser.next();
		pos.clear();
		toBitmapPositionsFromValues(current, tobitmap, pos);
		//BigUnsigned  bu;
		//toGrayCodeRankFromLocationOfOnes(&pos[0], pos.size(), bu);
		/*if(false) {
			toGrayCode(bu);
			BigUnsigned bu2;
			toBigUnsignedFromValues(current, tobitmap,bu2);
			if(bu2 != bu) {
				cout << bu << " "<< bu2<<endl;
				break;
			}
		}*/
		//cout << bu <<endl;
		//if(sparsemode) {
			if(! add( reinterpret_cast<const char *>(&pos[0]), pos.size()*sizeof(uint),  parser.line.c_str(), parser.line.size())) {
				cerr << "My b-tree broke at fact number "<< numberofrows<<endl;
				cerr << "The index file has now size "<< FileUtil::getFileSize(FileName.c_str())<<endl;
				cerr << "Syncing... "<< endl;
				sync();// not sure this is needed
				cerr << "The index file has now size "<< FileUtil::getFileSize(FileName.c_str())<<endl;
				cerr << "aborting"<<endl;
				break;				
			}
		//} 
			/*else {
		 if(! add( reinterpret_cast<const char *>(bu.blk), bu.len*sizeof(ulong), parser.line.c_str(), parser.line.size())) {
			cerr << "My b-tree broke at fact number "<< numberofrows<<endl;
			cerr << "The index file has now size "<< FileUtil::getFileSize(FileName.c_str())<<endl;
			cerr << "Syncing... "<< endl;
			sync();// not sure this is needed
			cerr << "The index file has now size "<< FileUtil::getFileSize(FileName.c_str())<<endl;
			cerr << "aborting"<<endl;
			break;
		 }
		}*/
	}
	sync();// not sure this is needed
	cout <<" number of rows "<< numberofrows<<endl;
	cout <<" number of records  "<< getNumberOfRecords()<<endl;
}

void GrayCodeCSVSorter::open() throw(ifstream::failure) {
  close();
  FileUtil::mkdir_parentsonly(mFileName.c_str());//
  if( ! (mVilla = vlopen(mFileName.c_str(), VL_OWRITER | VL_OCREAT|VL_OZCOMP, 
		     type == lexi ? (mReverseorder? vlsparselexicompare_reverse : vlsparselexicompare) : 
		    	 (mReverseorder?vlsparsegraycompare_reverse : vlsparsegraycompare) ))) {//VL_OCREAT//|VL_OZCOMP
    cerr  << __FILE__ << ":"<< __LINE__ << "I failed to open " << mFileName << endl;
    cerr << " The file might be corrupted: not a QDBM format. "<< endl;
    cerr << " You may have to delete it. "<< endl;
    stringstream strs;
    strs << " Failed to open file "<< mFileName<< ", error might be QDBM related. I'm throwing an ifstream::failure!";
    strs << dperrmsg(dpecode);
    //string errormsg;
    //strs >>  errormsg ; 
    throw ifstream::failure(strs.str());
  }
  //  vlsettuning(mVilla,14096,0,0,0);// vlsettuning seems to make no difference
}



void GrayCodeCSVSorter::sync() {
        if (! vlsync(mVilla)) {
       cerr  << __FILE__ << ":"<< __LINE__ << " I didn't manage to sync the data with "<< mFileName <<": " <<dperrmsg(dpecode)<< endl;  
       }
}

void GrayCodeCSVSorter::close() {
	            if(mVilla) {
	               if(!vlclose(mVilla)){
	                 cerr << __FILE__ << ":"<< __LINE__ <<" I failed to close " << mFileName << endl;       
	                 cerr << dperrmsg(dpecode)<<endl;
	                 // here, the user cannot do much, so I'm not throwing an exception. Hope for the best!
	               }
	            }
	            mVilla = NULL;
}


void GrayCodeCSVSorter::dumpSortedRows(string file) {
	if(mVilla == NULL) return;
	ofstream out(file.c_str());
	if(!out) {
		cerr << " I didn't manage to create the output file "<<file<<endl;
		return;
	}
	if(!vlcurfirst(mVilla)) {
		cerr << " No record found in the database"<<endl;
		return;
	}
	do {
		int size;
		char* val = vlcurval(mVilla, &size);
		out << val<<endl;
		free(val);
		//BigUnsigned  bu (easyDataToBI<ulong>(data1, size/sizeof(uword), BigInteger::positive));
		//toGrayCode(bu);
	} while(vlcurnext(mVilla));
	out.close();
}

bool unittesting() {
	//bool ok(true);
	vector<int> failures;
	// first, we check that 111 is larger than 110
	{
		vector<uint> a(3);
		a[0]=0;a[1]=1;a[2]=1;
		vector<uint> b(2);
		b[0]=0;b[1]=1;
		if(vlsparsegraycompare(reinterpret_cast<const char *>(&a[0]),a.size()*sizeof(uint),reinterpret_cast<const char *>(&b[0]),b.size()*sizeof(uint) )!=-1) 
			failures.push_back(1);//ok = false;
	}
	// we check that 100 is larger than 000
	{
		vector<uint> a(1);
		a[0]=0;
		vector<uint> b(0);
		if(vlsparsegraycompare(reinterpret_cast<const char *>(&a[0]),a.size()*sizeof(uint),reinterpret_cast<const char *>(&b[0]),b.size()*sizeof(uint) )!=-1)
			failures.push_back(2);//ok = false;
	}
	// we check that 010 is larger than 000
	{
		vector<uint> a(1);
		a[0]=1;
		vector<uint> b(0);
		if(vlsparsegraycompare(reinterpret_cast<const char *>(&a[0]),a.size()*sizeof(uint),reinterpret_cast<const char *>(&b[0]),b.size()*sizeof(uint) )!=-1)
			failures.push_back(3);//ok = false;
	}
	// we check that 1010 is smaller than 1011
	{
		vector<uint> a(2);
		a[0]=0;a[1]=2;
		vector<uint> b(3);
		b[0]=0;b[1]=2;b[3]=3;
		if(vlsparsegraycompare(reinterpret_cast<const char *>(&a[0]),a.size()*sizeof(uint),reinterpret_cast<const char *>(&b[0]),b.size()*sizeof(uint) )!=1)
			failures.push_back(4);//ok = false;
	}
	// we check that 1111 is smaller than 1110
	{
		vector<uint> a(4);
		a[0]=0;a[1]=1;a[2]=2;a[3]=3;
		vector<uint> b(3);
		b[0]=0;b[1]=1;b[2]=2;
		if(vlsparsegraycompare(reinterpret_cast<const char *>(&a[0]),a.size()*sizeof(uint),reinterpret_cast<const char *>(&b[0]),b.size()*sizeof(uint) )!=1)
			failures.push_back(5);//ok = false;
	}
	// we check that 1100 is smaller than 1101
	{
		vector<uint> a(2);
		a[0]=0;a[1]=1;
		vector<uint> b(3);
		b[0]=0;b[1]=1;b[2]=3;
		if(vlsparsegraycompare(reinterpret_cast<const char *>(&a[0]),a.size()*sizeof(uint),reinterpret_cast<const char *>(&b[0]),b.size()*sizeof(uint) )!=1)
			failures.push_back(6);//ok = false;
	}
	// we check that 0101 is smaller than 0100
	{
		vector<uint> a(2);
		a[0]=1;a[1]=3;
		vector<uint> b(1);
		b[0]=1;
		if(vlsparsegraycompare(reinterpret_cast<const char *>(&a[0]),a.size()*sizeof(uint),reinterpret_cast<const char *>(&b[0]),b.size()*sizeof(uint) )!=1)
			failures.push_back(7);//ok = false;
	}
	// we check that 0110 is smaller than 0111
	{
		vector<uint> a(2);
		a[0]=1;a[1]=2;
		vector<uint> b(3);
		b[0]=1;b[1]=2;b[2]=3;
		if(vlsparsegraycompare(reinterpret_cast<const char *>(&a[0]),a.size()*sizeof(uint),reinterpret_cast<const char *>(&b[0]),b.size()*sizeof(uint) )!=1)
			failures.push_back(8);//ok = false;
	}
	for(uint k = 0; k <failures.size();++k)
		cout<<"failed test "<<failures[k]<<endl;
	return (failures.size()==0);
}

int main(int argc, char ** argv) {
	std::ios::sync_with_stdio (false);
	if(!unittesting()) {
		cerr <<"failed unit testing. Aborting."<<endl;
		return -1;
	}
	int k = 1;
	int c;
	bool forcek(false);
	bool reverseorder(false);
	GrayCodeCSVSorter::sortingtype t(GrayCodeCSVSorter::gray);
	while ((c = getopt (argc, argv, "K:k:lr")) != -1) {
      switch (c)
        {
        case 'k':
          k = strtol(optarg,NULL,10);
          break;
        case 'r':
          reverseorder = true;
          break;
        case 'K':
          k = strtol(optarg,NULL,10);
          forcek = true;
          break;
        case 'l':
          t = GrayCodeCSVSorter::lexi;
          cout <<"using lexicographical sort"<<endl;
          break;
        default:
          abort ();
        }
    }
	cout << "k = "<<k<<endl;
    if(argc - optind < 2) {
		cout << "I need the name of an incoming CSV file and an incoming"<<endl;
		cout << "You may also specify the value of k"<<endl;
		cout << "To fall back on lexicographical sort of the (uncompress) bitmap index, use the -l switch"<<endl;
		cout << "Example: ./graycodesorting -k 2 myfile.csv outfile.csv"<<endl;
		cout << "You may also use the -K flag, see the indexcsv utility."<<endl;
		cout << "You can sort in reverse order with the -r flag."<<endl;
		return -1;
    }
    string csvfile = argv[optind];
    string outcsvfile = argv[optind+1];
    cout <<"reading file "<<csvfile<< " writing to file "<< outcsvfile<<endl;
	string indexfile = "graycodesorting.bin";
	cout <<" I will create a b-tree in file : "<<indexfile<<endl;
	cout <<" This file may grow very large even for small data files! "<<endl;
	::unlink(indexfile.c_str());
	KofNBitmapBudget g1ofNBitmapBudget(k, !forcek);
	GrayCodeCSVSorter gc(indexfile, t, reverseorder, csvfile,",",'#',g1ofNBitmapBudget);
	cout <<"B-tree constructed, dumping the rows... "<<endl;
	gc.dumpSortedRows(outcsvfile);
	cout <<"Size of the b-tree file : "<<gc.getSizeOfBTreeOnDisk()<<endl;
	cout <<"Size of the CSV file :"<<FileUtil::getFileSize(csvfile.c_str())<<endl;
	cout <<"Size of the output CSV file : "<<FileUtil::getFileSize(outcsvfile.c_str())<<endl;
	gc.close();
	cout <<"Done without crashing. I am now deleting the b-tree file"<<endl;
	::unlink(indexfile.c_str());
}
