/* QUATERNIONS.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>

#include "advmath.h"


/* Returns, in M, the 3x3 matrix form of the quaternion transformation
   *Q. A quaternion tranformation is an anticlockwise rotation about an
   axis; it is encoded in *Q as Q->Q1=cos(Theta/2),
   Q->Q2=sin(Theta/2)*cos(Alpha), Q->Q3=sin(Theta/2)*cos(Beta),
   Q->Q4=sin(Theta/2)*cos(Gamma), where Alpha, Beta And Gamma are the
   directional cosines of the axis and Theta is the rotation angle. e.g.
   the quaternion (cos(PI/4),sin(PI/4),0,0) transforms (0,1,0) to
   (0,0,1). */

void Q_Matrix(Q,M)
Quaternion *Q;
Matrix     M;
{
  M[0][0]=(Q->Q1)*(Q->Q1)+(Q->Q2)*(Q->Q2)-(Q->Q3)*(Q->Q3)-(Q->Q4)*(Q->Q4);
  M[0][1]=2.0*((Q->Q2)*(Q->Q3)-(Q->Q1)*(Q->Q4));
  M[0][2]=2.0*((Q->Q2)*(Q->Q4)+(Q->Q1)*(Q->Q3));
  M[1][0]=2.0*((Q->Q2)*(Q->Q3)+(Q->Q1)*(Q->Q4));
  M[1][1]=(Q->Q1)*(Q->Q1)-(Q->Q2)*(Q->Q2)+(Q->Q3)*(Q->Q3)-(Q->Q4)*(Q->Q4);
  M[1][2]=2.0*((Q->Q3)*(Q->Q4)-(Q->Q1)*(Q->Q2));
  M[2][0]=2.0*((Q->Q2)*(Q->Q4)-(Q->Q1)*(Q->Q3));
  M[2][1]=2.0*((Q->Q3)*(Q->Q4)+(Q->Q1)*(Q->Q2));
  M[2][2]=(Q->Q1)*(Q->Q1)-(Q->Q2)*(Q->Q2)-(Q->Q3)*(Q->Q3)+(Q->Q4)*(Q->Q4);
  return;
}


/* Multiplies the quaternions *A and *B and stores the result in *D;
   A, B, and D can all point to the same quaternion. */

void Q_Mult(A,B,D)
Quaternion *A;
Quaternion *B;
Quaternion *D;
{
  double TempQ1,TempQ2,TempQ3; /* Allow A, B, D to be identical pointers. */

  TempQ1=(A->Q1)*(B->Q1)-(A->Q2)*(B->Q2)-(A->Q3)*(B->Q3)-(A->Q4)*(B->Q4);
  TempQ2=(A->Q1)*(B->Q2)+(A->Q2)*(B->Q1)+(A->Q3)*(B->Q4)-(A->Q4)*(B->Q3);
  TempQ3=(A->Q1)*(B->Q3)-(A->Q2)*(B->Q4)+(A->Q3)*(B->Q1)+(A->Q4)*(B->Q2);
  D->Q4=(A->Q1)*(B->Q4)+(A->Q2)*(B->Q3)-(A->Q3)*(B->Q2)+(A->Q4)*(B->Q1);
  D->Q1=TempQ1;
  D->Q2=TempQ2;
  D->Q3=TempQ3;
  return;
}


/* Returns, in *D, the conjugate of the quaternion *A; A and D can
   point to the same quaternion. The conjugate of a quaternion is also
   its inverse, if *A's norm is 1; hence the name of the function. */

void Q_Invert(A,D)
Quaternion *A;
Quaternion *D;
{
  D->Q1=A->Q1;
  D->Q2=(-A->Q2);
  D->Q3=(-A->Q3);
  D->Q4=(-A->Q4);
  return;
}
