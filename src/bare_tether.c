/*****************************************************************************/
/*                                                                           */
/*   Module:    bare_tether.c                                                */
/*                                                                           */
/*   Purpose:	Compute current collected with a bare or partially bare      */
/*              conducting tether.                                           */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   05_May_97 NRV   Written.                                     */
/*                                                                           */
/*   RCS:	$Id: bare_tether.c,v 1.29 2008/04/17 09:54:40 nrv Exp $                                                        */
/*                                                                           *
 *              $Log: bare_tether.c,v 
 *              Revision 1.25  2005/05/26 21:06:47  voronk
 *              *** empty log message **
 *
 *              Revision 1.24  2001/01/03 14:15:48  nestorv
 *              Corrected setting off v_cathode_out.
 *
 *              Revision 1.23  2000/12/20 03:32:52  nestorv
 *              Removed extra line.
 *
 *              Revision 1.22  2000/12/20 03:23:17  nestorv
 *              Changed PM i0 calculation.
 *
 *              Revision 1.21  2000/12/19 05:34:11  nestorv
 *              Fixed PM for convergence, and NaN in V0_loc calculation.
 *
 *              Revision 1.20  2000/12/19 03:45:30  nestorv
 *              Fixed PM model due to errors when v goes negative.
 *
 *              Revision 1.19  2000/11/03 01:57:27  nestorv
 *              Fixed 0V determination code.
 *
 *              Revision 1.18  2000/10/25 19:37:17  nestorv
 *              Added V0_POSITION output.
 *
 *              Revision 1.17  2000/09/27 03:47:02  nestorv
 *              Added up & down contactor models
 *
 *              Revision 1.16  2000/08/15 02:16:26  nestorv
 *              Some updates, including Parker-Murphy addition.
 *
 * Revision 1.15  1999/12/08  05:14:28  nestorv
 * Bare tether and R(temp) converging together - don't work.
 *
 * Revision 1.14  1999/09/30  04:47:13  nestorv
 * Added altitude control mode.
 *
 * Revision 1.13  1999/09/30  04:14:55  nestorv
 * Check in after many updates
 *
 * Revision 1.12  1999/04/01  19:41:10  nestorv
 * Added vector based control mode.
 *
 * Revision 1.11  1999/03/10  02:40:59  nestorv
 * Fixed constant power model.
 *
 * Revision 1.10  1999/02/09  02:50:11  nestorv
 * Added constant power supply model.
 *
 * Revision 1.9  1999/01/27  04:56:28  nestorv
 * After code walkthrough with Brian G.
 *
 * Revision 1.8  1999/01/17  18:02:46  nestorv
 * fixed downward deploy
 *
 * Revision 1.7  1997/08/17  21:41:19  nestorv
 * Corrected iLxB computation and added output in Newtons
 *
 * Revision 1.6  1997/08/09  22:18:27  nestorv
 * Corrected bugs.
 *
 * Revision 1.5  1997/07/15  21:21:30  nestorv
 * FINALLY WORKS!!!
 *
 * Revision 1.4  1997/07/13  22:11:50  nestorv
 * Save.
 *
 * Revision 1.3  1997/06/11  18:06:25  nestorv
 * Seems to work.
 *
 * Revision 1.2  1997/06/06  00:43:28  nestorv
 * code that works but needs cleaning up.,
 *
 * Revision 1.1  1997/05/09  15:36:31  nestorv
 * Initial revision
 *
 *                                                       */
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
#include "bfield.h"
#include "emf.h"
#include "iri.h"
#include "bare_tether.h"

#include "hcpc.h"

#define HCPC_CONV_MIN_VOLT   -200.0
#define HCPC_CONV_MAX_VOLT 120000.0

#define MAX_BISECTIONS 50

/* double i_bare [1000] ; */

double upward_deploy   (double d_va, double d_vp, double d_len, int c_ixb) ;

double up_contactor_model (double i) ;

double downward_deploy (double d_va, double d_vp, double d_len, int c_ixb) ;

double down_contactor_model (double i) ;
double down_contactor_model_iofv (double v) ;

double power_supply_model (double i) ;

#define MAX_ITER_NR 25

double compute_fea_voltage (double current) ;

double compute_parker_murphy_v_of_i (double current) ;

double compute_parker_murphy_i_of_v (double v) ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void init_bare_tether_params ()
{
   int count ;

   if (bare_cathode_bias > 0.0)
      fprintf(stderr,"\n%s\007\n",
              "BAD: CATHODE_BIAS > 0 is BAD for downward deploy");

   if (bare_tether_seg > 0 && bare_tether_seg % 2 != 0)
   {
      bare_tether_seg++ ;
      fprintf (stderr,
               "WARNING: Number of integration segments needs to be even!!\n") ;
      fprintf (stderr,
               "         Increased number of segments to %d\n",bare_tether_seg);
   }

   if (V_Mag (&control_lvlh) < 1.0)
      fprintf (stderr,
         "WARNING: Magnitude of thrust control vector is less than 1.0!\007\n");

                                   /* Convert LLA at time 0 to ECI position  */
   E_ToECI (ephem_time, &pos_cntrl_lla, &pos_cntrl_eci) ;
   pos_cntrl_eci_mag = V_Mag (&pos_cntrl_eci) ;
   V_Mult (&pos_cntrl_eci, (MEAN_R+pos_cntrl_lla.Alt)/pos_cntrl_eci_mag,
           &pos_cntrl_eci) ;
   pos_cntrl_eci_mag = V_Mag (&pos_cntrl_eci) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "start curr_time=%f\n", curr_time) ;
   show_cartesian (debug_out, "start EtoECI =",pos_cntrl_eci,1.0,"\n") ;
   fprintf (debug_out, "pos_cntrl_eci_mag=%f ==> alt=%f\n", pos_cntrl_eci_mag,
                        pos_cntrl_eci_mag-MEAN_R) ;
   fprintf (debug_out, "di_dy_scale=%f di_dy_beta=%f\n",
                        di_dy_scale, di_dy_beta) ;
}
#endif
}



