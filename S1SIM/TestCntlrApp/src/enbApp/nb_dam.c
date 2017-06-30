/**********************************************************************

    Name:  LTE S1SIM - ENODEB Application Module 

    Type:  C include file
               
    Desc:  C source code for ENODEB Application

     File:     nb_dam.c

     Sid:     

     Prg:      

**********************************************************************/

#include "nb.h"
#include "rl_interface.h"
#include "rl_common.h"
#include "nb_lnb.h"
#include "nb_dam.h"
#include "nb_ifm_egtp.h"
#include "nb_ifm_dam.h"
#include "nb_smm_init.h" 
#include "nb_dam_ifm_app.h" 
#include "nb_utils.h"
#include "nb_log.h"
#include "nb_traffic_handler.x" 

EXTERN S16 cmPkUeDelCfm(Pst*, U8);
PRIVATE S16 nbDamLSapCfg(LnbMngmt *cfg, CmStatus *status);
PRIVATE S16 nbDamLSapCntrl(LnbCntrl *sapCntrl,CmStatus *status,Elmnt elmnt);
PRIVATE S16 nbDamBndLSap (NbLiSapCb *sapCb,CmStatus  *status,Elmnt elmnt);
PRIVATE S16 nbDamUbndLSap (NbLiSapCb  *sapCb);
PRIVATE  NbDamUeCb *nbDamGetueCbkeyUeIp(U32 ueIpAddr,U8 *drbId);
PUBLIC  NbDamUeCb *nbDamGetUe(U8 ueId);
PUBLIC S16 nbDamEgtpDatInd(Pst*, EgtUEvnt*);

/* Some basic default values used for E-GTP Tunnel Management*/
/* Default Interface type while adding the tunnel at E-GTP*/
#define EGU_DFLT_INTFTYPE    0
/* Default expected sequence number for E-GTP tunnel used for initilization*/
#define EGU_DFLT_EXPSEQNMB   1000
/* Default Maximum Transmit Pdu Size*/
#define EGU_DFLT_MAXTPDUSZ   5000
/* Default EGTP Message size */
#define NB_DAM_EGTP_MSG_SZ   1024
/* Default Interface type while adding the tunnel at GTP*/
#define NB_DAM_MAX_DRB_TNLS  3

PUBLIC NbDamCb   nbDamCb;
/** @brief This function is responsible for starting the inactivity timer.
 *
 * @details This function starts the inactivity timer with the provided ueCb.
 *
 *     Function: wrDamStartUeInactvTmr
 *
 *         Processing steps:
 *         - use the ueCb and call the wrStartTmr primitive to start the timer.
 *
 *
 * @param[in]  ueCb : UE CB for which the timer is started.
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 nbDamStartUeInactvTmr
(
 NbDamUeCb *ueCb
)
{
   NB_LOG_DEBUG(&nbCb,"Inactivity timer has been started");
   nbDamStartTmr((PTR)ueCb, NB_TMR_INACTIVITY, nbDamCb.inactvTmrVal);
   RETVALUE(ROK);
}
/** @brief This function is responsible for filling and requesting the EGTP
 *         to create a new tunnel with the provided id.
 *
 * @details
 *
 *     Function: nbDamAddTunnelAtGtp
 *
 *         Processing steps:
 *         - allocate memory for the event data structure (EgtUEvnt)
 *         - fill the EgtUEvnt structure for creating a new tunnel using 
 *           the values from tnl parameter passed to this function
 *
 *
 * @param[in]  tnl: tunnel info for creating new tunnel
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 nbDamAddTunnelAtGtp
(
NbDamTnlCb                   *tnlCb
)
{
   EgtUEvnt                  *eguEvtMsg;

   /*****************************************************************
    * allocate memory for the e-gtpu event message of size EgtUEvnt *
    *****************************************************************/
   NB_ALLOC_DATA_APP(&eguEvtMsg, sizeof(EgtUEvnt));
   if (eguEvtMsg == NULLP)
   {
      NB_LOG_ERROR(&nbCb,"Allocation of memory failed.");
      RETVALUE(RFAILED);
   }

   eguEvtMsg->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_ADD;

   /* Currently default values are filled */
   eguEvtMsg->u.tnlMgmt.intfType = EGU_DFLT_INTFTYPE;
   eguEvtMsg->u.tnlMgmt.reOrderPres = FALSE;
   eguEvtMsg->u.tnlMgmt.expSeqNmb = EGU_DFLT_EXPSEQNMB;
   eguEvtMsg->u.tnlMgmt.maxTPduSize = EGU_DFLT_MAXTPDUSZ;


   eguEvtMsg->u.tnlMgmt.teId = tnlCb->locTeId;
   eguEvtMsg->u.tnlMgmt.reordAlgo = EG_REORDER_TYPE_1;
   eguEvtMsg->u.tnlMgmt.tunnelType = EG_NORMAL_DATA_TUNN;
   eguEvtMsg->u.tnlMgmt.remTeId = tnlCb->remTeid;
   nbCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.srcAddr), &(tnlCb->lclAddr));
   nbCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.dstIpAddr), &(tnlCb->dstAddr));

   eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;
   /* ccpu00125004: Trigger primitive */
   if(ROK != NbIfmEgtpEguLclTnlMgmtReq(eguEvtMsg))
   {
	   NB_FREE_DATA_APP(eguEvtMsg, sizeof(EgtUEvnt));
   }

   RETVALUE(ROK);
}


