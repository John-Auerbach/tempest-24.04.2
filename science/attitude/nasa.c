/* NASA TO SETS COORDINATE TRANSFORMATIONS AND BACK.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>
#include "advmath.h"

#include "attitude.h"


/* BODY COORDINATES. */

/* Returns in M the transformation matrix from NASA to SETS body
   coordinates. The same function can return the reverse transformation
   matrix. */

void NASA_MBodySETS(M)
Matrix M;
{
  M_Unit(M);
  M[0][0]=(-1.0);
  M[2][2]=(-1.0);
  return;
}

/* Returns in *R the SETS body coordinates of a vector *V, specified
   in NASA body coordinates. The same function can perform the reverse
   transformation. */

void NASA_BodySETS(V,R)
Cartesian *V;
Cartesian *R;
{
  R->X=(-V->X);
  R->Y=V->Y;
  R->Z=(-V->Z);
  return;
}


/* LVLH COORDINATES. */

/* Returns in M the transformation matrix from NASA to SETS LVLH
   coordinates. The same function can return the reverse transformation
   matrix. */

void NASA_MLVLHSETS(M)
Matrix M;
{
  M_Unit(M);
  M[1][1]=(-1.0);
  M[2][2]=(-1.0);
  return;
}

/* Returns in *R the SETS LVLH coordinates of a vector *V, specified
   in NASA LVLH coordinates. The same function can perform the reverse
   transformation. */

void NASA_LVLHSETS(V,R)
Cartesian *V;
Cartesian *R;
{
  R->X=V->X;
  R->Y=(-V->Y);
  R->Z=(-V->Z);
  return;
}
