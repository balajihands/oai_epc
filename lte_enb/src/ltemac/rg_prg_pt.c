



/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for MAC to MAC (PRG) interface 
  
     File:     rg_mac_pt.c 
  
     Sid:      gk_mac_pt.c
  
     Prg:      rr
  
**********************************************************************/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"           /* layer manager for MAC */
#include "crg.h"           /* CRG interface includes*/
#include "rgu.h"           /* RGU interface includes*/
#include "tfu.h"           /* TFU interface includes*/
#include "rg_sch_inf.h"    /* SCH interface includes*/
#include "rg_prg.h"        /* PRG interface includes*/
#include "rg_env.h"        /* MAC Enviroments includes */
#include "rg.h"            /* MAC includes*/
#include "rg_err.h"        /* MAC error includes*/

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
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* layer management typedefs for MAC */
#include "rg_prg.x"        /* PRG interface typedefs*/
#include "rg.x"            /* typedefs for MAC */

#ifdef LTE_ADV
#include "rg_pom_scell.x"

/* Matrix for Ue SCell Config Req*/
PRIVATE CONSTANT RgPrgUeSCellCfgReq RgPrgPMacSMacUeSCellCfgReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
  cmPkPrgPMacSMacUeSCellCfgReq,
#else
   RgPrgPMacSMacUeSCellCfgReq
#endif
};

/**
* @brief Ue SCell config Req from PMac to SMac 
*
* @details
*
*     Function : RgPrgPMacSMacUeSCellCfg
*
*  @param[in]   Pst         *pst
*  @param[in]   RgPrgUeSCellCfgInfo *ueSCellCfgInfo;
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgPrgPMacSMacUeSCellCfg
(
 Pst         *pst,
 RgPrgUeSCellCfgInfo *ueSCellCfgInfo
)
#else
PUBLIC S16 RgPrgPMacSMacUeSCellCfg(pst, ueSCellCfgInfo)
 Pst         *pst;
 RgPrgUeSCellCfgInfo *ueSCellCfgInfo;
#endif
{

   TRC3(RgPrgPMacSMacUeSCellCfg);

   RETVALUE((*RgPrgPMacSMacUeSCellCfgReqMt[0])(pst, ueSCellCfgInfo));
}

/* Matrix for config confirm from SMac to Pmac*/
PRIVATE CONSTANT RgSMacPMacCfgCfm RgPrgSMacPMacCfgCfmMt[RG_PRG_MAX] =
{
#ifdef LCPRG
   cmPkPrgSMacPMacCfgCfm,
#else
   RgPrgSMacPMacCfgCfm
#endif
};

/**
* @brief Config confirm from SMac to PMac for Ue Cell Config Req 
*
* @details
*
*     Function :RgPrgSMacPMacCfg
*
*  @param[in]   Pst              *pst
*  @param[in]   RgPrgCfgCfmInfo  *cfmCfm
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgPrgSMacPMacCfg
(
 Pst             *pst,
 RgPrgCfgCfmInfo *cfgCfm
)
#else
PUBLIC S16 RgPrgSMacPMacCfg(pst, cfgCfm)
 Pst             *pst;
 RgPrgCfgCfmInfo *cfgCfm;
#endif
{

   TRC3(RgPrgSMacPMacCfg);

   RETVALUE((*RgPrgSMacPMacCfgCfmMt[0])(pst, cfgCfm));
}

/* Matrix for Ue SCell delete req/Ue Id change req from PMac to SMac */
PRIVATE CONSTANT RgUeSCellDelReq RgPrgPMacSMacUeSCellDelReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
   cmPkPrgPMacSMacUeSCellDelReq,
#else
   RgPrgPMacSMacUeSCellDelReq
#endif
};

/**
* @brief Ue SCell delete Req/Ue Id change Req from PMac to SMac
*
* @details
*
*     Function : RgPrgPMacSMacUeSCellDel 
*
*  @param[in]   Pst                 *pst
*  @param[in]   RgPrgUeSCellDelInfo *ueSCellDelInfo;
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgPrgPMacSMacUeSCellDel
(
 Pst                 *pst,
 RgPrgUeSCellDelInfo *ueSCellDelInfo
)
#else
PUBLIC S16 RgPrgPMacSMacUeSCellDel(pst, ueSCellDelInfo)
 Pst                 *pst;
 RgPrgUeSCellDelInfo *ueSCellDelInfo;
#endif
{

   TRC3(RgPrgPMacSMacUeSCellDel);

   RETVALUE((*RgPrgPMacSMacUeSCellDelReqMt[0])(pst, ueSCellDelInfo));
}



/* Matrix for Lch ReConfig Req*/
PRIVATE CONSTANT RgPrgUeScellModLchReq RgPrgPMacSMacUeScellModLchReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
  cmPkPrgPMacSMacUeSCellLchModReq,
#else
   RgPrgPMacSMacUeSCellLchModReq
#endif
};


