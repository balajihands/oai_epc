///////////////////////////////////////////////////////////////////////////////
//
// Tr069RpcGetParameterAttributes
//
// Copyright radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __Tr069RpcGetParameterAttributes_h_
#define __Tr069RpcGetParameterAttributes_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

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

typedef std::vector<std::string> ParameterNames;


class RpcGetParameterAttributes : public tr069::Tr069QueueableEvent
{
public:
    RpcGetParameterAttributes( ) :
        tr069::Tr069QueueableEvent( TR069_EVENT(RPC_GET_PARAMETER_NAMES), ANONYMOUS)
    {
    }

    virtual ~RpcGetParameterAttributes()
    {
        m_parameters.clear();
    };

    void Add( const std::string & parameter)
    {
        m_parameters.push_back(parameter);
    }

    void SetCwmpId( std::string cwmpId)
    {
        m_cwmpId = cwmpId;
    }

    std::string GetCwmpId(void)const
    {
        return m_cwmpId;
    }

    const ParameterNames & GetParameters(void) const
    {
        return m_parameters;
    }


private:
    ParameterNames m_parameters;
    std::string m_cwmpId;
};

}

#endif

