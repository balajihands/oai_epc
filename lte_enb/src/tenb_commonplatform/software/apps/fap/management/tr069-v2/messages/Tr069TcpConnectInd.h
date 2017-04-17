///////////////////////////////////////////////////////////////////////////////
//
// Tr069SocketsConnectRej
//
// Copyright radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __Tr069SocketsConnectInd_h_
#define __Tr069SocketsConnectInd_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <nanohttp/nanohttp-server.h>
#include <nanohttp/nanohttp-request.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "Tr069QueueableEvent.h"

namespace tr069
{

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////



class TcpConnectInd : public tr069::Tr069QueueableEvent
{
public:

    TcpConnectInd( ) :
        tr069::Tr069QueueableEvent( TR069_EVENT(TCP_CONNECT_IND), SOCKET_INTERFACE_FSM )
        { }
    virtual ~TcpConnectInd(){};

};

}

#endif