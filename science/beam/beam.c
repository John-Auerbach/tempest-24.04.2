/* ELECTRON BEAM TRACER.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>
#include "advmath.h"
#include "solid.h"

#include "user.h"
#include "beam.h"


/* BEAM-SURFACE INTERSECTION IDENTIFICATION. */

static Cartesian i,j,k;   /* Specification of current beam. */
static Cartesian AxisDir;
static Generator *Gen;

/* Returns TRUE iff the beam intersects the surface *S prior to
   intersecting any other surface of the body that we have already
   considered. The time of intersection is returned in *t, in seconds. */

static BOOLEAN BeamHitSurface(S,t)
Surface *S;
double  *t;
{
  double    tmin,tau0,tau1,Dummy;
  Cartesian *u;
  Side      *Sd,*SdPrev,*SdNxt;
  Cartesian Point;

  /* Linear beam. */

  if (Gen->BeamType==LINEAR)
    return (Int_NearHalfLineCrossPoly(&(Gen->Loc),&(Gen->a),S,t) &&
	    *t<Gen->ImpactT);
  u=(&((S->P).u));

  /* Circular beam. */

  if (Gen->BeamType==CIRCULAR) {

    /* Beam parallel to P. */

    if (V_DotIsZero(u,&j,&Dummy) && V_DotIsZero(u,&k,&Dummy)) {
      if (fabs(V_Dot(&(Gen->Loc),u)-(S->P).d)>PLANE_DISTANCE)
	return FALSE;

      /* Starting point of beam lies on S. */

      Sd=S->Sd;
      while (Sd) {
	if (V_Dot(&(Gen->Loc),&((Sd->Pi).u))>(Sd->Pi).d)
	  break;
	Sd=Sd->Nxt;
      }
      if (!Sd) {
	*t=0.0;
	return TRUE;
      }

      /* Starting point of beam lies on P, but outside S. */

      tmin=Gen->Period;
      Sd=S->Sd;
      SdPrev=S->LastSd;
      SdNxt=Sd->Nxt;
      while (Sd) {
	u=(&((Sd->Pi).u));
	if (Eqn_NextSolution(TRUE,V_Dot(&(Gen->a),u),V_Dot(&(Gen->b),u),
			     0.0,V_Dot(&(Gen->d),u)-(Sd->Pi).d,Gen->omega,
			     LENGTH_ACCURACY,t) &&
	    tmin>*t)
	  do {
	    if (!Beam_Point(Gen,*t,&Point))
	      break;
	    if (V_Dot(&Point,&((SdPrev->Pi).u))<=(SdPrev->Pi).d &&
		V_Dot(&Point,&((SdNxt->Pi).u))<=(SdNxt->Pi).d) {
	      tmin=(*t);
	      break;
	    }
	  } while (Eqn_NextSolution(FALSE,0.0,0.0,0.0,0.0,0.0,0.0,t) &&
		   tmin>*t);
	SdPrev=Sd;
	Sd=Sd->Nxt;
	if (!(SdNxt=SdNxt->Nxt))
	  SdNxt=S->Sd;
      }
      *t=tmin;
      return (tmin<Gen->Period);
    }

    /* Beam not parallel to P. */

    if (Eqn_NextSolution(TRUE,V_Dot(&(Gen->a),u),V_Dot(&(Gen->b),u),
			 0.0,V_Dot(&(Gen->d),u)-(S->P).d,Gen->omega,
			 LENGTH_ACCURACY,t))
      do {
	if (!Beam_Point(Gen,*t,&Point))
	  return FALSE;
	Sd=S->Sd;
	while (Sd) {
	  if (V_Dot(&Point,&((Sd->Pi).u))>(Sd->Pi).d)
	    break;
	  Sd=Sd->Nxt;
	}
	if (!Sd)
	  return TRUE;
      } while (Eqn_NextSolution(FALSE,0.0,0.0,0.0,0.0,0.0,0.0,t));
    return FALSE;
  }

  /* Helical beam. */

  /* Helix axis parallel to P. */

  if (V_DotIsZero(&i,u,&Dummy)) {
    if (!Eqn_NextSolution(TRUE,V_Dot(&(Gen->a),u),V_Dot(&(Gen->b),u),
			  0.0,V_Dot(&(Gen->d),u)-(S->P).d,Gen->omega,
			  LENGTH_ACCURACY,t))
      return FALSE;
    tmin=HUGE_VAL;
    do {
      if (!Beam_Point(Gen,*t,&Point))
	break;
      tau0=0;
      tau1=HUGE_VAL;
      if (Int_HalfLineCrossSides(&Point,&AxisDir,S,&tau0,&tau1)) {
	tau0=ceil(tau0);
	if (tau0<=tau1 && (Dummy=(*t+tau0*Gen->Period))<tmin)
	  tmin=Dummy;
      }
    } while (Eqn_NextSolution(FALSE,0.0,0.0,0.0,0.0,0.0,0.0,t));
    *t=tmin;
    return (tmin<Gen->ImpactT);
  }

  /* Helix axis not parallel to P. */

  if (Eqn_NextSolution(TRUE,V_Dot(&(Gen->a),u),V_Dot(&(Gen->b),u),
		       V_Dot(&(Gen->c),u),V_Dot(&(Gen->d),u)-(S->P).d,
		       Gen->omega,LENGTH_ACCURACY,t))
    do {
      if (!Beam_Point(Gen,*t,&Point))
	return FALSE;
      Sd=S->Sd;
      while (Sd) {
	if (V_Dot(&Point,&((Sd->Pi).u))>(Sd->Pi).d)
	  break;
	Sd=Sd->Nxt;
      }
      if (!Sd)
	return TRUE;
    } while (Eqn_NextSolution(FALSE,0.0,0.0,0.0,0.0,0.0,0.0,t));
  return FALSE;
}


