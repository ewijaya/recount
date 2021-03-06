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
#include <string>
#include <cstdlib>
using namespace std;



template <typename t> void  prn_vec(std::vector < t >&arg, string sep="")
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
         //prn_vec<double>(grp[i],"\t");
         //cout << endl;
         //cout << "GR : " <<  grp[i][0] << endl;
         Sums[0]   += grp[i][0];
         Sums[1]   += grp[i][1];
         Sums[2]   += grp[i][2];
         Sums[3]   += grp[i][3];
         Sums[4]   += grp[i][4];
         Sums[5]   += grp[i][5];
         Sums[6]   += grp[i][6];
         Sums[7]   += grp[i][7];
         Sums[8]   += grp[i][8];
         Sums[9]   += grp[i][9];
         Sums[10]  += grp[i][10];
         Sums[11]  += grp[i][11];
         Sums[12]  += grp[i][12];
         Sums[13]  += grp[i][13];
         Sums[14]  += grp[i][14];
         Sums[15]  += grp[i][15];
         Sums[16]  += grp[i][16];
         
    }


    for (unsigned j=0; j <grp[0].size(); j++) {
            Sums[j] /= grp.size();
            printf("%.3f", Sums[j]); 
        }
   
   

    cout << endl;

}


int main  ( int arg_count, char *arg_vec[] ) {
    if (arg_count !=2 ) {
        cerr << "Expected one argument" << endl;
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


            if((int)TAG.find("N") != static_cast<int>(TAG.npos)) {
                 continue;
            }

            //cout << TAG << endl;

            if (TAG.length() != 17) {
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



