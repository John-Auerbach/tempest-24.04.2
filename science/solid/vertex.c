/* VERTEX SPACE MANAGEMENT.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"

#include "solid.h"


/* Adds the vertex *V to the vertex space *VS, if it does not exist.
   It returns a pointer to the shared vertex whose location is *V. NULL
   is returned in case of memory allocation failure. */

Vertex *VS_Add(VS,V)
Vertex    **VS;
Cartesian *V;
{
  Vertex *SV;

  /* Try to find *V in current string space. */

  SV=(*VS);
  while (SV) {
    if (SV->V.X==V->X && SV->V.Y==V->Y && SV->V.Z==V->Z)
      return VS_Copy(SV);
    SV=SV->Nxt;
  }

  /* Create new vector space element. */

  if (!(SV=(Vertex *)malloc((unsigned int)sizeof(Vertex)))) {
    fprintf(stderr,"Warning: no more memory for new vertex.\n");
    return NULL;
  }
  SV->RefCount=1;
  SV->V=(*V);
  SV->UserData=NULL;

  /* Add to vector space. */

  SV->Nxt=(*VS);
  *VS=SV;
  return SV;
}


/* Increases the reference count of the vertex *SV. */

Vertex *VS_Copy(SV)
Vertex *SV;
{
  SV->RefCount++;
  return SV;
}


/* Reduces the reference count of the vertex *SV. */

void VS_Remove(SV)
Vertex *SV;
{
  SV->RefCount--;
  return;
}


/* Frees all unreferenced vertices from the vertex space *VS. This
   function should be called periodically to make sure that no memory is
   wasted on unused vertices. */

void VS_Cleanup(VS)
Vertex **VS;
{
  Vertex *TempV;

  while (*VS)
    if (!((*VS)->RefCount)) {
      TempV=(*VS);
      *VS=TempV->Nxt;
      free(TempV);
    } else
      VS=(&((*VS)->Nxt));
  return;
}
