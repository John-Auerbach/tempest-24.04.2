/* 
 *  Satellite Library routines
 *  
 *  SAT_GetSatPosition and SAT_Kepler originally from N3EMO-ORBIT 
 *  with some modifications to take advatage of Vector Algebra and
 *  to remove the time reference from GetSatPosition, so now it is given 
 *  the current RAAN, ArgPerigee, and other elements.
 *
 *  The N3EMO-ORBIT code is:
 *
 *      Copyright (c) 1986,1987,1988,1989,1990 Robert W. Berger N3EMO
 *      May be freely distributed, provided this notice remains intact.
 *
 *  The rest of the code is written by Adam London, 1992.
 *  
 *       adam@nova.stanford.edu
 *       aplondon@Athena.MIT.EDU
 *
 */

#include <math.h>
#include "advmath.h"
#include "earth.h"

#include "orbit.h"

/* Constant Vectors used in calculations */

static Cartesian Ivec = {1.0, 0.0, 0.0};
static Cartesian Kvec = {0.0, 0.0, 1.0};


/*
 *  Gets the current Orbital Elements for a Satellite given 
 *  *Sat and *SatV (the radius and velocity vectors) in ECI coordinates
 *  (units must be m and m/s)
 *
 *  returns:
 *   SemiMajorAxis (in m)
 *   Eccentricity of the orbit
 *   Inclination, RAAN, ArgPerigee, TrueAnomaly (in radians) (0 - TWO_PI);
 *   
 *  Basic algorithm and quadrant checks from:
 *
 *  Bate, Roger R., et.al., FUNDEMENTALS OF ASTRODYNAMICS, New York:
 *        Dover Publications, Inc.: 1971, pp. 24, 61-64.
 *
 *  If the orbit is eliptic and non-equatorial, returns all elements and has
 *  the value SUCCESS
 *
 *  If the orbit is either circular or Equatorial, returns FAILURE
 *  and gives undefined elements the value -1.0.
 *  - For equatorial, eliptic orbits, RAAN is undefined, and ArgPerigee is 
 *    measured from the X-axis. 
 *  - For circular, non-equatorial orbits, ArgPerigee is undefined, and
 *    TrueAnomaly is measured from the Assending node.
 *  - For circular, equatorial orbits, RAAN and ArgPerigee are not defined,
 *    and True Anomaly is measured from the X-axis.
 */

RETCODE SAT_GetElements(Sat, SatV, SemiMajorAxis, Eccentricity, Inclination, 
                        RAAN, ArgPerigee, TrueAnomaly)
Cartesian *Sat, *SatV;
double *SemiMajorAxis, *Eccentricity, *Inclination, *RAAN, 
       *ArgPerigee, *TrueAnomaly;
  {
    Cartesian AngMomVec, NodeVec, eVec;
    Cartesian temp1, temp2;
    double P;
    BOOLEAN Circular, Equatorial;

    Circular = Equatorial = FALSE;
    V_Cross(Sat, SatV, &AngMomVec);
    if (V_CrossIsZero(&Kvec, &AngMomVec, &NodeVec))
      Equatorial = TRUE;
    V_Mult(Sat, V_Dot(SatV, SatV)-GM/V_Mag(Sat), &temp1);
    V_Mult(SatV, V_Dot(Sat,SatV), &temp2); 
    V_Sub(&temp1, &temp2, &eVec);
    V_Mult(&eVec, 1.0/GM, &eVec);
    
    P = V_Dot(&AngMomVec, &AngMomVec)/GM;
    *Eccentricity = V_Mag(&eVec);
    if (*Eccentricity == 0.0)
      Circular = TRUE;
    *SemiMajorAxis = P/(1.0 - *Eccentricity*(*Eccentricity));    
    V_Angle(&Kvec, &AngMomVec, Inclination);
    if (!Equatorial && !Circular) {
      V_Angle(&Ivec, &NodeVec, RAAN);
      if (NodeVec.Y < 0.0)
        *RAAN = TWO_PI - *RAAN;
      V_Angle(&NodeVec, &eVec, ArgPerigee);
      if (eVec.Z < 0.0)
        *ArgPerigee = TWO_PI - *ArgPerigee;
      V_Angle(&eVec, Sat, TrueAnomaly);
      if (V_Dot(Sat, SatV) < 0.0)
        *TrueAnomaly = TWO_PI - *TrueAnomaly;
      return SUCCESS;
    }
    if (Circular && !Equatorial) {
      V_Angle(&Ivec, &NodeVec, RAAN);
      if (NodeVec.Y < 0.0)
        *RAAN = TWO_PI - *RAAN;
      V_Angle(&NodeVec, Sat, TrueAnomaly);
      if (V_Dot(Sat, SatV) < 0.0)
        *TrueAnomaly = TWO_PI - *TrueAnomaly;
      *ArgPerigee = -1.0;
      return FAILURE;
    }
    if (Equatorial && !Circular) {
      V_Angle(&Ivec, &eVec, ArgPerigee);
      if (eVec.Z < 0.0)
        *ArgPerigee = TWO_PI - *ArgPerigee;
      V_Angle(&eVec, Sat, TrueAnomaly);
      if (V_Dot(Sat, SatV) < 0.0)
        *TrueAnomaly = TWO_PI - *TrueAnomaly;
      *RAAN = -1.0;
      return FAILURE;
    }
    if (Equatorial && Circular) {
      V_Angle(&Ivec, Sat, TrueAnomaly);
      if (V_Dot(Sat, SatV) < 0.0)
        *TrueAnomaly = TWO_PI - *TrueAnomaly;
      *RAAN = -1.0;
      *ArgPerigee = -1.0;
      return FAILURE;
    }
  }

