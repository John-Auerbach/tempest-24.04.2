/*****************************************************************************/
/*                                                                           */
/*   Module:    fastorbit.c                                                  */
/*                                                                           */
/*   Purpose:	Compute tethered system position and velocity using          */
/*		Keplerian orbit propagator.                                  */
/*                                                                           */
/*   Inputs:    long int current_time                                        */
/*                     Time in seconds from 00:00 01-Jan-1970                */
/*              long int ephemeris_time                                      */
/*                     Time of ephemeris in seconds from 00:00 01-Jan-1970   */
/*              double orbital_period                                        */
/*                     Orbital period in seconds                             */
/*              double delta_orb_per                                         */
/*                     Rate of change of Orbital period in seconds per sec   */
/*              double delta_raan                                            */
/*                     Precession rate of the RAAN in radians/sec            */
/*              double sin_orbital_inclin                                    */
/*                     Sine of the orbital inclination                       */
/*              double cos_orbital_inclin                                    */
/*                     Cosine of the orbital inclination                     */
/*              double raan0                                                 */
/*                     The RAAN at ephemeris time (rad)                      */
/*              double phi0                                                  */
/*                     The phi (approx mean anomaly) at ephemeris time (rad) */
/*                                                                           */
/*   Outputs:   double lat_out                                               */
/*                     The approximate object latitude (radians)             */
/*              double long_out                                              */
/*                     The approximate object longitude (radians)            */
/*                                                                           */
/*   Uses:      The following functions from the standard math library:      */
/*              science library.                                             */
/*                                                                           */
/*   History:   04_Apr_06 NRV   Written.                                     */
/*                                                                           */
/*   RCS:       $Id: fastorbit.c,v 1.9 2006/06/16 21:37:22 voronka Exp voronka $                                                        */
/*                                                                           */
/*              $Log: fastorbit.c,v $
 *              Revision 1.9  2006/06/16 21:37:22  voronka
 *              Changed current time and ephemeris time to be long ints
 *
 *              Revision 1.8  2006/06/01 19:17:05  voronka
 *              Added num_orbits to debugging output
 *
 *              Revision 1.7  2006/06/01 18:08:19  voronka
 *              Commented out tempest stuff.
 *
 *              Revision 1.6  2006/06/01 16:01:24  voronka
 *              Added some documentation
 *
 *              Revision 1.5  2006/04/28 20:06:02  voronka
 *              Fixed extra debugging message output.
 *
 *              Revision 1.4  2006/04/06 01:07:46  voronka
 *              Updated to make it a bit faster.
 *
 *              Revision 1.3  2006/04/05 19:20:12  voronka
 *              Works!
 *
 *              Revision 1.2  2006/04/05 08:07:29  voronka
 *              Works other than atan quadrant needs fixing.
 *
 *              Revision 1.1  2006/04/05 02:33:51  voronka
 *              Initial revision
 *                                                       */
/*                                                                           */
/*****************************************************************************/

#ifndef __MATH__
#include <math.h>
#endif

#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288   /* pi */
#endif

#ifndef M_PI_2
#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2 */
#endif

#define M_TWO_PI  		6.28318530717958647692528676655900576
#define M_3_PI_2		4.71238898038468985769396507491925432

                     /* Mean solar day    - 24 hours    - 86400   secs   */
                     /* Mean sidereal day - 23h56'04.1" - 86164.1 secs   */
#define SOLAR_SIDEREAL 		1.0027379093

/* The conversion formula was copied from the "Explanatory
   Supplement To The Ephemeris," 1961, pages 84-85. */

double long_of_raan0 (long int current_time)
                      /* Current time in seconds since 00:00 01-Jan-1970 */ 
{
   double Time       ;
   double Time_floor ;
   double GMST       ;   /* Greenwich Mean Sideral Time */
   double T          ;   /* Greenwich Centuries since Jan 1, 1900 */
   double T2         ;
   double raan_long  ;

                                        /* Convert time from seconds to days */
   Time        = ((double) current_time) / 86400.0 ;
                                /* Correct for input being referenced to 1970 */
   Time       += 25568.0 ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
   fprintf (debug_out, "Time=%lf ", Time) ;
#endif

   Time_floor  = floor(Time) ;
   T           = (Time_floor - 0.5) / 36525.0 ;
   T2          = T * T ;
   Time       -= Time_floor ;

/* GMST  = (6.6460656 + 2400.051262 * T + 2.581E-5 * T * T) / 24.0 ; */
   GMST  = 6.6460656 ;
   GMST += 2400.051262 * T ;
   GMST += 2.581E-5 * T2 ;
   GMST /= 24.0 ;

/* raan_long  = M_TWO_PI * (Time * SOLAR_SIDEREAL + GMST - floor (GMST)) ; */
   raan_long  = Time * SOLAR_SIDEREAL ;
   raan_long += GMST ;
   raan_long -= floor (GMST) ;
   raan_long *= M_TWO_PI ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
   fprintf (debug_out, "Time=%lf T=%lf GMST=%lf raan_long=%lf\n", 
                        Time, T, GMST, raan_long) ;
#endif

   return (raan_long) ;
}



