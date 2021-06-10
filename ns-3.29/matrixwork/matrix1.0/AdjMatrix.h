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
using namespace std;

class AdjMatrix {
    private:
        int nodeCount;
        int** matchingnodes;
        //list object
    public:
        
        /* Initialize a adjMatrix object from a csv file
         * @param matchinglist_csv Input CSV file to be read 
         */

        AdjMatrix(const char* matchinglist_csv);

        //createMatrix()

        /* Accessor Function to check port connections
         * @param source port
         * @param destination port
         */
        bool existing_connection(int source, int destination);

        /* Checks that each input port is connected to at most one output port.
         * Checks that each output port is connected to at most one input port.
         */
        bool error_checking_connections();

        void printMatchings();
};
