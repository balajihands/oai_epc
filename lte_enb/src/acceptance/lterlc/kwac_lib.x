

/**********************************************************************
 
    Name:   LTE RLC layer
 
    Type:   C include file
 
    Desc:   Defines required by CKW interface of XTA LTE RLC
 
    File:   kwac_ckw.x
 
    Sid:      kwac_lib.x@@/main/TeNB_Main_BR/4 - Thu Apr 24 17:08:31 2014
 
    Prg:    gk 
 
**********************************************************************/
 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000371       LTE RLC
*
*/


#ifndef __KWAC_LIB_X__
#define __KWAC_LIB_X__

#ifdef KW_PDCP

#ifdef __cplusplus
extern "C" {
#endif

typedef Void (* CipherFn) ARGS(( U8 *key, U32 count, U32 bearer, U32 dir, U8 *data, U32 length ));
typedef U8* (* IntgFn) ARGS(( U8* key, U32 count, U32 fresh, U32 dir, U8 *data, U32 length));

typedef struct kwAcLibIntgCb
{
   CmLListCp   intList;
   CmLListCp   intOutList;
   PTR         fdCounter;
}KwAcLibIntgCb;
   
typedef struct kwAcLibCipCb
{
   CmLListCp  cipList;
   CmLListCp  cipOutList;
   PTR        fdCounter;
}KwAcLibCipCb;

typedef struct kwAcLibCompCb
{
   CmLListCp  compOutList;
}KwAcLibCompCb;

typedef struct kwAcLibSecEnt
{
   CmLList     listEnt;
   PTR         fd;
   U8          key[16]; /* 128 bit key*/
   U8          algoType;
}KwAcLibSecEnt;


typedef struct kwAcLibOutEnt
{
   CmLList     listEnt;
   U32      id;
   PTR      fd;
   Pst      pst;
   U32      macIStatus;
   PjLibTrans  libTrans;
   Buffer   *mBuf;
}KwAcLibOutEnt;

typedef struct kwAcLibCb
{
   KwAcLibIntgCb   intCb;
   KwAcLibCipCb    cipCb;
   KwAcLibCompCb  compCb;
}KwAcLibCb;

/*---------------------------------------------------------
 *					Kasumi.x
 *---------------------------------------------------------*/
/*------- unions: used to remove "endian" issues ------------------------*/
#ifdef KWAC_LIB_SUPPORT
typedef union {
   U32 b32;
   U16 b16[2];
   U8  b8[4];
} DWORD;

typedef union {
   U16 b16;
   U8  b8[2];
} WORD;

/* ---- a 64 bit structure to resolved endian issues */
typedef union {
   U32 b32[2];
   U16 b16[4];
   U8  b8[8];
} REGISTER64;
#endif

EXTERN KwAcLibCb libCb;
/*------- Function decalarations: ---------------------------------------*/

#ifdef KWAC_LIB_SUPPORT
/* Functions defines for ksmia */
EXTERN Void kwAcKasumi ARGS((
U8 *data
));
EXTERN Void kwAcKeySchedule ARGS((
U8 *k 
));
EXTERN void kwAcKasF8 ARGS((
U8 *key,
int count,
int bearer,
int dir,
U8 *data,
int length 
));
EXTERN U8 *kwAcKasF9 ARGS((
U8 *key,
int count,
int fresh,
int dir,
U8 *data,
int length 
));
#endif /* KWAC_LIB_SUPPORT */

#ifdef PJ_CMP_ASYNC

typedef S16 (*LibObdCmpInitReq)   ARGS((
Pst      *pst,                    /* post structure */
PjCmpCxt cmpCxt,                  /* Context for ROHC */
PjRohc   rohc,                    /* ROHC Config Info */
PTR      *fd                      /* FD pointer */
));

typedef S16 (*LibObdCmpReq)   ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId,                /* Context Id for compression */
PjLibTrans *libTrans,             /* Transaction Id */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu                  /* Decompressed SDU */
         ));

