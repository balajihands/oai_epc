/*
******************************************************************
**       Advanced Encryption Standard implementation in C.      **
**       By Niyaz PK                                            **
**       E-mail: niyazlife@gmail.com                            **
**       Downloaded from Website: www.hoozi.com                 **
******************************************************************
This is the source code for encryption using the latest AES algorithm.
AES algorithm is also called Rijndael algorithm. AES algorithm is 
recommended for non-classified by the National Institute of Standards 
and Technology(NIST), USA. Now-a-days AES is being used for almost 
all encryption applications all around the world.

THE MAIN FEATURE OF THIS AES ENCRYPTION PROGRAM IS NOT EFFICIENCY; IT
IS SIMPLICITY AND READABILITY. THIS SOURCE CODE IS PROVIDED FOR ALL
TO UNDERSTAND THE AES ALGORITHM.

Comments are provided as needed to understand the program. But the 
user must read some AES documentation to understand the underlying 
theory correctly.

It is not possible to describe the complete AES algorithm in detail 
here. For the complete description of the algorithm, point your 
browser to:
// http:www.csrc.nist.gov/publications/fips/fips197/fips-197.pdf 

Find the Wikipedia page of AES at:
// http:en.wikipedia.org/wiki/Advanced_Encryption_Standard 
******************************************************************
*/

/*  Include stdio.h for standard input/output. */
/*  Used for giving output to the screen. */
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
#include "pju.h"           /* PJU defines */
#include "rgu.h"           /* RGU defines */
#include "kwu.h"           /* KWU defines */
#include "cpj.h"           /* CPJ defines */
#include "lpj.h"           /* LPJ defines */
#include "pju.h"           /* PJU defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

#include "cm_xta.h"
#include "kwac_lib.h"
#include "kwac_acc.h"      /* Acceptance defines */
#include "kwac_pju.h"      /* CKW Acceptance defines */

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
#include "pju.x"           /* PJU */
#include "rgu.x"           /* RGU */
#include "kwu.x"           /* KWU */
#include "cpj.x"           /* CPJ */
#include "lpj.x"           /* LPJ */
#include "pju.x"           /* PJU */
#include "kw.x"

#include "cm_xta.x"
#include "kwac_lib.x"
#include "kwac_acc.x"      /* Acceptance defines */
#include "kwac_pju.x"      /* CKW Acceptance defines */


PRIVATE U32 kwAcGetSBoxValue ARGS(( U32 num));
PRIVATE Void kwAcKeyExpansion ARGS((Void));
PRIVATE Void kwAcAddRoundKey ARGS(( U32 round)); 
PRIVATE Void kwAcSubBytes ARGS((Void));
PRIVATE Void kwAcShiftRows ARGS((Void));
PRIVATE Void kwAcMixColumns ARGS((Void));
PRIVATE Void kwAcCipher ARGS((Void));
PUBLIC void mainAPI ARGS((Void));
/*  The number of rounds in AES Cipher. It is simply initiated to zero. The actual value is recieved in the program. */
U32 Nr=0;

/*  The number of 32 bit words in the key. It is simply initiated to zero. The actual value is recieved in the program. */
U32 Nk=0;

/*  in - it is the array that holds the plain text to be encrypted. */
/*  out - it is the array that holds the key for encryption. */
/*  state - the array that holds the intermediate results during encryption. */
U8 in[16], out[16], state[4][4];

/*  The array that stores the round keys. */
U8 RoundKey[240];

/*  The Key input to the AES Program */
U8 Key[32];

#ifdef ANSI
PRIVATE U32 kwAcGetSBoxValue
(
U32 num
)
#else
PRIVATE U32 kwAcGetSBoxValue(num)
U32 num;
#endif
{
   U32 sbox[256] =   {
   /* 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F */
   0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, /*0 */
   0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, /*1 */
   0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, /*2 */
   0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, /*3 */
   0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, /*4 */
   0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, /*5 */
   0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, /*6 */
   0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, /*7 */
   0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, /*8 */
   0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, /*9 */
   0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, /*A */
   0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, /*B */
   0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, /*C */
   0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, /*D */
   0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, /*E */
   0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };/* F */
   return sbox[num];
}

