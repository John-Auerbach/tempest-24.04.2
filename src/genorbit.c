/*****************************************************************************/
/*                                                                           */
/*   Module:    genorbit.h                                                   */
/*                                                                           */
/*   Purpose:	Compute tethered system position and velocity using          */
/*		Keplerian orbit propagator.                                  */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   01_Feb_94 NRV   Rewritten.                                   */
/*                                                                           */
/*   RCS:       $Id: genorbit.c,v 1.18 2006/06/16 21:37:55 voronka Exp voronka $                                                         */
/*                                                                           */
/*              $Log: genorbit.c,v 
 *              Revision 1.15  2001/02/05 03:57:45  nestor
 *              Changed decay threshold altitude from 0.0km to 130km.
 *
 *              Revision 1.14  1999/12/08 03:55:21  nestorv
 *              Added solar angle computation.
 *
 * Revision 1.13  1997/08/17  21:10:27  nestorv
 * Added ATM_DRAG_FORCE variables.
 *
 * Revision 1.12  1997/08/09  22:16:23  nestorv
 * Added bare tether thrust forces to RK propagation algorithm.
 *
 * Revision 1.11  1996/10/27  23:05:26  nestorv
 * Added parameter to recompute ephemeri in precision orbit.
 *
 * Revision 1.10  1996/10/27  02:36:04  nestorv
 * Latest in RK4(5) with perturbations.
 *
 * Revision 1.9  1996/10/27  00:12:40  nestorv
 * Fixed some syntax errors.
 *
 * Revision 1.8  1996/10/27  00:09:21  nestorv
 * Added RK4(5) propagator.
 *
 * Revision 1.7  1996/01/30  23:51:42  nestorv
 * Make sure that 0 <= localtime < 24
 *
 * Revision 1.6  1995/11/07  16:45:36  nestorv
 * Changed DAY_FromGMT to Day_FromGMT.
 *
 * Revision 1.5  1995/11/01  05:29:35  nestorv
 * Recompute ephem_time every iteration in case TTP was used.
 *
 * Revision 1.4  1995/10/07  21:59:50  nestorv
 * Changed to support ability to sim with start_time < ephem_time.
 *
 * Revision 1.3  1995/09/28  22:06:04  nestorv
 * Changed && to & in show_debug conditional.
 *
 * Revision 1.2  1995/09/28  21:43:17  nestorv
 * Added && DEBUG_GENORBIT to show_debug conditionals and added RCS info to
 * header.
 *                                                        */