/* BEAM-BODY INTERSECTION IDENTIFICATION. */

/* Returns TRUE iff the beam produced by the generator *NewGen
   intersects any of the bodies in the body list BL. *B is the
   (presumably constant) magnetic field intensity in the volume of
   space traversed by the emitted electrons (in Tesla).  The function
   assumes that the Loc, Potential, Coelevation, Azimuth fields of
   *Gen contain meaningful values. In particular, Potential must be
   greater than zero.  Moreover, upon return, NewGen->ImpactS is a
   pointer to the intersected surface, and NewGen->ImpactT is the time
   of intersection, in seconds; the surface pointer is NULL if no
   intersection takes place.

   IMPORTANT: This function (or Beam_SecondaryHitBody() or
   Beam_Sources()) should be called prior to any of the other functions
   in this file. The reason is that all other functions use parameters of
   *NewGen set in the function below (or Beam_SecondaryHitBody() or
   Beam_Sources()). */

BOOLEAN Beam_HitBodyList(B,NewGen,BL)
Cartesian *B;
Generator *NewGen;
Body      *BL;
{
  double    t,Temp;
  Spherical Sph;
  Cartesian e;
  Surface   *S;

  Gen=NewGen;

  /* Direction of electron emission. */

  Sph.R=1.0;
  Sph.Ph=Gen->Coelevation;
  Sph.Th=Gen->Azimuth;
  S_ToCartesian(&Sph,&e);

  /* Beam preparation. */

  Gen->v=sqrt(2.0*QE*(Gen->Potential)/ME);
  if (V_CrossIsZero(&e,B,&j)) {
    V_Mult(&e,Gen->v,&(Gen->a));
    Gen->BeamType=LINEAR;
  } else {
    Temp=V_Mag(B);
    V_Mult(B,1/Temp,&i);
    V_Unit(&j,&j);
    V_Cross(&i,&j,&k);
    Gen->omega=QE*Temp/ME;
    Gen->Period=TWO_PI/Gen->omega;
    Temp=V_Dot(&e,&k)*(Gen->v)/(Gen->omega); /* Radius (rho). */
    V_Mult(&j,Temp,&(Gen->a));
    V_Mult(&k,Temp,&(Gen->b));
    if (V_DotIsZero(&e,&i,&Temp))
      Gen->BeamType=CIRCULAR;
    else {
      V_Mult(&i,Temp*(Gen->v),&(Gen->c));
      V_Mult(&(Gen->c),Gen->Period,&AxisDir);
      Gen->BeamType=HELICAL;
    }
    V_Sub(&(Gen->Loc),&(Gen->a),&(Gen->d));
  }

  /* Sequential examination of model surfaces. */

  Gen->ImpactT=HUGE_VAL;
  Gen->ImpactS=NULL;
  while (BL) {
    S=BL->S;
    while (S) {
      if (BeamHitSurface(S,&t)) {
	Gen->ImpactS=S;
	Gen->ImpactT=t;
	if (!Gen->ImpactT)
	  return TRUE;
      }
      S=S->Nxt;
    }
    BL=BL->Nxt;
  }
  return (Gen->ImpactS!=NULL);
}


