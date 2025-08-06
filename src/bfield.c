/*****************************************************************************/
/*                                                                           */
/*   Module:    bfield.c                                                     */
/*                                                                           */
/*   Purpose:	This module computes near-earth magnetic fields using        */
/*		the IGRF-91 magnetic field model.
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
/*   RCS:	$Id: bfield.c,v 1.6 1996/02/26 11:59:47 nestorv Exp nrv $                                                        */
/*                                                                           */
/*		$Log: bfield.c,v $
 *		Revision 1.6  1996/02/26 11:59:47  nestorv
 *		Added B_END_LVLH and removed delta Bs
 *
 * Revision 1.5  1996/01/31  01:03:26  nestorv
 * Removed hooks for b_mag_lat and b_mag_long.
 *
 * Revision 1.4  1996/01/30  06:15:35  nestorv
 * Added flag to allow secular variations.
 *
 * Revision 1.3  1995/11/22  21:56:10  nestorv
 * Added hooks for b_mag_lat and b_mag_long.
 *
 * Revision 1.2  1995/10/26  14:30:39  nestorv
 * Corrected field tracing routines.
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
#include "genorbit.h"
#include "tether.h"
#include "bfield.h"




/* Given a starting point *From, the function follows the magnetic
   field line passing through *From, until a point of altitude less than
   (if Dir is WHILE_ABOVE) or greater than (if Dir is WHILE_BELOW) or
   equal to ToAlt (in meters) is reached. The point at altitude ToAlt
   is returned in *To, and the function returns SUCCESS. Step is the
   distance for which we assume a constant magnetic field intensity, in
   meters; a negative value allows to trace field lines in the direction
   opposing the magnetic field intensity, i.e backwards tracing. If the
   initial point lies below (if Dir is WHILE_ABOVE) or above (if Dir is
   WHILE_BELOW), ToAlt, then *To is not changed and FAILURE is
   returned. FAILURE might also be returned if the magnetic field line
   tracing reaches a point with zero field intensity. */

int Bfield_Trace_Lat (From,ToLat,Dir,Step,To)
Earth           *From;
double          ToLat;
TRACE_DIRECTION Dir;
double          Step;
Cartesian       *To;
{
  Cartesian B,StepV;
  Spherical RS;
  Earth     RE;
  double    PrevLat;
  Matrix    M;

  if ((From->Lat<ToLat && Dir==WHILE_ABOVE) ||
      (From->Lat>ToLat && Dir==WHILE_BELOW))
    return (-1) ;

  RE=(*From);
  E_ToSpherical(From,&RS);      /* Position in all forms. */
  S_ToCartesian(&RS,To);
#if DEBUG
if (show_debug & DEBUG_BFIELD)
   show_earth (debug_out, "Trace start = ", *From, "\n") ;
#endif
  while ((RE.Lat>ToLat && Dir==WHILE_ABOVE) ||
	 (RE.Lat<ToLat && Dir==WHILE_BELOW)) {
    PrevLat=RE.Lat;
    IGRF_GetBField(&RS,&B);  /* BField in local geocentric coordinates. */
#if DEBUG
if (show_debug & DEBUG_BFIELD)
   show_cartesian (debug_out, "(X->E,Y->N,Z->up) = ", B, 1.0e-9, "\n") ;
#endif
    E_MFromLocalGeoc(&RS,M);
    M_VMult(M,&B,&B);        /* BField in ECR coordinates. */
    if (!V_Unit(&B,&B))      /* Direction of field line. */
      return (-1) ;
    V_Mult(&B,Step,&StepV);  /* Step vector along field line. */
    V_Add(To,&StepV,To);     /* Next position in all forms. */
    S_FromCartesian(To,&RS);
    E_FromSpherical(&RS,&RE);
#if DEBUG
if (show_debug & DEBUG_BFIELD)
   show_earth (debug_out, "current trace pos = ", RE, "\n") ;
#endif
  }
#if DEBUG
if (show_debug & DEBUG_BFIELD)
   show_earth (debug_out, "Trace end = ", RE, "\n") ;
#endif
  if (RE.Lat!=ToLat) {              /* Force final point to altitude ToAlt. */
    Step*=(ToLat-RE.Lat)/(PrevLat-RE.Lat);
    V_Mult(&B,-Step,&StepV);
    V_Add(To,&StepV,To);
  }
  return (0) ;
}


