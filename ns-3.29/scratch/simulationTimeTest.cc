#include "ns3/adjmatrix-module.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
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

int main(int argc, char *argv[]){
    
    // PYVIZ code
    CommandLine cmd;
    cmd.Parse (argc, argv);

    //ECMP added
    //Config::SetDefault ("ns3::Ipv4GlobalRouting::RandomEcmpRouting", BooleanValue (true));
    
    Time::SetResolution (Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // container of 4 general nodes
    NodeContainer nodes;
    nodes.Create(4);

    // server 1 to client 1 and client 2
    NodeContainer ser1toclients[2];
    ser1toclients[0] = NodeContainer(nodes.Get(0), nodes.Get(2));
    ser1toclients[1] = NodeContainer(nodes.Get(0), nodes.Get(3));

    // server 2 to client 1 and client 2
    NodeContainer ser2toclients[2];
    ser2toclients[0] = NodeContainer(nodes.Get(1), nodes.Get(2));
    ser2toclients[1] = NodeContainer(nodes.Get(1), nodes.Get(3));

    InternetStackHelper internet;
    internet.Install(nodes);
    
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue ("2ms"));

    
    NetDeviceContainer layer1[2];
    layer1[0] = pointToPoint.Install(ser1toclients[0]); //p2p 0-2
    layer1[1] = pointToPoint.Install(ser1toclients[1]); //p2p 0-3
    // int index = 0;
    // for(auto connection: ser1toclients){
    //     layer1[index++] = pointToPoint.Install(connection);
    // }

    NetDeviceContainer layer2[2];
    layer2[0] = pointToPoint.Install(ser2toclients[0]); //p2p 1-2
    layer2[1] = pointToPoint.Install(ser2toclients[1]); //p2p 1-3
    // index = 0;
    // for(auto connection: ser2toclients){
    //     layer2[index++] = pointToPoint.Install(connection);
    // }

    // Assign ip address for devices
    Ipv4AddressHelper address;
    Ipv4InterfaceContainer addressArray[4];
    //address.SetBase("10.1.1.0", "255.255.255.0");
    
    //int index = 0;
    
    // node 0 to node 2
    address.SetBase("10.1.1.0", "255.255.255.0");
    addressArray[0] = address.Assign(layer1[0]);

    //node 0 to node 3
    address.SetBase("10.1.2.0", "255.255.255.0");
    addressArray[1] = address.Assign(layer1[1]);

    // node 1 to node 2
    address.SetBase("10.1.3.0", "255.255.255.0");
    addressArray[2] = address.Assign(layer2[0]);

    // node 1 to node 3
    address.SetBase("10.1.4.0", "255.255.255.0");
    addressArray[3] = address.Assign(layer2[1]);


    //Will replace with our global routing 
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    //receiver 1
    UdpEchoServerHelper echoServer9 (9);
    ApplicationContainer serverApps9 = echoServer9.Install (nodes.Get (0));
    serverApps9.Start(Seconds (1.0));
    serverApps9.Stop(Seconds (10.0));

    // sender 1
    UdpEchoClientHelper echoClient1(addressArray[0].GetAddress (0), 9);
    echoClient1.SetAttribute("MaxPackets", UintegerValue (1));
    echoClient1.SetAttribute("Interval", TimeValue (Seconds (1.0)));
    echoClient1.SetAttribute("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApp1 = echoClient1.Install (nodes.Get (2));
    clientApp1.Start(Seconds (2.0));        
    clientApp1.Stop(Seconds (10.0));

    // receiver 2
    UdpEchoServerHelper echoServer10 (10);
    ApplicationContainer serverApps10 = echoServer10.Install(nodes.Get(1));
    serverApps10.Start(Seconds (1.0));
    serverApps10.Stop(Seconds (10.0));

    // sender 2
    UdpEchoClientHelper echoClient2(addressArray[3].GetAddress (0), 10);
    echoClient2.SetAttribute("MaxPackets", UintegerValue (1));
    echoClient2.SetAttribute("Interval", TimeValue (Seconds (1.0)));
    echoClient2.SetAttribute("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApp2 = echoClient2.Install(nodes.Get(3));
    clientApp2.Start(Seconds(2.0));
    clientApp2.Stop(Seconds(10.0));

    /*// sender 1 to receiver 2
    UdpEchoClientHelper echoClient3(addressArray[3].GetAddress (0), 10);
    echoClient3.SetAttribute("MaxPackets", UintegerValue (1));
    echoClient3.SetAttribute("Interval", TimeValue (Seconds (1.0)));
    echoClient3.SetAttribute("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApp3 = echoClient3.Install (nodes.Get (2));
    clientApp3.Start(Seconds (3.5));        
    clientApp3.Stop(Seconds (10.0));

    // sender 2 to receiver 1
    UdpEchoClientHelper echoClient4(addressArray[0].GetAddress (0), 9);
    echoClient4.SetAttribute("MaxPackets", UintegerValue (1));
    echoClient4.SetAttribute("Interval", TimeValue (Seconds (1.0)));
    echoClient4.SetAttribute("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApp4 = echoClient4.Install(nodes.Get(3));
    clientApp4.Start(Seconds(3.5));
    clientApp4.Stop(Seconds(10.0)); */

    //Call global Recompute when matching changes.
    Simulator::Stop (Seconds (5.0));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}