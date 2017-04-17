/*
  Steps to Add a new Ring Buffer

1. Add Buffer Id to enum in ss_rbuf.h
2. Update Static SsRngCfgTbl below for ring and element size. Add a macro in ss_rbuf.h
3. typedef a structure for element type if not available in ss_rbuf.h
4. Call SCreateSRngBuf for new Buffer id from bcInitRingBuffers() in bc_cpu_init.c
5. Call SAttachSRngBuf from producer and consumer ActvInit
6. Call SEnqSRngBuf and SDeqSRngBuf from producer and consumer respectively
 
*/
#include "envopt.h"        /* environment options     */
#include "envdep.h"        /* environment dependent   */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general                 */
#include "ssi.h"           /* system services         */
#include "cm_err.h"       
#include <stdlib.h>
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "ss_rbuf.h"
#include "ss_rbuf.x"

U32 ssRngBufStatus = 0;

/* Global Ring Loop Up Table */
SsRngBufTbl SsRngInfoTbl[SS_RNG_BUF_MAX];


PUBLIC Void SsRngBufEnable(Void)
{
  ssRngBufStatus = TRUE;
}


PUBLIC Void SsRngBufDisable(Void)
{
  ssRngBufStatus = FALSE;

}
/*
Func: SCreateSRngBuf
Desc: Creates Ring Buffer for the given Id.
      Ring Structure is allocated from given 
      Region and Pool
*/
#ifdef ANSI
PUBLIC S16 SCreateSRngBuf
(
U8 id,  
Region region,
Pool pool,
U32 elmSize,
U32 rngSize
)
#else
PUBLIC S16 SCreateSRngBuf (id, region, pool, elmSize, rngSize)
U8 id; 
Region region;
Pool pool;
U32 elmSize;
U32 rngSize;
#endif
{
   SsRngBuf* ring;

   /* Validate Id */
   if(id >= SS_RNG_BUF_MAX)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, id, "Invalid RBUF ID");
#endif
      RETVALUE(RFAILED);
   }
   if(SsRngInfoTbl[id].r_addr != 0)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, id, 
            "Failed to Create Ring Buffer Id Ring already exist");
#endif
      RETVALUE(RFAILED);
   }
   /* Get Element Size */
   
   /* Get Ring Size    */
   /* Allocate memory for Ring structure */
   ring = (SsRngBuf* )malloc(sizeof(SsRngBuf));
   if(ring == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_INT_PAR, ESSXXX, ERRZERO, "Allocating Ring  Failed!!!")
#endif
      RETVALUE(RFAILED);
   }
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, ring, 
            "Failed to Create Ring Buffer Id Ring already exist");
#endif

   ring->size  = rngSize; /* No empty elem */
   ring->read  = 0;
   ring->write = 0;
   ring->type  = elmSize;

   /* Allocate elements memory */
   ring->elem  = calloc(ring->size, ring->type);
   if(ring->elem == NULLP)
    {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_INT_PAR, ESSXXX, ERRZERO, "Allocating Ring  Failed!!!")
#endif
      free(ring);
      RETVALUE(RFAILED);
    }
    /* Update Buffer Id Table */
    SsRngInfoTbl[id].r_addr   = ring;
    SsRngInfoTbl[id].rngState = SS_RNG_CREATED;
    SsRngInfoTbl[id].n_write  = 0;
    SsRngInfoTbl[id].n_read   = 0;

    printf("Ring Buffer Created with id =%d rSize:%ld eSize:%ld %x\n",id,ring->size,ring->type,(unsigned int)ring);
    RETVALUE(ROK);
}

/*
Func: SAttachSRngBuf
Desc: Attach the calling Entity to a ring buffer 
      as consumer(Rx) or producer (Tx)
*/
#ifdef ANSI
PUBLIC S16 SAttachSRngBuf
(
U8 id,  
U32 ent,
U32 txRx
)
#else
PUBLIC S16 SAttachSRngBuf (id, ent, txRx)
U8 id;
U32 ent;
U32 txRx;
#endif
{
    /* Retrive Buffer from Global Info Table */
    if( id >= SS_RNG_BUF_MAX)
    {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, id, "Invalid RBUF ID");
#endif
       RETVALUE(RFAILED);
    }
    if(SsRngInfoTbl[id].rngState < SS_RNG_CREATED)
    {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESSXXX, id, 
            "Attach Request in Invalid Ring ID");