void compute_bare_tether ()
{
   int    count              ;     /* Counter for loops                      */
   double delta_va           ;
   double a, b, dx           ;
   double delta_vp           ;     /* */
   double delta_length       ;     /* */
   double error        = 0.0 ;     /* */
   double prev_error   = 0.0 ;
   double error_a      = 0.0 ;     /* */
   double error_b      = 0.1 ;     /* */
   double error_thresh = 0.1 ;     /* */
   Cartesian  target_pos     ;
   double delta_pos          ;


#define IV_CURVE 0

#if IV_CURVE
   char   bare_debug_fname [80] ;
   FILE  *bare_debug = NULL ;
   double iv_volt ;
   double bdv ;

   sprintf (bare_debug_fname,"%s.%012.5f",simul_fname, curr_time) ;
   fprintf (debug_out, "***** [%s] *****\n", bare_debug_fname) ;
   if ((bare_debug = fopen (bare_debug_fname, "w")) == NULL)
   {
      fprintf (stderr, "ERROR: Fopen for %s failed\n", bare_debug_fname) ;
      bare_debug = stderr ;
   }
   fprintf (bare_debug, "Voltage Imax Iave Iend\n") ;
   
   bdv = bare_down_voltage ;

   for (iv_volt = 0.0 ; iv_volt <= bdv ; iv_volt += (0.01*bdv))
   {
      bare_down_voltage = iv_volt ;
#endif   
                                   /* Compute average induced e-field        */
   delta_vp     = vxb_l      / (double)(bare_tether_seg) ;
   delta_length = fabs (tether_lib_len / (double)(bare_tether_seg)) ;

   tether_temp   = compute_tether_temp (vxb_l / (tether_lib_len * dRt)) ;
   tether_resist = compute_tether_res  (tether_temp) ;
   dRt           = tether_resist / fabs (tether_lib_len) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
   fprintf (debug_out, "  === iter 00 R_tether = %f\n", tether_resist) ;
#endif

   if (bare_contact_model_down == BARE)
      error_thresh = 0.00001 * fabs (vxb_l / tether_resist) ;
   else
      error_thresh = fabs (0.0001 * vxb_l) ;

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "emf=%f l=%f threshold=%f ne=%13.5e Te=%13.5f B=%13.5e seg=%d\n",
                        vxb_l, tether_lib_len, error_thresh, iri_elect_density,
                        iri_elect_temp, bfield_loc_mag, bare_tether_seg) ;
   fprintf (debug_out, "tether_lib_len=%f\n", tether_lib_len) ;
   fprintf (debug_out, "delta_vp=%f delta_length=%f\n",
                        delta_vp, delta_length) ;
}
#endif

   if (tether_lib_len > 0.0)        /* Upward deployment of tether            */
   {
      if (bare_contact_model_down == BARE)
         delta_vp = fabs (delta_vp) ;

      a = bare_anode_bias ;
      error_a = upward_deploy (a, delta_vp, delta_length, TRUE) ;

      if (bare_contact_model_down == BARE)
         error_a = i_bare_end ;

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, " A ==> delta_va=%f error=%f\n", a, error_a) ;
}
#endif
      b = 0.0 ;
      b = bare_cathode_bias ;
      error_b = upward_deploy (b, delta_vp, delta_length, TRUE) ;

      if (bare_contact_model_down == BARE)
         error_b = i_bare_end ;

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, " B ==> delta_va=%f error=%f\n", b, error_b) ;
}
#endif

      if (error_a*error_b > 0.0)
      {
         fprintf (stderr,
                  "BAD:  Bounds for convergence inadequate [%f,%f]! @ %s\n",
                   a, b, Str_FromGMT (&curr_gmt)) ;
         fprintf (stderr, "      Errors = [%f, %f]\n", error_a, error_b) ;

         v_anode_out   = 0.0 ;
         v_cathode_out = 0.0 ;
         v_bare_res    = vxb_l ;
         i_bare_max    = 0.0 ;
         i_bare_ave    = 0.0 ;
         i_bare_end    = 0.0 ;
         imax_position = 0.0 ;
         v0_position   = tether_lib_len ;
         V_Mult (&bare_ixb_lvlh , 0.0, &bare_ixb_lvlh ) ;
         bare_ilxb_lvlh_mag = V_Mag (&bare_ilxb_lvlh) ;   

         return ;
      }
      dx = a - b ;

      for (count = 0 ; count < MAX_BISECTIONS ; count++)
      {
         delta_va = b + (dx *= 0.5) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
   fprintf (debug_out, "new delta_va=%f b=%f dx=%f\n", delta_va, b,dx);
#endif
         error = upward_deploy (delta_va, delta_vp, delta_length, FALSE) ;

         if (bare_contact_model_down == BARE)
            error = i_bare_end ;

         tether_temp   = compute_tether_temp (i_bare_ave) ;
         tether_resist = compute_tether_res  (tether_temp) ;
         dRt           = tether_resist / fabs (tether_lib_len) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
   fprintf (debug_out, "  === iter %2d R_tether = %f\n", count, tether_resist) ;
#endif

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "  *** bisection # %d ==> delta_va=%f error=%f i_ave=%f\n", 
                       count, delta_va, error, i_bare_ave) ;
}
#endif
         if (error <= 0.0)
            b = delta_va ;

         if (fabs (error) < error_thresh)
            break ;

         if (prev_error != 0.0 && (fabs (error-prev_error) < error_thresh))
         {
            dx *= 128.0 ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
   fprintf (debug_out, "BANG!\n") ;
#endif
         }

         prev_error = error ;
      }
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, " ===== CONVERGED! @ %s\n",Str_FromGMT (&curr_gmt)) ;
}
#endif
      error = upward_deploy (delta_va, delta_vp, delta_length, TRUE) ;
      if (bare_contact_model_down == BARE)
         error = i_bare_end ;

      tether_temp   = compute_tether_temp (i_bare_ave) ;
      tether_resist = compute_tether_res  (tether_temp) ;
      dRt           = tether_resist / fabs (tether_lib_len) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
   fprintf (debug_out, "  === iter %2d R_tether = %f\n", count, tether_resist) ;
