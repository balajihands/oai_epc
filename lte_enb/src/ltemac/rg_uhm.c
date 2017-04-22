



/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_uhm.c
  
     Sid:      gk_uhm.c@@/main/3 - Sat Jul 30 02:21:56 2011
  
     Prg:      sd
  
**********************************************************************/

/** @file rg_uhm.c
@brief This module handles uplink harq related functionality in MAC.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

#include "cm_lte.h"        /* Common LTE */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* memory management */

#include "rg_env.h"        /* MAC Environment Defines */
#include "tfu.h"           /* TFU Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "rg_sch_inf.h"           /* TFU Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* system services */
#include "cm_lte.x"        /* Common LTE */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* memory management */

#include "tfu.x"           /* TFU Interface defines */
#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "rg_sch_inf.x"    /* SCH Interface defines */
#include "rg_prg.x"        /* PRG Interface defines */
#include "lrg.x"           /* LRG Interface includes */

#include "rg.x"            /* MAC includes */

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */

/**
 * @brief Handler for initializing the HARQ entity.
 *
 * @details
 *
 *     Function: rgUHMCrgUeCfg
 *     
 *     Invoked by: CRG
 *
 *     Processing Steps: 
 *      -  Initialize maxHqRetx
 *           
 *  @param[in]      *cellCb
 *  @param[in,out]  *ueCb
 *  @param[in]      *ueCfg
 *  @param[out]     *err
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgUHMCrgUeCfg
(
RgCellCb       *cellCb,
RgUeCb         *ueCb,
CrgUeCfg       *ueCfg
)
#else
PUBLIC Void rgUHMCrgUeCfg(cellCb, ueCb, ueCfg) 
RgCellCb       *cellCb;
RgUeCb         *ueCb;
CrgUeCfg       *ueCfg;
#endif
{
   TRC2(rgUHMCrgUeCfg);

   ueCb->ul.hqEnt.maxHqRetx = (ueCfg->ueUlHqCfg.maxUlHqTx - 1); 
   RETVOID;
}  /* rgUHMCrgUeCfg */

/**
 * @brief Handler for re-initializing the HARQ entity.
 *
 * @details
 *
 *     Function: rgUHMCrgUeRecfg
 *     
 *     Invoked by: CRG
 *
 *     Processing Steps: 
 *      -  Re-initialize maxHqRetx
 *           
 *  @param[in]      *cellCb
 *  @param[in,out]  *ueCb
 *  @param[in]      *ueCfg
 *  @param[out]     *err
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgUHMCrgUeRecfg
(
RgCellCb       *cellCb,
RgUeCb         *ueCb,
CrgUeRecfg     *ueRecfg
)
#else
PUBLIC Void rgUHMCrgUeRecfg(cellCb, ueCb, ueRecfg) 
RgCellCb       *cellCb;
RgUeCb         *ueCb;
CrgUeRecfg     *ueRecfg;
#endif
{
   TRC2(rgUHMCrgUeRecfg);

   ueCb->ul.hqEnt.maxHqRetx = (ueRecfg->ueUlHqRecfg.maxUlHqTx - 1); 
   RETVOID;
}  /* rgUHMCrgUeCfg */

/**********************************************************************
 
         End of file:     gk_uhm.c@@/main/3 - Sat Jul 30 02:21:56 2011
 
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
/main/1      ---     sd    1. Initial Release.
/main/2      ---     sd    1. LTE MAC 2.1 release
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---     rt    1. LTE MAC 4.1 release
*********************************************************************91*/