/* ELECTRON BEAM LENGTH. */

/* Returns the length of the electron beam of the generator *Gen, in
   meters. */

double Beam_Length(Gen)
Generator *Gen;
{
  if (Gen->ImpactS)
    return Gen->v*Gen->ImpactT;
  else
    return HUGE_VAL;
}


/* ELECTRON POSITION. */

/* Calculates the location of the electrons at time t (in seconds)
   after emergence from the generator *Gen. The result is placed in *Pos.
   FALSE is returned iff (i) at time t the electron beam does not exist,
   i.e. the electrons have intercepted the model at an earlier time, or
   (ii) the beam is circular and at time t the electrons are retracing
   their path. */

BOOLEAN Beam_Point(Gen,t,Pos)
Generator *Gen;
double    t;
Cartesian *Pos;
{
  Cartesian Temp;

  if (t>Gen->ImpactT || (Gen->BeamType==CIRCULAR && t>=Gen->Period))
    return FALSE;

  if (Gen->BeamType==LINEAR) {
    V_Mult(&(Gen->a),t,&Temp);
    V_Add(&(Gen->Loc),&Temp,Pos);
  } else {
    V_Mult(&(Gen->a),cos(Gen->omega*t),&Temp);
    V_Add(&(Gen->d),&Temp,Pos);
    V_Mult(&(Gen->b),sin(Gen->omega*t),&Temp);
    V_Add(Pos,&Temp,Pos);
    if (Gen->BeamType==HELICAL) {
      V_Mult(&(Gen->c),t,&Temp);
      V_Add(Pos,&Temp,Pos);
    }
  }
  return TRUE;
}


/* ELECTRON VELOCITY. */

/* Calculates the velocity of the electrons at time t (in seconds)
   after emergence from the generator *Gen. The result is placed in *V. */

void Beam_Velocity(Gen,t,V)
Generator *Gen;
double    t;
Cartesian *V;
{
  Cartesian Temp;

  if (Gen->BeamType==LINEAR)
    *V=Gen->a;
  else {
    V_Mult(&(Gen->a),-Gen->omega*sin(Gen->omega*t),V);
    V_Mult(&(Gen->b),Gen->omega*cos(Gen->omega*t),&Temp);
    V_Add(V,&Temp,V);
    if (Gen->BeamType==HELICAL)
      V_Add(&(Gen->c),V,V);
  }
  return;
}


/* ELECTRON TRACING. */

/* Returns a sequence of points along the electron beam of the
   generator *NewGen. Object generation stops when the beam length
   exceeds NewLenRot (if NewMode is LENGTH) or when the number of beam
   rotations exceeds NewLenRot (if NewMode is ROTATIONS), or when the
   intersection point of the beam with a surface (if any) is
   reached. The beam points are obtained by successive calls to the
   Beam_Point() function: the parameters NewGen, NewMode, and
   NewLenRot need only be specified once, passing TRUE in
   NewSequence. The function can then be called repetitively with
   NewSequence being FALSE: *NextPoint will contain the successive
   beam points. The function returns FALSE only when a point is
   requested beyond the end of the sequence.  NOTE: the ROTATIONS mode
   is ignored if the beam is linear - a default section of the beam
   (LINEAR_LENGTH m) is drawn. */

