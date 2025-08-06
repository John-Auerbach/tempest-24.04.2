/* ATTITUDE ROUTINES' DEVELOPMENT PLATFORM.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>
#include "advmath.h"

#include "attitude.h"


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
  Matrix    M;
  double    P,R,Y;
  Cartesian A,B;

  PYR_ToMatrix(DTR(-140.0),DTR(60.0),DTR(-100.0),M);
  PYR_FromMatrix(M,&P,&Y,&R);
  printf("%lf %lf %lf\n",RTD(P),RTD(Y),RTD(R));
  A.X=1.0;
  A.Y=2.0;
  A.Z=3.0;
  NASA_BodySETS(&A,&A);
  printf("%lf %lf %lf\n",A.X,A.Y,A.Z);
}
