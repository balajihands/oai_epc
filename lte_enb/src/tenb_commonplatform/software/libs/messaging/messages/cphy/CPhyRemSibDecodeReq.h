///////////////////////////////////////////////////////////////////////////////
//
// $Id: CPhyRemSibDecodeReq.h 28519 2010-07-21 14:39:46Z mmcternan $
//
// CPhyRemSibDecodeReq message.
//
// Copyright � 3Way Networks Ltd
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __CPhyRemSibDecodeReq_h_
#define __CPhyRemSibDecodeReq_h_

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
#define MAX_PSC_PER_FREQ 6
///////////////////////////////////////////////////////////////////////////////
// Message Structures
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/** SIB decoding control.
 * This requests that SIB decoding either be started or stopped on some cell.
 *
 * \ingroup CPhyRemWcdma
 */
typedef struct CphyRemSibDecodeReqTag
{
/*Pradeep start*/
    uarfcn_t  frequency;

    /** The band to search. */
    uband_t    band;    
/*Pradeep end*/
    /** PSC of cell to decode. */
    upsc_t  psc[MAX_PSC_PER_FREQ];

    /** If TRUE, request decoding to start, otherwise stop. */
    bool    start;

    /*karan-qc_cl_changes*/
    /** Number of PSCs to be decoded in single req.Varies from 1 to 
        MAX_PSC_PER_FREQ+1 */
    upsccnt_t  count;
}
CphyRemSibDecodeReq;

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

/** Function to serialise a CphyRemSibDecodeReq structure to a buffer.
 * Serialisation of this message can be performed by this function.
 *
 * \param[in] msg         The message to serialise.
 * \param[in,out] buffer  The data buffer into which the data is serialised.
 * \param[in] maxLen       Pointer to size of \a buffer.
 * \returns  Either a negative error code, or the length of the data.
 */
s32 CphyRemSibDecodeReq_Serialise(const CphyRemSibDecodeReq *msg,
                                  u8 *const                  buffer,
                                  u32                        maxLen);


/** Function to deserialise a CphyRemSibDecodeReq structure from a buffer.
 * Deserialisation of this message can be performed by this function.
 *
 * \param[in]     buffer  The buffer into which the structure has been stored.
 * \param[in]     maxLen  Size of \a buffer[].
 * \param[in,out] msg     Pointer to structure for population with deserialised
 *                         message.
 * \returns Size of the deserialised message if successful, otherwise a
 *           negative error code.
 */
s32 CphyRemSibDecodeReq_Deserialise(const u8                  *buffer,
                                    u32                        maxLen,
                                    CphyRemSibDecodeReq *const msg);

#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus

namespace threeway
{

class CPhyRemSibDecodeReq : public CPhyMessage
{
public:
    /**
     * Construction / destruction.
     */
    CPhyRemSibDecodeReq() { memset(&m_Struct, 0, sizeof(m_Struct)); };
    CPhyRemSibDecodeReq(const CphyRemSibDecodeReq *msg);
    virtual ~CPhyRemSibDecodeReq() {};
    //CPhyRemSibDecodeReq(upsc_t psc[MAX_PSC_PER_FREQ], uarfcn_t freq, uband_t band, bool start);
    CPhyRemSibDecodeReq(upsc_t psc, uarfcn_t freq, uband_t band, bool start);
    CPhyRemSibDecodeReq(upsc_t* psc, uarfcn_t freq, uband_t band, bool start,u8 count);

    /**
     * Implement Serialisable pure virtuals.
     */
    virtual u32  GetSerialisationId() const { return SERIALISATION_ID_CPHY_REM_SIB_DECODE_REQ; };
    virtual s32  Serialise(u8* data, u32 dataMaxBytes) const;
    virtual bool DeSerialise(const u8* data, u32 dataLen);

    /**
     * Implement CPhyMessage pure virtual.
     */
    virtual u32 GetStruct(CphyMessageData *const msg);

    /**
     * Accessors.
     */
    // None

    /**
     * Convert to string for debug.
     */
    virtual std::string ToString() const;
    virtual std::string ToBriefString() const;

private:

    /** Embedded C structure carrying parameters.
     */
    CphyRemSibDecodeReq m_Struct;
};

}

#endif // __cplusplus

#endif // __CPhyHwConfigReq_h_
