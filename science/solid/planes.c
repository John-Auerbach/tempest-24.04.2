/* PLANE-LINE INTERSECTIONS.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>
#include "advmath.h"

#include "user.h"
#include "solid.h"


/* Finds the range of values of t for which the line (segment)
   (*A)+(*v)*t, t in [*t0,*t1], lies within the subspace bounded by the
   side planes of the polygonal surface *S. The side planes form a
   cylindrical object whose boundary (*S's side planes) is considered
   part of its interior. TRUE is returned iff intersection takes place.
   The resulting range is returned in [*t0,*t1]. */

BOOLEAN Int_HalfLineCrossSides(A,v,S,t0,t1)
Cartesian *A;
Cartesian *v;
Surface   *S;
double    *t0;
double    *t1;
{
  double a,b,Temp;
  Side   *Sd;

  Sd=S->Sd;                                 /* Solutions sets Qi. */
  while (Sd) {
    b=(Sd->Pi).d-V_Dot(A,&((Sd->Pi).u));
    if (V_DotIsZero(v,&((Sd->Pi).u),&a)) {
      if (b<0.0)
	return FALSE;
    } else {
      Temp=b/a;
      if (a<0.0 && Temp>*t0)
	*t0=Temp;
      else if (a>0.0 && Temp<*t1)
	*t1=Temp;
      if (*t1<*t0)
	return FALSE;
    }
    Sd=Sd->Nxt;
  }
  return TRUE;
}


/* Finds the smallest non-negative value of *t such that the
   expression (*A)+(*v)*(*t) yields a point on the surface *S. TRUE is
   returned iff intersection takes place. */

BOOLEAN Int_NearHalfLineCrossPoly(A,v,S,t)
Cartesian *A;
Cartesian *v;
Surface   *S;
double    *t;
{
  double t1,a,b;

  b=(S->P).d-V_Dot(A,&((S->P).u));          /* Solution set S. */
  if (!V_DotIsZero(v,&((S->P).u),&a)) {
    if ((*t=b/a)<0.0)
      return FALSE;
    t1=(*t);
  } else if (-PLANE_DISTANCE<=b && b<=PLANE_DISTANCE) {
    *t=0.0;
    t1=HUGE_VAL;
  } else
    return FALSE;

  return Int_HalfLineCrossSides(A,v,S,t,&t1);
}
