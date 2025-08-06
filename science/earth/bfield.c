/* IGRF MODEL IMPLEMENTATION.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>
#include "advmath.h"

#include "user.h"
#include "earth.h"


/* Obtain appropriate model coefficients. */

#include "igrf95.h"
#include "igrf90.h"
#include "igrf85.h"


/* Adjusted field coefficients. The user must first call
   IGRF_SetYear() to calculate the expansion coefficients for the
   required year, and then perform B estimations using IGRF_GetBField(),
   or tracing using IGRF_Trace().  */

static double g[NMAX+1][NMAX+1],h[NMAX+1][NMAX+1];
static double IGRF_YEAR = 0.0 ;
static double gMain[NMAX+1][NMAX+1], hMain[NMAX+1][NMAX+1] ;
static double gSec[NMAX_SEC+1][NMAX_SEC+1], hSec[NMAX_SEC+1][NMAX_SEC+1] ;

/* Sets the values of g and h for the required Year. Year must be in
   the range IGRF_YEAR-2.5 to IGRF_YEAR+2.5, for the model values to
   be reliable.  */

void IGRF_SetYear(Year)
double Year;
{
  register int n,m;

  if (Year >= 1995.0)
  {
    if (IGRF_YEAR != 1995.0)
    {
      IGRF_YEAR = 1995.0 ;
      for (n=1;n<=NMAX;n++)
        for (m=0;m<=n;m++) {
          gMain[n][m]=gMain1995[n][m] ;
          hMain[n][m]=hMain1995[n][m] ;
        }
      for (n=1;n<=NMAX_SEC;n++)
        for (m=0;m<=n;m++) {
          gSec[n][m]=gSec1995[n][m] ;
          hSec[n][m]=hSec1995[n][m] ;
        }
    }
  }
  else if (Year >= 1990.0)
  {
    if (IGRF_YEAR != 1990.0)
    {
      IGRF_YEAR = 1990.0 ;
      for (n=1;n<=NMAX;n++)
        for (m=0;m<=n;m++) {
          gMain[n][m]=gMain1990[n][m] ;
          hMain[n][m]=hMain1990[n][m] ;
        }
      for (n=1;n<=NMAX_SEC;n++)
        for (m=0;m<=n;m++) {
          gSec[n][m]=gSec1990[n][m] ;
          hSec[n][m]=hSec1990[n][m] ;
        }
    }
  }
  else
  {
    if (IGRF_YEAR != 1985.0)
    {
      IGRF_YEAR = 1985.0 ;
      for (n=1;n<=NMAX;n++)
        for (m=0;m<=n;m++) {
          gMain[n][m]=gMain1985[n][m] ;
          hMain[n][m]=hMain1985[n][m] ;
        }
      for (n=1;n<=NMAX_SEC;n++)
        for (m=0;m<=n;m++) {
          gSec[n][m]=gSec1985[n][m] ;
          hSec[n][m]=hSec1985[n][m] ;
        }
    }
  }

  for (n=1;n<=NMAX_SEC;n++)
    for (m=0;m<=n;m++) {
      g[n][m]=gMain[n][m]+(Year-IGRF_YEAR)*gSec[n][m];
      h[n][m]=hMain[n][m]+(Year-IGRF_YEAR)*hSec[n][m];
    }
  for (n=NMAX_SEC+1;n<=NMAX;n++)
    for (m=0;m<=n;m++) {
      g[n][m]=gMain[n][m];
      h[n][m]=hMain[n][m];
    }
  return;
}


/* Returns the magnetic field vector *B at an earth point given in *S.
   *B is in local geocentric cartesian coordinates (X is east, Y is
   north, Z is up), and in units of Tesla. */

