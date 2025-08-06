/* EXTENDED SURFACE MANAGEMENT.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"
#include "beam.h"
#include "strsp.h"

#include "reader.h"


/* Creates a new extended surface and adds it to the front of the
   extended surface list *SL. Returns a pointer to the new extended
   surface, or NULL if a memory allocation failure takes place. */

Surface *SuExt_Create(SL)
Surface **SL;
{
  Surface *S;

  if (!(S=Su_Create(SL)))
    return NULL;
  if (!SuExt_Extend(S)) {
    (*SL)=S->Nxt;
    Su_Free(S);
    return NULL;
  }
  return S;
}


/* Extends the surface *S. Returns FAILURE iff a memory allocation
   failure takes place. */

RETCODE SuExt_Extend(S)
Surface *S;
{
  if (!(S->UserData=(void *)malloc((unsigned int)sizeof(SurfaceExt)))) {
    fprintf(stderr,"Warning: no more memory for extended surface.\n");
    return FAILURE;
  }
  SURFACE_EXT(S)->Location=NULL;
  SURFACE_EXT(S)->UserData=NULL;
  return SUCCESS;
}


/* Frees the memory associated with the extended surface *S. */

void SuExt_Free(S)
Surface *S;
{
  if (SURFACE_EXT(S)->Location)
    SS_Remove(SURFACE_EXT(S)->Location);
  free(SURFACE_EXT(S));
  Su_Free(S);
  return;
}


/* Frees the memory used by the extended surface list SL. */

void SuExt_FreeList(SL)
Surface *SL;
{
  Surface *TempS;

  while (SL) {
    TempS=SL->Nxt;
    SuExt_Free(SL);
    SL=TempS;
  }
  return;
}
