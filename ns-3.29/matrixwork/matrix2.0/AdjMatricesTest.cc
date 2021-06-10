#include <fstream>
#include <iostream>
#include <string>
#include "AdjMatrices.h"
using namespace std;

// M1: 0 7,1 8,2 9,3 10,4 11,5 6
// M2: 0 8,1 9,2 10,3 11,4 6,5 7

//There are a total of 12 nodes, but saved as a 6x6 matrix: MATCHING 1
//    0(6)    1(7)    2(8)    3(9)   3(10)   3(11)
// 0   0       1       0       0       0       0
// 1   0       0       1       0       0       0
// 2   0       0       0       1       0       0
// 3   0       0       0       0       1       0
// 4   0       0       0       0       0       1
// 5   1       0       0       0       0       0

int main(int argc, char** argv){
  //0 7,1 8,2 9,3 10,4 11,5 7
    /*vector<vector<vector<int>>> mainMatchings;
    vector<vector<int>> matrix1 = {
      {1,0,0,0,0,0},
      {0,1,0,0,0,0},
      {0,0,1,0,0,0},
      {0,0,0,1,0,0},
      {0,0,0,0,1,0},
      {1,0,0,0,0,0}
    };

    0 7,1 8,2 9,3 10,4 11,4 6
      {1,0,0,0,0,0},
      {0,1,0,0,0,0},
      {0,0,1,0,0,0},
      {0,0,0,1,0,0},
      {1,0,0,0,1,0},
      {0,0,0,0,0,0}

    mainMatchings.push_back(matrix1); */

    AdjMatrices* toTest = new AdjMatrices(argv[1]);

    cout << "Performing injective test" << endl;
    toTest->injectiveCheck();

    cout << "Printing Matchings" << endl;
    toTest->printMatchings(); 
    
    cout << "Done!" << endl;
    delete toTest;
    return 0;
}