/*                                                                           */
/*****************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "advmath.h"
#include "earth.h"
#include "orbit.h"

#include "types.h"
#include "gmt.h"
#include "tempest.h"
#include "temputil.h"
#include "tether.h"
#include "genorbit.h"
#include "neutdens.h"
#include "global.h"
#include "tss_current.h"
#include "bare_tether.h"

#include "fastorbit.c"
#if _NO_PROTO_

void keplerian_propagate () ;
void precision_propagate () ;

void recompute_orbital_params () ;

double ground_sat_elev () ;

#else

void keplerian_propagate () ;
void precision_propagate () ;

void recompute_orbital_params (double a, double e, double *mm, double *per,
                               double *rapogee, double *rperigee) ;

double ground_sat_elev (double glat, double glon,
                        double slat, double slon, double srmag) ;

#endif

int  first_propagation = TRUE ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void init_orbital_params ()
{
   if (r_circular_alt > 0)
   {
      r_apogee  = r_circular_alt ;
      r_perigee = r_circular_alt ;

      fprintf (stderr, "WARNING: Circular altitude initialized to %f\n",
                       r_circular_alt) ;
   }

   if (r_apogee < r_perigee)
   {
      fprintf (stderr, "\nPerigee altitude is greater than apogee!!\007\n\n");
      exit (-1) ;
   }
                                   /* Compute orbital parameters             */
   semi_major_axis = 0.5 * (r_apogee + r_perigee) ;

   eccentricity = 0.5 * (r_apogee - r_perigee) / semi_major_axis ;
   if (eccentricity < 1e-40)
      eccentricity = 1e-40 ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "INITIAL apo=%f per=%f\n",
                       r_apogee-MEAN_R, r_perigee-MEAN_R) ;
   fprintf (debug_out, "INITIAL a=%f e=%f\n", semi_major_axis, eccentricity) ;
}
#endif

   if (sun_sync_ltan >= 0)
   {
#define DOMEGA_DT_DEG_DAY (0.9856)
#define DOMEGA_DT_RAD_SEC (DOMEGA_DT_DEG_DAY/(24.0*60.0*60.0)*M_PI/180.0)
#define J2                (0.00108263)
#define GM_M3_PER_SEC2    (398600.5e9)

      inclination = acos (DOMEGA_DT_RAD_SEC / ( -1.5*sqrt( GM_M3_PER_SEC2/pow(semi_major_axis,3.0) ) *
                    J2 * pow(MEAN_R / (semi_major_axis*(1-pow(eccentricity,2.0))),2.0))) ;

      fprintf (stderr, "SETTING: Sun Sync orbit with i=%f\n", inclination*180/M_PI) ;
   }

   recompute_orbital_params (semi_major_axis, eccentricity,
                             &mean_motion, &orbit_period, 
                             &r_apogee, &r_perigee) ;

   anomaly_type = strcmp (strupper(anomaly_str), "MEAN") ? 
                     TRUE_ANOMALY : MEAN_ANOMALY ;
   if (anomaly_type == MEAN_ANOMALY)
      true_anomaly = SAT_Kepler (mean_anomaly, eccentricity);

                               /* Get the precession of the RAAN and the arg */
                               /*  perigee due to the sun, moon, and J2      */
   SAT_GetPrecession (semi_major_axis, eccentricity, inclination, 
                      &d_raan, &d_arg_perigee) ;

                                   /* Convert times from GMT format          */
   ephem_time = Day_FromGMT (ephem_year, &ephem_gmt) ;
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "%s = %f (curr_time = %f)\n", Str_FromGMT (&ephem_gmt),
                       ephem_time, curr_time) ;
   fprintf (debug_out, "gs_lat=%lf gs_lon=%lf gs_elev_mask=%lf\n", 
                        gs_lat,gs_lon, gs_elev_mask) ;
}
#endif
}



void recompute_orbital_params (double a, double e, double *mm, double *per,
                               double *rapogee, double *rperigee)
{
                                  /* Compute Orbital Mean Motion (rad/sec)   */
   *mm = sqrt (GM/(a*a*a)) ;
                                  /* Compute period of an orbit (seconds)    */
   *per = 2.0 * PI / *mm ; 
                                  /* Computer radii of apogee and perigee    */
   *rapogee  = a * (1.0 + e) ;
   *rperigee = a * (1.0 - e) ;
                                  /* Compute altitudes of apogee and perigee */ 
   alt_apogee  = *rapogee  - MEAN_R ;
   alt_perigee = *rperigee - MEAN_R ;
}



double ground_sat_elev (double glat, double glon,
                        double slat, double slon, double srmag)
{
/* SMAD pp 110-111 'Earth Geometry Viewed from Space' */
   Earth     E_gs,E_sc ;
   Cartesian C_gs, C_sc ;
   Cartesian C_diff ;
   double vect_angle ;
   double rho ;
   double lambda ;
   double nu ;
   double epsilon ;

   E_gs.Lat  = glat ;
   E_gs.Long = glon ;
   E_gs.Alt  = 0.0 ;
   E_ToECI (curr_time, &E_gs, &C_gs) ;
   E_sc.Lat  = slat ;
   E_sc.Long = slon ; 
   E_sc.Alt  = srmag - MEAN_R ;
   E_ToECI (curr_time, &E_sc, &C_sc) ;
   V_Sub (&C_gs, &C_sc, &C_diff) ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
fprintf (debug_out, "%s g_lat=%f g_lon=%f s_lat=%f s_lon=%f s_r=%f\n",
         Str_FromGMT (&curr_gmt),
         glat, glon, slat, slon, srmag) ;
}
#endif
   rho = asin (MEAN_R / srmag) ;
 
   lambda = acos (sin(slat) * sin(glat) + cos(slat) * cos(glat) *
                  cos(abs(glon-slon)) ) ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
fprintf (debug_out, "lambda=%f PI/2-rho=%f\n", lambda, M_PI_2-rho) ;
}
#endif
   if (lambda > (M_PI_2 - rho))
      return (-1.0/180.0*M_PI) ;

   nu = atan2 (sin(rho)*sin(lambda), 1 - sin(rho)*cos(lambda)) ;

   epsilon = acos (sin(nu) / sin (rho)) ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