#endif

      v_anode_out   = delta_va          ;
      v_cathode_out = down_contactor_model(i_bare_end) ;
      v_bare_res    = error             ;
   }
   else if (tether_lib_len < 0.0)   /* Downward deployment of tether          */
   {
      a = bare_anode_bias ;
      error_a = downward_deploy (a, delta_vp, delta_length, TRUE) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, " A ==> delta_va=%f error=%f\n", a, error_a) ;
}
#endif
      b = 0.0 ;
      b = -a ;
      b = bare_cathode_bias ;

      error_b = downward_deploy (b, delta_vp, delta_length, TRUE) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, " B ==> delta_va=%f error=%f\n", b, error_b) ;
}
#endif

      if (error_a*error_b > 0.0)
      {
         fprintf (stderr,
                  "BAD:  Bounds for convergence inadequate [%f,%f]! @ %s\n",
                   a, b, Str_FromGMT (&curr_gmt)) ;
         fprintf (stderr, "      Errors = [%f, %f]\n", error_a, error_b) ;

         v_anode_out   = 0.0 ;
         v_cathode_out = 0.0 ;
         v_bare_res    = vxb_l ;
         i_bare_max    = 0.0 ;
         i_bare_ave    = 0.0 ;
         i_bare_end    = 0.0 ;
         imax_position = 0.0 ;
         v0_position   = tether_lib_len ;
         V_Mult (&bare_ixb_lvlh , 0.0, &bare_ixb_lvlh ) ;
         bare_ilxb_lvlh_mag = V_Mag (&bare_ilxb_lvlh) ;   

         return ;
      }
      dx = a - b ;

      for (count = 0 ; count < MAX_BISECTIONS ; count++)
      {
         delta_va = b + (dx *= 0.5) ;

         error = downward_deploy (delta_va, delta_vp, delta_length, FALSE) ;

         tether_temp   = compute_tether_temp (i_bare_ave) ;
         tether_resist = compute_tether_res  (tether_temp) ;
         dRt           = tether_resist / fabs (tether_lib_len) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
   fprintf (debug_out, "  === iter %2d R_tether = %f\n", count, tether_resist) ;
#endif

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "  *** bisection # %d ==> delta_va=%f error=%f i_ave=%f\n", 
                       count, delta_va, error, i_bare_ave) ;
}
#endif
         if (error <= 0.0)
            b = delta_va ;

         if (fabs (error) < error_thresh)
            break ;
      }
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, " ===== CONVERGED! @ %s\n",Str_FromGMT (&curr_gmt)) ;
}
#endif
      error = downward_deploy (delta_va, delta_vp, delta_length, TRUE) ;
      v_anode_out   = delta_va          ;
      v_cathode_out = up_contactor_model(-i_bare_end) ;
      v_bare_res    = error             ;

      tether_temp   = compute_tether_temp (i_bare_ave) ;
      tether_resist = compute_tether_res  (tether_temp) ;
      dRt           = tether_resist / fabs (tether_lib_len) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
   fprintf (debug_out, "  === iter %2d R_tether = %f\n", count, tether_resist) ;
#endif

   }

   if (bare_ilxb_lvlh_mag != 0.0)
      thrust_cone_angle = acos (V_Dot (&bare_ilxb_lvlh, &control_lvlh) /
                       (V_Mag (&control_lvlh) * bare_ilxb_lvlh_mag)) ;

   switch (vector_control)
   {
      case VECT_CNTRL_OFF  : vect_thrust_out = 1 ;
                             break ;
      case VECT_CNTRL_CONE : if (thrust_cone_angle <= max_cone_angle)
                                vect_thrust_out = 1 ;
                             else
                                vect_thrust_out = 0 ;
                             break ;
      default              : 
         fprintf (stderr,
                  "BAD:  Invalid vector control mode [%d]!\007\n\n",
                   vector_control) ;
   }

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   show_cartesian (debug_out, "Control vect=",control_lvlh,1.0,"\n") ;
   show_cartesian (debug_out, "Thrust vect =",bare_ilxb_lvlh,1.0,"\n") ;
   fprintf (debug_out, "thrust_angle = %13.4f vect_thrust_out=%d\n",
                        thrust_cone_angle*R_D_CONST, vect_thrust_out) ;
}
#endif

   switch (position_control)
   {
      case POS_CNTRL_OFF : pos_thrust_out = 1 ;
                           break ;
      case POS_CNTRL_CYL : 
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "POS_CNTRL_CYL==>curr_time=%f\n", curr_time) ;
   show_earth (debug_out, "Sat   Pos =",sat_r_lla,"\n") ;
   show_earth (debug_out, "Cntrl Pos =",pos_cntrl_lla,"\n") ;
   fprintf (debug_out, "sat_r=%f pos_cntrl=%f delta=%f\n", sat_r_eci_mag,
                       pos_cntrl_eci_mag, pos_cntrl_eci_mag-sat_r_eci_mag) ;
}
#endif                           
                           if ((pos_cntrl_d_alt > 0.0) &&
                           ((sat_r_eci_mag < pos_cntrl_eci_mag) ||
                           (sat_r_eci_mag > pos_cntrl_eci_mag+pos_cntrl_d_alt)))
                           {
                              pos_thrust_out = 0 ;
                              break ;
                           }

                           if ((pos_cntrl_d_alt < 0.0) &&
                           ((sat_r_eci_mag > pos_cntrl_eci_mag) ||
                           (sat_r_eci_mag < pos_cntrl_eci_mag+pos_cntrl_d_alt)))
                           {
                              pos_thrust_out = 0 ;
                              break ;
                           }

                           V_Mult (&pos_cntrl_eci,
                              sat_r_eci_mag/pos_cntrl_eci_mag, &target_pos) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   show_cartesian (debug_out, "Sat   Pos = ",sat_r_eci,1.0, "\n") ;
   show_cartesian (debug_out, "target_pos=",target_pos,1.0,"\n") ;
}
#endif
                           V_Sub (&target_pos, &sat_r_eci, &target_pos) ;
                           delta_pos = V_Mag (&target_pos) ;

                           if (delta_pos < pos_cntrl_radius)
                              pos_thrust_out = 1 ;
                           else
                              pos_thrust_out = 0 ;
                           break ;
      case POS_CNTRL_ALT : pos_cntrl_eci_mag = MEAN_R + pos_cntrl_lla.Alt ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "POS_CNTRL_ALT==>curr_time=%f\n", curr_time) ;
   show_earth (debug_out, "Sat   Pos =",sat_r_lla,"\n") ;
   show_earth (debug_out, "Cntrl Pos =",pos_cntrl_lla,"\n") ;
   fprintf (debug_out, "sat_r=%f pos_cntrl=%f delta=%f\n", sat_r_eci_mag,
                       pos_cntrl_eci_mag, pos_cntrl_eci_mag-sat_r_eci_mag) ;
}
#endif                           
                           if ((pos_cntrl_d_alt > 0.0) &&
                           ((sat_r_eci_mag < pos_cntrl_eci_mag) ||
                           (sat_r_eci_mag > pos_cntrl_eci_mag+pos_cntrl_d_alt)))
                           {
                              pos_thrust_out = 0 ;
                              break ;
                           }

                           if ((pos_cntrl_d_alt < 0.0) &&
                           ((sat_r_eci_mag > pos_cntrl_eci_mag) ||
                           (sat_r_eci_mag < pos_cntrl_eci_mag+pos_cntrl_d_alt)))
                           {
                              pos_thrust_out = 0 ;
                              break ;
                           }

                           pos_thrust_out = 1 ;
                           break ;
      default              : 
         fprintf (stderr,
                  "BAD:  Invalid position control mode [%d]!\007\n\n",
                   vector_control) ;
   }

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out,
         "Altitude Range = {%7.1f, %7.1f} delta_pos=%7.1f pos_thrust_out=%d\n",
         pos_cntrl_lla.Alt, pos_cntrl_lla.Alt+pos_cntrl_d_alt,
         delta_pos, pos_thrust_out) ;
}
#endif

