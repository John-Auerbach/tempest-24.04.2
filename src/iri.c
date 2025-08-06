/*****************************************************************************/
/*                                                                           */
/*   Module:    iri.h                                                        */
/*                                                                           */
/*   Purpose: 	This module interaces to the IRI-90 code from the NSSDC.     */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   21_Mar_94 NRV   Rewritten.                                   */
/*                                                                           */
/*   RCS:       $Id: iri.c,v 1.10 2001/02/05 03:58:05 nestorv Exp $                                                         */
/*
 *              $Log: iri.c,v $
 *              Revision 1.10  2001/02/05 03:58:05  nestorv
 *              Added error checking for iri model when altitude gets low.
 *
 *              Revision 1.9  2000/10/25 19:34:42  nestorv
 *              Removed IRI_SET_RZ.
 *
 *              Revision 1.8  1997/10/21 01:22:31  nestorv
 *              Revised to be able to set F10.7 and RZ directly.
 *
 * Revision 1.7  1996/02/25  09:02:45  nestorv
 * Added SET_RZ to iri parameters.
 *
 * Revision 1.6  1996/02/10  19:35:47  nestorv
 * Corrected Te at end
 *
 * Revision 1.5  1996/02/06  15:15:24  nestorv
 * Added Ne and Te at tether end.
 *
 * Revision 1.4  1996/02/01  06:10:08  nestorv
 * Added IRI_SET_NE and IRI_SET_TE as inputs.
 *
 * Revision 1.3  1995/09/28  22:06:04  nestorv
 * Changed && to & in show_debug conditional.
 *
 * Revision 1.2  1995/09/28  21:41:47  nestorv
 * Added && DEBUG_IRI to show_debug conditionals and added RCS info to header.
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
#include "iri.h"

#include "f2c.h"

#define ARRAY2D(ar2d,dimx,dimy,x,y)       (ar2d[x+y*dimx-(dimx+1)])

logical iri_flags [12] ;
real    iri_out [11*50] ;
integer jmag, mmdd ;
real    lat, lon, iri_f107, dhour, altb, alte, alts ;
real    iri_out_extras [31] ;
ftnlen  data_path_len ;

void init_iri_ionosphere    ()
{
   iri_flags [ 0] = TRUE_  ; /* Calculate electron density                  */

   iri_flags [ 1] = TRUE_  ; /* Calculate temperatures                      */

   iri_flags [ 2] = TRUE_  ; /* Calculate ion composition                   */

   if (iri_b0_from_table)
      iri_flags [ 3] = TRUE_  ; /* Get B0 from table                        */
   else
      iri_flags [ 3] = FALSE_ ; /* Get B0 from Gulyeava 1987                */

   if (iri_ccir_f2peak)
      iri_flags [ 4] = TRUE_  ; /* Get F2 peak from CCIR-67 model           */
   else
      iri_flags [ 4] = FALSE_ ; /* Get F2 peak from URSI-89 model           */

   iri_flags [ 5] = TRUE_  ; /* Ion composition standard                    */

   iri_flags [ 6] = TRUE_  ; /* Standard IRI topside                        */

   iri_flags [ 7] = TRUE_  ; /* NMF2 Peak Model                             */

   iri_flags [ 8] = TRUE_  ; /* HMF2 Peak Model                             */

   iri_flags [ 9] = TRUE_  ; /* Te model                                    */

   iri_flags [10] = TRUE_  ; /* Ne standard model                           */

   iri_flags [11] = FALSE_ ; /* Messages are written to Unit 12 ?????       */

   jmag           =      0 ; /* Latitude and Longitude are Geodetic         */

   data_path_len  = (ftnlen) strlen (data_path) ;

   if (iri_set_ne > 0.0 || iri_set_te > 0.0)
      if (iri_set_ne <= 0.0 || iri_set_te <= 0.0)
         fprintf (stderr, "WARNING: %s!!\n",
                  "Need to set both IRI_SET_NE and IRI_SET_TE") ;
}



