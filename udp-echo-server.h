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

#ifndef UDP_ECHO_SERVER_H
#define UDP_ECHO_SERVER_H

#include <queue>

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"

#include "server-packet-queue.h"
#include "ns3/random-variable-stream.h" 

namespace ns3 {

class Socket;
class Packet;

/**
 * \ingroup applications 
 * \defgroup udpecho UdpEcho
 */

/**
 * \ingroup udpecho
 * \brief A Udp Echo server
 *
 * Every packet received is sent back.
 */
class UdpEchoServer : public Application 
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  UdpEchoServer ();
  virtual ~UdpEchoServer ();

protected:
  virtual void DoDispose (void);

private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  /**
   * \brief Handle a packet reception.
   *
   * This function is called by lower layers.
   * This function is been modified to add the packet to the server packet buffer.   
   *
   * \param socket the socket the packet was received to.
   */
  void HandleRead (Ptr<Socket> socket);

  /**
   * \brief Schedule the next packet transmission
   */
  void ScheduleTransmit (void);  
  
  /**
   * \brief pick a packet from the buffer and reply the packet back to the sender
   */  
  void Reply(void);
  
  /**
   * \brief get the up-to-date average arrival rate (pps)
   */    
  double GetLambda (void)
  {
    return ( m_Lambda = (m_received/Simulator::Now ().GetSeconds ()));
  }
  
  /**
   * \brief get the average service rate (pps)
   */    
  double GetMue (void)
  {
    return m_Mue; 
  }

  /**
   * \brief send the statistics of the server periodically
   */   
  void SendPeriodicStat(void);

  uint16_t m_port; //!< Port on which we listen for incoming packets.
  Ptr<Socket> m_socket; //!< IPv4 Socket
  Ptr<Socket> m_socket6; //!< IPv6 Socket
  Address m_local; //!< local multicast address
  
  std::queue < ServerQueueEntry > m_serverBuffer; //!< server packet buffer (a FIFO queue)
  ServerQueueEntry m_serverQueueEntry; //!< queue entry 
  
  Ptr<UniformRandomVariable> m_rng; //!< Rng stream.
  double m_Mue; //!< the Mue of the server
  double m_Lambda; //!< the Lambda of the server
  double m_serviceRate; //!< service rate of the server
  double m_avgPacketSize; //!< the average packet of received pacekts
  uint32_t m_sent; //!< Counter for sent packets
  uint32_t m_received; //!< Counter for received packets
  EventId m_sendEvent; //!< Event to send the next packet 
  EventId m_nextPeriodicUpdate; //!< Event to send the server advertisements
  bool m_initialized; //!< initialized the reply scheduling process 
  
  Ipv4Address m_localAddress; //!< IP address of the server
  Ipv4Mask m_netMask; //!< Net mask of the server
  Ipv4Address m_ispAddress; //!< Ip address of the Default gateway (ISP's router interface)
};

} // namespace ns3

#endif /* UDP_ECHO_SERVER_H */

