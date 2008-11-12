#include "BitmapWriter.h"


//template <class DB>
void HistogramLoader::loadHistogram(const string filename, histogramtype & mHistograms,const  string delimiter, const char commentmarker, const bool verbose) {
	struct stat csv_stat;
	if(stat(filename.c_str(),&csv_stat) != 0) {
		cerr <<" File "<< filename<< " does not exist... can't index it!"<<endl;
		throw cantopencsv_exception("could not open");
	}
	struct stat my_stat;
	if (stat(getHistoFilename(filename).c_str(), &my_stat) == 0) {
          // OFK changed from st_mtimespec, which did not exist on
          // his system and did not match what "man 2 stat" said should be done
#ifdef __APPLE__
		if(my_stat.st_mtimespec.tv_nsec >= csv_stat.st_mtimespec.tv_nsec) {
#else
		if(my_stat.st_mtim.tv_nsec >= csv_stat.st_mtim.tv_nsec) {
#endif
			ifstream histoin (getHistoFilename(filename).c_str());
			if(histoin) {
				try {
					if(verbose) cout << " loading precomputed histogram... " <<endl;
					loadVectorMapFromTabSeparatedText(mHistograms,histoin);
					if(verbose) cout << " loading precomputed histogram... ok" <<endl;
				} catch(nottabseparatedtext_exception & e) {
					cerr << " I have a problem with histo. file " << getHistoFilename(filename)<<endl;
					mHistograms.clear();
				}
			} else {
				cerr << " can't open histo.file " <<getHistoFilename(filename)<< endl;
			}
		} else {
			cerr <<" stale histo. file : "<<getHistoFilename(filename)<< endl; 
		}
	  
	} 
	if(mHistograms.size() == 0) {
		ifstream in(filename.c_str());
		if (!in) {
			cerr << " could not open CSV file:"<< filename <<endl;
			throw cantopencsv_exception("could not open");
		}
		if(verbose) cout << "Doing a first pass through the data to compute histogram..."<<endl;
		CSVParser csvhisto(in, delimiter, commentmarker);
		csvhisto.readHistogram(mHistograms);
		in.close();
		if(verbose) cout << "histogram read"<<endl;
		if(verbose) cout << " saving histo file..."<<endl;
		ofstream histoout (getHistoFilename(filename).c_str());
		if(histoout) {
			saveVectorMapAsTabSeparatedText(mHistograms,histoout);
			histoout.close();
			if(verbose) cout << " saving histo file... ok"<<endl;
		} else {
			cerr <<" failed to save histo. to "<< getHistoFilename(filename)<<endl;
		}
	}
}
	

	//template <class DB>
	string HistogramLoader::getHistoFilename(const string csvfilename) {
		stringstream ss;
		ss<<csvfilename;
		ss<<".histo";
		string histoname;
		ss>>histoname;
		return histoname;
	}