/* 
 * Returns a Satellites Position and Velocity given the 6 classical orbital
 * elements, 
 * SemiMajorAxis  (of the orbit) (in meters)
 * Eccentricity   (of the orbit)
 * Inclination    (angle between the obital and equatorial planes)
 * RAAN           (angle between the I vector and the assending node)
 * ArgPerigee     (angle between the assending node and perigee)
 * True Anomaly   (angle between perigee and the position vector)
 *       (all angles in radians)
 *
 * Position is returned in *Sat (m), and Velocity in *SatV (m/s), 
 * both in ECI.
 */

void SAT_GetSatPosition(SemiMajorAxis, Eccentricity, Inclination,
                        RAAN, ArgPerigee, TrueAnomaly, Sat, SatV)
 
double SemiMajorAxis, Eccentricity, Inclination;
double RAAN, ArgPerigee, TrueAnomaly;
Cartesian *Sat, *SatV;
  {
    double Xw,Yw,VXw,VYw;	/* In orbital plane */
    double Tmp;
    double Px,Qx,Py,Qy,Pz,Qz;	/* Escobal transformation 31 */
    double CosArgPerigee,SinArgPerigee;
    double CosRAAN,SinRAAN,CoSinclination,SinInclination;
    double Radius;

    Radius = SemiMajorAxis*(1 - Eccentricity*Eccentricity)
             / (1+Eccentricity*cos(TrueAnomaly));

    Xw = Radius * cos(TrueAnomaly);
    Yw = Radius * sin(TrueAnomaly);
    
    Tmp = sqrt(GM/(SemiMajorAxis*(1  - Eccentricity*Eccentricity)));

    VXw = -Tmp*sin(TrueAnomaly);
    VYw = Tmp*(cos(TrueAnomaly) + Eccentricity);

    CosRAAN = cos(RAAN); SinRAAN = sin(RAAN);
    CosArgPerigee = cos(ArgPerigee); SinArgPerigee = sin(ArgPerigee);
    CoSinclination = cos(Inclination); SinInclination = sin(Inclination);
    
    Px = CosArgPerigee*CosRAAN - SinArgPerigee*SinRAAN*CoSinclination;
    Py = CosArgPerigee*SinRAAN + SinArgPerigee*CosRAAN*CoSinclination;
    Pz = SinArgPerigee*SinInclination;
    Qx = -SinArgPerigee*CosRAAN - CosArgPerigee*SinRAAN*CoSinclination;
    Qy = -SinArgPerigee*SinRAAN + CosArgPerigee*CosRAAN*CoSinclination;
    Qz = CosArgPerigee*SinInclination;

    Sat->X = Px*Xw + Qx*Yw;		/* Escobal, transformation #31 */
    Sat->Y = Py*Xw + Qy*Yw;
    Sat->Z = Pz*Xw + Qz*Yw;

    SatV->X = Px*VXw + Qx*VYw;
    SatV->Y = Py*VXw + Qy*VYw;
    SatV->Z = Pz*VXw + Qz*VYw;
  }

/* 
 * Given TrueAnomaly (in radians), find MeanAnomaly (in radians)
 */