#if IV_CURVE
   fprintf (bare_debug, "%f %f %f %f\n", bare_down_voltage,
                         i_bare_max, i_bare_ave, i_bare_end) ;
   }
   fclose (bare_debug) ;
#endif

   if ((vect_thrust_out != 1) && (pos_thrust_out != 1))
   {
      v_anode_out   = 0.0 ;
      v_cathode_out = 0.0 ;
      v_bare_res    = 0.0 ;
      i_bare_max    = 0.0 ;
      i_bare_ave    = 0.0 ;
      i_bare_end    = 0.0 ;
      imax_position = 0.0 ;
      v0_position   = tether_lib_len ;
      V_Mult (&bare_ixb_lvlh , 0.0, &bare_ixb_lvlh ) ;
      bare_ilxb_lvlh_mag = V_Mag (&bare_ilxb_lvlh) ;   
   }
 
   p_load = i_bare_end * i_bare_end * bare_up_load ;

   fflush (debug_out) ;
   fflush (simul_out) ;
}



double upward_deploy   (double d_va, double d_vp, double d_len, int c_ixb)
{
   double vp            ;
   double vt            ;
   double delta_v       ;
   double ib            ;
   double ib_sum        ;
   double position      ;
   double di_dy         ;
   double scale         ;
   Cartesian d_ixb_lvlh ;
   Cartesian s_ixb_lvlh ;
   int    count         ;

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "  UP ==> d_va=%f d_vp=%f, d_len=%f\n",
                        d_va, d_vp, d_len) ;
}
#endif

#define LOG_TETHER_PROFILE 0

#if LOG_TETHER_PROFILE
   char   bare_debug2_fname [80] ;
   FILE  *bare_debug2 = NULL ;

fprintf (stderr, "LOGGING TETHER UP PROFILE!\n") ;
if (c_ixb)
{
   sprintf (bare_debug2_fname,"%s.%012.5f.tether",simul_fname, curr_time) ;
   fprintf (debug_out, "***** [%s] *****\n", bare_debug2_fname) ;
   if ((bare_debug2 = fopen (bare_debug2_fname, "w")) == NULL)
   {
      fprintf (stderr, "ERROR: Fopen for %s failed\n", bare_debug2_fname) ;
      bare_debug2 = stderr ;
   }
   fprintf (bare_debug2, "L Vt Vp di_dy Ib\n") ;
}
#endif

   vp = 0.0  ;
   vt = d_va ;
/*   i_bare [bare_tether_seg-1] = 0.0 ; */
   ib     = 0.0 ;
   ib_sum = 0.0 ;
   position = tether_lib_len ;
   i_bare_max = 0.0 ;
   i_bare_ave = 0.0 ;
   imax_position = 0.0 ;
   v0_position = 0.0 ;

   if (bare_contact_model_up == PM)
      ib = compute_parker_murphy_i_of_v (d_va) ;
   else if (bare_contact_model_up == HCPC) 
   {
      ib = f_HCPC_Current (d_va, bare_contact_p1_up, bfield_loc_mag, iri_elect_temp*8.62069e-5, iri_elect_density, 
                                 bare_contact_p2_up, bare_contact_p3_up, bare_contact_p4_up) ;
   }

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   UP_CONTACT (%d) ==> %fI = f(%fV) (sortof)\n", 
            bare_contact_model_up, ib, d_va) ;
}
#endif

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER_INTEG)
{
   fprintf (debug_out, "contact model up =%d ib=%e\n", bare_contact_model_up, ib) ;
}
#endif
   if (c_ixb)
   {
      V_Cross (&tether_lib_lvlh, &bfield_lvlh, &d_ixb_lvlh) ;

      if (fabs (d_len) > 0.0)
         V_Mult (&d_ixb_lvlh, 1.0/tether_lib_len, &d_ixb_lvlh) ;
      else
         V_Mult (&d_ixb_lvlh, 0.0           , &d_ixb_lvlh) ;

      V_Mult (&bare_ixb_lvlh , 0.0           , &bare_ixb_lvlh ) ;
   }

