/* EARTH COORDINATE TRANSFORMATIONS: EARTH CENTERED INERTIAL TO EARTH
   (LONGITUDE,LATITUDE,ALTITUDE) AND BACK.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>
#include "advmath.h"

#include "earth.h"


/* Converts *A from earth centered inertial coordinates to earth
   coordinates, at time Time. Time should be in Greenwich solar days from
   January 0, 1900. The result is placed in *E. */

void E_FromECI(Time,A,E)
double    Time;
Cartesian *A;
Earth     *E;
{
  Spherical S;
  Cartesian R;

  E_ECIToECR(Time,A,&R);
  S_FromCartesian(&R,&S);
  E_FromSpherical(&S,E);
  return;
}


/* Converts *E from earth coordinates to earth centered inertial
   coordinates, at time Time. Time should be in Greenwich solar days from
   January 0, 1900. The result is placed in *D. */

void E_ToECI(Time,E,D)
double    Time;
Earth     *E;
Cartesian *D;
{
  Spherical S;

  E_ToSpherical(E,&S);
  S_ToCartesian(&S,D);
  E_ECRToECI(Time,D,D);
  return;
}
