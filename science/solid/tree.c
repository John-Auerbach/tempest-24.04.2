/* SURFACE ORDERING BY BINARY SPACE PARTITIONING TREE.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>
#include "advmath.h"

#include "user.h"
#include "solid.h"


/* ALLOCATION AND DEALLOCATION. */

/* Initializes the binary space partitioning tree *BSP. */

BSPTree *BSP_Create()
{
  BSPTree *BSP;

  if (!(BSP=(BSPTree *)malloc((unsigned int)sizeof(BSPTree)))) {
    fprintf(stderr,
	    "Warning: cannot create new binary space partitioning tree.\n");
    return NULL;
  }
  BSP->Root=NULL;
  BSP->Split=NULL;
  BSP->TemporaryExists=FALSE;
  return BSP;
}

/* Deallocates all the memory taken up by the binary space
   partitioning tree *BSP. */

void BSP_Free(BSP)
BSPTree *BSP;
{
  Su_FreeList(BSP->Split);
  free(BSP);
  return;
}


/* BODY ADDITION. */

/* Surface or vertex locations relative to a plane. SPLIT used only
   for surfaces. */

typedef enum {FRONT,BACK,ON_PLANE,SPLIT} LOCATION;

static BSPTree *Tree;   /* Tree under modification. */
static Vertex  **CurVS; /* Vertex space of split surface vertices. */
static int     NSplit;  /* Number of new split surfaces since the most */
		        /* recent call to BSP_Add(). */

/* Creates a new split surface, as a subsurface of surface
   *Originator. NULL is returned iff a memory failure occurs; otherwise,
   a pointer to the new surface is returned. */

static Surface *NewSurface(Originator)
Surface *Originator;
{
  Surface *S;

  /* Create surface. */

  if (!(S=Su_Create(&(Tree->Split)))) {
    fprintf(stderr,"Warning: no more memory for surface split.\n");
    return NULL;
  }
  S->Remove=FALSE;

  /* Set originator surface and other data to originator data. */

  S->Originator=Originator->Originator;
  S->Temporary=S->Originator->Temporary;
  S->UserData=S->Originator->UserData;

  /* Update informational count and return. */

  NSplit++;
  return S;
}

/* Adds a vertex to surface *S. If V is non-null, it points to the
   shared vertex to be added; otherwise, P contains the coordinates of
   the new vertex, letting the function seek (or create) the shared
   vertex. FAILURE is returned iff a memory allocation error occurs. */

static RETCODE AddVertex(S,V,P)
Surface   *S;
Vertex    *V;
Cartesian *P;
{
  if (V)
    V=VS_Copy(V);
  else if (!(V=VS_Add(CurVS,P)))
    return FAILURE;
  if (Su_AddVertex(S,V))
    return SUCCESS;
  VS_Remove(V);
  return FAILURE;
}

/* Finds the point of the plane *P which lies on the line joining *V1
   and *V2, and stores it in *I. Both *V1 and *V2 must lie outside *P, on
   opposite sides of *P, and their distances from *P must already be
   present in their Dist fields. */

static void PlaneCrossover(P,V1,V2,I)
Plane     *P;
Vertex    *V1;
Vertex    *V2;
Cartesian *I;
{
  double    Lambda;
  Cartesian Temp1,Temp2;

  Lambda=(P->d-V2->Dist)/(V1->Dist-P->d);
  V_Mult(&(V2->V),1.0/(Lambda+1.0),&Temp1);
  V_Mult(&(V1->V),Lambda/(Lambda+1.0),&Temp2);
  V_Add(&Temp1,&Temp2,I);
  return;
}

