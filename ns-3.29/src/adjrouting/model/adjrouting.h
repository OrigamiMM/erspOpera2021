/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ADJROUTING_H
#define ADJROUTING_H

#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-route.h"
#include "ns3/object.h"
#include "ns3/packet.h" 
#include "ns3/ipv4-header.h"
#include "ns3/data-rate.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"

#include <stdint.h>
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ptr.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/random-variable-stream.h"
#include "ns3/ipv4-route.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/data-rate.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/simulator.h"

#include "ns3/adjmatrix-module.h"

namespace ns3 {
    
    struct AdjRoutingEntry{
        Ipv4Address network;
        Ipv4Mask networkMask;
        u_int32_t port;
    };
    
    
    class Ipv4AdjRouting : public Ipv4RoutingProtocol
    {
    public:
    /**
    * \brief Get the type ID.
    * \return the object TypeId
    */

        //bool Stew(void);

        static TypeId GetTypeId (void);
        Ipv4AdjRouting ();
        ~Ipv4AdjRouting ();

        /**
        * \brief Query routing cache for an existing route, for an outbound packet
        *
        * This lookup is used by transport protocols.  It does not cause any
        * packet to be forwarded, and is synchronous.  Can be used for
        * multicast or unicast.  The Linux equivalent is ip_route_output()
        *
        * The header input parameter may have an uninitialized value
        * for the source address, but the destination address should always be 
        * properly set by the caller.
        *
        * \param p packet to be routed.  Note that this method may modify the packet.
        *          Callers may also pass in a null pointer. 
        * \param header input parameter (used to form key to search for the route)
        * \param oif Output interface Netdevice.  May be zero, or may be bound via
        *            socket options to a particular output interface.
        * \param sockerr Output parameter; socket errno 
        *
        * \returns a code that indicates what happened in the lookup
        */
        virtual Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);

        /**
        * \brief Route an input packet (to be forwarded or locally delivered)
        *
        * This lookup is used in the forwarding process.  The packet is
        * handed over to the Ipv4RoutingProtocol, and will get forwarded onward
        * by one of the callbacks.  The Linux equivalent is ip_route_input().
        * There are four valid outcomes, and a matching callbacks to handle each.
        *
        * \param p received packet
        * \param header input parameter used to form a search key for a route
        * \param idev Pointer to ingress network device
        * \param ucb Callback for the case in which the packet is to be forwarded
        *            as unicast
        * \param mcb Callback for the case in which the packet is to be forwarded
        *            as multicast
        * \param lcb Callback for the case in which the packet is to be locally
        *            delivered
        * \param ecb Callback to call if there is an error in forwarding
        * \returns true if the Ipv4RoutingProtocol takes responsibility for 
        *          forwarding or delivering the packet, false otherwise
        */ 
        virtual bool RouteInput  (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, 
                                    UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                                    LocalDeliverCallback lcb, ErrorCallback ecb);

        virtual void NotifyInterfaceUp (uint32_t interface);
    
        virtual void NotifyInterfaceDown (uint32_t interface);

        virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);

        virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);

        virtual void SetIpv4 (Ptr<Ipv4> ipv4);

        virtual void PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;

        virtual void DoDispose (void);

        void AddRoute (Ipv4Address network, Ipv4Mask networkMask, uint32_t port);

        Ptr<Ipv4Route> ConstructIpv4Route (uint32_t port, Ipv4Address destAddress);

        std::vector<AdjRoutingEntry> LookupAdjRoutingEntriesIP(Ipv4Address dest);

        std::vector<AdjRoutingEntry> LookupAdjRoutingEntries(Ipv4Address dest);

        void initMatrix(const char* filename);

        int addressToMatching(Ipv4Address address);

    private:
        std::vector<AdjRoutingEntry> m_routeEntryList;
        Ipv4Address m_addr;
        AdjMatrices m_adjmatrix;
    };

}

#endif