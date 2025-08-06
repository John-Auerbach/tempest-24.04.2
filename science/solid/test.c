/* SOLID OBJECT MANIPULATION ROUTINES' DEVELOPMENT PLATFORM.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"

#include "solid.h"

void ShowSurface(S)
Surface *S;
{
  Side *Sd;

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

void ShowVS(VS)
Vertex *VS;
{
  while (VS) {
    printf("%d; %d; %lf %lf %lf\n",
	   VS,VS->RefCount,VS->V.X,VS->V.Y,VS->V.Z);
    VS=VS->Nxt;
  }
  puts("");
  return;
}

void ShowSL(SL)
Surface *SL;
{
  while (SL) {
    printf("Surface %x: ",SL);
    ShowSurface(SL);
    SL=SL->Nxt;
  }
  puts("");
  return;
}

BOOLEAN ShowBSPSurface(S)
Surface *S;
{
  printf("Surface: %x Originator: %x\n",S,S->Originator);
  return TRUE;
}


main()
{
  Vertex *VS=NULL;
  Body   B;

  BSPTree   *BSP;
  Cartesian C;
  Vertex    *V;
  Surface   *S;

  int NSplit;

  B.S=NULL;

  S=Su_Create(&(B.S));
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


  S=Su_Create(&(B.S));
  S->Originator=S;

  C.X=0.0; C.Y=1.0; C.Z=2.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  C.X=1.0; C.Y=1.0; C.Z=2.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  C.X=1.0; C.Y=0.0; C.Z=2.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  C.X=0.0; C.Y=0.0; C.Z=2.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  Su_SetPlanes(S);


  S=Su_Create(&(B.S));
  S->Originator=S;

  C.X=0.0; C.Y=1.0; C.Z=2.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  C.X=1.0; C.Y=1.0; C.Z=8.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  C.X=1.0; C.Y=0.0; C.Z=2.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  C.X=0.0; C.Y=0.0; C.Z=-4.0;
  V=VS_Add(&VS,&C);
  Su_AddVertex(S,V);

  Su_SetPlanes(S);


  ShowSL(B.S);

  BSP=BSP_Create();
  BSP_Add(BSP,&B,&VS,&NSplit);
  printf("Split: %d\n",NSplit);
  ShowSL(BSP->Split);

  C.X=-1.0; C.Y=-1.0; C.Z=2.0;
  BSP_Traverse(BSP,&C,ShowBSPSurface);
}