/** @brief This function is responsible for filling and requesting the EGTP
 *         to delete an existing tunnel with the provided id.
 *
 * @details 
 *
 *     Function: nbDamDelTunnelAtGtp
 *
 *         Processing steps:
 *         - allocate memory for the event data structure (EgtUEvnt)
 *         - fill the EgtUEvnt structure for deleting an existing tunnel using 
 *           the values from tnl parameter passed to this function
 *
 *
 * @param[in]  tnl: tunnel info for deleting a tunnel
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 nbDamDelTunnelAtGtp
(
NbDamTnlCb                   *tnlCb
)
{
   EgtUEvnt    *eguEvtMsg = NULLP;

   /*****************************************************************
    * allocate memory for the e-gtpu event message of size EgtUEvnt *
    *****************************************************************/
   NB_ALLOC_DATA_APP(&eguEvtMsg, sizeof(EgtUEvnt));
   if (eguEvtMsg == NULLP)
   {
      NB_LOG_ERROR(&nbCb,"Allocation of memory failed.");
      RETVALUE(RFAILED);
   }

   eguEvtMsg->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_DEL;
   eguEvtMsg->u.tnlMgmt.teId   = tnlCb->locTeId;
 
   nbCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.srcAddr), &(tnlCb->lclAddr));
   nbCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.dstIpAddr), &(tnlCb->dstAddr));
   /* ccpu00125004 Trigger primitive */
   if(ROK!=NbIfmEgtpEguLclTnlMgmtReq(eguEvtMsg))
   {
      NB_FREE_DATA_APP(eguEvtMsg, sizeof(EgtUEvnt));
   }


   RETVALUE(ROK);
}
/** @brief This function is responsible for forwarding the tunnel management
 *         confirms (add/delete/modify) to the application.
 *
 * @details
 *
 *     Function: nbDamTnlMgmtCfm
 *
 *         Processing steps:
 *         - derive the cellId, rnti and rbId from the teId sent as part of the
 *           eguMsg
 *         - fetch the tunnelCb corresponding to these values
 *         - invoke the IFM primitive to send the confirm to application
 *         - free the eguMsg
 *
 * @param[in]  eguMsg : management info regarding the tunnel
 * @return void 
 */
PUBLIC Void nbDamTnlMgmtCfm
(
 EgtUEvnt                     *eguMsg
)
{
   NbDamTnlCb                *tnlCb = NULLP;
   U8                        ueId;
   U8                        rbId;
   NbDamUeCb                 *ueCb;
   NbDamDrbCb                *drbCb;

   ueId   = (eguMsg->u.tnlMgmt.teId & 0x00ffff00) >> 8;
   rbId   = (eguMsg->u.tnlMgmt.teId & 0x000000f8) >> 3;

   ueCb = nbDamGetUe(ueId);

   if (ueCb == NULLP)
   {
       NB_LOG_ERROR(&nbCb,"Invalid UE IDX [tnlMgmtCfm : %d]",EGT_TMGMT_REQTYPE_MOD);
       RETVOID;
   }
   if(eguMsg->u.tnlMgmt.action != EGT_TMGMT_REQTYPE_DEL)
   {
      if ( ROK != (cmHashListFind(&(ueCb->drbs), (U8 *)&(rbId),
                  sizeof(U8),0,(PTR *)&drbCb)))
      {   
         RETVOID;
      }
      else
      {
         tnlCb = drbCb->tnlInfo;
      }

      if (tnlCb == NULLP)
      {
         NB_LOG_ERROR(&nbCb,"Invalid Tunnel Info [tnlMgmtCfm : %d] [rbId : %d] "\
               "%d]", EGT_TMGMT_REQTYPE_MOD, rbId);
         RETVOID;
      }
   }
   switch(eguMsg->u.tnlMgmt.action)
   {
      case EGT_TMGMT_REQTYPE_ADD:
         {
            if (eguMsg->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK)
            {
               /* Confirm creation of tunnel to the UMM module */
               nbIfmDamNbTnlCreatCfm(NB_DAM_CFG_OK, tnlCb->locTeId);
            }
            else
            {
               nbIfmDamNbTnlCreatCfm(NB_DAM_CFG_FAIL, tnlCb->locTeId);
            }
            break;
         }
      case EGT_TMGMT_REQTYPE_DEL:
      {
         if (eguMsg->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK)
         {
            /* Confirm Deletion of tunnel to the UMM module */
            nbIfmDamNbTnlDelCfm(NB_DAM_CFG_OK, eguMsg->u.tnlMgmt.teId);
         }
         else
         {
            nbIfmDamNbTnlDelCfm(NB_DAM_CFG_FAIL, eguMsg->u.tnlMgmt.teId);
         }
         break;
      }
      default:
      {
         NB_LOG_ERROR(&nbCb,"Tnl Mgmt Unhandled Action :%x", eguMsg->u.tnlMgmt.action);
         break;
      }
   }
}/* nbDamTnlMgmtCfm */

/** @brief This function is responsible for deleting a UE
 *
 * @details
 *
 *     Function: nbDamDelUe
 *
 *         Processing steps:
 *         - get the cellCb corresponding to cellCb
 *         - validate the rnti
 *         - free the ue index
 *         - free the ueCb
 *
 * @param[in]  cellId    : unique cellID
 * @param[in]  rnti      : UE ID
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 nbDamDelUe
(
 U8 ueId 
)
{
   S16 ret = ROK;
   NbDamUeCb *ueCb = NULLP;
   NbIpInfo *ipInfo = NULLP;
   NbIpInfo *prevIpInfo = NULLP;
#if 0
   U8 idx;
   for(idx = 0 ; idx < nbDamCb.crntUeIdx; idx++)
   {
      if((nbDamCb.ueCbs[idx] != NULLP) && (nbDamCb.ueCbs[idx]->ueId == ueId))
      {
         ueCb = nbDamCb.ueCbs[idx];
         if(ueCb->numDrbs == 0)
         {
            NB_FREE(ueCb, sizeof(NbDamUeCb));
            nbDamCb.ueCbs[idx] = NULLP;
            nbDamCb.crntUeIdx--;
         }
         else
         {
            /* tunnel deletion is in progress */
           ret = RFAILED; 
         }
         break;
      } 
   }
#endif
   if(ROK != (cmHashListFind(&(nbDamCb.ueCbs), (U8 *)&(ueId),
      sizeof(U8), 0, (PTR *)&ueCb)))
   {   
      ret = RFAILED;
   }
   else 
   {
      if(ueCb->numDrbs == 0)
      {
         /* deinit tunnel and drb list */
         cmHashListDeinit(&(ueCb->drbs));
         for(; cmHashListGetNext(&ueCb->ipInfo, (PTR)prevIpInfo, (PTR*)&ipInfo) == ROK;)
         {
            cmHashListDelete(&(ueCb->ipInfo), (PTR)ipInfo);
            NB_FREE(ipInfo, sizeof(NbIpInfo));
         }
         cmHashListDeinit(&(ueCb->ipInfo));
         cmHashListDelete(&(nbDamCb.ueCbs), (PTR)ueCb);
         NB_FREE(ueCb, sizeof(NbDamUeCb));
      }
      else
      {
         /* tunnel deletion is in progress */
         ret = RFAILED;
      }
   }

   RETVALUE(ret);
}

