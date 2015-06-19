/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/address-utils.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"

#include "udp-echo-server.h"

#include "ns3/eslr-module.h"
#include "ns3/comm-header.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpEchoServerApplication");
NS_OBJECT_ENSURE_REGISTERED (UdpEchoServer);

TypeId
UdpEchoServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpEchoServer")
    .SetParent<Application> ()
    .AddConstructor<UdpEchoServer> ()
    .AddAttribute ("Port", "Port on which we listen for incoming packets.",
                   UintegerValue (9),
                   MakeUintegerAccessor (&UdpEchoServer::m_port),
                   MakeUintegerChecker<uint16_t> ())
		.AddAttribute ("ServerAddress",
									 "IP Address of the Server",
									 Ipv4AddressValue (),
									 MakeIpv4AddressAccessor (&UdpEchoServer::m_localAddress),
									 MakeIpv4AddressChecker ()) 
		.AddAttribute ("NetMask",
									 "Network Mask of the server",
									 Ipv4MaskValue (),
									 MakeIpv4MaskAccessor (&UdpEchoServer::m_netMask),
									 MakeIpv4MaskChecker ())
		.AddAttribute ("ISPAddress",
									 "IP address of the Default Gateway",
									 Ipv4AddressValue (),
									 MakeIpv4AddressAccessor (&UdpEchoServer::m_ispAddress),
									 MakeIpv4AddressChecker ()) 									 									                   
  ;
  return tid;
}

UdpEchoServer::UdpEchoServer ()
{
  NS_LOG_FUNCTION (this);
  
  m_rng = CreateObject<UniformRandomVariable> ();  
  
  m_Mue = 0.0;
  m_Lambda = 0.0;
  m_serviceRate = 0.0;
  m_avgPacketSize = 0.0;
  m_sent = 0;
  m_received = 0;
  m_initialized = false;
}

UdpEchoServer::~UdpEchoServer()
{
  NS_LOG_FUNCTION (this);
  m_socket = 0;
  m_socket6 = 0;
}

void
UdpEchoServer::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
UdpEchoServer::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), m_port);
      m_socket->Bind (local);
      if (addressUtils::IsMulticast (m_local))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, m_local);
            }
          else
            {
              NS_FATAL_ERROR ("Error: Failed to join multicast group");
            }
        }
    }

  if (m_socket6 == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket6 = Socket::CreateSocket (GetNode (), tid);
      Inet6SocketAddress local6 = Inet6SocketAddress (Ipv6Address::GetAny (), m_port);
      m_socket6->Bind (local6);
      if (addressUtils::IsMulticast (local6))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket6);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, local6);
            }
          else
            {
              NS_FATAL_ERROR ("Error: Failed to join multicast group");
            }
        }
    }

  m_socket->SetRecvCallback (MakeCallback (&UdpEchoServer::HandleRead, this));
  m_socket6->SetRecvCallback (MakeCallback (&UdpEchoServer::HandleRead, this));
}

void 
UdpEchoServer::StopApplication ()
{
  NS_LOG_FUNCTION (this);

  if (m_socket != 0) 
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
  if (m_socket6 != 0) 
    {
      m_socket6->Close ();
      m_socket6->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

void 
UdpEchoServer::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (InetSocketAddress::IsMatchingType (from))
      {
        NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << "s server received " << 
                                   packet->GetSize () << " bytes from " <<
                                   InetSocketAddress::ConvertFrom (from).GetIpv4 () << 
                                   " port " <<
                                   InetSocketAddress::ConvertFrom (from).GetPort ());
      }
      else if (Inet6SocketAddress::IsMatchingType (from))
      {
        NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << "s server received " << 
                                   packet->GetSize () << " bytes from " <<
                                   Inet6SocketAddress::ConvertFrom (from).GetIpv6 () << 
                                   " port " <<
                                   Inet6SocketAddress::ConvertFrom (from).GetPort ());
      }

      packet->RemoveAllPacketTags ();
      packet->RemoveAllByteTags ();

      m_avgPacketSize = ((m_avgPacketSize * m_received) + packet->GetSize ()) / (m_received +1);
      m_received ++;

      //NS_LOG_LOGIC ("Echoing packet");
      //socket->SendTo (packet, 0, from);
      NS_LOG_INFO ("Buffering the packet");
            
      m_serverQueueEntry.SetPacket (packet);
      m_serverQueueEntry.SetAddress (from);
      m_serverBuffer.push (m_serverQueueEntry);
      if (!m_initialized)
      {
        ScheduleTransmit ();
        m_initialized = true;
        SendPeriodicStat ();
      }
    }
}

