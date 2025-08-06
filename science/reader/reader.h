/* PRIVATE HEADER FILE OF CAMERA/GENERATOR/BODY FILE READER.

   Apostolos Lerios - TOLIS@NOVA. */


/* NEW OBJECTS IN THREE DIMENSIONAL SPACE. */

typedef struct _Camera { /* Camera (set of view system settings). */
  char      Name[11];    /* User specification. */
  Cartesian Origin;      /* Origin of view system. */
  Spherical Loc;         /* View point location relative to Origin. */

  struct _Camera *Nxt;

  /* User handle to additional camera data. */

  void *UserData;
} Camera;


/* USER DATA FOR OBJECTS IN THREE DIMENSIONAL SPACE. */

typedef struct {
  int  ID;        /* Vertex ID, specified in input file. */
  void *UserData; /* User handle to additional vertex data. */
} VertexExt;  

#define VERTEX_EXT(V) ((VertexExt *)((V)->UserData))

typedef struct {
  int          ID;        /* User-defined surface ID. */
  SharedString *Location; /* User-supplied surface location/description. */
  Body         *B;        /* Pointer to body, owner of surface. */
  void         *UserData; /* User handle to additional surface data. */
} SurfaceExt;

#define SURFACE_EXT(S) ((SurfaceExt *)((S)->UserData))

typedef struct {
  char Name[11];              /* User specification. */
  char Description[61];
  char EdgeColorName[21];
  char InteriorColorName[21];
  void *UserData;             /* User handle to additional body data. */
} BodyExt;

#define BODY_EXT(B) ((BodyExt *)((B)->UserData))


/* USER DATA FOR ELECTRON BEAM GENERATOR. */

typedef struct {
  char Name[11];              /* User specification. */
  char Description[61];
  char Mission[11];
  char ImpactColorName[21];
  char NoImpactColorName[21];
  void *UserData;             /* User handle to additional generator data. */
} GeneratorExt;

#define GENERATOR_EXT(G) ((GeneratorExt *)((G)->UserData))


/* FUNCTION DECLARATIONS. */

#ifdef _NO_PROTO

/* camera.c */

Camera *Cm_Create();
void Cm_Free();
void Cm_FreeList();

/* extvertex.c */

Vertex *VSExt_Add();
RETCODE VSExt_Extend();
void VSExt_Cleanup();

/* extsurf.c */

Surface *SuExt_Create();
RETCODE SuExt_Extend();
void SuExt_Free();
void SuExt_FreeList();

/* extbody.c */
 
Body *BdExt_Create();
RETCODE BdExt_Extend();
void BdExt_Free();
void BdExt_FreeList();

/* extgen.c */

Generator *GnExt_Create();
void GnExt_Free();
void GnExt_FreeList();

/* reader.c */

Camera *Read_Camera();
Generator *Read_Generator();
Body *Read_Body();

#else

/* camera.c */

Camera *Cm_Create(Camera **);
void Cm_Free(Camera *);
void Cm_FreeList(Camera *);

/* extvertex.c */

Vertex *VSExt_Add(Vertex **,Cartesian *);
RETCODE VSExt_Extend(Vertex *);
void VSExt_Cleanup(Vertex **);

/* extsurf.c */

Surface *SuExt_Create(Surface **);
RETCODE SuExt_Extend(Surface *);
void SuExt_Free(Surface *);
void SuExt_FreeList(Surface *);

/* extbody.c */
 
Body *BdExt_Create(Body **);
RETCODE BdExt_Extend(Body *);
void BdExt_Free(Body *);
void BdExt_FreeList(Body *);

/* extgen.c */

Generator *GnExt_Create(Generator **);
void GnExt_Free(Generator *);
void GnExt_FreeList(Generator *);

/* reader.c */

Camera *Read_Camera(char *,Camera **);
Generator *Read_Generator(char *,Generator **);
Body *Read_Body(char *,Body **,Vertex **,SharedString **);

#endif
