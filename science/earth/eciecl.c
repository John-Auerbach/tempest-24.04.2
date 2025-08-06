/* EARTH COORDINATE TRANSFORMATIONS: EARTH CENTERED INERTIAL
   EQUATORIAL TO ECLIPTIC AND BACK.

   Adam London - APLONDON@ATHENA.MIT.EDU, ADAM@NOVA
   Apostolos Lerios - TOLIS@NOVA.                   */


#include <math.h>
#include "advmath.h"

#include "earth.h"


/* The conversion formulas were copied from the "Explanatory
   Supplement To The Ephemeris," 1961, pages 98-99. */


/* Returns the transformation matrix from earth centered inertial
   equatorial to earth centered inertial ecliptic coordinates, at time
   Time. Time should be in Greenwich solar days from January 0, 1900. */

void E_MECIToECL(Time,M)
double Time;
Matrix M;
{
  double Days; /* Days since Jan 0.5, 1900. */

  Days=Time-0.5;
  M_xRotation(DTR(23.452294-3.5626E-7*Days-
		  1.23E-15*Days*Days+1.03E-20*Days*Days*Days),M);
  return;
}


/* Returns the transformation matrix from earth centered inertial
   ecliptic to earth centered inertial equatorial coordinates, at time
   Time. Time should be in Greenwich solar days from January 0, 1900. */

void E_MECLToECI(Time,M)
double Time;
Matrix M;
{
  double Days; /* Days since Jan 0.5, 1900. */

  Days=Time-0.5;
  M_xRotation(-DTR(23.452294-3.5626E-7*Days-
		   1.23E-15*Days*Days+1.03E-20*Days*Days*Days),M);
  return;
}


/* Converts the vector *A from earth centered inertial equatorial to
   earth centered inertial ecliptic coordinates, at time Time. Time
   should be in Greenwich solar days from January 0, 1900. The result is
   placed in *D. */

void E_ECIToECL(Time,A,D)
double    Time;
Cartesian *A;
Cartesian *D;
{
  Matrix M;

  E_MECIToECL(Time,M);
  M_VMult(M,A,D);
  return;
}


/* Converts the vector *A from earth centered inertial ecliptic to
   earth centered inertial equatorial coordinates, at time Time. Time
   should be in Greenwich solar days from January 0, 1900. The result is
   placed in *D. */

void E_ECLToECI(Time,A,D)
double    Time;
Cartesian *A;
Cartesian *D;
{
  Matrix M;

  E_MECLToECI(Time,M);
  M_VMult(M,A,D);
  return;
}
