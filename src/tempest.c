/*****************************************************************************/
/*                                                                           */
/*   Module:    tempest.c                                                    */
/*                                                                           */
/*   Purpose:	This is the main program of TEMPEST.                         */
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
/*   RCS:       $Id: tempest.c,v 1.35 2006/11/09 19:30:18 voronka Exp $                                                         */
/*                                                                           */
/*              $Log: tempest.c,v 
 *              Revision 1.31  1999/09/30 02:35:24  nestor
 *              Move module initialization back before time stuff.
 *
 * Revision 1.30  1999/04/02  18:05:55  nestorv
 * Moved module initialization after time stuff.
 *
 * Revision 1.29  1997/10/21  20:18:06  nestorv
 * Added output of plotl %title variable.
 *
 * Revision 1.28  1997/05/09  23:52:48  nestorv
 * Fixed BINARIFIED header output.
 *
 * Revision 1.27  1997/05/09  19:59:24  nestorv
 * Added MOD_BARE_TETHER.
 *
 * Revision 1.26  1997/05/09  15:37:01  nestorv
 * fixed some array_length stuff.
 *
 * Revision 1.25  1996/10/27  22:42:26  nestorv
 * Typo in signal code corrected.
 *
 * Revision 1.24  1996/10/27  22:35:20  nestorv
 * Added traps for the Cntrl-C and kill signal.
 *
 * Revision 1.23  1996/10/27  02:10:48  nestorv
 * Added WARNING statement when system decays.
 *
 * Revision 1.22  1996/10/27  01:06:15  nestorv
 * Added beep when to Stopping message.
 *
 * Revision 1.21  1996/07/02  20:34:59  nestorv
 * corrected label index in binarified file.
 *
 * Revision 1.20  1996/03/29  22:48:44  nestorv
 * Made provisions for comments (binarified and ASCII modes.)
 *
 * Revision 1.19  1996/02/25  02:09:39  nestorv
 * Fixed to work with only 1 TTP.
 *
 * Revision 1.18  1996/02/18  06:46:14  nestorv
 * Changed default lib_gmt and dply_gmt to curr_gmt.
 *
 * Revision 1.17  1996/02/06  15:10:59  nestorv
 * Made IRI dependent on TETHER as well.
 *
 * Revision 1.16  1995/11/07  16:55:00  nestorv
 * Changed DAY_FromGMT Day_FromGMT.
 *
 * Revision 1.15  1995/11/06  21:08:20  nestorv
 * Added TEMPEST_TTPS env parameter check.
 *
 * Revision 1.14  1995/11/01  05:35:45  nestorv
 * Improved accuracy of MET.  Dealt with lib_gmt and dply_gmt.
 *
 * Revision 1.13  1995/10/30  21:32:21  nestorv
 * Added check in ttp while to see if there are no more ttps to set.
 *
 * Revision 1.12  1995/10/27  07:03:57  nestorv
 * Added PLASMA module.
 *
 * Revision 1.11  1995/10/27  05:58:35  nestorv
 * Added call to sort_ttag_param_entries ().
 *
 * Revision 1.10  1995/10/27  02:54:37  nestorv
 * Allow for more that one TTP with the same time.
 *
 * Revision 1.9  1995/10/24  21:13:59  nestorv
 * Removed #include <time.h>.
 *
 * Revision 1.8  1995/10/24  20:11:30  nestorv
 * Added code for -REALTIME and -REALSYNC options for TEMPEST realtime
 * operations.
 *
 * Revision 1.7  1995/10/10  19:51:34  nestorv
 * Added time0 for libration to TETHER module.
 *
 * Revision 1.6  1995/10/10  05:33:02  nestorv
 * Corrected display of stop time.
 *
 * Revision 1.5  1995/10/07  22:05:46  nestorv
 * Revised to allow for ephem_time > start_time.
 *
 * Revision 1.4  1995/10/07  05:29:39  nestorv
 * Corrected MET being negative problem.
 *
 * Revision 1.3  1995/10/04  19:51:43  nestorv
 * Added time-tagged parameter file capability.
 *
 * Revision 1.2  1995/09/28  21:33:06  nestorv
 * Added && DEBUG_MODULES to show_debug conditionals and RCS info into header.
 *                                                        */
