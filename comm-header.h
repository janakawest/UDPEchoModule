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
 * Author: Janaka Wijekoon <janaka@west.sd.ekio.ac.jp>, Hiroaki Nishi <west@sd.keio.ac.jp>
 */

#ifndef COMM_HEADER_H
#define COMM_HEADER_H

#include "ns3/header.h"
#include "ns3/nstime.h"
#include "ns3/ipv4-address.h"

#include "ns3/node.h"
#include "ns3/timer.h"

namespace ns3 {

  #define SRC_SIZE 16 //!< Server Route Communication Message Header Size

/**
 * \ingroup ESLR
 * \brief this header will be attached to the clients' request packet. Then at the time server
 * responding to the packet, the relevant attributes will be changed (e.g., the packet type).
 * At the time, client received the packet, it uses the header information to analyze the 
 * statistics.
 */
class CommHeader : public Header
{
public:

/**
 * \brief This values are used inUDP echo-server/client application for testing purposes
 */
enum PacketType {
	REQ_PACKET = 0x01,//!<request packet
	REP_PACKET = 0x02,//!<reply pacekt
};

enum PacketStaus {
        PACKET_ANALYZED = 0x01,//!<all packets still to be analyzed by an SoR
        PACKET_NOTANALYZED = 0x00,//!<all analyzed packets by an SoR has this tag
};

	CommHeader();
	~CommHeader();

  /**
   * \brief Get and Set the time this packet was generated
   * \param time the time
   * \return the time the packet is generated
   */
	Time GetSentTime(void)
	{
		return m_sentTime;
	}
	  void SetSentTime(Time time)
	{
		m_sentTime = time;
	}

  /**
   * \brief Get and Set the type of the packet, (i.e., request or response packet)
   * \param packetType the type of the packet
   * \return the packet's type
   */		
	void SetPacketType(PacketType packetType)
	{
		m_packetType = packetType;
	}
	PacketType GetPacketType(void) const
	{
		return PacketType (m_packetType);
	}
	
  /**
   * \brief Get and Set whether the packet is tampered while in the propagation
   * \param packetAnalyzed is true if the packet is been analyzed
   * \return true if the packet is analyzed while packet is heading to its destination
   */		
	void SetPacketAnalyzed(PacketStaus packetAnalyzed)
	{
		m_packetAnalyzed = packetAnalyzed;
	}
	PacketStaus GetPacketAnalyzed(void) const
	{
		return PacketStaus (m_packetAnalyzed);
	}	

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);


	private:
	Time m_sentTime;
	uint8_t m_packetType;
	uint8_t m_packetAnalyzed;
	
};// end of the Comm header
/**
 * \brief Stream insertion operator.
 *
 * \param os the reference to the output stream
 * \param h the communication header
 * \returns the reference to the output stream
 */
std::ostream & operator << (std::ostream & os, const CommHeader & h);
} // namespace ns3
#endif /* COMM_HEADER_H*/ 
