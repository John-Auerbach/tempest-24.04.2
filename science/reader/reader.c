/* CAMERA/GENERATOR/BODY FILE READER.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"
#include "beam.h"
#include "strsp.h"

#include "reader.h"


/* AUXILIARY ROUTINE. */

/* Changes every occurence of the character '_' in Str to a space ' '. The
   resulting string is stored in Str. */

static void CleanStr(Str)
char *Str;
{
  while (*Str) {
    if (*Str=='_')
      *Str=' ';
    Str++;
  }
  return;
}


/* FILE READING ROUTINES. */

/* Reads camera information from the text file named *FileName. The
   information read is stored in a new camera structure, which is the
   function's return value. The new camera is stored in the camera list
   CL, too. NULL may also be returned, iff an error occurs. */

Camera *Read_Camera(FileName,CL)
char   *FileName;
Camera **CL;
{
  FILE   *Input;
  Camera *C;

  if (!(Input=fopen(FileName,"r"))) {
    fprintf(stderr,"Warning: cannot open camera file %s.\n",FileName);
    return NULL;
  }
  if (!(C=Cm_Create(CL))) {
    fclose(Input);
    return NULL;
  }

  /* Camera name. */

  fscanf(Input,"%*s %10s",C->Name);
  CleanStr(C->Name);

  /* Camera location and orientation. */

  fscanf(Input,"%*s %lf %lf %lf",&((C->Origin).X),&((C->Origin).Y),
	 &((C->Origin).Z));
  fscanf(Input,"%*s %lf",&(C->Loc.Ph));
  fscanf(Input,"%*s %lf",&(C->Loc.Th));
  fscanf(Input,"%*s %lf",&(C->Loc.R));
  C->Loc.Ph=DTR(C->Loc.Ph);
  C->Loc.Th=DTR(C->Loc.Th);
  if (C->Loc.R<0.1) {
    fprintf(stderr,
	    "Warning: camera eye distance adjusted to minimum: 0.1.\n");
    C->Loc.R=0.1;
  }

  fclose(Input);
  return C;
}

/* Reads electron generator information from the text file named
   *FileName. The information read is stored in a new generator
   structure, which is the function's return value. The new generator is
   stored in the generator list *GL, too. NULL may also be returned, iff
   an error occurs. */

Generator *Read_Generator(FileName,GL)
char      *FileName;
Generator **GL;
{
  FILE      *Input;
  Generator *Gen;

  if (!(Input=fopen(FileName,"r"))) {
    fprintf(stderr,"Warning: cannot open generator file %s.\n",FileName);
    return NULL;
  }
  if (!(Gen=GnExt_Create(GL)))
    goto Error_ReadGenerator2;
  
  /* Generator information, location and orientation. */

  fscanf(Input,"%*s %10s",GENERATOR_EXT(Gen)->Name);
  CleanStr(GENERATOR_EXT(Gen)->Name);
  fscanf(Input,"%*s %60s",GENERATOR_EXT(Gen)->Description);
  CleanStr(GENERATOR_EXT(Gen)->Description);
  fscanf(Input,"%*s %10s",GENERATOR_EXT(Gen)->Mission);
  CleanStr(GENERATOR_EXT(Gen)->Mission);
  fscanf(Input,"%*s %lf",&(Gen->Coelevation));
  fscanf(Input,"%*s %lf",&(Gen->Azimuth));
  Gen->Coelevation=DTR(Gen->Coelevation);
  Gen->Azimuth=DTR(Gen->Azimuth);
  fscanf(Input,"%*s %lf %lf %lf",&((Gen->Loc).X),&((Gen->Loc).Y),
	 &((Gen->Loc).Z));

  /* The electron accelerating potential must be positive. */

  fscanf(Input,"%*s %lf",&(Gen->Potential));
  if (Gen->Potential<=0.0) {
    fprintf(stderr,"Warning: non-positive potential in generator file %s.\n",
	    FileName);
    goto Error_ReadGenerator1;
  }

  /* Read color information. */

  fscanf(Input,"%*s %20s",GENERATOR_EXT(Gen)->ImpactColorName);
  CleanStr(GENERATOR_EXT(Gen)->ImpactColorName);
  fscanf(Input,"%*s %20s",GENERATOR_EXT(Gen)->NoImpactColorName);
  CleanStr(GENERATOR_EXT(Gen)->NoImpactColorName);

  fclose(Input);
  return Gen;

 Error_ReadGenerator1:
  (*GL)=Gen->Nxt;
  GnExt_Free(Gen);
 Error_ReadGenerator2:
  fclose(Input);
  return NULL;
}

