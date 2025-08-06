/* PRIVATE HEADER FILE OF EARTH MODELLING ROUTINES.

   Apostolos Lerios - TOLIS@NOVA. */


/* FUNDAMENTAL CONSTANTS. */

/* IAU 1966 (IGRF recommended) earth ellipsoid, in meters. */

#define EARTH_A 6378160.0   /* Semimajor axis. */
#define EARTH_B 6356774.719 /* Semiminor axis. */

/* IGRF mean earth radius, in meters. */

#define MEAN_R 6371200.0

/* Length of a mean solar day, in mean sidereal days. */

#define SOLAR_SIDEREAL 1.0027379093

/* Angular rotation rate of the earth, in radians per second. */

#define EARTH_OMEGA TWO_PI/(23.93447*3600.0)


/* DERIVED CONSTANTS. */

#define RATIO     (EARTH_A/EARTH_B)
#define RATIO_SQR (RATIO*RATIO)


/* UNIT CONVERSION MACROS. */

#define GAUSS_TO_TESLA(x)     ((x)/10000.0)
#define TESLA_TO_GAUSS(x)     ((x)*10000.0)
#define NANOTESLA_TO_TESLA(x) ((x)/1.0E9)


/* EARTH COORDINATES. */

/* Latitude: geodetic; + is North; in radians; -90 to 90, both ends inclusive.
   Longitude: + is east; in radians; -180 (exclusive) to 180 (inclusive).
   Altitude: from sea level; in meters. */

typedef struct {
  double Long,Lat,Alt;
} Earth;


/* TRACING PARAMETER. */

/* Direction of field line tracing. */

typedef enum {WHILE_BELOW,WHILE_ABOVE} TRACE_DIRECTION;


/* FUNCTION DECLARATIONS. */

#ifdef _NO_PROTO

/* bfield.c */

void IGRF_SetYear();
void IGRF_GetBField();
void IGRF_GetBField_ord();
RETCODE IGRF_Trace();

/* coord.c */

void E_ToSpherical();
void E_FromSpherical();
void E_LocalFrame();
void E_LocalGeocToGeod();
void E_LocalGeodToGeoc();
void E_MFromLocalGeod();
void E_MToLocalGeod();
void E_MFromLocalGeoc();
void E_MToLocalGeoc();

/* eciecr.c */

void E_MECIToECR();
void E_MECRToECI();
void E_ECIToECR();
void E_ECRToECI();

/* eciecl.c */

void E_MECIToECL();
void E_MECLToECI();
void E_ECIToECL();
void E_ECLToECI();

/* lvlheci.c */

RETCODE E_LVLHFrame();
RETCODE E_MECIToLVLH();
RETCODE E_MLVLHToECI();
RETCODE E_ECIToLVLH();
RETCODE E_LVLHToECI();

/* lvlhloc.c */

RETCODE E_AngleLocalLVLH();
RETCODE E_MLocalToLVLH();
RETCODE E_MLVLHToLocal();
RETCODE E_LocalToLVLH();
RETCODE E_LVLHToLocal();

/* eciearth.c */

void E_FromECI();
void E_ToECI();

/* days.c */

double DAY_FromYMDS();

#else

/* bfield.c */

void IGRF_SetYear(double);
void IGRF_GetBField(Spherical *,Cartesian *);
void IGRF_GetBField_ord(Spherical *,Cartesian *,int order);
RETCODE IGRF_Trace(Earth *,double,TRACE_DIRECTION,double,Cartesian *);

/* coord.c */

void E_ToSpherical(Earth *,Spherical *);
void E_FromSpherical(Spherical *,Earth *);
void E_LocalFrame(BOOLEAN,Earth *,Spherical *,Frame *);
void E_LocalGeocToGeod(Earth *,Spherical *,Cartesian *,Cartesian *);
void E_LocalGeodToGeoc(Earth *,Spherical *,Cartesian *,Cartesian *);
void E_MFromLocalGeod(Earth *,Matrix);
void E_MToLocalGeod(Earth *,Matrix);
void E_MFromLocalGeoc(Spherical *,Matrix);
void E_MToLocalGeoc(Spherical *,Matrix);

/* eciecr.c */

void E_MECIToECR(double,Matrix);
void E_MECRToECI(double,Matrix);
void E_ECIToECR(double,Cartesian *,Cartesian *);
void E_ECRToECI(double,Cartesian *,Cartesian *);

/* eciecl.c */

void E_MECIToECL(double,Matrix);
void E_MECLToECI(double,Matrix);
void E_ECIToECL(double,Cartesian *,Cartesian *);
void E_ECLToECI(double,Cartesian *,Cartesian *);

/* lvlheci.c */

RETCODE E_LVLHFrame(Cartesian *,Cartesian *,Frame *);
RETCODE E_MECIToLVLH(Cartesian *,Cartesian *,Matrix);
RETCODE E_MLVLHToECI(Cartesian *,Cartesian *,Matrix);
RETCODE E_ECIToLVLH(Cartesian *,Cartesian *,Cartesian *,Cartesian *);
RETCODE E_LVLHToECI(Cartesian *,Cartesian *,Cartesian *,Cartesian *);

/* lvlhloc.c */

RETCODE E_AngleLocalLVLH(Cartesian *,Cartesian *,double *);
RETCODE E_MLocalToLVLH(Cartesian *,Cartesian *,Matrix);
RETCODE E_MLVLHToLocal(Cartesian *,Cartesian *,Matrix);
RETCODE E_LocalToLVLH(Cartesian *,Cartesian *,Cartesian *,Cartesian *);
RETCODE E_LVLHToLocal(Cartesian *,Cartesian *,Cartesian *,Cartesian *);

/* eciearth.c */

void E_FromECI(double,Cartesian *,Earth *);
void E_ToECI(double,Earth *,Cartesian *);

/* days.c */

double DAY_FromYMDS(unsigned int,unsigned int,unsigned int,unsigned long);

#endif
