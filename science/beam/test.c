/* TESTER OF ELECTRON BEAM TRACER.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"

#include "beam.h"


void ShowSurface(S)
Surface *S;
{
  Side *Sd;

  printf("Surface: %x\n",S);
  printf("Plane: u %lf %lf %lf d %lf\n",
         S->P.u.X,S->P.u.Y,S->P.u.Z,S->P.d);
  printf("Vertices:\n");
  Sd=S->Sd;
  while (Sd) {
    printf(" Vertex: %lf %lf %lf\n  Side Plane: u %lf %lf %lf d %lf\n",
           Sd->V->V.X,Sd->V->V.Y,Sd->V->V.Z,
           Sd->Pi.u.X,Sd->Pi.u.Y,Sd->Pi.u.Z,Sd->Pi.d);
    Sd=Sd->Nxt;
  }
  puts("");
  return;
}


main()
{
  Vertex *VS=NULL;
  Body   B;
  Body   *BL=NULL;

  Cartesian C,BField;
  Vertex    *V;
  Surface   *S;
  Generator G;
  BOOLEAN   Hit;

  B.S=NULL;
  B.Nxt=NULL;

  S=Su_Create(&B.S);
  S->Originator=S;

  C.X=0.5; C.Y=1.0; C.Z=10.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  C.X=0.5; C.Y=-1.0; C.Z=10.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  C.X=0.5; C.Y=-1.0; C.Z=0.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  C.X=0.5; C.Y=1.0; C.Z=0.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  Su_SetPlanes(S);
  ShowSurface(S);

  G.Loc.X=G.Loc.Y=G.Loc.Z=0.0;
  G.Potential=1000.0;
  G.Coelevation=DTR(90.0);
  G.Azimuth=DTR(5.0);
  BField.X=5E-5; BField.Y=BField.Z=0.0;
  Hit=Beam_HitBodyList(&BField,&G,&B);
  if (Hit) {
    printf("Hit surface %x\n",G.ImpactS);
    Beam_Point(&G,G.ImpactT,&C);
    printf(" at %lf %lf %lf.\n",C.X,C.Y,C.Z);
  } else
    puts("No hit");
  printf("%x\n",Beam_Cylinder(&G,10,15.0,&BL,&VS));
}
