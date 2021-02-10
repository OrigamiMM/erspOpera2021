//K=4 fat tree                   
/*                                   Network Topology of k=4 fat tree                    
*                    c0               c1               c2               c3                           Core
*                  / /\ \           / /\ \           / /\ \           / /\ \
*                 / /  \ \         / /  \ \         / /  \ \         / /  \ \                                 
*                / /    \ \       / /    \ \       / /    \ \       / /    \ \                                    
*              a0 a2    a4 a6   a0 a2    a4 a6   a1 a3    a5 a7   a1 a3    a5 a7    
*                                               .
*                                               .
*                                               .
*      c0 c1 c2 c3          c0 c1 c2 c3            c0 c1 c2 c3          c0 c1 c2 c3
*       \ |  | /              \ |  | /              \ |  | /              \ |  | /                               
*        \|  |/                \|  |/                \|  |/                \|  |/
*        a0  a1                a2  a3                a4  a5                a6  a7                   Aggregation
*        |\  /|                |\  /|                |\  /|                |\  /|
*        | \/ |                | \/ |                | \/ |                | \/ |
*        | /\ |                | /\ |                | /\ |                | /\ |       
*        |/  \|                |/  \|                |/  \|                |/  \|
*        e0   e1               e2   e3               e4   e5               e6   e7                  Edges
*        /|   |\               /|   |\               /|   |\               /|   |\
*       / |   | \             / |   | \             / |   | \             / |   | \
*      |  |   |  |           |  |   |  |           |  |   |  |           |  |   |  |
*     p0 p1  p2  p3         p4 p5  p6  p7         p8 p9 p10 p11        p12 p13 p14 p15              Pods    
* 
*                       p0 IP: 10.1.1.0
*/ 
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "string"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("TreeExample");

