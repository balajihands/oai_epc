
/**********************************************************************
     Name:     LTE-PHY layer 
  
     Type:     C include file 
  
     Desc:     Defines required by the LTE PHY-RRC control (CTF) interface.

     File:     ctf.h 

     Sid:      ctf.h@@/main/3 - Thu Feb 16 19:38:59 2012

     Prg:      vkulkarni

**********************************************************************/
#ifndef __CTF_H__
#define __CTF_H__

/** 
 * @file
 * @brief Defines for CTF interface.
*/
/* ctf_h_001.main_1  Wireshark Compilation issue */
#ifdef WIRESHARK_LOG
#define EVTCTFWIRESHARK 8
#endif
/* selva end*/
/** Bind request event */
#define EVTCTFBNDREQ   1
/** Bind confirm event */
#define EVTCTFBNDCFM   2
/** Unbind request event */
#define EVTCTFUBNDREQ  3
/** Configuration request event */
#define EVTCTFCFGREQ   4
/** Configuration Confirm event */
#define EVTCTFCFGCFM   5
/** UE Id Change Request event */
#define EVTCTFUEIDCHGREQ  6
/** UE Id Change Request event */
#define EVTCTFUEIDCHGCFM  7

/** Configuration Confirm - Positive confirmation: config successful. */
#define CTF_CFG_CFM_OK  1
/** Configuration Confirm - Negative confirmation: config failed. */
#define CTF_CFG_CFM_NOK 2

/** Configuration type : CONFIG/SETUP (cfgType in CtfCfgReq). */
#define CTF_CONFIG   1
/** Configuration type : RECONFIG/MODIFY (cfgType in CtfCfgReq). */
#define CTF_RECONFIG 2
/** Configuration type : DELETE/RELEASE (cfgType in CtfCfgReq). */
#define CTF_DELETE   3

/** Configuration element type : Cell (cfgElem in CtfCfgInfo/CtfReCfgInfo). */
#define CTF_CELL_CFG 1
/** Configuration element type : UE (cfgElem in CtfCfgInfo/CtfReCfgInfo). */
#define CTF_UE_CFG   2

/** Configuration type for any sub-element : Setup */
#define CTF_IE_CFG_SETUP     1
/** Configuration type for any sub-element : Release */
#define CTF_IE_CFG_RELEASE   2

/** CQI reporting mode configuration type : Periodic
 *  (reportingMode in CtfCqiReportCfgInfo).
*/
#define CTF_CQI_RPTMODE_PRDIOC   1
/** CQI reporting mode configuration type : Aperiodic
 *  (reportingMode in CtfCqiReportCfgInfo).
*/
#define CTF_CQI_RPTMODE_APRDIOC  2

/** Periodic CQI format indicator configuration type : Wideband 
 *  (formatIndicator in CtfCqiRptModePeriodic).
*/
#define CTF_CQI_PRDIC_FMT_IND_WIDEBAND    1
/** Periodic CQI format indicator configuration type : Subband 
 *  (formatIndicator in CtfCqiRptModePeriodic).
*/
#define CTF_CQI_PRDIC_FMT_IND_SUBBAND     2


#define CTF_CFG_TRANSID_SIZE 9  /*!< Macro for Transaction ID size */

/* selector(coupling) values */
#define CTF_SEL_LC     0     /*!< Macro for Light Coupling */
#define CTF_SEL_TC     1     /*!< Macro for Tight Coupling */
#define CTF_SEL_LWLC   2     /*!< Macro for Light Weight Loose Coupling */


/* pack unpack error code */
#define   ECTFXXX      0 /* reserved */

#define ERRCTF                  0

#define ECTF001         (ERRCTF + 1)
#define ECTF002         (ERRCTF + 2)
#define ECTF003         (ERRCTF + 3)
#define ECTF004         (ERRCTF + 4)
#define ECTF005         (ERRCTF + 5)
#define ECTF006         (ERRCTF + 6)
#define ECTF007         (ERRCTF + 7)
#define ECTF008         (ERRCTF + 8)
#define ECTF009         (ERRCTF + 9)
#define ECTF010         (ERRCTF + 10)
#define ECTF011         (ERRCTF + 11)
#define ECTF012         (ERRCTF + 12)
#define ECTF013         (ERRCTF + 13)
#define ECTF014         (ERRCTF + 14)
#define ECTF015         (ERRCTF + 15)
#define ECTF016         (ERRCTF + 16)
#define ECTF017         (ERRCTF + 17)
#define ECTF018         (ERRCTF + 18)
#define ECTF019         (ERRCTF + 19)
#define ECTF020         (ERRCTF + 20)
#define ECTF021         (ERRCTF + 21)
#define ECTF022         (ERRCTF + 22)
#define ECTF023         (ERRCTF + 23)
#define ECTF024         (ERRCTF + 24)
#define ECTF025         (ERRCTF + 25)
#define ECTF026         (ERRCTF + 26)
#define ECTF027         (ERRCTF + 27)
#define ECTF028         (ERRCTF + 28)
#define ECTF029         (ERRCTF + 29)
#define ECTF030         (ERRCTF + 30)
#define ECTF031         (ERRCTF + 31)
#define ECTF032         (ERRCTF + 32)
#define ECTF033         (ERRCTF + 33)
#define ECTF034         (ERRCTF + 34)
#define ECTF035         (ERRCTF + 35)
#define ECTF036         (ERRCTF + 36)
#define ECTF037         (ERRCTF + 37)
#define ECTF038         (ERRCTF + 38)
#define ECTF039         (ERRCTF + 39)
#define ECTF040         (ERRCTF + 40)
#define ECTF041         (ERRCTF + 41)
#define ECTF042         (ERRCTF + 42)
#define ECTF043         (ERRCTF + 43)
#define ECTF044         (ERRCTF + 44)
#define ECTF045         (ERRCTF + 45)
#define ECTF046         (ERRCTF + 46)
#define ECTF047         (ERRCTF + 47)
#define ECTF048         (ERRCTF + 48)
#define ECTF049         (ERRCTF + 49)
#define ECTF050         (ERRCTF + 50)
#define ECTF051         (ERRCTF + 51)
#define ECTF052         (ERRCTF + 52)
#define ECTF053         (ERRCTF + 53)
#define ECTF054         (ERRCTF + 54)
#define ECTF055         (ERRCTF + 55)
#define ECTF056         (ERRCTF + 56)
#define ECTF057         (ERRCTF + 57)
#define ECTF058         (ERRCTF + 58)
#define ECTF059         (ERRCTF + 59)
/* ctf_h_001.main_2: Added new error code. */
#define ECTF060         (ERRCTF + 60)
#define ECTF061         (ERRCTF + 61)

#endif /* __CTF_H__ */


/**********************************************************************
  
         End of file:     ctf.h@@/main/3 - Thu Feb 16 19:38:59 2012
  
**********************************************************************/

/**********************************************************************
  
        Notes:
  
**********************************************************************/

/**********************************************************************

**********************************************************************/


/**********************************************************************
  
        Revision history:
  
**********************************************************************/
  
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---   vkulkarni         1. Initial release.
/main/1     ctf_h_001.main_1   sgm   1. Wireshark compilation fix. 
/main/3      ---      ctf_h_001.main_2   ragrawal  1. Added new error code.
*********************************************************************91*/
