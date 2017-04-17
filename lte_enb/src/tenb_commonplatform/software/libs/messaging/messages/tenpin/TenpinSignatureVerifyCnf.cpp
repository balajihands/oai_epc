///////////////////////////////////////////////////////////////////////////////
//
// TenpinSignatureVerifyCnf.cpp
//
// See header file for documentation.
//
// Copyright radisys Limited
//
// THE FORMAT OF THE MESSAGE IMPLEMENTED BY THIS CLASS MUST NOT BE CHANGED.
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

#include "TenpinSignatureVerifyCnf.h"

///////////////////////////////////////////////////////////////////////////////
// Class Functions
///////////////////////////////////////////////////////////////////////////////

s32 TenpinSignatureVerifyCnf::Serialise(u8* data, u32 dataMaxBytes) const
{
    u8 * start = data;

    SerialiseIt(m_result, data);

    return (data - start);
}

bool TenpinSignatureVerifyCnf::DeSerialise(const u8* data, u32 dataLen)
{
    DeSerialiseIt(m_result, data);

    return true;
}

std::string TenpinSignatureVerifyCnf::ToString() const
{
    ostringstream stream;

    stream << "TenpinSignatureVerifyCnf: result=" << m_result;

    return stream.str();
}