/* EARTH MODELLING ROUTINES' DEVELOPMENT PLATFORM.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "advmath.h"

#include "earth.h"


void ShowCartesian(V)
Cartesian *V;
{
  printf("%lf %lf %lf\n\n",V->X,V->Y,V->Z);
  return;
}


void ShowSpherical(S)
Spherical *S;
{
  printf("%lf %lf %lf\n\n",S->R,S->Ph,S->Th);
  return;
}


void ShowEarth(E)
Earth *E;
{
  printf("%lf %lf %lf\n\n",E->Alt,E->Long,E->Lat);
  return;
}

void ShowMatrix(M)
Matrix M;
{
  int i,j;

  for (i=0;i<3;i++)
    printf("%lf %lf %lf\n",M[i][0],M[i][1],M[i][2]);
  return;
}


main()
{
  Earth     E;
  Spherical S;
  Cartesian B;

  IGRF_SetYear(1985.0);
  E.Long=DTR(0.0); E.Lat=DTR(0.0);

  for (E.Alt=0.0;E.Alt<=0.0;E.Alt+=1000.0) {
    E_ToSpherical(&E,&S);
    IGRF_GetBField(&S,&B);
    E_LocalGeocToGeod(&E,&S,&B,&B);
    B.X=TESLA_TO_GAUSS(B.X);
    B.Y=TESLA_TO_GAUSS(B.Y);
    B.Z=TESLA_TO_GAUSS(B.Z);
    printf("%7.2lf %7.2lf %7.0lf %7.5lf N %7.5lf E %7.5lf D.\n",
	   RTD(E.Long),RTD(E.Lat),E.Alt/1000.0,B.Y,B.X,-B.Z);
  }
}
