/* EARTH COORDINATE TRANSFORMATIONS: LOCAL VERTICAL AND HORIZONAL
   (LVLH) TO EARTH CENTERED INERTIAL COORDINATES AND BACK.

   Adam London - APLONDON@ATHENA.MIT.EDU, ADAM@NOVA
   Apostolos Lerios - TOLIS@NOVA.                   */


#include <math.h>
#include "advmath.h"

#include "earth.h"


/* Returns in *F the LVLH coordinate system axes in ECI coordinates.
   R and V point to the satellite location and velocity, respectively.
   Returns FAILURE if the satellite is moving along its radial vector, or
   the radial vector is zero, (or if the satellite is not moving) in
   which case LVLH is undefined. */

RETCODE E_LVLHFrame(R,V,F)
Cartesian *R;
Cartesian *V;
Frame     *F;
{
  if (!V_Unit(R,&F->K))
    return FAILURE;
  V_Cross(R,V,&F->J);
  if (!V_Unit(&F->J,&F->J))
    return FAILURE;
  V_Cross(&F->J,&F->K,&F->I);
  return SUCCESS;
}


/* Returns in M the transformation matrix from earth centered inertial
   to LVLH coordinates, given the satellite location and velocity (*R and
   *V). Returns FAILURE if the satellite is moving along its radial
   vector, or the radial vector is zero, in which cases LVLH is
   undefined. */

RETCODE E_MECIToLVLH(R,V,M)
Cartesian *R;
Cartesian *V;
Matrix    M;
{
  Frame F;

  if (!E_LVLHFrame(R,V,&F))
    return FAILURE;
  M_FromGlobal(&F,M);
  return SUCCESS;
}


/* Returns in M the transformation matrix from LVLH to earth centered
   inertial coordinates, given the satellite location and velocity (*R
   and *V). Returns FAILURE if the satellite is moving along its radial
   vector, or the radial vector is zero, in which cases LVLH is
   undefined. */

RETCODE E_MLVLHToECI(R,V,M)
Cartesian *R;
Cartesian *V;
Matrix    M;
{
  Frame F;

  if (!E_LVLHFrame(R,V,&F))
    return FAILURE;
  M_ToGlobal(&F,M);
  return SUCCESS;
}


/* Converts *A from earth centered inertial to LVLH coordinates, given
   the satellite location and velocity (*R and *V). The result is placed
   in *D. Returns FAILURE if the satellite is moving along its radial
   vector, or the radial vector is zero, in which cases LVLH is
   undefined. */

RETCODE E_ECIToLVLH(R,V,A,D)
Cartesian *R;
Cartesian *V;
Cartesian *A;
Cartesian *D;
{
  Matrix M;

  if (!E_MECIToLVLH(R,V,M))
    return FAILURE;
  M_VMult(M,A,D);
  return SUCCESS;
}


/* Converts *A from LVLH to earth centered inertial coordinates, given
   the satellite location and velocity (*R and *V). The result is placed
   in *D. Returns FAILURE if the satellite is moving along its radial
   vector, or the radial vector is zero, in which cases LVLH is
   undefined. */

RETCODE E_LVLHToECI(R,V,A,D)
Cartesian *R;
Cartesian *V;
Cartesian *A;
Cartesian *D;
{
  Matrix M;

  if (!E_MLVLHToECI(R,V,M))
    return FAILURE;
  M_VMult(M,A,D);
  return SUCCESS;
}