void compute_iri_ionosphere ()
{
   int alt_ind        =  0 ;
   int count1, count2      ;

   if (iri_set_ne > 0.0 || iri_set_te > 0.0)
   {
      iri_elect_density   = iri_set_ne ;
      iri_elect_temp      = iri_set_te ;

      iri_neutral_temp    = 0.0 ;

      iri_ion_density     = iri_set_ne ;
      iri_ion_temp        = iri_set_te ;

      iri_rel_perc_o_ion  = 0.0 ;
      iri_rel_perc_h_ion  = 0.0 ;
      iri_rel_perc_he_ion = 0.0 ;
      iri_rel_perc_o2_ion = 0.0 ;
      iri_rel_perc_no_ion = 0.0 ;

      iri_density_o_ion   = 0.0 ;
      iri_density_h_ion   = 0.0 ;
      iri_density_he_ion  = 0.0 ;
      iri_density_o2_ion  = 0.0 ;
      iri_density_no_ion  = 0.0 ;
   }
   else
   {
      lat    = (real) (sat_r_lla.Lat  * R_D_CONST) ;
      lon    = (real) (sat_r_lla.Long * R_D_CONST) ;

      altb     = (real) (sat_r_lla.Alt / 1000.0) ;
      alts     =                            1.0  ;
      alte     =         altb          +    1.0  ;

      iri_f107 = -1.0 * f107_daily            ;

      mmdd     = (integer) (-1 * curr_gmt.d) ;

      dhour    = (real) local_time_h ;
#if DEBUG
if (show_debug & DEBUG_IRI)
   fprintf (debug_out, "lat=%f lon=%f f107=%f mmdd=%d dhour=%f alt=%f %f %f\n",
                        lat, lon, iri_f107, mmdd, dhour, altb, alte, alts) ;
#endif
      iris12_ (data_path,
               iri_flags, &jmag, &lat, &lon, &iri_f107, &mmdd, &dhour,
               &altb, &alte, &alts, iri_out, iri_out_extras,
               data_path_len) ; 

      iri_elect_density   = (double) iri_out [alt_ind * 11 +  0] ;
      iri_neutral_temp    = (double) iri_out [alt_ind * 11 +  1] ;
      iri_ion_temp        = (double) iri_out [alt_ind * 11 +  2] ;
      iri_elect_temp      = (double) iri_out [alt_ind * 11 +  3] ;

      iri_rel_perc_o_ion  = (double) iri_out [alt_ind * 11 +  4] ;
      iri_rel_perc_h_ion  = (double) iri_out [alt_ind * 11 +  5] ;
      iri_rel_perc_he_ion = (double) iri_out [alt_ind * 11 +  6] ;
      iri_rel_perc_o2_ion = (double) iri_out [alt_ind * 11 +  7] ;
      iri_rel_perc_no_ion = (double) iri_out [alt_ind * 11 +  8] ;

      iri_density_o_ion   = iri_elect_density * iri_rel_perc_o_ion  * 0.01 ;
      iri_density_h_ion   = iri_elect_density * iri_rel_perc_h_ion  * 0.01 ;
      iri_density_he_ion  = iri_elect_density * iri_rel_perc_he_ion * 0.01 ;
      iri_density_o2_ion  = iri_elect_density * iri_rel_perc_o2_ion * 0.01 ;
      iri_density_no_ion  = iri_elect_density * iri_rel_perc_no_ion * 0.01 ;

      iri_ion_density     = iri_density_o_ion  + iri_density_h_ion  +
                            iri_density_he_ion + iri_density_o2_ion +
                            iri_density_no_ion                      ;

      if (iri_elect_density < 0.0 || iri_elect_temp < 0.0 || iri_ion_temp < 0.0)
         iri_elect_density = iri_elect_temp = iri_ion_temp = 0.0 ;
        
#if DEBUG
if (show_debug & DEBUG_IRI)
   fprintf (debug_out, "IRI Tstart Ne=%e Te=%e Ti=%e\n",
            iri_elect_density, iri_elect_temp, iri_ion_temp) ;
#endif

      lat    = (real) (tether_end_r_lla.Lat  * R_D_CONST) ;
      lon    = (real) (tether_end_r_lla.Long * R_D_CONST) ;

      altb     = (real) (tether_end_r_lla.Alt / 1000.0) ;
      alts     =                                   1.0  ;
      alte     =                altb          +    1.0  ;

#if DEBUG
if (show_debug & DEBUG_IRI)
   fprintf (debug_out, "lat=%f lon=%f f107=%f mmdd=%d dhour=%f alt=%f %f %f\n",
                        lat, lon, iri_f107, mmdd, dhour, altb, alte, alts) ;
#endif
      iris12_ (data_path,
               iri_flags, &jmag, &lat, &lon, &iri_f107, &mmdd, &dhour,
               &altb, &alte, &alts, iri_out, iri_out_extras,
               data_path_len) ;

      iri_ne_end = (double) iri_out [alt_ind * 11 +  0] ;
      iri_te_end = (double) iri_out [alt_ind * 11 +  3] ;

      if (iri_ne_end < 0.0 || iri_te_end < 0.0)
         iri_ne_end = iri_te_end = 0.0 ;
#if DEBUG
if (show_debug & DEBUG_IRI)
   fprintf (debug_out, "IRI Tend Ne=%e Te=%e\n",
            iri_ne_end, iri_te_end) ;
#endif

   }
}
