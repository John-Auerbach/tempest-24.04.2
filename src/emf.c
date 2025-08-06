/*****************************************************************************/
/*                                                                           */
/*   Module:    emf.c                                                        */
/*                                                                           */
/*   Purpose: 	Compute the induced voltage (EMF) int the conducting tether  */
/*		and in the electric field double probes.                     */
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
/*   RCS:	$Id: emf.c,v 1.9 2006/04/28 23:58:50 voronka Exp $                                                         */
/*                                                                           */
/*              $Log: emf.c,v 
 *              Revision 1.8  1997/07/13 22:45:23  nestor
 *              Added EMF_SET parameter.
 *
 * Revision 1.7  1997/06/06  03:55:29  nestorv
 * Removed unnecessary globals.
 *
 * Revision 1.6  1996/07/23  20:36:13  nestorv
 * Improved integreated vxb.l with simpsons rule and v(l) and b(l).
 *
 * Revision 1.5  1996/02/25  09:35:50  sets
 * Fixed divide by zero problem in compute_angle.
 *
 * Revision 1.4  1996/01/30  06:37:18  nestorv
 * Removed delta computations.
 *
 * Revision 1.3  1995/09/28  22:06:04  nestorv
 * Changed && to & in show_debug conditional.
 *
 * Revision 1.2  1995/09/28  21:30:25  nestorv
 * Added && DEBUG_EMF to all show_debug conditionals and RCS info into header.
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
#include "tether.h"
#include "bfield.h"
#include "emf.h"

#define ONE_GEE 9.797645

double compute_integ_vxb_dl (Cartesian sat_r_eci , Cartesian sat_v_eci,
                             Cartesian tether_lvlh)
{
   Cartesian cur_r_eci    ;      
   Cartesian cur_v_eci    ;      
   Earth     cur_sat_lla  ;
   Spherical igrf_pos     ;
   Cartesian b_lgeo       ;
   Cartesian b_lvlh       ;
   Cartesian v_corot      ;
   Cartesian v_lvlh       ;
   Cartesian vxb_lvlh     ;
   Cartesian v_radial     ;
   Cartesian v_tan        ;
   double    vxb_l        ;
   Cartesian dl_vect_eci  ;
   Cartesian dl_vect_lvlh ;
   int       segment      ;
   double    scale        ;

   vxb_l   = 0.0 ;
                                   /* Compute dL in ECI and LVLH             */
   V_Mult (&tether_lvlh, 1.0/ (double) emf_integ_seg, &dl_vect_lvlh) ;
   E_LVLHToECI (&sat_r_eci, &sat_v_eci, &dl_vect_lvlh, &dl_vect_eci) ;
                                   /* Copy inital R to working R             */
   bcopy (&sat_r_eci, &cur_r_eci, sizeof (Cartesian)) ;

                                   /* Project V onto R for radial component  */
   V_Mult (&sat_r_eci, V_Dot (&sat_r_eci, &sat_v_eci) / 
                       (V_Mag (&sat_r_eci)*V_Mag (&sat_r_eci)), &v_radial) ;