typedef S16 (*LibObdDecmpReq)   ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId,                /* Context Id for compression */
PjLibTrans *libTrans,              /* Transaction Id */
Buffer   *mBuf,                   /* SDU to be decompressed */
Buffer   **opSdu,                 /* Decompressed SDU */
Buffer   **feedback               /* ROHC feedback buffer */
         ));

typedef S16 (*LibObdCmpCloseReq)   ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId                 /* ROHC Context to be closed */
         ));

typedef S16 (*LibObdCmpResetReq) ARGS((
Pst      *pst,
PTR      cmpCxtId
));

typedef S16 (*LibObdCmpFbReq)   ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId,                /* Context Id for compression */
PjLibTrans *libTrans,             /* Transaction Id */
Buffer   *feedback                /* SDU to be compressed */
));
#endif /* PJ_CMP_ASYNC */

#ifdef PJ_SEC_ASYNC

typedef S16 (*LibObdIntInitReq)   ARGS((
Pst      *pst,
PjSecCxt secCxt,
PjIntInfo intInfo,
PTR      *cxtId
));

typedef S16 (*LibObdIntProtReq)   ARGS((
Pst      *pst,                    /* post structure */
PTR      intCxtId,                /* Context Id for compression */
PjSecInp   secInp,                 /* Input for Integrity protection */                  
PjLibTrans *libTrans,              /* Transaction Id */
Buffer   *mBuf,                   /* SDU to be decompressed */
U32        *macI                    /* Message authentication code for the SDU */
         ));

typedef S16 (*LibObdIntVerReq)   ARGS((
Pst         *pst,                   /* post structure */
PTR        intCxtId,                /* Context Id for compression */
PjSecInp   secInp,                  /* Input for Integrity protection */                  
PjLibTrans *libTrans,                /* Transaction Id */
Buffer     *mBuf,                   /* SDU to be compressed */
U32        macI,                   /* Message authentication code for the SDU */
Status     *status                  /* Status of the Integrity verification */
         ));

typedef S16 (*LibObdIntCloseReq)   ARGS((
Pst      *pst,                    /* post structure */
PTR      intCxtId                 /* Integration CxtId to be closed */
         ));

typedef S16 (*LibObdCpInitReq)   ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Context for Integrity
                                     protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *cpKey,                  /* Control plane ciphering key */
PTR      *fd                      /* FD pointer */
         ));

typedef S16 (*LibObdUpInitReq)   ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Context for Integrity
                                     protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *upKey,                  /* User plane ciphering key */
PTR      *fd                      /* FD pointer */
         ));

typedef S16 (*LibObdCipherReq)   ARGS((
Pst      *pst,                    /* post structure */
PTR         cxtId,           /* Context Id for compression */
PjLibTrans  *libTrans,           /* Transaction Id */
PjSecInp    secInp,             /* Input for Deciphering */                  
Buffer      *mBuf,              /* SDU to be deciphered */
Buffer      **opSdu             /* deciphered SDU */
         ));

typedef S16 (*LibObdDecipherReq)   ARGS((
Pst      *pst,                    /* post structure */
PTR         cxtId,           /* Context Id for compression */
PjLibTrans  *libTrans,           /* Transaction Id */
PjSecInp    secInp,             /* Input for Deciphering */                  
Buffer      *mBuf,              /* SDU to be deciphered */
Buffer      **opSdu             /* deciphered SDU */
         ));

typedef S16 (*LibObdCipherCloseReq)   ARGS((
Pst      *pst,                    /* post structure */
PTR      cpCxtId                  /* Context Id for SRBs to be closed */
         ));

#endif /* PJ_SEC_ASYNC */

#ifdef KWAC_LIB_SUPPORT
/* Initialization. 
 * Input k[4]: Four 32-bit words making up 128-bit key. 
 * Input IV[4]: Four 32-bit words making 128-bit initialization variable. 
 * Output: All the LFSRs and FSM are initialized for key generation. 
 * See Section 4.1. 
 */ 
 
