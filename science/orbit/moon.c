/* 
 *  Moon Library routines
 *  
 *  ADAM LONDON, Summer 1992.
 *  
 *       adam@nova.stanford.edu
 *       aplondon@Athena.MIT.EDU
 *
 */

#include <math.h>
#include "advmath.h"
#include "earth.h"

#include "orbit.h"

/* Constants for Moon position determination  */
/* All from Wertz, p. 141-143                 */

#define Fi            0
#define Fpi           1
#define Gi            2
#define Gpi           3

#define MOON_INCLINATION      (DTR(5.145396374))   /* (deg)  to the ecliptic */
#define MOON_DIST_CONST       6378388.0            /* (m)                    */
#define MOON_RADIUS           1738200.0            /* (m)                    */

/*
 * Integer coefs for the sin series to correct the long. of Moon, L
 */
static int coef_dL[13][4] = {
    {0,0,0,0},
    {1,0,0,0},
    {1,0,0,-2},
    {0,0,0,2},
    {2,0,0,0},
    {0,1,0,0},
    {0,0,2,0},
    {2,0,0,-2},
    {1,1,0,-2},
    {1,0,0,2},
    {0,1,0,-2},
    {1,-1,0,0},
    {0,0,0,1}};

/*
 *  Integer coefs for the cos series to determine the moon's paralax, P
 */
static int coef_P[10][4] = {
    {0,0,0,0},
    {0,0,0,0},
    {1,0,0,0},
    {1,0,0,-2},
    {0,0,0,2},
    {2,0,0,0},
    {1,0,0,2},
    {0,1,0,-2},
    {1,1,0,-2},
    {1,-1,0,0}};

/* coef of sin in L series, (deg) */
static double Ai[13] = {0.0, 6.289, -1.274, 0.658, 0.213, -0.185, -0.114,
                        -0.059, -0.057, 0.053, -0.046, 0.041, -0.035};

/* coef of cos in P series (arc sec) */
static double Bi[10] = {0.0, 3422.7, 186.5398, 34.3117, 28.2373, 10.1657,
                        3.0861, 1.9178, 1.4437, 1.1528};



/*  
 *  Returns the position of the Moon in ECI in *MoonVec at time Time.
 *  Time in Days since Jan 0, 1900.
 *
 *  All formulas and constants from Wertz, Sapcecraft Attitude Determination 
 *  And Control, 1978, p. 141-143
 */

