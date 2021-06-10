/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "adjrouting-helper.h"
#include "ns3/log.h"
 
namespace ns3 {
    
    NS_LOG_COMPONENT_DEFINE("Ipv4AdjRoutingHelper");

    Ipv4AdjRoutingHelper::Ipv4AdjRoutingHelper ()
    {
        //NS_LOG_FUNCTION(this);
    }

    Ipv4AdjRoutingHelper::Ipv4AdjRoutingHelper (const Ipv4AdjRoutingHelper&)
    {
        //NS_LOG_FUNCTION(this);
    }

    Ipv4AdjRoutingHelper* 
    Ipv4AdjRoutingHelper::Copy (void) const
    {
        return new Ipv4AdjRoutingHelper(*this); 
    }
    
    Ptr<Ipv4RoutingProtocol>
    Ipv4AdjRoutingHelper::Create (Ptr<Node> node) const
    {
        Ptr<Ipv4AdjRouting> adjRouting = CreateObject<Ipv4AdjRouting> ();
        adjRouting->initMatrix("test.csv");
        return adjRouting;
    }

    Ptr<Ipv4AdjRouting> 
    Ipv4AdjRoutingHelper::GetAdjRouting (Ptr<Ipv4> ipv4) const
    {
        Ptr<Ipv4RoutingProtocol> ipv4rp = ipv4->GetRoutingProtocol ();
        if (DynamicCast<Ipv4AdjRouting> (ipv4rp))
        {
            return DynamicCast<Ipv4AdjRouting> (ipv4rp); 
        }
        return 0; 
    } 
    
}

