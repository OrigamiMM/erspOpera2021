#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <random>

#include "ns3/adjrouting-module.h"
#include "ns3/adjmatrix-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("OperaSim");

Ptr<Ipv4AdjRouting> GetAdjRouter(Ptr<Node> node)
{
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4>();
  Ptr<Ipv4ListRouting> list = DynamicCast<Ipv4ListRouting>(ipv4->GetRoutingProtocol());
  int16_t valuePlaceholder = 0;
  Ptr<Ipv4AdjRouting> router = DynamicCast<Ipv4AdjRouting>(list->GetRoutingProtocol(0, valuePlaceholder));
  return router;
}

void CreateAndAggregateObjectFromTypeId(Ptr<Node> node, const std::string typeId)
{
  ObjectFactory factory;
  factory.SetTypeId(typeId);
  Ptr<Object> protocol = factory.Create<Object>();
  node->AggregateObject(protocol);
}

/*static void AddInternetStack(Ptr<Node> node)
{

  ObjectFactory m_tcpFactory;
  
  //compile error
  Ipv4RoutingHelper *m_routing;

  m_tcpFactory.SetTypeId("ns3::TcpL4Protocol");
  Ipv4StaticRoutingHelper staticRouting;
  Ipv4GlobalRoutingHelper globalRouting;
  Ipv4AdjRoutingHelper adjRouting;
  Ipv4ListRoutingHelper listRouting;

  //The entire point of this routine is to add this call
  listRouting.Add(adjRouting, 1);
  listRouting.Add(staticRouting, 0);
  listRouting.Add(globalRouting, -10);
  
  // compile error 
  m_routing = listRouting.Copy();

  if (node->GetObject<Ipv4>() != 0)
  {
    NS_FATAL_ERROR("InternetStackHelper::Install (): Aggregating "
                   "an InternetStack to a node with an existing Ipv4 object");
    return;
  }

  CreateAndAggregateObjectFromTypeId(node, "ns3::ArpL3Protocol");
  CreateAndAggregateObjectFromTypeId(node, "ns3::Ipv4L3Protocol");
  CreateAndAggregateObjectFromTypeId(node, "ns3::Icmpv4L4Protocol");

  // Set routing!!!
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4>();
  Ptr<Ipv4RoutingProtocol> ipv4Routing = m_routing->Create(node);
  ipv4->SetRoutingProtocol(ipv4Routing);

  CreateAndAggregateObjectFromTypeId(node, "ns3::TrafficControlLayer");
  CreateAndAggregateObjectFromTypeId(node, "ns3::UdpL4Protocol");
  node->AggregateObject(m_tcpFactory.Create<Object>());
  Ptr<PacketSocketFactory> factory = CreateObject<PacketSocketFactory>();
  node->AggregateObject(factory);

  Ptr<ArpL3Protocol> arp = node->GetObject<ArpL3Protocol>();
  //Ptr<TrafficControlLayer> tc = node->GetObject<TrafficControlLayer>();
  Ptr<TrafficControlLayer> tc = DynamicCast<TrafficControlLayer>(node->GetObject<TrafficControlLayer>());
  NS_ASSERT(arp);
  NS_ASSERT(tc);
  arp->SetTrafficControl(tc);

} */

