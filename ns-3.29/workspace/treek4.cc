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
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

int main(int argc, char *argv[]){
    
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    //Create Nodes here
    NS_LOG_INFO("Create Nodes");




    //Install internet on each node
    InternetStackHelper internet;
    internet.Install();


    //create point to point helper, set the values
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
    
    //connect the nodes
    NetDeviceContainer;

    //assign the ip addresses
    Ipv4AddressHelper address;

    address.SetBase("10.1.1.0", "255.255.255.0");
    
    //create the echo server
    UdpEchoServerApplication server (9);

    ApplicationContainer serverApp = echoServer.Install();
    serverApp.Start(Seconds(1.0));
    serverApp.Stop(Seconds(1.0));

    //create the echo client
    UdpEchoClientApplication client ( ,9);
    client.SetAttribute ("MaxPacket", UintegerValue(1));
    client.SetAttribute ("Interval", TimeValue (Seconds(1.0)));
    client.SetAttribute ("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApp = client.Install();
    clientApp.Start(Seconds(2.0));
    clientApp.Stop(Seconds(10.0));

    //populate routing table
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    //run the simulation
    Simulator::Stop (Seconds(10.0));
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}

