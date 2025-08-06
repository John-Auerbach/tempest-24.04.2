/* EARTH MODELLING ROUTINES' TESTER.

 *       adam@nova.stanford.edu
 *       aplondon@Athena.MIT.EDU
 *
 */


#include <stdio.h>
#include <math.h>
#include "advmath.h"
#include "earth.h"

#include "orbit.h"


BOOLEAN EQ_AP(X,Y)
double X;
double Y;
{
  return (fabs(X-Y)<1E-2);
}


void TestMoon()
{
  Cartesian A,B;
  Spherical S;
  double    Time;

  Time=DAY_FromYMDS((unsigned int)1992,(unsigned int)7,(unsigned int)3,
		    (unsigned long)0);
  Time+=(60.0+59.997528)/(3600.0*24.0)+1.0;

  MN_GetMoonPos(Time,&A);
  S_FromCartesian(&A,&S);

  if (!EQ_AP(S.Ph,HF_PI-DTR(12.448375)) || !EQ_AP(S.Th,DTR(137.331467))) {
    printf("Moon Error.\n");
    return;
  }

  printf("Moon passed (distance not tested).\n");
  return;
}


main()
{
  printf("\nTesting the orbit library (liborbit).\n\n");

  TestMoon();
}
