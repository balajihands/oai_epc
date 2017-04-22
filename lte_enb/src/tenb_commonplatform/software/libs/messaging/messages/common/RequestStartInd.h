///////////////////////////////////////////////////////////////////////////////
//
// RequestStartInd.h
//
// Message sent to inform some management entity (e.g. OAM) that the entity
// is 'ready to start'.  The entity should then sit idle (or retry sending)
// until it receives a RequestStartRsp.
//
// Copyright radisys Ltd
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __RequestStartInd_h_
#define __RequestStartInd_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <system/Serialisable.h>
#include <system/SerialisationIds.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Classes
///////////////////////////////////////////////////////////////////////////////

class RequestStartInd : public threeway::Serialisable
{
public:
    /**
     * Construction / destruction.
     */
    RequestStartInd() {}
    virtual ~RequestStartInd() {};

    virtual u32 GetSerialisationId() const { return SERIALISATION_ID_REQUEST_START_IND; }
    virtual s32 Serialise(u8* data, u32 dataMaxBytes) const { return 1; }
    virtual bool DeSerialise(const u8* data, u32 dataLen) { return true; }

    virtual std::string ToString() const { return "RequestStartInd"; }

private:
};

#endif
