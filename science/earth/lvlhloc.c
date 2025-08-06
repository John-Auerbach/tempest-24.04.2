/* EARTH COORDINATE TRANSFORMATIONS: LVLH TO LOCAL GEOCENTRIC
   COORDINATES AND BACK.

   Adam London - APLONDON@ATHENA.MIT.EDU, ADAM@NOVA
   Apostolos Lerios - TOLIS@NOVA.                   */


#include <math.h>
#include "advmath.h"

#include "earth.h"


/* Returns, in *Angle, the angle by which the local geocentric
   coordinate system should be rotated around its z (up) axis, in order
   to overlap with the LVLH coordinate system of the satellite at *R,
   moving with velocity *V. The answer is in radians.  Returns SUCCESS,
   unless an error occurs. */

RETCODE E_AngleLocalLVLH(R,V,Angle)
Cartesian *R;
Cartesian *V;
double    *Angle;
{
  Cartesian Locali,LVLHi;

  V_Cross(R,V,&LVLHi);
  V_Cross(&LVLHi,R,&LVLHi);

  Locali.X=(-R->Y);
  Locali.Y=R->X;
  Locali.Z=0.0;

  if (!V_Angle(&LVLHi,&Locali,Angle))
    return FAILURE;
  if (LVLHi.Z<0.0)
    *Angle=(-(*Angle));
  return SUCCESS;
}


/* Returns in M the transformation matrix from local geocentric to
   LVLH coordinates for the satellite whose position is *R and velocity
   is *V. Returns SUCCESS, unless an error occurs. */

RETCODE E_MLocalToLVLH(R,V,M)
Cartesian *R;
Cartesian *V;
Matrix    M;
{
  double Theta;

  if (!E_AngleLocalLVLH(R,V,&Theta))
    return FAILURE;
  M_zRotation(Theta,M);
  return SUCCESS;
}


/* Returns in M the transformation matrix from LVLH coordinates (for
   the satellite whose position is *R and velocity is *V) to local
   geocentric coordinates. Returns SUCCESS, unless an error occurs. */

RETCODE E_MLVLHToLocal(R,V,M)
Cartesian *R;
Cartesian *V;
Matrix    M;
{
  double Theta;

  if (!E_AngleLocalLVLH(R,V,&Theta))
    return FAILURE;
  M_zRotation(-Theta,M);
  return SUCCESS;
}


/* Converts *A from local geocentric to LVLH coordinates, for the
   satellite whose position is *R and velocity is *V. The result is
   placed in *D. Returns SUCCESS, unless an error occurs. */

RETCODE E_LocalToLVLH(R,V,A,D)
Cartesian *R;
Cartesian *V;
Cartesian *A;
Cartesian *D;
{
  Matrix M;

  if (!E_MLocalToLVLH(R,V,M))
    return FAILURE;
  M_VMult(M,A,D);
  return SUCCESS;
}


/* Converts *A from LVLH coordinates (for the satellite whose position
   is *R and velocity is *V) to local geocentric coordinates. The result
   is placed in *D. Returns SUCCESS, unless an error occurs. */

RETCODE E_LVLHToLocal(R,V,A,D)
Cartesian *R;
Cartesian *V;
Cartesian *A;
Cartesian *D;
{
  Matrix M;

  if (!E_MLVLHToLocal(R,V,M))
    return FAILURE;
  M_VMult(M,A,D);
  return SUCCESS;
}
