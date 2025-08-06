#include "strsp.h"
#include <math.h>
#include "advmath.h"
#include "attitude.h"
#include "earth.h"

#include <stdio.h>
#include <stdlib.h>


#define D_R_CONST             (PI/180.0)
#define R_D_CONST             (180.0/PI)

#define DEG_TO_RAD(deg)       ((deg)/180.0*PI)
#define RAD_TO_DEG(rad)       ((rad)*180.0/PI)

#define ANGLE_MAX_PI(x)       (asin(sin(x)))
#define ANGLE_MAX_TWOPI(x)    (acos(cos(x)))


void show_cartesian (char *s1, Cartesian vect, double scale, char *s2)
{
   printf ("%s(%11.4f, %11.4f, %11.4f)%s", s1, vect.X/scale, vect.Y/scale,
                                               vect.Z/scale, s2) ;
}



void show_spherical (char *s1, Spherical vect, double scale, char *s2)
{
   printf ("%s(%11.4f, T=%11.4f, P=%11.4f)%s", s1, vect.R/scale,
                         RAD_TO_DEG (vect.Th), RAD_TO_DEG(vect.Ph), s2) ;
}



void show_earth (char *s1, Earth vect, char *s2)
{
   printf ("%s(%11.6f, %11.6f) @ %9.2f%s", s1, RAD_TO_DEG(vect.Long),
                                               RAD_TO_DEG(vect.Lat),
                                               vect.Alt / 1000.0, s2) ;
}



/* ========================================================================== */



int main (int argc, char *argv[])
{
   double     SemiMajorAxis,
              Eccentricity,
              Inclination,
              RAAN,
              ArgPerigee,
              TrueAnomaly ;
   double     apogee, perigee ;
   Cartesian  sat_r_eci ;
   Cartesian  sat_v_eci ;

   if (argc != 7)
   {
      fprintf (stderr, "\n%s",
" Usage:	eci2ephem	<Rx> <Ry> <Rz> <Vx> <Vy> <Vz>\007\n\n") ;
      fprintf (stderr,
"			Inputs are position and velocity in ECI (earth\n") ;
      fprintf (stderr,
"			centered inertial) coordinates in meters.  Outputs\n") ;
      fprintf (stderr,
"			are as follows:\n") ;
      fprintf (stderr,
"				Apogee			[meters]\n") ;
      fprintf (stderr,
"				Perigee			[meters]\n") ;
      fprintf (stderr,
"				Inclination		[degrees]\n") ;
      fprintf (stderr,
"				RAAN			[degrees]\n") ;
      fprintf (stderr,
"				Argument of Perigee	[degrees]\n") ;
      fprintf (stderr,
"				True Anomaly		[degrees]\n") ;
      fprintf (stderr,
"			(Mean earth radius used is %6.1f kilometers.)\n\n",
               MEAN_R / 1000.0) ;
      exit (-1) ;
   }

   sat_r_eci.X = atof (argv[1]) ;
   sat_r_eci.Y = atof (argv[2]) ;
   sat_r_eci.Z = atof (argv[3]) ;
   
   sat_v_eci.X = atof (argv[4]) ;
   sat_v_eci.Y = atof (argv[5]) ;
   sat_v_eci.Z = atof (argv[6]) ;

   SAT_GetElements (&sat_r_eci, &sat_v_eci,
                    &SemiMajorAxis, &Eccentricity, &Inclination, 
                    &RAAN, &ArgPerigee, &TrueAnomaly) ;

   apogee  = SemiMajorAxis * (1.0 + Eccentricity) - MEAN_R ;
   perigee = SemiMajorAxis * (1.0 - Eccentricity) - MEAN_R ;

   TrueAnomaly = ANGLE_MAX_PI (TrueAnomaly) ;
   
   if (TrueAnomaly < 0.0)
      TrueAnomaly += (2.0 * M_PI) ;

   printf ("%11.4f %11.4f %12.6f %13.6f %13.6f %13.6f\n",
           apogee, perigee, RAD_TO_DEG (Inclination),
            RAD_TO_DEG (RAAN), RAD_TO_DEG (ArgPerigee),
            RAD_TO_DEG (TrueAnomaly) ) ;

   exit (0) ;
}
