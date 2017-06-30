


/**********************************************************************
 
    Name:  
 
    Type:   C include file
 
    Desc:   
 
    File:  l2_tenb_stats.c
 
    Sid:   l2_tenb_stats.c@@/main/tenb_5.0_RIB/3 - Tue Oct 27 14:21:16 2015
 
    Prg:   sm
 
**********************************************************************/
 
/** @file pj_tenb_stats.c
@brief This file contains 
*/

/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* RLC error options */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_ul.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_ul.x"
#ifdef TENB_STATS
#include "l2_tenb_stats.x"    /* Total EnodeB Stats declarations */
#endif

#ifdef TENB_STATS
PUBLIC TSL2CellStatsCb* l2CellStats[L2_STATS_MAX_CELLS];
PUBLIC TSL2UeStatsCb*   l2UeStats[L2_STATS_MAX_UES];
PUBLIC CmLListCp        freeL2UeStatsLst; /*!< Free Pool of UE stats Blocks */
PUBLIC CmLListCp        inUseL2UeStatsLst;/*!< In Use Pool of UE stats Blocks */

/*
*
*       Fun:   TSL2AllocStatsMem
*
*       Desc:  Pre-Allocate Memory for L2 stats BLOCKs 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC Void TSL2AllocStatsMem 
(
 Region region,
 Pool   pool 
)
#else
PUBLIC Void TSL2AllocStatsMem(region, pool)
 Region region;
 Pool   pool;
#endif
{
   U32 cnt=0;

   TRC2(TSL2AllocStatsMem)

   cmLListInit(&inUseL2UeStatsLst);     
   cmLListInit(&freeL2UeStatsLst);      
   for (cnt=0; cnt < L2_STATS_MAX_CELLS; cnt++)
   {
      if (SGetSBuf(region, pool, (Data **)&l2CellStats[cnt],
               (Size)sizeof (TSL2CellStatsCb)) != ROK)
      {
         printf("\n STATS Unexpected MEM Alloc Failure\n");
      }
      cmMemset((U8 *)l2CellStats[cnt], 0x00, (Size)sizeof(TSL2CellStatsCb));
   }

   for (cnt=0; cnt < L2_STATS_MAX_UES; cnt++)
   {
      TSL2UeStatsCb *statsCb = NULLP;
      if (SGetSBuf(region, pool, (Data **)&statsCb,
               (Size)sizeof (TSL2UeStatsCb)) != ROK)
      {
         printf("\n STATS Unexpected MEM Alloc Failure at %d\n", (int)cnt);
      }
      cmMemset((U8 *)statsCb, 0x00, (Size)sizeof(TSL2UeStatsCb));
      statsCb->lnk.node = (PTR)statsCb;
      cmLListAdd2Tail(&freeL2UeStatsLst, &statsCb->lnk);
      l2UeStats[cnt] = statsCb;
   }

   RETVOID;
}

/*
*
*       Fun:   TSL2AllocUeStatsBlk
*
*       Desc:  Assign Stats Block for this UE[RNTI] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC TSL2UeStatsCb* TSL2AllocUeStatsBlk 
(
 U16   rnti
)
#else
PUBLIC TSL2UeStatsCb* TSL2AllocUeStatsBlk(rnti)
 U16   rnti;
#endif
{
   CmLList          *tmp = NULLP;
   TSL2UeStatsCb  *statsCb = NULLP;

   TRC2(TSL2AllocUeStatsBlk)

   tmp = freeL2UeStatsLst.first;
   if (tmp == NULLP)
   {
      printf("\n STATS Unexpected Mem BLK unavailable for UE %d\n", rnti);
   }
   cmLListDelFrm(&freeL2UeStatsLst, tmp);
   statsCb = (TSL2UeStatsCb *)(tmp->node);
   cmLListAdd2Tail(&inUseL2UeStatsLst, tmp);

   statsCb->stats.rnti = (U32)rnti;
   statsCb->inUse = TRUE;

   RETVALUE(statsCb);
}

/*
*
*       Fun:   TSL2DeallocUeStatsBlk
*
*       Desc:  Deassign Stats Block for this UE[RNTI] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC Void TSL2DeallocUeStatsBlk 
(
 U16              rnti,
 TSL2UeStatsCb  *statsCb
)
#else
PUBLIC Void TSL2DeallocUeStatsBlk(rnti, statsCb)
 U16              rnti;
 TSL2UeStatsCb  *statsCb;
#endif
{
   TRC2(TSL2DeallocUeStatsBlk)

   statsCb->inUse = FALSE;
   cmLListDelFrm(&inUseL2UeStatsLst, &statsCb->lnk);
   freeL2UeStatsLst.crnt = freeL2UeStatsLst.first;
   cmLListInsAfterCrnt(&freeL2UeStatsLst, &statsCb->lnk);

   RETVOID;
}

/*
*
*       Fun:   TSL2AllocCellStatsBlk
*
*       Desc:  Assign Stats Block for this CELL[CELLID] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC TSL2CellStatsCb* TSL2AllocCellStatsBlk 
(
 U32 cellId
)
#else
PUBLIC TSL2CellStatsCb* TSL2AllocCellStatsBlk(cellId)
 U32 cellId;
#endif
{
   TRC2(TSL2AllocCellStatsBlk)

   if (cellId != 1)
   {
      printf("\n STATS Unexpected CellID = %d\n", (int)cellId);
   }

   RETVALUE(l2CellStats[cellId-1]);
}

/*
*
*       Fun:   TSL2DeallocCellStatsBlk
*
*       Desc:  Deassign Stats Block for this CELL[CELLID] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC Void TSL2DeallocCellStatsBlk 
(
 U32 cellId
)
#else
PUBLIC Void TSL2DeallocCellStatsBlk(cellId)
 U32 cellId;
#endif
{
   TRC2(TSL2DeallocCellStatsBlk)

   RETVOID;
}

/*
*
*       Fun:   TSL2SendStatsToApp
*
*       Desc:  Collates and Sends STATS to Application 
*              Send UE STATS first. 10 UEs are grouped in one message.
*              Followed by CELL Stats. All CELLS are grouped in one msg.
*              At Reception of CELL stats APP assumes STATS reception cycle is complete.
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC Void TSL2SendStatsToApp
(
 Pst    *pst,
 SuId   suId
 )
#else
PUBLIC Void TSL2SendStatsToApp(pst, suId)
 Pst    *pst;
 SuId   suId;
#endif
{
   U32 idx;

   TRC2(TSL2SendStatsToApp)

   for (idx = 0; idx < L2_STATS_MAX_UES; idx++)
   {
      TSL2UeStatsCb *statsCb = l2UeStats[idx];
      U32 rnti;
      if (statsCb->inUse != TRUE)
      {
         continue;
      }
      if (pst->selector == 0)
      {
         /* Loose Coupling */
         TSInfPkSndL2UeStats(pst, suId, &statsCb->stats);
      }
      else
      {
#ifdef PX
         /* Tight Coupling */
         TSInfHdlL2UeStatsInd(pst, suId, &statsCb->stats);
#endif
      }
      rnti = statsCb->stats.rnti;
      cmMemset((U8 *)&statsCb->stats.nonPersistent, 0x00, (Size)sizeof(statsCb->stats.nonPersistent));
      /* cmMemset((U8 *)&statsCb->stats, 0x00, (Size)sizeof(TSInfL2UeStats)); */
      statsCb->stats.rnti = rnti;
   }

   /* Allocate mBuf for CELLSTATS */
   for (idx = 0; idx < L2_STATS_MAX_CELLS; idx++)
   {
      TSL2CellStatsCb *statsCellCb = l2CellStats[idx];
      U32 cellId;
      if (pst->selector == 0)
      {
         /* Loose Coupling */
         TSInfPkSndL2CellStats(pst, suId, l2CellStats[idx]);
      }
      else
      {
#ifdef PX
         /* Tight Coupling */
         TSInfHdlL2CellStatsInd(pst, suId, l2CellStats[idx]);
#endif
      }
      cellId = statsCellCb->cellId;
      cmMemset((U8 *)l2CellStats[idx], 0x00, (Size)sizeof(TSInfL2CellStats));
      statsCellCb->cellId = cellId;
   }
   RETVOID;
}
#endif /* TENB_STATS */
/**********************************************************************
         End of file:     pj_tenb_stats.c@@/main/tenb_5.0_RIB/3 - Tue Oct 27 14:21:16 2015
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------

*********************************************************************91*/
