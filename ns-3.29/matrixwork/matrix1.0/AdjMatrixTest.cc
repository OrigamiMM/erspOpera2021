#include <fstream>
#include <iostream>
#include <string>
#include "AdjMatrix.h"
using namespace std;

int main(int argc, char** argv){
    AdjMatrix* toTest = new AdjMatrix("example_matchings.csv");
    cout << "Printing matchings now..." << endl; 
    toTest->printMatchings();
    
    cout << "Printing status of 0,7 connection" << endl;
    cout << toTest->existing_connection(0,7) << endl;
    cout << "Printing status of 0,5 connection" << endl;
    cout << toTest->existing_connection(0,5) << endl;
    cout << "Printing status of 4,11 connection" << endl;
    cout << toTest->existing_connection(4,11) << endl;
    cout << "Printing status of 4,15 connection" << endl;
    cout << toTest->existing_connection(4,15) << endl;

    cout << "Done!" << endl;
    return 0;
}