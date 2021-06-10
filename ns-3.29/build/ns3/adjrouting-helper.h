/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ADJROUTING_HELPER_H
#define ADJROUTING_HELPER_H

#include "ns3/adjrouting.h"
#include "ns3/ipv4-routing-helper.h"

namespace ns3 {

    class Ipv4AdjRoutingHelper : public Ipv4RoutingHelper
    {
        public: 
            Ipv4AdjRoutingHelper ();
            Ipv4AdjRoutingHelper(const Ipv4AdjRoutingHelper&);

            Ipv4AdjRoutingHelper* Copy (void) const;

            // virtual? 
            virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const;

            Ptr<Ipv4AdjRouting> GetAdjRouting (Ptr<Ipv4> ipv4) const;

        }; 
    
}

#endif /* ADJROUTING_HELPER_H */

