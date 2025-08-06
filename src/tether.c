/*****************************************************************************/
/*                                                                           */
/*   Module:    tether.c                                                     */
/*                                                                           */
/*   Purpose:	This module computes zeroth-order tether dynamics using a    */
/*		a simple harmonic model for tether oscillations.             */
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
/*   RCS:       $Id: tether.c,v 1.13 2005/06/07 02:08:56 voronka Exp $                                                         */
/*                                                                           */
/*              $Log: tether.c,v 
 *              Revision 1.11  2000/11/03 00:27:56  nestor
 *              More temp stuff.
 *
 * Revision 1.10  1999/12/08  05:15:05  nestorv
 * Commented out tether temp stuff.
 *
 * Revision 1.9  1999/12/08  03:54:16  nestorv
 * Added temperature stuff.
 *
 * Revision 1.8  1996/02/26  12:25:25  sets
 * Check to see if |len| > 0.0
 *
 * Revision 1.7  1996/02/18  06:38:52  nestorv
 * Changed dply time from MET to GMT.
 *
 * Revision 1.6  1996/02/06  14:39:15  nestorv
 * Edited for style.
 *
 * Revision 1.5  1995/11/07  16:55:21  nestorv
 * Changed DAY_FromGMT to Day_FromGMT.
 *
 * Revision 1.4  1995/11/01  05:59:21  nestorv
 * Correctly compute tether_gg_lvlh vector now.
 *
 * Revision 1.3  1995/11/01  05:36:16  nestorv
 * Added polynomial for tether length calculations.
 *
 * Revision 1.2  1995/10/10  19:51:34  nestorv
 * Added time0 for libration to TETHER module.
 *
 * Revision 1.1  1995/09/28  22:11:03  nestorv
 * Initial revision
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
#include "genorbit.h"
#include "solarmag.h"
#include "tether.h"
#include "emf.h"
#include "bare_tether.h"

#define STEPH_BOLTZ	5.67e-8   /* Stephan-Boltzmann constant in units of  */
                                  /*    W/m^2 K^4                            */
void init_libration_params () 
{
                                  /* Define gravity gradient oriented tether */
   tether_gg_lvlh.X = 0.0 ;
   tether_gg_lvlh.Y = 0.0 ;
   tether_gg_lvlh.Z = tether_end ;

                        /* Orbital periods for tether libration              */
   ip_lib_period = SEC_TO_DAY (orbit_period / sqrt (3.0)) ;
   op_lib_period = SEC_TO_DAY (orbit_period *       0.5 ) ;
   ra_lib_period = SEC_TO_DAY (orbit_period / sqrt (3.0)) ;

   lib_time  = Day_FromGMT (lib_year,  &lib_gmt ) ;
   dply_time = Day_FromGMT (dply_year, &dply_gmt) ;

   tether_len     = length_poly[0] ;

   if (tether_cond_radius <= 0.0)
   {
      fprintf (stderr, "WARNING:  Tether conductor radius <= 0\007!\n") ;
      fprintf (stderr, "WARNING:  Setting conductor radius to 1mm\007!\n") ;
      tether_cond_radius = 0.001 ;
   }

   if (tether_resistivity <= 0.0)
   {
      fprintf (stderr, "WARNING:  Tether resistivity <= 0\007!\n") ;
      if (dRt > 0.0)
      {
         tether_resistivity = dRt * 100.0 *
                               (M_PI * tether_cond_radius*tether_cond_radius) ;
         fprintf (stderr,
            "WARNING:  Tether resistivity set to %f using dRt=%f ohms/meter\n",
            tether_resistivity, dRt) ;
      }
   }

   if (t_bare_emissivity <= 0.0)
   {
      fprintf (stderr, "WARNING: Bare tether emissivity <= 0.0\007!\n") ;
      fprintf (stderr, "WARNING: Setting bare tether emissivity to 0.034 (Al 6061-T6)\007!\n") ;
      t_bare_emissivity = 0.034 ;
   }

   if (t_bare_absorbtivity <= 0.0)
   {
      fprintf (stderr, "WARNING: Bare tether absorbtivity <= 0.0\007!\n") ;
      fprintf (stderr, "WARNING: Setting bare tether absorbtivity to 0.2 (Al 6061-T6)\007!\n") ;
      t_bare_absorbtivity = 0.2 ;
   }

   if (t_insul_emissivity <= 0.0)
   {
      fprintf (stderr, "WARNING: Insulated tether emissivity <= 0.0\007!\n") ;
      fprintf (stderr, "WARNING: Setting insulated tether emissivity to 0.66 (Silv Teflon)\007!\n") ;
      t_insul_emissivity = 0.66 ;
   }
   if (t_insul_absorbtivity <= 0.0)
   {
      fprintf (stderr, "WARNING: Insulated tether absorbtivity <= 0.0\007!\n") ;
      fprintf (stderr, "WARNING: Setting insulated tether absorbtivity to 0.08 (Silv Teflon)\007!\n") ;
      t_insul_absorbtivity = 0.08 ;
   }
   tether_resist = compute_tether_res  (tether_resist_temp) ;
}