fprintf (debug_out, "nu=%f epsilon=%f\n", nu, epsilon) ;
}
#endif

   return (epsilon) ;
}



void compute_sat_position ()
{
   Spherical sun_angle ;
                                   /* Convert times from GMT format          */
   ephem_time = Day_FromGMT (ephem_year, &ephem_gmt) ;

   if (global_sim)
   {
      E_ToECI (curr_time, &sat_r_lla, &sat_r_eci) ;

      sat_r_eci_mag = V_Mag (&sat_r_eci) ;

      V_Mult (&sat_r_eci, (global_alt+MEAN_R)/sat_r_eci_mag , &sat_r_eci) ;
   }
   else
   {
      if (orbit_precise)
      {
         precision_propagate () ;

         if (ephem_from_rv)
         {
            SAT_GetElements (&sat_r_eci, &sat_v_eci,
                      &semi_major_axis, &eccentricity, &inclination, &raan,
                       &arg_perigee, &curr_true_anomaly) ;
 
            recompute_orbital_params (semi_major_axis, eccentricity,
                                      &mean_motion, &orbit_period, 
                                      &r_apogee, &r_perigee) ;
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "a=%f e=%f i=%f raan=%f apogee=%f perigee=%f per=%f\n",
                       semi_major_axis, eccentricity, inclination, raan,
                       r_apogee-MEAN_R, r_perigee-MEAN_R, orbit_period) ;
   fprintf (debug_out, "alt_apogee=%f alt_perigee=%f\n",
                        alt_apogee, alt_perigee) ;
}
#endif
         }
      }
      else
         keplerian_propagate () ;
   }
                                   /* Compute magnitudes of vectors R&V      */
   sat_r_eci_mag = V_Mag (&sat_r_eci) ;
   sat_v_eci_mag = V_Mag (&sat_v_eci) ;

                                   /* Convert Earth Centered Inertial Coords */
                                   /*    to Latitude, Longitude and Altitude */
   E_FromECI (curr_time, &sat_r_eci, &sat_r_lla) ;

                                   /* Has satellite decayed and impacted?    */
   orbit_decayed = (sat_r_lla.Alt <= 130000.0) ;

                                   /* Compute altitude over mean earth       */
   altitude_mer = sat_r_eci_mag - MEAN_R ;
                                   /* Compute satellites RA in ECI           */
   sat_ra = atan2 (sat_r_eci.Y, sat_r_eci.X) ;

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "R=%f V=%f\n", sat_r_eci_mag/1000.0, sat_v_eci_mag) ;
   fprintf (debug_out, "lat=%f  long=%f alt=%f\n", sat_r_lla.Lat, 
                       sat_r_lla.Long, sat_r_lla.Alt) ;
}
#endif

   if (global_sim && g_loc_time != -1.0)
   {
      local_time_h  = g_loc_time ;
      sat_in_shadow = -1         ;
   }
   else
   {                               /* Compute position of the sun in ECI     */
      Sun_GetPosition (curr_time, &sun_r_eci) ;
                                   /* Compute position of the sun in LVLH    */
      E_ECIToLVLH (&sat_r_eci, &sat_v_eci, &sun_r_eci, &sun_pos_lvlh) ;
                                   /* Computer solar angle away from vertical */
      solar_angle =acos (sun_pos_lvlh.Z / V_Mag (&sun_pos_lvlh)) ;

      S_FromCartesian (&sun_pos_lvlh, &sun_angle) ;
      solar_az = sun_angle.Th ;
      solar_el = 0.5* M_PI- sun_angle.Ph ;

                                   /* Compute local time of satellite        */
      E_FromECI (curr_time, &sun_r_eci, &sun_r_lla) ;

      local_time_h = (sat_r_lla.Long - sun_r_lla.Long) * 12.0 / PI + 12.0 ;

      while (local_time_h >= 24.0)
         local_time_h -= 24.0 ;
      while (local_time_h <  0.0)
         local_time_h += 24.0 ;
                                   /* Is satellite in earth's shadown?       */
      sat_in_shadow = (SUN_Eclipsed (&sat_r_eci, &sun_r_eci) == TRUE) ;
   }

