#ifndef BITMAPEXCEPTIONS_H_
#define BITMAPEXCEPTIONS_H_

#include <iostream>

using namespace std;

class badcompressiontype_exception {
public:
	badcompressiontype_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};



class missingmetadatafile_exception {
public:
	missingmetadatafile_exception ( const string m ): message(m) {   }
        
	missingmetadatafile_exception (): message() {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};
        
class unsupportedwordsize_exception {
public:
	unsupportedwordsize_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class invalidrangequery_exception {
public:
	invalidrangequery_exception ( const string m ): message(m) {   }
        
	invalidrangequery_exception ( ): message() {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class dimensiondoesnotexist_exception {
public:
	dimensiondoesnotexist_exception ( const string m ): message(m) {   }
        
	dimensiondoesnotexist_exception ( ): message() {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class unsupportedoperation_exception {
public:
	unsupportedoperation_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};


class accesstoclosedbtree_exception {
public:
	accesstoclosedbtree_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class unexectedencoding_exception {
public:
	unexectedencoding_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class wordsizemismatch_exception {
public:
	wordsizemismatch_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class problemtoolarge_exception {
public:
	problemtoolarge_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class missingbitmap_exception {
public:
	missingbitmap_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};
class toomanybits_exception {
public:
	toomanybits_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};


class notenoughbitmaps_exception {
public: 
	notenoughbitmaps_exception( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;	
};

class versionmismatch_exception {
public:
	versionmismatch_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class cookiemismatch_exception {
public:
	cookiemismatch_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};


class failedtoreaddatablockheader_exception {
public:
	failedtoreaddatablockheader_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class failedtoreadbitmap_exception {
public:
	failedtoreadbitmap_exception (const  string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};


class missingattributevalue_exception {
public:
	missingattributevalue_exception (const  string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};


class forgottoflushlastrow_exception {
public:
	forgottoflushlastrow_exception (const  string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};


class cantwritebitmapindexheader_exception {
public:
	cantwritebitmapindexheader_exception (const  string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class couldnotwritedatablock_exception {
public:
	couldnotwritedatablock_exception (const  string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class cantopencsv_exception {
public:
	cantopencsv_exception (const  string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};


class cantindexfiletowrite_exception {
public:
	cantindexfiletowrite_exception (const  string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

class nottabseparatedtext_exception {
public:
	nottabseparatedtext_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};


class corruptedhistogram_exception {
public:
	corruptedhistogram_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};


class noneedforlogicaloperation_exception {
public:
	noneedforlogicaloperation_exception ( const string m ): message(m) {   }
   void what () { cerr << message<<endl;}; 
private:
      string message;
};

#endif /*BITMAPEXCEPTIONS_H_*/
