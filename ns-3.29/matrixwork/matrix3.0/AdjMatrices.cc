//Adjacency Matrix 
//Takes in matchings from CSV, and then creates an adjacency matrix for each matching
//Matching changes at each time
//Matching cycles through like a round robin 
//check that at most one output is connected to at most one input at a specific time.

// 0     6 
// 1     7
// 2     8
// 3     9
// 4     10
// 5     11

//REAL ADJ 

//There are a total of 12 nodes, but saved as a 6x6 matrix, DID THIS ONE
//    0(6)    1(7)    2(8)    3(9)   3(10)   3(11)
// 0   1       0       0       0       0       0
// 1   0       1       0       0       0       0
// 2   0       0       0       1       0       0
// 3   0       0       1       0       0       0
// 4   0       0       0       0       0       0
// 5   0       0       0       0       0       0

#include "AdjMatrices.h"
using namespace std;

//For the string, when it reaches the delimiter, it cuts that substring and adds to a vector
//all substrings between delimiters are part of vector
vector<string> splitString (string line, string delimiter){
    vector<string> toReturn;
    // size_t pos = 0;
    // string token;
    // cout << line << endl;
    // while ((pos = line.find(delimiter)) != string::npos) { 
    //     token = line.substr(0, pos);
    //     cout << "Printing current token: " << token << "! lets go." <<endl;
    //     toReturn.push_back(token);
    //     line.erase(0, pos + delimiter.length());
    // }
    //[1]
    auto start = 0U;
    auto end = line.find(delimiter);
    while (end != string::npos)
    {
        toReturn.push_back(line.substr(start, end - start));
        start = end + delimiter.length();
        end = line.find(delimiter, start);
    }
    toReturn.push_back(line.substr(start, end));

    
    return toReturn;
}
//[1]moswald, stackoverflow, 2021


//calls the splitString method and gets node count from the pairs. 
//In the csv, matchings are(should) be one to one, so we have 
void AdjMatrices::getMatrixSize(string line){
    //{"0 7", "1 8", "2 9", "3 10", "4 11", "5 6"}
    size = splitString(line, ",").size();
}

// constructor
//Takes in a csv file name and goes line by line
//3 Total variables, and int for souce, destination, and size + string for the line
//For each line, get get the size of the matrix by calling getSize. 
//call the createMatrix method that takes in a sting and size parameter
//set the size based on the matchings size array  
AdjMatrices::AdjMatrices(const char* matchinglist_csv){
    
    ifstream ip(matchinglist_csv);

    string matrix;
    while(ip.good()){
        getline(ip, matrix,'\n');
        getMatrixSize(matrix);
        createMatrix(matrix);
    }
    portCount = size * 2;
    currentMatchingIndex = 0;
}



//Takes in a line of csv file and a size
//calls the splitString method
void AdjMatrices::createMatrix(string line){
    vector<vector<int>> matrix (size, vector<int>(size, 0));
    vector<string> matchings;
    vector<string> pairs;

    matchings = splitString(line, ","); // {{"0 3"}, {"1 2"}}

    for(auto string : matchings){
        pairs = splitString(string, " "); //{"0"} {"3"}
        int source = stoi(pairs[0]);
        int dest = (stoi(pairs[1]) - size);
        matrix[source][dest] = 1;
    }

    matchesArray.push_back(matrix);
}

 bool AdjMatrices::existing_connection(vector<vector<int>> adjMat, int source, int destination){
    if(source < 0 || source >= size || destination < size || destination >= 2*size){
         cout << "Error Out of Bounds" << endl;
        return 0;
     }
     return adjMat[source][destination - size] == 1;
}

//Checks if a graph is one-to-one. Checks for typos in csv file
bool AdjMatrices::injectiveCheck(){
    vector<vector<int>> matchingToCheck;
    int sCount = 0;
    int dCount = 0;
    int matching = 0;
    bool failureFlag = false;
    for(auto m : matchesArray){
        matchingToCheck = m;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(matchingToCheck[i][j] == 1) sCount++;
                if(matchingToCheck[j][i] == 1) dCount++;
                if(sCount > 1){
                    cout << "Source port " << i << " has more than 1 destination connections in matching " << matching << endl;
                    failureFlag = true;
                }
                if(dCount > 1){
                    cout << "Destination port " << (i+size) << " has more than 1 source ports in matching " << matching << endl;
                    failureFlag = true;
                }
            }
            sCount = 0;
            dCount = 0;
        }
        matching++;
    }

    if (failureFlag == false){  
        cout << "The matchings passed the inductive test!" << endl;
        return true;
    }
    else{
        cout << "The matchings did not pass the inductive test!" << endl;
        return false;
    }
}



void AdjMatrices::next_matching_index(int time){
    currentMatchingIndex = (time / rotationTime) % matchesArray.size();
}


bool AdjMatrices::correctMatchingTimeCheck(int targetTime){
    //Matching changes every 3500 nanoseconds.
    //Matching 0: 0-3499        0-2.999     9-11.9999
    //Matching 1: 3500-6999     3-5.999     12-
    //Matching 2: 7000-10499    6-8.9999
    //


    //time t = 8 seconds, so which matching should return 2. 
    // 11 seconds it should be matching 0
    int expected_Matching_index = (targetTime/rotationTime) % matchesArray.size();
    //11/3 = 3%3 = 0;
    //12/3 = 4%3 = 1
  
    if(matchesArray[expected_Matching_index] == matchesArray[currentMatchingIndex]){
        cout << "Correct matching at current time" << endl;
        return true;
    } 
    else{
        cout << "Incorrect matching at current time" << endl;
        return false;
    }

    
}

void AdjMatrices::printCurrentMatching(){
    //auto m = matchesArray[currentMatchingIndex];

    cout << "for matching " << currentMatchingIndex << endl;
    
    for(int i = 0; i < size; i++){ 
        for(int j = 0; j < size; j++){
            if(existing_connection(matchesArray[currentMatchingIndex], i, (j+size))){
                cout << i << " " << (j + size) << endl;
            }
        }
    }
}

// 0 2, 1 3
//   0(2)     1(3)
//0   1       0
//1   0       1

void AdjMatrices::printAllMatchings(){
    int matchIndex = 0;
    for(auto m : matchesArray){
        cout << "for matching " << matchIndex << endl;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(existing_connection(m, i, (j+size))){
                    cout << i << " " << (j + size) << endl;
                }
            }
        }
        matchIndex++;
    }
}

int AdjMatrices::getRotationTime(){
    return rotationTime;
}
