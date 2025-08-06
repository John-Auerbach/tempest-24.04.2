/* SOLVER OF EQUATION A*cos(W*t)+B*sin(W*t)+C*t+D=0.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>

#include "user.h"
#include "advmath.h"


#define f(t) cos(omega*(t)-psi)+u*(t)+v /* Shorthand. */

typedef enum {ONE_MORE_SLN,MORE_SLNS,FINISHED} SLN_STATUS;

static SLN_STATUS SlnStatus; /* Status of solver. */

static double omega,psi,u,v;        /* Parameters of current equation. */
static double Period;               /* The ratio TWO_PI/omega. */
static double Accuracy,MinAccuracy; /* Accuracy of ApproxSln() result. */


/* AUXILIARY ROUTINES. */

/* Returns TRUE iff f(t)=0 has a solution in the interval [a,b],
   provided f(t) is monotonic over [a,b]. This solution is returned in
   *Sln. */

static BOOLEAN ApproxSln(a,b,Sln)
double a;
double b;
double *Sln;
{
  double h,fa,fb,fh;

  fa=f(a);
  fb=f(b);
  if (MinAccuracy<=fa && fa<=Accuracy) {
    *Sln=a;
    return TRUE;
  }
  if (MinAccuracy<=fb && fb<=Accuracy) {
    *Sln=b;
    return TRUE;
  }
  if ((fa>0.0 && fb>0.0) || (fa<0.0 && fb<0.0))
    return FALSE;
  while (1) {
    h=(b*fa-a*fb)/(fa-fb);
    fh=f(h);

    /* Deviation from theory: preventing infinite loop due to floating
       point rounding by examining saturation in h (h==a or h==b). */

    if ((MinAccuracy<=fh && fh<=Accuracy) ||
	h==a || h==b)
      break;
    if ((fa>0.0 && fh>0.0) || (fa<0.0 && fh<0.0)) {
      a=h;
      fa=fh;
    } else {
      b=h;
      fb=fh;
    }
  }
  *Sln=h;
  if (fh<MinAccuracy || Accuracy<fh)
    fprintf(stderr,"Warning: equation solver rounding error (%le,%le).\n",
	    Accuracy,fh);
  return TRUE;
}

/* If NewEqn=TRUE, it calculates the first solution of f(t)=0 in
   [0,+inf), if u!=0, or in [0,2*PI/omega), if u=0. If a solution exists,
   TRUE is returned and *Sln contains the smallest non-negative root of
   f(t)=0.  Otherwise, FALSE is returned.
   Subsequent calls to the same function, with NewEqn=FALSE, are only
   allowed if the static variable SlnStatus!=FINISHED right before the
   time of the new call. (The flag SlnStatus indicates that the search
   for solutions is completed, if SlnStatus=FINISHED. If
   SlnStatus!=FINISHED, the search is incomplete and additional solutions
   MIGHT exist.) Such subsequent calls place in *Sln all the other
   solutions (in case of multiple solutions) in non-decreasing order, and
   return TRUE; FALSE is returned when there are no more solutions. */

