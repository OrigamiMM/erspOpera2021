/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "adjmatrix.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"

namespace ns3 {

    /**
    Splits a string using a defined character or characters.
    @param line the string input
    @param delimiter the chosen string to use as split point 
    @return a vector of split strings
    */
    vector<string> splitString (string line, string delimiter){
        vector<string> toReturn;
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

    //calls the splitString method and gets node count from the pairs. 
    //In the csv, matchings are(should) be one to one, so we have 
    /**
    From line with matrix data, gets the estimated size of the resulting matrix
    @param line the matrix pairings
    */
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

    AdjMatrices::AdjMatrices(){
        printf("Default Constructor\n");
    }

    /**
    Creates a matrix from matrix data in string. Saves it in the data structure as a variable
    @param matrix string data
    */
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

        printf("Created Matrix\n");
        matchesArray.push_back(matrix);
    }

    /**
    Checks if two ports are connected at given time
    @param input matrix matching
    @param source port
    @param destination port
    @return true if connection exist 
    */
    bool AdjMatrices::existing_connection(vector<vector<int>> adjMat, int source, int destination){
        if(source < 0 || source >= size || destination < size || destination >= 2*size){
            cout << "Error Out of Bounds" << endl;
            return 0;
        }
        return adjMat[source][destination - size] == 1;
    }

     /**
    Checks if matix is valid/ is one-to-one
    @return true if graph is one-to-one 
    */
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


     /**
    Prints the expected index at the current simulator time when called
    */
    void AdjMatrices::next_matching_index(){
        currentMatchingIndex = (Simulator::Now().GetNanoSeconds() / rotationTime) % matchesArray.size();
        
        // debug purposes
        cout << "Current time: " << Simulator::Now().GetNanoSeconds() << endl;
        cout << "Current matching index: " << currentMatchingIndex << endl;

        if(Simulator::Now().GetNanoSeconds() < NanoSeconds(rotationTime * 5)){
            Simulator::Schedule(NanoSeconds(rotationTime), &AdjMatrices::next_matching_index, this);
        }
    }

    /**
    Returns the matching matrix at a given time in nanoseconds
    @param uint64_t time in nanoseconds
    @return vector<vector<int>> of the matrix 
    */
    vector<vector<int>> AdjMatrices::getMatchingIndex(uint64_t nanoseconds){
        int index = (nanoseconds / rotationTime) % matchesArray.size();
        currentMatchingIndex = index;
        return matchesArray[index];
    }

    /**
    Checks if the matrix at given time matches the expected matrix
    @param uint64_t time in nanoseconds
    @return true if correct matrix is set 
    */
    bool AdjMatrices::correctMatchingTimeCheck(int64_t targetTime){
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

    /**
    Prints the matchings for current matrix array
    */
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

    /**
    Prints matchings for all matching matrices
    */
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

    /**
    Returns the rotation time set in the constructor
    @return uint64_t time in nanoseconds
    */
    int AdjMatrices::getRotationTime(){
        return rotationTime;
    }
    
    /**
    Returns the current index
    @return int index of data structure
    */
    int AdjMatrices::getCurrentIndex(){
        return currentMatchingIndex;
    }
    
    vector<vector<vector<int>>> AdjMatrices::getMatchesArray(){
        return matchesArray;
    }
}