/** @brief This function is responsible for adding a new UE CB
 *
 * @details
 *
 *     Function: nbDamAddUe
 *
 *         Processing steps:
 *         - get the cell corresponding to the cellId
 *         - validate the rnti
 *         - allocate a new ueCb
 *         - initilize the ueCb with default values
 *         - initilize inactivity timer
 *         - start inactivity timer
 *
 * @param[in] cellId  : unique cell id
 * @param[in] rnti    : UE ID
 * @param[in] ueCb    : UE Control Block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 nbDamAddUe
(
NbDamUeCb                    **ueCb,
U8                           ueId
)
{
   NbDamDrbCb tmpDamDrbCb;
   NbIpInfo tmpDamIpInfo;
   U8 offset  = 0;
   NB_ALLOC(ueCb, sizeof(NbDamUeCb ));
   if ((*ueCb) == NULLP)
   {
      RETVALUE(RFAILED);
   }
   (*ueCb)->ueId       = ueId;
   (*ueCb)->numDrbs    = 0;
   (*ueCb)->numTunnels = 0;

   offset = (U8) ((PTR)&tmpDamDrbCb.ueHashEnt - (PTR)&tmpDamDrbCb);
   if( ROK != (cmHashListInit(&((*ueCb)->drbs), /* messages */
                        NB_MAX_HASH_SIZE,     /* HL bins for the msgs */
                        offset,               /* Offset of HL Entry */
                        FALSE,                /* Allow dup. keys ? */
                        CM_HASH_KEYTYPE_ANY,  /* HL key type */
                        nbCb.init.region,     /* Mem region for HL */
                        nbCb.init.pool)))      /* Mem pool for HL */
   {
      NB_LOG_ERROR(&nbCb,"Failed to initialized DamDrbCb List");
      NB_FREE(ueCb, sizeof(NbDamUeCb))
      RETVALUE(RFAILED);

   }
   offset = (U8) ((PTR)&tmpDamIpInfo.ueHashEnt - (PTR)&tmpDamIpInfo);
   if( ROK != (cmHashListInit(&((*ueCb)->ipInfo), /* messages */
                        NB_MAX_HASH_SIZE,     /* HL bins for the msgs */
                        offset,               /* Offset of HL Entry */
                        TRUE,                /* Allow dup. keys ? */
                        CM_HASH_KEYTYPE_ANY,  /* HL key type */
                        nbCb.init.region,     /* Mem region for HL */
                        nbCb.init.pool)))      /* Mem pool for HL */
   {
      NB_LOG_ERROR(&nbCb,"Failed to initialized IpInfo List");
      NB_FREE(*ueCb, sizeof(NbDamUeCb))
      RETVALUE(RFAILED);
   }
   if (ROK != cmHashListInsert(&(nbDamCb.ueCbs),(PTR)*(ueCb),
                     (U8 *) &(*ueCb)->ueId,sizeof(U8)))
   {   
      /* deinit the ip list and drblist */
      NB_FREE(*ueCb, sizeof(NbDamUeCb))
      NB_LOG_ERROR(&nbCb, "Failed to Insert UE into UeDamCbLst");
      RETVALUE(RFAILED);
   }   

   cmInitTimers(&(*ueCb)->inactivityTmr, 1);
   nbDamStartUeInactvTmr(*ueCb);
   RETVALUE(ROK);
}

/** @brief This function is reponsible to add a new tunnel to DAM for a 
 *         particular UE.
 *
 * @details
 *
 *     Function: nbDamAddTunnel
 *
 *         Processing steps:
 *         - fetch the cellCb corresponding to the cellId
 *         - validate that the rnti is within the configured range
 *         - validate that the rbId is within the range
 *         - if a ueCb does not exist (i.e, this is the first tunnel getting
 *           created for a UE),
 *           - then derive the ueIdx, create and initialize a new ueCb
 *           - start the ue inactivity timer
 *         - add the tunnel info in the ueCb and initialize the tunnel info
 *         - invoke nbDamAddTunnelAtGtp to send request to egtp to create a 
 *           new tunnel
 *
 *
 * @param[in]  transId : unique transaction id
 * @param[in]  tnlInfo : new tunnel info
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 nbDamAddTunnel
(
NbDamTnlInfo                 *tnlInfo
)
{
   U8 ueId;
   NbDamUeCb       *ueCb = NULLP;
   NbDamTnlCb      *tnlCb  = NULLP;
   U8              rbId    = tnlInfo->tnlId.drbId;
   NbDamTnlType    tnlType = tnlInfo->tnlType;
   NbDamDrbCb      *drbCb  = NULLP;
   NbIpInfo        *ipInfo = NULLP;

   if (rbId >= NB_DAM_MAX_DRBS)
   {
      RETVALUE(RFAILED);
   }

   ueId = (tnlInfo->lclTeid & 0x00ffff00) >>8;
   ueCb = nbDamGetUe(ueId);

   if (tnlType == NB_TNL_NORMAL)
   {
       if(ueCb == NULLP)
       {
          if (nbDamAddUe(&ueCb,ueId)!=ROK)
          {
             RETVALUE(RFAILED);
          }
       }
       if ( ROK != (cmHashListFind(&((ueCb)->drbs), (U8 *)&(rbId),
                   sizeof(U8),0,(PTR *)&drbCb)))
       {
          (ueCb)->numDrbs++;
          /* Create the tunnel i*/
          NB_ALLOC_DATA_APP(&(tnlCb), sizeof(NbDamTnlCb));
          NB_ALLOC_DATA_APP(&(drbCb), sizeof(NbDamDrbCb));
          if(drbCb == NULLP)
          {
             NB_LOG_ERROR(&nbCb, "Failed to allocate memory");
             RETVALUE(RFAILED);
          }
          drbCb->tnlInfo = tnlCb;
          drbCb->drbId = rbId;
          if (ROK != cmHashListInsert(&((ueCb)->drbs), (PTR)drbCb,
                     (U8 *)&drbCb->drbId, sizeof(U8)))
          {
               RETVALUE(RFAILED);
          }
       }
       else
       {
            RETVALUE(RFAILED);
       }

   }/* WR_TNL_NORMAL */
   else
   {
       NB_LOG_ERROR(&nbCb,"Invalid Tunnel type received.");
       RETVALUE(RFAILED);
   }
   if(NULLP == tnlCb)
   {
       NB_LOG_ERROR(&nbCb,"Memory allocation failed.");
       if ( ROK != (cmHashListFind(&((ueCb)->drbs), (U8 *)&(rbId),
                   sizeof(U8),0,(PTR *)&drbCb)))
       {
            RETVALUE(RFAILED);
       }
       else
       {
            NB_FREE_DATA_APP(drbCb, sizeof(*drbCb));
       }
       RETVALUE(RFAILED);
   }

   /* Create the tunnel */
   NB_ALLOC(&(ipInfo), sizeof(NbIpInfo));
   ipInfo->pdnAddr = tnlInfo->pdnAddr;
   ipInfo->drbId = rbId;
   if (ROK != cmHashListInsert(&(ueCb->ipInfo), (PTR)ipInfo,
                     (U8 *)&ipInfo->pdnAddr, sizeof(U32)))
   {   
      NB_FREE(ueCb, sizeof(NbIpInfo))
      NB_LOG_ERROR(&nbCb, "Failed to Insert ip address into ipInfo");
      RETVALUE(RFAILED);
   }

   tnlCb->locTeId = tnlInfo->lclTeid; 
   tnlCb->remTeid = tnlInfo->remTeid;
   nbCpyCmTptAddr(&(tnlCb->dstAddr), &tnlInfo->dstAddr);
   nbCpyCmTptAddr(&(tnlCb->lclAddr), &tnlInfo->srcAddr);

   /* Invoke GTP to add the tunnel here */
   nbDamAddTunnelAtGtp(tnlCb);
   RETVALUE(ROK);
}