#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "sat_long=%f  sun_long=%f  loctime=%f sat_shadow=%d\n",
                       RAD_TO_DEG (sat_r_lla.Long), RAD_TO_DEG (sun_r_lla.Long),
                       local_time_h, sat_in_shadow) ;
   fprintf (debug_out, "c=%lf p=%lf curr_time-pc_time=%lf\n",
                        curr_time, pc_time, curr_time-pc_time) ;
}
#endif

 
   fastorbit ((long int) (pc_time * 86400.0), (long int) fast_in_time0_pc,
              fast_in_period, fast_in_delta_per, fast_in_delta_raan,
              sin(fast_in_inclin),cos(fast_in_inclin),
              fast_in_raan0, fast_in_phi0,
              &fast_out_lat, &fast_out_long) ;

   fast_out_dlat  = fast_out_lat   - sat_r_lla.Lat  ;
   fast_out_dlong = fast_out_long - sat_r_lla.Long ;
   if (fast_out_dlong >= 2.0*M_PI)
      fast_out_dlong -= (2.0*M_PI) ;
   if (fast_out_dlong <= -2.0*M_PI)
      fast_out_dlong += (2.0*M_PI) ;

   gs_sat_elev = ground_sat_elev(gs_lat, gs_lon, 
                                 sat_r_lla.Lat, sat_r_lla.Long, sat_r_eci_mag) ;
   gs_sat_in_view = (gs_sat_elev >= gs_elev_mask)  ;

   fast_out_gs_sat_elev = ground_sat_elev(gs_lat, gs_lon, 
                             fast_out_lat, fast_out_long, 
                             pow(3177465.94739694 * fast_in_period, 2.0/3.0)) ; 
   fast_out_gs_sat_in_view = (fast_out_gs_sat_elev >= gs_elev_mask) ;
}



void keplerian_propagate ()
{
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
   fprintf (debug_out, "%s delta_t=%f delta_anom=%f mean_motion=%f\n",
                        Str_FromGMT (&curr_gmt), (curr_time - ephem_time),
                        true_anomaly, mean_motion) ;
#endif
   if (anomaly_type == TRUE_ANOMALY)
   {
      curr_mean_anomaly = mean_motion * DAY_TO_SEC(curr_time - ephem_time) ;

      curr_true_anomaly = SAT_Kepler (curr_mean_anomaly, eccentricity) + 
                             true_anomaly ;
   }
   else
   {
      curr_mean_anomaly = mean_motion * DAY_TO_SEC(curr_time - ephem_time) +
                             mean_anomaly ;

      curr_true_anomaly = SAT_Kepler (curr_mean_anomaly, eccentricity);
   }

                               /* Returns satellite position and velocity in */
                               /*  ECI coordinates in m and m/s              */
   SAT_GetSatPosition (semi_major_axis, eccentricity, inclination, raan,
                       arg_perigee,curr_true_anomaly,&sat_r_eci,&sat_v_eci) ;

   if (orbit_perturb)
   {                           /* Get the precession of the RAAN and the arg */
                               /*  perigee due to the sun, moon, and J2      */
      SAT_GetPrecession (semi_major_axis, eccentricity, inclination, 
                         &d_raan, &d_arg_perigee) ;
                               /* Precess the raan and arg_perigee per incr  */
      raan        += d_raan        * incr_time ;
      arg_perigee += d_arg_perigee * incr_time ;
   }
}



double rk_alpha [6] = {0.0,0.25,0.375,12.0/13.0,1.0,0.5} ;
double rk_c     [6] = {25.0/216.0,0.0,1408.0/2565.0,2197.0/4104.0,-0.2,0.0};
double rk_c_hat [6] = {16.0/135.0,0.0,6656.0/12825.0,28561.0/56430.0,
                       -0.18,2.0/55.0} ;
double rk_beta[6][5]={{0.0,0.0,0.0,0.0,0.0},
                      {0.25,0.0,0.0,0.0,0.0},
                      {3.0/32.0,9.0/32.0,0.0,0.0,0.0},
                      {1932.0/2197.0,-7200.0/2197.0,7296.0/2197.0,0.0,0.0},
                      {439.0/216.0,-8.0,3680.0/513.0,-845.0/4104.0,0.0},
                      {-8.0/27.0,2.0,-3544.0/2565.0,1859.0/4104.0,-11.0/40.0}};

