///////////////////////////////////////////////////////////////////////////////
//
// $Id: CPhyNlmCoscReq.h 28519 2012-03-21 14:39:46Z mmcternan $
//
// CPhyNlmCoscReq message.
//
// Copyright � 3Way Networks Ltd
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __CPhyNlmCoscReq_h_
#define __CPhyNlmCoscReq_h_

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
#include <string.h> // for memset
#endif
#include <system/SerialisationIds.h>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

#include "CPhyTypes.h"
#include "CPhyMessage.h"

///////////////////////////////////////////////////////////////////////////////
// Message Constants
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Message Structures
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif



/** NLM Cosc Req message 
 * This starts a COSC (acquire a cell), for freq-sync module.
 * CphyNlmCoscCnf will return the results if a cell is acquired, freq-sync 
 * module then adjusts its frequency using the cell as a reference. 
 *
 */
typedef struct CphyNlmCoscReqTag
{
    /** ARFCNs on which to do COSC.
     * This gives a list of ARFCNs on which to NLM has to do COSC.
     */
    uint8_t nmbArfcns;
    arfcn_t  arfcn[MAX_GSM_FREQS_FOR_NLM_COSC];
}
CphyNlmCoscReq;

#ifdef __cplusplus
}
#endif


///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/** Function to serialise a CphyNlmCoscReq structure to a buffer.
 * Serialisation of this message can be performed by this function.
 *
 * \param[in] msg         The message to serialise.
 * \param[in,out] buffer  The data buffer into which the data is serialised.
 * \param[in] maxLen       Pointer to size of \a buffer.
 * \returns  Either a negative error code, or the length of the data.
 */
s32 CphyNlmCoscReq_Serialise(const CphyNlmCoscReq *msg,
                                    u8 *const                  buffer,
                                    u32                        maxLen);


/** Function to deserialise a CphyNlmCoscReq structure from a buffer.
 * Deserialisation of this message can be performed by this function.
 *
 * \param[in]     buffer  The buffer into which the structure has been stored.
 * \param[in]     maxLen  Size of \a buffer[].
 * \param[in,out] msg     Pointer to structure for population with deserialised
 *                         message.
 * \returns Size of the deserialised message if successful, otherwise a
 *           negative error code.
 */
s32 CphyNlmCoscReq_Deserialise(const u8                  *buffer,
                                      u32                        maxLen,
                                      CphyNlmCoscReq *const msg);

#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus

namespace threeway
{

class CPhyNlmCoscReq : public CPhyMessage
{
public:
    /**
     * Construction / destruction.
     */
    //CPhyNlmCoscReq() { memset(&m_Struct, 0, sizeof(m_Struct)); };
    CPhyNlmCoscReq();
    CPhyNlmCoscReq(const CphyNlmCoscReq *msg);
    virtual ~CPhyNlmCoscReq() {};
    CPhyNlmCoscReq(uint8_t nmbArfcns, uarfcn_t arfcn[MAX_GSM_FREQS_FOR_NLM_COSC]);

    /**
     * Implement Serialisable pure virtuals.
     */
    virtual u32  GetSerialisationId() const { return SERIALISATION_ID_CPHY_NLM_COSC_REQ; };
    virtual s32  Serialise(u8* data, u32 dataMaxBytes) const;
    virtual bool DeSerialise(const u8* data, u32 dataLen);

    /**
     * Implement CPhyMessage pure virtual.
     */
    virtual u32 GetStruct(CphyMessageData *const msg);

    /**
     * Accessors.
     */
    uint8_t  GetNmbArfcns() {return m_Struct.nmbArfcns;}
    arfcn_t  GetArfcn(uint8_t n) {return m_Struct.arfcn[n];}

    /**
     * Convert to string for debug.
     */
    virtual std::string ToString() const;
    virtual std::string ToBriefString() const;

private:

    /** Embedded C structure carrying parameters.
     */
    CphyNlmCoscReq m_Struct;
};

}

#endif // __cplusplus

#endif // __CPhyHwConfigReq_h_
