///////////////////////////////////////////////////////////////////////////////
//
// ValidatorLteQOffsetRange.h
//
// See header file for documentation.
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __ValidatorLteQOffsetRange_h_
#define __ValidatorLteQOffsetRange_h_

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////
#include "ValidatorStringArray.h"

///////////////////////////////////////////////////////////////////////////////
// Class Functions
///////////////////////////////////////////////////////////////////////////////
class ValidatorLteQOffsetRange : public ValidatorStringArray
{
public:
    ValidatorLteQOffsetRange() {};
    virtual ~ValidatorLteQOffsetRange() {};

    virtual bool ValidateStringArray(const std::vector<std::string>& value, u32 minLength, u32 maxLength, s32 minValue, s32 maxValue, u32 maxInstances, ValidationFailureDescriptor& failureDescriptor);
    static const char* arrQOffsetRange[31];
};

#endif