/* Determines the relative location of surface *S with respect to
   plane *P. The return value may be FRONT, BACK, ON_PLANE (i.e. *S lies
   on *P), SPLIT. In the latter case, *S crosses *P. The points of
   intersection may be points of *S. If this is not the case, the
   coordinates of the intersection points are stored in *I (and *(I+1),
   if two such points exist - no more than two can exist, since all
   surfaces have convex polygonal boundaries).
   As a side effect, it determines the location of every vertex of *S
   with respect to *P (stored in the Location field as FRONT, BACK, or
   ON_PLANE), and the distance of each vertes from *P (stored in the
   Dist field). */

static LOCATION SurfaceLocation(S,P,I)
Surface   *S;
Plane     *P;
Cartesian *I;
{
  double  Dist;
  Side    *Sd;
  Vertex  *PrevV;
  BOOLEAN SInBack,SInFront;

  Sd=S->Sd;
  PrevV=NULL;
  SInBack=SInFront=TRUE;                     /* Assume that *S lies on *P. */
  while(Sd) {
    Sd->V->Dist=V_Dot(&(Sd->V->V),&(P->u));
    Dist=Sd->V->Dist-P->d;
    if (fabs(Dist)<=PLANE_DISTANCE)
      Sd->V->Location=ON_PLANE;
    else if (Dist>0.0) {
      Sd->V->Location=FRONT;
      SInBack=FALSE;
    } else {
      Sd->V->Location=BACK;
      SInFront=FALSE;
    }
    if (PrevV &&
	((PrevV->Location==FRONT && Sd->V->Location==BACK) ||
	 (PrevV->Location==BACK && Sd->V->Location==FRONT)))
      PlaneCrossover(P,PrevV,Sd->V,I++);
    PrevV=Sd->V;
    Sd=Sd->Nxt;
  }

  /* Case in which the first and last point of the surface boundary have
     different locations relative to *P. */

  if ((PrevV->Location==FRONT && S->Sd->V->Location==BACK) ||
      (PrevV->Location==BACK && S->Sd->V->Location==FRONT))
    PlaneCrossover(P,PrevV,S->Sd->V,I);

  if (SInBack && SInFront)
    return ON_PLANE;
  if (SInBack)
    return BACK;
  if (SInFront)
    return FRONT;
  return SPLIT;
}

/* Inserts surface *S in the binary space partitioning tree rooted at
   *Root. *FreeS is a return value and indicates that *S was split in half
   in order to be inserted in the tree; thus, if *S had been a split
   surface itself, it can be destroyed. SUCCESS is returned, unless a
   memory failure occurs. */

