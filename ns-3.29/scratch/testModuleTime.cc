#include "ns3/adjmatrix-module.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"

using namespace ns3;
using namespace std;

int main(){
    AdjMatrices matrixObject = AdjMatrices("matchings.csv");
    matrixObject.injectiveCheck();
    matrixObject.printAllMatchings();
    matrixObject.printCurrentMatching();

    cout << "Cycle through matchings" << endl;
    matrixObject.next_matching_index();


    Simulator::Run();
    Simulator::Destroy();
    return 0;
}