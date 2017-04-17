



/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_sch_tmr.c 
  
     Sid:      gk_sch_tmr.c@@/main/2 - Sat Jul 30 02:21:50 2011
  
     Prg:      sd 
  
**********************************************************************/

/** @file rg_sch_tmr.c 
@brief This module does processing related to timers for the scheduler.
*/
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=175;

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
#include "cm_lte.h"       /* common tokens */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "rgr.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"            /* defines and macros for MAC */
#include "rg_sch_err.h"            /* defines and macros for MAC */
#include "rg_sch_inf.h"            /* defines and macros for MAC */
#include "rg_sch.h"            /* defines and macros for MAC */
#include "rl_interface.h"
#include "rl_common.h"


/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"            /* typedefs for MAC */
#include "rg_sch.x"            /* typedefs for MAC */

#ifdef LTE_ADV
EXTERN PUBLIC Void rgSCHSCellActivation ARGS((
RgSchUeCellInfo  *sCell
));
#endif
 /** @brief This function is a utility function to start timers, it is a
  * generic function.
  *
  * @details
  *
  *     Function: rgSCHTmrStartTmr 
  *
  *         Processing steps:
  *         - Starts timer at scheduler
  *         
  *   @param[in]  RgSchCellCb    *cell
  *   @param[in]  Ptr            cb
  *   @param[in]  S16            tmrEvnt
  *   @param[in]  U32            tmrVal
  *   @return  Void
  */
#ifdef ANSI
PUBLIC Void rgSCHTmrStartTmr
(
RgSchCellCb *cell,
Ptr         cb,
S16         tmrEvnt,
U32         tmrVal
)
#else
PUBLIC Void rgSCHTmrStartTmr (cell, cb, tmrEvnt, tmrVal)
RgSchCellCb *cell;
Ptr         cb;
S16         tmrEvnt;
U32         tmrVal;
#endif
{
   CmTmrArg          arg;
   RgSchUeCb         *ue;
#ifdef LTE_ADV
   RgSchUeCellInfo *sCellCb = NULLP;
#endif

   TRC2(rgSCHTmrStartTmr);


#ifndef LTE_ADV
   ue = (RgSchUeCb*)cb;
#else
   if(tmrEvnt == RG_SCH_TMR_SCELL_DEACT)
   {
      sCellCb = (RgSchUeCellInfo *)cb;
   }
   else
   {
      ue = (RgSchUeCb*)cb;
   }
#endif

   switch (tmrEvnt)
   {
      case RG_SCH_TMR_ACKNACK_REP:
         arg.timers = &(ue->ackNakRepCb.ackNakRepTmr);
         RLOG_ARG0(L_ERROR,DBG_INSTID,cell->instIdx, 
            "Hit AckNackRep timer");
         break;
      case RG_SCH_TMR_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapTmr);
         break;
      case RG_SCH_TMR_UL_ACKNACK:
         arg.timers = &(ue->ackNakRepCb.ackNakRepUlInactvTmr);
         break;
      case RG_SCH_TMR_DL_ACKNACK:
         arg.timers = &(ue->ackNakRepCb.ackNakRepDlInactvTmr);
         break;
      case RG_SCH_TMR_UL_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapUlInactvTmr);
         break;
      case RG_SCH_TMR_DL_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapDlInactvTmr);
         break;
      case RG_SCH_TMR_TA:
         arg.timers = &(ue->taTmr);
         break;
	/*MS_WORKAROUND for CR FIXME */
#ifndef RGR_V1
      case RG_SCH_TMR_BSR:
         {
            arg.timers = &(ue->bsrTmr);
            break;
         }
#else
      case RG_SCH_TMR_BSR:
         {
            arg.timers = &(ue->bsrTmr);
            break;
         }
#endif
      case RG_SCH_TMR_TXMODE_TRNSTN:
         {
            arg.timers = &(ue->txModeTransTmr);
            break;
         }
