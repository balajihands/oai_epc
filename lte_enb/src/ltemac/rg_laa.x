



/************************************************************************

     Name:     LTE-MAC layer

     Type:     C source file

     Desc:     

     File:     

     Sid:      

     Prg:           

**********************************************************************/

/** @brief Request from PMAC to SMAC to request LAA TBs. */
EXTERN S16 RgPrgPMacSMacLaaTbReq ARGS
   ((
     Pst         *pst,    
     RgPrgLaaTbReq *laaTbReq
    ));


/** @brief Indication from SMAC to PMAC with requested LAA TBs. */
EXTERN S16 RgPrgSMacPMacLaaTbInd ARGS
   ((
     Pst         *pst,    
     RgPrgLaaTbInd *laaTbInd
    ));


/**********************************************************************

         End of file:     $SID$

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
*********************************************************************91*/