#if DEBUG
if ((show_debug & DEBUG_BARE_TETHER_INTEG) && c_ixb)
{
   fprintf (debug_out, "UPWARD DEPLOY ion_mass=%e Vc=%f\n",
            bare_ion_mass, bare_cathode_bias) ;
   fprintf (debug_out, "vp=%f vt=%f d_len=%f\n", vp, vt, d_len) ;
}
#endif
   for (count = bare_tether_seg ; count > 0 ; count--)
   {
                                   /* Determine Simpson's rule scalings      */
      if ((count == 0) || (count == bare_tether_seg))
         scale = 0.3333333333333333 ;  /* 1/3 */
      else if ((count %2) == 1)
         scale = 1.3333333333333333 ;  /* 4/3 */
      else
         scale = 0.6666666666666666 ;  /* 2/3 */
                                   /* Compute difference between Vp and Vt   */
      delta_v = vt - vp ;

      if ((position < t_bare_start) || (position > t_bare_end))
         di_dy = 0.0 ;
      else
      {
         if (delta_v > 0.0)     /* Collecting Electrons */
         {
            di_dy =        Q_EL * iri_elect_density * (2.0*tether_cond_radius) *
                    pow ( 2.0*Q_EL/M_EL*delta_v, di_dy_beta) *
                    di_dy_scale ;
         }
         else                   /* Collecting Ions */
         {
            di_dy = -1.0 * Q_EL * iri_elect_density * (2.0*tether_cond_radius) *
                    pow (-2.0*Q_EL/bare_ion_mass*delta_v, di_dy_beta) *
                    di_dy_scale ;
         }
      }

/*      i_bare [count] = i_bare [count+1] + di_dy * d_len ; */

      ib     += (di_dy * d_len * scale) ;
      ib_sum += (di_dy * d_len) ;

      if (c_ixb)
      {
         V_Mult (&d_ixb_lvlh, -1.0 * ib / 
                             ((double) (bare_tether_seg)), &s_ixb_lvlh) ;
         V_Add  (&s_ixb_lvlh, &bare_ixb_lvlh, &bare_ixb_lvlh) ;
      }
/*      if (i_bare [count] > i_bare_max) */
      if (ib > i_bare_max)
      {
         imax_position = position ;
/*         i_bare_max    = i_bare [count] ; */
         i_bare_max    = ib ;
      }
/*      i_bare_ave += i_bare [count] ; */
      i_bare_ave += (ib / ((double) bare_tether_seg)) ;
#if DEBUG
if ((show_debug & DEBUG_BARE_TETHER_INTEG) && c_ixb)
{
/* fprintf (debug_out, " l=%f vp=%f vt=%f delta_v=%f di_dy=%f i_bare[%d]=%f\n",
            position, vp, vt, delta_v, di_dy, count, i_bare[count]) ; */
   fprintf (debug_out, " l=%f vp=%f vt=%f delta_v=%f di_dy=%f i_bare[%d]=%f\n",
            position, vp, vt, delta_v, di_dy, count, ib) ; 
   fprintf (debug_out, "    ib_sum=%f ib_ave=%f scale=%f\n",
            ib_sum, i_bare_ave, scale) ;
}
#endif

#if LOG_TETHER_PROFILE
if (c_ixb)
{
   fprintf (bare_debug2, "%f %f %f %f %f\n", position, vt, vp, di_dy, ib) ;
}
#endif
                                   /* Update position and potentials         */
      position -= d_len ;
      vp       += d_vp ;
      vt       += (ib * d_len * dRt) ;
/*      vt = vt + d_len * i_bare [count+1] * dRt ; */
   }

#if LOG_TETHER_PROFILE
if (c_ixb)
   fclose (bare_debug2) ;
#endif

/*   i_bare_end  = i_bare [0] ; */
   i_bare_end  = ib ;
                                   /* Compute difference between Vp and Vt   */
   delta_v = vt - vp ;
                                  /* Compute magnitude of force              */
   bare_ixb_lvlh_mag  = V_Mag (&bare_ixb_lvlh) ;   
   V_Mult (&bare_ixb_lvlh, tether_lib_len, &bare_ilxb_lvlh) ;
   bare_ilxb_lvlh_mag = V_Mag (&bare_ilxb_lvlh) ;   

#if DEBUG
if ((show_debug & DEBUG_BARE_TETHER_INTEG) && c_ixb)
{
   fprintf (debug_out, " delta_v=%f i*R-Vc=%f\n", delta_v,
            (bare_up_load * i_bare_end - bare_cathode_bias)) ;
   if (c_ixb)
   {
      fprintf (debug_out, " ixb_mag =%e (N/m)\n", bare_ixb_lvlh_mag) ;
      fprintf (debug_out, " iLxb_mag=%e (N)\n", bare_ilxb_lvlh_mag) ;
   }
}
#endif

   return ( delta_v + (bare_up_load * i_bare_end - 
            down_contactor_model(i_bare_end)) ) ;
}



double up_contactor_model (double i)
{
   double v ;

   switch (bare_contact_model_up)
   {
      case FIXED  : v = bare_cathode_bias ;
                    break ;
      case FEA    : v = compute_fea_voltage (i) ;
                    break ;
      case PM     : v = compute_parker_murphy_v_of_i (i) ;
                    break ;
      case MODEL3 : if (i < 0) /* Electrons collected */
                    {
                       v =  1.0 * bare_contact_p1_up * 
                           exp (bare_contact_p2_up * i) +
                           bare_contact_p3_up ;
                    }
                    else
                    {
                       v = -1.0 * bare_contact_p1_up * 
                           exp (bare_contact_p2_up * i) +
                           bare_contact_p3_up ;
                    }
                    break ;
      case MODEL4 : v = bare_contact_p1_up * pow (i, bare_contact_p2_up) +
                        bare_contact_p3_up ;
                    break ;
      case CYLIND : if (i < 0) /* Electrons collected */
                    {
                        v =  0.5 * M_EL / Q_EL * 
                             pow (i / (Q_EL * iri_elect_density * 2.0 * bare_contact_p1_up * bare_contact_p2_up * di_dy_scale), 1.0/di_dy_beta) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   UP collected electrons\n") ;
}
#endif
                    }
                    else /* Ions collected */
                    {
                        v = -0.5 * bare_ion_mass / Q_EL * 
                             pow (i / (Q_EL * iri_elect_density * 2.0 * bare_contact_p1_up * bare_contact_p2_up * di_dy_scale), 1.0/di_dy_beta) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   UP collected ions\n") ;
}
#endif
                    }
                    break ;
      case HCPC   : v = f_HCPC_Voltage (i, bare_contact_p1_up, bfield_loc_mag, iri_elect_temp*8.62069e-5, iri_elect_density, 
                                        bare_contact_p2_up, bare_contact_p3_up, bare_contact_p4_up,
                                        HCPC_CONV_MIN_VOLT, HCPC_CONV_MAX_VOLT) ;
                    break ;
      default     : fprintf (stderr,
                       "BAD:  Invalid up contactor model number\007!\n") ;
                    v = bare_cathode_bias ;
                    break ;
   }

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   UP_CONTACT (%d) ==> %fV = f(%fI)\n", 
            bare_contact_model_up, v, i) ;
}
#endif

   return (v) ;
}



