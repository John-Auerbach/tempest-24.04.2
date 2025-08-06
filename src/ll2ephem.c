#include "strsp.h"
#include <math.h>
#include "gmt.h"
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

#define ERROR_DELTA           (0.000001*PI/180.0)
#define MAX_BISECTIONS        50

#define DEBUG 0

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
   GMT        curr_gmt ;
   double     curr_time, curr_year ;
   double     SemiMajorAxis,
              Eccentricity,
              Inclination,
              RAAN,
              ArgPerigee,
              TrueAnomaly ;
   double     r_apogee, r_perigee ;
   double     apogee, perigee ;
   Earth      sat_r_lla ;
   Cartesian  sol_r_eci ;
   Cartesian  sol_v_eci ;
   Earth      sol_r_lla ;
   double     altitude  ;
   double     a, b, start, delta, error ;
   int        count ;

   if (argc != 10)
   {
      fprintf (stderr, "\n%s",
" Usage:	ll2ephem	<Lat> <Long> <Alt> <Alt Apogee> <Alt Perigee>\n") ;
      fprintf (stderr, 
"                               <ArgPerigee> <Inclination> <GMT> <Year>\007\n\n") ;
      fprintf (stderr,
"                       Inputs are inclination and altitudes of desired orbit\n") ;
      fprintf (stderr,
"			along with initial latitude, longitude and altitude at\n") ;
      fprintf (stderr,
"			specified time.  The units for the parameters are:\n") ;
      fprintf (stderr,
"				Latitude		[degrees]\n") ;
      fprintf (stderr,
"				Longitude		[degrees]\n") ;
      fprintf (stderr,
"				Altitude		[meters]\n") ;
      fprintf (stderr,
"				Apogee			[meters]\n") ;
      fprintf (stderr,
"				Perigee			[meters]\n") ;
      fprintf (stderr,
"				Argument of Perigee	[degrees]\n") ;
      fprintf (stderr,
"				Inclination		[degrees]\n") ;
      fprintf (stderr,
"				GMT			ddd/hh:mm:ss.fff\n") ;
      fprintf (stderr,
"			(Mean earth radius used is %6.1f kilometers.)\n\n",
               MEAN_R / 1000.0) ;
      exit (-1) ;
   }

   sat_r_lla.Lat  = DEG_TO_RAD (atof (argv[1])) ;
   sat_r_lla.Long = DEG_TO_RAD (atof (argv[2])) ;
   sat_r_lla.Alt  = atof (argv[3]) ;
   r_apogee       = atof (argv[4]) + MEAN_R ;
   r_perigee      = atof (argv[5]) + MEAN_R ;
   ArgPerigee     = DEG_TO_RAD (atof (argv[6])) ;
   Inclination    = DEG_TO_RAD (atof (argv[7])) ;
   GMT_FromStr      (argv[8], &curr_gmt) ;
   curr_year      = atof (argv[9]) ;

   curr_time = Day_FromGMT (curr_year, &curr_gmt) ;

   SemiMajorAxis = 0.5 * (r_apogee + r_perigee) ;
   Eccentricity  = 0.5 * (r_apogee - r_perigee) / SemiMajorAxis ;
   if (Eccentricity < 1e-40)
      Eccentricity = 1e-40 ;

#if DEBUG
printf ("lat=%f long=%f alt=%f r_a=%f r_p=%f\n",
         sat_r_lla.Lat, sat_r_lla.Long, sat_r_lla.Alt,r_apogee,r_perigee) ;
printf ("arg_p=%f inclin=%f time=%f a=%f e=%f\n",
        ArgPerigee, Inclination, curr_time, SemiMajorAxis, Eccentricity) ;
#endif

   a     = 0.5 * PI  ;
   b     = 0.0       ;

   TrueAnomaly = b ;
   SAT_GetSatPosition (SemiMajorAxis, Eccentricity, Inclination,
                       RAAN, ArgPerigee, TrueAnomaly,
                       &sol_r_eci, &sol_v_eci) ;
  
   altitude = V_Mag (&sol_r_eci) ; 
   E_FromECI (curr_time, &sol_r_eci, &sol_r_lla) ;
   error = sat_r_lla.Lat - sol_r_lla.Lat ;
#if DEBUG
printf ("TrueAnomaly = %f lat=%f lon=%f alt=%f Altitude=%f error_b=%f\n",
         RAD_TO_DEG (TrueAnomaly), RAD_TO_DEG(sol_r_lla.Lat),
         RAD_TO_DEG (sol_r_lla.Long), sol_r_lla.Alt*0.001, altitude*0.001,
         RAD_TO_DEG (error)) ;
#endif
   
   TrueAnomaly = a ;
   SAT_GetSatPosition (SemiMajorAxis, Eccentricity, Inclination,
                       RAAN, ArgPerigee, TrueAnomaly,
                       &sol_r_eci, &sol_v_eci) ;
  
   altitude = V_Mag (&sol_r_eci) ; 
   E_FromECI (curr_time, &sol_r_eci, &sol_r_lla) ;
   error = sat_r_lla.Lat - sol_r_lla.Lat ;
#if DEBUG
printf ("TrueAnomaly = %f lat=%f lon=%f alt=%f Altitude=%f error_a=%f\n",
         RAD_TO_DEG (TrueAnomaly), RAD_TO_DEG(sol_r_lla.Lat),
         RAD_TO_DEG (sol_r_lla.Long), sol_r_lla.Alt*0.001, altitude*0.001,
         RAD_TO_DEG (error)) ;
