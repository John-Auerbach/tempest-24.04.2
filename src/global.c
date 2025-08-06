
/*****************************************************************************/
/*                                                                           */
/*   Module:    global.c                                                     */
/*                                                                           */
/*   Purpose:  	This module controls the TEMPEST global simulation feature.  */
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
#include "global.h"
#include "genorbit.h"


void init_global_sim    ()
{
   if (g_lat_start > g_lat_stop)
   {
      fprintf (stderr, "LAT_START must be less than LAT_STOP\007!\n") ;
      exit (-1) ;
   }

   if (g_lon_start > g_lon_stop)
   {
      fprintf (stderr, "LON_START must be less than LON_STOP\007!\n") ;
      exit (-1) ;
   }

   if (g_lat_incr < 0.0)
   {
      fprintf (stderr, "LAT_INCR must be greater that 0.0\007!\n") ;
      exit (-1) ;
   }

   if (g_lon_incr < 0.0)
   {
      fprintf (stderr, "LON_INCR must be greater that 0.0\007!\n") ;
      exit (-1) ;
   }
}



void perform_global_sim ()
{
   int    step_lon, step_lat ;
   int    num_lon , num_lat  ;
   double curr_lon, curr_lat ;
   int    count              ;
   int    line_len = 0       ;

   num_lon = (int) ((g_lon_stop - g_lon_start) / g_lon_incr) + 1 ;
   num_lat = (int) ((g_lat_stop - g_lat_start) / g_lat_incr) + 1 ;

   fprintf (simul_out, "%d %d\n", num_lon, num_lat) ;

   fprintf (simul_out, "%10.5f %10.5f %10.5f\n", RAD_TO_DEG (g_lon_start),
                        RAD_TO_DEG (g_lon_stop), RAD_TO_DEG (g_lon_incr )) ;

   fprintf (simul_out, "%10.5f %10.5f %10.5f\n", RAD_TO_DEG (g_lat_start),
                        RAD_TO_DEG (g_lat_stop), RAD_TO_DEG (g_lat_incr )) ;

   for (step_lat = 0 ; step_lat < num_lat ; step_lat++) 
   {
      curr_lat = g_lat_start + (double) step_lat * g_lat_incr ;

      for (step_lon = 0 ; step_lon < num_lon ; step_lon++)
      {
         curr_lon = g_lon_start + (double) step_lon * g_lon_incr ;

         sat_r_lla.Lat  = curr_lat   ;
         sat_r_lla.Long = curr_lon   ;
         sat_r_lla.Alt  = global_alt ;

         for (count = 0 ; count < NUM_MODULES ; count++)
            if ((module_list[count].sim_fun != NULL) &&
                (module_list[count].compute_module))
            {
               (*module_list[count].sim_fun) () ; 
            }

         adjust_min_max () ;

         print_sim_results () ;
                                 /* This is here since IDL's READF routine   */
                                 /*    has a buffer length of 2048           */
         line_len += 15 ;
         if (line_len > 2000) 
         {
            fprintf (simul_out, "\n") ;
            line_len = 0 ;
         }
      }
      fprintf (simul_out, "\n") ;
      line_len = 0 ;
   }

   count = 0 ;
   while (display_list [count] != 0)
   {
      if (outvar_list[display_list[count]-1]->param_type == P_REAL)
         fprintf (simul_out,
                   outvar_list[display_list[count]-1]->p_fmt, min_val[count]) ;

      count++ ;
   }

   count = 0 ;
   while (display_list [count] != 0)
   {
      if (outvar_list[display_list[count]-1]->param_type == P_REAL)
         fprintf (simul_out,
                   outvar_list[display_list[count]-1]->p_fmt, max_val[count]) ;

      count++ ;
   }
   fprintf (simul_out, "\n") ;
   fflush  (simul_out) ;

}