/*  The round constant word array, Rcon[i], contains the values given by  */
/*  x to th e power (i-1) being powers of x (x is denoted as {02}) in the field GF(28) */
/*  Note that i starts at 1, not 0). */
U32 Rcon[255] = {
   0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
   0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
   0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
   0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
   0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
   0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
   0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
   0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
   0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
   0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
   0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
   0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
   0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
   0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
   0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
   0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };

/*  This function produces Nb(Nr+1) round keys. The round keys are used in each round to encrypt the states.  */
#ifdef ANSI
PRIVATE Void kwAcKeyExpansion
(
)
#else
PRIVATE Void kwAcKeyExpansion()
#endif
{
   U32 i,j;
   U8 temp[4],k;
   
   /*  The first round key is the key itself. */
   for(i=0;i<Nk;i++)
   {
      RoundKey[i*4]=Key[i*4];
      RoundKey[i*4+1]=Key[i*4+1];
      RoundKey[i*4+2]=Key[i*4+2];
      RoundKey[i*4+3]=Key[i*4+3];
   }

   /*  All other round keys are found from the previous round keys. */
   while (i < (Nb * (Nr+1)))
   {
      for(j=0;j<4;j++)
      {
         temp[j]=RoundKey[(i-1) * 4 + j];
      }
      if (i % Nk == 0)
      {
         /*  This function rotates the 4 bytes in a word to the left once. */
         /*  [a0,a1,a2,a3] becomes [a1,a2,a3,a0] */

         /*  Function RotWord() */
         {
            k = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = k;
         }

         /*  SubWord() is a function that takes a four-byte input word and  */
         /*  applies the S-box to each of the four bytes to produce an output word. */

         /*  Function Subword() */
         {
            temp[0]=kwAcGetSBoxValue(temp[0]);
            temp[1]=kwAcGetSBoxValue(temp[1]);
            temp[2]=kwAcGetSBoxValue(temp[2]);
            temp[3]=kwAcGetSBoxValue(temp[3]);
         }

         temp[0] =  temp[0] ^ Rcon[i/Nk];
      }
      else if (Nk > 6 && i % Nk == 4)
      {
         /*  Function Subword() */
         {
            temp[0]=kwAcGetSBoxValue(temp[0]);
            temp[1]=kwAcGetSBoxValue(temp[1]);
            temp[2]=kwAcGetSBoxValue(temp[2]);
            temp[3]=kwAcGetSBoxValue(temp[3]);
         }
      }
      RoundKey[i*4+0] = RoundKey[(i-Nk)*4+0] ^ temp[0];
      RoundKey[i*4+1] = RoundKey[(i-Nk)*4+1] ^ temp[1];
      RoundKey[i*4+2] = RoundKey[(i-Nk)*4+2] ^ temp[2];
      RoundKey[i*4+3] = RoundKey[(i-Nk)*4+3] ^ temp[3];
      i++;
   }
}

/*  This function adds the round key to state. */
/*  The round key is added to the state by an XOR function. */
#ifdef ANSI
PRIVATE Void kwAcAddRoundKey
(
U32 round
) 
#else
PRIVATE Void kwAcAddRoundKey(round) 
U32 round;
#endif
{
   U32 i,j;
   for(i=0;i<4;i++)
   {
      for(j=0;j<4;j++)
      {
         state[j][i] ^= RoundKey[round * Nb * 4 + i * Nb + j];
      }
   }
}

/*  The SubBytes Function Substitutes the values in the */
/*  state matrix with values in an S-box. */
#ifdef ANSI
PRIVATE Void kwAcSubBytes
(
)
#else
PRIVATE Void kwAcSubBytes()
#endif
{
   U32 i,j;
   for(i=0;i<4;i++)
   {
      for(j=0;j<4;j++)
      {
         state[i][j] = kwAcGetSBoxValue(state[i][j]);

      }
   }
}

