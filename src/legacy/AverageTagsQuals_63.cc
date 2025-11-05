// =====================================================================================
// Compute average quality score from common tags.
// Input:
// <tag_count>  <tag>  <qA1 qC1 qG1 qT1>  <qA2 qC2 qG2 qT2> etc
//
// Copyright 2009, Edward Wijaya
// =====================================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;



template <typename T> void  prn_vec(std::vector < T >&arg, string sep="")
{
    for (int n = 0; n < arg.size(); n++) {
        cout << arg[n] << sep; 
    }
}

void processBlock(string &tag, vector <vector <double> > &grp) {

    cout << grp.size() << "\t" << tag << "\t";

    vector <double> Sums;
    Sums.assign(grp[0].size(),0);

    for (unsigned i = 0; i < grp.size(); i++) {
         for (unsigned j = 0; j < grp[i].size(); j++) {
             Sums[j] += grp[i][j];
         }
    }

    for (unsigned j=0; j <grp[0].size(); j++) {
        Sums[j] /= grp.size();

        printf("%.3f", Sums[j]); 
        cout << " ";
    }
   

    cout << endl;

}


int main  ( int arg_count, char *arg_vec[] ) {
    if (arg_count !=2 ) {
        cerr << "expected one argument" << endl;
        return EXIT_FAILURE;
    }

    string line;
    ifstream myfile (arg_vec[1]);
    //ifstream myfile ("test.txt");


    string lastKey;
    vector <vector <double> >  lastErrorRates;

    if (myfile.is_open())
    {
        while (getline(myfile,line) )
        {
            stringstream ss(line);
            string TAG;
            double QUAL;
            vector <double> temp;

            ss >>  TAG; 
            //cout << TAG << endl;

            if((int)TAG.find("N") != static_cast<int>(TAG.npos)) {
                 continue;
            }

            if (TAG.length() != 63) {
                cerr << "Incorrect Tag Length" << endl;
                return EXIT_FAILURE;
            }

            while (ss >> QUAL) {
                //cout << count++ << " " << QUAL << endl;
                temp.push_back(QUAL);
            }


            if (TAG != lastKey) {

                if (lastKey.length() == TAG.length()) {
                   processBlock(lastKey,lastErrorRates);
                }

                lastKey = TAG;
                lastErrorRates.clear();
            }
            
            lastErrorRates.push_back(temp);

            //cout << endl;

        }
        myfile.close();
    }
    else { cout << "Unable to open file"; } 

    processBlock(lastKey, lastErrorRates);


    


    return 0;
}



