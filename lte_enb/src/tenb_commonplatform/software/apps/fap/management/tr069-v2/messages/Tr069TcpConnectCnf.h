///////////////////////////////////////////////////////////////////////////////
//
// Tr069SocketsConnectCnf
//
// Copyright radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __Tr069SocketsConnectCnf_h_
#define __Tr069SocketsConnectCnf_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

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



class TcpConnectCnf : public tr069::Tr069QueueableEvent
{
public:

    TcpConnectCnf( ) :
        tr069::Tr069QueueableEvent( TR069_EVENT(TCP_CONNECT_CNF), TRANSACTION_SESSION_FSM ) { }
    virtual ~TcpConnectCnf(){};

private:
};

}

#endif
