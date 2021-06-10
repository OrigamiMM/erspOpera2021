/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "adjmatrixmodule.h"

namespace ns3 {

/* ... */
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
}



//Takes in a line of csv file and a size
//calls the splitString methhod
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

// 0 2, 1 3
//   0(2)     1(3)
//0   1       0
//1   0       1

void AdjMatrices::printMatchings(){
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

}

