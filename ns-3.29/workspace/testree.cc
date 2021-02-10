//K=2 fat tree 

/* Network Topology                    
*           c1          Core
*          / \
*         /   \
*        |     |
*       a1     a2       Aggregation
*        |     |
*       e1     e2       Edges
*        |     |
*       p1     p2       Pods
* 
*                       p1 IP: 10.1.1.0
*/ 
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/ipv4-global-routing-helper.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TreeExample2");

int main(int argc, char *argv[]){
    
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    //Creates Nodes for each Core, Aggregation, Edge, and Pod of the K=2 fat tree. 
    //Network Topology can be found on top. 
    NS_LOG_INFO ("Create Nodes");
    NodeContainer c1, a1, a2, e1, e2, p1, p2;
    c1.Create(1);
    a1.Create(1);
    a2.Create(1);
    e1.Create(1);
    e2.Create(1);
    p1.Create(1);
    p2.Create(1);

    //Creates NodeContainer that connects each component of the k=2 fat tree based on the topology found on top.
    NodeContainer p1e1 = NodeContainer (p1, e1);
    NodeContainer p2e2 = NodeContainer (p2, e2);

    NodeContainer e1a1 = NodeContainer (e1, a1);
    NodeContainer e2a2 = NodeContainer (e2, a2);

    NodeContainer a1c1 = NodeContainer (a1, c1);
    NodeContainer a2c1 = NodeContainer (a2, c1);

    //Installs internet on each Node.
    InternetStackHelper internet;
    internet.Install (c1);
    internet.Install (a1);
    internet.Install (a2);
    internet.Install (e1);
    internet.Install (e2);
    internet.Install (p1);
    internet.Install (p2);

    //Creates channels for the pod to edge.
    // NS_LOG_INFO ("Create Channels");
    // CsmaHelper csma;
    // csma.SetChannelAttribute ("DataRate",DataRateValue (DataRate ("2Mbps")));
    // csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
    // csma.SetDeviceAttribute ("Mtu", UintegerValue (1400));
    // NetDeviceContainer p1e1ChannelConnect = csma.Install (p1e1);
    // NetDeviceContainer p2e2ChannelConnect = csma.Install (p2e2);

    //creates a single point to point helper, set to values seen in examples
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
    
    //create the channels with netdevice containers
    NetDeviceContainer p1e1Connect = pointToPoint.Install(p1e1);
    NetDeviceContainer p2e2Connect = pointToPoint.Install(p2e2);
    NetDeviceContainer e1a1Connect = pointToPoint.Install(e1a1);
    NetDeviceContainer e2a2Connect = pointToPoint.Install(e2a2);
    NetDeviceContainer a1c1Connect = pointToPoint.Install(a1c1);
    NetDeviceContainer a2c1Connect = pointToPoint.Install(a2c1);

    //Assign the IP address
    NS_LOG_INFO("Assign IP");
    Ipv4AddressHelper address;
   

    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer ipp1e1;
    ipp1e1 = address.Assign(p1e1Connect);

    address.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer ipp2e2;
    ipp2e2 = address.Assign(p2e2Connect);

    address.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer ipe1a1;
    ipe1a1 = address.Assign(e1a1Connect);

    address.SetBase ("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer ipe2a2;
    ipe2a2 = address.Assign(e2a2Connect);

    address.SetBase ("10.1.5.0", "255.255.255.0");
    Ipv4InterfaceContainer ipa1c1;
    ipa1c1 = address.Assign(a1c1Connect);

    address.SetBase ("10.1.6.0", "255.255.255.0");
    Ipv4InterfaceContainer ipa2c1;
    ipa2c1 = address.Assign(a2c1Connect);
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    cout << "Routing complete.";

    uint16_t port = 9;
    UdpEchoServerHelper echoServer (port);

    //ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
    ApplicationContainer serverApp = echoServer.Install(p1.Get(0));
    serverApp.Start(Seconds(1.0));
    serverApp.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient (ipp1e1.GetAddress (0), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (10));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApps = echoClient.Install (p2.Get (0));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

    // csma.EnablePcap ("server", p1e1ChannelConnect, false);
    // csma.EnablePcap ("client", p2e2ChannelConnect, false);
    
    //Runs the simulation
    Simulator::Stop (Seconds (10.0));
    Simulator::Run ();
    Simulator::Destroy ();
    
    return 0;

    
}