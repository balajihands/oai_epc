///////////////////////////////////////////////////////////////////////////////
//
// ValidatorDeltaFPUCCHFormat1b.h
//
// A validator for DeltaFPUCCHFormat1b.
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __ValidatorDeltaFPUCCHFormat1b_h_
#define __ValidatorDeltaFPUCCHFormat1b_h_

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

class ValidatorDeltaFPUCCHFormat1b: public ValidatorU32
{
public:

    ValidatorDeltaFPUCCHFormat1b() {};
    virtual ~ValidatorDeltaFPUCCHFormat1b() {};

    virtual bool ValidateU32(u32 value, u32 min, u32 max, ValidationFailureDescriptor& failureDescriptor);

protected:

private:

};

#endif