void IGRF_GetBField(S,B)
Spherical *S;
Cartesian *B;
{
  register int n,m; /* Sum iterators, */

  double Phi;                  /* Longitude, */
  double Theta;                /* Geocentric co-latitude, */
  double AoR;                  /* Mean earth radius (a) over r, */
  double AoRn2;                /* AoR raised to the n+2. */
  double Bu,Bw,Bn;             /* Field components (w:west). */
  double Term,mPhi;            /* Temporary storage areas. */
  double BuTerm,BwTerm,BnTerm;

  /* Local storage of Schmidt quasi-normalized Legendre associated
     functions and derivatives w.r.t. Theta. CAUTION: one row for each
     value of m, not for n, as above. */

  static double SchP[NMAX+1][NMAX+1],dSchP[NMAX+1][NMAX+1];


  if (!S->R) {
    fprintf(stderr,"Warning: IGRF model not applicable at earth's center.\n");
    B->X=B->Y=B->Z=0.0;
    return;
  }

  /* Make certain that sin(Theta) will be non-zero. */

  if (S->Ph<POLES)
    Theta=POLES;
  else if (S->Ph>PI-POLES)
    Theta=PI-POLES;
  else
    Theta=S->Ph;

  /* Calculate P and dP values. */

  for (m=0;m<=NMAX;m++)
    Schmidt_Array((unsigned int)NMAX,(unsigned int)m,Theta,SchP[m],dSchP[m]);

  /* IGRF spherical harmonic expansion. */

  AoR=MEAN_R/S->R;
  Phi=S->Th;
  Bu=Bw=Bn=0.0;
  AoRn2=AoR*AoR;
  for (n=1;n<=NMAX;n++) {
    mPhi=BuTerm=BwTerm=BnTerm=0.0;
    for (m=0;m<=n;m++) {
      Term=g[n][m]*cos(mPhi)+h[n][m]*sin(mPhi);
      BuTerm+=Term*SchP[m][n];
      BwTerm+=(-g[n][m]*sin(mPhi)+h[n][m]*cos(mPhi))*m*SchP[m][n];
      BnTerm+=Term*dSchP[m][n];
      mPhi+=Phi;
    }
    AoRn2*=AoR;
    Bu+=(n+1.0)*AoRn2*BuTerm;
    Bw+=AoRn2*BwTerm;
    Bn+=AoRn2*BnTerm;
  }

  /* Return local geocentric components. */
  
  B->Y=NANOTESLA_TO_TESLA(Bn);
  B->X=NANOTESLA_TO_TESLA(-Bw/sin(Theta));
  B->Z=NANOTESLA_TO_TESLA(Bu);
  return;
}


/* Given a starting point *From, the function follows the magnetic
   field line passing through *From, until a point of altitude less than
   (if Dir is WHILE_ABOVE) or greater than (if Dir is WHILE_BELOW) or
   equal to ToAlt (in meters) is reached. The point at altitude ToAlt
   is returned in *To, and the function returns SUCCESS. Step is the
   distance for which we assume a constant magnetic field intensity, in
   meters; a negative value allows to trace field lines in the direction
   opposing the magnetic field intensity, i.e backwards tracing. If the
   initial point lies below (if Dir is WHILE_ABOVE) or above (if Dir is
   WHILE_BELOW), ToAlt, then *To is not changed and FAILURE is
   returned. FAILURE might also be returned if the magnetic field line
   tracing reaches a point with zero field intensity. */