int main(int argc, char *argv[]){
    
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    //Creates Nodes for each Core, Aggregation, Edge, and Pod of the K=4 fat tree. 
    //Network Topology can be found on top. 
    NS_LOG_INFO ("Create Nodes");
    NodeContainer core, agg, edg, p;
    core.Create(4);
    agg.Create(8);
    edg.Create(8);
    p.Create(16);

    //Creates NodeContainer that connects each component of the k=4 fat tree based on the topology found on top.
    
    //core to aggregate containers
    NodeContainer coreToAgg[16];
    int index = 0; //the special int index
    int coreIndex = 0;
    for(int i = 0; i < 8; i++){
        coreToAgg[index] = NodeContainer(agg.Get(i), core.Get(coreIndex % 4));
        coreIndex++;
        index++;
        coreToAgg[index] = NodeContainer(agg.Get(i), core.Get(coreIndex % 4));
        index++;
        coreIndex++;
    }
    //i = 0
    //coreToAgg[0] = NodeContainer(a[0], c[0]) a1c1
    //coreToAgg[1] = NodeContainer(a[0], c[1]) a1c2
    //i=1
    //coreToAgg[2] = NodeContainer(a[1], c[2]) a2c3
    //coreToAgg[3] = NodeContainer(a[1], c[3]) a2c4
    //coreToAgg[4] = a3c1   5 = a3c2
    
    // 0 = a0c0     1 = a0c1        2 = a1a2       3 = a1c3
    // 4 = a2c0     5 = a3c1        6 = a4c2       7 = a4c3     
    // 8 = a5c0     9 = a5c1       10 = a6c2      11 = a6c3
    //12 = a7c0    13 = a7c1       14 = a8c2      15 = a8c3      
    

    //aggregate to edge containers
    NodeContainer aggToEdg[16];
    index = 0;
    for(int i = 0; i < 8; i+=2){
        aggToEdg[index] = NodeContainer(edg.Get(i), agg.Get(i)); //e0a0  // e3a3
        index++;
        aggToEdg[index] = NodeContainer(edg.Get(i), agg.Get(i + 1)); //e0a1
        index++;
        aggToEdg[index] = NodeContainer(edg.Get(i + 1), agg.Get(i)); //e1a0
        index++;
        aggToEdg[index] = NodeContainer(edg.Get(i + 1), agg.Get(i + 1)); //e1a0
        index++;
    }
    //i = 0
    //

    //0  = e0a0     1  = e0a1       2  = e1a0       3 = e1a1
    //4  = e2a2     5  = e2a3       6  = e3a2       7 = e3a3     
    //8  = e4a4     9  = e4a5       10 = e5a4      11 = e5a5
    //12 = e6a6    13  = e6a7       14 = e7a6      15 = e7a7

    //edge to pc container
    NodeContainer edgToP[16];
    index=0;
    for(int i = 0; i < 8; i++){
        edgToP[index] = NodeContainer(p.Get(index), edg.Get(i)); //p0e0 //p2e1
        index++;
        edgToP[index] = NodeContainer(p.Get(index), edg.Get(i)); //p1e0 //p3e1
        index++;
    }

    //Installs internet on each Node.
    InternetStackHelper internet;
    internet.Install (core);
    internet.Install (agg);
    internet.Install (edg);
    internet.Install (p);

    //creates a single point to point helper, set to values seen in examples
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
    
    NetDeviceContainer layer1[16];
    index = 0;
    for(auto connection: coreToAgg){
        layer1[index++] = pointToPoint.Install(connection);
    }

    NetDeviceContainer layer2[16];
    index = 0;
    for(auto connection: aggToEdg){
        layer2[index++] = pointToPoint.Install(connection);
    }

    NetDeviceContainer layer3[16];
    index = 0;
    for(auto connection: edgToP){
        layer3[index++] = pointToPoint.Install(connection);
    }

    //Assign the IP address
    Ipv4AddressHelper address;
    Ipv4InterfaceContainer addressArray[48]; 
    index = 0;
    int ipindex = 1;

    //string addy = "255.255.255.0";
    for(auto channel: layer3){
        string ipAddy = "10.1." + to_string(ipindex++) + ".0"; 
        char ipA[1024];
        strncpy(ipA, ipAddy.c_str(), sizeof(ipA));
        ipA[sizeof(ipA) - 1] = 0;  //by Chowlett
        address.SetBase (ipA, "255.255.255.0"); 
        addressArray[index] = address.Assign(channel);
        index++;
    }

    for(auto channel: layer2){
        string ipAddy = "10.1." + to_string(ipindex++) + ".0";
        char ipA[1024];
        strncpy(ipA, ipAddy.c_str(), sizeof(ipA));
        ipA[sizeof(ipA) - 1] = 0;  //by Chowlett
        address.SetBase (ipA, "255.255.255.0");
        addressArray[index] = address.Assign(channel);
        index++;
    }

    for(auto channel: layer1){
        string ipAddy = "10.1." + to_string(ipindex++) + ".0";
        char ipA[1024];
        strncpy(ipA, ipAddy.c_str(), sizeof(ipA));
        ipA[sizeof(ipA) - 1] = 0;  //by Chowlett
        address.SetBase (ipA, "255.255.255.0");
        addressArray[index] = address.Assign(channel);
        index++;
    }
    
    
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    UdpEchoServerHelper server (9);
    ApplicationContainer apps = server.Install (p.Get (0));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    UdpEchoClientHelper client (addressArray[0].GetAddress (0), 9);
    client.SetAttribute ("MaxPackets", UintegerValue (10));
    client.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    client.SetAttribute ("PacketSize", UintegerValue (1024));

    apps = client.Install (NodeContainer (p.Get (15)));
    apps.Start (Seconds (2.0));
    apps.Stop (Seconds (10.0));

    
    //Runs the simulation
    Simulator::Stop (Seconds (10.0));
    Simulator::Run ();
    Simulator::Destroy ();
    NS_LOG_INFO ("Done.");

    return 0;
}