void MN_GetMoonPos(CurrentTime, MoonVec)
double CurrentTime;
Cartesian *MoonVec;
  {
    double L;       /* Mean Long. of Moon                          */
    double Omega;   /* Mean Long. of Moon's A.N.                   */
    double Gamma;   /* Mean Long. of Moon's Perigee                */
    double D;       /* Mean elongation of Moon from Sun            */
    double SunM;    /* Mean anomaly of Sun                         */
    double deltaL;  /* correction to mean Long. of Moon            */
    double P;       /* Moon's paralax                              */
    

    Cartesian xprime, yprime;
    double sinOmega, cosOmega, cosMoonI;
    double phi, alpha, delta, beta;
    double d, d2, d3;
    double MoonRad;
    Matrix MatA;
    int i;
    
    d = CurrentTime - 0.5;      /* Make it days since 12:00 Jan 0, 1900 */
    d2 = d*d;
    d3 = d2*d;
 
    L =     DTR(270.434164 + 13.1763965268*d - 8.5E-13*d2 + 3.9E-20*d3);
    Gamma = DTR(334.329356 + 0.1114040803*d - 7.739E-12*d2 - 2.6E-19*d3);
    Omega = DTR(259.183275 - 0.0529539222*d + 1.557E-12*d2 + 5E-20*d3);
    D =     DTR(350.737486 + 12.1907491914*d - 1.076E-12*d2 + 3.9E-20*d3);
    SunM =  DTR(358.475845 + 0.985600267*d - 1.12E-13*d2 - 7E-20*d3);
    
    deltaL = 0.0;
    for (i=1; i<=12; i++)
      deltaL += Ai[i]*sin(coef_dL[i][Fi]*(L-Gamma) +
                          coef_dL[i][Fpi]*SunM +
                          coef_dL[i][Gi]*(L-Omega) +
                          coef_dL[i][Gpi]*D);
    
    deltaL = DTR(deltaL);
    P = 0.0;
    for (i=1; i<=9; i++)
      P += Bi[i]*cos(coef_P[i][Fi]*(L-Gamma) +
                     coef_P[i][Fpi]*SunM +
                     coef_P[i][Gi]*(L-Omega) +
                     coef_P[i][Gpi]*D);
    P = DTR(P/3600.0);

    L += deltaL;
    L -= floor(L / TWO_PI) * TWO_PI;
    Omega -= floor(Omega/TWO_PI) * TWO_PI;
    
    cosOmega = cos(Omega);
    sinOmega = sin(Omega); 
    cosMoonI = cos(MOON_INCLINATION);

    xprime.X = cosOmega;
    xprime.Y = sinOmega;
    xprime.Z = 0.0;
    
    yprime.X = -cosMoonI*sinOmega;
    yprime.Y = cosMoonI*cosOmega;
    yprime.Z = sin(MOON_INCLINATION);

    MoonRad = MOON_DIST_CONST/sin(P);
    phi = L - Omega;
    
    V_Mult(&xprime, MoonRad*cos(phi), &xprime);
    V_Mult(&yprime, MoonRad*sin(phi), &yprime);
    V_Add(&xprime, &yprime, MoonVec);

    E_ECLToECI(CurrentTime, MoonVec, MoonVec);
  }


/*
 * Returns TRUE if the Moon is not obscured by the earth as observed from
 * the given point *Sat in ECI. Accounts for width of moons disk. *Moon in
 * ECI as well.
 */

BOOLEAN MN_MoonVisable(Moon, Sat)
Cartesian *Moon, *Sat;
  {
    Cartesian StoM, MoonRad, Z, StoE, temp;
    double theta, SatR;   /* angle between earth and moon at sat, and  */
                          /* radius of Sat from Earth */


    V_Sub(Moon, Sat, &StoM);
    V_Mult(Sat, -1.0, &StoE);
    V_Cross(&StoM, &StoE, &Z);
    V_Mult(&StoM, -1.0, &temp);
    V_Cross(&Z, &temp, &MoonRad);
    V_Unit(&MoonRad, &MoonRad);
    V_Mult(&MoonRad, MOON_RADIUS, &MoonRad);
    V_Add(&StoM, &MoonRad, &StoM);
    SatR = V_Mag(Sat);                       
    if (V_Angle(&StoM, &StoE, &theta) == SUCCESS) {
      if (theta > HF_PI)                       /*between Moon and earth */
        return TRUE;
      else if (SatR*sin(theta) > EARTH_A)
        return TRUE;
      else
        return FALSE;
    }
    else
      return FALSE;
  }


/* 
 * Returns the phase of the Moon (0 - PI) in radians, (Full moon returns
 * PI, and New Moon returns 0) as observed from point *Sat (in ECI), 
 * given position of Sun (*Sun) and Moon (*Moon) in ECI.
 * If any error occurs (one vector has mag. Zero), returns -1.0
 */

double MN_GetMoonPhase(Sat, Sun, Moon)
Cartesian *Sat, *Sun, *Moon;
  {
    Cartesian Moon2Sat, Moon2Sun;
    double phi;  /* angle between sun and sat at moon */

    V_Sub(Sun, Moon, &Moon2Sun);
    V_Sub(Sat, Moon, &Moon2Sat);
    if (V_Angle(&Moon2Sat, &Moon2Sun, &phi) == SUCCESS)
      return (PI - phi);
    else
      return (-1.0);
  }

