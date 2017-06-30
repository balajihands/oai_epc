
/********************************************************************20**

     Name:     

     Type:     C source file

     Desc:     Defines required by the NBT layer service user

     File:     nbt.x

     Sid:      
     Prg:      


*******************************************************************21 */
typedef enum _nbMsgTypes
{
   NB_CONFIG_REQ,
   NB_CONFIG_CFM,
   NB_S1_SETUP_REQ,
   NB_S1_SETUP_RSP,
   NB_S1_SETUP_TIMEOUT_IND,
   NB_UE_CNTXT_REL_REQ,
   NB_RESET_REQUEST,
   NB_RESET_ACK,
   NB_ERR_IND_MSG,
   NB_ERAB_REL_IND,
   NB_ERAB_REL_CMD_IND,
   NB_ERAB_REL_RSP,
   NB_UE_CTX_REL_IND,
   NB_INT_CTX_SETUP_IND,
   NB_SCTP_ABORT_REQ,
   NB_SCTP_SHUTDOWN_REQ,
   NB_NAS_NON_DEL,
   NB_NAS_NON_DEL_RSP,
   NB_INIT_CTXT_SETUP_FAIL,
   NB_DROP_INIT_CTXT_SETUP,
   NB_INIT_CTXT_SETUP_DROPPD_IND,
   NB_DELAY_INIT_CTXT_SETUP_RSP,
   NB_SEND_UE_CTXT_REL_FOR_ICS,
   NB_UNKNOWN_MSG_TYPE
}NbMsgTypes;

typedef enum _cfgFailCause
{
   NB_TUCL_CFG_FAILED,
   NB_SCTP_CFG_FAILED,
   NB_EGTP_CFG_FAILED,
   NB_S1AP_CFG_FAILED,
   NB_ENBAPP_CFG_FAILED,
   NB_EGTP_BINDING_FAILED,
   NB_S1AP_BINDING_FAILED,
   NB_PCAP_CFG_FAILED,
   NB_UNKNOWN_ERROR
}CfgFailCause;

typedef enum _cfgSts
{
   CFG_DONE_ROK,
   CFG_DONE_NOK
}CfgSts;

typedef enum result
{
  S1_SETUP_SUCC,
  S1_SETUP_FAIL
}S1SetupRes;

typedef struct _failureCause
{
   U32 causeType;
   U32 causeVal;
}FailCause;

typedef struct _timeToWaitIe
{
   U8  pres;
   U32 val;
}TimeToWaitIe;

typedef struct _nbtPlmnId
{
   U8 numMncDigits;
   U8 mcc[3];
   U8 mnc[3];
}NbtPlmnId;

/* Broadcasted PLMN List */
typedef struct _nbtBPlmnList
{
   U8 numBPlmns;
   NbtPlmnId plmnIds[NBT_MAX_NUM_BPLMNS];
}NbtBPlmnList;

typedef struct _nbtSuppTA
{
   U16 tac;
   NbtBPlmnList bPlmnList; /* Broadcasted PLMNs */
}NbtSuppTA;

typedef struct _nbtSuppTAList
{
   U8 pres;
   U8 numTAs;
   NbtSuppTA suppTA[NBT_MAX_NUM_TAC];
}NbtSuppTAList;

/* eNodeB Name : Macro NB or Home NB */
typedef struct _nbtEnodeBName
{
   U8 pres;
   U8 val;
}NbtEnodeBName;

/* CSG Id List */
typedef struct _nbtCsgIdList
{
   U8 pres;
   U8 numCsgIds;
   U32 csgId[NBT_MAX_NUM_CSG_ID]; /* Bit string, size(27), lower 27 bits used */
}NbtCsgIdList;

typedef struct _nbtMmeName
{
   U8 len;
   U8 val[150];
}NbtMmeName;

typedef struct  _nbS1SetupRsp
{
   S1SetupRes     res;
   FailCause      cause;
   TimeToWaitIe   waitIe;
   U8             numPlmnIds;
   NbtPlmnId      plmnIds[NBT_MAX_PLMNS_PER_MME];
   U16            numGrpIds;
   U16            groupIds[NBT_MAX_GRPS_PER_MME];
   U16            numCodes;
   U8             codes[NBT_MAX_CODES_PER_MME];
   U8             relCapacity;
   NbtMmeName     mmeName;
   U32            mmeRelaySuppInd;
}NbS1SetupRsp;

