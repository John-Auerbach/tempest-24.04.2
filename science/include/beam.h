/* PRIVATE HEADER FILE OF ELECTRON BEAM TRACER.

   Apostolos Lerios - TOLIS@NOVA. */


/* FUNDAMENTAL CONSTANTS. */

#define ME 9.1095E-31 /* Electron mass in kg. */
#define QE 1.602E-19  /* Absolute value of electron charge in Cb. */


/* BEAM PARAMETERS. */

/* Shape of electron beam. */

typedef enum {LINEAR,CIRCULAR,HELICAL} BEAM_SHAPE;

/* Mode of beam point sequence retrieval. */

typedef enum {LENGTH,ROTATIONS} BEAM_MODE;


/* ELECTRON BEAM GENERATOR. */

typedef struct _Generator {
  double    Coelevation,Azimuth; /* In radians. */
  Cartesian Loc;                 /* In meters. */
  double    Potential;           /* In Volts. */

  BEAM_SHAPE BeamType;       /* Beam shape. */
  Cartesian  a,b,c,d;        /* Beam equation parameters. */
  double     omega,Period,v;

  double  ImpactT;  /* Model intersection: time in seconds. */
  Surface *ImpactS; /* Model intersection: surface pointer (NULL if miss). */

  struct _Generator *Nxt;

  /* User handle to additional generator data. */

  void *UserData;
} Generator;


/* FUNCTION DECLARATIONS. */

#ifdef _NO_PROTO

/* gen.c */

Generator *Gn_Create();
void Gn_Free();
void Gn_FreeList();

/* beam.c */

BOOLEAN Beam_HitBody();
double Beam_Length();
BOOLEAN Beam_Point();
void Beam_Velocity();
BOOLEAN Beam_Trace();
Body *Beam_Cylinder();
BOOLEAN Beam_SecondaryHitBody();
void Beam_SecondaryUtil();
int Beam_Sources();

#else

/* gen.c */

Generator *Gn_Create(Generator **);
void Gn_Free(Generator *);
void Gn_FreeList(Generator *);

/* beam.c */

BOOLEAN Beam_HitBodyList(Cartesian *,Generator *,Body *);
double Beam_Length(Generator *);
BOOLEAN Beam_Point(Generator *,double,Cartesian *);
void Beam_Velocity(Generator *,double,Cartesian *);
BOOLEAN Beam_Trace(BEAM_MODE,double,Generator *,Cartesian *,BOOLEAN);
Body *Beam_Cylinder(Generator *,int,double,double,Body **,Vertex **);
BOOLEAN Beam_SecondaryHitBody(Cartesian *,Generator *,Body *,Body *,
			      double,double,double,double,
			      Generator *,Generator *);
void Beam_SecondaryUtil(Generator *,Generator *,Generator *,
			double *,double *,double *,double *,double *);
int Beam_Sources(Cartesian *,Generator *,Body *,double,double,
		 Generator [],int [],int);

#endif