void init_bfield ()
{
                                  /* Set epoch for IGRF calculations         */
   IGRF_SetYear ((double) start_year + DAY_TO_YEAR((double) start_gmt.d)
                                     + MIN_TO_YEAR((double) start_gmt.m) ) ;
                                  /* Make sure that trace step is positive   */
   b_trace_step = fabs (b_trace_step) ;
}



void compute_bfields ()
{
   Spherical igrf_pos  ;           /* Position in spherical coords for IGRF  */

   if (b_igrf_secvar)
   {
      IGRF_SetYear ((double) curr_year + DAY_TO_YEAR((double) curr_gmt.d)
                                       + MIN_TO_YEAR((double) curr_gmt.m) ) ;
   }
                                   /* Convert Satellite position to format   */
                                   /*    required by IGRF routines           */
   igrf_pos.Th =         sat_r_lla.Long ;
   igrf_pos.Ph = HF_PI - sat_r_lla.Lat  ;
   igrf_pos.R  = V_Mag (&sat_r_eci)     ;
                                   /* Computer magnetic field at the 0       */
                                   /*   position on the tether using IGRF    */
   IGRF_GetBField_ord (&igrf_pos, &bfield_loc_gc, b_igrf_order) ;
   E_LocalToLVLH      (&sat_r_eci, &sat_v_eci, &bfield_loc_gc, &bfield_lvlh) ;
   E_LVLHToECI        (&sat_r_eci, &sat_v_eci, &bfield_lvlh  , &bfield_eci ) ;

                                   /* Compute B mag, horiz, incl, decl       */
   bfield_loc_mag = V_Mag (&bfield_loc_gc) ;
   bfield_horiz   = sqrt (bfield_loc_gc.X * bfield_loc_gc.X +
                          bfield_loc_gc.Y * bfield_loc_gc.Y ) ;

   bfield_incl    = atan2 (-1.0*bfield_loc_gc.Z, bfield_horiz   ) ;
   bfield_decl    = atan2 (     bfield_loc_gc.X, bfield_loc_gc.Y) ;

                                   /* Compute ip-plane angle of B            */
   b_angle_ip = atan2 (bfield_lvlh.X, bfield_lvlh.Z) ;

                                   /* Compute out-of-plane angle of B        */
   b_angle_op = atan2 (bfield_lvlh.Y, bfield_lvlh.Z) ;

                                   /* Convert Satellite position to format   */
                                   /*    required by IGRF routines           */
   igrf_pos.Th =         tether_start_r_lla.Long ;
   igrf_pos.Ph = HF_PI - tether_start_r_lla.Lat  ;
   igrf_pos.R  = V_Mag (&tether_start_r_eci)     ;
                                   /* Computer magnetic field @ tether start */
   IGRF_GetBField_ord (&igrf_pos, &bfield_start, b_igrf_order) ;
   bfield_sta_mag =  V_Mag (&bfield_start) ;

                                   /* Convert Satellite position to format   */
                                   /*    required by IGRF routines           */
   igrf_pos.Th =         tether_end_r_lla.Long ;
   igrf_pos.Ph = HF_PI - tether_end_r_lla.Lat  ;
   igrf_pos.R  = V_Mag (&tether_end_r_eci)     ;
                                   /* Computer magnetic field @ tether end   */
   IGRF_GetBField_ord (&igrf_pos, &bfield_end, b_igrf_order) ;
   E_LocalToLVLH      (&sat_r_eci, &sat_v_eci, &bfield_end, &bfield_end_lvlh) ;
   bfield_end_mag =  V_Mag (&bfield_end) ;

                                  /* Trace field line in 1km steps from      */
                                  /*   tether 0 to ground                    */
   if (b_trace_step != 0.0)
   {
      IGRF_Trace (&sat_r_lla, b_trace_alt, WHILE_ABOVE, 
                              b_trace_step, &b_tracep_cart) ;

      E_FromECI (curr_time, &b_tracep_cart, &b_tracep_lla) ;

      IGRF_Trace (&sat_r_lla, b_trace_alt, WHILE_ABOVE,
                              -1.0*b_trace_step, &b_tracem_cart) ;

      E_FromECI (curr_time, &b_tracem_cart, &b_tracem_lla) ;

      if (sat_r_lla.Lat > 0.0)
         Bfield_Trace_Lat (&sat_r_lla, 0.0, WHILE_ABOVE,
                              -1.0*b_trace_step, &b_traceeq_cart) ;
      else
         Bfield_Trace_Lat (&sat_r_lla, 0.0, WHILE_BELOW,
                                   b_trace_step, &b_traceeq_cart) ;

      E_FromECI (curr_time, &b_traceeq_cart, &b_traceeq_lla) ;
   }
}