static RETCODE AddSurface(S,Root)
Surface *S;
Surface *Root;
{
  Side      *Sd,*PrevSd;
  Cartesian *CrossP;
  Cartesian CrossPs[2];
  Surface   *SubS[2];
  char      SLocation;

  /* Determine location of *S relative to *Root. */

  SLocation=SurfaceLocation(S,&(Root->P),CrossPs);

  /* *S does not have to be split: try to make it a direct child of
     *Root. If this is not possible, recurse. */

  if (SLocation!=SPLIT) {
    if (SLocation==FRONT) {           /* *S is in front of *Root. */
      if (Root->Front)
	return AddSurface(S,Root->Front);
      Root->Front=S;
    } else if (SLocation==BACK) {     /* *S is behind *Root. */
      if (Root->Back)
	return AddSurface(S,Root->Back);
      Root->Back=S;
    } else {                          /* *S and *Root lie on the same plane. */
      if (!Root->Front)
	Root->Front=S;
      else if (!Root->Back)
	Root->Back=S;
      else
	return AddSurface(S,Root->Front); /* Could have added to */
                                          /* Root->Back, instead. */
    }
    return SUCCESS;
  }

  /* *S must be split into two surfaces: SubS[0] is the front surface. */

  S->Remove=TRUE;
  if (!(SubS[0]=NewSurface(S)) || !(SubS[1]=NewSurface(S)))
    return FAILURE;
  CrossP=CrossPs;
  PrevSd=NULL;
  Sd=S->Sd;
  while(Sd) {                    /* Adding points to the two subsurfaces. */
    switch (Sd->V->Location) {

    case FRONT:
      if (PrevSd && PrevSd->V->Location==BACK) {
	if (!AddVertex(SubS[0],NULL,CrossP))
	  return FAILURE;
	if (!AddVertex(SubS[1],NULL,CrossP++))
	  return FAILURE;
      }
      if (!AddVertex(SubS[0],Sd->V,NULL))
	return FAILURE;
      break;

    case BACK:
      if (PrevSd && PrevSd->V->Location==FRONT) {
	if (!AddVertex(SubS[1],NULL,CrossP))
	  return FAILURE;
	if (!AddVertex(SubS[0],NULL,CrossP++))
	  return FAILURE;
      }
      if (!AddVertex(SubS[1],Sd->V,NULL))
	return FAILURE;
      break;

    case ON_PLANE:
      if (!AddVertex(SubS[0],Sd->V,NULL))
	return FAILURE;
      if (!AddVertex(SubS[1],Sd->V,NULL))
	return FAILURE;
      break;
    }

    PrevSd=Sd;
    Sd=Sd->Nxt;
  }
  if ((PrevSd->V->Location==FRONT && S->Sd->V->Location==BACK) ||
      (PrevSd->V->Location==BACK && S->Sd->V->Location==FRONT)) {
    if (!AddVertex(SubS[0],NULL,CrossP))
      return FAILURE;
    if (!AddVertex(SubS[1],NULL,CrossP))
      return FAILURE;
  }

  /* Calculate internal surface representation parameters. */

  Su_SetPlanes(SubS[0]);
  Su_SetPlanes(SubS[1]);

  /* Add partial surfaces to the tree and destroy them if they are themselves
     split later on (during recursion). */
  
  if (Root->Front) {
    if (!AddSurface(SubS[0],Root->Front))
      return FAILURE;
  } else
    Root->Front=SubS[0];
  if (Root->Back) {
    if (!AddSurface(SubS[1],Root->Back))
      return FAILURE;
  } else
    Root->Back=SubS[1];
  return SUCCESS;
}

/* Adds the surfaces of body B to the binary space partitioning tree
   *BSP. If the body is temporarily added (Temporary is TRUE), then it
   can subsequently be removed using BSP_RemoveTemporary(); also, no
   bodies can be added until a temporary body is removed. Upon return,
   *Splits contains the number of new split surfaces created during
   insertion.  Those surfaces may require the creation of new vertices;
   those are added to the vertex space *VS. FAILURE is returned in case
   of memory allocation failure, or an attempt to add a body to a tree
   which contains a temporary body. */

RETCODE BSP_Add(BSP,B,VS,Splits,Temporary)
BSPTree *BSP;
Body    *B;
Vertex  **VS;
int     *Splits;
BOOLEAN Temporary;
{
  Surface **SPtr,*S;
  int     Count;

  /* Cannot add any body when tree contains temporary one. */

  if (BSP->TemporaryExists)
    return FAILURE;

  /* No surfaces in body. */

  if (!B->S)
    return SUCCESS;
  S=B->S;

  /* Set originator field and temporary field: new body iis temporary
     until successfully added. */

  while (S) {
    S->Originator=S;
    S->Temporary=TRUE;
    S=S->Nxt;
  }

  /* Reset count of split surfaces added, and current operation
     parameters. */

  NSplit=0;
  Tree=BSP;
  CurVS=VS;

  /* Create root of BSP tree, if empty. */

  S=B->S;
  if (!BSP->Root) {
    BSP->Root=S;
    S=S->Nxt;
  }

  /* Add surfaces sequentially. */

  while (S) {
    if (!AddSurface(S,BSP->Root)) {
      BSP_RemoveTemporary(BSP);
      return FAILURE;
    }
    S=S->Nxt;
  }

  /* Remove split surfaces marked for removal, and mark temporary
     split surfaces. */

  Count=NSplit;
  SPtr=(&(BSP->Split));
  while (Count) {
    if (((*SPtr)->Remove)) {
      S=(*SPtr);
      *SPtr=S->Nxt;
      Su_Free(S);
      NSplit--;
    } else {
      (*SPtr)->Temporary=Temporary;
      SPtr=(&((*SPtr)->Nxt));
    }
    Count--;
  }
  *Splits=NSplit;

  /* Reset temporary field to user-specified value. */

  S=B->S;
  while (S) {
    S->Temporary=Temporary;
    S=S->Nxt;
  }

  /* Mark existence of temporary body in tree. */

  BSP->TemporaryExists=Temporary;
  return SUCCESS;
}


