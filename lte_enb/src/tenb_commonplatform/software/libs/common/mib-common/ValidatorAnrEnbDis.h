///////////////////////////////////////////////////////////////////////////////
//
// ValidatorAnrEnbDis.h
//
// A validator for DrxQci
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __ValidatorAnrEnbDis_h_
#define __ValidatorAnrEnbDis_h_

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

class ValidatorAnrEnbDis: public ValidatorU32
{
public:

    ValidatorAnrEnbDis() {};
    virtual ~ValidatorAnrEnbDis() {};

    virtual bool ValidateU32(MibDN fapLteDn, u32 value, u32 min, u32 max, ValidationFailureDescriptor& failureDescriptor);
protected:

private:
   u32 DrxQci;
   u32 DrxEnable;
};

#endif