#if DEBUG
if (show_debug & DEBUG_EMF)
{
   show_cartesian (debug_out, "orbiter R", sat_r_eci, 1000.0, "\n") ;
   show_cartesian (debug_out, "orbiter V", sat_v_eci, 1000.0, "\n") ;
   show_cartesian (debug_out, "tether L", tether_lvlh, 1.0, "\n") ;
   show_cartesian (debug_out, "dL (ECI)", dl_vect_eci, 1.0, "\n") ;
}
#endif
   for (segment = 0 ; segment <= emf_integ_seg ; segment++) 
   {
#if DEBUG
if (show_debug & DEBUG_EMF)
{
   show_cartesian (debug_out, "current R", cur_r_eci, 1000.0, "\n") ;
   show_cartesian (debug_out, "initial V", cur_v_eci, 1000.0, "\n") ;
}
#endif
#if DEBUG
if (show_debug & DEBUG_EMF)
{
   show_cartesian (debug_out, "Vrad", v_radial, 1000.0, "\n\n") ;
   show_cartesian (debug_out, "Vtan", v_tan   , 1000.0, "\n\n") ;
}
#endif
                                   /* Take difference for tangential compont */
      V_Sub  (&sat_v_eci, &v_radial, &v_tan) ;
                                   /* Scale tangential component             */
      V_Mult (&v_tan, V_Mag (&cur_r_eci) / V_Mag (&sat_r_eci), &v_tan) ;
                                   /* Add radial and tangential for V        */
      V_Add  (&v_tan, &v_radial, &cur_v_eci) ;
#if DEBUG
if (show_debug & DEBUG_EMF)
{
   show_cartesian (debug_out, "Vtan scaled", v_tan, 1000.0, "\n\n") ;
   show_cartesian (debug_out, "corrected V", cur_v_eci, 1000.0, "\n\n") ;
}
#endif
                                   /* Convert Earth Centered Inertial Coords */
                                   /*    to Latitude, Longitude and Altitude */
      E_FromECI (curr_time, &cur_r_eci, &cur_sat_lla) ;

                                   /* Convert Lat, Long, Alt to spherical    */
      igrf_pos.Th =         cur_sat_lla.Long ;
      igrf_pos.Ph = HF_PI - cur_sat_lla.Lat  ;
      igrf_pos.R  = V_Mag (&cur_r_eci)     ;
                                   /* Compute magnetic field using IGRF      */
      IGRF_GetBField_ord (&igrf_pos, &b_lgeo, b_igrf_order) ;
                                   /* Convert IGRF magnetic field from local */
                                   /*  geocentric to LVLH coordinates        */
      E_LocalToLVLH (&sat_r_eci, &sat_v_eci, &b_lgeo, &b_lvlh) ;
   
                                  /* Correct for the rotation of the earth   */
      v_corot.X =  EARTH_OMEGA * cur_r_eci.Y ;
      v_corot.Y = -EARTH_OMEGA * cur_r_eci.X ;
      v_corot.Z =  0.0 ;
      V_Add (&cur_v_eci, &v_corot, &v_lvlh) ;
   
      E_ECIToLVLH (&sat_r_eci, &sat_v_eci, &v_lvlh, &v_lvlh) ;

                                   /* Compute V x B  (in V/m)                */
      V_Cross (&v_lvlh, &b_lvlh, &vxb_lvlh) ;
                                   /* Convert dL to proper lvlh coords       */
      E_ECIToLVLH (&sat_r_eci, &sat_v_eci, &dl_vect_eci, &dl_vect_lvlh) ;
                                   /* Determine Simpson's rule scalings      */
      if ((segment == 0) || (segment == emf_integ_seg))
         scale = 1.0 ;
      else if ((segment % 2) == 1)
         scale = 4.0 ;
      else
         scale = 2.0 ;
                                   /* Compute V x B . dL  ( in Volts)        */
      vxb_l += (scale * V_Dot (&vxb_lvlh, &dl_vect_lvlh)) ;
#if DEBUG
if (show_debug & DEBUG_EMF)
   fprintf (debug_out, "seg=%d scale=%f vxb_l=%f\n", segment, scale, vxb_l) ;
#endif
      V_Add (&cur_r_eci, &dl_vect_eci, &cur_r_eci) ;
   }
                                   /* Multiply by 1/3 for Simpson's Rule     */
   return (vxb_l*0.3333333333) ;
}



double compute_angle (Cartesian *v1, Cartesian *v2)
{
   double    mag_v1     ;
   double    mag_v2     ;
   Cartesian v1_x_v2    ;
   double    sin_theta  ;
   double    cos_theta  ;
   double    theta      ;

   mag_v1 = V_Mag (v1) ;
   mag_v2 = V_Mag (v2) ;

   if (mag_v1 > 0.0 && mag_v2 > 0.0)
   {
      V_Cross (v1, v2, &v1_x_v2) ;

      sin_theta = V_Mag (&v1_x_v2) / (mag_v1 * mag_v2) ;
      cos_theta = V_Dot (v1, v2) / (mag_v1 * mag_v2) ;

      theta = atan2 (sin_theta, cos_theta) ;

      return (theta) ;
   }
   else
      return (0.0) ;
}



