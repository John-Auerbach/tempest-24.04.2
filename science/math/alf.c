/* ASSOCIATED LEGENDRE FUNCTIONS.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>

#include "advmath.h"


/* Returns the value of the associated Legendre function Pn,m(cos Theta). */

double ALF(n,m,Theta)
unsigned int n;
unsigned int m;
double       Theta;
{
  double       Pmm,Pm1m,Pim,sinTh,cosTh,I,M;
  register int i;

  sinTh=sin(Theta);
  Pmm=1.0;               /* Calculate Pm,m(cos Theta). */
  for (i=2*m-1;i>0;i-=2)
    Pmm*=sinTh*i;
  if (n==m)              /* Pm,m(cos Theta) was requested. */
    return Pmm;
  
  cosTh=cos(Theta);
  M=(double)m;
  Pm1m=Pmm*(2.0*M+1.0)*cosTh; /* Find Pm+1,m(cos Theta). */
  if (n==m+1)
    return Pm1m;

  for (i=m+2;i<=n;i++) { /* Recursive calculation. */
    I=(double)i;
    Pim=(cosTh*(2.0*I-1.0)*Pm1m-(I+M-1.0)*Pmm)/(I-M);
    Pmm=Pm1m;
    Pm1m=Pim;
  }
  return Pim;
}


/* Returns the first derivative w.r.t. Theta of the associated
   Legendre function Pn,m(cos Theta). */

double ALF_Derivative(n,m,Theta)
unsigned int n;
unsigned int m;
double       Theta;
{
  double       Pmm,Pm1m,Pim,dPmm,dPm1m,dPim,sinTh,cosTh,I,M;
  register int i;

  sinTh=sin(Theta);
  cosTh=cos(Theta);
  M=(double)m;
  Pmm=1.0;               /* Calculate Pm,m(cos Theta). */
  for (i=2*m-1;i>2;i-=2)
    Pmm*=sinTh*i;
  dPmm=Pmm*M*cosTh;      /* Calculate dPm,m(cos Theta). */
  if (m>0)
    Pmm*=sinTh;
  if (n==m)              /* dPm,m(cos Theta) was requested. */
    return dPmm;

  dPm1m=(2.0*M+1.0)*(-sinTh*Pmm+cosTh*dPmm); /* Find dPm+1,m(cos Theta). */
  if (n==m+1)
    return dPm1m;

  Pm1m=Pmm*(2.0*M+1.0)*cosTh; /* Calculate Pm+1,m(cos Theta). */
  for (i=m+2;i<=n;i++) {      /* Recursive calculation. */
    I=(double)i;
    dPim=((2.0*I-1.0)*(cosTh*dPm1m-sinTh*Pm1m)-(I+M-1.0)*dPmm)/(I-M);
    dPmm=dPm1m;
    dPm1m=dPim;
    Pim=(cosTh*(2.0*I-1.0)*Pm1m-(I+M-1.0)*Pmm)/(I-M);
    Pmm=Pm1m;
    Pm1m=Pim;
  }
  return dPim;
}
