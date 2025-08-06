/* SPHERICAL COORDINATE CONVERSIONS.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>

#include "user.h"
#include "advmath.h"


/* Converts the Spherical vector *A to its Cartesian form. The result
   is returned in *D. */

void S_ToCartesian(A,D)
Spherical *A;
Cartesian *D;
{
  D->Z=(A->R)*cos(A->Ph);
  D->X=(A->R)*sin(A->Ph)*cos(A->Th);
  D->Y=(A->R)*sin(A->Ph)*sin(A->Th);
  return;
}


/* Converts the Cartesian vector *A to its Spherical form. The result
   is returned in *D. */

void S_FromCartesian(A,D)
Cartesian *A;
Spherical *D;
{
  D->R=V_Mag(A);
  if (!(D->R))
    D->Th=D->Ph=0.0;
  else {
    D->Ph=acos(A->Z/D->R);
    if (!A->Y && !A->X)
      D->Th=0.0;
    else
      D->Th=atan2(A->Y,A->X);
  }
  return;
}


/* Spherical coordinates' normalization: forces the components of *A
   to be within their normal ranges (R non-negative, Th from -PI
   exclusive to PI inclusive, Ph from 0 to PI). */

void S_Normalize(A)
Spherical *A;
{
  Cartesian Temp;

  S_ToCartesian(A,&Temp);
  S_FromCartesian(&Temp,A);
  return;
}


/* Returns a sequence of points sampling the cone whose vertex is
   located at the origin, whose axis direction is SetData->E, and whose
   (half) angle is SetData->Spread. SetData->dSpread is the sampling
   step. When requesting a new sequence, SetData->Spread must be in the
   range [0,HF_PI], SetData->dSpread should be positive, and NewSet must
   be TRUE. To retrieve new points in the same sequence, set NewSet to
   FALSE; do not alter the elements of the *SetData structure. The
   function will be returning TRUE as long as a sample point is found;
   this point will be returned in *EOut and will lie on the unit sphere
   centered at the origin. */

BOOLEAN S_NextSample(NewSet,SetData,EOut)
BOOLEAN        NewSet;
SphSampleParam *SetData;
Cartesian      *EOut;
{
  Cartesian Temp;
  Spherical S;
  double    Angle;

  if (NewSet) {

    /* Initialize horizontal iterator (-1 for dummy first iteration). */

    SetData->Spanj=floor(SetData->Spread/SetData->dSpread);
    SetData->j=(-SetData->Spanj)-1;

    /* Forcing first iteration to reset i. */

    SetData->CosSpread=0.0;

    /* Create orthonormal axis system. */

    S_FromCartesian(&SetData->E,&S);
    S.R=1.0;
    S_ToCartesian(&S,&SetData->Se);
    S.Ph-=HF_PI;
    S_ToCartesian(&S,&SetData->Si);
    V_Cross(&SetData->Se,&SetData->Si,&SetData->Sj);
  }

  while (1) {
    if (!SetData->CosSpread || SetData->i>SetData->Spani) {
      SetData->j++;                       /* Move to new vertical circle. */
      if (SetData->j>SetData->Spanj)
	return FALSE;
      V_Mult(&SetData->Sj,sin(SetData->j*SetData->dSpread),&SetData->Ej);
      if ((SetData->CosSpread=cos(SetData->j*SetData->dSpread))>
	  PERPENDICULAR_COSINE) {
	SetData->dSideSpread=SetData->dSpread/SetData->CosSpread;
	SetData->Spani=floor(PI*SetData->CosSpread/(2.0*SetData->dSpread));
	SetData->i=(-SetData->Spani);
      } else {
	SetData->CosSpread=0.0;
	SetData->Ej=SetData->Sj;          /* Make sure SetData->Ej is unit. */
	if (SetData->j<0)
	  V_Mult(&SetData->Ej,-1.0,&SetData->Ej);
      }
    } else                                /* Move on vertical circle. */
      SetData->i++;

    /* Find probable sample vector. */

    if (SetData->CosSpread) {
      V_Mult(&SetData->Se,
	     SetData->CosSpread*cos(SetData->i*SetData->dSideSpread),EOut);
      V_Add(&SetData->Ej,EOut,EOut);
      V_Mult(&SetData->Si,
	     SetData->CosSpread*sin(SetData->i*SetData->dSideSpread),&Temp);
      V_Add(&Temp,EOut,EOut);
    } else
      *EOut=SetData->Ej;

    /* Return sample vector if within the cone. */

    V_Angle(EOut,&SetData->E,&Angle);
    if (Angle<=SetData->Spread)
      return TRUE;
  }
}
