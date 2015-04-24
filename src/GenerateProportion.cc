#include "Utilities.hh"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
using namespace std;

std::pair<int,int> number_of_lines( char* myfn ) {

    int lineno = 0;
    int tagLength =0;

    string line;
    ifstream myfilename (myfn);

    if (myfilename.is_open())
    {
        while (getline(myfilename,line) )
        {
          if (line.find("#") == 0 ) {
              continue;
          }
           stringstream ss(line);
           string DNA;
           
           ss >> DNA >> DNA;
           tagLength = DNA.size();

           lineno += 1;
        }
        myfilename.close();
    }


    return std::make_pair(lineno,tagLength); 

}



// End of functions
//



int main  ( int arg_count, char *arg_vec[] ) {

    if (arg_count !=2 ) {
        cerr << "expected one argument" << endl;
        return EXIT_FAILURE;
    }

    pair <int,int> tuple = number_of_lines(arg_vec[1]); 
    //cout << "Number of Tags = " << tuple.first << endl;
    //cout << "Tag lengh = " << tuple.second << endl << endl;

    //int taglen = tuple.second;
    int noftag = tuple.first;


    string line;
    ifstream myfile (arg_vec[1]);
    int idval = 0; 

    if (myfile.is_open())
    {
        while (getline(myfile,line) )
        {
                if (line.find("#") == 0 ) {
                    continue;
                }
                idval += 1;
                stringstream ss(line);
                string dummy;
                double tableEntry;


                ss >> tableEntry >> dummy;
                //cout << "Tagno : " << idval << ", Rawcount: " << tableEntry << "\t" << noftag << endl;
                if (tableEntry == 0.00) {
                    tableEntry = tableEntry + 0.00001;
                }
                
                //vector <int> numTag = id2tagnum(idval,taglen);
                vector <int> numTag = acgt2tagnum(dummy);
                float prop = double(tableEntry)/double(noftag);
               
				prn_vec(numTag);
				cout << "\t";
				printf("%.10f\n", prop);
                //cout << endl;
                

        }
        myfile.close();
    }
    else {
        cout << "Unable to open file\n"; 
    }
   
    return 0;
}
