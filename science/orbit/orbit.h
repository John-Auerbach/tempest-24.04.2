/*
 * PRIVATE HEADER FILE OF ORBIT MODELLING ROUTINES.
 * (Satellites, Moon, & Sun)
 *
 *  ADAM LONDON, Summer 1992.
 *  
 *       adam@nova.stanford.edu
 *       aplondon@Athena.MIT.EDU
 *
 */

/* FUNDAMENTAL CONSTANTS. */

#define SUN_RADIUS              695990000.0        /* meters           */
#define SUN_SEMI_MAJOR_AXIS     149598845000.0     /* meters           */
#define GM                      3.986005E14        /* meters^3/sec^2   */
#define GM_D                    2.97553678848E24   /* meters^3/day^2   */

/* DERIVED CONSTANTS. */

#define HALF_ARC_SEC            2.4240684056E-6    /* 0 deg 0 min 1/2 sec  */
                                                   /* in radians           */


/* FUNCTION DECLARATIONS. */

#ifdef _NO_PROTO

/* sat.c */
RETCODE SAT_GetElements();
void SAT_GetSatPosition();
double SAT_GetMeanAnomaly();
double SAT_Kepler();
void SAT_GetBearings();


/* moon.c */

void MN_GetMoonPos();
BOOLEAN MN_MoonVisable();
double MN_GetMoonPhase();


/* sun.c */

BOOLEAN SUN_Eclipsed();
void SUN_GetSunsElements();
void SUN_GetSunPosition();
                        

#else


/* sat.c */

RETCODE SAT_GetElements(Cartesian *,Cartesian *,double *,double *,
                        double *,double *,double *,double *);
void SAT_GetSatPosition(double, double, double, double, double, double, 
                        Cartesian *, Cartesian *);
double SAT_GetMeanAnomaly(double, double);
double SAT_Kepler(double, double);
void SAT_GetBearings(double, Earth *, Cartesian *, double *, double *, 
                     double *);


/* moon.c */

void MN_GetMoonPos(double, Cartesian *);
BOOLEAN MN_MoonVisable(Cartesian *, Cartesian *);
double MN_GetMoonPhase(Cartesian *, Cartesian *, Cartesian *);


/* sun.c */

BOOLEAN SUN_Eclipsed(Cartesian *, Cartesian *);
void SUN_GetSunsElements(double, double *, double *, double *, double *, 
                         double *, double *, double *);
void SUN_GetSunPosition(double, double, double, double, double, double, 
                        double, double, Cartesian *);
void Sun_GetPosition(double,Cartesian *);


#endif