#ifdef LTE_ADV
      case RG_SCH_TMR_SCELL_DEACT:
         {
            arg.timers = &(sCellCb->deactTmr);
            break;
         }
      case RG_SCH_TMR_SCELL_ACT_DELAY:
         {
            sCellCb = (RgSchUeCellInfo *)cb;
            arg.timers = &(sCellCb->actDelayTmr);
            break;
         }
#endif
      default:
         RLOG_ARG0(L_ERROR,DBG_INSTID,cell->instIdx,  
            "rgSCHTmrStartTmr() Incorrect Timer event");
         RETVOID;
   }

   arg.tqCp = &(cell->tqCp);
   arg.tq   = cell->tq;
   arg.cb   = (PTR)cb;
   arg.evnt = tmrEvnt;
   arg.wait = tmrVal;
   arg.max  = 1;
   arg.tNum = NOTUSED;
   cmPlcCbTq(&arg);
   RETVOID;

} /* end of */ 

 /** @brief This function stops the timer.
  *
  * @details
  *
  *     Function: rgSCHTmrStopTmr 
  *
  *         Processing steps:
  *         - Stops timer at scheduler. 
  *
  *   @param[in]  RgSchCellCb    *cell
  *   @param[in]  S16            tmrEvnt
  *   @param[in]  Ptr            cb
  *   @return  Void
  */
#ifdef ANSI
PUBLIC Void rgSCHTmrStopTmr
(
RgSchCellCb   *cell,
S16           tmrEvnt,
Ptr           cb
)
#else
PUBLIC Void rgSCHTmrStopTmr (cell, tmrEvnt, cb)
RgSchCellCb   *cell;
S16           tmrEvnt;
Ptr           cb;
#endif
{
   CmTmrArg          arg;
   RgSchUeCb         *ue;
#ifdef LTE_ADV
   RgSchUeCellInfo *sCellCb = NULLP;
#endif

   TRC2(rgSCHTmrStopTmr);


#ifndef LTE_ADV
   ue = (RgSchUeCb*)cb;
#else
   if(tmrEvnt == RG_SCH_TMR_SCELL_DEACT)
   {
      sCellCb = (RgSchUeCellInfo *)cb;
   }
   else
   {
      ue = (RgSchUeCb*)cb;
   }
#endif

   switch (tmrEvnt)
   {
      case RG_SCH_TMR_ACKNACK_REP:
         arg.timers = &(ue->ackNakRepCb.ackNakRepTmr);
         break;
      case RG_SCH_TMR_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapTmr);
         break;
      case RG_SCH_TMR_UL_ACKNACK:
         arg.timers = &(ue->ackNakRepCb.ackNakRepUlInactvTmr);
         break;
      case RG_SCH_TMR_DL_ACKNACK:
         arg.timers = &(ue->ackNakRepCb.ackNakRepDlInactvTmr);
         break;
      case RG_SCH_TMR_UL_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapUlInactvTmr);
         break;
      case RG_SCH_TMR_DL_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapDlInactvTmr);
         break;
      case RG_SCH_TMR_TA:
         arg.timers = &(ue->taTmr);
         break;
	 	/*MS_WORKAROUND for CR FIXME */
#ifndef RGR_V1
      case RG_SCH_TMR_BSR:

         {
            arg.timers = &(ue->bsrTmr);
            break;
         }
#else
      case RG_SCH_TMR_BSR:
         {
            arg.timers = &(ue->bsrTmr);
            break;
         }

#endif
      case RG_SCH_TMR_TXMODE_TRNSTN:
         {
            arg.timers = &(ue->txModeTransTmr);
            break;
         }
#ifdef LTE_ADV
      case RG_SCH_TMR_SCELL_DEACT:
         {
            arg.timers = &(sCellCb->deactTmr);
            break;
         }
      case RG_SCH_TMR_SCELL_ACT_DELAY:
         {
            sCellCb = (RgSchUeCellInfo *)cb;
            arg.timers = &(sCellCb->actDelayTmr);
            break;
         }
