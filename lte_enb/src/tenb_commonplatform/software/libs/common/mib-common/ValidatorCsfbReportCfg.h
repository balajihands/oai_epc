//
// CfsbReportCfg validator.
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __ValidatorCsfbReportCfg_h_
#define __ValidatorCsfbReportCfg_h_

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "ValidatorString.h"

///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////

class ValidatorCsfbReportCfg : public ValidatorString
{
public:
   ValidatorCsfbReportCfg() {};
   virtual ~ValidatorCsfbReportCfg() {};

   virtual bool ValidateString(const std::string& value, u32 minLength, u32 maxLength, ValidationFailureDescriptor& failureDescriptor);
};

#endif