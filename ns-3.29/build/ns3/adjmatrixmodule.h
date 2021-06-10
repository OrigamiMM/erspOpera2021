/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ADJMATRIXMODULE_H
#define ADJMATRIXMODULE_H

#include <vector>
#include <unordered_set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

namespace ns3 {

    class AdjMatrices {
        private:
            int portCount;
            int size;
            vector<vector<vector<int>>> matchesArray;
            void createMatrix(string line);
        public:
            
            /* Initialize a ‘AdjMatrices’ object from a csv file
            * @param matchinglist_csv Input CSV file to be read 
            */

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

            void printMatchings();
    };
}

#endif /* ADJMATRIXMODULE_H */

