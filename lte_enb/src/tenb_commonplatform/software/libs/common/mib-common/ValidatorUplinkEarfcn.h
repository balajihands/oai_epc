///////////////////////////////////////////////////////////////////////////////
//
// ValidatorUplinkEarfcn.h
//
// A validator for UplinkEarfcn.
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __ValidatorUplinkEarfcn_h_
#define __ValidatorUplinkEarfcn_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "ValidatorU32.h"
#include <vector>

struct UplinkFreq
{
      u32 minNUL;
         u32 maxNUL;
};

extern struct UplinkFreq uplinkFreq[41];

///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////

class ValidatorUplinkEarfcn : public ValidatorU32
{
public:

    ValidatorUplinkEarfcn() {};
    virtual ~ValidatorUplinkEarfcn() {};

    virtual bool ValidateU32(u32 value, u32 min, u32 max, ValidationFailureDescriptor& failureDescriptor);

private:
    std::vector<u32> BandSupported;
};

#endif