/*****************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>

#include "advmath.h"
#include "earth.h"
#include "orbit.h"

#define TEMPEST_MAIN 1

#include "types.h"
#include "gmt.h"
#include "genorbit.h"
#include "tether.h"
#include "bfield.h"
#include "emf.h"
#include "iri.h"
#include "solarmag.h"
#include "neutdens.h"
#include "tempest.h"
#include "global.h"
#include "tss_current.h"
#include "plasma.h"
#include "bare_tether.h"

#include "temputil.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void init_simulation (int argc, char *argv [])
{
   int        count  ;             /* Counter for loops                      */
   struct tm  tm_gmt ;
   struct tm *tm_loc ;
   GMT        mmet_gmt ;
   int        mmet_year ;
   char       line [400] ;
   int        index  ;
   int        count2 ;

   if ((comments = malloc (65535)) == NULL)
   {
      fprintf (stderr, "\nERROR:  problems allocating space\007!!\n\n") ;
      exit (-1) ;
   }
                                   /* Display title and header               */
   fprintf (stderr, 
      "\nTEMPEST V%s (compiled on %s %s)\n", VERSION, __DATE__, __TIME__) ;
   sprintf (comments,"! TEMPEST V%s (compiled on %s %s)\n",
            VERSION, __DATE__, __TIME__) ;
   fprintf (stderr, 
      "===========================================================\n\n") ;

                                   /* Get path for IRI, MSIS etc. data files */
   if ((data_path = getenv ("TEMPEST_DATA")) == NULL)
   {
      data_path = malloc (2)   ;
      strcpy (data_path, "./") ;
   }
   sprintf (line, "! TEMPEST_DATA = %s\n", data_path) ;
   strcat  (comments, line) ;
                                   /* Get path for TTP files                 */
   if ((ttp_path = getenv ("TEMPEST_TTPS")) == NULL)
   {
      ttp_path = malloc (2)   ;
      strcpy (ttp_path, "./") ;
   }
   sprintf (line, "! TEMPEST_TTPS = %s\n", ttp_path) ;
   strcat  (comments, line) ;
                                   /* Make param and output lists            */
   make_lists () ;
                                   /* Process command line parameters        */
   sprintf (line, "! %s", argv[0]) ;
   for (count = 1 ; count < argc ; count++)
   {
      strcat (line, " ") ;
      strcat (line, argv[count]) ;
   }
   strcat (line, "\n") ;
   strcat (comments, line) ;
   process_cmdline_params (argc, argv) ;
                                   /* Sort time-tagged parameter list        */
   sort_ttag_param_entries () ;
                                   /* If there are not output variables-help */
   if (display_list [0] == 0)
   {
      fprintf (stderr, "No outputs requested\007!\n") ;

      show_simulation_help () ;
      exit (-1) ;
   }
                                   /* Add y column labels to comments        */
   count  = 0 ;
   count2 = 1 ;
   while (display_list [count] != 0)
   {
      if (outvar_list[display_list[count]-1]->param_type == P_REAL_ARRAY)
      {
         for (index = 0 ;
              index < (*(outvar_list[display_list[count]-1]->array_length)) ;
              index++)
         {
            sprintf (line, "%%ylabel%d=%s [%d]\n", count2, 
               outvar_list[display_list[count]-1]->plotl_label, index) ;
            strcat (comments, line) ;
            count2++ ;
         }
      }
      else
      {
         sprintf (line, "%%ylabel%d=%s\n", count2, 
            outvar_list[display_list[count]-1]->plotl_label) ;
         strcat (comments, line) ;
         count2++ ;
      }
      count++ ;
   }
   sprintf (line, "%%title=%s\n", plotl_title) ;
   strcat (comments, line) ;
                                   /* Open appropriate files if needed       */
   init_file_output () ;
                                   /* Convert times from GMT format          */
   start_time = Day_FromGMT (start_year, &start_gmt) ;
   stop_time  = Day_FromGMT ( stop_year, &stop_gmt ) ;
   incr_time  = Day_FromGMT (      1900, &incr_gmt ) ;

   if (start_time > stop_time)
   {
      fprintf (stderr, "ERROR: Stop time is earlier than start time\007!\n") ;
      exit (-1) ;
   }
                                   /* Initialize modules                     */
   for (count = 0 ; count < NUM_MODULES ; count++)
   {
      if (module_list[count].init_fun != NULL)
      {
         (*module_list[count].init_fun) () ;
      }
   }

                                  /* Set start time appropriately            */
   if (start_time > ephem_time)
   {
      bcopy (&ephem_gmt, &curr_gmt, sizeof (GMT)) ;
      curr_year = ephem_year ;
      curr_time = ephem_time ;
   }
   else
   {
      bcopy (&start_gmt, &curr_gmt, sizeof (GMT)) ;
      curr_year = start_year ;
      curr_time = start_time ;
   }
                                   /* Set MET0 time to start time if blank   */
   if (met_gmt.d == -1) 
   {
      bcopy (&curr_gmt, &met_gmt, sizeof (GMT)) ;
      met_year = curr_year ;
   }
                                   /* Compute MET time for start of sim      */
   bcopy (&curr_gmt, &mmet_gmt, sizeof (GMT)) ;
   mmet_year = curr_year ;

   GMT_Decr (&mmet_gmt, &mmet_year, &met_gmt) ;
   if (mmet_year >= curr_year)
      mmet_year -= curr_year ;

   bcopy (&mmet_gmt, &met_gmt, sizeof (GMT)) ;
   met_year = mmet_year + 1900 ;
   met_time = Day_FromGMT (met_year, &met_gmt) ;

                                   /* Set DPLY0 time to start time if blank  */
   if (dply_year == -1)
      dply_year = curr_year ;
   if (dply_gmt.d == -1)
   {
      bcopy (&curr_gmt, &dply_gmt, sizeof (GMT)) ;
      dply_year = curr_year ;
      dply_time = curr_time ;
   }
                                   /* Set LIB0 time to start time if blank   */
   if (lib_year == -1)
      lib_year = curr_year ;
   if (lib_gmt.d == -1)
   {
      bcopy (&curr_gmt, &lib_gmt, sizeof (GMT)) ;
      lib_year = curr_year ;
      lib_time = curr_time ;
   }