#define UNIT_ROUNDOFF 2.2e-16    /* Roundoff Error for IEEE 64-bit float     */
#define STEP_TOLERANCE    5.0    /* Tolerance (units of m and m/s??)         */

void precision_propagate ()
{
   Cartesian cur_r         ;
   Cartesian cur_v         ;
   Cartesian r_dot     [6] ;
   Cartesian v_dot     [6] ;
   Cartesian temp_r        ;
   Cartesian temp_v        ;
   double    t0            ;
   double    t1            ;
   double    mu_over_r3    ;
   int       k             ;
   int       lambda        ;
   double    trunc_err [6] ;
   double    te_max        ;
   double    delta_step    ;
   double    h             ; 
                                   /* If at start time, do ephemeri -> R&V   */
   if (first_propagation)
   {
      if (anomaly_type == TRUE_ANOMALY)
      {
         curr_mean_anomaly = mean_motion * DAY_TO_SEC(curr_time - ephem_time) ;

         curr_true_anomaly = SAT_Kepler (curr_mean_anomaly, eccentricity) +
                                true_anomaly ;
      }
      else
      {
         curr_mean_anomaly = mean_motion * DAY_TO_SEC(curr_time - ephem_time) +
                                mean_anomaly ;

         curr_true_anomaly = SAT_Kepler (curr_mean_anomaly, eccentricity);
      }

                               /* Returns satellite position and velocity in */
                               /*  ECI coordinates in m and m/s              */
      SAT_GetSatPosition (semi_major_axis, eccentricity, inclination, raan,
                          arg_perigee,curr_true_anomaly,&sat_r_eci,&sat_v_eci) ;

      first_propagation = FALSE ;
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "First step in RK (keplerian calc)\n") ;
   fprintf (debug_out, "R=%f V=%f\n", sat_r_eci_mag/1000.0, sat_v_eci_mag) ;
   fprintf (debug_out, "lat=%f  long=%f alt=%f\n", sat_r_lla.Lat, 
                       sat_r_lla.Long, sat_r_lla.Alt) ;
}
#endif
   }
   else
   {
      t0 = curr_time ;
      h  = DAY_TO_SEC (incr_time) ;
                                   /* Add manual thrust to perturbation      */
      E_LVLHToECI (&sat_r_eci, &sat_v_eci, &thrust_lvlh, &thrust_accel) ;
      V_Mult (&thrust_accel, 1.0 / system_mass, &thrust_accel) ;
      V_Add (&oblate_earth, &thrust_accel, &total_perturb) ;
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "Earth+Thrust Perturb = %e (m/s^2)\n",
                        V_Mag (&total_perturb));
}
#endif
                                   /* Add iLxB force to perturbation         */
      E_LVLHToECI (&sat_r_eci, &sat_v_eci, &ixb_lvlh, &ilxb_accel) ;
      V_Mult (&ilxb_accel, -1.0 * tether_lib_len / system_mass, &ilxb_accel) ;
      V_Add (&total_perturb, &ilxb_accel, &total_perturb) ;
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "Earth+Thrust+iLxB Perturb = %e (m/s^2)\n",
                        V_Mag (&total_perturb));
}
#endif
                                   /* Add Bare iLxB force to perturbation    */
      E_LVLHToECI (&sat_r_eci, &sat_v_eci, &bare_ixb_lvlh, &b_ilxb_accel) ;
      V_Mult (&b_ilxb_accel, -1.0 * tether_lib_len/system_mass, &b_ilxb_accel) ;
      V_Add (&total_perturb, &b_ilxb_accel, &total_perturb) ;
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out, "Earth+Thrust+iLxB+B_iLxB Perturb = %e (m/s^2)\n",
                        V_Mag (&total_perturb));
}
#endif
                                   /* Add atmospheric drag term              */
      if (orbit_decay)
      {
         V_Mult (&sat_v_eci, -0.5*neutdens_tot_mass*sat_v_eci_mag/bal_coeff,
                 &atmos_drag) ;
         V_Add (&total_perturb, &atmos_drag, &total_perturb) ;
         V_Mult (&atmos_drag, system_mass, &atmos_drag_force) ;
         atmos_drag_force_m = V_Mag (&atmos_drag_force) ;
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   fprintf (debug_out,"Earth+Thrust+iLxB+B_iLxB AtmDrag Perturb = %e (m/s^2)\n",
                        V_Mag (&total_perturb));
   show_cartesian (debug_out, "V  = ",sat_v_eci,1.0," (m/s)\n") ;
   show_cartesian (debug_out, "dV = ",total_perturb,0.001," (/1000)(m/s^2)\n") ;
}
#endif
      }
                                   /* Evaluate right side of difeq 6 times   */
      for (k = 0 ; k <= 5 ; k++)
      {
         t1 = t0 + rk_alpha [k] * h ;

         bcopy (&sat_r_eci, &cur_r, sizeof (Cartesian)) ;
         bcopy (&sat_v_eci, &cur_v, sizeof (Cartesian)) ;
         for (lambda = 0 ; lambda < k ; lambda++)
         {
            V_Mult (&r_dot [lambda], h * rk_beta [k][lambda], &temp_r) ;
            V_Add (&cur_r, &temp_r, &cur_r) ;
            V_Mult (&v_dot [lambda], h * rk_beta [k][lambda], &temp_v) ;
            V_Add (&cur_v, &temp_v, &cur_v) ;
         }

         mu_over_r3 = V_Mag (&cur_r) ;
         mu_over_r3 = -1.0 * GM / (mu_over_r3 * mu_over_r3 * mu_over_r3) ;

         V_Mult (&cur_r, mu_over_r3, &v_dot [k]) ;
         V_Add  (&v_dot [k], &total_perturb, &v_dot [k]) ;
         bcopy (&cur_v, &r_dot [k], sizeof (Cartesian)) ;
      }
                                   /* Compute truncation error vector        */
      for (k = 0 ; k < 6 ; k++)
         trunc_err[k] = 0.0 ;
      for (k = 0 ; k <= 5 ; k++)
      {
         trunc_err[0] += h * (rk_c[k] - rk_c_hat [k]) * r_dot[k].X ;
         trunc_err[1] += h * (rk_c[k] - rk_c_hat [k]) * r_dot[k].Y ;
         trunc_err[2] += h * (rk_c[k] - rk_c_hat [k]) * r_dot[k].Z ;
         trunc_err[3] += h * (rk_c[k] - rk_c_hat [k]) * v_dot[k].X ;
         trunc_err[4] += h * (rk_c[k] - rk_c_hat [k]) * v_dot[k].Y ;
         trunc_err[5] += h * (rk_c[k] - rk_c_hat [k]) * v_dot[k].Z ;
      }

                                   /* Find maximum component of TE vector    */
      te_max = trunc_err [0] ;
      for (k = 0 ; k < 6 ; k++)
         if (trunc_err [k] > te_max)
            te_max = trunc_err [k] ;
                                   /* Check to see if step accepted or not   */
      if (te_max > STEP_TOLERANCE)
      {
         fprintf (stderr, "WARNING: Step in RK4(5) rejected!!\n") ;
         fprintf (stderr, "         Truncation Error (%f) > Tolerance (%f)\n",
                          te_max, STEP_TOLERANCE) ;

         delta_step = pow ( (STEP_TOLERANCE / (te_max + UNIT_ROUNDOFF)), 0.2) ;

         fprintf (stderr, "         New step size h = h * MAX (%f, 0.1)\n",
                          delta_step) ;
      }
#if DEBUG
if (show_debug & DEBUG_GENORBIT)
{
   delta_step = pow ( (STEP_TOLERANCE / (te_max + UNIT_ROUNDOFF)), 0.2) ;

   fprintf (debug_out, "Truncation Error Max = %f\n", te_max) ;
   fprintf (debug_out, "Delta Step = %f\n", delta_step) ;
}
#endif

                                   /* Compute final solution at t = t0 + h   */
      for (k = 0 ; k <= 5 ; k++)
      {
         V_Mult (&r_dot [k], rk_c_hat [k] * h, &temp_r) ;
         V_Add  (&sat_r_eci, &temp_r, &sat_r_eci) ;

         V_Mult (&v_dot [k], rk_c_hat [k] * h, &temp_v) ;
         V_Add  (&sat_v_eci, &temp_v, &sat_v_eci) ;
      }
   }
}
