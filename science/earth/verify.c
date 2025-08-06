/* EARTH MODELLING ROUTINES' TESTER.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>
#include "advmath.h"

#include "earth.h"


BOOLEAN EQ(X,Y)
double X;
double Y;
{
  return (fabs(X-Y)<1E-10);
}


BOOLEAN EQ_AP(X,Y)
double X;
double Y;
{
  return (fabs(X-Y)<1E-5);
}


BOOLEAN M_EQ(M,A00,A01,A02,A10,A11,A12,A20,A21,A22)
Matrix M;
double A00,A01,A02,A10,A11,A12,A20,A21,A22;
{
  return (EQ(M[0][0],A00) && EQ(M[0][1],A01) && EQ(M[0][2],A02) && 
	  EQ(M[1][0],A10) && EQ(M[1][1],A11) && EQ(M[1][2],A12) && 
	  EQ(M[2][0],A20) && EQ(M[2][1],A21) && EQ(M[2][2],A22));
}


BOOLEAN M_EQ_AP(M1,M2)
Matrix M1;
Matrix M2;
{
  return (EQ_AP(M1[0][0],M2[0][0]) && EQ_AP(M1[0][1],M2[0][1]) &&
	  EQ_AP(M1[0][2],M2[0][2]) && 
	  EQ_AP(M1[1][0],M2[1][0]) && EQ_AP(M1[1][1],M2[1][1]) &&
	  EQ_AP(M1[1][2],M2[1][2]) && 
	  EQ_AP(M1[2][0],M2[2][0]) && EQ_AP(M1[2][1],M2[2][1]) &&
	  EQ_AP(M1[2][2],M2[2][2]));
}


void TestCoord()
{
  Earth     E;
  Spherical S;
  Frame     F;
  Cartesian C,D1,D2;
  Matrix    M1,M2;

  E.Long=DTR(120.0); E.Lat=DTR(90.0); E.Alt=10.0;
  E_ToSpherical(&E,&S);
  if (!EQ(S.R,EARTH_B+10.0) || !EQ(S.Ph,0.0) || !EQ(S.Th,DTR(120.0))) {
    printf("Coord Error (to spherical 1).\n");
    return;
  }
  E_FromSpherical(&S,&E);
  if (!EQ(E.Long,DTR(120.0)) || !EQ(E.Lat,DTR(90.0)) || !EQ(E.Alt,10.0)) {
    printf("Coord Error (from spherical 1).\n");
    return;
  }

  E.Long=DTR(120.0); E.Lat=DTR(0.0); E.Alt=(-10.0);
  E_ToSpherical(&E,&S);
  if (!EQ(S.R,EARTH_A-10.0) || !EQ(S.Ph,HF_PI) || !EQ(S.Th,DTR(120.0))) {
    printf("Coord Error (to spherical 2).\n");
    return;
  }
  E_FromSpherical(&S,&E);
  if (!EQ(E.Long,DTR(120.0)) || !EQ(E.Lat,DTR(0.0)) || !EQ(E.Alt,-10.0)) {
    printf("Coord Error (from spherical 2).\n");
    return;
  }

  E.Long=DTR(120.0); E.Lat=DTR(90.0); E.Alt=(-EARTH_B);
  E_ToSpherical(&E,&S);
  if (!EQ(S.R,0.0) || !EQ(S.Ph,0.0) || !EQ(S.Th,DTR(120.0))) {
    printf("Coord Error (to spherical 3).\n");
    return;
  }
  E_FromSpherical(&S,&E);
  if (!EQ(E.Long,DTR(120.0)) || !EQ(E.Lat,DTR(90.0)) || !EQ(E.Alt,-EARTH_B)) {
    printf("Coord Error (from spherical 3).\n");
    return;
  }

  E.Long=DTR(120.0); E.Lat=DTR(45.0); E.Alt=10000.0;
  E_ToSpherical(&E,&S);
  E_FromSpherical(&S,&E);
  if (!EQ(E.Long,DTR(120.0)) || !EQ(E.Lat,DTR(45.0)) ||
      !EQ_AP(E.Alt,10000.0)) {
    printf("Coord Error (from spherical 4).\n");
    return;
  }

  E.Long=DTR(18.139454); E.Lat=DTR(-7.164981); E.Alt=425831.635;
  E_ToSpherical(&E,&S);
  if (!EQ_AP(S.R,6803661.5500911) ||
      !EQ(S.Ph,DTR(90.0+7.1204795568)) ||
      !EQ(S.Th,DTR(18.139454))) {
    printf("Coord Error (to spherical 5).\n");
    return;
  }
  E_FromSpherical(&S,&E);
  if (!EQ(E.Long,DTR(18.139454)) ||
      !EQ(E.Lat,DTR(-7.164981)) ||
      !EQ_AP(E.Alt,425831.635)) {
    printf("Coord Error (from spherical 5).\n");
    return;
  }

  E.Long=DTR(70.0); E.Lat=DTR(40.0); E.Alt=10000.0;
  E_LocalFrame(TRUE,&E,NULL,&F);
  if (!EQ(F.I.X,cos(DTR(160.0))) || !EQ(F.I.Y,sin(DTR(160.0))) ||
      !EQ(F.I.Z,0.0) ||
      !EQ(F.J.X,sin(DTR(40.0))*cos(DTR(250.0))) ||
      !EQ(F.J.Y,sin(DTR(40.0))*sin(DTR(250.0))) ||
      !EQ(F.J.Z,cos(DTR(40.0))) ||
      !EQ(F.K.X,sin(DTR(50.0))*cos(DTR(70.0))) ||
      !EQ(F.K.Y,sin(DTR(50.0))*sin(DTR(70.0))) ||
      !EQ(F.K.Z,cos(DTR(50.0)))) {
    printf("Coord Error (local frame earth).\n");
    return;
  }

  S.Th=DTR(70.0); S.Ph=DTR(50.0); S.R=EARTH_A;
  E_LocalFrame(FALSE,NULL,&S,&F);
  if (!EQ(F.I.X,cos(DTR(160.0))) || !EQ(F.I.Y,sin(DTR(160.0))) ||
      !EQ(F.I.Z,0.0) ||
      !EQ(F.J.X,sin(DTR(40.0))*cos(DTR(250.0))) ||
      !EQ(F.J.Y,sin(DTR(40.0))*sin(DTR(250.0))) ||
      !EQ(F.J.Z,cos(DTR(40.0))) ||
      !EQ(F.K.X,sin(DTR(50.0))*cos(DTR(70.0))) ||
      !EQ(F.K.Y,sin(DTR(50.0))*sin(DTR(70.0))) ||
      !EQ(F.K.Z,cos(DTR(50.0)))) {
    printf("Coord Error (local frame spherical).\n");
    return;
  }

  E.Long=DTR(70.0); E.Lat=DTR(40.0); E.Alt=10000.0;
  E_MFromLocalGeod(&E,M1);
  E_LocalFrame(TRUE,&E,NULL,&F);
  M_FromGlobal(&F,M2);
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("Coord Error (from local geod).\n");
    return;
  }

  E.Long=DTR(70.0); E.Lat=DTR(40.0); E.Alt=10000.0;
  E_MToLocalGeod(&E,M1);
  E_LocalFrame(TRUE,&E,NULL,&F);
  M_ToGlobal(&F,M2);
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("Coord Error (to local geod).\n");
    return;
  }

  S.Ph=DTR(70.0); S.Th=DTR(40.0); S.R=10000.0;
  E_MFromLocalGeoc(&S,M1);
  E_LocalFrame(FALSE,NULL,&S,&F);
  M_FromGlobal(&F,M2);
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("Coord Error (from local geoc).\n");
    return;
  }

  S.Ph=DTR(70.0); S.Th=DTR(40.0); S.R=10000.0;
  E_MToLocalGeoc(&S,M1);
  E_LocalFrame(FALSE,NULL,&S,&F);
  M_ToGlobal(&F,M2);
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("Coord Error (to local geoc).\n");
    return;
  }

  C.X=C.Y=C.Z=1.0;
  E.Long=DTR(70.0); E.Lat=DTR(40.0); E.Alt=10000.0;
  E_ToSpherical(&E,&S);
  E_LocalGeocToGeod(&E,&S,&C,&D1);
  E_MFromLocalGeoc(&S,M1);
  E_MToLocalGeod(&E,M2);
  M_MMult(M2,M1,M2);
  M_VMult(M2,&C,&D2);
  if (!EQ(D1.X,D2.X) || !EQ(D1.Y,D2.Y) || !EQ(D1.Z,D2.Z)) {
    printf("Coord Error (local geoc to geod).\n");
    return;
  }

  E_LocalGeodToGeoc(&E,&S,&D1,&D2);
  if (!EQ(D2.X,1.0) || !EQ(D2.Y,1.0) || !EQ(D2.Z,1.0)) {
    printf("Coord Error (local geod to geoc).\n");
    return;
  }

  printf("Coord passed.\n");
  return;
}


RETCODE TestOneBField(Year,Long,Lat,Alt,BN,BE,BD)
double Year;
double Long;
double Lat;
double Alt;
double BN;
double BE;
double BD;
{
  Earth     E;
  Spherical S;
  Cartesian B;

  IGRF_SetYear(Year);
  E.Long=Long; E.Lat=Lat; E.Alt=Alt;
  E_ToSpherical(&E,&S);
  IGRF_GetBField(&S,&B);
  E_LocalGeocToGeod(&E,&S,&B,&B);
  B.X=TESLA_TO_GAUSS(B.X);
  B.Y=TESLA_TO_GAUSS(B.Y);
  B.Z=TESLA_TO_GAUSS(B.Z);
  if (!EQ_AP(B.X,BE) || !EQ_AP(B.Y,BN) || !EQ_AP(B.Z,-BD)) {
    printf("BField Error (GetBFIeld %le %le %le %le; assumed IGRF 1985).\n",
	   Year,RTD(Long),RTD(Lat),Alt/1000.0);
    return FAILURE;
  }
  return SUCCESS;
}

void TestBField()
{
  Earth     E1,E2;
  Spherical S;
  Cartesian T;

  if (!TestOneBField(1985.0,DTR(0.0),DTR(-90.0),0.0*1000.0,
		     0.14378,-0.07466,-0.54542))
    return;
  if (!TestOneBField(1985.0,DTR(0.0),DTR(90.0),0.0*1000.0,
		     0.01886,-0.01253,0.56557))
    return;
  if (!TestOneBField(1985.0,DTR(0.0),DTR(50.0),0.0*1000.0,
		     0.19895,-0.01695,0.43169))
    return;
  if (!TestOneBField(1985.0,DTR(-75.5),DTR(37.8),300.0*1000.0,
		     0.17755,-0.02948,0.43024))
    return;
  if (!TestOneBField(1987.0,DTR(0.0),DTR(0.0),0.0*1000.0,
		     0.27525,-0.04303,-0.14150))
    return;

  IGRF_SetYear(1985.0);

  E1.Long=0.0; E1.Lat=0.0; E1.Alt=80000.0;
  if (IGRF_Trace(&E1,90000.0,WHILE_ABOVE,100.0,&T)) {
    printf("BField Error (Trace 80000; assumed IGRF 1985).\n");
    return;
  }

  E1.Long=DTR(120.0); E1.Lat=DTR(-45.0); E1.Alt=90000.0;
  if (!IGRF_Trace(&E1,90000.0,WHILE_ABOVE,100.0,&T)) {
    printf("BField Error (Trace 90000 1; assumed IGRF 1985).\n");
    return;
  }
  S_FromCartesian(&T,&S);
  E_FromSpherical(&S,&E2);
  if (!EQ(E1.Long,E2.Long) || !EQ(E1.Lat,E2.Lat) || !EQ_AP(E1.Alt,E2.Alt)) {
    printf("BField Error (Trace 90000 2; assumed IGRF 1985).\n");
    return;
  }

  E1.Long=DTR(120.0); E1.Lat=DTR(-45.0); E1.Alt=300000.0;
  if (!IGRF_Trace(&E1,90000.0,WHILE_ABOVE,-500.0,&T)) {
    printf("BField Error (Trace 300000 1; assumed IGRF 1985).\n");
    return;
  }
  S_FromCartesian(&T,&S);
  E_FromSpherical(&S,&E2);
  if (!EQ_AP(E2.Alt,89999.99979) ||
      !EQ_AP(E2.Long,DTR(120.09446)) ||
      !EQ_AP(E2.Lat,DTR(-45.410202)) ||
      !IGRF_Trace(&E2,300000.0,WHILE_BELOW,500.0,&T)) {
    printf("BField Error (Trace 300000 2; assumed IGRF 1985).\n");
    return;
  }
  S_FromCartesian(&T,&S);
  E_FromSpherical(&S,&E2);
  if (fabs(E1.Long-E2.Long)>=1E-4 ||
      fabs(E1.Lat-E2.Lat)>=1E-4 ||
      fabs(E1.Alt-E2.Alt)>=1E-2) {
    printf("BField Error (Trace 300000 3; assumed IGRF 1985).\n");
    return;
  }

  printf("BField passed (assumed IGRF 1985).\n");
  return;
}


void TestDays()
{
  if (!EQ(DAY_FromYMDS((unsigned int)1901,(unsigned int)1,(unsigned int)1,
		       (unsigned long)3600),365.0+1.0/24.0)) {
    printf("Days Error (1/1/1901).\n");
    return;
  }

  if (!EQ(DAY_FromYMDS((unsigned int)1903,(unsigned int)12,(unsigned int)31,
		       (unsigned long)86400),365.0*4.0)) {
    printf("Days Error (12/31/1903).\n");
    return;
  }

  if (!EQ(DAY_FromYMDS((unsigned int)1904,(unsigned int)3,(unsigned int)1,
		       (unsigned long)82800),365.0*4.0+31.0+29.0+23.0/24.0)) {
    printf("Days Error (3/1/1904).\n");
    return;
  }

  if (!EQ(DAY_FromYMDS((unsigned int)2099,(unsigned int)12,(unsigned int)31,
		       (unsigned long)86400),365.0*200.0+49.0)) {
    printf("Days Error (12/31/2099).\n");
    return;
  }

  printf("Days passed.\n");
  return;
}


void TestLVLHLoc()
{
  Cartesian R,V,A,D;
  double    Angle;
  Matrix    M1,M2;

  R.X=R.Y=R.Z=1000.0;
  V.X=V.Y=(-10.0); V.Z=10.0;
  if (!E_AngleLocalLVLH(&R,&V,&Angle) || !EQ(Angle,DTR(90.0))) {
    printf("LVLHLoc Error (Angle 90.0).\n");
    return;
  }
  V.X=V.Y=10.0; V.Z=(-10.0);
  if (!E_AngleLocalLVLH(&R,&V,&Angle) || !EQ(Angle,DTR(-90.0))) {
    printf("LVLHLoc Error (Angle -90.0).\n");
    return;
  }
  V.X=(-10.0); V.Y=10.0; V.Z=0.0;
  if (!E_AngleLocalLVLH(&R,&V,&Angle) || !EQ(Angle,0.0)) {
    printf("LVLHLoc Error (Angle 0.0).\n");
    return;
  }
  V.X=V.Y=V.Z=10.0;
  if (E_AngleLocalLVLH(&R,&V,&Angle)) {
    printf("LVLHLoc Error (Angle error).\n");
    return;
  }

  R.X=R.Y=R.Z=1000.0;
  V.X=V.Y=(-10.0); V.Z=10.0;
  if (!E_MLocalToLVLH(&R,&V,M1)) {
    printf("LVLHLoc Error (MLocalLVLH 1).\n");
    return;
  }
  M_zRotation(DTR(-90.0),M2);
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("LVLHLoc Error (MLocalLVLH 2).\n");
    return;
  }

  if (!E_MLVLHToLocal(&R,&V,M1)) {
    printf("LVLHLoc Error (MLVLHLocal 1).\n");
    return;
  }
  M_zRotation(DTR(90.0),M2);
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("LVLHLoc Error (MLVLHLocal 2).\n");
    return;
  }

  R.X=R.Y=R.Z=1000.0;
  V.X=V.Y=(-10.0); V.Z=10.0;
  A.X=A.Y=A.Z=1.0;
  E_MLocalToLVLH(&R,&V,M1);
  M_VMult(M1,&A,&D);
  if (!E_LocalToLVLH(&R,&V,&A,&A) ||
      !EQ(D.X,A.X) || !EQ(D.Y,A.Y) || !EQ(D.Z,A.Z)) {
    printf("LVLHLoc Error (LocalLVLH).\n");
    return;
  }

  A.X=A.Y=A.Z=1.0;
  E_MLVLHToLocal(&R,&V,M1);
  M_VMult(M1,&A,&D);
  if (!E_LVLHToLocal(&R,&V,&A,&A) ||
      !EQ(D.X,A.X) || !EQ(D.Y,A.Y) || !EQ(D.Z,A.Z)) {
    printf("LVLHLoc Error (LVLHLocal).\n");
    return;
  }

  printf("LVLHLoc passed.\n");
  return;
}


void TestLVLHECI()
{
  Cartesian R,V,A,D;
  Frame     F;
  Matrix    M1,M2;

  R.X=R.Y=R.Z=1000.0;
  V.X=V.Y=(-10.0); V.Z=10.0;
  if (!E_LVLHFrame(&R,&V,&F)) {
    printf("LVLHECI Error (LVLHFrame 1).\n");
    return;
  }
  V_Cross(&F.J,&F.K,&A);
  V_Unit(&A,&D);
  if (!EQ(F.K.X,1.0/sqrt(3.0)) || !EQ(F.K.Y,1.0/sqrt(3.0)) ||
      !EQ(F.K.Z,1.0/sqrt(3.0)) ||
      !EQ(F.J.X,1.0/sqrt(2.0)) || !EQ(F.J.Y,-1.0/sqrt(2.0)) ||
      !EQ(F.J.Z,0.0) ||
      !EQ(F.I.X,D.X) || !EQ(F.I.Y,D.Y) || !EQ(F.I.Z,D.Z)) {
    printf("LVLHECI Error (LVLHFrame 2).\n");
    return;
  }

  if (!E_MECIToLVLH(&R,&V,M1)) {
    printf("LVLHECI Error (MECILVLH 1).\n");
    return;
  }
  M_ToGlobal(&F,M2);
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("LVLHECI Error (MECILVLH 2).\n");
    return;
  }

  if (!E_MLVLHToECI(&R,&V,M1)) { 
    printf("LVLHECI Error (MLVLHECI 1).\n");
    return;
  }
  M_FromGlobal(&F,M2);
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("LVLHECI Error (MLVLHECI 2).\n");
    return;
  }

  A.X=A.Y=A.Z=1.0;
  E_MECIToLVLH(&R,&V,M1);
  M_VMult(M1,&A,&D);
  if (!E_ECIToLVLH(&R,&V,&A,&A) ||
      !EQ(D.X,A.X) || !EQ(D.Y,A.Y) || !EQ(D.Z,A.Z)) {
    printf("LVLHECI Error (ECILVLH).\n");
    return;
  }

  A.X=A.Y=A.Z=1.0;
  E_MLVLHToECI(&R,&V,M1);
  M_VMult(M1,&A,&D);
  if (!E_LVLHToECI(&R,&V,&A,&A) ||
      !EQ(D.X,A.X) || !EQ(D.Y,A.Y) || !EQ(D.Z,A.Z)) {
    printf("LVLHECI Error (LVLHECI).\n");
    return;
  }

  printf("LVLHECI passed.\n");
  return;
}


void TestECIECR()
{
  Cartesian A,D;
  Spherical S;
  Matrix    M1,M2;
  double    Time;

  Time=DAY_FromYMDS((unsigned int)1992,(unsigned int)7,(unsigned int)3,
		    (unsigned long)0);
  Time+=(60.0+59.997528)/(3600.0*24.0)+1.0;
  E_MECIToECR(Time,M1);
  A.X=3365979.5; A.Y=(-5852242.0); A.Z=(-843355.875);
  M_VMult(M1,&A,&D);
  S_FromCartesian(&D,&S);
  if (!EQ_AP(S.R,6803661.04041) ||
      !EQ_AP(RTD(S.Ph),90.0+7.120481) ||
      !EQ_AP(RTD(S.Th),18.139454)) {
    printf("ECIECR Error (MECIECR).\n");
    return;
  }

  E_MECRToECI(Time,M2); 
  M_MMult(M1,M2,M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("ECIECR Error (MECRECI).\n");
    return;
  }

  E_ECIToECR(Time,&A,&D);
  S_FromCartesian(&D,&S);
  if (!EQ_AP(S.R,6803661.04041) ||
      !EQ_AP(RTD(S.Ph),90.0+7.120481) ||
      !EQ_AP(RTD(S.Th),18.139454)) {
    printf("ECIECR Error (ECIECR).\n");
    return;
  }

  E_ECRToECI(Time,&D,&A);
  if (!EQ(A.X,3365979.5) || !EQ(A.Y,-5852242.0) ||
      !EQ(A.Z,-843355.875)) {
    printf("ECIECR Error (ECRECI).\n");
    return;
  }

  printf("ECIECR passed.\n");
  return;
}


void TestECIEarth()
{
  Cartesian A,B;
  Earth     E1,E2;
  Spherical S;
  double    Time;

  Time=DAY_FromYMDS((unsigned int)1992,(unsigned int)7,(unsigned int)3,
		    (unsigned long)0);
  Time+=(60.0+59.997528)/(3600.0*24.0)+1.0;

  A.X=A.Y=A.Z=1000000.0;
  E_FromECI(Time,&A,&E1);
  E_ECIToECR(Time,&A,&B);
  S_FromCartesian(&B,&S);
  E_FromSpherical(&S,&E2);  
  if (!EQ(E1.Long,E2.Long) || !EQ(E1.Lat,E2.Lat) || !EQ(E1.Alt,E2.Alt)) {
    printf("ECIEarth Error (FromECI).\n");
    return;
  }

  E1.Long=HF_PI; E1.Lat=0.0; E1.Alt=1000.0;
  E_ToECI(Time,&E1,&A);
  E_ToSpherical(&E1,&S);
  S_ToCartesian(&S,&B);
  E_ECRToECI(Time,&B,&B);
  if (!EQ(B.X,A.X) || !EQ(B.Y,A.Y) || !EQ(B.Z,A.Z)) {
    printf("ECIEarth Error (ToECI).\n");
    return;
  }

  printf("ECIEarth passed.\n");
  return;
}


void TestECIECL()
{
  Cartesian A,B,D;
  Matrix    M1,M2;
  double    Time;

  Time=2440000.5-2415020.0;
  E_MECIToECL(Time,M1);
  M_xRotation(DTR(23.0+26.0/60.0+36.22/3600.0),M2);
  if (!M_EQ_AP(M1,M2)) {
    printf("ECIECL Error (MECIECL).\n");
    return;
  }

  Time=2430000.5-2415020.0;
  E_MECLToECI(Time,M1);
  M_xRotation(-DTR(23.0+26.0/60.0+49.05/3600.0),M2);
  if (!M_EQ_AP(M1,M2)) {
    printf("ECIECL Error (MECLECI).\n");
    return;
  }

  A.X=10.0; A.Y=11.0; A.Z=12.0;
  E_ECIToECL(Time,&A,&D);
  M_xRotation(DTR(23.0+26.0/60.0+49.05/3600.0),M1);
  M_VMult(M1,&A,&B);
  if (!EQ(D.X,10.0) || !EQ_AP(D.Y,B.Y) || !EQ_AP(D.Z,B.Z)) {
    printf("ECIECL Error (ECIECL).\n");
    return;
  }

  E_ECLToECI(Time,&A,&D);
  M_xRotation(-DTR(23.0+26.0/60.0+49.05/3600.0),M1);
  M_VMult(M1,&A,&B);
  if (!EQ(D.X,10.0) || !EQ_AP(D.Y,B.Y) || !EQ_AP(D.Z,B.Z)) {
    printf("ECIECL Error (ECLECI).\n");
    return;
  }

  printf("ECIECL passed.\n");
  return;
}


main()
{
  printf("\nTesting the earth library (libearth).\n\n");

  TestCoord();
  TestBField();
  TestDays();
  TestLVLHLoc();
  TestLVLHECI();
  TestECIECR();
  TestECIEarth();
  TestECIECL();
}
