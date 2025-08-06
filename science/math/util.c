/* AUXILIARY ROUTINES.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>

#include "advmath.h"


/* Returns the square of X. */

double Sqr(X)
double X;
{
  return X*X;
}


/* Sign function: returns 1 if X is positive, -1 if X is negative, 0
   if X is zero. */

int Sign(X)
double X;
{
  if (X>0.0)
    return 1;
  else if (X<0.0)
    return -1;
  else
    return 0;
}


/* Returns the factorial of N. */

double Factorial(N)
unsigned int N;
{
  int    i;
  double Result;

  if (!N)
    return 1.0;
  Result=1.0;
  while (N>1)
    Result*=(N--);
  return Result;
}


/* Returns an approximation of the Modified Bessel Function of order
   Order, and argument X. From Hildbrand, Adv. Calculus for Applications,
   p.147-148. Valid for small values of X. */

double ModBessel(Order,X)
unsigned int Order;
double       X;
{
  return pow(X/2.0,(double)Order)/Factorial(Order);
}


/* Returns the value of the Legendre polynomial Pn(cos Theta) for
   given n and Theta. */

double LP(n,Theta)
unsigned int n;
double       Theta;
{
  return ALF(n,(unsigned int)0,Theta);
}


/* Places in *R1 and *R2 the roots of A*X*X+B*X+C. Iff any roots
   exist, SUCCESS is returned. In case of a single root, it is placed in
   both *R1 and *R2. In case of infinite roots (A=B=C=0), *R1 and *R2 are
   set to zero. */

RETCODE Quadratic(A,B,C,R1,R2)
double  A;
double  B;
double  C;
Complex *R1;
Complex *R2;
{
  double Disc;

  if (A!=0.0) {
    Disc=B*B-4*A*C;
    if (Disc>=0.0) {
      R1->R=(-B-sqrt(Disc))/(2*A);
      R2->R=(-B+sqrt(Disc))/(2*A);
      R1->I=R2->I=0.0;
    } else {
      R1->R=R2->R=(-B/(2*A));
      R1->I=sqrt(-Disc)/(2*A);
      R2->I=(-R1->I);
    }
    return SUCCESS;
  }
  if (B!=0.0) {
    R1->R=R2->R=(-C/B);
    R1->I=R2->I=0.0;
    return SUCCESS;
  }
  if (C!=0.0)
    return FAILURE;
  R1->R=R2->R=R1->I=R2->I=0.0;
  return SUCCESS;
}
