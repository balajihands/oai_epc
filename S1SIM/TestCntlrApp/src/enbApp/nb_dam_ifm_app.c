/**********************************************************************

    Name:  LTE S1SIM - ENODEB Application Module 

    Type:  C include file
               
    Desc:  C source code for ENODEB Application

     File:     nb_dam_ifm_app.c

     Sid:      

     Prg:       

**********************************************************************/

#include "rl_interface.h"
#include "rl_common.h"
#include "nb_ifm_dam.h"
#include "nb_ifm_dam_utils.h"
#include "nb_dam_ifm_app.h"
#include "nb_log.h"
#include "nb_ifm_dam_utils.x"

EXTERN Void nbIfmDamNbSendLmAlarm(U16, U16, U16);
EXTERN Void  nbDamTnlCreatReq(NbDamTnlInfo*);
EXTERN NbDamUeCb* nbDamGetUe(U8 ueId);

/** @brief This function is used to configure the Data Application Module.
 *
 * @details
 *
 *     Function: nbIfmDamWrCntrlReq 
 *
 *         Processing steps:
 *         - This function is invoked by Stack manager to configre the DAM
 *           module 
 *         - This function invokes the function wrDamCntrlReq to process the
 *           control req.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return  Void 
 */
PUBLIC Void  nbIfmDamNbCntrlReq
(
LnbMngmt    *cfg
)
{
   TRC2(nbIfmDamNbCntrlReq);
   nbDamCntrlReq(cfg);
   RETVOID;
} /*nbIfmDamNbCntrlReq*/

/** @brief This function is used to configure the Data Application Module.
 *
 * @details
 *
 *     Function: nbIfmDamNbCfgReq 
 *
 *         Processing steps:
 *         - This function is invoked by Stack manager to configure the DAM
 *           module 
 *         - This function invokes the function wrIfmDamWrCfgReq to process the
 *           configuration Req.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return Void  
 */
PUBLIC Void nbIfmDamNbCfgReq
(
LnbMngmt *cfg
)
{
   TRC2(nbIfmDamNbCfgReq);
   nbDamCfgReq(cfg);
   RETVOID;
} /*nbIfmDamNbCfgReq*/

/** @brief This function is used to send the configure confirmation 
 *   the enbapp .
 *
 * @details
 *
 *     Function: nbIfmDamNbCfgCfm 
 *
 *         Processing steps:
 *         - This function is invoked the DAM  module 
 *         - This function invokes the pack function to send the 
 *           configuration Confirm.
 *
 * @param[in] cfg    : Data Application Module Configuration.  
 * @param[in] cfgCfm : Status of the configuration process.  
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 nbIfmDamNbCfgCfm
(
LnbMngmt    *cfg,
CmStatus   *cfgCfm
)
{
   Pst  *pst;
   TRC2(nbIfmDamNbCfgCfm);
   pst = &nbDamCb.nbAppPst;

   RETVALUE(cmPkDamCfgCfm(pst,cfg,cfgCfm));
} /*nbIfmDamNbCfgCfm*/

/** @brief This function is used to send the control confirm back to the Application.
 *
 * @details
 *
 *     Function: wrIfmDamWrCntrlCfm 
 *
 *         Processing steps:
 *         - This function is invoked by the DAM module to send the confirm for the same. 
 *         - This function invokes the pack function for sending the confirm.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 nbIfmDamNbCntrlCfm
(
CmStatus   *status,
LnbMngmt    *cntrl
)
{
   Pst  *pst;

   TRC2(nbIfmDamNbCntrlCfm);

   pst = &nbDamCb.nbAppPst;
   RETVALUE(cmPkDamCntrlCfm(pst,status,cntrl));

} /*nbIfmDamWrCntrlCfm*/

/*
 *       Fun:     wrSendLmAlarm
 *
 *       Desc:    Function trigger alarm
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  None
 */
PUBLIC Void nbIfmDamNbSendLmAlarm
(
U16                          category,
U16                          event,
U16                          cause
)
{
   Pst  *pst;

   TRC2(nbIfmDamWrSendLmAlarm);

   pst = &nbDamCb.nbAppPst;
   cmPkDamSendAlarmInd(pst,category,event,cause);
   RETVOID;

}/* nbIfmDamNbSendLmAlarm */