void compute_tether_libration () 
{
   Cartesian tether_lib_eci  ;
   Cartesian delta_eci_start ;
   Cartesian delta_eci_end   ;
   double    dt              ;
   int       count           ;
   double    prev_resist     ;

   lib_time  = Day_FromGMT (lib_year,  &lib_gmt ) ;
   dply_time = Day_FromGMT (dply_year, &dply_gmt) ;

   dt = DAY_TO_SEC (curr_time - dply_time) ;
#if DEBUG
if (show_debug & DEBUG_TETHER)
{
  fprintf (debug_out, "curr_time =%f dply_time=%f\n",curr_time, dply_time) ;
  fprintf (debug_out, "%s => dt=%f\n", Str_FromGMT (&dply_gmt), dt) ;
}
#endif
   tether_len     = length_poly[0] + (length_poly[1] + (length_poly[2] +
                                      length_poly[3] * dt) * dt) * dt ;

   tether_l_dot   = length_poly[1] + (2.0 * length_poly[2] +
                                      3.0 * length_poly[3] * dt) * dt ;

                                /* Define gravity gradient oriented tether   */
   tether_gg_lvlh.Z = tether_len ;
                               /* Compute tether libration magnitudes        */
   tether_lib_lvlh.X = libration_mag_ip *
                     sin (2.0*PI * (curr_time - lib_time)
                                            / ip_lib_period + ip_lib_phase0);
   tether_lib_lvlh.Y = libration_mag_op *
                     sin (2.0*PI * (curr_time - lib_time)
                                            / op_lib_period + op_lib_phase0);

                               /* Compute scaling factor for elongation lib  */
   cur_ra_elong = libration_mag_ra * 0.01 *
                     sin (2.0*PI * (curr_time - lib_time)
                                            / ra_lib_period + ra_lib_phase0);

                               /* Find length (& orientation) of gg tether   */
   tether_lib_len = tether_len * (1.0 + cur_ra_elong) ;

   tether_lib_lvlh.Z = sqrt ( tether_lib_len    * tether_lib_len    -
                              tether_lib_lvlh.X * tether_lib_lvlh.X -
                              tether_lib_lvlh.Y * tether_lib_lvlh.Y   ) ;

                               /* Compute tether libration angles            */
   cur_ip_lib = atan2 (tether_lib_lvlh.X, tether_lib_lvlh.Z) ;
   cur_op_lib = atan2 (tether_lib_lvlh.Y, tether_lib_lvlh.Z) ;

                               /* Compute position of tether end and start   */
   E_LVLHToECI (&sat_r_eci, &sat_v_eci, &tether_lib_lvlh, &tether_lib_eci) ;

   if (fabs(tether_lib_len) > 0.0)
   {
      V_Mult (&tether_lib_eci, tether_start/tether_lib_len, &delta_eci_start) ;
      V_Mult (&tether_lib_eci, tether_end  /tether_lib_len, &delta_eci_end  ) ;
   }
   else
   {
      V_Mult (&tether_lib_eci, 0.0, &delta_eci_start) ;
      V_Mult (&tether_lib_eci, 0.0, &delta_eci_end  ) ;
   }
   V_Add  (&sat_r_eci, &delta_eci_start, &tether_start_r_eci) ;
   V_Add  (&sat_r_eci, &delta_eci_end  , &tether_end_r_eci  ) ;

   E_FromECI (curr_time, &tether_start_r_eci, &tether_start_r_lla) ;
   E_FromECI (curr_time, &tether_end_r_eci  , &tether_end_r_lla  ) ;

/*
#if DEBUG
if (show_debug & DEBUG_TETHER)
   fprintf (debug_out, "iter --- tether_resist = %f\n", tether_resist) ;
#endif
   count = 0 ; 
   do
   {
      prev_resist = tether_resist ;

      tether_temp   = compute_tether_temp (vxb_l/tether_resist) ;
      tether_resist = compute_tether_res  (tether_temp) ;
#if DEBUG
if (show_debug & DEBUG_TETHER)
{
   fprintf (debug_out, "iter %03d tether_temp = %f\n", count, tether_temp) ;
   fprintf (debug_out, "iter %03d tether_resist = %f\n", count, tether_resist) ;
}
#endif
   }
   while (fabs (prev_resist-tether_resist) > RES_CONV_THRESH &&
          ++count < MAX_TEMP_ITER) ;
*/
#if DEBUG
if (show_debug & DEBUG_TETHER)
{
   show_cartesian (debug_out, "L(lvlh) = ",tether_lib_lvlh , 1.0, "\n") ;

   show_cartesian (debug_out, "sat 0 = ",sat_r_eci         , 1.0, "\n") ;
   show_cartesian (debug_out, "start = ",tether_start_r_eci, 1.0, "\n") ;
   show_cartesian (debug_out, "end   = ",tether_end_r_eci  , 1.0, "\n") ;

   show_earth     (debug_out, "sat 0 = ",sat_r_lla         , "\n") ;
   show_earth     (debug_out, "start = ",tether_start_r_lla, "\n") ;
   show_earth     (debug_out, "end   = ",tether_end_r_lla  , "\n") ;
}
#endif
}