/**
* @brief Ue Lch Reconfig Req from PMac to SMac 
*
* @details
*
*     Function : RgPrgPMacSMacUeScellLchMod
*
*  @param[in]   Pst                      *pst
*  @param[in]   RgPrgUeSCellLchModInfo   *lchCfgInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgPrgPMacSMacUeScellLchMod
(
 Pst                      *pst,
 RgPrgUeSCellLchModInfo   *lchCfgInfo
)
#else
PUBLIC S16 RgPrgPMacSMacUeScellLchMod(pst, lchCfgInfo)
 Pst                      *pst;
 RgPrgUeSCellLchModInfo   *lchCfgInfo;
#endif
{

   TRC3(RgPrgPMacSMacUeScellLchMod);

   RETVALUE((*RgPrgPMacSMacUeScellModLchReqMt[0])(pst, lchCfgInfo));
}


/* Matrix for delete Lch Req*/
PRIVATE CONSTANT RgPrgUeScellDelLchReq RgPrgPMacSMacUeScellDelLchReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
  cmPkPrgPMacSMacUeSCellLchDelReq,
#else
   RgPrgPMacSMacUeSCellLchDelReq
#endif
};


/**
* @brief Delete Lch Req from PMac to SMac 
*
* @details
*
*     Function : RgPrgPMacSMacUeScellLchDel
*
*  @param[in]   Pst                      *pst
*  @param[in]   RgPrgUeSCellLchDelInfo   *delLcCb
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgPrgPMacSMacUeScellLchDel
(
 Pst                       *pst,
 RgPrgUeSCellLchDelInfo    *delLcCb
)
#else
PUBLIC S16 RgPrgPMacSMacUeScellLchDel(pst, delLcCb)
 Pst                       *pst;
 RgPrgUeSCellLchDelInfo    *delLcCb;
#endif
{

   TRC3(RgPrgPMacSMacUeScellLchDel);

   RETVALUE((*RgPrgPMacSMacUeScellDelLchReqMt[0])(pst, delLcCb));
}


/* Matrix for Lch Config Req*/
PRIVATE CONSTANT RgPrgUeScellAddLchReq RgPrgPMacSMacUeScellAddLchReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
  cmPkPrgPMacSMacUeSCellLchAddReq,
#else
   RgPrgPMacSMacUeSCellLchAddReq
#endif
};


/**
* @brief Ue Lch config Req from PMac to SMac 
*
* @details
*
*     Function : RgPrgPMacSMacUeScellLchAdd
*
*  @param[in]   Pst                      *pst
*  @param[in]   RgPrgUeSCellLchAddInfo   *lchCfgInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgPrgPMacSMacUeScellLchAdd
(
 Pst                      *pst,
 RgPrgUeSCellLchAddInfo   *lchCfgInfo
)
#else
PUBLIC S16 RgPrgPMacSMacUeScellLchAdd(pst, lchCfgInfo)
 Pst                      *pst;
 RgPrgUeSCellLchAddInfo   *lchCfgInfo;
#endif
{

   TRC3(RgPrgPMacSMacUeScellLchAdd);

   RETVALUE((*RgPrgPMacSMacUeScellAddLchReqMt[0])(pst, lchCfgInfo));
}

PRIVATE CONSTANT RgLaaPrgPmacTbReq RgLaaPrgSndPmacTbReqMt[RG_PRG_MAX] =
{
#ifdef LCPRG
#else
  rgLaaPrgRcvPmacTbReq 
#endif
};

/**
* @brief TB Req from PMac to SMac 
*
* @details
*
*     Function : RgLaaPrgSndPmacTbReq 
*
*  @param[in]   Pst            *pst
*  @param[in]   RgPrgLaaTbReq  *tbReq 
*  @return   S16
*      -# ROK
**/

#ifdef ANSI
PUBLIC Void RgLaaPrgSndPmacTbReq
(
Pst            *pst,
RgLaaPrgTbReq  *tbReq
)
#else
PUBLIC Void RgLaaPrgSndPmacTbReq(pst,tbReq)
Pst            *pst;
RgLaaPrgTbReq  *tbReq;
#endif
{
   TRC3(RgLaaPrgSndPmacTbReq);

   (*RgLaaPrgSndPmacTbReqMt[0])(pst, tbReq);
}

PRIVATE CONSTANT RgLaaPrgPmacTbInd RgLaaPrgSndTbIndMt[RG_PRG_MAX] =
{
#ifdef LCPRG
#else
 rgLaaPrgRcvSmacTbInd 
#endif
};

/**
* @brief TB Ind from PSac to PMac 
*
* @details
*
*     Function : RgLaaPrgSndTbInd 
*
*  @param[in]   Pst            *pst
*  @param[in]   RgPrgLaaTbInd  *tbInd  
*  @return   S16
*      -# ROK
**/

#ifdef ANSI
PUBLIC Void RgLaaPrgSndTbInd 
(
Pst            *pst,
RgLaaPrgTbInd  *tbInd
)
#else
PUBLIC Void RgLaaPrgSndTbInd(pst,tbInd)
Pst            *pst;
RgLaaPrgTbInd  *tbInd;
#endif
{
   TRC3(RgLaaPrgSndTbInd);

   (*RgLaaPrgSndTbIndMt[0])(pst, tbInd);
}
#endif /* LTE_ADV */

/**********************************************************************
 
         End of file:     gk_prg_pt.c
 
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
/main/1      ---     mpatel                1. CA release
*********************************************************************91*/