RETCODE IGRF_Trace(From,ToAlt,Dir,Step,To)
Earth           *From;
double          ToAlt;
TRACE_DIRECTION Dir;
double          Step;
Cartesian       *To;
{
  Cartesian B,StepV;
  Spherical RS;
  Earth     RE;
  double    PrevAlt;
  Matrix    M;

  if ((From->Alt<ToAlt && Dir==WHILE_ABOVE) ||
      (From->Alt>ToAlt && Dir==WHILE_BELOW))
    return FAILURE;

  RE=(*From);
  E_ToSpherical(From,&RS);      /* Position in all forms. */
  S_ToCartesian(&RS,To);
  while ((RE.Alt>ToAlt && Dir==WHILE_ABOVE) ||
	 (RE.Alt<ToAlt && Dir==WHILE_BELOW)) {
    PrevAlt=RE.Alt;
    IGRF_GetBField(&RS,&B);  /* BField in local geocentric coordinates. */
    E_MFromLocalGeoc(&RS,M);
    M_VMult(M,&B,&B);        /* BField in ECR coordinates. */
    if (!V_Unit(&B,&B))      /* Direction of field line. */
      return FAILURE;
    V_Mult(&B,Step,&StepV);  /* Step vector along field line. */
    V_Add(To,&StepV,To);     /* Next position in all forms. */
    S_FromCartesian(To,&RS);
    E_FromSpherical(&RS,&RE);
  }
  if (RE.Alt!=ToAlt) {              /* Force final point to altitude ToAlt. */
    Step*=(ToAlt-RE.Alt)/(PrevAlt-RE.Alt);
    V_Mult(&B,-Step,&StepV);
    V_Add(To,&StepV,To);
  }
  return SUCCESS;
}



/* Returns the magnetic field vector *B at an earth point given in *S.
   *B is in local geocentric cartesian coordinates (X is east, Y is
   north, Z is up), and in units of Tesla. */

void IGRF_GetBField_ord(S,B,order)
Spherical *S;
Cartesian *B;
int    order;
{
  register int n,m; /* Sum iterators, */

  double Phi;                  /* Longitude, */
  double Theta;                /* Geocentric co-latitude, */
  double AoR;                  /* Mean earth radius (a) over r, */
  double AoRn2;                /* AoR raised to the n+2. */
  double Bu,Bw,Bn;             /* Field components (w:west). */
  double Term,mPhi;            /* Temporary storage areas. */
  double BuTerm,BwTerm,BnTerm;

  /* Local storage of Schmidt quasi-normalized Legendre associated
     functions and derivatives w.r.t. Theta. CAUTION: one row for each
     value of m, not for n, as above. */

  static double SchP[NMAX+1][NMAX+1],dSchP[NMAX+1][NMAX+1];


  if (!S->R) {
    fprintf(stderr,"Warning: IGRF model not applicable at earth's center.\n");
    B->X=B->Y=B->Z=0.0;
    return;
  }

  /* Make certain that sin(Theta) will be non-zero. */

  if (S->Ph<POLES)
    Theta=POLES;
  else if (S->Ph>PI-POLES)
    Theta=PI-POLES;
  else
    Theta=S->Ph;

  /* Calculate P and dP values. */

  for (m=0;m<=NMAX;m++)
    Schmidt_Array((unsigned int)NMAX,(unsigned int)m,Theta,SchP[m],dSchP[m]);

  /* IGRF spherical harmonic expansion. */

  AoR=MEAN_R/S->R;
  Phi=S->Th;
  Bu=Bw=Bn=0.0;
  AoRn2=AoR*AoR;
  for (n=1;n<=order;n++) {
    mPhi=BuTerm=BwTerm=BnTerm=0.0;
    for (m=0;m<=n;m++) {
      Term=g[n][m]*cos(mPhi)+h[n][m]*sin(mPhi);
      BuTerm+=Term*SchP[m][n];
      BwTerm+=(-g[n][m]*sin(mPhi)+h[n][m]*cos(mPhi))*m*SchP[m][n];
      BnTerm+=Term*dSchP[m][n];
      mPhi+=Phi;
    }
    AoRn2*=AoR;
    Bu+=(n+1.0)*AoRn2*BuTerm;
    Bw+=AoRn2*BwTerm;
    Bn+=AoRn2*BnTerm;
  }

  /* Return local geocentric components. */

  B->Y=NANOTESLA_TO_TESLA(Bn);
  B->X=NANOTESLA_TO_TESLA(-Bw/sin(Theta));
  B->Z=NANOTESLA_TO_TESLA(Bu);
  return;
}