BOOLEAN Beam_Trace(NewMode,NewLenRot,NewGen,NextPoint,NewSequence)
BEAM_MODE  NewMode;
double     NewLenRot;
Generator  *NewGen;
Cartesian  *NextPoint;
BOOLEAN    NewSequence;
{
  static BOOLEAN   Done;
  static double    t,dt,LenRot,Limit;
  static BEAM_MODE Mode;
  static Generator *Gen;

  BOOLEAN ReturnValue;

  if (NewSequence) {
    Gen=NewGen;
    Mode=NewMode;
    LenRot=NewLenRot;
    dt=SEGMENT_LENGTH/Gen->v;
    t=0.0;
    Done=FALSE;
    if (Mode==LENGTH)
      Limit=LenRot/Gen->v;
    else if (Gen->BeamType==LINEAR)
      Limit=LINEAR_LENGTH/Gen->v;
    else
      Limit=Gen->Period*LenRot;
  } else if (Done)
    return FALSE;

  /* Move along the beam and approximate beam by line segments. */

  if (!Beam_Point(Gen,t,NextPoint))
    if (Gen->ImpactS) {
      Beam_Point(Gen,Gen->ImpactT,NextPoint); /* Return intersection point. */
      Done=TRUE;
      return TRUE;
    } else
      return FALSE;

  ReturnValue=(Gen->ImpactS || (t<=Limit));
  t+=dt;
  return ReturnValue;
}


/* CONTAINING CYLINDER FOR HELICAL BEAM. */

/* Returns in *P the coordinates of a point on the projection of the
   electron beam of the generator *Gen on the local (beam construction)
   axes. This point corresponds to time k/Gen->omega since the beam
   emerged from the generator. */

static void CircumPoint(Gen,k,P)
Generator *Gen;
double    k;
Cartesian *P;
{
  Cartesian Temp;

  V_Mult(&(Gen->a),cos(k),&Temp);
  V_Add(&(Gen->d),&Temp,P);
  V_Mult(&(Gen->b),sin(k),&Temp);
  V_Add(P,&Temp,P);
  return;
}

/* Returns a cylindrical body (approximated by a polygonal mesh of
   Sides sides) of length (height) Length, in meters, which contains
   tightly the (helical) electron beam of the generator *Gen. The
   cylinder is also extended Offset meters behind the beam. The new
   vertex information is stored in the vertex space *VS. The new body
   is stored in the body list *BL, and is a pointer to it is
   returned. NULL may also be returned, iff an error occurs. */

Body *Beam_Cylinder(Gen,Sides,Offset,Length,BL,VS)
Generator *Gen;
int       Sides;
double    Offset;
double    Length;
Body      **BL;
Vertex    **VS;
{
  Cartesian VLength,VOffset,P[4];
  Body      *B;
  Surface   *S;
  Vertex    *V;
  int       i,j;
  double    k,dk;

  if (Gen->BeamType!=HELICAL) {
    fprintf(stderr,"Warning: cannot create cylinder for non-helical beam.\n");
    return NULL;
  }
  if (!(B=Bd_Create(BL)))
    return NULL;

  /* Set vector in helical beam travelling axis and length Length. */

  V_Unit(&(Gen->c),&VLength);
  VOffset=VLength;
  V_Mult(&VLength,Length,&VLength);
  V_Mult(&VOffset,Offset,&VOffset);

  /* Calculate first surface side. */

  k=0.0;
  dk=TWO_PI/Sides;
  CircumPoint(Gen,k,&P[0]);
  V_Add(&P[0],&VOffset,&P[0]);
  V_Add(&P[0],&VLength,&P[1]);
  for (i=0;i<Sides;i++) {

    /* Calculate next side. */

    k+=dk;
    CircumPoint(Gen,k,&P[3]);
    V_Add(&P[3],&VOffset,&P[3]);
    V_Add(&P[3],&VLength,&P[2]);

    /* Create new surface. */

    if (!(S=Su_Create(&B->S)))
      goto Error_Beam_Cylinder;
    for (j=0;j<4;j++) {
      if (!(V=VS_Add(VS,&P[j])))
	goto Error_Beam_Cylinder;
      if (!Su_AddVertex(S,V))
	goto Error_Beam_Cylinder;
    }
    Su_SetPlanes(S);

    /* Turn second side into first one. */

    P[0]=P[3];
    P[1]=P[2];
  }
  return B;

  /* Error recovery: removing bad data from lists. */

 Error_Beam_Cylinder:
  (*BL)=B->Nxt;
  Bd_Free(B);
  VS_Cleanup(VS);
  return NULL;
}