/*  The ShiftRows() function shifts the rows in the state to the left. */
/*  Each row is shifted with different offset. */
/*  Offset = Row number. So the first row is not shifted. */
#ifdef ANSI
PRIVATE Void kwAcShiftRows
(
)
#else
PRIVATE Void kwAcShiftRows()
#endif
{
   U8 temp;

   /*  Rotate first row 1 columns to left   */
   temp=state[1][0];
   state[1][0]=state[1][1];
   state[1][1]=state[1][2];
   state[1][2]=state[1][3];
   state[1][3]=temp;

   /*  Rotate second row 2 columns to left  */
   temp=state[2][0];
   state[2][0]=state[2][2];
   state[2][2]=temp;

   temp=state[2][1];
   state[2][1]=state[2][3];
   state[2][3]=temp;

   /*  Rotate third row 3 columns to left */
   temp=state[3][0];
   state[3][0]=state[3][3];
   state[3][3]=state[3][2];
   state[3][2]=state[3][1];
   state[3][1]=temp;
}

/*  MixColumns function mixes the columns of the state matrix */
#ifdef ANSI
PRIVATE Void kwAcMixColumns
(
)
#else
PRIVATE Void kwAcMixColumns()
#endif
{
   U32 i;
   U8 Tmp,Tm,t;
   for(i=0;i<4;i++)
   {  
      t=state[0][i];
      Tmp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i] ;
      Tm = state[0][i] ^ state[1][i] ; Tm = XTIME(Tm); state[0][i] ^= Tm ^ Tmp ;
      Tm = state[1][i] ^ state[2][i] ; Tm = XTIME(Tm); state[1][i] ^= Tm ^ Tmp ;
      Tm = state[2][i] ^ state[3][i] ; Tm = XTIME(Tm); state[2][i] ^= Tm ^ Tmp ;
      Tm = state[3][i] ^ t ; Tm = XTIME(Tm); state[3][i] ^= Tm ^ Tmp ;
   }
}

/*  Cipher is the main function that encrypts the PlainText. */
#ifdef ANSI
PRIVATE Void kwAcCipher
(
)
#else
PRIVATE Void kwAcCipher()
#endif
{
   U32 i,j,round=0;

   /* Copy the input PlainText to state array. */
   for(i=0;i<4;i++)
   {
      for(j=0;j<4;j++)
      {
         state[j][i] = in[i*4 + j];
      }
   }

   /*  Add the First round key to the state before starting the rounds. */
   kwAcAddRoundKey(0); 
   
   /*  There will be Nr rounds. */
   /*  The first Nr-1 rounds are identical. */
   /*  These Nr-1 rounds are executed in the loop below. */
   for(round=1;round<Nr;round++)
   {
      kwAcSubBytes();
      kwAcShiftRows();
      kwAcMixColumns();
      kwAcAddRoundKey(round);
   }
   
   /*  The last round is given below. */
   /*  The MixColumns function is not here in the last round. */
   kwAcSubBytes();
   kwAcShiftRows();
   kwAcAddRoundKey(Nr);

   /*  The encryption process is over. */
   /*  Copy the state array to output array. */
   for(i=0;i<4;i++)
   {
      for(j=0;j<4;j++)
      {
         out[i*4+j]=state[j][i];
      }
   }
}

#ifdef ANSI
PUBLIC void kwAcAesEncrypt
(
U8 *data,
U32 len,
U8 *key
)
#else
PUBLIC void kwAcAesEncrypt(data, len, key)
U8 *data;
U32 len;
U8 *key;
#endif
{
   U32 i;

   U32 inpLen;
   U32 outLen;

   /*  Calculate Nk and Nr from the recieved value. */
   Nr =  128; /* size of key */
   Nk = Nr / 32;
   Nr = Nk + 6;
   inpLen   =  0;
   outLen   =  0;
   
   while(inpLen !=  len)
   {
      /*  The array temp stores the key. */
      /*  The array temp2 stores the plaintext. */
      /*  Copy the Key and PlainText */
      cmMemset((U8 *)in, 0, 32);

      for(i=0;i<Nk*4;i++)
      {
         Key[i]=key[i];
      }

      for(i=0;i<Nk*4;i++)
      {
         in[i]=data[inpLen++];
         if(inpLen == len)
         {
            break;
         }
      }

      /*  The KeyExpansion routine must be called before encryption. */
      kwAcKeyExpansion();

      /*  The next function call encrypts the PlainText with the Key using AES algorithm. */
      kwAcCipher();

      for(i=0;i<Nb*4;i++)
      {
         data[i] = out[outLen++];
         if(outLen == len)
         {
            break;
         }
      }
   }
}
