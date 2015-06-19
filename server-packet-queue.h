/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Hiroaki Nishi Laboratory, Keio University, Japan
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Author: Janaka Wijekoon <janaka@west.sd.ekio.ac.jp>
 */
#ifndef SERVER_PACKETQUEUE_H
#define SERVER_PACKETQUEUE_H

#include "ns3/simulator.h"
#include "ns3/core-module.h" 
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/packet.h" 
#include "ns3/address-utils.h"

namespace ns3 {
//using namespace ns3
class ServerQueueEntry
{
public:
ServerQueueEntry (Ptr<const Packet> pa = 0, Address const address = Address ())
      		:m_packet (pa),
      		m_address (address)
{
  //cstrctr
}

/**
* \brief get the packet addeed to the queue
* \returns the packet 
*/
Ptr<const Packet> GetPacket () const
{
      return m_packet;
}

/**
* \brief set the packet on the queue
* \params p the packet 
*/
void SetPacket (Ptr<const Packet> p)
{
      m_packet = p;
}

/**
* \brief get the packet's destination address
* \returns the destination address of the packet
*/
Address GetAddress () const
{
      return m_address;
}

/**
* \brief set the packet's destination address
* \params add destination address of the packet
*/
void SetAddress (Address add)
{
      m_address = add;
}

private:

  Ptr<const Packet> m_packet; //!< data packet
  Address m_address; //!< destination address of the packet
};
} // END OF ns3
#endif /* sorrouting_PACKETQUEUE_H */