/* SECONDARY BEAM EMISSIONS. */

/* Identifies secondary beam emissions from the generator *NewGen.
   Returns TRUE iff the primary or the secondary beam intersects the body
   *BTarget. The primary beam has a spread (half-angle) of Spread
   radians, while the secondary beam has SecSpread spread. The dSpread
   and dSecSpread parameters set the step between successive samples of
   the emitted beams; each spread parameter must lie in [0,HF_PI] and the
   spread steps much be positive. The energy (potential) of the secondary
   emissions should be set in SecGenOut->Potential; this must be greater
   than 0 Volts. *B is the (presumably constant) magnetic field intensity
   in the volume of space traversed by the emitted electrons (in Tesla).
   The body list BL contains other bodies which are present in the model
   and might intercept the emissions before the target is reached; this
   list may contain the target body, although this is not recommended.
   Upon return, GenOut->ImpactS is a pointer to the intersected surface
   of the primary emissions (NULL if there is no hit), and
   GenOut->ImpactT is the time of intersection, in seconds. Similarly for
   SecGenOut and secondary beam emissions (provided the primary beam
   misses the target; otherwise, SecGenOut->ImpactS is NULL). The other
   fields of the returned generators are also properly set to the
   intersecting beam (set) parameters. */

BOOLEAN Beam_SecondaryHitBody(B,NewGen,BL,BTarget,
			      Spread,dSpread,SecSpread,dSecSpread,
			      GenOut,SecGenOut)
Cartesian *B;
Generator *NewGen;
Body      *BL;
Body      *BTarget;
double    Spread;
double    dSpread;
double    SecSpread;
double    dSecSpread;
Generator *GenOut;
Generator *SecGenOut;
{
  Body           Target;
  Spherical      Sph;
  Cartesian      e;
  Surface        *ImpactS;
  double         Dot,ImpactT,Dummy;
  SphSampleParam PrimSP,SecSP;

  Target=(*BTarget); /* Turn target body into one-element list. */
  Target.Nxt=NULL;

  GenOut->Loc=NewGen->Loc;             /* Primary generator same as first. */
  GenOut->Potential=NewGen->Potential;
  Sph.R=1.0;
  Sph.Ph=NewGen->Coelevation;
  Sph.Th=NewGen->Azimuth;
  S_ToCartesian(&Sph,&PrimSP.E);

  /* Set spreading parameters. */

  PrimSP.Spread=Spread;
  PrimSP.dSpread=dSpread;
  SecSP.Spread=SecSpread;
  SecSP.dSpread=dSecSpread;

  /* Cover spread of primary emissions. */

  if (S_NextSample(TRUE,&PrimSP,&e))
    do {
      S_FromCartesian(&e,&Sph);
      GenOut->Coelevation=Sph.Ph;
      GenOut->Azimuth=Sph.Th;

      Beam_HitBodyList(B,GenOut,&Target);
      ImpactT=GenOut->ImpactT;            /* Retain target intersection. */
      ImpactS=GenOut->ImpactS;

      if (Beam_HitBodyList(B,GenOut,BL) || ImpactS) {

	/* Primary emission hit. */

	if (!GenOut->ImpactS || GenOut->ImpactT>=ImpactT) {
	  GenOut->ImpactT=ImpactT;
	  GenOut->ImpactS=ImpactS;
	  SecGenOut->ImpactS=NULL;
	  return TRUE;
	}

	/* Source and direction of secondary emissions. */

	Beam_Point(GenOut,GenOut->ImpactT,&(SecGenOut->Loc));
	Beam_Velocity(GenOut,GenOut->ImpactT,&SecSP.E);
	if (V_DotIsZero(&SecSP.E,&(GenOut->ImpactS->P.u),&Dot))
	  V_Mult(&SecSP.E,-1.0,&SecSP.E);
	else {
	  SecSP.E=GenOut->ImpactS->P.u;
	  if (Dot>0.0)
	    V_Mult(&SecSP.E,-1.0,&SecSP.E);
	}

	/* Moving source away from intersected surface by a small distance to
	   avoid intersection reporting on emergence. */

	V_Mult(&SecSP.E,2.0*LENGTH_ACCURACY/V_Mag(&SecSP.E),&e);
	V_Add(&(SecGenOut->Loc),&e,&(SecGenOut->Loc));

	/* Cover spread of secondary emissions. */

	if (S_NextSample(TRUE,&SecSP,&e))
	  do {
	    S_FromCartesian(&e,&Sph);
	    SecGenOut->Coelevation=Sph.Ph;
	    SecGenOut->Azimuth=Sph.Th;

	    /* First, we check to see if the target is hit
	       (since it is a small target in most cases, this test is
	       fast). Then we check for prior intersection with the originating
	       surface (since tight helical beams are quickly curved).
	       Finally, we check the rest of the model, looking for an
	       intersection prior to the one with the target. */

	    if (Beam_HitBodyList(B,SecGenOut,&Target)) {
	      ImpactT=SecGenOut->ImpactT;     /* Retain target intersection. */
	      ImpactS=SecGenOut->ImpactS;
	      if (!BeamHitSurface(GenOut->ImpactS,&Dummy) &&
		  (!Beam_HitBodyList(B,SecGenOut,BL) ||
		   SecGenOut->ImpactT>=ImpactT)) {
		SecGenOut->ImpactT=ImpactT;
		SecGenOut->ImpactS=ImpactS;
		return TRUE;
	      }
	    }
	  } while (S_NextSample(FALSE,&SecSP,&e));
      }
    } while (S_NextSample(FALSE,&PrimSP,&e));

  GenOut->ImpactS=NULL;
  return FALSE;
}

