// =====================================================================================
// Pick corresponding base quality from
// <tag_count>  <tag>  <qA1 qC1 qG1 qT1>  <qA2 qC2 qG2 qT2> etc
//
//  into 
//
// <tag_count> [ACGT] qA qC qG qT
//
// Copyright 2009, Edward Wijaya
// =====================================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

template <typename t> void  prn_vec(std::vector < t >&arg, string sep="")
{
    for (unsigned n = 0; n < arg.size(); n++) {
        cout << arg[n] << sep; 
    }
}

int main  ( int arg_count, char *arg_vec[] ) {
    if (arg_count !=2 ) {
        cerr << "expected one argument" << endl;
        return EXIT_FAILURE;
    }

    string line;
    ifstream myfile (arg_vec[1]);


    if (myfile.is_open())
    {
        while (getline(myfile,line) )
        {
            stringstream ss(line);
            string COUNT;
            double QUAL;
            string TAG;
            vector <double> QualVec;


            ss >> COUNT >> TAG; // read first column

            if ((int)TAG.find("N") != static_cast<int>(TAG.npos)) {
                 continue;
            }

            cout << COUNT << "\t" << TAG << "\t";
            while (ss >> QUAL) {
                QualVec.push_back(QUAL);

            }


            //prn_vec<double>(QualVec,",");
            //cout << endl;
         

            double nofGroup = QualVec.size()/4;

            if (nofGroup != TAG.length()){
                cerr << "Unequal Group Size: " << nofGroup << ", with Tag Length: " << TAG.length() << endl;
                return EXIT_FAILURE;
            }

            vector <double> pickedVal;
             pickedVal.assign(TAG.length(),0);

            //cout << "NOFGROUP " << nofGroup << endl;

            for (unsigned p=0; p<TAG.length(); p++) {

                char Base = static_cast<char>(TAG[p]);
                double BaseQual = 0.00;

                //cout << Base << " ";

                int b=0;
                for (unsigned i=0; i < nofGroup; i++) {

                    switch (Base)
                    {
                        case 'A': 
                                BaseQual = QualVec[b];
                                break;
                        case 'C': 
                                BaseQual = QualVec[b+1];
                                break;
                        case 'G': 
                                BaseQual = QualVec[b+2];
                                break;
                        case 'T': 
                                BaseQual = QualVec[b+3];
                                break;

                    }
                    
                     
                    b += 4;
                        //cout << p << "-" << i << " " ;
                        //cout <<  BaseQual << ",";
                        //cout << endl;
                    if( p == i) {
                       pickedVal[p] = BaseQual;
                    }
                }




            }

                prn_vec<double>(pickedVal," ");
                cout << endl;


        }
        myfile.close();
    }

    else cout << "Unable to open file\n"; 
    return 0;
}