/** @brief This function is used to an EGTP create tuunel.
 *
 * @details
 *
 *     Function: nbIfmDamWrTnlCreatReq
 *
 *         Processing steps:
 *         - This function is invoked by the UMM module to ceate a EGTP tunnel.
 *         - This function invokes the function wrDamTnlCreatReq to create
 *           tunnel.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]  tnlInfo: Tunnel Info which needs to be create.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 nbIfmDamNbTnlCreatReq
(
 NbDamTnlInfo *tnlInfo
)
{
   TRC2(nbIfmDamNbTnlCreatReq);
   nbDamTnlCreatReq(tnlInfo);
   RETVALUE(ROK);
} /*nbIfmDamNbTnlCreatReq */

PUBLIC S16 nbIfmDamHandleUeCntxtRelReq
(
 U16 ueId,
 U8 cause
)
{
   NbDamUeCb *ueCb = NULLP;

   ueCb = nbDamGetUe(ueId);
   if(ueCb == NULLP)
   {
      NB_LOG_ERROR(&nbCb, "Failed to find UE Cb for UeId %d in DAM", ueId);
      RETVALUE(RFAILED);
   }
   /* Stop the in activity timer */
   nbDamStopTmr((PTR)ueCb, NB_TMR_INACTIVITY);

   /* Send Ue context release request to EnbApp, which will trigger 
    * release towards EPC */
   nbIfmDamNbCtxtRel(ueId, cause);

   RETVALUE(ROK);
} /* nbIfmDamHandleUeCntxtRelReq */

/** @brief This function informs the tunnel was ceated successfully with local
 *         teid or tunnel was not created to the UMM Module. 
 *
 * @details
 *
 *     Function: nbIfmDamNbTnlCreatCfm
 *
 *         Processing steps:
 *         - This function is process the tunnel creation conformation from DAM
 *           module.
 *         - This function invokes the UMM Module function wrUmmTnlCreatCfm to
 *           process the tunnel create conformation from DAM.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]   status: status of tunnel creation.
 * @param[in]  lclTeid: Local Tunnel ID.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 nbIfmDamNbTnlCreatCfm
(
 U8                           status,
 U32                          lclTeid
)
{
   Pst *pst;
   TRC2(nbIfmDamNbTnlCreatCfm);

   pst = &nbDamCb.nbAppPst;
   RETVALUE(cmPkTnlCreatCfm(pst, status, lclTeid));
} /*nbIfmDamNbTnlCreatCfm*/

/** @brief This function informs the tunnel was ceated successfully with local
 *         teid or tunnel was not created to the UMM Module. 
 *
 * @details
 *
 *     Function: nbIfmDamNbTnlDelCfm
 *
 *         Processing steps:
 *         - This function is process the tunnel creation conformation from DAM
 *           module.
 *         - This function invokes the UMM Module function wrUmmTnlCreatCfm to
 *           process the tunnel create conformation from DAM.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]   status: status of tunnel creation.
 * @param[in]  lclTeid: Local Tunnel ID.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 nbIfmDamNbTnlDelCfm
(
 U8 status,
 U32 lclTeid
)
{
   Pst *pst;
   U8 ueId  = (lclTeid & 0x00ffff00) >> 8;

   TRC2(nbIfmDamNbTnlDelCfm);

   if(status == LCM_PRIM_OK)
   {
      NB_LOG_DEBUG(&nbCb, "Tunnel deleted successfully with LclTeid[%d]",lclTeid);
   }
   else
   {
      NB_LOG_ERROR(&nbCb, "Tunnel deletion Failed with LclTeid[%d]",lclTeid);
   }
#if 0
   nbDamDelUe(ueId);
#endif

   if(ROK == nbDamDelUe(ueId))
   {
      pst = &nbDamCb.nbAppPst;
      RETVALUE(cmPkUeDelCfm(pst, ueId));
   }
   RETVALUE(ROK);
} /*nbIfmDamNbTnlDelCfm*/

/** @brief This function is used to send UE Context release to Application. 
 *
 * @details
 *
 *     Function:nbIfmDamNbCtxtRel
 *
 *         Processing steps:
 *         - This function is invoked by the DAM module to send Context release to app. 
 *         - This function invokes the pack function for sending the conetxt release.
 *
 * @param[in] crnti: UE Identifier.  
 * @param[in] cellId: Cell ID.  
 * @param[in] event: Event causing context release.  
 * @return  Void 
 */
PUBLIC Void nbIfmDamNbCtxtRel
(
U8                     ueId,
U8                     event 
)
{
   Pst  *pst;

   TRC2(nbIfmDamNbCtxtRel);
   pst = &nbDamCb.nbAppPst;

   cmPkDamSendCtxtRel(pst,ueId,event);
   RETVOID;
}
