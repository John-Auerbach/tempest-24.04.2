/* TESTER OF CAMERA/GENERATOR/BODY FILE READER.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"
#include "beam.h"
#include "strsp.h"

#include "reader.h"


void ShowCamera(C)
Camera *C;
{
  printf("Camera: %x\n",C);
  printf("Name: %s\n",C->Name);
  printf("Origin: %lf %lf %lf\n",C->Origin.X,C->Origin.Y,C->Origin.Z);
  printf("Location: %lf %lf %lf\n",C->Loc.Ph,C->Loc.Th,C->Loc.R);
}

void ShowCL(CL)
Camera *CL;
{
  while (CL) {
    ShowCamera(CL);
    CL=CL->Nxt;
  }
  puts("");
  return;
}


void ShowGenerator(G)
Generator *G;
{
  printf("Generator: %x\n",G);
  printf("Name: %s\n",GENERATOR_EXT(G)->Name);
  printf("Description: %s\n",GENERATOR_EXT(G)->Description);
  printf("Mission: %s\n",GENERATOR_EXT(G)->Mission);
  printf("Impact Color Name: %s\n",GENERATOR_EXT(G)->ImpactColorName);
  printf("No Impact Color Name: %s\n",GENERATOR_EXT(G)->NoImpactColorName);
  printf("Potential: %lf\n",G->Potential);
  printf("Coelevation: %lf; Azimuth: %lf\n",G->Coelevation,G->Azimuth);
  printf("Location: %lf %lf %lf\n",G->Loc.X,G->Loc.Y,G->Loc.Z);
}

void ShowGL(GL)
Generator *GL;
{
  while (GL) {
    ShowGenerator(GL);
    GL=GL->Nxt;
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

void ShowSS(SS)
SharedString *SS;
{
  while (SS) {
    printf("%d; %d; %s\n",SS,SS->RefCount,SS->Data);
    SS=SS->Nxt;
  }
  puts("");
  return;
}

void ShowSurface(S)
Surface *S;
{
  Side *Sd;

  printf("Surface %x:\n",S);
  printf("ID: %d\n",SURFACE_EXT(S)->ID);
  printf("Location: %s\n",SURFACE_EXT(S)->Location->Data);
  printf("Body: %x\n",SURFACE_EXT(S)->B);
  printf("Vertices:\n");
  Sd=S->Sd;
  while (Sd) {
    printf(" (%d,%lf,%lf,%lf)\t",
	   VERTEX_EXT(Sd->V)->ID,Sd->V->V.X,Sd->V->V.Y,Sd->V->V.Z);
    Sd=Sd->Nxt;
  }
  puts("");
  return;
}

void ShowSL(SL)
Surface *SL;
{
  while (SL) {
    ShowSurface(SL);
    SL=SL->Nxt;
  }
  puts("");
  return;
}

void ShowBody(B)
Body *B;
{
  printf("Body: %x\n",B);
  printf("Name: %s\n",BODY_EXT(B)->Name);
  printf("Description: %s\n",BODY_EXT(B)->Description);
  printf("Edge Color Name: %s\n",BODY_EXT(B)->EdgeColorName);
  printf("Interior Color Name: %s\n",BODY_EXT(B)->InteriorColorName);
  ShowSL(B->S);
}

void ShowBL(BL)
Body *BL;
{
  while (BL) {
    ShowBody(BL);
    BL=BL->Nxt;
  }
  puts("");
  return;
}

BOOLEAN ShowBSPSurface(S)
Surface *S;
{
  printf("%x %x %c\n",S,S->Originator,S->Temporary?'Y':'N');
  return TRUE;
}


int main(argc,argv)
unsigned int argc;
char         *argv[];
{
  Vertex *VS=NULL;
  SharedString *SS=NULL;

  Body *BL=NULL;
  Body *B;

  BSPTree *BSP;

  Cartesian C;

  int Temp;


  BSP=BSP_Create();
  B=Read_Body("/users/d4/sets/beam/tss/dcore.ply",&BL,&VS,&SS);
  BSP_Add(BSP,B,&VS,&Temp,FALSE);
  ShowVS(VS);
  B=Read_Body("/users/d4/sets/beam/tss/dep.ply",&BL,&VS,&SS);
  BSP_Add(BSP,B,&VS,&Temp,TRUE);
  ShowVS(VS);
  
  C.X=C.Y=C.Z=0.0;
  BSP_Traverse(BSP,&C,ShowBSPSurface);
  ShowVS(VS);

  puts("\nTemporary removed\n");
  BSP_RemoveTemporary(BSP);
  BSP_Traverse(BSP,&C,ShowBSPSurface);
  VS_Cleanup(&VS);
  ShowVS(VS);

  Bd_Free(B);
  VS_Cleanup(&VS);
  SS_Cleanup(&SS);
  
  ShowVS(VS);
  printf("%x %x\n",VS,SS);
}
