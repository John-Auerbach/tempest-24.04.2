/* EARTH COORDINATE TRANSFORMATIONS: EARTH CENTERED ROTATING TO
   INERTIAL AND BACK.

   Adam London - APLONDON@ATHENA.MIT.EDU, ADAM@NOVA
   Apostolos Lerios - TOLIS@NOVA.                   */


#include <math.h>
#include "advmath.h"

#include "earth.h"


/* The conversion formulas were copied from the "Explanatory
   Supplement To The Ephemeris," 1961, pages 84-85. */


/* Returns the transformation matrix from earth centered inertial to
   earth centered rotating coordinates, at time Time. Time should be in
   Greenwich solar days from January 0, 1900. */

void E_MECIToECR(Time,M)
double Time;
Matrix M;
{
  double GMST; /* Greenwich Mean Sidereal Time. */
  double T;    /* Julian Centuries since Jan 0.5, 1900. */

  T=(floor(Time)-0.5)/36525.0;
  Time-=floor(Time);
  GMST=(6.6460656+2400.051262*T+2.581E-5*T*T)/24.0;
  M_zRotation(TWO_PI*(Time*SOLAR_SIDEREAL+GMST-floor(GMST)),M);
  return;
}


/* Returns the transformation matrix from earth centered rotating to
   earth centered inertial coordinates, at time Time. Time should be in
   Greenwich solar days from January 0, 1900. */

void E_MECRToECI(Time,M)
double Time;
Matrix M;
{
  double GMST; /* Greenwich Mean Sidereal Time. */
  double T;    /* Julian Centuries since Jan 0.5, 1900. */

  T=(floor(Time)-0.5)/36525.0;
  Time-=floor(Time);
  GMST=(6.6460656+2400.051262*T+2.581E-5*T*T)/24.0;
  M_zRotation((-TWO_PI)*(Time*SOLAR_SIDEREAL+GMST-floor(GMST)),M);
  return;
}


/* Converts the vector *A from earth centered inertial to earth
   centered rotating coordinates, at time Time. Time should be in
   Greenwich solar days from January 0, 1900. The result is placed in *D. */

void E_ECIToECR(Time,A,D)
double    Time;
Cartesian *A;
Cartesian *D;
{
  Matrix M;

  E_MECIToECR(Time,M);
  M_VMult(M,A,D);
  return;
}


/* Converts the vector *A from earth centered rotating to earth
   centered inertial coordinates, at time Time. Time should be in
   Greenwich solar days from January 0, 1900. The result is placed in *D. */

void E_ECRToECI(Time,A,D)
double    Time;
Cartesian *A;
Cartesian *D;
{
  Matrix M;

  E_MECRToECI(Time,M);
  M_VMult(M,A,D);
  return;
}