typedef struct _nbConfigReq
{
   U16   cellId;                  /* cell id */            
   U16   tac;                     /* tracking area code*/
   U32   enbIpAddr;               /* enodeb address */ 
   U16   mmeId;                   /* mme id */
   U32   mmeAddr;                 /* mme code */
   U32   sctpIpAddr;              /* sctp address */ 
   U8    enbName[151];            /* enodeb name */
   U8    plmnId[NBT_MAX_PLMN_ID]; /*  plmn  */ 
   U32   heartBeatInterval;       /* sctp heartbeat timer value */
   U32   rtoInitial;              /* sctp rto initial */
   U32   rtoMin;                  /* sctp rto min */
   U32   rtoMax;                  /* sctp rto max */
   U32   s1PrepTimerVal;          /* s1-Preparation Timer Value */
   U32   s1OvrAllTimerVal;        /* s1-Overall Timer Value */
   U8    sctpUdpServiceType;      /* sctp server type */
   U32   inactvTmrVal;            /* inactivity timer value */
   U32   maxExpires;              /* max expiry count */
   U16   s1SetupTmr;              /* timer for s1 setup req */
   NbtSuppTAList suppTAs;         /* Supported TAs */
   NbtEnodeBName eNodeBType;      /* Macro eNodeB or Home eNodeB */
   NbtCsgIdList csgIdList;        /* CSG Id List */
   U16    noOfSctpInStreams;      /* No of Sctp In Streams  */
   U16    noOfSctpOutStreams;     /* No of Sctp Out Streams  */
   S8     ueEthIntf[10];
}NbConfigReq;

typedef struct _nbConfigCfm
{
 CfgSts       status;
 CfgFailCause cause; 
}NbConfigCfm;

typedef struct _nbUeCntxtRelReq
{
   U16 ueId;
   U8 cause;
}NbUeCntxtRelReq;

typedef enum
{
   NB_COMPLETE_RESET = 0,
   NB_PARTIAL_RESET
}NbResetType;

typedef struct _nbCompleteReset
{
   U32 s1IntfId;
}NbCompleteReset;

typedef struct _mnPartialReset
{
   U16 numOfConn;
   U8 *ueIdLst;
}NbPartialReset;

typedef struct _nbCause
{
   U8 causeType;
   U32 causeVal;
}NbResetCause;

typedef struct _resetRequest
{
   NbResetType rstType;
   NbResetCause cause;
   union
   {
      NbCompleteReset completeRst;
      NbPartialReset partialRst;
   }u;
}NbResetRequest;

typedef struct _nbResetRequest
{
   U8 status;
   U16 numOfUes;
   U8 *ueIdLst;
}NbResetAckldg;

typedef struct _nbCriticalityDiag_IE_Item
{
   U8 pres;
   U32 iECriticality;
   U32 iE_ID;
   U32 TypOfErr;
}NbCriticalityDiag_IE_Item;

typedef struct _nbCriticalityDiag_IE_Lst
{
   U16 noComp;
   NbCriticalityDiag_IE_Item *member;
}NbCriticalityDiag_IE_Lst;

typedef struct _nbCriticalityDiag
{
   U8  pres;
   U8  procCodePres;
   U8  proCriticalityPres;
   U8  trigMsgPres;
   U32 procedureCode;
   U32 triggeringMsg;
   U32 procedureCriticality;
   NbCriticalityDiag_IE_Lst ieLst;
}NbCriticalityDiag;

typedef struct _nbErrIndMsg
{
   U8          isUeAssoc;
   U8          ue_Id;
   U8          causePres;
   FailCause   cause;
   NbCriticalityDiag criticalityDiag;
}NbErrIndMsg;

typedef struct _nbSctpAbortReqMsg
{
   U8 cause;
}NbSctpAbortReqMsg;

typedef struct _nbErabRelInd
{
   U8 ueId;
   U8 numOfErabIds;
   U8 *erabIdLst;
}NbErabRelInd;

typedef NbErabRelInd NbErabRelReq;

typedef struct _nbErabRelCmd
{
   U8 ueId;
   U32 mmeUeS1apId;
   U32 enbUeS1apId;
   U8 numOfErabIds;
   U8 *erabIdLst;
}NbErabRelCmd;

typedef NbErabRelCmd NbErabRelRsp;

