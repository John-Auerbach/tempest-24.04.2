/* BODY MANAGEMENT.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"

#include "solid.h"


/* Creates a new body and adds it to the front of the body list
   *BL. Returns a pointer to the new body, or NULL if a memory
   allocation failure takes place. */

Body *Bd_Create(BL)
Body **BL;
{
  Body *B;

  if (!(B=(Body *)malloc((unsigned int)sizeof(Body)))) {
    fprintf(stderr,"Warning: cannot create new body.\n");
    return NULL;
  }
  B->S=NULL;
  B->UserData=NULL;  
  B->Nxt=(*BL);
  *BL=B;
  return B;
}


/* Frees the memory associated with the body *B. */

void Bd_Free(B)
Body *B;
{
  Su_FreeList(B->S);
  free(B);
  return;
}


/* Frees the memory used by the body list BL. */

void Bd_FreeList(BL)
Body *BL;
{
  Body *TempB;

  while (BL) {
    TempB=BL->Nxt;
    Bd_Free(BL);
    BL=TempB;
  }
  return;
}