double downward_deploy(double d_va, double d_vp, double d_len, int c_ixb)
{
   double vp            ;
   double vt            ;
   double delta_v       ;
   double position      ;
   double ib            ;
   double ib_sum        ;
   double di_dy         ;
   double scale         ;
   Cartesian d_ixb_lvlh ;
   Cartesian s_ixb_lvlh ;
   int    count         ;
   double v0_neg_val    ;
   double v0_neg_pos    ;
   double v0_pos_val    ;
   double v0_pos_pos    ;

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "  DOWN ==> d_va=%f d_vp=%f, d_len=%f\n",
                        d_va, d_vp, d_len) ;
}
#endif

#define LOG_TETHER_PROFILE 0

#if LOG_TETHER_PROFILE
   char   bare_debug2_fname [80] ;
   FILE  *bare_debug2 = NULL ;

if (c_ixb)
{
   sprintf (bare_debug2_fname,"%s.%012.5f.tether",simul_fname, curr_time) ;
   fprintf (debug_out, "***** [%s] *****\n", bare_debug2_fname) ;
   if ((bare_debug2 = fopen (bare_debug2_fname, "w")) == NULL)
   {
      fprintf (stderr, "ERROR: Fopen for %s failed\n", bare_debug2_fname) ;
      bare_debug2 = stderr ;
   }
   fprintf (bare_debug2, "L Vt Vp di_dy Ib\n") ;
}
#endif
   vp = 0.0  ;
   vt = d_va ;
/*   i_bare [bare_tether_seg-1] = 0.0 ; */
   ib = 0.0 ;
   ib_sum = 0.0 ;
   position = tether_lib_len ;
   i_bare_max = 0.0 ;
   i_bare_ave = 0.0 ;
   imax_position = 0.0 ;
   v0_position = 0.0 ;
   v0_neg_val = -1000.0 ;
   v0_neg_pos =     0.0 ;
   v0_pos_val =  1000.0 ;
   v0_pos_pos =     0.0 ;

#if 0
   ib = down_contactor_model_iofv (d_va) ;
#endif

   if (bare_contact_model_down == PM)
      ib = compute_parker_murphy_i_of_v (d_va) ;
   else if (bare_contact_model_down == HCPC)
   {
      ib = f_HCPC_Current (d_va, bare_contact_p1_down, bfield_loc_mag, iri_elect_temp*8.62069e-5, iri_elect_density,
                                 bare_contact_p2_down, bare_contact_p3_down, bare_contact_p4_down) ;
   }

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   DOWN_CONTACT (%d) ==> %fI = f(%fV) (sortof)\n", 
            bare_contact_model_down, ib, d_va) ;
}
#endif

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER_INTEG)
{
   fprintf (debug_out, "contact model down =%d ib=%e\n", bare_contact_model_down, ib) ;
}
#endif
   if (c_ixb)
   {
      V_Cross (&tether_lib_lvlh, &bfield_lvlh, &d_ixb_lvlh) ;

      if (fabs (d_len) > 0.0)
         V_Mult (&d_ixb_lvlh, 1.0/tether_lib_len, &d_ixb_lvlh) ;
      else
         V_Mult (&d_ixb_lvlh, 0.0           , &d_ixb_lvlh) ;

      V_Mult (&bare_ixb_lvlh, 0.0           , &bare_ixb_lvlh) ;
   }