typedef struct _nbUeCtxRelInd
{
   U8 ueId;
}NbUeCtxRelInd;
typedef struct _nbIntCtxtSetUpInd
{
   U8 ueId;
   U8 status;
}NbIntCtxtSetUpInd;
typedef struct _nbIntCtxtSetUpDrpdInd 
{
   U8 ueId;
}NbIntCtxtSetUpDrpdInd;
typedef struct _nbNasNonDel
{
   U8 ueId;
   Bool flag;
   U32 causeType;
   U32 causeVal;
}NbNasNonDel;
typedef struct _nbInitCtxtSetupFail
{
   U8 ueId;
   Bool initCtxtSetupFailInd;
   U32 causeType;
   U32 causeVal;
}NbInitCtxtSetupFail;

typedef struct _nbDropInitCtxtSetup
{
   U8 ueId;
   Bool isDropICSEnable;
   U32 tmrVal;
}NbDropInitCtxtSetup;
typedef struct _nbDelayICSRsp
{
   U8 ueId;
   Bool isDelayICSRsp;
   U32 tmrVal;
}NbDelayICSRsp;
typedef struct _nbSendUeCtxtRelForICSRsp
{
   U8 ueId;
   U32 causeType;
   U32 causeVal;
   Bool sndICSRspUeCtxtRel;
}NbSendUeCtxtRelForICSRsp;
typedef struct _nbNasNonDelRsp
{
   U8 ueId;
}NbNasNonDelRsp;

typedef struct _nbtMsg
{
   NbMsgTypes msgType;
   union
   {
      NbConfigReq     configReq;
      NbConfigCfm     configCfm;
      NbS1SetupRsp    s1SetupRsp; 
      NbUeCntxtRelReq ueCntxtRelReq;
      NbResetRequest  resetReq;
      NbResetAckldg   resetAck;
      NbErabRelInd   erabRelInd;
      NbErabRelCmd   erabRelCmdInfo;
      NbErabRelRsp   erabRelRsp;
      NbUeCtxRelInd   ueCtxRelInd;
      NbIntCtxtSetUpInd IntCtxtSetUpInd;
      NbIntCtxtSetUpDrpdInd intCtxtSetUpDrpdInd;
      NbErrIndMsg     s1ErrIndMsg;
      NbSctpAbortReqMsg  sctpAbortReqMsg;
      NbInitCtxtSetupFail initCtxtSetupFail; 
      NbDropInitCtxtSetup dropInitCtxtSetup; 
      NbDelayICSRsp      delayInitCtxtSetupRsp;
      NbSendUeCtxtRelForICSRsp sendUeCtxtRelForICS; 
      NbNasNonDel    nasNondel;
      NbNasNonDelRsp  nasNondelRsp;
   }t;
}NbtMsg;

/* nbt Interface general Structure declerations */
typedef NbtMsg   NbtRequest;      

typedef NbtMsg   NbtResponse;


typedef S16 (*NbtReqHdl)(Pst*, NbtRequest*);

typedef S16 (*NbtRspHdl)(Pst*, NbtResponse*);

EXTERN S16 cmPkNbtMsgReq(Pst*, NbtRequest*);

EXTERN S16 cmPkNbtMsgRsp(Pst*, NbtResponse*);

EXTERN S16 cmUnPkNbtMsgReq ARGS((NbtReqHdl, Pst*,Buffer *));

EXTERN S16 cmUnPkNbtMsgRsp ARGS((NbtRspHdl, Pst*, Buffer *));
EXTERN Void nbUiSendConfigFailIndToUser ARGS((CfgFailCause cause));
EXTERN S16 NbUiNbtMsgReq(Pst *pst, NbtMsg *req);
EXTERN Void nbUiSendSuccessResponseToUser(NbMsgTypes msgType);
EXTERN S16 NbEnbCfgReqHdl(NbConfigReq   *cfg);
EXTERN S16 nbUiSendResetAckToUser(NbResetAckldg *resetAck);
EXTERN S16 nbUiSendUeCtxRelIndToUser(U8 ueId);
EXTERN S16 nbUiSendIntCtxtSetupIndToUser(U8 ueId, U8 status);
EXTERN S16 nbUiSendErabRelCmdInfoToUser(NbErabRelCmd *erabRelInfo);
/********************************************************************30**

         End of file:     

*********************************************************************31*/