static BOOLEAN GetSln(NewEqn,Sln)
BOOLEAN NewEqn;
double	*Sln;
{
  static double NextSln;
  static double miu0,miu1,Seq1,Seq2;

  double Arccos,Temp,Term1,Term2;

  if (NewEqn) {

    /* u=0. */

    if (!u) {
      if (v<-1.0 || v>1.0) {              /* v<-1 or v>1. */
	SlnStatus=FINISHED;
	return FALSE;
      }
      if (v==1.0) {                       /* v=1. */
	SlnStatus=FINISHED;
        *Sln=(PI+psi)/omega;
      } else if (v==-1.0) {               /* v=-1. */
	SlnStatus=FINISHED;
	if (psi>=0.0)
	  *Sln=psi/omega;
	else
	  *Sln=(TWO_PI+psi)/omega;
      } else {                            /* -1<v<1. */
	SlnStatus=ONE_MORE_SLN;
	Arccos=acos(-v);
	if ((*Sln=(Arccos+psi)/omega)<0.0)
	  *Sln+=Period;
	if ((Temp=(-Arccos+psi)/omega)<0.0)
	  Temp+=Period;
	if (Temp>*Sln)
	  NextSln=Temp;
	else {
	  NextSln=(*Sln);
	  *Sln=Temp;
	}
      }
      return TRUE;

    /* u!=0. */

    } else {
      SlnStatus=FINISHED;
      miu0=(-(1.0+v)/u);                  /* Determining [lambda0,lambda1]. */
      if ((Temp=(-(-1.0+v)/u))>miu0)
	miu1=Temp;
      else {
	miu1=miu0;
	miu0=Temp;
      }
      if (miu1<0.0)                       /* Determining [miu0,miu1]. */
	return FALSE;
      if (miu0<0.0)
	miu0=0.0;
      if (u>=omega || u<=-omega)          /* |u|>=omega. */
	return ApproxSln(miu0,miu1,Sln);

      /* |u|<omega. */

      Temp=asin(u/omega);                 /* -PI/2<Temp<PI/2. */
      Term1=(psi+Temp);
      Term2=(psi+PI-Temp);

      /* Observe that 0<Term2-Term1<2*PI. Hence, (Term1+2*K*PI)/omega and
	 (Term2+2*K'*PI)/omega will never yield identical values. Seq1 and
	 Seq2 contain the values produced by the above equations,
	 respectively. */

      Temp=floor((miu0*omega-Term1)/TWO_PI)+ /* (Term1+2*K*PI)/omega>miu0. */
	   1.0;
      Seq1=(Term1+TWO_PI*Temp)/omega;
      Temp=floor((miu0*omega-Term2)/TWO_PI)+ /* (Term2+2*K'*PI)/omega>miu0. */
	   1.0;
      Seq2=(Term2+TWO_PI*Temp)/omega;
      while (1) {
        if (Seq1<Seq2) {
          Temp=Seq1;
          Seq1+=Period;
	} else {                          /* Seq1>Seq2. */
	  Temp=Seq2;
          Seq2+=Period;
	}
    	if (Temp>=miu1)
	  return ApproxSln(miu0,miu1,Sln);
	if (ApproxSln(miu0,Temp,Sln)) {
	  miu0=Temp;
	  SlnStatus=MORE_SLNS;
	  return TRUE;
        }
	miu0=Temp;
      }
    }
  }

  /* Additional solutions requested. */

  if (SlnStatus==ONE_MORE_SLN) {
    SlnStatus=FINISHED;
    *Sln=NextSln;
    return TRUE;
  }

  /* u!=0 and |u|<omega. */

  while (1) {                             /* SlnStatus==MORE_SLNS. */
    if (Seq1<Seq2) {
      Temp=Seq1;
      Seq1+=Period;
    } else {
      Temp=Seq2;
      Seq2+=Period;
    }
    if (Temp>=miu1) {
      SlnStatus=FINISHED;
      return ApproxSln(miu0,miu1,Sln);
    }
    if (ApproxSln(miu0,Temp,Sln)) {
      miu0=Temp;
      return TRUE;
    }
    miu0=Temp;
  }
}


/* EXTERNAL INTERFACE. */

/* If NewEqn=TRUE, it calculates the first solution of
   x(t)=alpha*cos(NewOmega*t)+beta*sin(NewOmega*t)+gamma*t+delta=0 in
   [0,+inf), if gamma!=0, or in [0,2*PI/NewOmega), if gamma=0. If a
   solution exists, TRUE is returned and *Sln contains the smallest
   non-negative root of the above equation.  Otherwise, FALSE is
   returned. Subsequent calls to the same function, with NewEqn=FALSE,
   place in *Sln the other solutions (in case of multiple solutions) in
   non-decreasing order, and return TRUE; FALSE is returned when there
   are no more solutions. In case of infinite solutions, only 0 is
   returned.
   NewAccuracy determines the accuracy of the approximation method
   utilized to obtain solutions (applies for the non-degenerate case). It
   sets the upper bound for the error in x(t), i.e. fabs(x(t)) is bound
   by NewAccuracy. */

BOOLEAN Eqn_NextSolution(NewEqn,alpha,beta,gamma,delta,
			 NewOmega,NewAccuracy,Sln)
BOOLEAN NewEqn;
double	alpha;
double	beta;
double	gamma;
double	delta;
double  NewOmega;
double  NewAccuracy;
double	*Sln;
{
  if (NewEqn) {
    if (!alpha || fabs(beta/alpha)>RELATIVE_IMPORTANCE) {
      if (!beta) {
        SlnStatus=FINISHED;
	if (!gamma) {
	  if (delta)
	    return FALSE;
	  *Sln=0.0;       /* Infinite solutions, return first one. */
	  return TRUE;
	}
	*Sln=(-delta/gamma);
        return *Sln>=0.0;
      }
      psi=HF_PI;
      u=gamma/beta;
      v=delta/beta;
      Accuracy=fabs(NewAccuracy/beta);
    } else {
      psi=atan(beta/alpha);
      u=gamma*cos(psi)/alpha;
      v=delta*cos(psi)/alpha;
      Accuracy=fabs(NewAccuracy/alpha);
    }
    omega=NewOmega;
    MinAccuracy=(-Accuracy);
    Period=TWO_PI/omega;
    return GetSln(TRUE,Sln);
  }

  if (SlnStatus==FINISHED)
    return FALSE;
  return GetSln(FALSE,Sln);
}