double SAT_GetMeanAnomaly(Eccentricity, TrueAnomaly)
double Eccentricity, TrueAnomaly;
  {
    double E;                       /* Eccentric Anomaly                   */
    
    E = acos((Eccentricity + cos(TrueAnomaly))/
             (1.0 +  Eccentricity*cos(TrueAnomaly)));
    if (TrueAnomaly > PI)
      E = TWO_PI - E;
    return (E - Eccentricity*sin(E));
  }

/*
 * Solve Kepler's equation
 *
 * Inputs:
 *      MeanAnomaly     Time Since last perigee, in radians.
 *                      TWO_PI = one complete orbit.
 *      Eccentricity    Eccentricity of orbit's ellipse.
 * Output:
 *      TrueAnomaly     Angle between perigee, geocenter, and
 *                      current position.
 */
 
double SAT_Kepler(MeanAnomaly,Eccentricity)
register double MeanAnomaly,Eccentricity;
  {
    register double E;              /* Eccentric Anomaly                   */
    register double Error;
    register double TrueAnomaly;
 
    E = MeanAnomaly ;/*+ Eccentricity*sin(MeanAnomaly);   /* Initial guess */
    do {
      Error = (E - Eccentricity*sin(E) - MeanAnomaly)
        / (1 - Eccentricity*cos(E));
      E -= Error;
    }  while (fabs(Error) >= HALF_ARC_SEC);

    if (fabs(E-PI) < HALF_ARC_SEC)
      TrueAnomaly = PI;
    else
      TrueAnomaly = 2*atan(sqrt((1+Eccentricity)/(1-Eccentricity))
                           *tan(E/2));
    if (TrueAnomaly < 0)
      TrueAnomaly += TWO_PI;
 
    return TrueAnomaly;
  }

/*
 * Gets the Precession of the A.N. and the Perapsis.
 * Formulas from Wertz and Larson, p. 125-126.
 *
 * Given A (in m), E, and I (in radians),
 * * returns deltaRAAN and deltaArgPerigee in radians/day
 *
 * Accounting for the effects of Sun, Moon, and J2 (earth oblateness)
 */

void SAT_GetPrecession(A, E, I, deltaRAAN, deltaArgPerigee)
double A, E, I, *deltaRAAN, *deltaArgPerigee;
  {
    double n, Denominator;
    
    n = sqrt(GM_D/(A*A*A))/TWO_PI;
    
    /* Moon's effects */
    *deltaRAAN = -0.00338*cos(I)/n;
    *deltaArgPerigee = 0.00169*(4.0 - 5.0*Sqr(sin(I)))/n;

    /* Sun's effects */
    *deltaRAAN += -0.00154*cos(I)/n;
    *deltaArgPerigee += 0.00077*(4.0 - 5.0*Sqr(sin(I)))/n;

    /* Earth's oblateness effect */
    Denominator = pow(A/1000.0, 7.0/2.0)*Sqr(1 - E*E);
    *deltaRAAN += -2.06474E14*cos(I)/Denominator;
    *deltaArgPerigee += 1.03237E14*(4.0 - 5.0*Sqr(sin(I)))/Denominator;

    /* Switch to Radians */
    *deltaRAAN = DTR(*deltaRAAN);
    *deltaArgPerigee = DTR(*deltaArgPerigee);
  }

/* 
 * Ave. Scale Height in km for a given alt. (100, 200, 300, etc. (km) 
 * from Wertz, Table L-6, p. 820.                          
 */
static double H[38] = {8.44,6.49,6.75,7.07,7.47,7.83,7.95,7.73,7.29,6.81,6.33,
                         6.00,5.70,5.41,5.38,5.74,6.15,8.06,11.6,16.1,20.6,
                         24.6,26.3,29.8,33.2,35.8,38.5,46.9,52.5,56.4,59.4,
                         62.2,65.8,79,109,164,225,268};

/* 
 * Ave. Atmospheric Density (kg/m^3) for a given alt (100, 200, 300, etc.)
 * from Wertz, Table L-6, p. 820.                          
 */
