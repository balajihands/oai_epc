///////////////////////////////////////////////////////////////////////////////
//
// TenpinDnsServerAddReq.cpp
//
// See header file for documentation.
//
// Copyright radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <system/Trace.h>
#include <system/SerialisationUtils.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "TenpinDnsServerAddReq.h"

///////////////////////////////////////////////////////////////////////////////
// Class Functions
///////////////////////////////////////////////////////////////////////////////

s32 TenpinDnsServerAddReq::Serialise(u8* data, u32 dataMaxBytes) const
{
    u8 * start = data;

    if (serialisedLen <= dataMaxBytes)
    {
        SerialiseIt(m_id, data);
        SerialiseIt(m_addr, data);
    }

    return (data - start);
}

bool TenpinDnsServerAddReq::DeSerialise(const u8* data, u32 dataLen)
{
    bool decode = (serialisedLen <= dataLen);

    if (decode)
    {
        DeSerialiseIt(m_id, data);
        DeSerialiseIt(m_addr, data);
    }

    return decode;
}

std::string TenpinDnsServerAddReq::ToString() const
{
    ostringstream stream;

    stream << "TenpinDnsServerAddReq: id=" << m_id << " " << "addr=" << m_addr;

    return stream.str();
}
