///////////////////////////////////////////////////////////////////////////////
//
// CliFsmInterfaceStandard.h
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __CliFsmInterfaceStandard_h_
#define __CliFsmInterfaceStandard_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include <system/Serialisable.h>

#include "CliFsmInterface.h"

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace boost;

///////////////////////////////////////////////////////////////////////////////
// Local Constants
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Classes
/////////////////////////////////////////////////////////////////////////////////

class CliFsmInterfaceStandard : public CliFsmInterface
{
public:
    CliFsmInterfaceStandard( shared_ptr<CliInput> input) :
        CliFsmInterface( input ),
        m_prompt("fap:/$ ")
    {}
    virtual ~CliFsmInterfaceStandard() {}

    // Actions
    virtual void FeedbackCompletionResult(shared_ptr<CliExecuteCmdCnf> event);

    virtual const string & GetPrompt() {return m_prompt;};

private:
    string m_prompt;

};


#endif