double compute_tether_temp (double current)
{
   double proj_i_tether_area ;
   double proj_b_tether_area ;
   double tot_i_tether_area ;
   double tot_b_tether_area ;
   double q_i_sun       ;
   double q_i_earth_ir  ;
   double q_i_albedo    ;
   double q_i_radiated  ;
   double q_b_sun       ;
   double q_b_earth_ir  ;
   double q_b_albedo    ;
   double q_b_radiated  ;
   double q_internal    ;
   double rho           ; /* Angular radius of the earth */
   double computed_temp ;

   tot_b_tether_area = 2.0 * PI * tether_cond_radius  *
                       fabs (t_bare_start - t_bare_end) ;
   tot_i_tether_area = 2.0 * PI * tether_outer_radius *
                       fabs (tether_len - (t_bare_start - t_bare_end)) ;

   proj_b_tether_area = 2.0 * tether_cond_radius *
                        fabs (t_bare_start - t_bare_end) ;
   proj_i_tether_area = 2.0 * tether_outer_radius *
                        fabs (tether_len - (t_bare_start - t_bare_end)) ;

   if (sat_in_shadow)
   {
      q_i_sun    = 0.0 ;
      q_b_sun    = 0.0 ;
      q_i_albedo = 0.0 ;
      q_b_albedo = 0.0 ;
   }
   else
   {
      q_i_sun = solar_constant * proj_i_tether_area * t_insul_absorbtivity *
                sin (solar_angle) ;
      q_b_sun = solar_constant * proj_b_tether_area * t_bare_absorbtivity  *
                sin (solar_angle) ;

      rho = asin (MEAN_R / sat_r_eci_mag) ;

      q_i_albedo = solar_constant * earth_albedo * proj_i_tether_area *
                   t_insul_absorbtivity * (0.664 + 0.521*rho - 0.203*rho*rho) *
                   (MEAN_R / sat_r_eci_mag) * (MEAN_R / sat_r_eci_mag) ;
      q_b_albedo = solar_constant * earth_albedo * proj_b_tether_area *
                   t_bare_absorbtivity * (0.664 + 0.521*rho - 0.203*rho*rho) *
                   (MEAN_R / sat_r_eci_mag) * (MEAN_R / sat_r_eci_mag) ;
   }

   q_i_earth_ir = t_insul_emissivity * proj_i_tether_area * 
                     earth_ir_flux * (MEAN_R / sat_r_eci_mag) *
                                     (MEAN_R / sat_r_eci_mag)  ;
   q_b_earth_ir = t_bare_emissivity  * proj_b_tether_area * 
                     earth_ir_flux * (MEAN_R / sat_r_eci_mag) *
                                     (MEAN_R / sat_r_eci_mag)  ;

   q_internal = current * current * tether_resist ;

   computed_temp = pow ((q_i_sun + q_i_albedo + q_i_earth_ir +
                         q_b_sun + q_b_albedo + q_b_earth_ir + q_internal) /
                         ((tot_i_tether_area * t_insul_emissivity +
                           tot_b_tether_area * t_bare_emissivity) *
                           STEPH_BOLTZ), 0.25) ;

   computed_temp = 273.15 + 20.0 ;

#if DEBUG
if (show_debug & DEBUG_TETHER)
{
   fprintf (debug_out, " insul_area=%f bare_area=%f sat_in_shadow=%d\n",
            tot_i_tether_area, tot_b_tether_area, sat_in_shadow) ;
   fprintf (debug_out, " q_i_sun=%f q_b_sun=%f\n", q_i_sun, q_b_sun) ;
   fprintf (debug_out, " q_i_earth_ir=%f q_b_earth_ir=%f\n",
            q_i_earth_ir, q_b_earth_ir) ;
   fprintf (debug_out, " q_i_albedo=%f q_b_albedo=%f\n",
            q_i_albedo, q_b_albedo) ;
   fprintf (debug_out, " q_internal=%f\n", q_internal) ;
   fprintf (debug_out, "tether_temp=%f\n", computed_temp - 273.15) ;
}
#endif
    
    return (computed_temp - 273.15) ;
}

double compute_tether_res (double temp)
{
#if DEBUG
if (show_debug & DEBUG_TETHER)
{
   fprintf (debug_out, "tether_res (T=%f, L=%f, dR=%f, tc=%f, R=%f) = %f\n",
           temp, tether_len, tether_resistivity,
           tether_temp_coeff, tether_cond_radius,
           tether_resistivity * 0.01 * fabs (tether_len) /
           (M_PI * tether_cond_radius * tether_cond_radius) *
           (1.0 + tether_temp_coeff * (temp - tether_resist_temp))) ;
}
#endif
   return (tether_resistivity * 0.01 * fabs (tether_len) /
           (M_PI * tether_cond_radius * tether_cond_radius) *
           (1.0 + tether_temp_coeff * (temp - tether_resist_temp))) ;
}