/** @brief This function handles the incoming tunnel create request.
 *
 * @details
 *
 *     Function: nbDamTnlCreatReq
 *
 *         Processing steps:
 *         - invoke wrDamAddTunel to create a new tunnel
 *         - send the tunnel create confirm to application
 *
 *
 * @param[in]  tnlInfo : new tunnel info
 * @return S16
 */
PUBLIC Void  nbDamTnlCreatReq
(
 NbDamTnlInfo                 *tnlInfo
)
{
  
   if(nbDamAddTunnel(tnlInfo) != ROK)
   {
      /* Send failure back to the calling module. The last parameter      */
      /* as 0 should be ignored by the receiving module as status is      */
      /* marked as FAILURE                                                */
      nbIfmDamNbTnlCreatCfm(NB_DAM_CFG_FAIL, 0);
   }
   NB_FREE_DATA_APP_SHAREBLE_BUF(tnlInfo, sizeof(NbDamTnlInfo));
}

/** @brief This function is responsible for deleting the DRB of ueCb
 *
 * @details
 *
 *     Function: nbDamDelDrbCb
 *
 *         Processing steps:
 *         - Get Tunnel Cb from DrbCb
 *         - Delete the tunnel at GTP
 *
 *
 * @param[in]  ueCb   : Ue Control Block
 * @param[in]  drbCb  : DRB Control Block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 nbDamDelDrbCb
(
NbDamUeCb                   *ueCb,
NbDamDrbCb                  *drbCb
)
{
   NbDamTnlCb *tnlCb = NULLP;

   tnlCb = drbCb->tnlInfo;
   nbDamDelTunnelAtGtp(tnlCb);
   NB_FREE_DATA_APP(tnlCb, sizeof(NbDamTnlCb));

   RETVALUE(ROK);
}

/** @brief This function is responsible for filling the egtp event structure
 *         for sending data packet.
 *
 * @details
 *
 *     Function: nbFillEgtpDatMsg
 *
 *         Processing steps:
 *         - allocate a new EgtUEvnt
 *         - fill the src and dst addresses
 *         - fill the message header with default values and with the 
 *           tnl info
 *
 *
 * @param[in]   tnl       : tunnel info
 * @param[out]  eguEvtMsg : outgoing egtp message
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16  nbFillEgtpDatMsg
(
NbDamTnlCb                   *tnl,
EgtUEvnt                     **eguEvtMsg,
U8                           msgType
)
{
   EgUMsgHdr                 *eguHdr = NULLP;

   NB_ALLOC_DATA_APP(eguEvtMsg, sizeof(EgtUEvnt));
   if (*eguEvtMsg == NULLP)
   {
      NB_LOG_ERROR(&nbCb,"SGetSBuf  failed.");
      RETVALUE(RFAILED);
   }
   NB_ALLOCEVNT_DATA_APP(&((*eguEvtMsg)->u.egMsg), sizeof(EgUMsg));
   if ((*eguEvtMsg)->u.egMsg == NULLP)
   {
      NB_LOG_ERROR(&nbCb,"cmAllocEvnt failed.");
      RETVALUE(RFAILED);
   }

   /* Src & Dst Address */
   nbCpyCmTptAddr(&((*eguEvtMsg)->u.egMsg->srcAddr), &(tnl->lclAddr));
   nbCpyCmTptAddr(&((*eguEvtMsg)->u.egMsg->remAddr), &(tnl->dstAddr));
   (*eguEvtMsg)->u.egMsg->lclTeid = tnl->locTeId;

   /* Populating Header Info */
   eguHdr = &(*eguEvtMsg)->u.egMsg->msgHdr;
   eguHdr->msgType = msgType;
   eguHdr->nPduNmb.pres = FALSE;
   eguHdr->seqNumber.pres = FALSE;
   eguHdr->extHdr.udpPort.pres = FALSE;
   eguHdr->extHdr.pdcpNmb.pres = FALSE;
   eguHdr->teId = tnl->remTeid;

   RETVALUE(ROK);
}