double compute_vxb_l_lvlh (Cartesian sat_r_eci , Cartesian sat_v_eci,
                           Cartesian bfield_lgeo, Cartesian tether_lvlh,
                           int compute_angle_dist) 
{
   Cartesian v_corot     ;
   Cartesian v_lvlh      ;
   Cartesian vxb_lvlh    ;
   double    vxb_l       ;
                                   /* Convert IGRF magnetic field from local */
                                   /*  geocentric to LVLH coordinates        */
   E_LocalToLVLH (&sat_r_eci, &sat_v_eci, &bfield_lgeo, &bfield_lvlh) ;

                                  /* Correct for the rotation of the earth ***/
   v_corot.X =  EARTH_OMEGA * sat_r_eci.Y ;
   v_corot.Y = -EARTH_OMEGA * sat_r_eci.X ;
   v_corot.Z =  0.0 ;
#if 0
if (show_debug & DEBUG_EMF)
{
   E_ECIToLVLH (&sat_r_eci, &sat_v_eci, &sat_v_eci, &v_lvlh) ;
   show_cartesian (debug_out, "r_eci        ", sat_r_eci, 1000.0, "\n") ;
   show_cartesian (debug_out, "v_eci        ", sat_v_eci, 1000.0, "\n") ;
   show_cartesian (debug_out, "v_lvlh       ", v_lvlh, 1000.0, "\n") ;
   show_cartesian (debug_out, "v_corot      ", v_corot, 1000.0, "\n") ;
}
#endif
   V_Add (&sat_v_eci, &v_corot, &v_lvlh) ;
#if 0
if (show_debug & DEBUG_EMF)
{
   show_cartesian (debug_out, "v_corot+v_eci", v_lvlh, 1000.0, "\n") ;
}
#endif
   
   E_ECIToLVLH (&sat_r_eci, &sat_v_eci, &v_lvlh, &v_lvlh) ;
#if 0
if (show_debug & DEBUG_EMF)
{
   show_cartesian (debug_out, "v_lvlh       ", v_lvlh, 1000.0, "\n") ;
}
#endif
                                   /* Compute V x B  (in V/m)                */
   V_Cross (&v_lvlh, &bfield_lvlh, &vxb_lvlh) ;
                                   /* Compute V X B . L  ( in Volts)         */
   vxb_l = V_Dot (&vxb_lvlh, &tether_lvlh) ;

   if (compute_angle_dist == COMPUTE_DA_B_L)
   {                               /* Compute angle between tether and B     */
      theta_bl = compute_angle (&tether_lvlh, &bfield_lvlh) ;

                                   /* Compute angle between v and B          */
      theta_vb = compute_angle (&bfield_lvlh, &v_lvlh) ;

                                   /* Compute angle between tether and E     */
      phi_el   = compute_angle (&tether_lvlh, &vxb_lvlh) ;

                                   /* Store value of induced electric field  */
      vxb_lvlh0.X = vxb_lvlh.X ;
      vxb_lvlh0.Y = vxb_lvlh.Y ;
      vxb_lvlh0.Z = vxb_lvlh.Z ;

      vxb_lvlh0_mag = V_Mag (&vxb_lvlh0) ;

                                   /* Compute distance between B & L         */
      d_b_l_start = fabs (sin (DEG_TO_RAD (theta_bl)) * tether_start) ;
      d_b_l_end   = fabs (sin (DEG_TO_RAD (theta_bl)) * tether_end  ) ;
   }

   return (vxb_l) ;
}



void init_emfs ()
{
   if (emf_integ_seg > 0 && emf_integ_seg % 2 != 0)
   {
      emf_integ_seg++ ;
      fprintf (stderr,
               "WARNING: Number of integration segments needs to be eveN!!\n") ;
      fprintf (stderr,
               "         Increased number of segments to %d\n", emf_integ_seg) ;
   }
   
   boom_thrust_run_avg_x = -1.0 ;
   boom_thrust_run_avg_y = -1.0 ;
   boom_thrust_run_avg_z = -1.0 ;
}