EXTERN Void kwAcSnowInit(U32 k[4], U32 IV[4]); 
 
 
/* Generation of Keystream. 
 * input n: number of 32-bit words of keystream. 
 * input z: space for the generated keystream, assumes 
 *          memory is allocated already. 
 * output: generated keystream which is filled in z 
 * See section 4.2. 
 */ 
 
EXTERN Void kwAcSnowGenKeystrm(U32 n, U32 *z); 
/* f8.
 * Input key: 128 bit Confidentiality Key.
 * Input count:32-bit Count, Frame dependent input.
 * Input bearer: 5-bit Bearer identity (in the LSB side).
 * Input dir:1 bit, direction of transmission.
 * Input data: length number of bits, input bit stream.
 * Input length: 16 bit Length, i.e., the number of bits to be encrypted or 
 *               decrypted.
 * Output data: Output bit stream. Assumes data is suitably memory 
 * allocated.
 * Encrypts/decrypts blocks of data between 1 and 20000 bits in length as 
 * defined in Section 3.
 */
EXTERN Void kwAcSnowF8 ARGS(( U8 *key, U32 count, U32 bearer, U32 dir, U8 *data, U32 length ));

/* f9.
 * Input key: 128 bit Integrity Key.
 * Input count:32-bit Count, Frame dependent input.
 * Input fresh: 32-bit Random number.
 * Input dir:1 bit, direction of transmission (in the LSB).
 * Input data: length number of bits, input bit stream.
 * Input length: 64 bit Length, i.e., the number of bits to be MAC'd.
 * Output  : 32 bit block used as MAC
 * Generates 32-bit MAC using UIA2 algorithm as defined in Section 4.
 */

EXTERN U8* kwAcSnowF9 ARGS(( U8* key, U32 count, U32 fresh, U32 dir, U8 *data, U32 length));

#endif /* KWAC_LIB_SUPPORT */


/* Functions in kwac_libcb.c */
EXTERN S16 kwAcLibObdCmpInitReq  ARGS((
Pst      *pst,                    /* post structure */
PjCmpCxt cmpCxt,                  /* Context for ROHC */
PjRohc   rohc,                    /* ROHC Config Info */
PTR      *cmpCxtId                /* FD pointer */
));
EXTERN S16 kwAcLibObdCmpReq  ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId,                /* Context Id for compression */
PjLibTrans *libTrans,             /* Transaction Id */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu                  /* Compressed SDU */
));
EXTERN S16 kwAcLibObdDecmpReq  ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId,                /* Context Id for compression */
PjLibTrans *libTrans,              /* Transaction Id */
Buffer   *mBuf,                   /* SDU to be decompressed */
Buffer   **opSdu,                 /* Decompressed SDU */
Buffer   **feedback               /* ROHC feedback buffer */
));
EXTERN S16 kwAcLibObdCmpResetReq ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId                 /* ROHC Context to be closed */
));
EXTERN S16 kwAcLibObdCmpFbReq  ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId,                /* Context Id for compression */
PjLibTrans *libTrans,             /* Transaction Id */
Buffer   *feedback                /* SDU to be decompressed */
));
EXTERN S16 kwAcLibObdCmpCloseReq ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId                 /* ROHC Context to be closed */
));

EXTERN S16 kwAcLibObdIntInitReq  ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Context for Integrity
                                     protection/verificatino */
