
/********************************************************************20**

     Name:     EnodeB Application

     Type:     C include file

     Desc:     This file contains 
               

     File:     wr_umm_dlnas.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_UMM_DLNAS_H__
#define __WR_UMM_DLNAS_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

typedef struct wrUmmDlNasTransCb
{
   WrUmmIncMsg               *msg;
   TknStrOSXL                *nasPdu;
} WrUmmDlNasTransCb;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* Enf of __WR_UMM_DLNAS_H__ */

/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
