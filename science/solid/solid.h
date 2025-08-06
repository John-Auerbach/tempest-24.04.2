/* PRIVATE HEADER FILE OF SOLID OBJECT MANIPULATION ROUTINES.

   Apostolos Lerios - TOLIS@NOVA. */


/* OBJECTS IN THREE DIMENSIONAL SPACE. */

typedef struct { /* Vector equation of a plane. */
  Cartesian u;   /* Unit normal to plane. */
  double    d;   /* Distance from origin (not necessarily positive). */
} Plane;

typedef struct _Vertex { /* A point in the model space. */
  unsigned int RefCount; /* Reference count. */
  Cartesian    V;        /* Global coordinates. */

  struct _Vertex *Nxt; /* Vertex space linked list. */

  /* Binary space partitioning tree parameters. */

  double  Dist;      /* Vertex distance from a plane. */
  int     Location;  /* Vertex location relative to a plane. */

  /* User handle to additional vertex data. */

  void *UserData;
} Vertex;

typedef struct _Side { /* Polygonal surface side. */
  Vertex *V;           /* Associated vertex. */
  Plane  Pi;           /* Side plane. */

  struct _Side *Nxt;
} Side;

typedef struct _Surface { /* Planar surface with convex polygonal boundary. */
  Side  *Sd;              /* Side information. */
  Side  *LastSd;          /* Last side. */
  Plane P;                /* Equation of surface plane. */

  struct _Surface *Nxt; /* Linked list of surfaces. */

  /* Binary space partitioning tree parameters. */

  BOOLEAN         Remove;       /* Split surface was split again; remove it. */
  BOOLEAN         Temporary;    /* Surface is part of temporary body. */
  struct _Surface *Originator;  /* Split surface parent, or self o|w. */
  struct _Surface *Front,*Back; /* Tree children. */

  /* User handle to additional surface data. */

  void *UserData;
} Surface;

typedef struct _Body { /* Body (set of polygonal surfaces). */
  Surface *S;          /* Surface set. */

  struct _Body *Nxt;

  /* User handle to additional body data. */

  void *UserData;
} Body;


/* BINARY SPACE PARTITIONING TREE. */

typedef struct _BSPTree {
  Surface *Root;           /* Root of tree. */
  Surface *Split;          /* List of split surfaces. */
  BOOLEAN TemporaryExists; /* Temporary body exists on tree. */
} BSPTree;


/* FUNCTION DECLARATIONS. */

#ifdef _NO_PROTO

/* vertex.c */

Vertex *VS_Add();
Vertex *VS_Copy();
void VS_Remove();
void VS_Cleanup();

/* surf.c */

Surface *Su_Create();
RETCODE Su_AddVertex();
void Su_SetPlanes();
void Su_Free();
void Su_FreeList();

/* body.c */

Body *Bd_Create();
void Bd_Free();
void Bd_FreeList();

/* planes.c */

BOOLEAN Int_HalfLineCrossSides();
BOOLEAN Int_NearHalfLineCrossPoly();

/* tree.c */

BSPTree *BSP_Create();
void BSP_Free();
RETCODE BSP_Add();
void BSP_RemoveTemporary();
RETCODE BSP_Traverse();

#else

/* vertex.c */

Vertex *VS_Add(Vertex **,Cartesian *);
Vertex *VS_Copy(Vertex *);
void VS_Remove(Vertex *);
void VS_Cleanup(Vertex **);

/* surf.c */

Surface *Su_Create(Surface **);
RETCODE Su_AddVertex(Surface *,Vertex *);
void Su_SetPlanes(Surface *);
void Su_Free(Surface *);
void Su_FreeList(Surface *);

/* body.c */

Body *Bd_Create(Body **);
void Bd_Free(Body *);
void Bd_FreeList(Body *);

/* planes.c */

BOOLEAN Int_HalfLineCrossSides(Cartesian *,Cartesian *,Surface *,
			       double *,double *);
BOOLEAN Int_NearHalfLineCrossPoly(Cartesian *,Cartesian *,Surface *,double *);

/* tree.c */

BSPTree *BSP_Create();
void BSP_Free(BSPTree *);
RETCODE BSP_Add(BSPTree *,Body *,Vertex **,int *,BOOLEAN);
void BSP_RemoveTemporary(BSPTree *);
RETCODE BSP_Traverse(BSPTree *,Cartesian *,BOOLEAN (*)());

#endif