#endif
       printf("Attach Request in Invalid Ring State %ld id%d \n",
         SsRngInfoTbl[id].rngState,id);
       RETVALUE(RFAILED);
    }
    if(txRx == SS_RNG_TX)
    {
        SsRngInfoTbl[id].txEnt = ent;
        SsRngInfoTbl[id].rngState = SS_RNG_TX_ATTACHED;
    }
    else if(txRx == SS_RNG_RX)
    {
        SsRngInfoTbl[id].rxEnt = ent;
        SsRngInfoTbl[id].rngState = SS_RNG_RX_ATTACHED;
    }
    RETVALUE(ROK);
}
/* 
Func: SConnectSRngBuf
Desc: Establish a pipe between producer and consumer

*/
#ifdef ANSI
PUBLIC S16 SConnectSRngBuf
(
U8 id,  
U32 rxEnt
)
#else
PUBLIC S16 SConnectSRngBuf (id, rxEnt)
U8 id;
U32 rxEnt;
#endif
{
   /* Send to Reciever ENT*/ 
   RETVALUE(ROK); 
}

/*
Func: IsFull
Desc: Checks if Ring is full
*/
inline static S16 IsFull(SsRngBuf* rBuf)
{
#if 0
   /* write+1 == read implies ring is full */
   return ((((rBuf->write + 1)> rBuf->size)?0:(rBuf->write+1))==rBuf->read);
#endif
#if 1
   if((rBuf->write+1) == rBuf->read)
   {
     return 1;
   }
   if((rBuf->write+1) == (rBuf->read + rBuf->size))
   {
     return 1;
   }
   if((rBuf->write + 1) > rBuf->size )
   {
     return 1;
   }
   return 0;
#endif
} 
/*
Func: IsEmpty
Desc: Checks if ring is empty
*/
inline static S16 IsEmpty(SsRngBuf* rBuf)
{
   /* write == read implies Buffer is empty*/
   return (rBuf->write == rBuf->read); 
}

PUBLIC S16 isRngEmpty(U8 id)
{
   return (IsEmpty(SsRngInfoTbl[id].r_addr));
}
/*
Func: SEnqSRngBuf
Desc: Perform Queue operation on Ring bufer
*/
#ifdef ANSI
PUBLIC S16 SEnqSRngBuf 
(
U8 id, 
Void* elem
)
#else
PUBLIC S16 SEnqSRngBuf(id,elem) 
U8 id;
Void* elem;
#endif
{
   U8* w_ptr;
   U8 i=0;
   U8 *element = (U8 *)elem;
   U32 wrIndex;
   /* TBD To replace id with ring addr when SAttachSRngBuf is used*/
   /* Retrive Buffer from Id */
   SsRngBuf* ring = SsRngInfoTbl[id].r_addr;
   if (IsFull(ring))
   {
        SsRngInfoTbl[id].nWriteFail++;
        RETVALUE(RFAILED);
   }
   /* TBD Avoid multiplication for optimisation */
   w_ptr = (U8*)ring->elem + (ring->write * ring->type);
   /* TBD Avoid for loop - use memcpy */
   for( i=0; i < ring->type; i++)
   {
      *(U8*)w_ptr = *(U8*)element;
      w_ptr++;
      (U8*)element++;
   }
   /* Increment write index */
    wrIndex = ring->write + 1 ;
    ring->write = (wrIndex == ring->size)?0: wrIndex;
   /* Update Statistics */
   SsRngInfoTbl[id].n_write++;
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 SGetNumElemInRng
(
U8 id
)
#else
PUBLIC S16 SGetNumElemInRng (id)
U8 id;
#endif
{

   S16 freeDist = ((SsRngInfoTbl[id].n_write- SsRngInfoTbl[id].n_read)/20);
	 if (freeDist < SS_FREE_MGR_MAX_FREE)
	 {
		 freeDist = SS_FREE_MGR_MAX_FREE;
	 }

	RETVALUE(freeDist);
}
/*
Func: SDeqSRngBuf
Desc: Perform DeQueue operation on Ring bufer
*/
#ifdef ANSI
PUBLIC S16 SDeqSRngBuf
(
U8 id,
Void *elem
)
#else
PUBLIC S16 SDeqSRngBuf (id,elem)
U8 id;
Void *elem;
#endif
{
   U8* r_ptr;
   U8 i=0;
   U8 *element = (U8 *)elem;
   U32 rdIndex;
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[id].r_addr;
   if(IsEmpty(ring))
   {  
       SsRngInfoTbl[id].nReadFail++;
       RETVALUE(RFAILED);
   }
   r_ptr = (U8*)ring->elem + (ring->read * ring->type);
   for(i=0; i<ring->type; i++)
   {
      *(U8*)element = *r_ptr;
      (U8*)element++;
      r_ptr++;
   }
   // Avoiding % operation for wrap around
   rdIndex= ring->read + 1;
   ring->read = (rdIndex == ring->size)?0:rdIndex;
   SsRngInfoTbl[id].n_read++;
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 SDestroySRngBuf 
(
U8 id,
Region region,
Pool pool
)
#else
PUBLIC S16 SDestroySRngBuf(id, region, pool)
U8 id;
Region region;
Pool pool;
#endif
{
   /* Retrive Buffer from Id */
   SsRngBuf* ring = SsRngInfoTbl[id].r_addr;
   if(ring)
   {   
      //SPutSBuf(region, pool, (Data *)ring->elem, (ring->size * ring->type));
      //SPutSBuf(region, pool, (Data *)ring, sizeof(SsRngBuf));
      
      free(ring->elem); /* OK if null */ 
      free(ring); /* OK if null */ 

      /* Update Global Info  table */
      SsRngInfoTbl[id].rngState = SS_RNG_DESTROYED;
      SsRngInfoTbl[id].r_addr = 0;
   }
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 SPrintSRngStats
(
Void
)
#else
PUBLIC S16 SPrintSRngStats ()
Void;
#endif
{
   U8 i;
   Txt   prntBuf[100];
   for(i=0; i< SS_RNG_BUF_MAX;i++)
   {
      if(SsRngInfoTbl[i].r_addr != 0 )
      {
         sprintf(prntBuf,"\n=======Ring %d Stats========\n",i);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"r_addr      = %x\n", (unsigned int)SsRngInfoTbl[i].r_addr);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"txEnt       = %x\n", (unsigned int)SsRngInfoTbl[i].txEnt);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"rxEnt       = %x\n", (unsigned int)SsRngInfoTbl[i].rxEnt);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"n_write     = %lu\n", SsRngInfoTbl[i].n_write);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"n_read      = %lu\n", SsRngInfoTbl[i].n_read);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"nWriteFail  = %lu\n", SsRngInfoTbl[i].nWriteFail);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"nReadFail   = %lu\n", SsRngInfoTbl[i].nReadFail);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"pktDrop     = %lu\n", SsRngInfoTbl[i].pktDrop);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"State       = %lu\n\n", SsRngInfoTbl[i].rngState);
         SDisplay(0, prntBuf);
         sprintf(prntBuf,"nPktProc    = %lu\n\n", SsRngInfoTbl[i].nPktProc);
         SDisplay(0, prntBuf);
      }
   }
   RETVALUE(ROK); 
}

