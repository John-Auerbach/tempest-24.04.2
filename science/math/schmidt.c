/* SCHMIDT QUASI-NORMALIZED LEGENDRE ASSOCIATED FUNCTIONS.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>

#include "advmath.h"


/* Returns the Schmidt quasi-normalization constant for the Legendre
   associated function Pn,m(cos Theta). */

double Schmidt_Coeff(n,m)
unsigned int n;
unsigned int m;
{
  double       D; /* Normalization coefficient denominator. */
  register int i;

  if (!m)
    return 1.0;
  D=1.0;
  for (i=n+m;i>n-m;i--)
    D*=i;
  return sqrt(2.0/D);
}


/* Fills in the two arrays P and dP with the values of all the Schmidt
   quasi-normalized Legendre associated functions, P, and their
   derivatives w.r.t Theta, dP, for given m, for all n such that
   m<=n<=Nmax, and for given Theta. P[n] receives the value of the
   normalized Pn,m(cos Theta). */

void Schmidt_Array(Nmax,m,Theta,P,dP)
unsigned int Nmax;
unsigned int m;
double       Theta;
double       *P;
double       *dP;
{
  double       Pmm,Pm1m,Pnm,dPmm,dPm1m,dPnm,sinTh,cosTh,N,M,QNorm;
  register int i,n;

  if (Nmax<m)
    return;

  n=m;                            /* Start from Pm,m, and dPm,m. */
  sinTh=sin(Theta);
  cosTh=cos(Theta);
  M=(double)m;
  Pmm=1.0;                        /* Calculate Pm,m(cos Theta). */
  for (i=2*m-1;i>2;i-=2)
    Pmm*=sinTh*i;
  dPmm=Pmm*M*cosTh;               /* Calculate dPm,m(cos Theta). */
  if (m>0)
    Pmm*=sinTh;
  QNorm=Schmidt_Coeff(n,m);       /* Normalize and store. */
  P[n]=QNorm*Pmm;
  dP[n++]=QNorm*dPmm;

  if (n>Nmax)
    return;
  dPm1m=(2.0*M+1.0)*(-sinTh*Pmm+cosTh*dPmm); /* dPm+1,m(cos Theta). */
  Pm1m=Pmm*(2.0*M+1.0)*cosTh;                /* Pm+1,m(cos Theta). */
  QNorm=Schmidt_Coeff((unsigned int)n,m);    /* Normalize and store. */
  P[n]=QNorm*Pm1m;
  dP[n++]=QNorm*dPm1m;

  while (n<=Nmax) {               /* Recursive calculation. */
    N=(double)n;
    dPnm=((2.0*N-1.0)*(cosTh*dPm1m-sinTh*Pm1m)-(N+M-1.0)*dPmm)/(N-M);
    dPmm=dPm1m;
    dPm1m=dPnm;
    Pnm=(cosTh*(2.0*N-1.0)*Pm1m-(N+M-1.0)*Pmm)/(N-M);
    Pmm=Pm1m;
    Pm1m=Pnm;
    QNorm=Schmidt_Coeff((unsigned int)n,m);  /* Normalize and store. */
    P[n]=QNorm*Pnm;
    dP[n++]=QNorm*dPnm;
  }
  return;
}
