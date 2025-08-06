/* SURFACE MANAGEMENT.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"

#include "solid.h"


/* Creates a new surface and adds it to the front of the surface list
   *SL. Returns a pointer to the new surface, or NULL if a memory
   allocation failure takes place. */

Surface *Su_Create(SL)
Surface **SL;
{
  Surface *S;

  if (!(S=(Surface *)malloc((unsigned int)sizeof(Surface)))) {
    fprintf(stderr,"Warning: cannot create new surface.\n");
    return NULL;
  }
  S->Front=S->Back=NULL;
  S->Sd=S->LastSd=NULL;
  S->UserData=NULL;
  S->Nxt=(*SL);
  *SL=S;
  return S;
}


/* Adds the shared vertex *V to the vertex list of the surface *S.
   FAILURE is returned only in case of a memory allocation error. */

RETCODE Su_AddVertex(S,V)
Surface *S;
Vertex  *V;
{
  Side *Sd;

  if (!(Sd=(Side *)malloc((unsigned int)sizeof(Side)))) {
    fprintf(stderr,"Warning: no more memory for surface side.\n");
    return FAILURE;
  }
  Sd->V=V;
  Sd->Nxt=NULL;
  if (!(S->LastSd)) {
    S->LastSd=Sd;
    S->Sd=Sd;
  } else {
    S->LastSd->Nxt=Sd;
    S->LastSd=Sd;
  }
  return SUCCESS;
}


/* Produces the standard polygon representation of surface *S
   (equations of the polygon and side planes). The function presumes the
   validity of the surface vertices (e.g. their number must be at least
   3, all vertices should lie on the same plane, etc.). */

#define VERTEX(S) &(((S)->V)->V) /* Shorthand. */

void Su_SetPlanes(S)
Surface *S;
{
  Side      *Sd0,*Sd1,*Sdi_1;
  Cartesian s0,s1,si_1;
  Cartesian *u;
  Cartesian *V0,*V1,*Vi_1,*Vi;

  u=(&((S->P).u));                          /* P's equation. */
  Sd0=S->Sd;
  V0=VERTEX(Sd0);
  Sd1=Sd0->Nxt;
  V1=VERTEX(Sd1);
  Sdi_1=Sd1->Nxt;
  Vi_1=VERTEX(Sdi_1);
  V_Sub(V1,V0,&s0);
  V_Sub(Vi_1,V1,&s1);
  V_Cross(&s1,&s0,u);
  V_Unit(u,u);
  (S->P).d=V_Dot(V0,u);

  V_Cross(u,&s0,&((Sd0->Pi).u));            /* P0 and P1. */
  V_Unit(&((Sd0->Pi).u),&((Sd0->Pi).u));
  (Sd0->Pi).d=V_Dot(V0,&((Sd0->Pi).u));

  V_Cross(u,&s1,&((Sd1->Pi).u));
  V_Unit(&((Sd1->Pi).u),&((Sd1->Pi).u));
  (Sd1->Pi).d=V_Dot(V1,&((Sd1->Pi).u));

  while (Sdi_1->Nxt) {                      /* Other vertices. */
    Vi=VERTEX(Sdi_1->Nxt);
    V_Sub(Vi,Vi_1,&si_1);
    V_Cross(u,&si_1,&((Sdi_1->Pi).u));
    V_Unit(&((Sdi_1->Pi).u),&((Sdi_1->Pi).u));
    (Sdi_1->Pi).d=V_Dot(Vi_1,&((Sdi_1->Pi).u));
    Sdi_1=Sdi_1->Nxt;
    Vi_1=Vi;
  }

  V_Sub(V0,Vi_1,&si_1);                     /* Last vertex (k=i-1). */
  V_Cross(u,&si_1,&((Sdi_1->Pi).u));
  V_Unit(&((Sdi_1->Pi).u),&((Sdi_1->Pi).u));
  (Sdi_1->Pi).d=V_Dot(Vi_1,&((Sdi_1->Pi).u));    
  return;
}


/* Frees the memory associated with the surface *S. */

void Su_Free(S)
Surface *S;
{
  Side *TempSd;

  while (TempSd=S->Sd) {
    S->Sd=S->Sd->Nxt;
    VS_Remove(TempSd->V);
    free(TempSd);
  }
  free(S);
  return;
}


/* Frees the memory used by the surface list SL. */

void Su_FreeList(SL)
Surface *SL;
{
  Surface *TempS;

  while (SL) {
    TempS=SL->Nxt;
    Su_Free(SL);
    SL=TempS;
  }
  return;
}