/** @brief This function handles the incoming Uplink PCAP data indication.
 *
 * @details
 *
 *     Function: nbDamPcapDatInd
 *
 *         Processing steps:
 *         - fetch the tunnel Cb
 *         - fetch the ueCb
 *
 *
 * @param[in]  mBuf   : uplink data packet
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 nbDamPcapDatInd
(
 Buffer                       *mBuf
)
{
   U32                       ueIpAddr;
   MsgLen                    len = 0;
   NbDamTnlCb                *tnl;
   NbDamUeCb                 *ueCb = NULLP;
   U8  ipPkt[4] = {0}; 
   U8  idx = 0; 
   U8 drbId; 
   MsgLen ipIdx = 12;

   EgtUEvnt                  *eguEvtMsg;
   EgUMsg                    *egMsg;
   NbDamDrbCb                *drbCb;

   SFndLenMsg(mBuf, &len);
   if(len == 0)
   {
      NB_LOG_ERROR(&nbCb,"Empty Buffer recieved");
      SPutMsg(mBuf);
      RETVALUE(ROK);
   }

   for(idx = 0; idx < 4; idx++)
   {
      if((SExamMsg(&ipPkt[idx], mBuf, ipIdx) != ROK))
      {    
         NB_LOG_ERROR(&nbCb,"Failed to get message type");
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      ipIdx++;
   }
   ueIpAddr = (ipPkt[0] << 24) + (ipPkt[1] << 16) +
      (ipPkt[2] << 8 ) + ipPkt[3];

   /* get the ueCb */
   ueCb = nbDamGetueCbkeyUeIp(ueIpAddr, &drbId);
   if(ueCb == NULLP)
   {
      NB_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* mark for ue data received */
   ueCb->dataRcvd = TRUE;   
   /* get the tunnel Cb */
   if ( ROK != (cmHashListFind(&(ueCb->drbs), (U8 *)&(drbId),
               sizeof(U8), 0, (PTR *)&drbCb)))
   {    
      RETVALUE(RFAILED);
   }    
   else 
   {    
      tnl = drbCb->tnlInfo;   
   }    

   if(ROK != nbFillEgtpDatMsg(tnl, &eguEvtMsg, EGT_GTPU_MSG_GPDU))
   {
      NB_FREEMBUF(mBuf);
      RETVALUE(ROK);
   }

   egMsg = eguEvtMsg->u.egMsg;
   egMsg->u.mBuf = mBuf;

   /* Trigger EGTP Data Req */
   NbIfmEgtpEguDatReq(eguEvtMsg);

   RETVALUE(ROK);
}

/** @brief This function handles the incoming EGTP data packets and forwards
 *         them to PDCP.
 *
 * @details
 *
 *     Function: nbDamEgtpDatInd
 *
 *         Processing steps:
 *         - derive the cellId, ueId, rbId from the teId of the incoming 
 *           message and fill the CmLtePdcpId structure
 *         - get the tunnelCb
 *         - get the ueCb
 *         - set the dataRcvd flag to TRUE
 *         - invoke WrIfmPdcpDatReq to send the packet to PDCP.
 *
 *
 * @param[in]  eguMsg : incoming egtp message
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC S16 nbDamEgtpDatInd
(
Pst                          *pst,
EgtUEvnt                     *eguMsg
)
{
   U32                       lclTeid = eguMsg->u.egMsg->msgHdr.teId;
   NbDamTnlCb                *tnlCb = NULLP;
   NbDamUeCb                 *ueCb = NULLP;
   NbDamDrbCb                *drbCb = NULLP;
   U16                       ueId;
   CmLteRbId                 rbId;        /*!< PDCP Instance ID */

   MsgLen                    len       = 0;
   MsgLen                    cnt       = 0;
   Data                      *flatBuf  = NULLP;
   S16 ret;

   ueId  = (lclTeid & 0x00ffff00) >> 8;
   rbId   = (lclTeid & 0x000000f8) >> 3;

   /* get the ueCb */ 
   ueCb  = nbDamGetUe(ueId);
   if (ueCb == NULLP)
   {
      NB_LOG_ERROR(&nbCb,"Error in getting ueCb for "
            "ueId = %u", ueId);
      RETVALUE(RFAILED);
   }
   /* mark for ue data received */
   ueCb->dataRcvd = TRUE;   

   if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_SUPP_EXT_HDR_NTF)
   {
      NB_LOG_ERROR(&nbCb,"Received EGTP HDR NOTIFY");
      RETVALUE(RFAILED);
   }
   if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_ERROR_IND)
   {
      NB_LOG_ERROR(&nbCb,"EGTP ERROR IND:Received");

      RETVALUE(RFAILED);
   }

   if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_ECHO_RSP)
   {
      NB_LOG_ERROR(&nbCb,"Received EGTP ECHO RSP ");
      RETVALUE(RFAILED);
   }

#if 0
   drbCb = ueCb->drbs[rbId];
#else
   if ( ROK != (cmHashListFind(&(ueCb->drbs), (U8 *)&(rbId),
               sizeof(U8),0,(PTR *)&drbCb)))
   {    
      RETVALUE(RFAILED);
   }    
#endif
   if(drbCb)
   {
      tnlCb = drbCb->tnlInfo;
   }
   if( tnlCb == NULLP )
   {
      NB_LOG_ERROR(&nbCb,"tnlCb not found");
      RETVALUE(RFAILED);
   }

   /* copy the data  buffer*/
   ret = SFndLenMsg(eguMsg->u.egMsg->u.mBuf, &len);
   if((len == 0) || (ret != ROK))
   {
      NB_LOG_ERROR(&nbCb,"In correct buffer len [%d]", len);
      RETVALUE(RFAILED);
   } 

   NB_ALLOC(&flatBuf, len);
   if (flatBuf == NULLP)
   {
      NB_LOG_ERROR(&nbCb,"Failed to allocate memory for flatBuf");
      RETVALUE(RFAILED);
   }
   /* Convert the mBuf to U8 array */
   SCpyMsgFix(eguMsg->u.egMsg->u.mBuf, 0, len, flatBuf, &cnt); 
   /* send the downlink packet to pcap */
   nbAppFrwdIpPkt(flatBuf, cnt);
   NB_FREE(flatBuf, len);

   cmFreeMem(eguMsg->u.egMsg);
   NB_FREE_DATA_APP(eguMsg, sizeof(EgtUEvnt));
   RETVALUE(ROK);
}

/** @brief This function is resposible for deleting all the ue info for the 
 *         provided crnti.
 *
 * @details
 *
 *     Function: nbDamUeDelReq
 *
 *         Processing steps:
 *         - fetch the ueCb corresponding to the crnti
 *         - for all the established rbs in the ueCb send a tunnel delete
 *           request to egtp
 *         - send ue delete confirm to application
 *
 *
 * @param[in]  transId : unique transaction Id
 * @param[in]  cellId  : unique cell Id
 * @param[in]  crnti   : unique ue Id
 * @return S16
 *    -#Success : ROK
 */
