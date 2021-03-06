///////////////////////////////////////////////////////////////////////////////
//
// ValidatorInterAnrA5RsrpThrsld1.h
//
// A validator for InterAnrA5RsrpThrsld2
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __ValidatorInterAnrA5RsrpThrsld1_h_
#define __ValidatorInterAnrA5RsrpThrsld1_h_

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

class ValidatorInterAnrA5RsrpThrsld1: public ValidatorU32
{
public:

    ValidatorInterAnrA5RsrpThrsld1() {};
    virtual ~ValidatorInterAnrA5RsrpThrsld1() {};

    virtual bool ValidateU32(MibDN fapLteDn, u32 value, u32 min, u32 max, ValidationFailureDescriptor& failureDescriptor);
protected:

private:
   u32 InterA5RsrpThrsld1;
};

#endif
