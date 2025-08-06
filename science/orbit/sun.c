/* 
 *  Sun Library routines
 *  
 *  ADAM LONDON, Summer 1992.
 *  
 *       adam@nova.stanford.edu
 *       aplondon@Athena.MIT.EDU
 *
 *
 *  SUN_Eclipsed originally from N3EMO-ORBIT with some modifications to 
 *  take advatage of Vector Algebra. SUN_GetSunsElements originally part of
 *  InitOrbitRoutines from N3EMO-ORBIT. 
 *
 *  The N3EMO-ORBIT code is:
 *
 *      Copyright (c) 1986,1987,1988,1989,1990 Robert W. Berger N3EMO
 *      May be freely distributed, provided this notice remains intact.
 *
 */

#include <math.h>
#include "advmath.h"
#include "earth.h"

#include "orbit.h"

/* 
 * Returns TRUE if the point Sat in ECI is in the shadow of the Earth,
 * given the position of the Sun in ECI.
 */

BOOLEAN SUN_Eclipsed(Sat, Sun)
Cartesian *Sat, *Sun;
  {
    double SunDistance, SatRadius;
    double CosTheta;
    double SinPenumbra, CosPenumbra;

    SunDistance = V_Mag(Sun);
    SatRadius = V_Mag(Sat);

    SinPenumbra = (SUN_RADIUS-EARTH_A)/SunDistance;
    CosPenumbra = sqrt(1 - SinPenumbra*SinPenumbra);

    CosTheta = V_Dot(Sun,Sat)/(SunDistance*SatRadius)
		 *CosPenumbra + (SatRadius/EARTH_A)*SinPenumbra;

    if (CosTheta < 0)
        if (CosTheta < -sqrt(SatRadius*SatRadius-EARTH_A*EARTH_A)/SatRadius
	    		*CosPenumbra + (SatRadius/EARTH_A)*SinPenumbra){
          return TRUE;
        }
    return FALSE;
  }

/*
 * Initialize the Sun's keplerian elements for a given epoch.
 * Formulas are from "Explanatory Supplement to the Astronomical Ephemeris".
 */

void SUN_GetSunsElements(EpochDay,SunEpochTime,SunRAAN,SunInclination,
                         SunEccentricity,SunArgPerigee,SunEpochMeanAnomaly,
                         SunMeanMotion)
double EpochDay;
double *SunEpochTime, *SunRAAN,  *SunInclination;
double *SunEccentricity, *SunArgPerigee;
double *SunEpochMeanAnomaly, *SunMeanMotion;
  {
    double T,T2,T3,Omega;
    int n;

    T = (floor(EpochDay)-0.5)/36525;
    T2 = T*T;
    T3 = T2*T;

    /* Omega is used to correct for the nutation and the abberation */
    Omega = DTR(259.18 - 1934.142*T);
    n = Omega / TWO_PI;
    Omega -= n*TWO_PI;

    *SunEpochTime = floor(EpochDay);
    *SunRAAN = 0;

    *SunInclination = DTR(23.452294 - 0.0130125*T - 0.00000164*T2 
                          + 0.000000503*T3 + 0.00256*cos(Omega));
    *SunEccentricity = (0.01675104 - 0.00004180*T - 0.000000126*T2);
    *SunArgPerigee = DTR(281.220833 + 1.719175*T + 0.0004527*T2 
                         + 0.0000033*T3);
    *SunEpochMeanAnomaly = DTR(358.475845 + 35999.04975*T - 0.00015*T2
                          - 0.00000333333*T3);
    n = *SunEpochMeanAnomaly / TWO_PI;
    *SunEpochMeanAnomaly -= n*TWO_PI;

    *SunMeanMotion = 1/(365.24219879 - 0.00000614*T);
  }


/* 
 * Gives Suns Position at time Time. Need to Call GetSunsElements prior to
 * this to set parameters 
 */

void SUN_GetSunPosition(Time,SunEpochTime,SunEpochMeanAnomaly,SunMeanMotion,
                        SunRAAN,SunArgPerigee,SunInclination,SunEccentricity,
                        Sun)
double Time,SunEpochTime,SunEpochMeanAnomaly,SunMeanMotion;
double SunRAAN,SunArgPerigee,SunInclination,SunEccentricity;
Cartesian *Sun;
  {
    double SunMeanAnomaly, SunTrueAnomaly;
    Cartesian SunV;                         /*  unused   */

    SunMeanAnomaly = SunEpochMeanAnomaly + 
      (Time-SunEpochTime)*SunMeanMotion*TWO_PI;
    SunTrueAnomaly = SAT_Kepler(SunMeanAnomaly,SunEccentricity);

    SAT_GetSatPosition(SUN_SEMI_MAJOR_AXIS, SunEccentricity, SunInclination,
                   SunRAAN, SunArgPerigee, SunTrueAnomaly, Sun, &SunV);
  }


/* Returns in *R the position of the sun in earth centered inertial
   coordinates, at time Time. Time should be in solar days from January
   0, 1900. */

void Sun_GetPosition(Time,R)
double    Time;
Cartesian *R;
{
  double SunEpochTime,SunRAAN,SunInclination;
  double SunEccentricity,SunArgPerigee,SunEpochMeanAnomaly;
  double SunMeanMotion;

  SUN_GetSunsElements(Time,&SunEpochTime,&SunRAAN,&SunInclination,
		      &SunEccentricity,&SunArgPerigee,&SunEpochMeanAnomaly,
		      &SunMeanMotion);
  SUN_GetSunPosition(Time,SunEpochTime,SunEpochMeanAnomaly,SunMeanMotion,
		     SunRAAN,SunArgPerigee,SunInclination,SunEccentricity,
		     R);
  return;
}