PUBLIC Void nbDamUeDelReq
(
 U8  ueId
)
{
   NbDamUeCb *ueCb = NULLP;
   NbDamDrbCb *drbCb = NULLP;
   NbDamDrbCb *prevDrbCb = NULLP;
   S16 ret = ROK;
   U8 isEmpty = FALSE;
   Pst *pst;
   /* Delete all the tunnels that have been established for this UE */
   ueCb = nbDamGetUe(ueId);
   if (ueCb != NULLP)
   {
      /* Stop the Inactivity timer for this perticular UE */
      nbDamStopTmr((PTR)ueCb, NB_TMR_INACTIVITY);

      /* Invoke delete towards EGTP for all the tunnels but do not wait for */
      /* response. If any response is failure, raise a DEBUG ERROR */
      /* If the number of DRBs become zero remove the UeCb */
      if (ueCb->numDrbs == 0)
      {
         nbDamDelUe(ueId);
         RETVOID;
      }
      for(;!isEmpty && ((cmHashListGetNext(&(ueCb->drbs),(PTR)prevDrbCb,(PTR*)&drbCb)) == ROK);)
      {   
         ueCb->numDrbs--;
         ret  = cmHashListDelete(&(ueCb->drbs), (PTR)drbCb);
         if (ret == RFAILED)
         {
            NB_LOG_ERROR(&nbCb,"Failed to delete DrbCb");
         }
         if(ueCb->numDrbs == 0)
            isEmpty = TRUE;
         nbDamDelDrbCb(ueCb, drbCb);
         NB_FREE_DATA_APP(drbCb, sizeof(NbDamDrbCb)); 
         drbCb = NULLP;
      }
   }
   else  /* DAM-UE CB is not present */ 
   {
      pst = &nbDamCb.nbAppPst;
      cmPkUeDelCfm (pst, ueId);
      RETVOID;
   }
}

/** @brief This function is resposible for deleting all the Erab info for the particular ue. *
 * @details
 *
 *     Function: nbIfmDamNbErabDelReq
 *
 *         Processing steps:
 *         - fetch the ueCb corresponding to the crnti
 *         - for all the established rbs in the ueCb send a tunnel delete
 *           request to egtp
 *         - send ue delete confirm to application
 *
 *
 * @param[in]  transId : unique transaction Id
 * @param[in]  cellId  : unique cell Id
 * @param[in]  crnti   : unique ue Id
 * @return S16
 *    -#Success : ROK
 */
PUBLIC Void nbDamNbErabDelReq
(
 NbuErabRelIndList  *erabRelReq
)
{
   NbDamUeCb *ueCb = NULLP;
   NbDamDrbCb *drbCb = NULLP;
   U8 count = 0;
   S16 ret = ROK;
   Pst *pst;
   /* Get UE */
   ueCb = nbDamGetUe(erabRelReq->ueId);
   if (ueCb != NULLP)
   {
      /* Invoke delete towards EGTP for listed tunnels but do not wait for */
      /* response. If any response is failure, raise a DEBUG ERROR */
      for(count = 0; count < erabRelReq->numOfErabIds; count++)
      {
         if ( ROK != (cmHashListFind(&((ueCb)->drbs),
                     (U8 *)&(erabRelReq->erabIdLst[count]), sizeof(U8), 0,
                     (PTR *)&drbCb)))
         {
            NB_LOG_ERROR(&nbCb, "Failed to find DrbCb for drbId %d",
                  erabRelReq->erabIdLst[count]); 
            RETVOID;
         }
         ueCb->numDrbs--;
         ret  = cmHashListDelete(&(ueCb->drbs), (PTR)drbCb);
         if (ret == RFAILED)
         {
            NB_LOG_ERROR(&nbCb,"Failed to delete DrbCb");
         }
         nbDamDelDrbCb(ueCb, drbCb);
         /* NB_FREE_DATA_APP(drbCb, sizeof(NbDamDrbCb));*/ 
         drbCb = NULLP;
      }
   }
   else  /* DAM-UE CB is not present */
   {
      pst = &nbDamCb.nbAppPst;
      cmPkUeDelCfm(pst, erabRelReq->ueId);
      RETVOID;
   }
}

/** @brief This function is responsible for initializing the DAM.
 *
 * @details
 *
 *     Function: nbDamInit
 *
 *         Processing steps:
 *         - all the cells in nbDamCb are initialised to default value.
 *
 *
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 nbDamInit
(
 Void
)
{
   RETVALUE(ROK);
}

/*
 *  Fun:  nbDamActvInit 
 *
 *  Desc:    
 *
 *  Ret:  ROK   - ok
 *
 *  Notes :The task initializes its global variables.
 */
PUBLIC S16 nbDamActvInit
(
 Ent                          entity,
 Inst                         inst,
 Region                       region,
 Reason                       reason
)
{
   U8 offset = 0;
   NbDamUeCb nbDamUeCb;

   nbDamCb.init.procId  = SFndProcId();
   nbDamCb.init.ent     = entity;
   nbDamCb.init.inst    = inst;
   nbDamCb.init.region  = region;
   nbDamCb.init.reason  = reason;
   nbDamCb.init.cfgDone = FALSE;
   nbDamCb.init.pool    = 1;
   nbDamCb.init.acnt    = FALSE;
   nbDamCb.init.trc     = FALSE; 
   nbDamCb.init.usta    = TRUE;

   nbDamCb.nbAppPst.srcProcId = SFndProcId();
   nbDamCb.nbAppPst.srcEnt = ENTEU;
   nbDamCb.nbAppPst.srcInst = 0;
   nbDamCb.nbAppPst.dstProcId = SFndProcId();
   nbDamCb.nbAppPst.dstEnt = ENTNB;
   nbDamCb.nbAppPst.dstInst = 0;
   nbDamCb.nbAppPst.prior = 0;
   nbDamCb.nbAppPst.route = 0;
   nbDamCb.nbAppPst.selector = 0;
   nbDamCb.nbAppPst.region = region;
   offset = (U8) ((PTR)&nbDamUeCb.ueHashEnt - (PTR)&nbDamUeCb);
/* initialize the ueCbLst */
   if( ROK != (cmHashListInit(&(nbDamCb.ueCbs), /* messages */
                        NB_MAX_HASH_SIZE,     /* HL bins for the msgs */
                        offset,               /* Offset of HL Entry */
                        FALSE,                /* Allow dup. keys ? */
                        CM_HASH_KEYTYPE_ANY,  /* HL key type */
                        nbCb.init.region,     /* Mem region for HL */
                        nbCb.init.pool)))      /* Mem pool for HL */
   {
      NB_LOG_ERROR(&nbCb,"Failed to initialized DamCb List");
      RETVALUE(RFAILED);

   }
   
   RETVALUE(ROK);
} /*nbDamActvInit*/