void fastorbit (long int current_time, 
                long int ephemeris_time,
                double orbital_period,
                double delta_orb_per,
                double delta_raan,
                double sin_orbital_inclin,
                double cos_orbital_inclin,
                double raan0,
                double phi0,
                double *lat_out, 
                double *long_out)
{
   double delta_t ;
   double comp_orb_per ;
   double num_orbits ;
   double frac_orbit ;
   double raan ;
   double phi ;
   double ra ;
   double dec ;
   double longitude ;
   double raan_long ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "c=%ld e=%ld\n", current_time, ephemeris_time) ;
   fprintf (debug_out, "per=%lf draan=%lf raan0=%lf phi0=%lf\n", 
                        orbital_period, delta_raan, raan0, phi0) ;
}
#endif
                                    /* Compute time since ephemeris (sec)    */
   delta_t = (double) (current_time - ephemeris_time) ;
                                    /* Compute current orbital period (sec)  */
   comp_orb_per = orbital_period + (delta_t * delta_orb_per) ;
                                    /* Compute fraction of current orbit     */
   num_orbits = delta_t / comp_orb_per ;
   frac_orbit = num_orbits - floor (num_orbits) ;
                                    /* Compute current RAAN                  */
   raan = raan0 + delta_raan * delta_t ;
                                    /* Compute phi (basically mean anomaly)  */
   phi  = phi0 + frac_orbit * M_TWO_PI ;

#if 0
#warning ***************
#warning ***************
#warning ***************
#warning this next function call differs from flight software
#warning ***************
#warning ***************
#warning ***************

   phi = fmod (phi, M_TWO_PI) ;

#endif

#if DEBUG
   fast_out_period = comp_orb_per ;
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "dt=%lf no=%lf f=%lf raan=%lf phi=%lf\n", 
            delta_t, num_orbits, frac_orbit, raan, phi) ;
}
#endif
                                    /* Compute RA (kinda longitude)          */
                                    /* Spherical geometry - however result   */
                                    /*    in +/- 90 deg range only!?         */
   ra   = atan (tan(phi) * cos_orbital_inclin) ;

                                    /* Correct output range of arctangent    */

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
   fprintf (debug_out, "phi=%lf ra=%lf ",  phi, ra) ;
#endif

#warning In the following comparisons, the '=' in the left comparison will
#warning cause glitches in the OS_X environment but works fine in the PIC!

   if (cos_orbital_inclin >= 0.0)    /* Orbit prograde */
   {
      if      ((phi >= M_PI_2) && (phi <= M_PI))
         ra += M_PI ;
      else if ((phi >= M_PI  ) && (phi <= M_3_PI_2))
         ra -= M_PI ;
   }
   else                             /* Orbit retrograde */
   {
      if      ((phi >= M_PI_2) && (phi <= M_PI))
         ra -= M_PI ;
      else if ((phi >= M_PI  ) && (phi <= M_3_PI_2))
         ra += M_PI ;
   }
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
   fprintf (debug_out, "cos(i)=%lf ra_fixed=%lf\n", cos_orbital_inclin, ra) ;
#endif
                                   /* Add RAAN to atan component of RA      */
   ra += raan ;
                                    /* Compute Declination (latitude!)       */
   dec  = asin (sin(phi) * sin_orbital_inclin) ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
   fprintf (debug_out, "ra=%lf dec=%lf\n", ra, dec) ;
#endif

                                    /* Get long of RAAN=0 at current time    */
   raan_long = long_of_raan0 (current_time) ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
   fprintf (debug_out, "raan_long=%lf\n", raan_long) ;
#endif
                                   /* Compute longitude as +/- PI            */
   longitude = fmod (ra - raan_long, M_TWO_PI) ;
   if (longitude < 0.0)
      longitude += M_TWO_PI ;
   if (longitude >= M_PI)
      longitude -= M_TWO_PI ;

   *lat_out  = dec ; 
   *long_out = longitude ;

/* The following is for debugging purpose with TEMPEST */
#if 0
   fast_out_delta_t = delta_t   ;

   ra = fmod (ra, M_TWO_PI) ;
   if (ra > M_PI)
      ra -= M_TWO_PI ;
   fast_out_ra      = ra        ;

   fast_out_dec     = dec       ;
#endif
}
