// =====================================================================================
// Finds Neighbors of a Tag within 1 Hamming Distance
// Copyright 2009, Edward Wijaya
// =====================================================================================

#include "Utilities.hh"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include  <map>
#include <cmath>
#include <cstdlib>
using namespace std;

double Solexa2Phred (double sQ) {
     // Convert Solexa into Phred Quality
     double pQ = 10.00 * log(1.00 + pow(10.00,(sQ/10.0)))/ log(10.00);
     return pQ;
}

string ConvertInt2String(int IntVal) {
    std::string S;
    std::stringstream out;
    out << IntVal;
    S = out.str();
     
    return S;
}

vector <int> MismatchPos (std::vector<int>STag, string Nb) {
    vector <int> MMPos;

    for (unsigned i = 0; i < STag.size(); i++) {
         
         string Base     = string(1,Nb[i]);
         string STagBase = ConvertInt2String(STag[i]);

         if (Base != STagBase) {
             //cout << "\t -- " << Base << " " << STag[i] << endl;
             MMPos.push_back(i);
         }
    }
     
    return MMPos;
}

double MultiplyError (std::vector <double> &Qual, std::vector <int> &Pos) {
    double Result = 1;

    for (unsigned i = 0; i < Pos.size(); i++) {
        double err = (pow(10, -(Solexa2Phred(Qual[Pos[i]]) / 10.00))) / 3;
        //cout << "QUAL: " << Qual[i] << " , ERR " << err << endl;
        Result *= err;
    }


    return Result;
}

vector <int> MismatchPosByVec (std::vector<int>STag, vector<int> Nb) {
    vector <int> MMPos;

    for (unsigned i = 0; i < STag.size(); i++) {
         
         int Base     = Nb[i];
         int STagBase = STag[i];

         if (Base != STagBase) {
             //cout << "\t -- " << Base << " " << STag[i] << endl;
             MMPos.push_back(i);
         }
    }
     
    return MMPos;
}

double normalizeQualByMismatches1Tag(std::vector<int>&SeedTag,std::vector<int> NbTags,std::vector <double> &qual) {
     vector <int> mmPos = MismatchPosByVec(SeedTag,NbTags);
     double ErrorProb   = MultiplyError(qual,mmPos);
     return ErrorProb;
} 


vector <int> neighbors(vector<int>& arg, int posNo, int baseNo) {
    // pass base position and return neighbors
    
    vector <int> transfVec;
    transfVec = arg;
    transfVec[posNo] = baseNo;

    return transfVec;

}


string Vec2Str (vector <int> NTg) {

    string StTg = "";
    for (unsigned i = 0; i < NTg.size(); i++) {
         StTg += ConvertInt2String(NTg[i]);
    }
    return StTg;
}



