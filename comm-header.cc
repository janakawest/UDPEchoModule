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

#include "comm-header.h"
#include "ns3/nstime.h"

//#include "ipv4.h"
//#include "ipv4-interface.h"
//#include "ipv4-l3-protocol.h"
//#include "ns3/node.h"
//#include "ns3/node-list.h"
//#include "ns3/net-device.h"
//#include "ns3/channel.h"

//#include "ns3/header.h"
//#include "ns3/log.h"
//#include "ns3/ipv4-address.h"
//#include "ns3/nstime.h"
//#include "ns3/simulator.h"
//#include "ns3/uinteger.h"

using namespace std;
NS_LOG_COMPONENT_DEFINE ("CommHeader");
namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED (CommHeader);

TypeId
CommHeader::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::CommHeader")
		.SetParent<Header> ()
		.AddConstructor<CommHeader> ();
	return tid;
}

TypeId
CommHeader::GetInstanceTypeId (void) const
{
	NS_LOG_FUNCTION (this);
	return GetTypeId ();
}

CommHeader::CommHeader() : m_sentTime (0),
                           m_packetType (0),
                           m_packetAnalyzed (0)
{
  //cstrctr
}

CommHeader::~CommHeader()
{
  //dstrctr	
}

void  
CommHeader::Print (std::ostream &os) const
{
  os << " Packet Genarated Time: " << m_sentTime << std::endl;
} 

uint32_t  
CommHeader::GetSerializedSize (void) const 
{
	return (sizeof(uint8_t) + 
	        sizeof(uint32_t) + 
	        sizeof(uint8_t));
}//6;

void
CommHeader::Serialize (Buffer::Iterator start) const
{
	Buffer::Iterator i = start;
	i.WriteU32 ((uint32_t)(m_sentTime.GetSeconds() * 1000 + 0.5));
	i.WriteU8 (m_packetType);
	i.WriteU8(m_packetAnalyzed);
}

uint32_t
CommHeader::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;
	
	m_sentTime = Seconds( ((double) i.ReadU32 ()) / 1000.0 );
	m_packetType = i.ReadU8();
	m_packetAnalyzed = i.ReadU8();
	
	return GetSerializedSize();
}

std::ostream & operator << (std::ostream & os, const CommHeader & h)
{
  h.Print (os);
  return os;
}

}//Namespace 