static void AddInternetStack(Ptr<Node> node)
{
  ObjectFactory m_tcpFactory;
  Ipv4RoutingHelper *m_routing; 
  m_tcpFactory.SetTypeId("ns3::TcpL4Protocol");
  Ipv4StaticRoutingHelper staticRouting;
  Ipv4GlobalRoutingHelper globalRouting;
  Ipv4AdjRoutingHelper adjRouting;
  Ipv4ListRoutingHelper listRouting;
  //The entire point of this routine is to add this call
  listRouting.Add(adjRouting, 1);

  //Change complete
  listRouting.Add(staticRouting, 0);
  listRouting.Add(globalRouting, -10);
  m_routing = listRouting.Copy();
  if (node->GetObject<Ipv4>() != 0)
  {
    NS_FATAL_ERROR("InternetStackHelper::Install (): Aggregating "
                   "an InternetStack to a node with an existing Ipv4 object");
    return;
  }
  CreateAndAggregateObjectFromTypeId(node, "ns3::ArpL3Protocol");
  CreateAndAggregateObjectFromTypeId(node, "ns3::Ipv4L3Protocol");
  CreateAndAggregateObjectFromTypeId(node, "ns3::Icmpv4L4Protocol");
  //Ptr<ArpL3Protocol> arp = node->GetObject<ArpL3Protocol> ();
  //NS_ASSERT (arp);
  //arp->SetAttribute ("RequestJitter", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));
  // Set routing
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4>();
  Ptr<Ipv4RoutingProtocol> ipv4Routing = m_routing->Create(node);
  ipv4->SetRoutingProtocol(ipv4Routing);
  CreateAndAggregateObjectFromTypeId(node, "ns3::TrafficControlLayer");
  CreateAndAggregateObjectFromTypeId(node, "ns3::UdpL4Protocol");
  node->AggregateObject(m_tcpFactory.Create<Object>());
  Ptr<PacketSocketFactory> factory = CreateObject<PacketSocketFactory>();
  node->AggregateObject(factory);
  Ptr<ArpL3Protocol> arp = node->GetObject<ArpL3Protocol>();
  //Ptr<TrafficControlLayer> tc = node->GetObject<TrafficControlLayer&>(); 
  //NS_ASSERT(arp);
  //NS_ASSERT(tc);
  //arp->SetTrafficControl(tc);
}