/* A utility function, calculating useful information of
   primary/secondary beam emission pairs. In particular, the function
   first assumes that Gen, GenOut, and SecGenOut are the NewGen, GenOut,
   SecGenOut parameters of Beam_SecondaryHitBody(), respectively;
   moreover, Beam_SecondaryHitBody() must have been called before
   Beam_SecondaryUtil(). Then, this function places in *Spread and
   *Incidence the angular deviation of the primary beam from its main
   direction and the angle between the primary beam and the intersected
   surface at intersection. If secondary emissions were necessary to hit
   the target, *SecSpreadV is assigned the angular deviation of the
   secondary beam from the vector normal to the secondary beam emission
   originating surface, *SecSpreadR is the angular deviation of the
   secondary beam from the reflected primary beam direction, and
   *SecIncidence is the angle between the secondary beam and the
   intersected target surface at intersection. */

void Beam_SecondaryUtil(Gen,GenOut,SecGenOut,
			Spread,Incidence,SecSpreadV,SecSpreadR,SecIncidence)
Generator *Gen;
Generator *GenOut;
Generator *SecGenOut;
double    *Spread;
double    *SecSpreadV;
double    *SecSpreadR;
double    *Incidence;
double    *SecIncidence;
{
   Spherical S1,S2;
  Cartesian C1,C2,HitDir,SecDir;

  S1.R=S2.R=1.0;

  /* Spread of primary emission. */

  S1.Ph=GenOut->Coelevation;
  S1.Th=GenOut->Azimuth;
  S2.Ph=Gen->Coelevation;
  S2.Th=Gen->Azimuth;
  S_ToCartesian(&S1,&C1);
  S_ToCartesian(&S2,&C2);
  V_Angle(&C1,&C2,Spread);

  /* Incidence of primary emission. */

  Beam_Velocity(GenOut,GenOut->ImpactT,&HitDir);
  V_Angle(&HitDir,&(GenOut->ImpactS->P.u),Incidence);
  if (*Incidence>HF_PI)
    *Incidence=PI-*Incidence;

  /* Ignore secondary emission parameters if there is none. */

  if (!SecGenOut->ImpactS)
    return;

  /* Vertical spread of secondary emission. */

  S1.Ph=SecGenOut->Coelevation;
  S1.Th=SecGenOut->Azimuth;
  S_ToCartesian(&S1,&SecDir);
  V_Angle(&SecDir,&(GenOut->ImpactS->P.u),SecSpreadV);
  if (*SecSpreadV>HF_PI)
    *SecSpreadV=PI-*SecSpreadV;

  /* Reflected spread of secondary emission. */

  V_Mult(&(GenOut->ImpactS->P.u),
	 2.0*V_Dot(&HitDir,&(GenOut->ImpactS->P.u)),
	 &C2);
  V_Sub(&C2,&HitDir,&C2);
  V_Angle(&SecDir,&C2,SecSpreadR);
  if (*SecSpreadR>HF_PI)
    *SecSpreadR=PI-*SecSpreadR;

  /* Incidence of secondary emission. */

  Beam_Velocity(SecGenOut,SecGenOut->ImpactT,&C1);
  V_Angle(&C1,&(SecGenOut->ImpactS->P.u),SecIncidence);
  if (*SecIncidence>HF_PI)
    *SecIncidence=PI-*SecIncidence;

  return;
}


