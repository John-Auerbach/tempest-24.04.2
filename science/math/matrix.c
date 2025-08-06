/* 3x3 MATRICES.

   Apostolos Lerios - TOLIS@NOVA. */


#include <string.h>
#include <math.h>

#include "advmath.h"


/* D becomes the unit matrix. */

void M_Unit(D)
Matrix D;
{
  D[0][0]=D[1][1]=D[2][2]=1.0;
  D[1][0]=D[0][1]=D[2][0]=0.0;
  D[0][2]=D[2][1]=D[1][2]=0.0;
  return;
}


/* D is assigned the transpose of the matrix M. M and D can point
   to the same matrix. */

void M_Transpose(M,D)
Matrix M;
Matrix D;
{
  double Temp;

  if (M!=D)
    memcpy(D,M,sizeof(Matrix));

  Temp=D[1][0]; D[1][0]=D[0][1]; D[0][1]=Temp;
  Temp=D[2][0]; D[2][0]=D[0][2]; D[0][2]=Temp;
  Temp=D[1][2]; D[1][2]=D[2][1]; D[2][1]=Temp;
  return;
}


/* Returns the determinant of matrix M. */

double M_Determinant(M)
Matrix M;
{
  return (M[0][0]*(M[1][1]*M[2][2]-M[1][2]*M[2][1])-
	  M[0][1]*(M[1][0]*M[2][2]-M[1][2]*M[2][0])+
	  M[0][2]*(M[1][0]*M[2][1]-M[1][1]*M[2][0]));
}


/* *D becomes the product of M, a matrix, and *V, a 3-dimensional vector.
   D and V can point to the same vector. */

void M_VMult(M,V,D)
Matrix    M;
Cartesian *V;
Cartesian *D;
{
  double TempX,TempY;

  TempX=M[0][0]*(V->X)+M[0][1]*(V->Y)+M[0][2]*(V->Z);
  TempY=M[1][0]*(V->X)+M[1][1]*(V->Y)+M[1][2]*(V->Z);
  D->Z=M[2][0]*(V->X)+M[2][1]*(V->Y)+M[2][2]*(V->Z);
  D->X=TempX;
  D->Y=TempY;
  return;
}


/* D becomes the x-axis rotation matrix. Theta is the angle of
   rotation, in radians.  Sign of Theta: + when D should transform
   (0,1,0) to (0,cos Th,-sin Th). */

void M_xRotation(Theta,D)
double Theta;
Matrix D;
{
  D[1][1]=D[2][2]=cos(Theta);
  D[1][2]=sin(Theta);
  D[2][1]=(-D[1][2]);
  D[0][0]=1.0;
  D[0][1]=D[0][2]=D[1][0]=D[2][0]=0.0;
  return;
}


/* D becomes the y-axis rotation matrix. Theta is the angle of
   rotation, in radians.  Sign of Theta: + when D should transform
   (0,0,1) to (-sin Th,0,cos Th). */

void M_yRotation(Theta,D)
double Theta;
Matrix D;
{
  D[0][0]=D[2][2]=cos(Theta);
  D[2][0]=sin(Theta);
  D[0][2]=(-D[2][0]);
  D[1][1]=1.0;
  D[0][1]=D[1][0]=D[1][2]=D[2][1]=0.0;
  return;
}


/* D becomes the z-axis rotation matrix. Theta is the angle of
   rotation, in radians.  Sign of Theta: + when D should transform
   (1,0,0) to (cos Th,-sin Th,0). */

void M_zRotation(Theta,D)
double Theta;
Matrix D;
{
  D[0][0]=D[1][1]=cos(Theta);
  D[0][1]=sin(Theta);
  D[1][0]=(-D[0][1]);
  D[2][2]=1.0;
  D[0][2]=D[1][2]=D[2][0]=D[2][1]=0.0;
  return;
}


/* D becomes the product of the matrices M1 and M2. All three
   paremeters can point to the same matrix. */

void M_MMult(M1,M2,D)
Matrix M1;
Matrix M2;
Matrix D;
{
  register int i,j,k;
  Matrix       Temp;

  for (i=0;i<3;i++)
    for (j=0;j<3;j++) {
      Temp[i][j]=0;
      for (k=0;k<3;k++)
	Temp[i][j]+=M1[i][k]*M2[k][j];
    }	

  memcpy(D,Temp,sizeof(Matrix));
  return;
}


/* D becomes the transformation matrix from the *F coordinate system to
   the global coordinate system (i.e. the system whose transformation martix
   is the unit matrix). */

void M_ToGlobal(F,D)
Frame  *F;
Matrix D;
{
  D[0][0]=(F->I).X;
  D[1][0]=(F->I).Y;
  D[2][0]=(F->I).Z;
  D[0][1]=(F->J).X;
  D[1][1]=(F->J).Y;
  D[2][1]=(F->J).Z;
  D[0][2]=(F->K).X;
  D[1][2]=(F->K).Y;
  D[2][2]=(F->K).Z;
  return;
}


/* D becomes the transformation matrix from the global coordinate system
   to the *F coordinate system. */

void M_FromGlobal(F,D)
Frame  *F;
Matrix D;
{
  D[0][0]=(F->I).X;
  D[1][0]=(F->J).X;
  D[2][0]=(F->K).X;
  D[0][1]=(F->I).Y;
  D[1][1]=(F->J).Y;
  D[2][1]=(F->K).Y;
  D[0][2]=(F->I).Z;
  D[1][2]=(F->J).Z;
  D[2][2]=(F->K).Z;
  return;
}


/* Generic function for rotation about any local axis system: D
   becomes the matrix which, applied on a global vector V which is the
   position vector of a point P, returns a vector V' which is the
   position vector of point P': P' is the rotation of P about the x, y,
   or z axis of the *F coordinate system by an angle Theta, in radians.
   See the rotation functions above for Theta's sign. */

#define GENERIC_ROTATION(FUNCTION_NAME,ROTATION_FUNCTION) \
                                                          \
void FUNCTION_NAME(F,Theta,D)                             \
Frame  *F;                                                \
double Theta;                                             \
Matrix D;                                                 \
{                                                         \
  Matrix FRotation,FToGlobal,GlobalToF;                   \
                                                          \
  M_FromGlobal(F,GlobalToF);                              \
  M_ToGlobal(F,FToGlobal);                                \
  ROTATION_FUNCTION(Theta,FRotation);                     \
  M_MMult(FRotation,GlobalToF,D);                         \
  M_MMult(FToGlobal,D,D);                                 \
  return;                                                 \
}

/* Functions based on generic above: x, y, and z local rotations. */

GENERIC_ROTATION(M_LocalxRotation,M_xRotation)
GENERIC_ROTATION(M_LocalyRotation,M_yRotation)
GENERIC_ROTATION(M_LocalzRotation,M_zRotation)