#ifdef ANSI
PUBLIC Void* SRngGetWIndx
(
U8 rngId
)
#else
PUBLIC Void* SRngGetWIndx (rngId)
U8 rngId;
#endif
{
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[rngId].r_addr;
   if (IsFull(ring))
   {
      SsRngInfoTbl[rngId].nWriteFail++;
      RETVALUE(NULLP);
   }
   else
   {
      RETVALUE(((U8 *)ring->elem) + (ring->type * ring->write));
   }
}

#ifdef ANSI
PUBLIC Void* SRngGetRIndx
(
U8 rngId
)
#else
PUBLIC Void* SRngGetRIndx (rngId)
U8 rngId;
#endif
{
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[rngId].r_addr;
   if(IsEmpty(ring))
   {
      SsRngInfoTbl[rngId].nReadFail++;
      RETVALUE(NULLP);
   }
   else
   {
      RETVALUE(((U8 *)ring->elem) + (ring->type * ring->read));
   }
}

#ifdef ANSI
PUBLIC Void SRngIncrWIndx
(
U8 rngId
)
#else
PUBLIC Void SRngIncrWIndx (rngId)
U8 rngId;
#endif
{
   U32 wrIndex;
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[rngId].r_addr;
   wrIndex = ring->write + 1;
   ring->write = (wrIndex == ring->size)?0:wrIndex;
   /* Update Statistics */
   SsRngInfoTbl[rngId].n_write++;
}

#ifdef ANSI
PUBLIC Void SRngIncrRIndx
(
U8 rngId
)
#else
PUBLIC Void SRngIncrRIndx (rngId)
U8 rngId;
#endif
{
   U32 rdIndex;
   /* Retrive Buffer from Id*/
   SsRngBuf* ring  = SsRngInfoTbl[rngId].r_addr;
   rdIndex = ring->read + 1;
   ring->read = (rdIndex == ring->size)?0:rdIndex;
   /* Update Statistics */
   SsRngInfoTbl[rngId].n_read++;
}


#if 0
int main(int argc, char **argv) 
{
    SCreateSRngBuf(1,sizeof(SsRngBuf), 10);
    rngElem* buf; 
    /* Fill buffer with test elements 3 times */
    buf = (SsRngBuf*)malloc(sizeof(rngBuf));
    buf->ptr = 1;
    SEnqSRngBuf(1, buf);
    buf = (SsRngBuf*)malloc(sizeof(rngBuf));
    buf->ptr = 2;
    SEnqSRngBuf(1, buf);
    buf = (SsRngBuf*)malloc(sizeof(rngBuf));
    buf->ptr = 3;
    SEnqSRngBuf(1, buf);

   SDeqSRngBuf(1,buf);
   printf("buf 1 = %d",buf->ptr);
   SDeqSRngBuf(1,buf);
   printf("buf 2 = %d",buf->ptr);
   SDeqSRngBuf(1,buf);
   printf("buf 3 = %d",buf->ptr);
 
    return 0;
}
#endif