void 
UdpEchoServer::ScheduleTransmit (void)
{
  NS_LOG_FUNCTION (this);
  
  m_initialized = true;
  
	m_serviceRate = 10000000.0; //--> 10Mbps  //100000000.0; //--> 100Mbps
	double tempTime;
	double randValue;
	Time t_reSchedule;
	
	randValue = m_rng->GetValue (0.0, 1.0);
	
	m_Mue = m_serviceRate / (m_avgPacketSize * 8.0);	
	tempTime = (-1/m_Mue) *(log (randValue));
	t_reSchedule = Seconds (tempTime);
	
	//Cutoff calling a heavy function to speedup the execution.
  if (!m_serverBuffer.empty ())
  {
    Reply ();
  }
  m_sendEvent = Simulator::Schedule (t_reSchedule, &UdpEchoServer::ScheduleTransmit, this);    
}

void
UdpEchoServer::Reply(void)
{
  NS_LOG_FUNCTION (this);
  
  if (m_serverBuffer.empty ())
  {
    NS_LOG_LOGIC ("Server buffer is empty. Return!");
    return;
  }
  
  Ptr<Packet> packet;
  Address to;
  CommHeader oldHdr, newHdr;
  
  packet = m_serverBuffer.front ().GetPacket ()->Copy ();
  to = m_serverBuffer.front ().GetAddress ();
  
  m_serverBuffer.pop ();
  
  // Remove the communication header
  packet->RemoveHeader (oldHdr);
  
  // set the new header values
  newHdr.SetSentTime (oldHdr.GetSentTime ());
  newHdr.SetPacketType (CommHeader::REP_PACKET);
  newHdr.SetPacketAnalyzed (CommHeader::PACKET_ANALYZED);
  
  // add the new communication header
  packet->AddHeader (newHdr);  
  m_socket->SendTo (packet, 0, to);
  
  if (InetSocketAddress::IsMatchingType (to))
  {
    NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << "s server sent " << 
                               packet->GetSize () << 
                               " bytes to " <<
                               InetSocketAddress::ConvertFrom (to).GetIpv4 () << 
                               " port " <<
                               InetSocketAddress::ConvertFrom (to).GetPort ());
  }
  else if (Inet6SocketAddress::IsMatchingType (to))
  {
    NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << "s server sent " << 
                               packet->GetSize () << 
                               " bytes to " <<
                               Inet6SocketAddress::ConvertFrom (to).GetIpv6 () << 
                               " port " <<
                               Inet6SocketAddress::ConvertFrom (to).GetPort ());
  }  
  m_sent++;
}

void
UdpEchoServer::SendPeriodicStat(void)
{
  NS_LOG_FUNCTION (this);
  
  Ptr<Packet> p = Create<Packet> ();
  SocketIpTtlTag tag;
  p->RemovePacketTag (tag);
  tag.SetTtl (1);
  p->AddPacketTag (tag);
  
  eslr::ESLRRoutingHeader hdr;
  hdr.SetCommand (eslr::SRC);
  hdr.SetRuCommand (eslr::NO);
  hdr.SetRoutingTableRequestType (eslr::NON);
  hdr.SetAuthType (eslr::PLAIN_TEXT); 
  hdr.SetAuthData (1234); 
  
  eslr::SRCHeader srcHeader;
  srcHeader.SetMue (uint32_t (GetMue ()));
  srcHeader.SetLambda (uint32_t (GetLambda ()));
  
  srcHeader.SetServerAddress (m_localAddress);
  srcHeader.SetNetMask (m_netMask);
  
  hdr.AddSrc (srcHeader);
  
  p->AddHeader (hdr);
  
  m_socket->SendTo (p, 0, InetSocketAddress (m_ispAddress, 276));   
  
  m_nextPeriodicUpdate = Simulator::Schedule (Seconds (10), &UdpEchoServer::SendPeriodicStat, this);  
}

} // Namespace ns3