PjIntInfo intInfo,                /* Integrity Cfg Info */
PTR      *cxtId                   /* FD pointer */

));
EXTERN S16 kwAcLibObdIntProtReq  ARGS((
Pst        *pst,                    /* post structure */
PTR        cxtId,                   /* Context Id for compression */
PjSecInp   secInp,                 /* Input for Integrity protection */                  
PjLibTrans *libTrans,               /* Transaction Id */
Buffer     *mBuf,                   /* SDU to be compressed */
U32        *macI                    /* Message authentication code for the SDU */
));
EXTERN S16 kwAcLibObdIntVerReq  ARGS((
Pst         *pst,                   /* post structure */
PTR        intCxtId,                /* Context Id for compression */
PjSecInp   secInp,                  /* Input for Integrity protection */                  
PjLibTrans *libTrans,                /* Transaction Id */
Buffer     *mBuf,                   /* SDU to be compressed */
U32        macI,                   /* Message authentication code for the SDU */
Status     *status                  /* Status of the Integrity verification */
));
EXTERN S16 kwAcLibObdIntCloseReq ARGS((
Pst      *pst,                    /* post structure */
PTR      intCxtId                 /* Integration CxtId to be closed */
));

EXTERN S16 kwAcLibObdCpInitReq ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Context for Integrity
                                     protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *cpKey,                   /* Control plane ciphering key */
PTR      *cxtId                   /* FD pointer */

));
EXTERN S16 kwAcLibObdUpInitReq ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Context for Integrity
                                     protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *upKey,                   /* User plane ciphering key */
PTR      *cxtId                   /* FD pointer */
));
EXTERN S16 kwAcLibObdCipherReq  ARGS((
Pst      *pst,                    /* post structure */
PTR         secCxtId,           /* Context Id for compression */
PjLibTrans  *libTrans,           /* Transaction Id */
PjSecInp    secInp,             /* Input for Ciphering */                  
Buffer      *mBuf,              /* SDU to be ciphered */
Buffer      **opSdu             /* ciphered SDU */
));
EXTERN S16 kwAcLibObdDecipherReq  ARGS((
Pst      *pst,                    /* post structure */
PTR         cxtId,           /* Context Id for compression */
PjLibTrans  *libTrans,           /* Transaction Id */
PjSecInp    secInp,             /* Input for Deciphering */                  
Buffer      *mBuf,              /* SDU to be deciphered */
Buffer      **opSdu             /* deciphered SDU */
));
EXTERN S16 kwAcLibObdCipherCloseReq ARGS((
Pst      *pst,                    /* post structure */
PTR      cpCxtId                  /* Context Id for SRBs to be closed */
));

/* kwac_libutl.c */
EXTERN S16 kwAcLibUtlBufInit ARGS((Void));
EXTERN S16 kwAcLibUtlGetEnt ARGS((
CmLListCp   *lCp,
PTR         fd,
KwAcLibSecEnt **ent
));
EXTERN void kwAcLibUtlDelEnt ARGS((
CmLListCp   *lCp,
PTR         fd
));
EXTERN S16 kwAcLibUtlInsOutEnt ARGS((
CmLListCp   *lCp,
KwAcLibOutEnt *ent
));
EXTERN S16 kwAcLibUtlGetOutEnt ARGS((
CmLListCp   *lCp,
U32         id,
KwAcLibOutEnt **ent
));
EXTERN Void kwAcLibUtlDelOutEnt ARGS((
CmLListCp   *lCp,
KwAcLibOutEnt  *ent
));

/* kwac_libhdl.c */
EXTERN S16 kwAcLibHdlDirective ARGS((
CmXtaTCCb    *tcCb,    /* tcCb data structure  */
CmXtaSpCb  *spCb
));
EXTERN S16 kwAcLibHdlIntProtOut ARGS((
CmXtaTCCb    *tcCb,    /* tcCb data structure  */
CmXtaSpCb  *spCb
));
EXTERN S16 kwAcLibHdlIntVerOut ARGS((
CmXtaTCCb    *tcCb,    /* tcCb data structure  */
CmXtaSpCb  *spCb
));
EXTERN S16 kwAcLibHdlCipherOut ARGS((
CmXtaTCCb    *tcCb,    /* tcCb data structure  */
CmXtaSpCb  *spCb
));
EXTERN S16 kwAcLibHdlDecipherOut ARGS((
CmXtaTCCb    *tcCb,    /* tcCb data structure  */
CmXtaSpCb  *spCb
));
EXTERN S16 kwAcLibHdlCompOut ARGS((
CmXtaTCCb    *tcCb,    /* tcCb data structure  */
CmXtaSpCb  *spCb
));
EXTERN S16 kwAcLibHdlDecmpOut ARGS((
CmXtaTCCb    *tcCb,    /* tcCb data structure  */
CmXtaSpCb  *spCb
));

