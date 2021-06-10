/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ADJMATRIX_H
#define ADJMATRIX_H

#include <vector>
#include <unordered_set>
#include <string> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
using namespace std;

namespace ns3 {

    class AdjMatrices {
        private:
            int portCount;
            int size;
            int64_t rotationTime = 3500;
            vector<vector<vector<int>>> matchesArray;
            void createMatrix(string line);
            int currentMatchingIndex;

        public:
            
            /* Initialize a ‘AdjMatrices’ object from a csv file
            * @param matchinglist_csv Input CSV file to be read 
            */

            AdjMatrices();
            AdjMatrices(const char* matchinglist_csv);

            void getMatrixSize(string line);
            /* Accessor Function to check port connections
            * @param source port
            * @param destination port
            */
            bool existing_connection(vector<vector<int>> adjMat, int source, int destination);

            /* Checks that each input port is connected to at most one output port.
            * Checks that each output port is connected to at most one input port.
            */
            bool injectiveCheck();

            bool correctMatchingTimeCheck(int64_t targetTime);

            void printAllMatchings();

            void printCurrentMatching();

            void next_matching_index();

            int getRotationTime();

            int getCurrentIndex();

            vector<vector<int>> getMatchingIndex(uint64_t nanoseconds);

            vector<vector<vector<int>>> getMatchesArray();
    };

}

#endif /* ADJMATRIX_H */

