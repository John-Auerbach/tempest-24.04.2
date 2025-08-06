/* PRIVATE HEADER FILE OF ATTITUDE ROUTINES.

   Apostolos Lerios - TOLIS@NOVA. */


/* FUNCTION DECLARATIONS. */

#ifdef _NO_PROTO

/* pyr.c */

void PYR_FromMatrix();
void PYR_ToMatrix();

/* nasa.c */

void NASA_MBodySETS();
void NASA_BodySETS();
void NASA_MLVLHSETS();
void NASA_LVLHSETS();

#else

/* pyr.c */

void PYR_FromMatrix(Matrix,double *,double *,double *);
void PYR_ToMatrix(double,double,double,Matrix);

/* nasa.c */

void NASA_MBodySETS(Matrix);
void NASA_BodySETS(Cartesian *,Cartesian *);
void NASA_MLVLHSETS(Matrix);
void NASA_LVLHSETS(Cartesian *,Cartesian *);

#endif
