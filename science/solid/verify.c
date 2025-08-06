/* SOLID OBJECT MANIPULATION ROUTINES' TESTER.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"

#include "solid.h"


BOOLEAN EQ(X,Y)
double X;
double Y;
{
  return (fabs(X-Y)<1E-10);
}


BOOLEAN V_EQ(A,B)
Cartesian *A;
Cartesian *B;
{
  return EQ(A->X,B->X) && EQ(A->Y,B->Y) && EQ(A->Z,B->Z);
}


void TestVertex()
{
  Vertex *VS=NULL;

  Vertex    *V1,*V2,*V3,*V4;
  Cartesian C1,C2;

  C1.X=1.0; C1.Y=2.0; C1.Z=3.0;
  C2.X=8.0; C2.Y=7.0; C2.Z=4.0;
  
  if (!(V1=VS_Add(&VS,&C1)) || VS!=V1 ||
      V1->Nxt || !V_EQ(&V1->V,&C1) || V1->RefCount!=1) {
    printf("Vertex Error (add C1).\n");
    return;
  }
  if (!(V2=VS_Add(&VS,&C2)) || VS!=V2 || 
      V1->Nxt || !V_EQ(&V1->V,&C1) || V1->RefCount!=1 ||
      V2->Nxt!=V1 || !V_EQ(&V2->V,&C2) || V2->RefCount!=1) {
    printf("Vertex Error (add C2).\n");
    return;
  }
  if (!(V3=VS_Add(&VS,&C1)) || VS!=V2 || V3!=V1 ||
      V1->Nxt || !V_EQ(&V1->V,&C1) || V1->RefCount!=2 ||
      V2->Nxt!=V1 || !V_EQ(&V2->V,&C2) || V2->RefCount!=1) {
    printf("Vertex Error (add C1 again).\n");
    return;
  }

  if (!(V4=VS_Copy(V1)) || V4!=V1 || V1->RefCount!=3) {
    printf("Vertex Error (copy V1).\n");
    return;
  }

  VS_Remove(V1);
  if (V1->RefCount!=2) {
    printf("Vertex Error (remove V1).\n");
    return;
  }
  VS_Remove(V2);
  if (V2->RefCount!=0) {
    printf("Vertex Error (remove V2).\n");
    return;
  }
  VS_Cleanup(&VS);
  if (VS!=V1 || V1->Nxt || !V_EQ(&V1->V,&C1) || V1->RefCount!=2) {
    printf("Vertex Error (cleanup 1).\n");
    return;
  }

  VS_Remove(V3);
  if (V3->RefCount!=1) {
    printf("Vertex Error (remove V3).\n");
    return;
  }
  VS_Remove(V4);
  if (V4->RefCount!=0) {
    printf("Vertex Error (remove V4).\n");
    return;
  }

  VS_Cleanup(&VS);
  if (VS) {
    printf("Vertex Error (cleanup 2).\n");
    return;
  }

  printf("Vertex passed.\n");
  return;
}


/*
void TestBSP()
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
*/

main()
{
  printf("\nTesting the solid library (libsolid).\n\n");

  TestVertex();
}
