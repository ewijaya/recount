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
         Sums[17]  += grp[i][17];
         Sums[18]  += grp[i][18];
         Sums[19]  += grp[i][19];
         Sums[20]  += grp[i][20];
         Sums[21]  += grp[i][21];
         Sums[22]  += grp[i][22];
         Sums[23]  += grp[i][23];
         Sums[24]  += grp[i][24];
         Sums[25]  += grp[i][25];
         Sums[26]  += grp[i][26];
         Sums[27]  += grp[i][27];
         Sums[28]  += grp[i][28];
         Sums[29]  += grp[i][29];
         Sums[30]  += grp[i][30];
         Sums[31]  += grp[i][31];
         Sums[32]  += grp[i][32];
         Sums[33]  += grp[i][33];
         Sums[34]  += grp[i][34];
         Sums[35]  += grp[i][35];
         Sums[36]  += grp[i][36];
         Sums[37]  += grp[i][37];
         Sums[38]  += grp[i][38];
         Sums[39]  += grp[i][39];
         Sums[40]  += grp[i][40];
         Sums[41]  += grp[i][41];
         Sums[42]  += grp[i][42];
         Sums[43]  += grp[i][43];
         Sums[44]  += grp[i][44];
         Sums[45]  += grp[i][45];
         Sums[46]  += grp[i][46];
         Sums[47]  += grp[i][47];
         Sums[48]  += grp[i][48];
         Sums[49]  += grp[i][49];
         Sums[50]  += grp[i][50];
         Sums[51]  += grp[i][51];
         Sums[52]  += grp[i][52];
         Sums[53]  += grp[i][53];
         Sums[54]  += grp[i][54];
         Sums[55]  += grp[i][55];
         Sums[56]  += grp[i][56];
         Sums[57]  += grp[i][57];
         Sums[58]  += grp[i][58];
         Sums[59]  += grp[i][59];
         Sums[60]  += grp[i][60];
         Sums[61]  += grp[i][61];
         Sums[62]  += grp[i][62];
         Sums[63]  += grp[i][63];
         Sums[64]  += grp[i][64];
         Sums[65]  += grp[i][65];
         Sums[66]  += grp[i][66];
         Sums[67]  += grp[i][67];
         Sums[68]  += grp[i][68];
         Sums[69]  += grp[i][69];
         Sums[70]  += grp[i][70];
         Sums[71]  += grp[i][71];
         Sums[72]  += grp[i][72];
         Sums[73]  += grp[i][73];
         Sums[74]  += grp[i][74];
         Sums[75]  += grp[i][75];
         Sums[76]  += grp[i][76];
         Sums[77]  += grp[i][77];
         Sums[78]  += grp[i][78];
         Sums[79]  += grp[i][79];
         Sums[80]  += grp[i][80];
         Sums[81]  += grp[i][81];
         Sums[82]  += grp[i][82];
         Sums[83]  += grp[i][83];
         Sums[84]  += grp[i][84];
         Sums[85]  += grp[i][85];
         Sums[86]  += grp[i][86];
         Sums[87]  += grp[i][87];
         Sums[88]  += grp[i][88];
         Sums[89]  += grp[i][89];
         Sums[90]  += grp[i][90];
         Sums[91]  += grp[i][91];
         Sums[92]  += grp[i][92];
         Sums[93]  += grp[i][93];
         Sums[94]  += grp[i][94];
         Sums[95]  += grp[i][95];
         Sums[96]  += grp[i][96];
         Sums[97]  += grp[i][97];
         Sums[98]  += grp[i][98];
         Sums[99]  += grp[i][99];
         Sums[100] += grp[i][100];
         Sums[101] += grp[i][101];
         Sums[102] += grp[i][102];
         Sums[103] += grp[i][103];
         Sums[104] += grp[i][104];
         Sums[105] += grp[i][105];
         Sums[106] += grp[i][106];
         Sums[107] += grp[i][107];
         
    }


    for (unsigned j=0; j <grp[0].size(); j++) {
        Sums[j] /= grp.size();

        printf("%.3f", Sums[j]); 
        cout << " ";
        if ( ((j+1)%4) == 0) {
            cout << "\t\t";
        }
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

            if (TAG.length() != 27) {
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



