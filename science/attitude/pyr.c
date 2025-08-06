/* ATTITUDE TRANFORMATION.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>
#include "advmath.h"

#include "attitude.h"


/* Returns the pitch (in *Pitch), yaw (in *Yaw), and roll (in *Roll),
   which correspond to the tranformation matrix M. The function assumes
   that these transformations (i) are right-handed and are (ii) applied
   in the following order: pitch, yaw, roll. Moreover, M transforms a
   vector from LVLH to body coordinates. The returned angles are in
   radians. */

void PYR_FromMatrix(M,Pitch,Yaw,Roll)
Matrix M;
double *Pitch;
double *Yaw;
double *Roll;
{
  double SinYaw,CosYaw;

  SinYaw=M[0][1];
  CosYaw=sqrt(M[1][1]*M[1][1]+M[2][1]*M[2][1]);
  *Yaw=atan2(SinYaw,CosYaw);
  if (CosYaw!=0.0) {
    *Pitch=atan2(-M[0][2]/CosYaw,M[0][0]/CosYaw);
    *Roll=atan2(-M[2][1]/CosYaw,M[1][1]/CosYaw);
    return;
  }
  *Roll=0.0;
  if (SinYaw>0.0)
    *Pitch=atan2(M[2][0],M[2][2]);
  else
    *Pitch=atan2(M[2][0],M[1][0]);
  return;
}


/* Returns in M the transformation matrix which corresponds to pitch
   Pitch, yaw Yaw, and roll Roll. The function assumes that these
   transformations (i) are right-handed and are (ii) applied in the
   following order: pitch, yaw, roll. Moreover, M transforms a vector
   from LVLH to body coordinates. The given angles must be in radians. */

void PYR_ToMatrix(Pitch,Yaw,Roll,M)
double Pitch;
double Yaw;
double Roll;
Matrix M;
{
  M[0][0]=cos(Yaw)*cos(Pitch);
  M[0][1]=sin(Yaw);
  M[0][2]=(-cos(Yaw)*sin(Pitch));
  M[1][0]=sin(Roll)*sin(Pitch)-cos(Roll)*sin(Yaw)*cos(Pitch);
  M[1][1]=cos(Roll)*cos(Yaw);
  M[1][2]=sin(Roll)*cos(Pitch)+cos(Roll)*sin(Yaw)*sin(Pitch);
  M[2][0]=cos(Roll)*sin(Pitch)+sin(Roll)*sin(Yaw)*cos(Pitch);
  M[2][1]=(-sin(Roll)*cos(Yaw));
  M[2][2]=cos(Roll)*cos(Pitch)-sin(Roll)*sin(Yaw)*sin(Pitch);
  return;
}