/** @brief This function is responsible for configuring the wrDamCb with the 
 *         incoming configuration values.
 *
 * @details
 *
 *     Function: nbDamCfgReq
 *
 *         Processing steps:
 *         - update the existing values with the new values.
 *
 *
 * @param[in]  cfg : configuration values
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 nbDamCfgReq
(
 LnbMngmt      *cfg
)
{
   S16 retVal;
   CmStatus cfgCfm;


   cfgCfm.status = LCM_PRIM_OK;
   cfgCfm.reason = LCM_REASON_NOT_APPL;
   retVal = ROK;

   switch (cfg->hdr.elmId.elmnt)
   {
      case STNBGEN:
         {
            LnbGenCfg *genCfg = &cfg->t.cfg.t.genCfg;
            nbDamCb.inactvTmrVal = genCfg->inactvTmrVal;
            nbDamCb.endMrkTmrVal = genCfg->endMrkTmrVal;
            nbDamCb.expiryCnt    = genCfg->expiryCnt;
            nbDamCb.timeRes = genCfg->timeRes;
            /* Allocate memory */
            NB_ALLOC_DATA_APP(&nbDamCb.egtSap, (sizeof(NbLiSapCb *) * NB_NUM_EGT_SAPS));
            if(NULLP == nbDamCb.egtSap)
            {
               NB_LOG_ERROR(&nbCb,"Failed to allocate Memory.");
               retVal = RFAILED;
               break;
            }
            nbDamRegInitTmr();
            break;
         }
      case STNBEGTSAP:
         {
            retVal = nbDamLSapCfg(cfg, &cfgCfm);
            break;
         }
      default:
         {
            cfgCfm.status = LCM_PRIM_NOK;
            cfgCfm.reason = LCM_REASON_INVALID_ELMNT;
            retVal = RFAILED;
            break;
         }
   }

   nbIfmDamNbCfgCfm(cfg, &cfgCfm);

   RETVALUE(retVal);
}/* nbDamCfgReq */

/*
 *       Fun:     nbDamLSapCfg
 * 
 *       Desc:    This function is used to configure the Lower SAP.
 * 
 *       Ret:     ROK - ok / RFAILED - Failure 
 *
 *       Notes :  None.
 */
PRIVATE S16 nbDamLSapCfg
(     
 LnbMngmt                      *cfg,
 CmStatus                     *status
)
{
   NbLiSapCb *sap = NULLP;
   LnbLSapCfg *lSap = &cfg->t.cfg.t.lSap;

   TRC2(nbLSapCfg);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (NULLP == lSap)
   {
      NB_LOG_ERROR(&nbCb,"Invalid Sap Cfg");
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_PAR_VAL;
      RETVALUE(RFAILED);
   }

   switch(cfg->hdr.elmId.elmnt)
   {
#ifdef EU_DAT_APP
      case STNBEGTSAP:
         {
            NB_ALLOC_DATA_APP(&nbDamCb.egtSap[0], sizeof(NbLiSapCb));
            if(NULLP == nbDamCb.egtSap[0])
            {
               NB_LOG_ERROR(&nbCb,"Failed to allocate Memory");
               status->status = LCM_PRIM_NOK;
               status->reason = LCM_REASON_INVALID_PAR_VAL;
               RETVALUE(RFAILED);
            }
            sap = nbDamCb.egtSap[0];
            break;
         }
#endif 
      default:
         {
            NB_LOG_ERROR(&nbCb,"Invalid Sap ElemenT");
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_PAR_VAL;
            RETVALUE(RFAILED);
            break;
         }
   }

   sap->suId = lSap->suId;
   sap->spId = lSap->spId;
   sap->pst.srcProcId = SFndProcId();
   sap->pst.srcEnt = lSap->srcEnt;
   sap->pst.srcInst = lSap->srcInst;
   sap->pst.dstProcId = lSap->dstProcId;
   sap->pst.dstEnt = lSap->dstEnt;
   sap->pst.dstInst = lSap->dstInst;
   sap->pst.prior = lSap->dstPrior;
   sap->pst.route = lSap->dstRoute;
   sap->pst.selector = lSap->dstSel;
   sap->pst.region = 0;
   sap->mem.region = 0;
   sap->mem.pool = lSap->mem.pool;
   sap->maxBndRetry = lSap->maxBndRetry;
   sap->bndTmr.val = lSap->bndTmr.val;
   sap->bndTmr.enb = lSap->bndTmr.enb;
   sap->sapState = LNB_SAP_UNBOUND;
   cmInitTimers(&sap->timer , 1);

   RETVALUE(ROK);
} /* nbLSapCfg */

/*
 *       Fun:     nbDamCntrlReq
 *
 *       Desc:    This primitive is called by LM to issue control 
 *                requests to the DAM.
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 nbDamCntrlReq
(
 LnbMngmt                      *cntrl
)
{
   S16                        retVal = ROK;
   CmStatus                   cfmStatus;    

   TRC3(nbDamCntrlReq);
   /* Initialize the status memebrs to Success */
   cfmStatus.status = LCM_PRIM_OK;
   cfmStatus.reason = LCM_REASON_NOT_APPL;
   /* Identify the element for control */
   switch (cntrl->hdr.elmId.elmnt)
   { 
      case STNBGEN: 
         {
            switch (cntrl->t.cntrl.action)
            {
               case ASHUTDOWN:
                  {
                     nbDamDeInit();
                     break;
                  }
               case AENA:
                  {
                     if(cntrl->t.cntrl.subAction == SAUSTA)
                     {
                        /* Sub Action Unsolicited Status Alarm */
                        nbDamCb.init.usta = TRUE;
                     }
                     else if (cntrl->t.cntrl.subAction == SADBG)
                     {
                     }
                     break;
                  }
               case ADISIMM:        /* Action DISABLE */
                  {
                     if(cntrl->t.cntrl.subAction == SAUSTA)
                     {
                        nbDamCb.init.usta = FALSE;
                     }
                     else if (cntrl->t.cntrl.subAction == SADBG)
                     {

                     }
                     break;
                  }
            }

            break;
         } /* end of  case STNBGEN */
      case STNBEGTSAP:
         {
            retVal = nbDamLSapCntrl(&cntrl->t.cntrl, &cfmStatus,
                  cntrl->hdr.elmId.elmnt);
            break;
         }

      default:
         {
            retVal = RFAILED;
            break;
         }
   }
   if(retVal == RFAILED)
   {
      cfmStatus.status = LCM_PRIM_NOK;
      cfmStatus.reason = LCM_REASON_INVALID_ELMNT;
   }

   nbIfmDamNbCntrlCfm(&cfmStatus, cntrl);

   RETVALUE(retVal);
} /*nbDamCntrlReq*/