/* BEAM BACKTRACING. */

/* Identifies the possible sources of secondary beam emissions. The
   location and orientation of the electron detector, and the energy of
   the detected electrons should be specified in *Det (fields Loc,
   Coelevation, Azimuth, Potential). Spread specifies the angular range
   of the detector in radians (i.e. what are the possible angles
   [relative to the detector's orientation] at which the electrons can
   hit the detector).  The dSpread parameter sets the step between
   successive samples of the incoming beams; the Spread parameter must
   lie in [0,HF_PI] and the spread step much be positive. *B is the
   (presumably constant) magnetic field intensity in the volume of space
   traversed by the electrons (in Tesla). The body list BL contains all
   the bodies which are present in the model and which are possible
   sources of secondary emissions.
   Upon return, the GenOut array (of size NGen) is filled with the
   electron beams which hit the detector: ImpactS is a pointer to a
   surface which might have generated the detected electrons. Also,
   Coelevation and Azimuth contain the direction of the detected
   electrons which orginated from the ImpactS surface. If multiple beam
   emissions from the same surface reach the detector, only one sample
   emission is recorded in the GenOut array, while the corresponding
   entry (same array index) in NHits contains the number of sample
   emissions originating from the same surface. If more than NGen
   surfaces might have been possible originators, then the first NGen
   discovered will be listed in GenOut. The function's return value
   indicates the number of valid entries in those two output arrays. */

int Beam_Sources(B,Det,BL,Spread,dSpread,GenOut,NHits,NGen)
Cartesian *B;
Generator *Det;
Body      *BL;
double    Spread;
double    dSpread;
Generator GenOut[];
int       NHits[];
int       NGen;
{
  Spherical      Sph;
  Cartesian      RevB,e;
  Generator      Gen;
  int            i,NextGen;
  void           *Save;
  SphSampleParam SP;

  NextGen=0;                /* No hits found yet. */
  V_Mult(B,-1.0,&RevB);     /* Reverse magnetic field intensity. */
  Gen=(*Det);               /* Initialize sample back-generator. */
  Sph.R=1.0;                /* Initiate covering of electron source. */
  Sph.Ph=Gen.Coelevation;
  Sph.Th=Gen.Azimuth;
  S_ToCartesian(&Sph,&SP.E);
  SP.Spread=Spread;
  SP.dSpread=dSpread;

  /* Back-trace electrons. */

  if (S_NextSample(TRUE,&SP,&e))
    do {
      S_FromCartesian(&e,&Sph);
      Gen.Coelevation=Sph.Ph;
      Gen.Azimuth=Sph.Th;
      if (Beam_HitBodyList(&RevB,&Gen,BL)) {
	for (i=0;i<NextGen;i++)
	  if (GenOut[i].ImpactS==Gen.ImpactS) { /* Increase hit count. */
	    NHits[i]++;
	    break;
	  }
	if (i==NextGen && NextGen<NGen) {       /* Create new entry. */
	  NextGen++;
	  Save=GenOut[i].UserData;              /* Retain extension, if any. */
	  GenOut[i]=Gen;
	  GenOut[i].UserData=Save;
	  NHits[i]=1;
	}
      }
    } while (S_NextSample(FALSE,&SP,&e));

  return NextGen;
}