int main  ( int arg_count, char *arg_vec[] ) {
    if (arg_count < 3 ) {
        cerr << "Expected two arguments FileName and Max Hamming Distance" << endl;
        return EXIT_FAILURE;
    }

    string line;

    string filename = arg_vec[1];
    ifstream myfile (filename.c_str());

    // Max Hamming Distance to Generate Neighbours
    int hd = static_cast<int>(atoi(arg_vec[2]));
    double BaseErrProbLim = static_cast<double>(atof(arg_vec[3]));


    //cerr << "HD " <<  hd << endl;

    if ( hd > 2 ) {
        cerr << "Hamming Distance should be <= 2" << endl;
        return EXIT_FAILURE;
    }



    //cerr << BaseErrProbLim << endl;

    string baseName    = GetBaseNameFromFilename(filename);
    string pathName    = GetPathNameFromFilename(filename);

    string nbFileName  = pathName + baseName + ".nb";
    string nbqFileName = pathName + baseName + ".nbq";

    ofstream nbFile;
    nbFile.open(nbFileName.c_str(), ios::out);

    ofstream nbqFile;
    nbqFile.open(nbqFileName.c_str(), ios::out);

    vector<string>  DNAStrings;


    if (myfile.is_open())
    {
        while (getline(myfile,line) )
        {
                if (line.find("#") == 0 ) {
                    continue;
                }

                stringstream ss(line);
                string DNA;
                double qualSc;
                double rawCount;
                vector<double> qualBase;
                
                 // ss >> rawCount >> DNA;
                ss >> rawCount >> DNA;

                if (rawCount == 0.00) {
                    rawCount = rawCount + 0.00001;
                }

                string dnaTag;
                dnaTag = DNA;

                while (ss >> qualSc) {
                    qualBase.push_back(qualSc);
                }

            // we process string line by line here
            // avoiding slurping with push_back

                nbFile <<  DNA << "\t"; 
                nbqFile <<  DNA << "\t"; 
                
            //Convert string to numeric
            map<char, int> lookup;
                lookup['A'] = 0;
                lookup['C'] = 1;
                lookup['G'] = 2;
                lookup['T'] = 3;

                vector<int> numTag;

               for (unsigned j=0; j<DNA.size(); j++) {
                   int cb = lookup[DNA[j]]; // converted base
                   numTag.push_back(cb);
               }

               //prn_vec(numTag);
               prn_vec_binos<int>(numTag,nbFile);
               nbFile << "\t\t";

               prn_vec_binos<int>(numTag,nbqFile);
               nbqFile << "\t\t";

               if (hd == 1) {
              
                    for ( unsigned p=0; p<numTag.size() ; p++ ) {

                        for ( int b=1; b<=3 ; b++ ) {
                            //cerr << " Pos: " << p << ", base= " << b << endl;

                            int bval = b;
                            if (numTag[p] == b) {
                                    bval = 0;
                            }

                            vector <int> nbnumTag = neighbors(numTag, p, bval);
                            double nrmQual        = normalizeQualByMismatches1Tag(numTag,nbnumTag,qualBase);
                            

                            //prn_vec <int>(nbnumTag);
                            prn_vec_binos <int>(nbnumTag,nbFile);
                            nbFile << "\t";
                            nbqFile << nrmQual << "\t";
                            
                        }
                    }

                    nbFile << endl;
                    nbqFile << endl;
               }
               else {

                   int TagLen = static_cast<int>(numTag.size()); 

                   for ( int p=0; p< TagLen  ; p++ ) {

                       // First loop is to generate tags 1 position differ
                       for ( int b=0; b<=3 ; b++ ) {

                           int bval = b;
                           if (numTag[p] == b) {
                               continue;
                           }

                           vector <int> nbnumTag = neighbors(numTag, p, bval);
                           string SnbnumTag      = Vec2Str(nbnumTag);
                           double nrmQual        = normalizeQualByMismatches1Tag(numTag,nbnumTag,qualBase);


                           // We want to keep all 1 mismatch neighbors
                           nbqFile << nrmQual << "\t";
                            

                           //
                           // Second loop for tags in 2 position differ 
                           for (int l=p+1; l < TagLen; l++) {


                               for (int  c=0; c<=3; c++) {

                                   int cval = c;

                                   if (nbnumTag[l] == c) {
                                       continue;
                                   }
                                   vector <int> nbnumTag2 = neighbors(nbnumTag, l, cval);
                                   string SnbnumTag2      = Vec2Str(nbnumTag2);
                                   double nrmQual2        = normalizeQualByMismatches1Tag(numTag,nbnumTag2,qualBase);

                                   if (nrmQual >= BaseErrProbLim) {
                                        nbFile << SnbnumTag2;
                                        nbFile << "\t";
                                        nbqFile << nrmQual2 << "\t";
                                   }
                               } 
                           }

                       }
                   }


                   nbFile << endl;
                   nbqFile << endl;

               }

        }
        myfile.close();
        nbFile.close();
        nbqFile.close();
    }

    else nbFile << "Unable to open file\n"; 
    return 0;
}