/*
 *       Fun:     nbDamLSapCntrl
 *
 *       Desc:    This function processes control request for the
 *                Lower SAP.
 *
 *       Ret:     ROK - Ok/RFAILED - Failure 
 *
 *       Notes :  This function implements the State transition
 *                for Lower SAP control requests. The function
 *                is the entry point for all control request
 *                events defined.
 *
 */
PRIVATE S16 nbDamLSapCntrl
(
 LnbCntrl                      *sapCntrl,
 CmStatus                     *status,
 Elmnt                        elmnt
)
{
   NbLiSapCb                 *lSapCb = NULLP;
   S16                       retVal;

   retVal = ROK;

   switch (elmnt) 
   {
      case STNBEGTSAP:
         {
            lSapCb = nbDamCb.egtSap[0];
            break;
         }
      default:
         {
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_PAR_VAL;
            RETVALUE(RFAILED);
         }
   }

   /* Initialize the status members to success */
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch (sapCntrl->action)
   {
      case ABND_ENA:
         {
            /* Bind Lower Sap  */
            nbDamBndLSap(lSapCb, status, elmnt);
            retVal = ROK;
            break;
         }
      case AUBND_DIS:
         {
            /* Disconnect PVC and DONT send Unbind Request */
            retVal = nbDamUbndLSap(lSapCb);
            if (ROK != retVal)
            {
               NB_LOG_ERROR(&nbCb,"Invalid Receiver State in call to nuUbndLSap");
               status->status = LCM_PRIM_NOK;
               status->reason = retVal;
            }
            break;
         }
      default:
         {
            NB_LOG_ERROR(&nbCb,"Invalid Action Value");
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_ACTION;
            retVal = RFAILED;
            break;
         }
   }

   RETVALUE(retVal);
} /* nbLSapCntrl */

/********************************************************************IB**
 Layer Management Incoming Primitives
*********************************************************************IB*/

/*
 *       Fun:     wrUbndLSap
 *
 *       Desc:    This function is invoked to unbind & disable a SAP 
 *                A disconnect is sent and the SAP state is changed
 *                to NB_SAP_CFG without sending a unbind request.
 *
 *       Ret:     ROK or LCM_REASON_INVALID_STATE
 *
 *       Notes :  This function provides run time support for the
 *                stack reinitialization procedure.
 */
PRIVATE S16 nbDamUbndLSap
(
 NbLiSapCb                    *sapCb
)
{
   S16                       retVal;

   TRC2(nbDamUbndLSap);

   switch(sapCb->sapState)
   {
      case LNB_SAP_BINDING:
         {
            sapCb->sapState = LNB_SAP_UNBOUND;
            retVal = ROK;
            break;
         }
      case LNB_SAP_BOUND:
         {
            sapCb->sapState = LNB_SAP_UNBOUND;
            retVal = ROK;
            break;
         }
      default:
         {
            retVal = LCM_REASON_INVALID_STATE;
            break;
         }
   }

   RETVALUE(retVal);
} /* nbUbndLSap */

/*
 *       Fun:     wrBndLSap
 *
 *       Desc:    This function binds the lower sap.
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  This function is a state transition action handler
 *                for lower SAP ABND_ENA control request. 
 */
PRIVATE S16 nbDamBndLSap
(
 NbLiSapCb                    *sapCb,
 CmStatus                     *status,
 Elmnt                        elmnt
)
{
   if (LNB_SAP_UNBOUND == sapCb->sapState)
   {
      status->status = LCM_PRIM_OK_NDONE;
      sapCb->sapState = LNB_SAP_BINDING;
      sapCb->bndRetryCnt = 0;

      switch(elmnt)
      {
#ifdef EU_DAT_APP
         case STNBEGTSAP:
            {
               nbDamStartTmr ((PTR)sapCb, NB_TMR_EGT_SAP_BND, sapCb->bndTmr.val);
               NbIfmEgtpBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
               break;
            }
#endif 
         default:
            break;
      }
   }
   else
   {
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_STATE;
   }


   RETVALUE (ROK);
} /* nbBndLSap */

/** @brief This function is responsible for deinitialization of Dam Function.
 *
 * @details
 *
 *     Function:wrDamDeInit 
 *
 *         Processing steps:
 *         - Function de-registers the intiated timer.
 *
 *
 * @param[in]  void
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 nbDamDeInit
(
 Void
)
{
   U32            sapCount = 0;

   for(sapCount = 0 ; sapCount < NB_NUM_EGT_SAPS; sapCount++)
   {
      if(nbDamCb.egtSap[sapCount] != NULLP)
      {
         NB_FREE_DATA_APP(nbDamCb.egtSap[sapCount], sizeof(NbLiSapCb));
      }
   }
   NB_FREE_DATA_APP(nbDamCb.egtSap, (sizeof(NbLiSapCb *) * NB_NUM_EGT_SAPS));

   RETVALUE(nbDamDeRegTmr());
}

PRIVATE  NbDamUeCb * nbDamGetueCbkeyUeIp(U32 ueIpAddr,U8 *drbId)
{
   NbDamUeCb   *ueCb = NULLP;
   NbDamUeCb   *prevUeCb = NULLP;
   NbIpInfo    *ipInfo = NULLP;

   for(;((cmHashListGetNext(&(nbDamCb.ueCbs), (PTR)prevUeCb, (PTR*)&ueCb)) == ROK);)
   {
      if ( ROK == (cmHashListFind(&(ueCb->ipInfo), (U8 *)&(ueIpAddr),
                  sizeof(U32), 0, (PTR *)&ipInfo)))
      {    
         *drbId = ipInfo->drbId; 
         break;
      }
      prevUeCb = ueCb;
      ueCb = NULLP;
   } 

   RETVALUE(ueCb);
}

PUBLIC NbDamUeCb* nbDamGetUe(U8 ueId)
{
   NbDamUeCb *ueCb = NULLP;
   if ( ROK != (cmHashListFind(&(nbDamCb.ueCbs), (U8 *)&(ueId),
      sizeof(U8),0,(PTR *)&ueCb)))
   {    
      RETVALUE(NULLP);
   }

   RETVALUE(ueCb);
}

PUBLIC Void nbDamSetDatFlag(U8 ueId)
{
   NbDamUeCb *ueCb = NULLP;
   ueCb = nbDamGetUe(ueId);
   if(ueCb != NULLP)
   {
      ueCb->dataRcvd = TRUE;
   }
   RETVOID;   
}
