// 0     4 
// 1     5
// 2     6
// 3     7

//M1
//0,5 & 1,6 & 2,7 & 3,4 "\n"
//M2
//0,6 & 1,7 & 2,4 & 3,5 "\n"
#include <vector>
#include <unordered_set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
using namespace std;

class AdjMatrices {
    private:
        int portCount;
        int size;
        const int64_t rotationTime = 3500;
        vector<vector<vector<int>>> matchesArray;
        void createMatrix(string line);
        int currentMatchingIndex;

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

        bool correctMatchingTimeCheck(int64_t targetTime);

        void printAllMatchings();

        void printCurrentMatching();

        void next_matching_index(void);

        int getRotationTime();

};