/* Reads body information from the text file named *FileName. The
   vertex information is stored in the vertex space *VS, and surface
   location information in the string space *SS. The body information
   read is stored in a new body structure, which is the function's return
   value.  The new body is stored in the body list *BL, too. NULL may
   also be returned, iff an error occurs. */

Body *Read_Body(FileName,BL,VS,SS)
char         *FileName;
Body         **BL;
Vertex       **VS;
SharedString **SS;
{
  FILE      *Input;
  Body      *B;
  Surface   *S;
  int       ID;
  Vertex    *V;
  Cartesian NewP;
  char      NextChar;
  char      Location[100];
  
  if (!(Input=fopen(FileName,"r"))) {
    fprintf(stderr,"Warning: cannot open body file %s.\n",FileName);
    return NULL;
  }
  if (!(B=BdExt_Create(BL)))
    goto Error_ReadSurfaces2;

  /* Miscalleneous information. */

  fscanf(Input,"%*s %10s",BODY_EXT(B)->Name);
  CleanStr(BODY_EXT(B)->Name);
  fscanf(Input,"%*s %60s",BODY_EXT(B)->Description);
  CleanStr(BODY_EXT(B)->Description);
  fscanf(Input,"%*s %20s",BODY_EXT(B)->EdgeColorName);
  CleanStr(BODY_EXT(B)->EdgeColorName);
  fscanf(Input,"%*s %20s",BODY_EXT(B)->InteriorColorName);
  CleanStr(BODY_EXT(B)->InteriorColorName);
  fscanf(Input,"\n");

  /* Clear old vertex IDs to avoid ID clashes with previous files. */

  V=(*VS);
  while (V) {
    if (VERTEX_EXT(V))
      VERTEX_EXT(V)->ID=(-1);
    V=V->Nxt;
  }
  
  /* Vertex reading. */

  while (fscanf(Input,"[VERTEX V%d %lf %lf %lf ]\n",
		&ID,&(NewP.X),&(NewP.Y),&(NewP.Z))==4) {
    if (!(V=VSExt_Add(VS,&NewP)))
      goto Error_ReadSurfaces1;
    VS_Remove(V);
    VERTEX_EXT(V)->ID=ID;
  }

  /* Surface reading. */

  while (fscanf(Input,"POLYGON P%d",&ID)==1) {

    /* Create new surface, and initialize it. */

    if (!(S=SuExt_Create(&B->S)))
      goto Error_ReadSurfaces1;
    SURFACE_EXT(S)->ID=ID;
    SURFACE_EXT(S)->B=B;

    /* Parse vertex space for vertex identification by ID. */

    while (fscanf(Input," V%d",&ID)==1) {
      V=(*VS);
      while (V) {
	if (VERTEX_EXT(V) && VERTEX_EXT(V)->ID==ID)
	  break;
	V=V->Nxt;
      }
      VS_Copy(V);
      if (!Su_AddVertex(S,V))
	goto Error_ReadSurfaces1;
    }
    
    /* Obtain standard polygon representation. */

    Su_SetPlanes(S);

    /* Read (optional) surface location. */
    
    fscanf(Input," ]\n[");
    NextChar=getc(Input);
    if (NextChar=='L') {
      fscanf(Input,"OCATION %99s ]\n[",Location);
      CleanStr(Location);
      SURFACE_EXT(S)->Location=SS_Add(SS,Location);
    } else {
      ungetc(NextChar,Input);
      SURFACE_EXT(S)->Location=NULL;
    }
  }

  fclose(Input);
  return B;

  /* Error recovery: removing bad data from lists. */

 Error_ReadSurfaces1:
  (*BL)=B->Nxt;
  BdExt_Free(B);
  VSExt_Cleanup(VS);
 Error_ReadSurfaces2:
  fclose(Input);
  return NULL;
}
