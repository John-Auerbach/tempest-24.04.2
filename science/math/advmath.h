/* PRIVATE HEADER FILE OF ADVANCED MATH ROUTINES.

   Apostolos Lerios - TOLIS@NOVA. */


/* BASIC DATA TYPES AND CONSTANTS. */

#ifndef TRUE
typedef enum {FALSE,TRUE} BOOLEAN; /* Logical values. */
#else

#undef TRUE
#undef FALSE

typedef enum {FALSE,TRUE} BOOLEAN; /* Logical values. */

#endif

typedef enum {FAILURE,SUCCESS} RETCODE; /* Function result. */


#define PI     M_PI     /* The constant pi. */
#define HF_PI  M_PI_2   /* The constant pi/2. */
#define TWO_PI (2.0*PI) /* The constant 2*pi. */


/* MACROS. */

#define DTR(Degrees) ((Degrees)/360.0*TWO_PI)
#define RTD(Radians) ((Radians)*360.0/TWO_PI)


/* ABSTRACT MATHEMATICAL OBJECTS. */

typedef struct {      /* Quaternion: Q1 is scalar, (Q2,Q3,Q4) is vector. */
  double Q1,Q2,Q3,Q4;
} Quaternion;

typedef struct { /* Complex number: real and imaginary parts. */
  double R,I;
} Complex;

typedef double Matrix[3][3]; /* 3x3 matrix: first index is row number. */

typedef struct {  /* Cartesian vector: 3 orthogonal components. */
  double X,Y,Z;
} Cartesian;

typedef struct {   /* Orthogonal axis system. */
  Cartesian I,J,K; /* Unit vectors along local x,y,z axes, respectively. */
} Frame;

typedef struct {  /* Spherical coordinates. */
  double R,Th,Ph; /* All angles in radians. */
} Spherical;


/* PARAMETER SET FOR SPHERICAL SAMPLING. */

typedef struct {
  Cartesian E;                     /* Set by user. */
  double    Spread,dSpread;

  Cartesian Ej,Se,Si,Sj;           /* System-maintained. */
  double    CosSpread,dSideSpread;
  double    i,Spani,j,Spanj;
} SphSampleParam;


/* FUNCTION DECLARATIONS. */

#ifdef _NO_PROTO

/* vector.c */

void V_Add();
void V_Sub();
void V_Mult();
RETCODE V_Angle();
double V_Dot();
BOOLEAN V_DotIsZero();
void V_Cross();
BOOLEAN V_CrossIsZero();
double V_Mag();
RETCODE V_Unit();

/* quat.c */

void Q_Matrix();
void Q_Mult();
void Q_Invert();

/* matrix.c */

void M_Unit();
double M_Determinant();
void M_Transpose();
void M_VMult();
void M_xRotation();
void M_yRotation();
void M_zRotation();
void M_MMult();
void M_ToGlobal();
void M_FromGlobal();
void M_LocalxRotation();
void M_LocalyRotation();
void M_LocalzRotation();

/* eqn.c */

BOOLEAN Eqn_NextSolution();

/* sph.c */

void S_ToCartesian();
void S_FromCartesian();
void S_Normalize();
BOOLEAN S_NextSample();

/* schmidt.c */

double Schmidt_Coeff();
void Schmidt_Array();

/* alf.c */

double ALF();
double ALF_Derivative();

/* util.c */

double Sqr();
int Sign();
double Factorial();
double ModBessel();
double LP();
RETCODE Quadratic();

#else

/* vector.c */

void V_Add(Cartesian *,Cartesian *,Cartesian *);
void V_Sub(Cartesian *,Cartesian *,Cartesian *);
void V_Mult(Cartesian *,double,Cartesian *);
RETCODE V_Angle(Cartesian *,Cartesian *,double *);
double V_Dot(Cartesian *,Cartesian *);
BOOLEAN V_DotIsZero(Cartesian *,Cartesian *,double *);
void V_Cross(Cartesian *,Cartesian *,Cartesian *);
BOOLEAN V_CrossIsZero(Cartesian *,Cartesian *,Cartesian *);
double V_Mag(Cartesian *);
RETCODE V_Unit(Cartesian *,Cartesian *);

/* quat.c */

void Q_Matrix(Quaternion *,Matrix);
void Q_Mult(Quaternion *,Quaternion *,Quaternion *);
void Q_Invert(Quaternion *,Quaternion *);

/* matrix.c */

void M_Unit(Matrix);
double M_Determinant(Matrix);
void M_Transpose(Matrix,Matrix);
void M_VMult(Matrix,Cartesian *,Cartesian *);
void M_xRotation(double,Matrix);
void M_yRotation(double,Matrix);
void M_zRotation(double,Matrix);
void M_MMult(Matrix,Matrix,Matrix);
void M_ToGlobal(Frame *,Matrix);
void M_FromGlobal(Frame *,Matrix);
void M_LocalxRotation(Frame *,double,Matrix);
void M_LocalyRotation(Frame *,double,Matrix);
void M_LocalzRotation(Frame *,double,Matrix);

/* eqn.c */

BOOLEAN Eqn_NextSolution(BOOLEAN,
			 double,double,double,double,double,double,double *);

/* sph.c */

void S_ToCartesian(Spherical *,Cartesian *);
void S_FromCartesian(Cartesian *,Spherical *);
void S_Normalize(Spherical *);
BOOLEAN S_NextSample(BOOLEAN,SphSampleParam *,Cartesian *);

/* schmidt.c */

double Schmidt_Coeff(unsigned int,unsigned int);
void Schmidt_Array(unsigned int,unsigned int,double,double *,double *);

/* alf.c */

double ALF(unsigned int,unsigned int,double);
double ALF_Derivative(unsigned int,unsigned int,double);

/* util.c */

double Sqr(double);
int Sign(double);
double Factorial(unsigned int);
double ModBessel(unsigned int,double);
double LP(unsigned int,double);
RETCODE Quadratic(double,double,double,Complex *,Complex *);

#endif
