/* EXTENDED GENERATOR MANAGEMENT.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"
#include "beam.h"
#include "strsp.h"

#include "reader.h"


/* Creates a new extended generator and adds it to the front of the
   extended generator list *GL. Returns a pointer to the new extended
   generator, or NULL if a memory allocation failure takes place. */

Generator *GnExt_Create(GL)
Generator **GL;
{
  Generator *G;

  if (!(G=Gn_Create(GL)))
    return NULL;
  if (!(G->UserData=(void *)malloc((unsigned int)sizeof(GeneratorExt)))) {
    fprintf(stderr,"Warning: no more memory for extended generator.\n");
    (*GL)=G->Nxt;
    Gn_Free(G);
    return NULL;
  }
  GENERATOR_EXT(G)->UserData=NULL;
  return G;
}


/* Frees the memory associated with the extended generator *G. */

void GnExt_Free(G)
Generator *G;
{
  free(GENERATOR_EXT(G));
  Gn_Free(G);
  return;
}


/* Frees the memory used by the extended generator list GL. */

void GnExt_FreeList(GL)
Generator *GL;
{
  Generator *TempG;

  while (GL) {
    TempG=GL->Nxt;
    GnExt_Free(GL);
    GL=TempG;
  }
  return;
}
