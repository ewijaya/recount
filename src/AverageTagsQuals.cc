// =====================================================================================
// Compute average quality score from common tags.
// Input:
// <tag_count>  <tag>  <qA1 qC1 qG1 qT1>  <qA2 qC2 qG2 qT2> etc
//
// This optimized version replaces 47+ hardcoded length-specific versions
// and accepts tag length as a command-line parameter.
//
// Copyright 2009, Edward Wijaya
// Optimized 2025
// =====================================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
using namespace std;


template <typename T> void prn_vec(std::vector<T>& arg, string sep="")
{
    for (size_t n = 0; n < arg.size(); n++) {
        cout << arg[n] << sep;
    }
}

void processBlock(string& tag, vector<vector<double>>& grp) {
    if (grp.empty() || grp[0].empty()) return;

    cout << grp.size() << "\t" << tag << "\t";

    size_t numCols = grp[0].size();
    vector<double> sums(numCols, 0.0);

    // Optimized: Use loop instead of hardcoded indices
    for (size_t i = 0; i < grp.size(); i++) {
        for (size_t j = 0; j < numCols; j++) {
            sums[j] += grp[i][j];
        }
    }

    // Compute and print averages
    for (size_t j = 0; j < numCols; j++) {
        sums[j] /= grp.size();
        printf("%.3f", sums[j]);
        if (j < numCols - 1) cout << " ";
    }

    cout << endl;
}


int main(int arg_count, char* arg_vec[]) {
    if (arg_count != 3) {
        cerr << "Usage: " << arg_vec[0] << " <input_file> <expected_tag_length>" << endl;
        return EXIT_FAILURE;
    }

    int expectedTagLength = atoi(arg_vec[2]);
    if (expectedTagLength <= 0) {
        cerr << "Error: expected_tag_length must be a positive integer" << endl;
        return EXIT_FAILURE;
    }

    string line;
    ifstream myfile(arg_vec[1]);

    string lastKey;
    vector<vector<double>> lastErrorRates;

    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            stringstream ss(line);
            string TAG;
            double QUAL;
            vector<double> temp;

            ss >> TAG;

            // Skip tags containing 'N'
            if (TAG.find("N") != string::npos) {
                continue;
            }

            // Check tag length
            if ((int)TAG.length() != expectedTagLength) {
                cerr << "Error: Incorrect Tag Length (expected " << expectedTagLength
                     << ", got " << TAG.length() << ")" << endl;
                return EXIT_FAILURE;
            }

            // Read quality scores
            while (ss >> QUAL) {
                temp.push_back(QUAL);
            }

            // Process block when tag changes
            if (TAG != lastKey) {
                if (lastKey.length() == TAG.length()) {
                    processBlock(lastKey, lastErrorRates);
                }
                lastKey = TAG;
                lastErrorRates.clear();
            }

            lastErrorRates.push_back(temp);
        }
        myfile.close();
    } else {
        cerr << "Unable to open file: " << arg_vec[1] << endl;
        return EXIT_FAILURE;
    }

    // Process final block
    processBlock(lastKey, lastErrorRates);

    return 0;
}
