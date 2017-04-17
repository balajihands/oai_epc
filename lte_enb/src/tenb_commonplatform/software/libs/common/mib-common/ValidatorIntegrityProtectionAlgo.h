///////////////////////////////////////////////////////////////////////////////
//
// ValidatorIntegrityProtectionAlgo.h
//
// IntegrityProtectionAlgo validator.
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __ValidatorIntegrityProtectionAlgo_h_
#define __ValidatorIntegrityProtectionAlgo_h_

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "ValidatorStringArray.h"

///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////

class ValidatorIntegrityProtectionAlgo : public ValidatorStringArray
{
public:
    ValidatorIntegrityProtectionAlgo() {};
    virtual ~ValidatorIntegrityProtectionAlgo() {};

    virtual bool ValidateStringArray(const std::vector<std::string>& value, u32 minLength, u32 maxLength, s32 minValue, s32 maxValue, u32 maxInstances, ValidationFailureDescriptor& failureDescriptor);

};

#endif