#if DEBUG
if ((show_debug & DEBUG_BARE_TETHER_INTEG) && c_ixb)
{
   fprintf (debug_out, "DOWNWARD DEPLOY\n") ;
   fprintf (debug_out, "vp=%f vt=%f ib=%f\n", vp, vt, ib) ;
}
#endif
   for (count = bare_tether_seg ; count > 0 ; count--)
   {
                                   /* Determine Simpson's rule scalings      */
      if ((count == 0) || (count == bare_tether_seg))
         scale = 0.3333333333333333 ;  /* 1/3 */
      else if ((count %2) == 1)
         scale = 1.3333333333333333 ;  /* 4/3 */
      else
         scale = 0.6666666666666666 ;  /* 2/3 */

                                   /* Compute difference between Vp and Vt   */
      delta_v = vt - vp ;

      if (delta_v <= 0.0 && delta_v >= v0_neg_val)
      {
         v0_neg_val = delta_v ;
         v0_neg_pos = position ;
      }

      if (delta_v >= 0.0 && delta_v <= v0_pos_val)
      {
         v0_pos_val = delta_v ;
         v0_pos_pos = position ;
      }

      if ((position > t_bare_start) || (position < t_bare_end))
         di_dy = 0.0 ;
      else
      {
         if (delta_v > 0.0)     /* Electrons being collected */
         {
            di_dy =        Q_EL * iri_elect_density * (2.0*tether_cond_radius) *
                    pow ( 2.0*Q_EL/M_EL*delta_v, di_dy_beta) *
                    di_dy_scale ;
         }
         else                   /* Ions being collected */
         {
            di_dy = -1.0 * Q_EL * iri_elect_density * (2.0*tether_cond_radius) *
                    pow (-2.0*Q_EL/bare_ion_mass*delta_v, di_dy_beta) *
                    di_dy_scale ;
         }
      }

/*      i_bare [count] = i_bare [count+1] + di_dy * d_len ; */

      ib     += (di_dy * d_len * scale) ;
      ib_sum += (di_dy * d_len) ;

      if (c_ixb)
      {
         V_Mult (&d_ixb_lvlh, ib / 
                             ((double) (bare_tether_seg)), &s_ixb_lvlh) ;
         V_Add  (&s_ixb_lvlh, &bare_ixb_lvlh, &bare_ixb_lvlh) ;
      }
/*      if (i_bare [count] > i_bare_max) */
      if (ib > i_bare_max)
      {
         imax_position = position ;
/*         i_bare_max    = i_bare [count] ; */
         i_bare_max    = ib ;
      }

#if LOG_TETHER_PROFILE
if (c_ixb)
{
   fprintf (bare_debug2, "%f %f %f %f %f\n", position, vt, vp, di_dy, ib) ;
}
#endif

/*      i_bare_ave += i_bare [count] ; */
      i_bare_ave += (ib / ((double) bare_tether_seg));
#if DEBUG
if ((show_debug & DEBUG_BARE_TETHER_INTEG) && c_ixb)
{
/* fprintf (debug_out, " l=%f vp=%f vt=%f delta_v=%f di_dy=%f i_bare[%d]=%f\n",
            position, vp, vt, delta_v, di_dy, count, i_bare[count]) ; */
   fprintf (debug_out, " l=%f vp=%f vt=%f delta_v=%f di_dy=%f i_bare[%d]=%f\n",
            position, vp, vt, delta_v, di_dy, count, ib) ; 
   fprintf (debug_out, "    ib_sum=%f\n", ib_sum) ;
   fprintf (debug_out, "    ib_sum=%f ib_ave=%f\n", ib_sum, i_bare_ave) ;
}
#endif
                                   /* Update position and potentials         */
      position += d_len ;
      vp       -= d_vp ;
      vt       += (ib * d_len * dRt) ;
/*      vt = vt - d_len * i_bare [count+1] * dRt ; */
   }

#if LOG_TETHER_PROFILE
if (c_ixb)
   fclose (bare_debug2) ;
#endif

/*   i_bare_end  = i_bare [0] ; */
   i_bare_end  = ib ;
                                   /* Compute difference between Vp and Vt   */
   delta_v = vt - vp ;
                                  /* Compute magnitude of force              */
   bare_ixb_lvlh_mag = V_Mag (&bare_ixb_lvlh) ;   
   V_Mult (&bare_ixb_lvlh, tether_lib_len, &bare_ilxb_lvlh) ;
   bare_ilxb_lvlh_mag = V_Mag (&bare_ilxb_lvlh) ;   

   v_power_supply = power_supply_model (ib) ;
   p_power_supply = v_power_supply * ib ;

#if DEBUG
if ((show_debug & DEBUG_BARE_TETHER_INTEG) && c_ixb)
{
   fprintf (debug_out, " l=%f vp=%f vt=%f delta_v=%f di_dy=%f i_bare[%d]=%f\n",
            position, vp, vt, delta_v, di_dy, count, ib) ; 
   fprintf (debug_out, " v_power_supply=%f delta_v=%f Vc=%f\n",
            v_power_supply, delta_v, up_contactor_model (-1.0 * i_bare_end) );
   if (c_ixb)
   {
      fprintf (debug_out, " ixb_mag=%e (N/m)\n", bare_ixb_lvlh_mag) ;
      fprintf (debug_out, " iLxb_mag=%e (N)\n", bare_ilxb_lvlh_mag) ;
   }
}
#endif

   if ((fabs(v0_pos_pos) >= 0.001) && (fabs(v0_neg_pos) >= 0.001))
   {
      if (v0_pos_val == v0_neg_val)
         v0_position = 0.5 * (v0_neg_pos + v0_pos_pos) ;
      else
         v0_position = (v0_neg_pos * v0_pos_val - v0_pos_pos * v0_neg_val) /
                       (v0_pos_val - v0_neg_val) ;
   }
   else
      v0_position = tether_lib_len ;

#if DEBUG
if ((show_debug & DEBUG_BARE_TETHER_INTEG) && c_ixb)
{
   fprintf (debug_out,
      "%s ==>v0_neg_pos=%f neg_val=%f v0_pos_pos=%f pos_val=%f v0_position=%f\n",
                        Str_FromGMT(&curr_gmt),
                        v0_neg_pos, v0_neg_val, v0_pos_pos, v0_pos_val,
                        v0_position) ;
}
#endif
   return ( delta_v - (v_power_supply - (bare_up_load * i_bare_end) +
                                     up_contactor_model (-1.0 * i_bare_end)) );
}



double power_supply_model (double i)
{
   double v ;

   switch (thrust_control)
   {
      case BATTERY    : v = bare_down_voltage ;
                        break ;
      case DOUBLE_EMF : v = 2.0 * vxb_l ;
                        break ;
      case CONST_PWR  : if (i > 0.0)
                        {
                           v = bare_down_power / i ;
                           if (v > bare_down_voltage)
                              v = bare_down_voltage ;
                        }
                        else
                           v = bare_down_voltage ;
                        break ;
      default         : fprintf (stderr,
                           "BAD: Invalid option for power supply model\n") ;
                        v = 0.0 ;
                        break ;
   }

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   POWER_SUPPLY ==> %fV = f(%fA)\n", v, i) ;
}
#endif

   return (v) ;
}



double down_contactor_model_iofv (double v)
{
   double i ;

   switch (bare_contact_model_down)
   {
      case MODEL3 : if (v > 0) /* Electrons collected */
                    {
                       i =  1.0 * bare_contact_p1_down * 
                           exp (bare_contact_p2_down * v) +
                           bare_contact_p3_down ;
                    }
                    else
                    {
                       i = -1.0 * bare_contact_p1_down * 
                           exp (bare_contact_p2_down * v * -1.0) +
                           bare_contact_p3_down ;
                    }
                    break ;
      default     : fprintf (stderr,
                       "BAD:  Invalid down contactor model number\007!\n") ;
                    v = bare_cathode_bias ;
                    break ;
   }

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   DOWN_CONTACT (%d) ==> %fA = f(%fV)\n", 
               bare_contact_model_down, i, v) ;
}
#endif

   return (i) ;
}

