///////////////////////////////////////////////////////////////////////////////
//
// MibCreateObjectCnf.h
//
// Message for confirming creation of a MIB object.
// Currently this has to succeed so this message just contains the new DN.
// Core MIB will assert if creation is not possible - it's a coding bug!
//
// Copyright Radisys Limited
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __MibCreateObjectCnf_h_
#define __MibCreateObjectCnf_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "MibDnMessage.h"

///////////////////////////////////////////////////////////////////////////////
// Functions / Classes
///////////////////////////////////////////////////////////////////////////////

class MibCreateObjectCnf : public MibDnMessage
{
public:
    /**
     * Constructor
     */
    MibCreateObjectCnf(const MibDN& dn) : MibDnMessage(dn) {};

    /**
     * Default constructor to allow deserialisation.
     */
    MibCreateObjectCnf() {};

    /**
     * Destructor.
     */
    virtual ~MibCreateObjectCnf() {};

    /**
     * Implement Serialisable pure virtuals.
     */
    u32 GetSerialisationId() const { return SERIALISATION_ID_MIB_CREATE_OBJECT_CNF; };

};

#endif
