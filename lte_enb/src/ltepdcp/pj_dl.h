


/********************************************************************20**
  
        Name:     LTE RLC file 
    
        Type:     C include file
  
        Desc:     This file contain the hash definations for RLC 
 
        File:     kw.h

        Sid:      pj_dl.h@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB_GA/3 - Fri Feb 12 12:09:19 2016
  
        Prg:      na 
  
*********************************************************************21*/
/** @file kw.h
@brief RLC Hash definitions
*/

#ifndef __PJ_DLH__
#define __PJ_DLH__

 
#define PJ_DBM_FETCH_DL_RBCB(_rbId, _rbType, _ueCb, _rbCb)     \
{                                                           \
   _rbCb = (_rbType==CM_LTE_SRB)?_ueCb->srbCb[_rbId]:_ueCb->drbCb[_rbId];\
}

/* HO_FIX: RRC expects the results as ROK or RFAILED 
           Hence, changing the vlaue to ROK from CPJ_CFG_CFM_OK */
#define  PJ_FILL_DL_SDU_CFM_INFO(_cb,_cfmInfo, _ueCb)                    \
{                                                                 \
   U8 _rbCnt;                                                     \
   U8 _numRb = 0;                                                 \
   PjDlHoCfmInfo *_hoCfmInfo;                                       \
   UdxSduStaInfo  *_staInfo;                                      \
   _cfmInfo->ueId    = _ueCb->key.ueId;                           \
   _cfmInfo->cellId  = _ueCb->key.cellId;                         \
   _cfmInfo->transId = _ueCb->hoInfo->transId;                    \
   for(_rbCnt = 0; _rbCnt < PJ_MAX_DRB_PER_UE && _numRb < CPJ_MAX_DRB; _rbCnt++)          \
   {                                                              \
      _hoCfmInfo   =  &_ueCb->hoInfo->hoCfmInfo[_rbCnt];          \
      _staInfo     =  &_cfmInfo->sduStaInfo[_numRb];              \
      if(_hoCfmInfo->pres != TRUE)                                \
      {                                                           \
         continue;                                                \
      }                                                           \
                                                                  \
      _staInfo->rbId  =  _hoCfmInfo->rbId;                        \
      _staInfo->dir   =  _hoCfmInfo->dir;                         \
      _staInfo->dlSduStaInfo.count     =  _hoCfmInfo->count;   \
      _staInfo->dlSduStaInfo.hoPres    =  TRUE;                \
      _numRb++;                                                   \
   }                                                              \
   _cfmInfo->numRb = _numRb;                                      \
   _cfmInfo->status = ROK;                                      \
   _cfmInfo->reason = CPJ_CFG_REAS_NONE;                                      \
   PJ_FREE(_cb,_ueCb->hoInfo->hoCfmInfo,                          \
              (PJ_MAX_DRB_PER_UE * sizeof(PjDlHoCfmInfo))) \
   PJ_FREE(_cb,_ueCb->hoInfo, sizeof(PjDlHoInfo));                      \
}
#define PJ_DBM_DELETE_DL_RBCB(_cb, _rbCb)                           \
{                                                           \
   /* Free memory for rbCb */                               \
   PJ_FREE(_cb, _rbCb, sizeof(PjDlRbCb));                            \
   PJ_LMM_RB_STS_DEC(_cb);                                     \
}

#endif /* __PJH__ */
  
/********************************************************************30**
  
         End of file:     pj_dl.h@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB_GA/3 - Fri Feb 12 12:09:19 2016
  
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       mn  1. Initial release.
/main/2      ---       nm  1. LTERLC Release 2.1
*********************************************************************91*/