#if DEBUG
if (show_debug & DEBUG_TEMPUTIL)
   fprintf (debug_out, "MET = %s %d = %f\n", Str_FromGMT (&met_gmt), met_year,
                        met_time) ;
#endif
                                   /* Do additional work if in realtime mode */
   if (delta_out_time > (time_t) 0)
   {
      delta_out_time =  (time_t) 
                        ((double) sub_samp_out * incr_time * 86400.0) ;
      if (delta_out_time < (time_t) 1)
      {
         fprintf (stderr,"WARNING: Realtime increment set to 1 second!!\007\n");
         delta_out_time = (time_t) 1 ;
      }
                                   /* Syncronize sim GMT to current GMT      */
      if (next_out_time > (time_t) 0)
      {
                                   /* Get local time and timezone info       */
         next_out_time = time ((time_t *) NULL) ;
         tm_loc = localtime (&next_out_time) ;

                                   /* Convert GMT to time in time_t format   */
         tm_gmt.tm_year  = start_year - 1900 ;
         GMT_MonthDay (&start_gmt, start_year, &tm_gmt.tm_mon, &tm_gmt.tm_mday);
         tm_gmt.tm_mon-- ;
         tm_gmt.tm_hour  = start_gmt.h ;
         tm_gmt.tm_min   = start_gmt.m ;
         tm_gmt.tm_sec   = start_gmt.s ;
         tm_gmt.tm_isdst = -1 ;
         
         next_out_time = mktime (&tm_gmt) + tm_loc->tm_gmtoff ;
      }
      else
         next_out_time  = time ((time_t *) NULL) + delta_out_time ;
#if DEBUG
if (show_debug & DEBUG_TEMPUTIL)
   fprintf (debug_out, "next = %u delta = %u\n",
                        next_out_time, delta_out_time) ;
   fprintf (debug_out, "curtime = %u\n", time ((time_t *) NULL)) ;
#endif
   }
                                   /* Show simulation parameters             */
   if (show_params)
      show_parameters () ; 
                                   /* Print tabular header                   */
   if (show_header)
      show_headers () ;

   if (start_time < ephem_time)
   {
      fprintf (stderr, "WARNING: Start time is earlier than ephemeris time\007!\n") ;
   }

   free (comments) ;
}

 

