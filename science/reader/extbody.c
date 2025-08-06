/* EXTENDED BODY MANAGEMENT.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"
#include "beam.h"
#include "strsp.h"

#include "reader.h"


/* Creates a new extended body and adds it to the front of the
   extended body list *BL. Returns a pointer to the new extended
   body, or NULL if a memory allocation failure takes place. */

Body *BdExt_Create(BL)
Body **BL;
{
  Body *B;

  if (!(B=Bd_Create(BL)))
    return NULL;
  if (!BdExt_Extend(B)) {
    (*BL)=B->Nxt;
    Bd_Free(B);
    return NULL;
  }
  return B;
}


/* Extends the body *B. Returns FAILURE iff a memory allocation
   failure takes place. */

RETCODE BdExt_Extend(B)
Body *B;
{
  if (!(B->UserData=(void *)malloc((unsigned int)sizeof(BodyExt)))) {
    fprintf(stderr,"Warning: no more memory for extended body.\n");
    return FAILURE;
  }
  BODY_EXT(B)->UserData=NULL;
  return SUCCESS;
}


/* Frees the memory associated with the extended body *B. */

void BdExt_Free(B)
Body *B;
{
  free(BODY_EXT(B));
  SuExt_FreeList(B->S);
  B->S=NULL;
  Bd_Free(B);
  return;
}


/* Frees the memory used by the extended body list BL. */

void BdExt_FreeList(BL)
Body *BL;
{
  Body *TempB;

  while (BL) {
    TempB=BL->Nxt;
    BdExt_Free(BL);
    BL=TempB;
  }
  return;
}
