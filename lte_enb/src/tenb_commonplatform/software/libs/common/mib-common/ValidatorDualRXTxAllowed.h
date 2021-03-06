///////////////////////////////////////////////////////////////////////////////
//
// ValidatorDualRXTxAllowed.h
//
// A validator for DualRxTxAllowed
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __ValidatorDualRXTxAllowed_h_
#define __ValidatorDualRXTxAllowed_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "ValidatorU32.h"

///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////

class ValidatorDualRXTxAllowed : public ValidatorU32
{
public:
    ValidatorDualRXTxAllowed(){};
    virtual ~ValidatorDualRXTxAllowed() {};
    virtual bool ValidateU32(MibDN fapLteDn,u32 value, u32 min, u32 max, ValidationFailureDescriptor& failureDescriptor);
private:
    std::string EcsfbReportCfgValue;
};
#endif


