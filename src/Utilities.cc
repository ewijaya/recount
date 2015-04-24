#include "Utilities.hh"
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
using std::string;
using std::vector;
using std::cout;

std::string GetBaseNameFromFilename(const std::string &fileName) {

    string fbaseNameWithExt = fileName.substr((fileName.find_last_of("/\\")) + 1);
    int    fperiodIn = fbaseNameWithExt.find(".");
    string fbaseName = fbaseNameWithExt.substr(0,fperiodIn);

    return fbaseName;
}



std::string GetPathNameFromFilename( const std::string &filename )
{
    string foundPath = filename.substr( 0, filename.rfind("/")+1 ); 

    if (foundPath == "") { 
        foundPath = "./";
    }
    return foundPath; 
}


/*template <typename T> void  prn_vec(std::vector < T >&arg)
{
	for (int n = 0; n < arg.size(); n++) {
		cout << arg[n] << " ";
	}
}
*/

vector <char> tagnum2acgt(vector<int>arg ) {

    vector<char> stringACGT;

    map<int,char> lookup;
        lookup[0] = 'A';
        lookup[1] = 'C';
        lookup[2] = 'G';
        lookup[3] = 'T';

    for ( unsigned i=0 ;i<arg.size() ; i++ ) {
        char alphACGT = lookup[arg[i]];
        stringACGT.push_back(alphACGT);
    }


    return stringACGT;

}

vector <int> acgt2tagnum(string arg ) {

    vector<int> numAll;

    map<int,char> lookup;
        lookup['A'] = 0 ;
        lookup['C'] = 1;
        lookup['G'] = 2;
        lookup['T'] = 3;

    for ( unsigned i=0 ;i<arg.size() ; i++ ) {
        int numTag = lookup[arg[i]];
        numAll.push_back(numTag);
    }


    return numAll;

}

vector <int> id2tagnum(int id, int tgl ) {

    // replicate
    vector <int> Rep;
    Rep.assign(tgl, id-1);
    vector <int> nuMTag;

    for ( int i=tgl-1; i>=0  ; i-- ) {
        double pw = pow(4.00, double (i));
        int fm  = static_cast<int>(double(Rep[i])/pw);
        int md  = fm % 4;
        nuMTag.push_back(md);
    }

    return nuMTag;

}

/*double get_prop( map<string,double>&m, string queryNumTag) {

    // test how to pass map in function

    map<string,double>::iterator iter;
    for ( iter = m.begin(); iter != m.end(); ++iter) {
        cout << "Key " << iter->first << ", Value = " << iter->second << endl;
    }
    return 0;
}
*/


