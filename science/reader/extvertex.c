/* EXTENDED VERTEX SPACE MANAGEMENT.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"
#include "beam.h"
#include "strsp.h"

#include "reader.h"


/* Adds the extended vertex *V to the extended vertex space *VS, if it
   does not exist.  It returns a pointer to the shared extended vertex
   whose location is *V. NULL is returned in case of memory allocation
   failure. */

Vertex *VSExt_Add(VS,V)
Vertex    **VS;
Cartesian *V;
{
  Vertex *SV;

  if (!(SV=VS_Add(VS,V)))
    return NULL;
  if (!SV->UserData && !VSExt_Extend(SV)) {
    VS_Remove(SV);
    return NULL;
  }
  return SV;
}


/* Adds the extension to the shared vertex *SV. Returns FAILURE iff a
   memory allocation failure takes place. */

RETCODE VSExt_Extend(SV)
Vertex *SV;
{
  if (!(SV->UserData=(void *)malloc((unsigned int)sizeof(VertexExt)))) {
    fprintf(stderr,"Warning: no more memory for extended vertex.\n");
    return FAILURE;
  }
  VERTEX_EXT(SV)->UserData=NULL;
  return SUCCESS;
}


/* Frees all unreferenced vertices from the extended vertex space *VS.
   This function should be called periodically to make sure that no
   memory is wasted on unused extended vertices. */

void VSExt_Cleanup(VS)
Vertex **VS;
{
  Vertex *TempV;

  TempV=(*VS);
  while (TempV) {
    if (!(TempV->RefCount))
      free(VERTEX_EXT(TempV));
    TempV=TempV->Nxt;
  }
  VS_Cleanup(VS);
  return;
}