#endif

      default:
         RLOG_ARG0(L_ERROR,DBG_INSTID,cell->instIdx,  
            "rgSCHTmrStopTmr() Incorrect Timer event");
         RETVOID;
   }

   arg.tqCp = &(cell->tqCp);
   arg.tq   = cell->tq;
   arg.cb   = (PTR)cb;
   arg.evnt = tmrEvnt;
   arg.wait = NOTUSED;
   arg.max  = 0;
   arg.tNum = NOTUSED;
   cmRmvCbTq(&arg);
   RETVOID;
} /* end of */ 

 /** @brief This function handles timer expiry.
  *
  * @details
  *
  *     Function: rgSCHTmrProcTmr
  *
  *         Processing steps:
  *         - Handles processing on timer expiry at scheduler.
  *
  *   @param[in]  Ptr            cb
  *   @param[in]  S16            tmrEvnt
  *   @return  Void
  */
#ifdef ANSI
PUBLIC Void rgSCHTmrProcTmr 
(
Ptr         cb,
S16         tmrEvnt
)
#else
PUBLIC Void rgSCHTmrProcTmr (cb, tmrEvnt)
Ptr         cb;
S16         tmrEvnt;
#endif
{
   RgSchUeCb      *ue = NULLP;
#ifdef LTE_ADV
   RgSchUeCellInfo *sCellCb = NULLP;
#endif

   TRC2(rgSCHTmrProcTmr);

#ifndef LTE_ADV
   ue = (RgSchUeCb*)cb;
#else
   if(tmrEvnt == RG_SCH_TMR_SCELL_DEACT)
   {
      sCellCb = (RgSchUeCellInfo *)cb;
   }
   else
   {
      ue = (RgSchUeCb*)cb;
   }
#endif


   switch (tmrEvnt)
   {
      case RG_SCH_TMR_ACKNACK_REP:
         rgSCHAckNakRepTmrExpry (ue);
         break;
      case RG_SCH_TMR_MEASGAP:
         rgSCHMeasGapANRepTmrExpry (ue);
         break;
      case RG_SCH_TMR_UL_MEASGAP:
      case RG_SCH_TMR_UL_ACKNACK:
         rgSCHMeasGapANRepUlInactvTmrExpry (ue, (U8)tmrEvnt);
         break;
      case RG_SCH_TMR_DL_ACKNACK:
      case RG_SCH_TMR_DL_MEASGAP:
         rgSCHMeasGapANRepDlInactvTmrExpry (ue, (U8)tmrEvnt);
         break;
      case RG_SCH_TMR_TA:
         rgSCHDhmProcTAExp (ue);
         break;
	 	/*MS_WORKAROUND for CR FIXME */
#ifndef RGR_V1
      case RG_SCH_TMR_BSR:
         {
            rgSCHCmnBsrTmrExpry(ue);
         }
         break;
#else
      case RG_SCH_TMR_BSR:
         {
            rgSCHCmnBsrTmrExpry(ue);
         }
         break;

#endif
      case RG_SCH_TMR_TXMODE_TRNSTN:
         {
            ue->txModeTransCmplt = TRUE;
            break;
         }
#ifdef LTE_ADV
      case RG_SCH_TMR_SCELL_DEACT:
         {
            rgSCHSCellDeactTmrExpry(sCellCb);
            break;
         }
      case RG_SCH_TMR_SCELL_ACT_DELAY:
         {
            sCellCb = (RgSchUeCellInfo *)cb;
            rgSCHSCellActivation(sCellCb);
            break;
         }
#endif
      default:
         RLOG_ARG0(L_ERROR,DBG_INSTID,ue->cell->instIdx,  
            "rgSCHTmrProcTmr() Incorrect Timer event");
         RETVOID;
   }
   RETVOID;
} /* end of */ 


/**********************************************************************
 
         End of file:     gk_sch_tmr.c@@/main/2 - Sat Jul 30 02:21:50 2011
 
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
/main/1      ---     sd   1. LTE MAC 2.1 release
           rg003.201 ns   1. Trace added to functions.
           rg006.201 sd   1. ccpu00112398: Added periodic BSR timer
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---     rt  1. LTE MAC 4.1 release
*********************************************************************91*/