/* kwac_libreg.c */

#ifdef PJ_CMP_ASYNC

EXTERN S16 cmPkLibObdCmpInitReq ARGS((
Pst * pst,
PjCmpCxt cmpCxt,
PjRohc rohc
));
EXTERN S16 cmUnpkLibObdCmpInitReq ARGS((
LibObdCmpInitReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdCmpReq ARGS((
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 cmUnpkLibObdCmpReq ARGS((
LibObdCmpReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdDecmpReq ARGS((
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 cmUnpkLibObdDecmpReq ARGS((
LibObdDecmpReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdCmpCloseReq ARGS((
Pst * pst,
PTR cmpCxtId
));
EXTERN S16 cmUnpkLibObdCmpCloseReq ARGS((
LibObdCmpCloseReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdCmpResetReq ARGS ((
Pst * pst,
PTR cpCxtId
));
EXTERN S16 cmUnpkLibObdCmpResetReq ARGS ((
LibObdCmpResetReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdCmpFbReq ARGS((
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * feedback
));
EXTERN S16 cmUnpkLibObdCmpFbReq ARGS((
LibObdCmpFbReq func,
Pst *pst,
Buffer *mBuf
));


#endif /* PJ_CMP_ASYNC */

#ifdef PJ_SEC_ASYNC

EXTERN S16 cmPkLibObdIntInitReq ARGS((
Pst * pst,
PjSecCxt secCtx,
PjIntInfo intInfo
));
EXTERN S16 cmUnpkLibObdIntInitReq ARGS((
LibObdIntInitReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdCpInitReq ARGS((
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *cpKey
));
EXTERN S16 cmUnpkLibObdCpInitReq ARGS((
LibObdCpInitReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdUpInitReq ARGS((
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *upKey
));
EXTERN S16 cmUnpkLibObdUpInitReq ARGS((
LibObdUpInitReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdIntProtReq ARGS((
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 cmUnpkLibObdIntProtReq ARGS((
LibObdIntProtReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdCipherReq ARGS((
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 cmUnpkLibObdCipherReq ARGS((
LibObdCipherReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdIntVerReq ARGS((
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf,
U32   macI
));
EXTERN S16 cmUnpkLibObdIntVerReq ARGS((
LibObdIntVerReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdDecipherReq ARGS((
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 cmUnpkLibObdDecipherReq ARGS((
LibObdDecipherReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdIntCloseReq ARGS((
Pst * pst,
PTR intCxtId
));
EXTERN S16 cmUnpkLibObdIntCloseReq ARGS((
LibObdIntCloseReq func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmPkLibObdCipherCloseReq ARGS((
Pst * pst,
PTR cpCxtId
));
EXTERN S16 cmUnpkLibObdCipherCloseReq ARGS((
LibObdCipherCloseReq func,
Pst *pst,
Buffer *mBuf
));

#endif /* PJ_SEC_ASYNC */

#ifdef KWAC_LIB_SUPPORT
/* AES files */
EXTERN void kwAcAesEncrypt ARGS((
U8 *data,
U32 len,
U8 *key
));
EXTERN Void kwAcAesDecrypt ARGS((
U8 *data,
U32 len,
U8 *key
));
#endif /* KWAC_LIB_SUPPORT */

#ifdef __cplusplus
}
#endif   /* __cplusplus */

#endif /* KW_PDCP */

#endif   /* __KWAC_LIB_X__ */


/**********************************************************************
         End of file:     kwac_lib.x@@/main/TeNB_Main_BR/4 - Thu Apr 24 17:08:31 2014
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---   nm         1. Initial Release.
*********************************************************************91*/

