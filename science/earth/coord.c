/* EARTH COORDINATE TRANSFORMATIONS: EARTH CENTERED ROTATING
   GEOCENTRIC (SPHERICAL), GEODETIC (LONGITUDE,LATITUDE,ALTITUDE), LOCAL
   GEOCENTRIC AND GEODETIC.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>
#include "advmath.h"

#include "user.h"
#include "earth.h"


/* Returns in *S the spherical coordinates of the point whose earth
   coordinates are given in *E. The transformation taking place mainly
   consists of a geodetic to geocentric latitute conversion. */

void E_ToSpherical(E,S)
Earth     *E;
Spherical *S;
{
  double cosLat,sinLat,Rhoa,Rhob,X,Y;

  cosLat=cos(E->Lat);
  sinLat=sin(E->Lat);
  Rhoa=sqrt(cosLat*cosLat+sinLat*sinLat/RATIO_SQR);
  Rhob=sqrt(sinLat*sinLat+cosLat*cosLat*RATIO_SQR);
  X=cosLat*(EARTH_A/Rhoa+E->Alt);
  Y=sinLat*(EARTH_B/Rhob+E->Alt);
  if (fabs(X)<1.0 && fabs(Y)<1.0) { /* Earth's center. */
    S->R=0.0;
    S->Ph=0.0;
  } else {
    S->R=sqrt(X*X+Y*Y);
    S->Ph=HF_PI-atan2(Y,X);
  }
  S->Th=E->Long;
  return;
}


/* Returns in *E the earth coordinates of the point whose spherical
   coordinates are given in *S. The transformation taking place mainly
   consists of a geocentric to geodetic latitute conversion. */

void E_FromSpherical(S,E)
Spherical *S;
Earth     *E;
{
  double Beta,X,Y,A,C,k,f,df;

  E->Long=S->Th;

  /* Boundary cases. */

  Beta=HF_PI-S->Ph;
  if (S->Ph<POLES || S->Ph>PI-POLES) { /* Latitude of 90 or -90 degrees. */
    E->Alt=S->R-EARTH_B;
    E->Lat=Beta;
    return;
  } else if (!Beta) {                  /* Latitude of 0 degrees. */
    E->Alt=S->R-EARTH_A;
    E->Lat=Beta;
    return;
  } else if (!S->R) {                  /* Earth's center. */
    E->Alt=(-EARTH_B);
    E->Lat=HF_PI;
    return;
  }

  /* General case; here Y is non-zero since Beta and S->R are
     non-zero. */

  X=S->R*cos(Beta);
  Y=S->R*sin(Beta);
  A=(X/Y)*RATIO;
  C=(EARTH_A/Y)*RATIO-EARTH_B/Y;
  k=Beta;

  /* Approximate by Newton-Raphson method. Conversion converges. */

  while (1) {
    f=A*sin(k)-cos(k)-C*sin(2.0*k)/2.0;
    if (fabs(f)<=LATITUDE_CONVERSION_ACCURACY)
      break;
    df=A*cos(k)+sin(k)-C*cos(2.0*k);
    k-=f/df;
  }

  E->Lat=atan(RATIO*tan(k));
  E->Alt=sqrt(Sqr(X-EARTH_A*cos(k))+Sqr(Y-EARTH_B*sin(k)));
  if (Sqr(S->R/EARTH_B)<RATIO_SQR*Sqr(cos(k))+Sqr(sin(k)))
    E->Alt=(-E->Alt);
  return;
}


/* Returns in *F the local coordinate system axes at an earth point
   given in *E, if UseEarth is TRUE, or *S, otherwise. The local system
   is geodetic or geocentric, respectively. In both cases, the I vector
   (x-axis unit vector) is pointing towards the east, J (y-axis)
   northwards, and K (z-axis) upwards; the unit vectors are described in
   earth-centered rotating coordinates. */

void E_LocalFrame(UseEarth,E,S,F)
BOOLEAN   UseEarth;
Earth     *E;
Spherical *S;
Frame     *F;
{
  Spherical Up,East,North;
  double    Phi,Theta;

  if (UseEarth) {
    Phi=HF_PI-E->Lat;
    Theta=E->Long;
  } else {
    Phi=S->Ph;
    Theta=S->Th;
  }

  East.R=1.0;
  East.Ph=HF_PI;
  East.Th=Theta+HF_PI;
  S_ToCartesian(&East,&(F->I));

  North.R=1.0;
  North.Ph=Phi-HF_PI;
  North.Th=Theta;
  S_ToCartesian(&North,&(F->J));

  Up.R=1.0;
  Up.Ph=Phi;
  Up.Th=Theta;
  S_ToCartesian(&Up,&(F->K));

  return;
}


/* Converts a vector *A in local geocentric coordinates to local
   geodetic coordinates in *D, assuming that *A measures a quantity at an
   earth point given in *E and *S. A and D can point to the same vector. */

void E_LocalGeocToGeod(E,S,A,D)
Earth     *E;
Spherical *S;
Cartesian *A;
Cartesian *D;
{
  double Delta,TempY;

  Delta=S->Ph-(HF_PI-E->Lat);
  D->X=A->X;
  TempY=A->Y*cos(Delta)-A->Z*sin(Delta);
  D->Z=A->Z*cos(Delta)+A->Y*sin(Delta);
  D->Y=TempY;
  return;
} 


/* Converts a vector *A in local geodetic coordinates to local
   geocentric coordinates in *D, assuming that *A measures a quantity at
   an earth point given in *E and *S. A and D can point to the same
   vector. */

void E_LocalGeodToGeoc(E,S,A,D)
Earth     *E;
Spherical *S;
Cartesian *A;
Cartesian *D;
{
  double Delta,TempY;

  Delta=S->Ph-(HF_PI-E->Lat);
  D->X=A->X;
  TempY=A->Y*cos(Delta)+A->Z*sin(Delta);
  D->Z=A->Z*cos(Delta)-A->Y*sin(Delta);
  D->Y=TempY;
  return;
} 


/* Returns in M the transformation matrix from local geodetic
   coordinates to earth centered rotating coordinates at an earth point
   *E. */

void E_MFromLocalGeod(E,M)
Earth  *E;
Matrix M;
{
  Frame F;

  E_LocalFrame(TRUE,E,NULL,&F);
  M_ToGlobal(&F,M);
  return;
}


/* Returns in M the transformation matrix from earth centered rotating
   coordinates to local geodetic coordinates at an earth point *E. */

void E_MToLocalGeod(E,M)
Earth  *E;
Matrix M;
{
  Frame F;

  E_LocalFrame(TRUE,E,NULL,&F);
  M_FromGlobal(&F,M);
  return;
}


/* Returns in M the transformation matrix from local geocentric
   coordinates to earth centered rotating coordinates at an earth point
   *S. */

void E_MFromLocalGeoc(S,M)
Spherical *S;
Matrix    M;
{
  Frame F;

  E_LocalFrame(FALSE,NULL,S,&F);
  M_ToGlobal(&F,M);
  return;
}


/* Returns in M the transformation matrix from earth centered rotating
   coordinates to local geocentric coordinates at an earth point *S. */

void E_MToLocalGeoc(S,M)
Spherical *S;
Matrix    M;
{
  Frame F;

  E_LocalFrame(FALSE,NULL,S,&F);
  M_FromGlobal(&F,M);
  return;
}
