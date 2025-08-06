/* GENERATOR MANAGEMENT.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"

#include "beam.h"


/* Creates a new generator and adds it to the front of the generator list
   *GL. Returns a pointer to the new generator, or NULL if a memory
   allocation failure takes place. */

Generator *Gn_Create(GL)
Generator **GL;
{
  Generator *G;

  if (!(G=(Generator *)malloc((unsigned int)sizeof(Generator)))) {
    fprintf(stderr,"Warning: cannot create new generator.\n");
    return NULL;
  }
  G->UserData=NULL;
  G->Nxt=(*GL);
  *GL=G;
  return G;
}


/* Frees the memory associated with the generator *G. */

void Gn_Free(G)
Generator *G;
{
  free(G);
  return;
}


/* Frees the memory used by the generator list GL. */

void Gn_FreeList(GL)
Generator *GL;
{
  Generator *TempG;

  while (GL) {
    TempG=GL->Nxt;
    Gn_Free(GL);
    GL=TempG;
  }
  return;
}
