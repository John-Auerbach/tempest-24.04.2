/* CARTESIAN VECTORS.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>

#include "user.h"
#include "advmath.h"


/* Vector addition: *D is assigned the vector sum of *A and *B. A, B,
   and D can all point to the same vector. */

void V_Add(A,B,D)
Cartesian *A;
Cartesian *B;
Cartesian *D;
{
  D->X=(A->X)+(B->X);
  D->Y=(A->Y)+(B->Y);
  D->Z=(A->Z)+(B->Z);
  return;
}


/* Vector subtraction: *D is assigned the vector difference of *A and
   *B, i.e.  (*D)=(*A)-(*B). A, B, and D can all point to the same
   vector. */

void V_Sub(A,B,D)
Cartesian *A;
Cartesian *B;
Cartesian *D;
{
  D->X=(A->X)-(B->X);
  D->Y=(A->Y)-(B->Y);
  D->Z=(A->Z)-(B->Z);
  return;
}


/* Multiplication by a scalar: *D is assigned the product of vector *A
   and the scalar B. A and D can point to the same vector. */

void V_Mult(A,B,D)
Cartesian *A;
double    B;
Cartesian *D;
{
  D->X=(A->X)*B;
  D->Y=(A->Y)*B;
  D->Z=(A->Z)*B;
  return;
}


/* Vector angle: returns in *R the angle between vectors *A and *B, in
   radians. A and B can point to the same vector. FAILURE is returned iff
   *A or *B is the zero vector (non-directional). */

RETCODE V_Angle(A,B,R)
Cartesian *A;
Cartesian *B;
double    *R;
{
  double MagA,MagB,Dot,Cos;

  Dot=V_Dot(A,B);
  MagA=V_Mag(A);
  MagB=V_Mag(B);
  if (!MagA || !MagB)
    return FAILURE;
  Cos=Dot/(MagA*MagB);
  if (Cos<-1.0)        /* Rounding error might yield |Cos|>1.0. */
    Cos=(-1.0);
  else if (Cos>1.0)
    Cos=1.0;
  *R=acos(Cos);
  return SUCCESS;
}


/* Dot product: returns the dot product of vectors *A and *B. A and B
   can point to the same vector. */

double V_Dot(A,B)
Cartesian *A;
Cartesian *B;
{
  return (A->X)*(B->X)+(A->Y)*(B->Y)+(A->Z)*(B->Z);
}


/* Dot product evaluation: calculates the dot product of vectors *A
   and *B in *R. It returns TRUE iff A and B are perpendicular. A and B
   can point to the same vector. */

BOOLEAN V_DotIsZero(A,B,D)
Cartesian *A;
Cartesian *B;
double    *D;
{
  double MagA,MagB,Temp;

  *D=V_Dot(A,B);
  MagA=V_Mag(A);
  MagB=V_Mag(B);
  if (!MagA || !MagB)
    return TRUE;
  Temp=(*D/(MagA*MagB));
  return -PERPENDICULAR_COSINE<Temp && Temp<PERPENDICULAR_COSINE;
}


/* Cross product: *D is assigned the cross product of vectors *A and
   *B. A, B, and D can all point to the same vector. */

void V_Cross(A,B,D)
Cartesian *A;
Cartesian *B;
Cartesian *D;
{  
  double TempX,TempY;
  
  TempX=(A->Y)*(B->Z)-(A->Z)*(B->Y);
  TempY=(A->Z)*(B->X)-(A->X)*(B->Z);
  D->Z=(A->X)*(B->Y)-(A->Y)*(B->X);
  D->X=TempX;
  D->Y=TempY;
  return;
}


/* Cross product evaluation: calculates the cross product of vectors
   *A and *B in *D. It returns TRUE iff A and B are parallel. A, B and D
   can all point to the same vector. */

BOOLEAN V_CrossIsZero(A,B,D)
Cartesian *A;
Cartesian *B;
Cartesian *D;
{  
  double MagA,MagB;

  V_Cross(A,B,D);
  MagA=V_Mag(A);
  MagB=V_Mag(B);
  if (!MagA || !MagB)
    return TRUE;
  return V_Mag(D)<MagA*MagB*PARALLEL_SINE;
}


/* Magnitude: returns the magnitude (length) of vector *A. */

double V_Mag(A)
Cartesian *A;
{
  return sqrt((A->X)*(A->X)+(A->Y)*(A->Y)+(A->Z)*(A->Z));
}


/* Turn to unit: *D is assigned the unit vector in the direction of
   *A. A and D can point to the same vector. FAILURE is returned iff *A
   is the zero vector (non-directional). */

RETCODE V_Unit(A,D)
Cartesian *A;
Cartesian *D;
{
  double Length;

  if (!(Length=V_Mag(A)))
    return FAILURE;
  V_Mult(A,1.0/Length,D);
  return SUCCESS;
}