void adjust_module_dependencies () 
{
   int count     ;                /* Counter for loops                       */

#if DEBUG
if (show_debug & DEBUG_MODULES)
{
   fprintf (debug_out, "BEFORE adjusting dependencies\n") ;
   for (count = 0 ; count < NUM_MODULES ; count++)
      if (module_list [count].compute_module)
         fprintf(debug_out,"will compute module %s\n",module_list[count].token);
}
#endif
   if (module_list [MOD_BARE_TETHER].compute_module)
   {
      module_list [MOD_EMF     ].compute_module = TRUE ;
      module_list [MOD_BFIELD  ].compute_module = TRUE ;
      module_list [MOD_TETHER  ].compute_module = TRUE ;
      module_list [MOD_IRI     ].compute_module = TRUE ;
   }
   
   if (module_list [MOD_PLASMA].compute_module)
   {
      module_list [MOD_IRI     ].compute_module = TRUE ;
      module_list [MOD_BFIELD  ].compute_module = TRUE ;
   }

   if (module_list [MOD_TSS_CURRENT].compute_module)
   {
      module_list [MOD_EMF     ].compute_module = TRUE ;
      module_list [MOD_BFIELD  ].compute_module = TRUE ;
      module_list [MOD_TETHER  ].compute_module = TRUE ;
      module_list [MOD_IRI     ].compute_module = TRUE ;
   }   

   if (module_list [MOD_GENORBIT].compute_module && orbit_decay)
      module_list [MOD_NEUTDENS].compute_module = TRUE ;

   if (module_list [MOD_IRI     ].compute_module)
   {
      module_list [MOD_SOLARMAG].compute_module = TRUE ;
      module_list [MOD_TETHER  ].compute_module = TRUE ;
   }

   if (module_list [MOD_NEUTDENS].compute_module)
      module_list [MOD_SOLARMAG].compute_module = TRUE ;

   if (module_list [MOD_EMF     ].compute_module)
   {
      module_list [MOD_BFIELD  ].compute_module = TRUE ;
      module_list [MOD_TETHER  ].compute_module = TRUE ;
   }
   if (module_list [MOD_BFIELD ].compute_module)
      module_list [MOD_TETHER  ].compute_module = TRUE ;
#if DEBUG
if (show_debug & DEBUG_MODULES)
{
   fprintf (debug_out, "AFTER adjusting dependencies\n") ;
   for (count = 0 ; count < NUM_MODULES ; count++)
      if (module_list [count].compute_module)
         fprintf(debug_out,"will compute module %s\n",module_list[count].token);
}
#endif
}



