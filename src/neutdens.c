/*****************************************************************************/
/*                                                                           */
/*   Module:    neutdens.h                                                   */
/*                                                                           */
/*   Purpose:	This module interfaces to the MSIS-86 and MSISE90 models     */
/*		that were provided by the NSSDC.                             */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   26_Mar_94 NRV   Rewritten.                                   */
/*                                                                           */
/*   RCS:       $Id: neutdens.c,v 1.4 2005/06/07 00:11:52 voronka Exp nrv $                                                         */
/*                                                                           */
/*              $Log: neutdens.c,v 
 *              Revision 1.3  1997/08/09 21:38:14  nestor
 *              Added debugging in effort to determine drag problem.
 *
 * Revision 1.2  1995/09/28  21:42:17  nestorv
 *  Added && DEBUG_NEUTDENS to show_debug conditionals and added RCS info to
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
#include "genorbit.h"
#include "solarmag.h"
#include "neutdens.h"

#include "f2c.h"

integer    yyddd         ;
real       utsec         ;
real       altitude      ;
real       geod_lat      ;
real       geod_long     ;
real       loc_sol_time  ;
real       f107_3ma      ;
real       f107_d        ;
real       daily_ap [7]  ;
integer    msis_mass     ;
real       dens_out [8]  ;
real       temp_out [2]  ;
ftnlen     data_path_len ;



void init_neutral_densities    ()
{
   integer meter_true ;

   msis_mass = 48 ;                 /* Compute all densities and mass        */

   meter_true = TRUE_ ;
   meters_ (&meter_true) ;          /* Results from MSIS86  will be in KG&M  */

   meter_true = TRUE_ ;
   meter6_ (&meter_true) ;          /* Results from MSISE90 will be in KG&M  */

   daily_ap [0] = (real) mag_ind_ap ;
   daily_ap [1] = (real) mag_ind_ap ;
   daily_ap [2] = (real) mag_ind_ap ;
   daily_ap [3] = (real) mag_ind_ap ;
   daily_ap [4] = (real) mag_ind_ap ;
   daily_ap [5] = (real) mag_ind_ap ;
   daily_ap [6] = (real) mag_ind_ap ;

   data_path_len = (ftnlen) strlen (data_path) ;

   cum_flux_ao = 0.0 ;
}



void compute_neutral_densities ()
{
   yyddd = (integer) (1000 * (curr_year % 100) + curr_gmt.d) ;

   utsec = (real) GMT_Secs (&curr_gmt) ;

   altitude  = (real) (sat_r_lla.Alt  / 1000.0   ) ;
   geod_lat  = (real) (sat_r_lla.Lat  * R_D_CONST) ;
   if (sat_r_lla.Long < 0.0)
      geod_long = (real) (sat_r_lla.Long * R_D_CONST + 360.0) ;
   else
      geod_long = (real) (sat_r_lla.Long * R_D_CONST) ;

   loc_sol_time = (real) local_time_h            ;
   f107_3ma     = (real) f107_3mo_ave            ;
   f107_d       = (real) f107_daily              ;
#if DEBUG
if (show_debug && DEBUG_NEUTDENS)
{
   fprintf (debug_out, "data_path_len={%s}\n", data_path) ;
   fprintf (debug_out, "yyddd=%d utsec=%f\n", (int) yyddd, utsec) ;
   fprintf (debug_out, "altitude=%f geod_lat=%f geod_long=%f\n",
                        altitude, geod_lat, geod_long) ;
   fprintf (debug_out, "loc_sol_time=%f f107_3ma=%f f107_d=%f\n",
                        loc_sol_time, f107_3ma, f107_d) ;
}
#endif
   if (altitude < 85.0)
   {                                /* Need to use MSISE-90 (0 to 400km)     */
      gts6_ (&yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time, 
             &f107_3ma, &f107_d, daily_ap, &msis_mass,
             dens_out, temp_out) ;
      strcpy (neutdens_model, neutdens_msise90) ;
   }
   else if (altitude > 400.0)
   {                                /* Need to use MSIS-86 (85 to 1000km)    */
      gts5_ (data_path, 
             &yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time,
             &f107_3ma, &f107_d, daily_ap, &msis_mass,
             dens_out, temp_out,
             data_path_len) ; 
      strcpy (neutdens_model, neutdens_msis86) ;
   }
   else
   {
      if (neutdens_prefer_msise_90)
      {
         gts6_ (&yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time,
                &f107_3ma, &f107_d, daily_ap, &msis_mass,
                dens_out, temp_out) ;
         strcpy (neutdens_model, neutdens_msise90) ;
      }
      else
      {
         gts5_ (data_path,
                &yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time,
                &f107_3ma, &f107_d, daily_ap, &msis_mass,
                dens_out, temp_out,
                data_path_len) ;
         strcpy (neutdens_model, neutdens_msis86) ;
      }
   }

                                    /* Put vals from output array to globals */
   neutdens_numb_he    = (double) dens_out [0] ;
   neutdens_numb_o     = (double) dens_out [1] ;
   neutdens_numb_n2    = (double) dens_out [2] ;
   neutdens_numb_o2    = (double) dens_out [3] ;
   neutdens_numb_ar    = (double) dens_out [4] ;
   neutdens_numb_h     = (double) dens_out [6] ;
   neutdens_numb_n     = (double) dens_out [7] ;

   neutdens_tot_mass   = (double) dens_out [5] ;

   neutdens_temp_exos  = (double) temp_out [0] ;

   neutdens_temp_atalt = (double) temp_out [1] ;

                                    /* Integrate AO density over time to get flux */
   cum_flux_ao += neutdens_numb_o * sat_v_eci_mag * incr_time * (24.0 * 60.0 * 60.0) ;

#if DEBUG
if (show_debug && DEBUG_NEUTDENS)
{
   fprintf (debug_out, "neutdens_numb_he  = %e\n", neutdens_numb_he) ;
   fprintf (debug_out, "neutdens_numb_o   = %e\n", neutdens_numb_o ) ;
   fprintf (debug_out, "neutdens_numb_n2  = %e\n", neutdens_numb_n2) ;
   fprintf (debug_out, "neutdens_numb_o2  = %e\n", neutdens_numb_o2) ;
   fprintf (debug_out, "neutdens_numb_ar  = %e\n", neutdens_numb_ar) ;
   fprintf (debug_out, "neutdens_numb_h   = %e\n", neutdens_numb_h ) ;
   fprintf (debug_out, "neutdens_numb_n   = %e\n", neutdens_numb_n ) ;
   fprintf (debug_out, "neutdens_tot_mass = %e\n", neutdens_tot_mass) ;
}
#endif
}
