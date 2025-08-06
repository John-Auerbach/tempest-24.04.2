/* CAMERA MANAGEMENT.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"
#include "beam.h"
#include "strsp.h"

#include "reader.h"


/* Creates a new camera and adds it to the front of the camera list
   *CL. Returns a pointer to the new camera, or NULL if a memory
   allocation failure takes place. */

Camera *Cm_Create(CL)
Camera **CL;
{
  Camera *C;

  if (!(C=(Camera *)malloc((unsigned int)sizeof(Camera)))) {
    fprintf(stderr,"Warning: cannot create new camera.\n");
    return NULL;
  }
  C->UserData=NULL;
  C->Nxt=(*CL);
  *CL=C;
  return C;
}


/* Frees the memory associated with the camera *C. */

void Cm_Free(C)
Camera *C;
{
  free(C);
  return;
}


/* Frees the memory used by the camera list CL. */

void Cm_FreeList(CL)
Camera *CL;
{
  Camera *TempC;

  while (CL) {
    TempC=CL->Nxt;
    Cm_Free(CL);
    CL=TempC;
  }
  return;
}
