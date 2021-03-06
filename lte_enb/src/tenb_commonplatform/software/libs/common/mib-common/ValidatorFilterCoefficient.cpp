///////////////////////////////////////////////////////////////////////////////
//
// ValidatorFilterCoefficient.cpp
//
// See header file for documentation.
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <system/Trace.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "ValidatorFilterCoefficient.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Class Functions
///////////////////////////////////////////////////////////////////////////////

bool ValidatorFilterCoefficient::ValidateU32(u32 value, u32 min, u32 max, ValidationFailureDescriptor& failureDescriptor)
{
    ENTER();

    bool isValid = ValidatorU32::ValidateU32(value, min, max, failureDescriptor);

    if ( isValid )
    {
        switch ( value )
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 11:
            case 13:
            case 15:
            case 17:
            case 19:
                isValid = true;
                break;
            default:
                isValid = false;
                break;
        }
    }

    if(!isValid)
    {
        ostringstream description;
        description << "NotValidValue (" << value << ") supported FilterCoefficient are (" <<
                "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13, 15, 17, 19)";
        failureDescriptor.Set(ValidationFailureDescriptor::CAUSE_NOT_VALID_VALUE, description.str());
    }

    RETURN(isValid);
}
