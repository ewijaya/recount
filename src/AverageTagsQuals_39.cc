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
         //prn_vec<double>(grp[i],"\t");
         //cout << endl;
         //cout << "GR : " <<  grp[i][0] << endl;
         Sums[0] += grp[i][0];
         Sums[1] += grp[i][1];
         Sums[2] += grp[i][2];
         Sums[3] += grp[i][3];
         Sums[4] += grp[i][4];
         Sums[5] += grp[i][5];
         Sums[6] += grp[i][6];
         Sums[7] += grp[i][7];
         Sums[8] += grp[i][8];
         Sums[9] += grp[i][9];
         Sums[10] += grp[i][10];
         Sums[11] += grp[i][11];
         Sums[12] += grp[i][12];
         Sums[13] += grp[i][13];
         Sums[14] += grp[i][14];
         Sums[15] += grp[i][15];
         Sums[16] += grp[i][16];
         Sums[17] += grp[i][17];
         Sums[18] += grp[i][18];
         Sums[19] += grp[i][19];
         Sums[20] += grp[i][20];
         Sums[21] += grp[i][21];
         Sums[22] += grp[i][22];
         Sums[23] += grp[i][23];
         Sums[24] += grp[i][24];
         Sums[25] += grp[i][25];
         Sums[26] += grp[i][26];
         Sums[27] += grp[i][27];
         Sums[28] += grp[i][28];
         Sums[29] += grp[i][29];
         Sums[30] += grp[i][30];
         Sums[31] += grp[i][31];
         Sums[32] += grp[i][32];
         Sums[33] += grp[i][33];
         Sums[34] += grp[i][34];
         Sums[35] += grp[i][35];
         Sums[36] += grp[i][36];
         Sums[37] += grp[i][37];
         Sums[38] += grp[i][38];
         
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

            if (TAG.length() != 39) {
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



