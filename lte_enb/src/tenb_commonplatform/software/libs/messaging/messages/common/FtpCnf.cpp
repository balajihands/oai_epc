///////////////////////////////////////////////////////////////////////////////
//
// FtpGetCnf.cpp
//
// See header file for documentation.
//
// Copyright radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <system/Trace.h>
#include <system/SerialisationUtils.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "FtpCnf.h"

using namespace std;
using namespace threeway;

///////////////////////////////////////////////////////////////////////////////
// Class Functions
///////////////////////////////////////////////////////////////////////////////

FtpCnf::FtpCnf(
        u32 ftpRequestIdentity,
        u32 ftpConfirmIdentity ) :
            m_ftpRequestIdentity(ftpRequestIdentity),
            m_ftpConfirmIdentity(ftpConfirmIdentity)
{
    ENTER();

    EXIT();
}

s32 FtpCnf::Serialise(u8* data, u32 dataMaxBytes) const
{
    ENTER();

    // Preconditions.
    RSYS_ASSERT(dataMaxBytes >= (1 + 4 + 4));

    // Serialise all the members.
    u8* serialisedData = data;

    SerialiseIt(m_ftpRequestIdentity, serialisedData);
    SerialiseIt(m_ftpConfirmIdentity, serialisedData);

    RETURN(serialisedData - data);
}

bool FtpCnf::DeSerialise(const u8* data, u32 dataLen)
{
    ENTER();

    DeSerialiseIt(m_ftpRequestIdentity, data );
    DeSerialiseIt(m_ftpConfirmIdentity, data );

    RETURN(true);
}

std::string FtpCnf::ToString() const
{
    ENTER();

    ostringstream stream;

    stream << "FtpCnf( " << m_ftpRequestIdentity <<  ":" << m_ftpConfirmIdentity << ")";

    RETURN(stream.str());
}