static void signal_handler (int sig_number)
{
   if ((sig_number == SIGINT) || (sig_number == SIGTERM))
      prog_interrupt = TRUE ;      
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



main (int argc, char *argv [])
{
   int    count     ;             /* Counter for loops                       */
   int    count2    ;             /* Counter for loops                       */
   int    ss_count  ;             /* Counter for decimated/sub-sample output */
   GMT    jan1_gmt  = {1, 0, 0, 0, 0} ;
   double jan1_day  ;

                                  /* Initialize orbital and other parameters */
                                  /*    and process command line parameters  */
   init_simulation (argc, argv) ;
                                   /* Computations to have GMT in seconds    */
   jan1_day  = Day_FromGMT (  curr_year, &jan1_gmt  ) ;
   curr_time_year = curr_time - jan1_day ;
   time_years = curr_time/365.25 + 1900.0 ;

   jan1_day  = Day_FromGMT ( 1970, &jan1_gmt) ;
   pc_time = curr_time - jan1_day ;
                                  /* Adjust selection of which modules used  */
   adjust_module_dependencies () ;

   if (global_sim)
   {
      init_global_sim    () ;

      perform_global_sim () ;
   }
   else
   {                              /* Set up time fot t=t0+1*ts               */
      bcopy (&curr_gmt, &time1_gmt, sizeof (GMT)) ;
      time1_year = curr_year ;
      GMT_Incr (&time1_gmt, &time1_year, &incr_gmt) ;

                                  /* Print message that starting simulation  */
      fprintf (stderr, "\nStarting simulation at %s %d\n", 
         Str_FromGMT (&curr_gmt), curr_year) ;
                                  /* Initialize sub-sampling counter         */
      ss_count = sub_samp_out - 1 ;

      if (num_ttag_entries > 0)
      {
         if (ttag_param_index [0] == TTG_BY_MET)
            curr_ttag_entry_time = Day_FromGMT (met_year ,
                                 (GMT *)ttag_param_list [curr_ttag_entry][0]) ;
         else
            curr_ttag_entry_time = Day_FromGMT (curr_year,
                                 (GMT *)ttag_param_list [curr_ttag_entry][0]) ;
      }

                                  /* Set up signal handlers                 */
      if (signal (SIGINT,  signal_handler) == SIG_ERR)
         fprintf (stderr,
            "\nWARNING:  Unable to catch Cntl-C (SIGINT) signal!\007\n") ;
      if (signal (SIGTERM, signal_handler) == SIG_ERR)
         fprintf (stderr,
            "\nWARNING:  Unable to catch kill (SIGTERM) signal!\007\n") ;

      while ((curr_time <= stop_time) && (orbit_decayed == FALSE) &&
             (prog_interrupt == FALSE))
      {
         if ((num_ttag_entries > 0) && (curr_ttag_entry < num_ttag_entries))
         {
            if (ttag_param_index [0] == TTG_BY_MET)
            {
               while ((met_time  - curr_ttag_entry_time) >= 0.0 &&
                      (curr_ttag_entry < num_ttag_entries))
                  set_param_ttag_entry () ;
            }
            else
            {
               while ((curr_time - curr_ttag_entry_time) >= 0.0 &&
                      (curr_ttag_entry < num_ttag_entries))
                  set_param_ttag_entry () ;
            }
         }

         if (curr_time >= ephem_time)
         {
            for (count = 0 ; count < NUM_MODULES ; count++)
            {
               if ((module_list[count].sim_fun != NULL) &&
                   (module_list[count].compute_module))
               {
#if DEBUG
if (show_debug & DEBUG_MODULES)
   fprintf (debug_out,"entering module %s\n", module_list[count].token) ;
#endif
                  (*module_list[count].sim_fun) () ;
#if DEBUG
if (show_debug & DEBUG_MODULES)
   fprintf (debug_out,"leaving  module %s\n", module_list[count].token) ;
#endif
               }
            }
                                  /* Print results                           */
            if (curr_time >= start_time)
            {   
               if (++ss_count == sub_samp_out)
               {
                  print_sim_results () ;
                  ss_count = 0 ;
               }
            }
                                  /* Adjust minimum and maximum values       */
            if (show_min_max)
               adjust_min_max () ;
         }
         else
         {
            if (++ss_count == sub_samp_out)
            {
               print_zero_results () ;
               ss_count = 0 ;
            }
         }
                                  /* Increment times                         */
         GMT_Incr (&curr_gmt, &curr_year, &incr_gmt) ;
         curr_time = Day_FromGMT (curr_year, &curr_gmt) ;

         GMT_Incr (&met_gmt, &met_year, &incr_gmt) ;
         met_time  = Day_FromGMT (met_year, &met_gmt) ;

         jan1_day  = Day_FromGMT (  curr_year, &jan1_gmt  ) ;
         curr_time_year = curr_time - jan1_day ;
         time_years = curr_time/365.25 + 1900.0 ;

         jan1_day  = Day_FromGMT ( 1970, &jan1_gmt) ;
         pc_time = curr_time - jan1_day ;
      }

      if (orbit_decayed)
         fprintf (stderr, "\007\nWARNING: System has reentered!!\n") ;

      if (prog_interrupt)
         fprintf (stderr, "\007\nWARNING: Interrupt signal received!!\n") ;
      GMT_Decr (&curr_gmt, &curr_year, &incr_gmt) ;
      fprintf (stderr, "\nStopping simulation at %s %d\007\n\n",
         Str_FromGMT(&curr_gmt), curr_year) ;
   }

                                  /* Free up allocated memory                */
   if (strcmp (data_path, "./") == 0)
      free (data_path) ;

   for (count = 0 ; count < MAX_TTAG_ENTRIES ; count++)
      for (count2 = 0 ; count2 < MAX_TTAG_PARAMS ; count2++)
         if (ttag_param_list [count] [count2] != NULL)
            free (ttag_param_list [count] [count2]) ;

                                  /* Show extreme values if requested        */
   if (show_min_max)
      print_min_max () ;
                                  /* Show plotl y-axis labels if requested   */
   if (plotl_labels)
      show_plotl_labels () ;

   close_file_output () ;

   exit (0) ;
}