static double AtDensity[38] = {1.225,3.899E-2,1.774E-2,8.279E-3,3.972E-3,
                                 1.995E-3,1.057E-3,5.821E-4,3.206E-4,1.718E-4,
                                 8.770E-5,4.178E-5,1.905E-5,8.337E-6,3.396E-6,
                                 1.343E-6,5.297E-7,9.661E-8,2.438E-8,8.484E-9,
                                 3.845E-9,2.070E-9,1.244E-9,8.19E-10,5.464E-10,
                                 4.064E-10,2.789E-10,7.248E-11,2.418E-11,
                                 9.158E-12,3.725E-12,1.585E-12,6.967E-13,
                                 1.454E-13,3.614E-14,1.170E-14,5.245E-15,
                                 3.019E-15};


/*
 * Gets the Decay of the Semimajor Axis and the Eccentricity.
 * Formulas from Wertz and Larson, p. 125-126.
 * given: A (in m), E, and the BalCoef ( Mass(kg)/(Cd * Area (m^3)) )
 * Cd is appox. 2.2.
 *
 * returns deltaA (m per rev), and deltaE (per rev);
 */

void SAT_GetDecay(A, E, BalCoef, deltaA, deltaE)
double A, E, BalCoef, *deltaA, *deltaE;
  {
    double B, c, Rp, R, Alt, Altp;
    int altindex, altPindex;

    B = A*sqrt(1 - E*E);

    Rp = A*(1.0 - E);
    Altp = Rp - EARTH_A;
    if (Altp < 25000.0)
      altPindex = 0;
    if (Altp >= 25000.0 && Altp < 100000.0)
      altPindex = (int) rint(Altp/5000.0) - 4;
    if (Altp >= 100000.0 && Altp < 200000.0)
      altPindex = (int) rint(Altp/10000.0) + 6;
    if (Altp >= 200000.0 && Altp < 500000.0)
      altPindex = (int) rint(Altp/50000.0) + 22;
    if (Altp >= 500000.0 && Altp <= 1000000.0)
      altPindex = (int) rint(Altp/100000.0) + 27;
    if (Altp > 1000000.0)
      altPindex = 37;

    R = (A + B) / 2.0;
    Alt = R - EARTH_A;
    if (Alt < 25000.0)
      altindex = 0;
    if (Alt >= 25000.0 && Alt < 100000.0)
      altindex = (int) rint(Alt/5000.0) - 4;
    if (Alt >= 100000.0 && Alt < 200000.0)
      altindex = (int) rint(Alt/10000.0) + 6;
    if (Alt >= 200000.0 && Alt < 500000.0)
      altindex = (int) rint(Alt/50000.0) + 22;
    if (Alt >= 500000.0 && Alt <= 1000000.0)
      altindex = (int) rint(Alt/100000.0) + 27;
    if (Alt > 1000000.0)
      altindex = 37;

    c = (A/H[altindex])*(E/1000.0);
    
    *deltaA = -TWO_PI*(1.0/BalCoef)*A*A*AtDensity[altPindex]*exp(-c)
      *(ModBessel(0,c) + 2.0*E*ModBessel(1,c));

    *deltaE = -TWO_PI*(1.0/BalCoef)*A*AtDensity[altPindex]*exp(-c)
      *(ModBessel(1,c) + (E/2.0)*(ModBessel(0,c) + ModBessel(2,c)));
  }


/* 
 *  Gets Bearings from a *SitePos (in Earth (geodetic) coordinates) to
 *  a *Sat (in ECI (meters) coordinates).
 *
 *  *Azimuth is in radians: 0 = North, pi/2 = East, pi = South, 3*pi/2 = West
 *  *Elevation is in radians (+ is up)
 *  *Range is in meters
 *
 */


void SAT_GetBearings(curTime,SitePos,Sat,Azimuth,Elevation,Range)
double curTime;
Earth *SitePos;
Cartesian *Sat;
double *Azimuth, *Elevation, *Range;

  {
    Matrix SiteMat;
    Cartesian RotSite, RotSat, SiteToSat;
    Spherical S;


    E_ECIToECR(curTime,Sat,&RotSat);
    E_ToSpherical(SitePos, &S);
    S_ToCartesian(&S, &RotSite);
    V_Sub(&RotSat, &RotSite, &SiteToSat);
    E_MToLocalGeod(SitePos, SiteMat);
    M_VMult(SiteMat,&SiteToSat,&SiteToSat);

    *Range = V_Mag(&SiteToSat);
    *Elevation = asin(SiteToSat.Z/(*Range));
    *Azimuth = atan2(-SiteToSat.X,SiteToSat.Y);
    if (*Azimuth < 0.0)
      *Azimuth = -(*Azimuth);
    else
      *Azimuth = TWO_PI - *Azimuth;
  }
