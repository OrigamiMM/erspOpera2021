/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "adjrouting.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/net-device.h"
#include "ns3/channel.h"
#include "ns3/node.h"
#include "ns3/flow-id-tag.h"

#include "ns3/adjmatrix-module.h"


namespace ns3 {

    NS_LOG_COMPONENT_DEFINE("Ipv4AdjRouting"); 
    NS_OBJECT_ENSURE_REGISTERED(Ipv4AdjRouting);

    Ipv4AdjRouting::Ipv4AdjRouting() {
        NS_LOG_FUNCTION(this);
    }

    Ipv4AdjRouting::~Ipv4AdjRouting(){
        NS_LOG_FUNCTION(this);
    }

    bool Ipv4AdjRouting::RouteInput  (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                                LocalDeliverCallback lcb, ErrorCallback ecb)
    {
        /*
        //work with ucb (callback for packets forwarded as unicast)
        
        Ipv4Header headerPrime = header;

        Ptr<Packet> packet = ConstCast<Packet>(p);
        Ipv4Address destAddress = headerPrime.GetDestination();
        
        //ucb(route, packet, header);
        printf("ROUTE INPUT\n");


        //stew code
        // Packet arrival time
        Time now = ns3::Simulator::Now();
        std::vector<AdjRoutingEntry> routeEntries = LookupAdjRoutingEntriesIP(destAddress);
        //Find routing entry for the given IP
        if (routeEntries.size() > 0)
        {
            //NS_LOG_WARN("Host: " << stringIP(m_addr.Get()) << " Entry Hit for " << stringIP(destAddress.Get()) << " Found " << routeEntries.size() << " Entries");
        }
        else
        {
            //NS_LOG_WARN("Host: " << stringIP(m_addr.Get()) << " Entries MISS for " << stringIP(destAddress.Get()));
            return false;
        }

        //Ecmp
        uint32_t selectedPort = routeEntries[rand() % routeEntries.size()].port;

        //NS_LOG_WARN("Host: " << stringIP(m_addr.Get()) << " Setting up route for dest address " << stringIP(header.GetDestination().Get()) << " port " << selectedPort);
        Ptr<Ipv4Route> route = Ipv4AdjRouting::ConstructIpv4Route(selectedPort, destAddress);

        //Check channel state (compile error)
        //Ptr<Channel> chan = idev->GetChannel();

        //ucb (route, packet, header);
        ucb(route, packet, headerPrime); */

        printf("ROUTE INPUT May 17th\n");
        return true;    
    }

    int Ipv4AdjRouting::addressToMatching(Ipv4Address address){
        printf("Converting address to matching\n");

        address.Print(cout);
        printf("\n");
        

        //if address refers to 10.1.1.0, this is the address btwn node 0 core 0.
        if(address.Get() == Ipv4Address("10.1.1.2").Get()){
            return 0;
        } else if(address.Get() == Ipv4Address("10.1.2.2").Get()){
            return 1;
        } else if(address.Get() == Ipv4Address("10.1.3.2").Get()){
            return 2;
        } else if(address.Get() == Ipv4Address("10.1.4.2").Get()){
            return 3;
        }

        //printf("Cannot Convert Address to Matching\n");
        return -1;
    }

 
    Ptr<Ipv4Route> Ipv4AdjRouting::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
    {
        printf("ROUTING OUTPUT\n");

        Ipv4Address p_Source = header.GetSource();
        Ipv4Address p_Destination = header.GetDestination();

        //function to take in p_Destination and return a matching index active_Matching
        int sourceIndex = addressToMatching(p_Source);
        int destIndex = addressToMatching(p_Destination);

        if(sourceIndex == -1 || destIndex == -1){
            return NULL;
        }

        vector<vector<int>> currentMatchingSet =  m_adjmatrix.getMatchingIndex(Simulator::Now().GetNanoSeconds());
        m_adjmatrix.printCurrentMatching();

        if(currentMatchingSet[sourceIndex][destIndex] == 1){
            printf("Thumbs Up Case 1\n");
        }else if (currentMatchingSet[destIndex][sourceIndex] == 1){
            printf("Thumbs Up Case 2\n");
        }else{
            printf("Thumbs Down\n");
        }
        
            

        return NULL;
    }  

    TypeId Ipv4AdjRouting::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Ipv4AdjRouting")
                                .SetParent<Object>()
                                .SetGroupName("Internet")
                                .AddConstructor<Ipv4AdjRouting>();

        return tid;
    }
    
    
    void Ipv4AdjRouting::NotifyInterfaceUp(uint32_t interface)
    {
        printf("NotifyInterfaceUp\n");
        return;
    }

    void Ipv4AdjRouting::NotifyInterfaceDown(uint32_t interface)
    {
        printf("NotifyInterfaceDown\n");
        return;
    }

    void Ipv4AdjRouting::NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address)
    {
        printf("NotifyAddAddress\n");
        return;
    }

    void Ipv4AdjRouting::NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address)
    {
        printf("Notify Remove Address\n");
        return;
    }

    void Ipv4AdjRouting::SetIpv4(Ptr<Ipv4> ipv4)
    {
        //NS_LOG_LOGIC(this << "Setting up Ipv4: " << ipv4);
        printf("SetIpv4\n");
        return;
    } 

    void Ipv4AdjRouting::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
    {
        printf("Print Routing Table\n");
        return;
    }

    void Ipv4AdjRouting::DoDispose(void)
    {
        /*std::map<uint32_t, Flowlet *>::iterator itr = m_flowletTable.begin();
        for (; itr != m_flowletTable.end(); ++itr)
        {
            delete (itr->second);
        }
        m_dreEvent.Cancel();
        m_agingEvent.Cancel();
        m_ipv4 = 0;*/
        
        Ipv4RoutingProtocol::DoDispose();
        printf("DoDispose\n");
        return;
    }

    void Ipv4AdjRouting::AddRoute (Ipv4Address network, Ipv4Mask networkMask, uint32_t port){
        AdjRoutingEntry adjroutingentry;
        adjroutingentry.network = network;
        adjroutingentry.networkMask = networkMask;
        adjroutingentry.port = port;
        m_routeEntryList.push_back(adjroutingentry);
        
    }


    std::vector<AdjRoutingEntry>
    Ipv4AdjRouting::LookupAdjRoutingEntries(Ipv4Address dest){
        std::vector<AdjRoutingEntry> AdjRoutingEntries;
        std::vector<AdjRoutingEntry>::iterator itr = m_routeEntryList.begin();
        
        for (; itr != m_routeEntryList.end(); ++itr){
            if ((*itr).networkMask.IsMatch(dest, (*itr).network)){
                AdjRoutingEntries.push_back(*itr);
            }
        }
        return AdjRoutingEntries;
    }

    std::vector<AdjRoutingEntry>
    Ipv4AdjRouting::LookupAdjRoutingEntriesIP(Ipv4Address dest){
        
        std::vector<AdjRoutingEntry> AdjRouteEntries;
        std::vector<AdjRoutingEntry>::iterator itr = m_routeEntryList.begin();
        
        for (; itr != m_routeEntryList.end(); ++itr){
            if (dest.Get() == (*itr).network.Get()){
                AdjRouteEntries.push_back(*itr);
            }
        }
        return AdjRouteEntries;
    }

    void Ipv4AdjRouting::initMatrix (const char* filename){
        //m_adjmatrix = AdjMatrices::AdjMatrices();
        m_adjmatrix = AdjMatrices(filename);
    }

}
