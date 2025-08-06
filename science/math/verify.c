/* ADVANCED MATH ROUTINES' TESTER.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <math.h>

#include "advmath.h"


BOOLEAN EQ(X,Y)
double X;
double Y;
{
  return (fabs(X-Y)<1E-10);
}


BOOLEAN M_EQ(M,A00,A01,A02,A10,A11,A12,A20,A21,A22)
Matrix M;
double A00,A01,A02,A10,A11,A12,A20,A21,A22;
{
  return (EQ(M[0][0],A00) && EQ(M[0][1],A01) && EQ(M[0][2],A02) && 
	  EQ(M[1][0],A10) && EQ(M[1][1],A11) && EQ(M[1][2],A12) && 
	  EQ(M[2][0],A20) && EQ(M[2][1],A21) && EQ(M[2][2],A22));
}


void TestSchmidt()
{
  double P[5],dP[5];

  Schmidt_Array(4,0,PI,P,dP);
  if (!EQ(P[0],1.0) ||
      !EQ(P[1],-1.0) ||
      !EQ(P[2],1.0) ||
      !EQ(P[3],-1.0) ||
      !EQ(P[4],1.0)) {
    printf("Schmidt Error (P4,0).\n");
    return;
  }
  Schmidt_Array(4,0,HF_PI,P,dP);
  if (!EQ(dP[0],0.0) ||
      !EQ(dP[1],-1.0) ||
      !EQ(dP[2],0.0) ||
      !EQ(dP[3],1.5) ||
      !EQ(dP[4],0.0)) {
    printf("Schmidt Error (dP4,0).\n");
    return;
  }

  Schmidt_Array(3,1,HF_PI,P,dP);
  if (!EQ(P[3],-1.5*sqrt(1.0/6.0))) {
    printf("Schmidt Error (P3,1).\n");
    return;
  }
  Schmidt_Array(3,1,0.0,P,dP);
  if (!EQ(dP[3],6.0*sqrt(1.0/6.0))) {
    printf("Schmidt Error (dP3,1).\n");
    return;
  }

  printf("Schmidt passed.\n");
  return;
}


void TestALF()
{
  double X;

  X=ALF(0,0,HF_PI);
  if (!EQ(X,1.0)) {
    printf("ALF Error (P0,0).\n");
    return;
  }

  X=ALF_Derivative(0,0,PI);
  if (!EQ(X,0.0)) {
    printf("ALF Error (dP0,0).\n");
    return;
  }

  X=ALF(3,1,HF_PI);
  if (!EQ(X,-1.5)) {
    printf("ALF Error (P3,1).\n");
    return;
  }

  X=ALF_Derivative(3,1,0.0);
  if (!EQ(X,6.0)) {
    printf("ALF Error (dP3,1).\n");
    return;
  }

  printf("ALF passed.\n");
  return;
}


void TestUtil()
{
  Complex R1,R2;

  if (!EQ(Sqr(7.0),49.0)) {
    printf("Sqr Error.\n");
    return;
  }

  if (Sign(49.0)!=1 ||
      Sign(-0.1)!=-1 ||
      Sign(0.0)!=0) {
    printf("Sign Error.\n");
    return;
  }

  if (!EQ(Factorial(0),1.0) ||
      !EQ(Factorial(1),1.0) ||
      !EQ(Factorial(3),6.0)) {
    printf("Factorial Error.\n");
    return;
  }

  if (!EQ(ModBessel(2,10.0),12.5)) {
    printf("Modified Bessel Error.\n");
    return;
  }

  if (!EQ(LP(0,0.0),1.0) || !EQ(LP(1,0.0),1.0) || !EQ(LP(2,HF_PI),-0.5)) {
    printf("Legendre Polynomial Error.\n");
    return;
  }

  if (!Quadratic(1.0,2.0,1.0,&R1,&R2) ||
      !EQ(R1.R,-1.0) || !EQ(R1.I,0.0) ||
      !EQ(R2.R,-1.0) || !EQ(R2.I,0.0)) {
    printf("Quadratic Error (1,2,1).\n");
    return;
  }
  if (!Quadratic(1.0,0.0,-1.0,&R1,&R2) ||
      !EQ(R1.R,-1.0) || !EQ(R1.I,0.0) ||
      !EQ(R2.R,1.0) || !EQ(R2.I,0.0)) {
    printf("Quadratic Error (1,0,1).\n");
    return;
  }
  if (!Quadratic(1.0,1.0,1.0,&R1,&R2) ||
      !EQ(R1.R,-0.5) || !EQ(R1.I,sqrt(3.0)/2.0) ||
      !EQ(R2.R,-0.5) || !EQ(R2.I,-sqrt(3.0)/2.0)) {
    printf("Quadratic Error (1,1,1).\n");
    return;
  }
  if (!Quadratic(0.0,2.0,6.0,&R1,&R2) ||
      !EQ(R1.R,-3.0) || !EQ(R1.I,0.0) ||
      !EQ(R2.R,-3.0) || !EQ(R2.I,0.0)) {
    printf("Quadratic Error (0,2,6).\n");
    return;
  }
  if (Quadratic(0.0,0.0,8.0,&R1,&R2)) {
    printf("Quadratic Error (0,0,8).\n");
    return;
  }
  if (!Quadratic(0.0,0.0,0.0,&R1,&R2) ||
      !EQ(R1.R,0.0) || !EQ(R1.I,0.0) ||
      !EQ(R2.R,0.0) || !EQ(R2.I,0.0)) {
    printf("Quadratic Error (0,0,0).\n");
    return;
  }

  printf("Util passed.\n");
  return;
}


void TestVector()
{
  Cartesian A,B,C,D;
  double    X;

  A.X=1.0; A.Y=2.0; A.Z=3.0;
  B.X=4.0; B.Y=5.0; B.Z=6.0;

  D.X=D.Y=D.Z=0.0;

  V_Add(&A,&B,&C);
  if (!EQ(C.X,5.0) || !EQ(C.Y,7.0) || !EQ(C.Z,9.0)) {
    printf("Vector Error (add).\n");
    return;
  }
  
  V_Sub(&A,&B,&C);
  if (!EQ(C.X,-3.0) || !EQ(C.Y,-3.0) || !EQ(C.Z,-3.0)) {
    printf("Vector Error (subtract).\n");
    return;
  }

  V_Mult(&A,2.0,&C);
  if (!EQ(C.X,2.0) || !EQ(C.Y,4.0) || !EQ(C.Z,6.0)) {
    printf("Vector Error (multiply).\n");
    return;
  }
  
  if (!EQ(V_Dot(&A,&B),32.0)) {
    printf("Vector Error (dot).\n");
    return;
  }

  V_Cross(&A,&B,&B);
  if (!EQ(B.X,-3.0) || !EQ(B.Y,6.0) || !EQ(B.Z,-3.0)) {
    printf("Vector Error (cross).\n");
    return;
  }
  B.X=4.0; B.Y=5.0; B.Z=6.0;
  
  if (!EQ(V_Mag(&A),sqrt(14.0))) {
    printf("Vector Error (magniitude).\n");
    return;
  }

  if (!V_Unit(&A,&A) ||
      !EQ(A.X,1.0/sqrt(14.0)) ||
      !EQ(A.Y,2.0/sqrt(14.0)) ||
      !EQ(A.Z,3.0/sqrt(14.0))) {
    printf("Vector Error (unit 1).\n");
    return;
  }
  A.X=1.0; A.Y=2.0; A.Z=3.0;
  if (V_Unit(&D,&C)) {
    printf("Vector Error (unit 2).\n");
    return;
  }

  if (!V_Angle(&A,&B,&X) ||
      !EQ(X,acos(V_Dot(&A,&B)/(V_Mag(&A)*V_Mag(&B))))) {
    printf("Vector Error (angle 1).\n");
    return;
  }
  if (V_Angle(&D,&B,&X)) {
    printf("Vector Error (angle 2).\n");
    return;
  }

  if (V_DotIsZero(&A,&B,&X) ||
      !EQ(X,32.0)) {
    printf("Vector Error (dot zero 1).\n");
    return;
  }
  V_Cross(&A,&B,&C);
  if (!V_DotIsZero(&A,&C,&X)) {
    printf("Vector Error (dot zero 2).\n");
    return;
  }

  if (V_CrossIsZero(&A,&B,&A) ||
      !EQ(A.X,-3.0) || !EQ(A.Y,6.0) || !EQ(A.Z,-3.0)) {
    printf("Vector Error (cross zero 1).\n");
    return;
  }
  A.X=1.0; A.Y=2.0; A.Z=3.0;
  V_Mult(&A,2.0,&C);
  if (!V_CrossIsZero(&A,&C,&C)) {
    printf("Vector Error (cross zero 2).\n");
    return;
  }

  printf("Vector passed.\n");
  return;
}


void TestMatrix()
{
  Matrix    M1,M2,M3;
  Cartesian A;
  Frame     F;

  M1[0][0]=1.0; M1[0][1]=2.0; M1[0][2]=3.0;
  M1[1][0]=4.0; M1[1][1]=5.0; M1[1][2]=6.0;
  M1[2][0]=7.0; M1[2][1]=8.0; M1[2][2]=9.0;

  A.X=1.0; A.Y=2.0; A.Z=3.0;

  F.I.X=0.0; F.I.Y=0.0; F.I.Z=1.0;
  F.J.X=1.0; F.J.Y=0.0; F.J.Z=0.0;
  F.K.X=0.0; F.K.Y=1.0; F.K.Z=0.0;

  if (!EQ(M_Determinant(M1),0.0)) {
    printf("Matrix Error (determinant).\n");
    return;
  }

  M_Unit(M3);
  if (!M_EQ(M3,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("Matrix Error (unit).\n");
    return;
  }

  M_Transpose(M1,M2);
  if (!M_EQ(M2,
	    1.0,4.0,7.0,
	    2.0,5.0,8.0,
	    3.0,6.0,9.0)) {
    printf("Matrix Error (transpose).\n");
    return;
  }
  M_Transpose(M1,M1);
  if (!M_EQ(M1,
	    1.0,4.0,7.0,
	    2.0,5.0,8.0,
	    3.0,6.0,9.0)) {
    printf("Matrix Error (transpose self).\n");
    return;
  }

  M_VMult(M1,&A,&A);
  if (!EQ(A.X,30.0) || !EQ(A.Y,36.0) || !EQ(A.Z,42.0)) {
    printf("Matrix Error (vector multiply).\n");
    return;
  }

  M_MMult(M1,M1,M2);
  if (!M_EQ(M2,
	    30.0,66.0,102.0,
	    36.0,81.0,126.0,
	    42.0,96.0,150.0)) {
    printf("Matrix Error (multiply).\n");
    return;
  }

  M_xRotation(DTR(30),M2);
  if (!M_EQ(M2,
	    1.0,0.0,0.0,
	    0.0,cos(DTR(30)),sin(DTR(30)),
	    0.0,-sin(DTR(30)),cos(DTR(30)))) {
    printf("Matrix Error (x rotation).\n");
    return;
  }

  M_yRotation(DTR(30),M2);
  if (!M_EQ(M2,
	    cos(DTR(30)),0.0,-sin(DTR(30)),
	    0.0,1.0,0.0,
	    sin(DTR(30)),0.0,cos(DTR(30)))) {
    printf("Matrix Error (y rotation).\n");
    return;
  }

  M_zRotation(DTR(30),M2);
  if (!M_EQ(M2,
	    cos(DTR(30)),sin(DTR(30)),0.0,
	    -sin(DTR(30)),cos(DTR(30)),0.0,
	    0.0,0.0,1.0)) {
    printf("Matrix Error (z rotation).\n");
    return;
  }

  M_ToGlobal(&F,M2);
  if (!M_EQ(M2,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0,
	    1.0,0.0,0.0)) {
    printf("Matrix Error (to global).\n");
    return;
  }

  M_FromGlobal(&F,M2);
  if (!M_EQ(M2,
	    0.0,0.0,1.0,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0)) {
    printf("Matrix Error (from global).\n");
    return;
  }

  M_LocalxRotation(&F,DTR(30),M2);
  M_zRotation(-DTR(30),M1);
  M_MMult(M1,M2,M3);
  if (!M_EQ(M3,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("Matrix Error (local x rotation).\n");
    return;
  }

  M_LocalyRotation(&F,DTR(30),M2);
  M_xRotation(-DTR(30),M1);
  M_MMult(M1,M2,M3);
  if (!M_EQ(M3,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("Matrix Error (local y rotation).\n");
    return;
  }

  M_LocalzRotation(&F,DTR(30),M2);
  M_yRotation(-DTR(30),M1);
  M_MMult(M1,M2,M3);
  if (!M_EQ(M3,
	    1.0,0.0,0.0,
	    0.0,1.0,0.0,
	    0.0,0.0,1.0)) {
    printf("Matrix Error (local z rotation).\n");
    return;
  }

  printf("Matrix passed.\n");
  return;
}


void TestQuat()
{
  Quaternion A,B,C;
  Matrix     M1;
  Cartesian  V;

  A.Q1=cos(PI/4.0); A.Q2=sin(PI/4.0); A.Q3=0.0; A.Q4=0.0;
  C.Q1=0.0; C.Q2=0.0; C.Q3=1.0; C.Q4=0.0;

  V.X=0.0; V.Y=1.0; V.Z=0.0;

  Q_Matrix(&A,M1);
  M_VMult(M1,&V,&V);
  if (!EQ(V.X,0.0) || !EQ(V.Y,0.0) || !EQ(V.Z,1.0)) {
    printf("Quaternion Error (matrix).\n");
    return;
  }

  Q_Invert(&A,&B);
  if (!EQ(B.Q1,cos(PI/4)) || !EQ(B.Q2,-sin(PI/4)) ||
      !EQ(B.Q3,0.0) || !EQ(B.Q4,0.0)) {
    printf("Quaternion Error (invert).\n");
    return;
  }

  Q_Mult(&A,&C,&C);
  Q_Mult(&C,&B,&C);
  if (!EQ(C.Q1,0.0) || !EQ(C.Q2,0.0) ||
      !EQ(C.Q3,0.0) || !EQ(C.Q4,1.0)) {
    printf("Quaternion Error (multiply).\n");
    return;
  }

  printf("Quat passed.\n");
  return;
}


void TestSph()
{
  Spherical      S;
  Cartesian      C,Eij;
  int            i;
  double         Angle;
  SphSampleParam SP;

  S.Ph=DTR(90.0); S.Th=DTR(45.0); S.R=10.0;

  S_ToCartesian(&S,&C);
  if (!EQ(C.X,10.0/sqrt(2.0)) || !EQ(C.Y,10.0/sqrt(2.0)) || !EQ(C.Z,0.0)) {
    printf("Spherical Error (to cartesian).\n");
    return;
  }

  S_FromCartesian(&C,&S);
  if (!EQ(S.Ph,HF_PI) || !EQ(S.Th,HF_PI/2.0) || !EQ(S.R,10.0)) {
    printf("Spherical Error (from cartesian 1).\n");
    return;
  }
  C.X=C.Y=C.Z=0.0;
  S_FromCartesian(&C,&S);
  if (!EQ(S.Ph,0.0) || !EQ(S.Th,0.0) || !EQ(S.R,0.0)) {
    printf("Spherical Error (from cartesian 2).\n");
    return;
  }
  C.Z=10.0;
  S_FromCartesian(&C,&S);
  if (!EQ(S.Ph,0.0) || !EQ(S.Th,0.0) || !EQ(S.R,10.0)) {
    printf("Spherical Error (from cartesian 2).\n");
    return;
  }

  S.Ph=DTR(350.0); S.Th=DTR(200.0); S.R=(-10.0);
  S_Normalize(&S);
  if (!EQ(S.Ph,DTR(170.0)) || !EQ(S.Th,DTR(-160.0)) || !EQ(S.R,10.0)) {
    printf("Spherical Error (normalize).\n");
    return;
  }

  SP.E.X=1.0; SP.E.Y=(-1.0); SP.E.Z=(-1.0);
  SP.Spread=DTR(90.0);
  SP.dSpread=DTR(44.9999);
  if (!S_NextSample(TRUE,&SP,&Eij)) {
    printf("Spherical Error (sampling 1).\n");
    return;
  }
  i=0;
  do {
    i++;
    if (!V_Angle(&Eij,&SP.E,&Angle)) {
      printf("Spherical Error (sampling 2).\n");
      return;
    }
  } while (S_NextSample(FALSE,&SP,&Eij));
  if (i!=13) {
    printf("Spherical Error (sampling 3).\n");
    return;
  }

  printf("Sph passed.\n");
  return;
}


RETCODE TestOneEqn(a,b,c,d,omega,N)
double a;
double b;
double c;
double d;
double omega;
int N;
{
  double t,tt;

  if (!Eqn_NextSolution(TRUE,a,b,c,d,omega,1E-14,&t) && N) {
    printf("Equation Error (1; %le %le %le %le %le).\n",a,b,c,d,omega);
    return FAILURE;
  }
  if (!N)
    return SUCCESS;
  while (1) {
    N--;
    if (!EQ(0.0,a*cos(omega*t)+b*sin(omega*t)+c*t+d)) {
      printf("Equation Error (2; %le %le %le %le %le).\n",a,b,c,d,omega);
      return FAILURE;
    }
    tt=t;
    if (!Eqn_NextSolution(FALSE,0.0,0.0,0.0,0.0,0.0,0.0,&t))
      break;
    if (EQ(t,tt)) {
      printf("Equation Error (3; %le %le %le %le %le).\n",a,b,c,d,omega);
      return FAILURE;
    }
  }
  if (N) {
    printf("Equation Error (4; %le %le %le %le %le).\n",a,b,c,d,omega);
    return FAILURE;
  }
  return SUCCESS;
}

void TestEqn()
{
  if (!TestOneEqn(4.0,3.0,0.1,0.0,0.02,0))
    return;
  if (!TestOneEqn(4.0,3.0,0.02,0.0,0.02,2))
    return;
  if (!TestOneEqn(4.0,3.0,0.01,-4.5,0.02,7))
    return;
  if (!TestOneEqn(2.0,1.0,0.1,-10.0,0.02,1))
    return;
  if (!TestOneEqn(0.0,1.0,0.0,0.0,1.0,2))
    return;

  printf("Eqn passed.\n");
  return;
}


main()
{
  printf("\nTesting the advanced math library (libam).\n\n");

  TestSchmidt();
  TestALF();
  TestUtil();
  TestVector();
  TestMatrix();
  TestQuat();
  TestSph();
  TestEqn();
}
