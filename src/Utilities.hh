 //=====================================================================================
 // Filename:  Utilities.hh
 // =====================================================================================
#ifndef UTILITIES_HH
#define UTILITIES_HH

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cstdlib>
using namespace std;
using std::cout;
using std::string;

std::string GetBaseNameFromFilename(const std::string &fileName);
std::string GetPathNameFromFilename( const std::string &filename);
std::vector<int> id2tagnum(int id, int tgl );
std::vector <int> acgt2tagnum(string arg );
vector <char> tagnum2acgt(vector<int>arg ); 


template <typename T> void  prn_vec_oneval(const std::vector < T >&arg, string sep="", unsigned lim=0)
{
	for (unsigned n = 0; n < arg.size(); n++) {
        if (n >= lim && n <=(lim+1)) {
			cout << arg[n] << sep; 
        }
	}
    return;
}

template <typename T> void  prn_vec(const std::vector < T >&arg, string sep="")
{
	for (unsigned n = 0; n < arg.size(); n++) {
		cout << arg[n] << sep; 
	}
    return;
}

template <typename T> void  prn_vec_binos(const std::vector < T >&arg,ostream
		&fn,string sep="")
{
	for (unsigned n = 0; n < arg.size(); n++) {
		fn << arg[n] << sep; 
        //fn.write(&arg[n].front, arg[n].size());
	}
    return;
}


template <typename T> void  prn_vec_err(const std::vector < T >&arg, string sep="")
{
	for (int n = 0; n < arg.size(); n++) {
		cerr << arg[n] << sep; 
	}
    return;
}


template <typename T1, typename T2> void  prn_map(std::map < T1,T2 >&arg, string sep="")
{
    for(typename map<T1,T2>::iterator iter = arg.begin();iter!= arg.end();++iter){
        cout << iter->first << "->" << iter->second << sep;
    }
    return;
}

template <typename T> void  prnf_vec(std::vector < T >&arg)
{
	for (int n = 0; n < arg.size(); n++) {

		printf("%.7f\t", arg[n]);
	}

    return;
}

#endif // UTILITIES_HH 
