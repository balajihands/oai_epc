
/********************************************************************20**
 
     Name:     Trillium LTE CNE - EMM/ESM EDM module
  
     Type:     C include file
  
     Desc:     C structures for EMM/ESM
  
     File:     cm_emm_esm.x 
  
     Sid:      cm_emm_esm.x@@/main/2 - Thu Jul  5 10:46:22 2012
  
     Prg:      rp
  
*********************************************************************21*/
#ifndef __CMEMMESMX__
#define __CMEMMESMX__

#ifdef __cplusplus
EXTERN "C" {
#endif


/* The NAS Event Structure */
/* This structure is used across by EMM, ESM Modules, MME/UE Lower Interface
   module and MME/UE controller module */

struct cmNasEvnt
{
   CmMemListCp    memCp;   /* Memory control pointer */

   U8          protDisc;   /* Protocol Discriminator to distinguish
                              between EMM and ESM from/to the NAS PDU of
                              S1AP/RRC Message */
/*cm_emm_esm_x_001.main_1: Security header type for both emm and esm messages*/
#ifdef VB_MME_AUTH
   U8          secHdrType; /* Security header type for both emm and 
                              esm messages */
#endif /* #ifdef VB_MME_AUTH */
   union
   {
      CmEmmMsg *emmEvnt;   /* Complete EMM PDU */
                           /* ESM can still be piggy backed in EMM Message
                              here */

      CmEsmMsg *esmEvnt;   /* Complete ESM PDU */
   }m;

   Ptr            pdu;
} ;

/* EDM Init structure */
typedef struct _cmEdmInit         /* EDM initialization */
{
   Ent ent;                       /* entity */
   Inst inst;                     /* instance */
   Region region;                 /* static region */
   Pool pool;                     /* static pool */
   ProcId procId;                 /* processor id */
#ifdef DEBUGP
   U32 dbgMask;                   /* debug mask */
   Txt prntBuf[PRNTSZE];          /* print buffer */
#endif
}CmEdmInit;

EXTERN CmEdmInit cmEdmInit;

/*******************************************************/
/* Function declarations */
/*******************************************************/
EXTERN S16 cmEmmEncMsg ARGS((CmNasEvnt  *evnt, U8 **buf, U16 *len));

EXTERN S16 cmEmmDecMsg ARGS((U8 *buf, CmNasEvnt **evnt, U32 len ));

EXTERN S16 cmEsmEncMsg ARGS((CmNasEvnt  *evnt, U8 **buf, U16 *len));

EXTERN S16 cmEsmDecMsg ARGS((U8 *buf, CmNasEvnt **evnt, U32 len ));

EXTERN S16 cmEmmEsmEdmInit ARGS((CmEdmInit init));

EXTERN S16 cmEdmUtlCreateEvent ARGS((CmNasEvnt   **newEvnt,
                                     U8          evntType));

EXTERN S16 cmEdmUtlFreeEvent ARGS((CmNasEvnt  **evnt));

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __CMEMMESMX__ */

/**********************************************************************

         End of file:     cm_emm_esm.x@@/main/2 - Thu Jul  5 10:46:22 2012

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
   /main/1   -       rp               1. LTE-CNE Initial Release.
/main/2      ---      cm_emm_esm_x_001.main_1 akaranth 1.Security header type for both 
                                            emm and esm messages.
*********************************************************************91*/