/* TEMPORARY BODY REMOVAL. */

/* Removes the temporary body of the binary space partitioning tree
   rooted at surface *Root, by recursively taversing the tree and
   nullifying the pointers to temporary surfaces. */

static void CleanTree(Root)
Surface *Root;
{
  if (Root->Front) {
    CleanTree(Root->Front);
    if (Root->Front->Temporary)
      Root->Front=NULL;
  }
  if (Root->Back) {
    CleanTree(Root->Back);
    if (Root->Back->Temporary)
      Root->Back=NULL;
  }
  return;
}

/* Removes the temporary body of the binary space partitioning tree
   *BSP. */

void BSP_RemoveTemporary(BSP)
BSPTree *BSP;
{
  Surface **SPtr,*S;

  /* Clean up tree. */

  if (BSP->Root) {
    CleanTree(BSP->Root);
    if (BSP->Root->Temporary)
      BSP->Root=NULL;
  }

  /* Remove temporary split surfaces. */

  SPtr=(&(BSP->Split));
  while (*SPtr) {
    if (((*SPtr)->Temporary)) {
      S=(*SPtr);
      *SPtr=S->Nxt;
      Su_Free(S);
    } else
      SPtr=(&((*SPtr)->Nxt));
  }

  /* Mark absence of temporary body in tree. */

  BSP->TemporaryExists=FALSE;
  return;
}


/* TRAVERSAL. */

static Cartesian *Eye;          /* Location of oberever's eye. */
static BOOLEAN   (*Callback)(); /* Callback upon tree node encounter. */

/* Traverses the binary space partitioning tree rooted at surface
   *Root, calling the callback function in back-to-front surface order.
   The callback function is passed one argument, a surface pointer, and
   is expected to return FALSE should the tree traversal be immediately
   arrested. Iff this happens, the Traverse() function returns FAILURE. */

static RETCODE Traverse(Root)
Surface *Root;
{
  if (!Root)
    return SUCCESS;

  if (V_Dot(Eye,&(Root->P.u))>(Root->P).d) {
    if (!Traverse(Root->Back))
      return FAILURE;
    if (!((*Callback)(Root)))
      return FAILURE;
    if (!Traverse(Root->Front))
      return FAILURE;
  } else {
    if (!Traverse(Root->Front))
      return FAILURE;
    if (!((*Callback)(Root)))
      return FAILURE;
    if (!Traverse(Root->Back))
      return FAILURE;
  }
  return SUCCESS;
}

/* Traverses the binary space partitioning tree *BSP, calling the
   callback function *NewCallback in back-to-front surface order. The
   callback function is passed one argument, a surface pointer, and is
   expected to return FALSE should the tree traversal be immediately
   arrested. Iff this happens, the BSP_Traverse() function returns
   FAILURE. The surface order is determined by the location of the
   observer's eye, given in *NewEye. */

RETCODE BSP_Traverse(BSP,NewEye,NewCallback)
BSPTree   *BSP;
Cartesian *NewEye;
BOOLEAN   (*NewCallback)();
{
  Eye=NewEye;
  Callback=NewCallback;
  return Traverse(BSP->Root);
}