double down_contactor_model (double i)
{
   double v ;

   switch (bare_contact_model_down)
   {
      case FIXED  :
      case BARE   : v = bare_cathode_bias ;
                    break ;
      case FEA    : v = compute_fea_voltage (i) ;
                    break ;
      case PM     : v = compute_parker_murphy_v_of_i (i) ;
                    break ; 
      case MODEL3 : if (i < 0) /* Electrons collected */
                    {
                       v =  1.0 * bare_contact_p1_down * 
                           exp (bare_contact_p2_down * i * -1.0) +
                           bare_contact_p3_down ;
                    }
                    else
                    {
                       v = -1.0 * bare_contact_p1_down * 
                           exp (bare_contact_p2_down * i *  1.0) +
                           bare_contact_p3_down ;
                    }
                    break ;
      case MODEL4 : v = bare_contact_p1_down * pow (i, bare_contact_p2_down) +
                        bare_contact_p3_down ;
                    break ;
      case CYLIND : if (i < 0) /* Electrons being collected */
                    {
                        v =  0.5 * M_EL / Q_EL * 
                             pow (i / (Q_EL * iri_elect_density * 2.0 * bare_contact_p1_down * bare_contact_p2_down * di_dy_scale), 1.0/di_dy_beta) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   DOWN collected electrons\n") ;
}
#endif
                    }
                    else /* Ions being collected */
                    {
                        v = -0.5 * bare_ion_mass / Q_EL * 
                             pow (i / (Q_EL * iri_elect_density * 2.0 * bare_contact_p1_down * bare_contact_p2_down * di_dy_scale), 1.0/di_dy_beta) ;
#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   DOWN collected ions\n") ;
}
#endif
                    }
                    break ;
      case HCPC   : v =  1.0 * f_HCPC_Voltage (-1.0 * i, bare_contact_p1_down, bfield_loc_mag, iri_elect_temp*8.62069e-5, iri_elect_density, 
                                               bare_contact_p2_down, bare_contact_p3_down, bare_contact_p4_down,
                                               HCPC_CONV_MIN_VOLT, HCPC_CONV_MAX_VOLT) ;
                    break ;
      default     : fprintf (stderr,
                       "BAD:  Invalid down contactor model number\007!\n") ;
                    v = bare_cathode_bias ;
                    break ;
   }

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "   DOWN_CONTACT (%d) ==> %fV = f(%fA)\n", 
            bare_contact_model_down, v, i) ;
}
#endif

   return (v) ;
}



double compute_fea_voltage (double current)
{
   double v    ;                   /* Solution voltage                       */
   double i    ;                   /* Current i (v)                          */
   double di   ;                   /* Derivative of current dI/dV (v)        */
   double dv   ;                   /* Voltage step for convergence           */
   double vexp ;                   /* Temporary variable to reduces calcs    */
   int    k    ;                   /* Iteration loop counter                 */

                                   /* Ensure that current != 0               */
   if (current < 0.000001)
      current = 0.000001 ;
                                   /* Set initial guess to 100 volts         */
   v = 100.0 ;
                                   /* Execute Newton-Raphson algorithm       */
   for (k = 0 ; k < MAX_ITER_NR ; k++)
   {
      vexp = exp (-1.0 * fea_b / v) ;
      i  = fea_n * fea_a * v * v * vexp - current ;
      di = fea_n * fea_a * vexp * (2.0 * v + fea_b) ;

      dv = i / di ;

      v -= dv ;
      if (fabs (dv) < 0.000001)
         return (v) ;
   }

   fprintf (stderr, "BAD: Newton-Raphson did not converge for FEA!\n") ;

   return (0.0) ;
}



double compute_parker_murphy_v_of_i (double current)
{
   double v    ;                   /* Solution voltage                       */
   double i0   ;                   /* Temporary variable to reduces calcs    */
   double phi0 ;                   /* Temporary variable to reduces calcs    */

   i0   = 2.0 * M_PI * pm_r_sat * pm_r_sat * Q_EL * iri_elect_density *
             sqrt (8.0 * K_BOLTZ * iri_elect_temp / (M_PI * M_EL)) * 0.25 ;
   phi0 = Q_EL * pm_r_sat * pm_r_sat * bfield_loc_mag * bfield_loc_mag /
                 (8.0 * M_EL) ;
   
   v = phi0 * pow (current / (i0 * pm_alpha) - 1.0, 1.0/pm_beta) ;

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "  PM ==> %fV = f (%fA)\n", v, current) ;
   fprintf (debug_out, "     i0 = %e phi0 = %e\n", i0, phi0) ;
}
#endif

   return (v) ;
}



double compute_parker_murphy_i_of_v (double v)
{
   double i    ;                   /* Solution current                       */
   double i0   ;                   /* Temporary variable to reduces calcs    */
   double phi0 ;                   /* Temporary variable to reduces calcs    */

   i0   = 2.0 * M_PI * pm_r_sat * pm_r_sat * Q_EL * iri_elect_density *
             sqrt (8.0 * K_BOLTZ * iri_elect_temp / (M_PI * M_EL)) * 0.25 ;
   phi0 = Q_EL * pm_r_sat * pm_r_sat * bfield_loc_mag * bfield_loc_mag /
                  (8.0 * M_EL) ;


   if (v >= 5.0)
      i = i0 * pm_alpha * (1.0 + pow (v/phi0, pm_beta)) ;
   else if ((v <= 5.0) && (v >= 0.0))
      i = v / 5.0 * i0 * pm_alpha * (1.0 + pow (5.0/phi0, pm_beta)) ;
   else
      i = 0.0 ;

#if DEBUG
if (show_debug & DEBUG_BARE_TETHER)
{
   fprintf (debug_out, "  PM ==> %eA = f (%fV)\n", i, v) ;
   fprintf (debug_out, "     i0 = %e phi0 = %e\n", i0, phi0) ;
}
#endif

   return (i) ;
}
