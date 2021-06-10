 // -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*-
 //
 // Copyright (c) 2008 University of Washington
 //
 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License version 2 as
 // published by the Free Software Foundation;
 //
 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 // GNU General Public License for more details.
 //
 // You should have received a copy of the GNU General Public License
 // along with this program; if not, write to the Free Software
 // Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 //
 //
 
 #ifndef IPV4_GLOBAL_ROUTING_H
 #define IPV4_GLOBAL_ROUTING_H
 
 #include <list>
 #include <stdint.h>
 #include "ns3/ipv4-address.h"
 #include "ns3/ipv4-header.h"
 #include "ns3/ptr.h"
 #include "ns3/ipv4.h"
 #include "ns3/ipv4-routing-protocol.h"
 #include "ns3/random-variable-stream.h"
 
 namespace ns3 {
 
 class Packet;
 class NetDevice;
 class Ipv4Interface;
 class Ipv4Address;
 class Ipv4Header;
 class Ipv4RoutingTableEntry;
 class Ipv4MulticastRoutingTableEntry;
 class Node;
 
 
 class Ipv4GlobalRouting : public Ipv4RoutingProtocol
 {
 public:
   static TypeId GetTypeId (void);
   Ipv4GlobalRouting ();
   virtual ~Ipv4GlobalRouting ();
 
   // These methods inherited from base class
   virtual Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
 
   virtual bool RouteInput  (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                             UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                             LocalDeliverCallback lcb, ErrorCallback ecb);
   virtual void NotifyInterfaceUp (uint32_t interface);
   virtual void NotifyInterfaceDown (uint32_t interface);
   virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
   virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
   virtual void SetIpv4 (Ptr<Ipv4> ipv4);
   virtual void PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;
 
   void AddHostRouteTo (Ipv4Address dest, 
                        Ipv4Address nextHop, 
                        uint32_t interface);
   void AddHostRouteTo (Ipv4Address dest, 
                        uint32_t interface);
 
   void AddNetworkRouteTo (Ipv4Address network, 
                           Ipv4Mask networkMask, 
                           Ipv4Address nextHop, 
                           uint32_t interface);
 
   void AddNetworkRouteTo (Ipv4Address network, 
                           Ipv4Mask networkMask, 
                           uint32_t interface);
 
   void AddASExternalRouteTo (Ipv4Address network,
                              Ipv4Mask networkMask,
                              Ipv4Address nextHop,
                              uint32_t interface);
 
   uint32_t GetNRoutes (void) const;
 
   Ipv4RoutingTableEntry *GetRoute (uint32_t i) const;
 
   void RemoveRoute (uint32_t i);
 
   int64_t AssignStreams (int64_t stream);
 
 protected:
   void DoDispose (void);
 
 private:
   bool m_randomEcmpRouting;
   bool m_respondToInterfaceEvents;
   Ptr<UniformRandomVariable> m_rand;
 
   typedef std::list<Ipv4RoutingTableEntry *> HostRoutes;
   typedef std::list<Ipv4RoutingTableEntry *>::const_iterator HostRoutesCI;
   typedef std::list<Ipv4RoutingTableEntry *>::iterator HostRoutesI;
 
   typedef std::list<Ipv4RoutingTableEntry *> NetworkRoutes;
   typedef std::list<Ipv4RoutingTableEntry *>::const_iterator NetworkRoutesCI;
   typedef std::list<Ipv4RoutingTableEntry *>::iterator NetworkRoutesI;
 
   typedef std::list<Ipv4RoutingTableEntry *> ASExternalRoutes;
   typedef std::list<Ipv4RoutingTableEntry *>::const_iterator ASExternalRoutesCI;
   typedef std::list<Ipv4RoutingTableEntry *>::iterator ASExternalRoutesI;
 
   Ptr<Ipv4Route> LookupGlobal (Ipv4Address dest, Ptr<NetDevice> oif = 0);
 
   HostRoutes m_hostRoutes;             
   NetworkRoutes m_networkRoutes;       
   ASExternalRoutes m_ASexternalRoutes; 
 
   Ptr<Ipv4> m_ipv4; 
 };
 
 } // Namespace ns3
 
 #endif /* IPV4_GLOBAL_ROUTING_H */
