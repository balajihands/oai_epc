///////////////////////////////////////////////////////////////////////////////
//
// IpTrace.h
//
// Provides methods for tracing/debugging IP packets. 
//
// Copyright � 3 Way Networks Ltd
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __IpTrace_h_
#define __IpTrace_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <3waytypes.h>

///////////////////////////////////////////////////////////////////////////////
// Classes
///////////////////////////////////////////////////////////////////////////////

namespace threeway
{

class IpTrace
{
public:
    // Trace an IP packet.
    static void TraceIpPacket(std::ostringstream& trace, const u8 *ipPacket, u32 ipPacketLen);

};

}

#endif