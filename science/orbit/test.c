/* EARTH MODELLING ROUTINES' DEVELOPMENT PLATFORM.

 *       adam@nova.stanford.edu
 *       aplondon@Athena.MIT.EDU
 *
 */


#include <stdio.h>
#include <math.h>
#include "advmath.h"

#include "orbit.h"


void ShowCartesian(V)
Cartesian *V;
{
  printf("%lf %lf %lf\n\n",V->X,V->Y,V->Z);
  return;
}


TestDecay()
  {
    double A, E, I, RAAN, WP, N; 
    double dA, dE, dRAAN, dWP, n;
    double BalCoef = (221827.0/2.2)/((2.2)*(1208.48/0.3048/0.3048));

    A = 6808960.500000;
    E = 0.000817;
    I = DTR(28.345736);
    RAAN = DTR(332.484650);
    WP = DTR(257.674683);
    N = DTR(349.233490);

    SAT_GetPrecession(A, E, I, &dRAAN, &dWP);
    SAT_GetDecay(A, E, BalCoef, &dA, &dE);

    printf("dRAAN: %lf\n", dRAAN);
    printf("  dWP: %lf\n", dWP);
    printf("   dA: %lf\n", dA);
    printf("   dE: %lf\n", dE);
 


  }

TestSat()
{
  Cartesian R, V;
  double A, E, I, RAAN, WP, N; 

  R.X = 3791889.880154;
  R.Y = 4788992.238968;
  R.Z = 2998532.672531;

  V.X = -6298.848495;
  V.Y = 4129.123737;
  V.Z = 1371.181818;

  if (SAT_GetElements(&R, &V, &A, &E, &I, &RAAN, &WP, &N) == SUCCESS)
      { 
        printf("Success... \n");
        printf("   A: %lf\n", A);
        printf("   E: %le\n", E);
        printf("   I: %lf\n", RTD(I));
        printf("RAAN: %lf\n", RTD(RAAN));
        printf("  WP: %lf\n", RTD(WP));
        printf("   N: %lf\n", RTD(N));
      }
  else printf("Faliure.. ;-(\n");
}

TestSatPos()
{
  Cartesian R, V;
  double A, E, I, RAAN, WP, N; 

  A = 6677517.8624;
  E = 0.000616;
  I = DTR(28.23147);
  RAAN = DTR(290.72013);
  WP = DTR(280.56619);
  N = DTR(333.02234);

  SAT_GetSatPosition(A, E, I, RAAN, WP, N, &R, &V);
  printf("Position:\n");
  ShowCartesian(&R);
  printf("Velocity:\n");
  ShowCartesian(&V);
  

}



main()
{
  Cartesian B;
  Frame     F;
  Spherical S;
  Matrix    M1,M2;

  TestSat();

}
