



/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code SSI Interface Implementation
  
     File:     rg_ex_ms.c 
  
     Sid:      gk_ex_ms.c@@/main/3 - Sat Jul 30 02:21:28 2011
  
     Prg:      sm
  
**********************************************************************/

/** @file rg_ex_ms.c
@brief This file contains the implementation of callback functions 
registered with SSI during the LTE MAC Task initialization.
*/
/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"        /* common tokens */
#include "rgu.h"           /* RGU defines */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "crg.h"           /* CRG interface includes */
#include "rg_sch_inf.h"    /* SCH interface includes */
#include "rg_prg.h"        /* PRG interface includes */
#include "rg_env.h"        /* customisable defines and macros for LTE-MAC */
#include "rg.h"            /* defines and macros for MAC */


/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* CRG interface typedes */
#include "rg_sch_inf.x"    /* SCH interface typedefs */
#include "rg_prg.x"        /*PRG interface includes*/
#include "rg.x"            /* typedefs for MAC */


/**
 * @brief Task Activation callback function Entity SM. 
 *
 * @details
 *
 *     Function : rgHdlSMEvents
 *     
 *     Process Messages received from Entity SM
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlSMEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlSMEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlSMEvents)

   switch(pst->event)
   {
#ifdef LCRGMILRG
      case EVTLRGCFGREQ:
         /* Process a config. request */
         cmUnpkLrgCfgReq(RgMiLrgCfgReq, pst, mBuf);
         break;
      case EVTLRGCNTRLREQ:
         /* Process a control request */
         cmUnpkLrgCntrlReq(RgMiLrgCntrlReq, pst, mBuf);
         break;
      case EVTLRGSSTAREQ:
         /* Process a status request  */
         cmUnpkLrgStaReq(RgMiLrgStaReq, pst, mBuf);
         break;
      case EVTLRGSTSREQ:
         /* Process a statistics request */
         cmUnpkLrgStsReq(RgMiLrgStsReq, pst, mBuf);
         break;
#endif /* LCRGMILRG */
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}


/**
 * @brief Task Activation callback function Entity NH. 
 *
 * @details
 *
 *     Function : rgHdlNHEvents
 *     
 *     Process Messages received from Entity NH
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlNHEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlNHEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlNHEvents)

   switch(pst->event)
   {
#ifdef LCRGUICRG
      case EVTCRGBNDREQ:
         cmUnpkCrgBndReq(RgUiCrgBndReq, pst, mBuf);
         break;
      case EVTCRGUBNDREQ:
         cmUnpkCrgUbndReq(RgUiCrgUbndReq, pst, mBuf);
         break;
      case EVTCRGCFGREQ:
         cmUnpkCrgCfgReq(RgUiCrgCfgReq, pst, mBuf);
         break;
#endif            
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}

/**
 * @brief Task Activation callback function Entity KW. 
 *
 * @details
 *
 *     Function : rgHdlKWEvents
 *     
 *     Process Messages received from Entity KW
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlKWEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlKWEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlKWEvents)

   switch(pst->event)
   {
#ifdef LCRGUIRGU
      case EVTRGUBNDREQ:
         cmUnpkRguBndReq(RgUiRguBndReq, pst, mBuf);
         break;
      case EVTRGUUBNDREQ:
         cmUnpkRguUbndReq(RgUiRguUbndReq, pst, mBuf);
         break;
      case EVTRGUCDATREQ:
         cmUnpkRguCDatReq(RgUiRguCDatReq, pst, mBuf);
         break;
      case EVTRGUDDATREQ:
         cmUnpkRguDDatReq(RgUiRguDDatReq, pst, mBuf);
         break;
      case EVTRGUCSTARSP:
         cmUnpkRguCStaRsp(RgUiRguCStaRsp, pst, mBuf);
         break;
      case EVTRGUDSTARSP:
         cmUnpkRguDStaRsp(RgUiRguDStaRsp, pst, mBuf);
         break;
#ifdef LTE_L2_MEAS

      case EVTRGUL2MULTHRPMEASREQ:
         cmUnpkRguL2MUlThrpMeasReq(RgUiRguL2MUlThrpMeasReq, pst,mBuf);
         break;

#endif
#endif            
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}

/**
 * @brief Task Activation callback function Entity TF. 
 *
 * @details
 *
 *     Function : rgHdlTFEvents
 *     
 *     Process Messages received from Entity TF
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlTFEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlTFEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlTFEvents)

   switch(pst->event)
   {
#if  (defined(LCRGLITFU) || defined(LWLCRGLITFU))
      case EVTTFUBNDCFM:
         cmUnpkTfuBndCfm(RgLiTfuBndCfm, pst, mBuf);
         break;
      case EVTTFUDATIND:
         cmUnpkTfuDatInd(RgLiTfuDatInd, pst, mBuf);
         break;
      case EVTTFUTTIIND:
         cmUnpkTfuTtiInd(RgLiTfuTtiInd, pst, mBuf);
         break;
#if defined(TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD)
      case EVTTFUNONRTIND:
         cmUnpkTfuNonRtInd(RgLiTfuNonRtInd, pst, mBuf);
         break;
#endif
#endif            
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}


/**
 * @brief Task Activation callback function Entity RG SCH. 
 *
 * @details
 *
 *     Function : rgHdlRGEvents
 *     
 *     Process Messages received from Entity RG SCH
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlRGEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlRGEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlRGEvents)

   switch(pst->event)
   {
#ifdef LCRG
      case EVTINFCELLREGREQ:
         cmUnpkSchMacCellRegReq(RgSchMacCellRegReq, pst, mBuf);
         break;
      case EVTINFSFALLOCREQ:
         cmUnpkSchMacSfAllocReq(RgSchMacSfAllocReq, pst, mBuf);
         break;
      case EVTINFRLSHQREQ:
         cmUnpkSchMacRlsHqReq(RgSchMacRlsHqReq, pst, mBuf);
         break;
#ifdef LTE_ADV
      case EVTINFHQENTRESET:
         cmUnpkSchMacRstHqEntReq(RgSchMacRstHqEntReq, pst, mBuf);
         break;
#endif
      case EVTINFRLSRNTIREQ:
         cmUnpkSchMacRlsRntiReq(RgSchMacRlsRntiReq, pst, mBuf);
         break;
#ifdef LTEMAC_SPS
      case EVTINFSPSLCREG:
         cmUnpkSchMacSpsLcRegReq(RgSchMacSpsLcRegReq, pst, mBuf);
         break;
      case EVTINFSPSLCDEREG:
         cmUnpkSchMacSpsLcDeregReq(RgSchMacSpsLcDeregReq, pst, mBuf);
         break;
      case EVTINFSPSRESET:
         cmUnpkSchMacUlSpsResetReq(RgSchMacUlSpsResetReq, pst, mBuf);
         break;
#endif /* LTEMAC_SPS */
#ifdef LTE_L2_MEAS
      case EVTINFL2MEASREQ:
         cmUnpkSchMacL2MeasReq(RgSchMacL2MeasReq, pst, mBuf);
         break;
      case EVTINFL2MEASSENDREQ :
         cmUnpkSchMacL2MeasSendReq(RgSchMacL2MeasSendReq, pst , mBuf);
         break;
      case EVTINFL2MEASSTOPREQ:
         cmUnpkSchMacL2MeasStopReq(RgSchMacL2MeasStopReq, pst , mBuf);
         break;
