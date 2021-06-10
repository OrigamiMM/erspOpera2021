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

//A directional graph
//     0   1   2   3   4   5   6   7   
// 0   0   0   0   0   1   0   0   0
// 1   0   0   0   0   0   1   0   0
// 2   0   0   0   0   0   0   0   1
// 3   0   0   0   0   0   0   1   0
// 4   0   0   0   0   0   0   0   0
// 5   0   0   0   0   0   0   0   0
// 6   0   0   0   0   0   0   0   0
// 7   0   0   0   0   0   0   0   0  

//graph with bi-direction
//     0   1   2   3   4   5   6   7
// 0   0   0   0   0   1   0   0   0
// 1   0   0   0   0   0   1   0   0
// 2   0   0   0   0   0   0   0   1
// 3   0   0   0   0   0   0   1   0
// 4   1   0   0   0   0   0   0   0
// 5   0   1   0   0   0   0   0   0
// 6   0   0   0   1   0   0   0   0
// 7   0   0   1   0   0   0   0   0

//REAL ADJ 
#include "AdjMatrix.h"
using namespace std;

// constructor
AdjMatrix::AdjMatrix(const char* matchinglist_csv){
    nodeCount = 0;

    //open file and check the first matching to determine the Node size. 
    ifstream infile(matchinglist_csv, ifstream::in);
    string line;
    unordered_set<int> nodes;
    
    while(getline(infile,line)){
        istringstream initial(line);
        string first, second;
        getline(initial, first, ' ');
        getline(initial, second, '\n');

        if(first == "&" || second == "&")
            break;
        
        int first_val = stoi(first);
        int second_val = stoi(second);

        nodes.insert(first_val);
        nodes.insert(second_val);
    }
    infile.close();

    nodeCount = nodes.size();
    matchingnodes = new int* [nodeCount];
    for(int i = 0; i < nodeCount; i++){
        matchingnodes[i] = new int [nodeCount];
        for(int j = 0; j < nodeCount; j++){
            matchingnodes[i][j] = 0;
        }
    }

    infile.open(matchinglist_csv, ifstream::in);

    while(getline(infile, line)){
        istringstream ss(line);
        string first, second;

        getline(ss, first, ' ');
        getline(ss, second, '\n');
        if(first == "&" || second == "&")
            break;

        int first_val = stoi(first);
        int second_val = stoi(second);
        
        matchingnodes[first_val][second_val] = 1;
        matchingnodes[second_val][first_val] = 1;
    }
    infile.close();
    // int adjMatrix[][] = new int[n][n];
 
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //     adjMatrix[i][j] = 0;
    //     }
    // }
}

bool AdjMatrix::existing_connection(int source, int destination){
    if(source < 0 || source > nodeCount || destination < 0 || destination > nodeCount){
        cout << "Error Out of Bounds" << endl;
        return 0;
    }
    return matchingnodes[source][destination] == 1;
}

bool AdjMatrix::error_checking_connections(){
    int count = 0;
    for(int i = 0; i < nodeCount; i++){
        for(int j = 0; j < nodeCount; j++){
            if(existing_connection(i,j))
                count++;
            if(count > 1) 
		    return false;
        }
        count = 0;
    }
    return true;
}

void AdjMatrix::printMatchings(){
    for(int i = 0; i < nodeCount; i++)
        for(int j = 0; j < nodeCount; j++)
            if(existing_connection(i,j))
                cout << i << " " << j << endl;
}
