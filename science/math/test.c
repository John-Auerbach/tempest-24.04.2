/* ADVANCED MATH ROUTINES' DEVELOPMENT PLATFORM.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>

#include "advmath.h"


void ShowCartesian(V)
Cartesian *V;
{
  printf("%lf %lf %lf\n",V->X,V->Y,V->Z);
  return;
}


void ShowComplex(C)
Complex *C;
{
  printf("%lf %lfi\n",C->R,C->I);
  return;
}


void ShowSpherical(S)
Spherical *S;
{
  printf("%lf %lf %lf\n\n",S->R,S->Ph,S->Th);
  return;
}


void ShowMatrix(M)
Matrix M;
{
  register int i;

  for (i=0;i<3;i++)
    printf("%lf %lf %lf\n",M[i][0],M[i][1],M[i][2]);
  putchar('\n');
  return;
}


void ShowQuat(Q)
Quaternion *Q;
{
  printf("%lf %lfi %lfj %lfk\n\n",Q->Q1,Q->Q2,Q->Q3,Q->Q4);
  return;
}


main()
{
  Cartesian A,B;
  Spherical S;
  Matrix    M;
  Complex   C1,C2;
  double    R,P,Y;

  SphSampleParam SP;
  Cartesian      Eij;
  int            i;
  double         Angle;

  SP.E.X=1.0; SP.E.Y=0.0; SP.E.Z=0.0;
  SP.Spread=DTR(90.0);
  SP.dSpread=DTR(30.0);
  if (!S_NextSample(TRUE,&SP,&Eij)) {
    printf("Spherical Error (sampling 1).\n");
    return;
  }
  i=0;
  do {
    i++;
    if (!V_Angle(&Eij,&SP.E,&Angle)) {
      printf("Spherical Error (sampling 2).\n");
      return;
    }
    printf("%lf ",RTD(Angle));
    ShowCartesian(&Eij);
  } while (S_NextSample(FALSE,&SP,&Eij));
}