void compute_emfs ()
{
   double n, np1 ;

   Cartesian vector_i = {1.0, 0.0, 0.0} ;
   Cartesian vector_j = {0.0, 1.0, 0.0} ;
   Cartesian vector_k = {0.0, 0.0, 1.0} ;
   Matrix    MM                         ;
   
   if (emf_set > 0.0)
   {
      vxb_l        = emf_set ;
      vxb_l_gg     = emf_set ;
      vxb_dl_integ = emf_set ;
   }
   else
   {
                                  /* Compute induced EMF on gg  tether @ 0   */
      vxb_l_gg  = compute_vxb_l_lvlh (sat_r_eci, sat_v_eci, bfield_loc_gc,
                                      tether_gg_lvlh , NO_COMPUTE_DA_B_L) ;

                                  /* Compute induced EMF on lib tether @ 0   */
      vxb_l     = compute_vxb_l_lvlh (sat_r_eci, sat_v_eci, bfield_loc_gc,
                                      tether_lib_lvlh, COMPUTE_DA_B_L) ;

      if (emf_integ_seg > 0)
      {
                                  /* Compute induced EMF by integrading / dl */
      vxb_dl_integ = compute_integ_vxb_dl (sat_r_eci,sat_v_eci,tether_lib_lvlh);
      }

      if (boom_current > 0.0)
      {
         V_Mult (&vector_i, 2.0*efield_boom_x*boom_current, &vector_i) ;
         V_Mult (&vector_j, 2.0*efield_boom_y*boom_current, &vector_j) ;
         V_Mult (&vector_k, 2.0*efield_boom_z*boom_current, &vector_k) ;

         V_Cross (&vector_i, &bfield_lvlh, &boom_thrust_x) ;
         V_Cross (&vector_j, &bfield_lvlh, &boom_thrust_y) ;
         V_Cross (&vector_k, &bfield_lvlh, &boom_thrust_z) ;

         V_Add (&boom_thrust_x, &boom_thrust_y, &total_boom_thrust) ;
         V_Add (&boom_thrust_z, &total_boom_thrust, &total_boom_thrust) ;

         boom_thrust_x_mag = V_Mag (&boom_thrust_x) ;
         boom_thrust_y_mag = V_Mag (&boom_thrust_y) ;
         boom_thrust_z_mag = V_Mag (&boom_thrust_z) ;
/*
         MM [0][0] = boom_thrust_x.X ;
         MM [0][1] = boom_thrust_x.Y ;
         MM [0][2] = boom_thrust_x.Z ;
         MM [1][0] = boom_thrust_y.X ;
         MM [1][1] = boom_thrust_y.Y ;
         MM [1][2] = boom_thrust_y.Z ;
         MM [2][0] = boom_thrust_z.X ;
         MM [2][1] = boom_thrust_z.Y ;
         MM [2][2] = boom_thrust_z.Z ;

         det_span_set = M_Determinant (MM) ;
*/

         if (boom_thrust_run_avg_x < 0.0)
         {
            boom_thrust_run_avg_x = boom_thrust_x_mag ;
            boom_thrust_run_avg_y = boom_thrust_y_mag ;
            boom_thrust_run_avg_z = boom_thrust_z_mag ;
         }
         else
         {
            n = (curr_time - start_time) / incr_time ;
            np1 = n + 1.0 ;

            boom_thrust_run_avg_x = boom_thrust_run_avg_x * (n / np1) + boom_thrust_x_mag / np1 ;
            boom_thrust_run_avg_y = boom_thrust_run_avg_y * (n / np1) + boom_thrust_y_mag / np1 ;
            boom_thrust_run_avg_z = boom_thrust_run_avg_z * (n / np1) + boom_thrust_z_mag / np1 ;
         }

         boom_thrust_x_isp = boom_thrust_run_avg_x / (fuel_mass_flow * ONE_GEE) ;
         boom_thrust_y_isp = boom_thrust_run_avg_y / (fuel_mass_flow * ONE_GEE) ;
         boom_thrust_z_isp = boom_thrust_run_avg_z / (fuel_mass_flow * ONE_GEE) ;

         boom_thrust_x_dv = ONE_GEE * boom_thrust_x_isp * 
                            log(system_mass / (system_mass - total_fuel_mass)) ;
         boom_thrust_y_dv = ONE_GEE * boom_thrust_y_isp * 
                            log(system_mass / (system_mass - total_fuel_mass)) ;
         boom_thrust_z_dv = ONE_GEE * boom_thrust_z_isp * 
                            log(system_mass / (system_mass - total_fuel_mass)) ;

         boom_thrust_x_dv_pkg = boom_thrust_x_dv / system_mass ;
         boom_thrust_y_dv_pkg = boom_thrust_y_dv / system_mass ;
         boom_thrust_z_dv_pkg = boom_thrust_z_dv / system_mass ;

         boom_thrust_x_t2p = boom_thrust_run_avg_x / boom_power_in ;
         boom_thrust_y_t2p = boom_thrust_run_avg_y / boom_power_in ;
         boom_thrust_z_t2p = boom_thrust_run_avg_z / boom_power_in ;

         V_Unit (&vector_i, &vector_i) ;
         V_Unit (&vector_j, &vector_j) ;
         V_Unit (&vector_k, &vector_k) ;

         V_Mult (&vector_i, 0.5*efield_boom_x, &vector_i) ;
         V_Mult (&vector_j, 0.5*efield_boom_y, &vector_j) ;
         V_Mult (&vector_k, 0.5*efield_boom_z, &vector_k) ;

         V_Cross (&vector_i, &boom_thrust_x, &boom_torque_x) ;
         V_Cross (&vector_j, &boom_thrust_y, &boom_torque_y) ;
         V_Cross (&vector_k, &boom_thrust_z, &boom_torque_z) ;

         boom_torque_x_mag = V_Mag (&boom_torque_x) ;
         boom_torque_y_mag = V_Mag (&boom_torque_y) ;
         boom_torque_z_mag = V_Mag (&boom_torque_z) ;

         boom_torque_x_t2p = boom_torque_x_mag / boom_power_in ;
         boom_torque_y_t2p = boom_torque_y_mag / boom_power_in ;
         boom_torque_z_t2p = boom_torque_z_mag / boom_power_in ;
   
         fuel_used += fuel_mass_flow * DAY_TO_SEC (incr_time) ; 
      }
   }
}