int main(int argc, char *argv[]){
    
    // PYVIZ code
    CommandLine cmd;
    cmd.Parse (argc, argv);

    //ECMP added
    //Config::SetDefault ("ns3::Ipv4GlobalRouting::RandomEcmpRouting", BooleanValue (true));
    
    Time::SetResolution (Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    //AdjMatrices matrixObject = AdjMatrices("test.csv");
    //matrixObject.printCurrentMatching();
    //matrixObject.printAllMatchings();

    // container of 4 general nodes
    NodeContainer nodes;
    nodes.Create(4);

    NodeContainer core;
    core.Create(1);

    // server 1 to client 1 and client 2
    /*NodeContainer ser1toclients[2];
    ser1toclients[0] = NodeContainer(nodes.Get(0), nodes.Get(2));
    ser1toclients[0] = NodeContainer(nodes.Get(0), nodes.Get(2));
    ser1toclients[1] = NodeContainer(nodes.Get(0), nodes.Get(3)); */
    
    

    //NEW
    NodeContainer coretoclients[4];
    coretoclients[0] = NodeContainer(nodes.Get(0), core.Get(0));
    coretoclients[1] = NodeContainer(nodes.Get(1), core.Get(0));
    coretoclients[2] = NodeContainer(nodes.Get(2), core.Get(0));
    coretoclients[3] = NodeContainer(nodes.Get(3), core.Get(0));



    // //OLD
    // NodeContainer ser1toclients[2];
    // ser1toclients[0] = NodeContainer(nodes.Get(0), nodes.Get(2));
    // ser1toclients[1] = NodeContainer(nodes.Get(0), nodes.Get(3));

    // // server 2 to client 1 and client 2
    // NodeContainer ser2toclients[2];
    // ser2toclients[0] = NodeContainer(nodes.Get(1), nodes.Get(2));
    // ser2toclients[1] = NodeContainer(nodes.Get(1), nodes.Get(3));
    


    //not our code
    InternetStackHelper internet;
    // internet.Install(nodes);
    // internet.Install(core);

    NodeContainer::Iterator i;
    for (i = nodes.Begin(); i != nodes.End(); ++i)
    {
      NS_LOG_WARN("Installing " << (*i)->GetId());
      AddInternetStack(*i);
    }
    for (i = core.Begin(); i != core.End(); ++i)
    {
      NS_LOG_WARN("Installing " << (*i)->GetId());
      AddInternetStack(*i);
    }

    //AddInternetStack(nodes.Get(1));
    //AddInternetStack(core.Get(0));

    /*AddInternetStack(nodes.Get(0));
    AddInternetStack(nodes.Get(1));
    AddInternetStack(nodes.Get(2));
    AddInternetStack(nodes.Get(3));*/

    
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue ("2ms"));


    //NEW
    NetDeviceContainer layer1[4];
    layer1[0] = pointToPoint.Install(coretoclients[0]);
    layer1[1] = pointToPoint.Install(coretoclients[1]);
    layer1[2] = pointToPoint.Install(coretoclients[2]);
    layer1[3] = pointToPoint.Install(coretoclients[3]);


    //OLD
    // NetDeviceContainer layer1[2];
    // layer1[0] = pointToPoint.Install(ser1toclients[0]); //p2p 0-2
    // layer1[1] = pointToPoint.Install(ser1toclients[1]); //p2p 0-3
    // // int index = 0;
    // // for(auto connection: ser1toclients){
    // //     layer1[index++] = pointToPoint.Install(connection);
    // // }

    // NetDeviceContainer layer2[2];
    // layer2[0] = pointToPoint.Install(ser2toclients[0]); //p2p 1-2
    // layer2[1] = pointToPoint.Install(ser2toclients[1]); //p2p 1-3
    // index = 0;
    // for(auto connection: ser2toclients){
    //     layer2[index++] = pointToPoint.Install(connection);
    // }







    // Assign ip address for devices
    Ipv4AddressHelper address;
    Ipv4InterfaceContainer addressArray[4];
    //address.SetBase("10.1.1.0", "255.255.255.0");

    //address.SetBase("10.1.1.0", "255.255.255.0");
    //addressArray[0] = address.Assign(layer1[0]);

    //printf(addressArray[0].GetAddress(0));
    
    //int index = 0;

    //need to implement AddRoute

    //Add Route Ipv4Address network, Ipv4Mask networkMask, uint32_t port 
    // Ptr<Ipv4AdjRouting> router = GetAdjRouter(nodes.Get(0));
    // router->AddRoute(Ipv4Address("10.1.2.0"), Ipv4Mask("255.255.255.0"), 9);
    // router->AddRoute(Ipv4Address("10.1.3.0"), Ipv4Mask("255.255.255.0"), 9);
    // router->AddRoute(Ipv4Address("10.1.4.0"), Ipv4Mask("255.255.255.0"), 9);

    // router = GetAdjRouter(nodes.Get(1));
    // router->AddRoute(Ipv4Address("10.1.1.0"), Ipv4Mask("255.255.255.0"), 9);
    // router->AddRoute(Ipv4Address("10.1.3.0"), Ipv4Mask("255.255.255.0"), 9);
    // router->AddRoute(Ipv4Address("10.1.4.0"), Ipv4Mask("255.255.255.0"), 9);


    // router = GetAdjRouter(nodes.Get(2));
    // router->AddRoute(Ipv4Address("10.1.1.0"), Ipv4Mask("255.255.255.0"), 9);
    // router->AddRoute(Ipv4Address("10.1.2.0"), Ipv4Mask("255.255.255.0"), 9);
    // router->AddRoute(Ipv4Address("10.1.4.0"), Ipv4Mask("255.255.255.0"), 9);


    // router = GetAdjRouter(nodes.Get(3));
    // router->AddRoute(Ipv4Address("10.1.1.0"), Ipv4Mask("255.255.255.0"), 9);
    // router->AddRoute(Ipv4Address("10.1.2.0"), Ipv4Mask("255.255.255.0"), 9);
    // router->AddRoute(Ipv4Address("10.1.3.0"), Ipv4Mask("255.255.255.0"), 9);
  

    // // // //OLD CODE
    // // // node 0 to node 2
    // address.SetBase("10.1.1.0", "255.255.255.0");
    // addressArray[0] = address.Assign(layer1[0]);

    // // //node 0 to node 3
    // address.SetBase("10.1.2.0", "255.255.255.0");
    // addressArray[1] = address.Assign(layer1[1]);

    // // // node 1 to node 2
    // address.SetBase("10.1.3.0", "255.255.255.0");
    // addressArray[2] = address.Assign(layer2[0]);

    // // node 1 to node 3
    // address.SetBase("10.1.4.0", "255.255.255.0");
    // addressArray[3] = address.Assign(layer2[1]);

    address.SetBase("10.1.1.0", "255.255.255.0");
    addressArray[0] = address.Assign(layer1[0]);
    address.SetBase("10.1.2.0", "255.255.255.0");
    addressArray[1] = address.Assign(layer1[1]);
    address.SetBase("10.1.3.0", "255.255.255.0");
    addressArray[2] = address.Assign(layer1[2]);
    address.SetBase("10.1.4.0", "255.255.255.0");
    addressArray[3] = address.Assign(layer1[3]);

    //everything after here is okay

    //Will replace with our global routing 
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    //receiver 1
    UdpEchoServerHelper echoServer9 (9);
    ApplicationContainer serverApps9 = echoServer9.Install (nodes.Get (0));
    serverApps9.Start(NanoSeconds (100.0));

    // sender 1
    UdpEchoClientHelper echoClient1(addressArray[2].GetAddress(0), 9); // old
    //UdpEchoClientHelper echoClient1(Ipv4Address("10.1.1.0"), 9); //new
    echoClient1.SetAttribute("MaxPackets", UintegerValue (1));
    echoClient1.SetAttribute("Interval", TimeValue (NanoSeconds (1.0)));
    echoClient1.SetAttribute("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApp1 = echoClient1.Install (nodes.Get (2));
    clientApp1.Start(NanoSeconds (1000.0));   

    // receiver 2
    UdpEchoServerHelper echoServer10 (10);
    ApplicationContainer serverApps10 = echoServer10.Install(nodes.Get(1));
    serverApps10.Start(NanoSeconds (150.0));

    // sender 2
    UdpEchoClientHelper echoClient2(addressArray[3].GetAddress(0), 10); //old
    //UdpEchoClientHelper echoClient2(Ipv4Address("10.1.4.0"), 10);

    echoClient2.SetAttribute("MaxPackets", UintegerValue (1));
    echoClient2.SetAttribute("Interval", TimeValue (NanoSeconds (1.0)));
    echoClient2.SetAttribute("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApp2 = echoClient2.Install(nodes.Get(3));
    clientApp2.Start(NanoSeconds(1000.0));

    
    // modify existing echoclient and reinstall
    /*echoClient1.SetAttribute("RemoteAddress", AddressValue(addressArray[2].GetAddress(0)));
    echoClient1.SetAttribute("RemotePort", UintegerValue(10));
    clientApp1 = echoClient1.Install(nodes.Get(2));
    clientApp1.Start(NanoSeconds(5000));

    echoClient2.SetAttribute("RemoteAddress", AddressValue(addressArray[1].GetAddress(0)));
    echoClient2.SetAttribute("RemotePort", UintegerValue(9));
    clientApp2 = echoClient2.Install(nodes.Get(3));
    clientApp2.Start(NanoSeconds(5000)); */
    

    /*
    // sender 1 to receiver 2
    UdpEchoClientHelper echoClient3(addressArray[3].GetAddress (0), 10);
    echoClient3.SetAttribute("MaxPackets", UintegerValue (1));
    echoClient3.SetAttribute("Interval", TimeValue (NanoSeconds (1.0)));
    echoClient3.SetAttribute("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApp3 = echoClient3.Install (nodes.Get (2));
    clientApp3.Start(NanoSeconds (5000));        

    // sender 2 to receiver 1
    UdpEchoClientHelper echoClient4(addressArray[0].GetAddress (0), 9);
    echoClient4.SetAttribute("MaxPackets", UintegerValue (1));
    echoClient4.SetAttribute("Interval", TimeValue (NanoSeconds (1.0)));
    echoClient4.SetAttribute("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApp4 = echoClient4.Install(nodes.Get(3));
    clientApp4.Start(NanoSeconds(5000)); */

    //Call global Recompute when matching changes.
    Simulator::Stop (Seconds (6));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
