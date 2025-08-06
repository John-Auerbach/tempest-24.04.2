/* ATTITUDE ROUTINES' TESTER.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>
#include "advmath.h"

#include "attitude.h"


BOOLEAN EQ(X,Y)
double X;
double Y;
{
  return (fabs(X-Y)<1E-10);
}


BOOLEAN M_EQ(M,A00,A01,A02,A10,A11,A12,A20,A21,A22)
Matrix M;
double A00,A01,A02,A10,A11,A12,A20,A21,A22;
{
  return (EQ(M[0][0],A00) && EQ(M[0][1],A01) && EQ(M[0][2],A02) &&
          EQ(M[1][0],A10) && EQ(M[1][1],A11) && EQ(M[1][2],A12) &&
          EQ(M[2][0],A20) && EQ(M[2][1],A21) && EQ(M[2][2],A22));
}


void TestPYR()
{
  Matrix    M1,M2;
  double    P,R,Y;

  PYR_ToMatrix(DTR(90.0),DTR(180.0),DTR(180.0),M1);
  M_yRotation(DTR(90.0),M2);
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
            1.0,0.0,0.0,
            0.0,1.0,0.0,
            0.0,0.0,1.0)) {
    printf("PYR Error (to matrix).\n");
    return;
  }

  PYR_ToMatrix(DTR(-140.0),DTR(60.0),DTR(-100.0),M1);
  PYR_FromMatrix(M1,&P,&Y,&R);
  if (!EQ(P,DTR(-140.0)) || !EQ(Y,DTR(60.0)) || !EQ(R,DTR(-100.0))) {
    printf("PYR Error (from matrix).\n");
    return;
  }

  printf("PYR passed.\n");
  return;
}


void TestNASA()
{
  Cartesian A,B;
  Matrix    M;

  A.X=1.0; A.Y=2.0; A.Z=3.0;
  NASA_MBodySETS(M);
  M_VMult(M,&A,&B);
  NASA_BodySETS(&A,&A);
  if (!EQ(A.X,-1.0) || !EQ(A.Y,2.0) || !EQ(A.Z,-3.0) ||
      !EQ(B.X,-1.0) || !EQ(B.Y,2.0) || !EQ(B.Z,-3.0)) {
    printf("NASA Error (body SETS).\n");
    return;
  }

  A.X=1.0; A.Y=2.0; A.Z=3.0;
  NASA_MLVLHSETS(M);
  M_VMult(M,&A,&B);
  NASA_LVLHSETS(&A,&A);
  if (!EQ(A.X,1.0) || !EQ(A.Y,-2.0) || !EQ(A.Z,-3.0) ||
      !EQ(B.X,1.0) || !EQ(B.Y,-2.0) || !EQ(B.Z,-3.0)) {
    printf("NASA Error (body SETS).\n");
    return;
  }

  printf("NASA passed.\n");
  return;
}


main()
{
  printf("\nTesting the attitude library (libatt).\n\n");

  TestPYR();
  TestNASA();
}