#endif

   if (error < 0.0)
   {
      delta = b - a ;
      start = a ;
   }
   else
   {
      delta = a - b ;
      start = b ;
   }

   for (count = 0 ; count < MAX_BISECTIONS ; count++)
   {
      TrueAnomaly = start + (delta *= 0.5) ;

      SAT_GetSatPosition (SemiMajorAxis, Eccentricity, Inclination,
                          RAAN, ArgPerigee, TrueAnomaly,
                          &sol_r_eci, &sol_v_eci) ;
  
      altitude = V_Mag (&sol_r_eci) ; 
      E_FromECI (curr_time, &sol_r_eci, &sol_r_lla) ;
      error = sat_r_lla.Lat - sol_r_lla.Lat ;

#if DEBUG
printf ("start=%f delta=%f\n", RAD_TO_DEG(start), RAD_TO_DEG(delta)) ;
printf ("TrueAnomaly = %f lat=%f lon=%f alt=%f Altitude=%f error=%f\n",
         RAD_TO_DEG (TrueAnomaly), RAD_TO_DEG(sol_r_lla.Lat),
         RAD_TO_DEG (sol_r_lla.Long), sol_r_lla.Alt*0.001, altitude*0.001,
         RAD_TO_DEG (error)) ;
#endif
      if (error <= 0.0) 
         start = TrueAnomaly ;

      if (fabs(error) < ERROR_DELTA)
         break ;
   }

   a     = 2.0 * PI  ;
   b     = 0.0       ;

   RAAN = b ;
   SAT_GetSatPosition (SemiMajorAxis, Eccentricity, Inclination,
                       RAAN, ArgPerigee, TrueAnomaly,
                       &sol_r_eci, &sol_v_eci) ;
  
   altitude = V_Mag (&sol_r_eci) ; 
   E_FromECI (curr_time, &sol_r_eci, &sol_r_lla) ;
   error = sat_r_lla.Long - sol_r_lla.Long ;
#if DEBUG
printf ("RAAN = %f lat=%f lon=%f alt=%f Altitude=%f error_b=%f\n",
         RAD_TO_DEG (RAAN), RAD_TO_DEG(sol_r_lla.Lat),
         RAD_TO_DEG (sol_r_lla.Long), sol_r_lla.Alt*0.001, altitude*0.001,
         RAD_TO_DEG (error)) ;
#endif
   
   RAAN = a ;
   SAT_GetSatPosition (SemiMajorAxis, Eccentricity, Inclination,
                       RAAN, ArgPerigee, TrueAnomaly,
                       &sol_r_eci, &sol_v_eci) ;
  
   altitude = V_Mag (&sol_r_eci) ; 
   E_FromECI (curr_time, &sol_r_eci, &sol_r_lla) ;
   error = sat_r_lla.Long - sol_r_lla.Long ;
#if DEBUG
printf ("RAAN = %f lat=%f lon=%f alt=%f Altitude=%f error_a=%f\n",
         RAD_TO_DEG (RAAN), RAD_TO_DEG(sol_r_lla.Lat),
         RAD_TO_DEG (sol_r_lla.Long), sol_r_lla.Alt*0.001, altitude*0.001,
         RAD_TO_DEG (error)) ;
#endif

   if (error < 0.0)
   {
      delta = b - a ;
      start = a ;
   }
   else
   {
      delta = a - b ;
      start = b ;
   }

   for (count = 0 ; count < MAX_BISECTIONS ; count++)
   {
      RAAN = start + (delta *= 0.5) ;

      SAT_GetSatPosition (SemiMajorAxis, Eccentricity, Inclination,
                          RAAN, ArgPerigee, TrueAnomaly,
                          &sol_r_eci, &sol_v_eci) ;
  
      altitude = V_Mag (&sol_r_eci) ; 
      E_FromECI (curr_time, &sol_r_eci, &sol_r_lla) ;
      error = sat_r_lla.Long - sol_r_lla.Long ;

#if DEBUG
printf ("start=%f delta=%f\n", RAD_TO_DEG(start), RAD_TO_DEG(delta)) ;
printf ("RAAN = %f lat=%f lon=%f alt=%f Altitude=%f error=%f\n",
         RAD_TO_DEG (RAAN), RAD_TO_DEG(sol_r_lla.Lat),
         RAD_TO_DEG (sol_r_lla.Long), sol_r_lla.Alt*0.001, altitude*0.001,
         RAD_TO_DEG (error)) ;
#endif
      if (error <= 0.0) 
         start = RAAN ;

      if (fabs(error) < ERROR_DELTA)
         break ;
   }

   apogee  = SemiMajorAxis * (1.0 + Eccentricity) - MEAN_R ;
   perigee = SemiMajorAxis * (1.0 - Eccentricity) - MEAN_R ;

   TrueAnomaly = ANGLE_MAX_PI (TrueAnomaly) ;
   
   if (TrueAnomaly < 0.0)
      TrueAnomaly += (2.0 * M_PI) ;

   printf ("%s %s %s %s %s %s\n",
           "# Apogee_Alt", " Perigee_Alt",
           " Inclination","        RAAN","   Arg_Perigee"," True_Anomaly") ;
   printf ("%s %s %s %s %s %s\n",
           "#-----------", "------------",
           "------------","-------------","-------------","-------------") ;

   printf ("%12.4f %12.4f %12.6f %13.6f %13.6f %13.6f\n",
           apogee, perigee, RAD_TO_DEG (Inclination),
            RAD_TO_DEG (RAAN), RAD_TO_DEG (ArgPerigee),
            RAD_TO_DEG (TrueAnomaly) ) ;

   exit (0) ;
}