#endif/* LTE_L2_MEAS */
      case EVTINFLCGREG:
         cmUnpkSchMacLcgRegReq(RgSchMacLcgRegReq, pst, mBuf);
         break;
#endif            
#if defined(LTE_ADV) && defined(LCPRG)
      case EVTPRGUESCELLCFGREQ:
      {
         cmUnpkPrgPMacSMacUeSCellCfgReq(RgPrgPMacSMacUeSCellCfgReq, pst, mBuf);
      }
      break;
      case EVTPRGUESCELLCFGCFM:
      case EVTPRGUESCELLLCHMODCFM:
      case EVTPRGUESCELLLCHDELCFMDEL:
      case EVTPRGUESCELLLCHADDCFM:
      {
         cmUnpkPrgSMacPMacCfgCfm(RgPrgSMacPMacCfgCfm, pst, mBuf);
      }
      break;
      case EVTPRGUESCELLDELREQ:
      {
         cmUnpkPrgPMacSMacUeSCellDelReq(RgPrgPMacSMacUeSCellDelReq, pst, mBuf);
      }
      break;
      case EVTPRGUESCELLLCHMODREQ:
      {
         cmUnpkPrgPMacSMacUeSCellLchModReq(RgPrgPMacSMacUeSCellLchModReq, pst,
                                           mBuf);
      }
      break;
      case EVTPRGUESCELLLCHDELREQ:
      {
         cmUnpkPrgPMacSMacUeSCellLchDelReq(RgPrgPMacSMacUeSCellLchDelReq, pst,
                                           mBuf);
      }
      break;
      case EVTPRGUESCELLLCHADDREQ:
      {
         cmUnpkPrgPMacSMacUeSCellLchAddReq(RgPrgPMacSMacUeSCellLchAddReq, pst,
                                           mBuf);
      }
      break;

#endif
      default:
      {
         RG_FREE_MSG(mBuf);
         break;
      }

   }
}



/**
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *     Function : rgActvTsk
 *     
 *     Primitives invoked by MAC's users/providers through
 *     a loosely coupled interface arrive here by means of 
 *     SSI's message handling. This API is registered with
 *     SSI during the Task Registration of MAC.
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgActvTsk
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PUBLIC S16 rgActvTsk(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgActvTsk)

   switch(pst->srcEnt)
   {
      /* The originator of this message is the stack manager,
       * unpack and go to the respective primitive processing function */
      case ENTSM:
          rgHdlSMEvents(pst, mBuf);
           break;
      case ENTNH:
          rgHdlNHEvents(pst, mBuf);
          break;
      case ENTKW:
          rgHdlKWEvents(pst, mBuf);
          break;
      case ENTTF:
          rgHdlTFEvents(pst, mBuf);
          break;
      case ENTRG: /* When scheduler instance sends msg to MAC */
          rgHdlRGEvents(pst, mBuf);
          break;
       default:
          RG_FREE_MSG(mBuf);
          break;
   }
   SExitTsk();
   RETVALUE(ROK);
}/* end of rgActvTsk */


/**********************************************************************
 
         End of file:     gk_ex_ms.c@@/main/3 - Sat Jul 30 02:21:28 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sm                1. Initial Release.
/main/2      ---     sm                1. LTE MAC 2.1 release
         rg003.201   ns                1. Trace added for functions.
         rg008.201   rsharon           1. Added support for SPS.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1         
$SID$        ---     rt      1. LTE MAC 4.1 release
*********************************************************************91*/
