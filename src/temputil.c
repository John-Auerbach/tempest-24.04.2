/*****************************************************************************/
/*                                                                           */
/*   Module:    temputil.c                                                   */
/*                                                                           */
/*   Purpose:	This module contains various TEMPEST utility functions.      */
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
/*   RCS:       $Id: temputil.c,v 1.27 2003/07/01 22:52:48 nrv Exp $                                                        */
/*                                                                           */
/*              $Log: temputil.c,v $
 *              Revision 1.27  2003/07/01 22:52:48  nrv
 *              Removed reference to malloc.h
 *
 *              Revision 1.26  1997/06/06 04:00:40  nestorv
 *              Added reference to bare_tether.h.
 *
 * Revision 1.25  1997/05/09  15:37:18  nestorv
 * Making arrays print
 *
 * Revision 1.24  1996/10/27  00:53:59  nestorv
 * Added fflush after labels are written out.
 *
 * Revision 1.23  1996/03/29  22:48:44  nestorv
 * Made provisions for comments (binarified and ASCII modes.)
 *
 * Revision 1.22  1996/02/18  07:52:20  nestorv
 * Adjusted setting of ,METs to find , with index and not strok.
 *
 * Revision 1.21  1996/02/18  06:48:38  nestorv
 * Changed all settings of P_GMT to allow ,MET
 *
 * Revision 1.20  1996/02/16  02:35:04  nestorv
 * Added ',MET' qualifier to input times in GMT format.
 *
 * Revision 1.19  1996/02/10  20:26:05  nestorv
 * Added checking for NaN.
 *
 * Revision 1.18  1996/01/30  18:58:42  nestorv
 * Updated way gsetime is stored.
 *
 * Revision 1.17  1995/11/22  00:44:43  nestorv
 * Added -GSETIME flag and associated processing.
 *
 * Revision 1.16  1995/11/07  16:53:39  nestorv
 * Changed DAY_FromGMT Day_FromGMT.
 *
 * Revision 1.15  1995/11/06  21:08:40  nestorv
 * Added TEMPEST_TTPS environtal variable for path for TTPs.
 *
 * Revision 1.14  1995/11/03  08:20:58  nestorv
 * Correctly fixed MET/FET equivalence.
 *
 * Revision 1.13  1995/11/02  22:16:37  nestorv
 * Allow MET and FET to be interchangable.
 *
 * Revision 1.12  1995/11/02  18:03:25  nestorv
 * Allowed read_in_line to filter empty lines.
 *
 * Revision 1.11  1995/10/31  02:03:05  nestorv
 * Correct problem in sort_ttag when num_ttag_params == 0.
 *
 * Revision 1.10  1995/10/27  05:59:25  nestorv
 * Added the sort_ttag_param_entries () routine.
 *
 * Revision 1.9  1995/10/26  21:33:04  nestorv
 * Updated code for realtime operations.
 *
 * Revision 1.8  1995/10/24  20:11:30  nestorv
 * Added code for -REALTIME and -REALSYNC options for TEMPEST realtime
 * operations.
 *
 * Revision 1.7  1995/10/23  17:48:16  nestorv
 * Changed output with -binarify option from '$P$\n' to '$P\r\n' per plotl
 * binarify spec.
 *
 * Revision 1.6  1995/10/10  19:51:34  nestorv
 * Added time0 for libration to TETHER module.
 *
 * Revision 1.5  1995/10/09  00:34:08  nestorv
 * Updated to provide for correct min/max adjustment.
 *
 * Revision 1.4  1995/10/07  22:05:46  nestorv
 * Revised to allow for ephem_time > start_time.
 *
 * Revision 1.3  1995/10/07  18:12:54  nestorv
 * Added -binarify option to produce binarified output for plotl.
 *
 * Revision 1.2  1995/10/04  19:51:43  nestorv
 * Added time-tagged parameter file capability.
 *                                                       */
/*                                                                           */
/*****************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*  Actually part of libc now 
#include <malloc.h>
*/
#include "advmath.h"
#include "earth.h"
#include "orbit.h"

#include "types.h"
#include "gmt.h"
#include "tempest.h"
#include "global.h"
#include "temputil.h"
#include "genorbit.h"
#include "tether.h"
#include "bfield.h"
#include "emf.h"
#include "tss_current.h"
#include "bare_tether.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void show_simulation_help ()
{
   int count ;

printf ("\n") ;
printf ("Usage: tempest [-help] [-doc <module name>]\n") ;
printf ("               [-nopshow] [-noheader]\n") ;
printf ("               [-plotl] [-tdelim]\n") ;
printf ("               [-minmax]\n") ;
printf ("               [-f  <filename>]\n") ;
printf ("               [-var <variable> <value>]\n") ;
printf ("               [-show <variable list>]\n") ;
printf ("               [-pout <filename>] [-o <filename>]\n") ;
printf ("               [-mout <filename>] [-lout <filename>]\n") ;
printf ("\nNotes:\n") ;
printf ("The simulation is run with a set of default hardcoded parameters,\n") ;
printf ("unless a parameter file is specified with the -f parameter.\n") ;
printf ("The file is of the format <variable> = <value> where the =\n") ;
printf ("needs at least one leading and one trailing space and there is\n") ;
printf ("one variable per line.  After the values are all read in, the\n") ;
printf ("command line parameters are processed in order specified.\n") ;
printf ("The parameters of the simulation and column headers are initially\n") ;
printf ("printed out unless the -nopshow and -noheader are specified.\n") ;
printf ("Lastly, parameters may be altered using the -var option in the\n") ;
printf ("format specified in the command summary above.\n\n") ;

printf ("To specify what simulation results to display, either use the\n") ;
printf ("SHOW_VARS parameter in your parameter file, modify the SHOW_VAR\n") ;
printf ("variable using the -var option, or use the -show argumenent to\n") ;
printf ("specify what values to display.  The <variable list> must contain\n") ;
printf ("variable names separated by commas only - NO SPACES!\n\n") ;

printf ("The -plotl option is used to place Y axis label information to be\n") ;
printf ("used by plotl into the output file.  The -tdelim option will\n") ;
printf ("tab-delimit the output fields.  The -minmax option displays the\n") ;
printf ("minimum and maximum values for all of the simulation parameters\n") ;
printf ("computed after the simulation is finished.  These values are\n") ;
printf ("printed to the stderr device unless redirectued using -mout.\n\n") ;

printf ("The -pout parameter specifies a file for simulation parameter\n") ;
printf ("output.  The -o parameter specifies a file for simulation output.\n") ;
printf ("The -mout output and -lout parameters specify simulation minmax\n") ;
printf ("and plotl label output respectively.  The defaults for all of\n") ;
printf ("outputs is standard out (stdout).\n\n") ;

printf ("TEMPEST assumes that data files required my some modules are\n") ;
printf ("located in the directory specified by the environmental variable\n") ;
printf ("TEMPEST_DATA.  If this variable is not set, then the data files\n") ;
printf ("are expected in the current working directory.\n\n") ;

printf ("The -doc option will provide more documentation about the module\n") ;
printf ("requested.  The modules currently linked into TEMPEST include:\n\n") ;

for (count = 0 ; count < NUM_MODULES ; count++) 
   printf ("%-12s   %s\n",module_list[count].token,module_list[count].descr) ;

printf ("\n") ;
}



void show_simulation_docs (char *module_name)
{
   int count  = 0 ;
   int count2     ;

   while (count < NUM_MODULES && 
          (strcmp (strupper(module_name),module_list [count].token) != 0) )
      count++ ;

   if (count < NUM_MODULES)
   {
      printf ("Documentation for module:  %s\n\n", module_list [count].token) ;
      printf ("Description:\n") ;
      printf ("------------\n") ;
      printf ("%s\n", module_list [count].docs) ;

      if (module_list [count].module_params != NULL)
      {
         printf ("Input Parameter List:\n") ;
         printf ("---------------------\n") ;
         for (count2 = 0 ; count2 < module_list [count].num_params ; count2++)
             printf (" %-15s = %s\n",
                           (module_list [count].module_params[count2]).token,
                           (module_list [count].module_params[count2]).descr) ;
         printf ("\n") ;
      }

      if (module_list [count].module_outputs != NULL)
      {
         printf ("Display Parameter List:\n") ;
         printf ("-----------------------\n") ;
         for (count2 = 0 ; count2 < module_list [count].num_outputs ; count2++)
             printf (" %-15s = %s\n",
                           (module_list [count].module_outputs[count2]).token,
                           (module_list [count].module_outputs[count2]).descr) ;
         printf ("\n") ;
      }
   }
   else   
      show_simulation_help () ;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



void make_lists ()
{
   int count, count2 ;          /* Counter for loops                         */

   for (count = 0 ; count < NUM_MODULES ; count++)
   {
      if (module_list [count].module_params != NULL)
      {
         count2 = 0 ;
         while (count2 < module_list [count].num_params &&
                num_params < MAX_PARAMS)
            param_list  [num_params++ ] = 
               &(module_list [count].module_params  [count2++]) ;
      }

      if (num_params >= MAX_PARAMS)
         fprintf (stderr, "WARNING: %s\n",
            "Not all parameters may have been loaded - limit reached!\007") ;

      if (module_list [count].module_outputs != NULL)
      {
         count2 = 0 ;
         while (count2 < module_list [count].num_outputs)
         {
            outvar_mod  [num_outvars  ] = count ;
            outvar_list [num_outvars++] = 
               &(module_list [count].module_outputs [count2++]) ;
         }
      }

      if (num_outvars >= MAX_OUTPUTS)
         fprintf (stderr, "WARNING: %s\n",
            "Not all outputs may have been loaded - limit reached!\007") ;

   }

   for (count = 0 ; count < MAX_OUTPUTS ; count++)
      display_list [count] = 0 ; 
}



void get_disp_var_list (char *var_list)
{
   char *var_name       ;
   int   count          ;
   int   var_OK         ;
   int   disp_pos =  0  ;
   int   tok_len        ;

                                /* Find first available position in list     */
   while (display_list [disp_pos] != 0)
      disp_pos++ ;
                                /* Append display variables to list          */
   while ((var_name = strtok (var_list, ", \t")) != NULL)
   {
#if DEBUG
if (show_debug & DEBUG_TEMPUTIL)
   fprintf (stderr, "var_list={%s} var_name={%s}\n",var_list, var_name) ;
#endif
      var_list = NULL ;

      var_OK = FALSE ;
      count  = 0     ;
      tok_len = strlen (var_name) ;
      while (count < num_outvars)
      {
         if (strncmp (var_name, outvar_list[count]->token,
                        MAX (tok_len,outvar_list[count]->min_tok_len)) == 0)
         {
            var_OK = TRUE ;
            display_list [disp_pos++] = count + 1 ;
            module_list  [outvar_mod [count]].compute_module = TRUE ;
         }
         count++ ;
      }
      if (var_OK == FALSE)
         fprintf(stderr,"WARNING: Invalid output variable name \"%s\"\007\n\n",
                          var_name) ;     
   }

}



void set_parameter (char *token, char *value)
{
   int   count     = 0     ;
   int   var_OK    = FALSE ;
   void *param_ptr         ;
   char *met_str           ;
   int   yr        = 1900  ;

   if (strlen (token) == 0 || strlen (value) == 0)
      return ;

   while (count < num_params)
   {
      if (strcmp (strupper(token), param_list[count]->token) == 0)
      {
         var_OK = TRUE ;
         param_ptr = param_list[count]->param_ptr ;
         switch (param_list[count]->param_type)
         {
            case P_REAL : *((double *) param_ptr) = atof (value) ;
                          break ;
            case P_INT  : *((int *) param_ptr) = atoi (value) ;
                          break ;
            case P_DEG  : *((double *) param_ptr) = 
                             DEG_TO_RAD (atof (value)) ;
                          break ;
            case P_ALT  : *((double *) param_ptr) = 
                             MEAN_R + atof (value) ;
                          break ; 
            case P_GMT  : met_str = index (value, ',') ;
                          GMT_FromStr (value, (GMT *) param_ptr) ;
                          if (met_str != NULL)
                             if (strcmp (strupper(met_str), ",MET") == 0)
                             {
                                if (met_gmt.d == -1)
                                   fprintf (stderr, "WARNING: %s%s\007\n",
                                      "Attempting to convert MET to GMT when",
                                      "MET0_TIME is not set") ;
                                else
                                   GMT_Incr ((GMT *) param_ptr, &yr, &met_gmt) ;
                             }
                             else
                             {
                                fprintf (stderr, "WARNING: %s%s\007\n",
                                   "Unknown qualifier for GMT format time",
                                   "entry - ignoring") ;
                             }
                          break ;
            case P_STR  : strcpy ((char *) param_ptr, value) ;
                          break ;
            case P_BOOL : *((int *) param_ptr) = 
                          strcmp (strupper(value), "YES") ? FALSE : TRUE ;
                          break ;
            case P_VARS : get_disp_var_list (strupper(value)) ;
                          break ;
            case P_FILE : load_ttag_params (value) ;
                          break ;
         }
      }
      count++ ;
   }
   if (var_OK == FALSE)
      fprintf (stderr, "WARNING: Invalid variable name \"%s\"\007\n\n",
                        token) ;
}



int read_in_line (FILE *file, char *linein)
{
   int linelen = 0 ;

   while (((linein [linelen] = (char) fgetc (file)) != '\n') &&
           (linein [linelen] != EOF))
      linelen++ ;

   linein [linelen] = '\0' ;

                                   /* Remove lines full of spaces            */
   for (;;)
   {
      if (linelen > 0)
         if (linein[linelen-1] == ' ' || linein[linelen-1] == '\t')
            linein [--linelen] = '\0' ;
         else
            break ;
      else
         break ;
   }

   return (linelen) ;
}



void read_in_params (char filename[])
{
   FILE *param_file   ;
   int   linelen      ;
   char  linein [900] ;
   char  token  [ 20] ;
   char  value  [800] ;

   if ((param_file = fopen (filename, "r")) != NULL)
   {
      fprintf (stderr, "Reading in parameters from \"%s\"\n", filename) ;

      do
      {
         token [0] = value [0] = '\0' ;

         linelen = read_in_line (param_file, linein) ;

         if (linein [0] != '#')
         {
            sscanf (linein, "%s = %s\n", token, value) ;
            set_parameter (token, value) ;
#if DEBUG
if (show_debug & DEBUG_TEMPUTIL)
   fprintf (stderr, "token={%s} value={%s} EOF=%d\n", token, value,
                    feof (param_file) ) ;
#endif
         }
      }
      while ( feof (param_file) == 0 ) ;

      fclose (param_file) ;
   }
   else
      fprintf (stderr,
         "WARNING: File %s not found - parameters not read in!\007\n\n",
         filename) ;
}



void load_ttag_params (char filename[])
{
   FILE *param_file       ;
   int   linelen          ;
   char  linein [900]     ;
   char *token            ;
   char *value            ;
   char *incl_fname       ;
   char *incl_time0       ;
   void *param_ptr        ;
   int   count            ;
   int   count2           ;
   int   non_year  = 1900 ;
   int   time_type        ;
   int   new_index        ;
   int   num_load_params                = 0 ;
   int   load_param_index [MAX_TTAG_PARAMS] ;
   char *met_str          ;
   int   yr        = 1900 ;

   for (count = 0 ; count < MAX_TTAG_PARAMS ; count++)
      load_param_index [count] = -1 ;

   if (num_ttag_entries == 0)
   {
      for (count = 0 ; count < MAX_TTAG_PARAMS ; count++)
     {
        ttag_param_index [count] = -1 ;
        for (count2 = 0 ; count2 < MAX_TTAG_ENTRIES ; count2++)
           ttag_param_list [count2] [count] = NULL ;
     }
   }

   if ((param_file = fopen (filename, "r")) == NULL)
   {
      sprintf (linein, "%s/%s", ttp_path, filename) ;
      param_file = fopen (linein, "r") ;
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "file {%s} opened  = 0x%08X\n", linein, param_file) ;
#endif
   }

   if (param_file != NULL)
   {
      fprintf (stderr, "Reading in time tagged parameters from \"%s\"\n",
                       filename) ;

                              /* Read in first non-blank non commented line  */
      do
      {
         linelen = read_in_line (param_file, linein) ;

         if (linein [0] == '#')
            linelen = 0 ;
      }
      while ( feof (param_file) == 0  && linelen == 0) ;

#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "ttag header = {%s}\n", linein) ;
#endif
                                   /* Check first entry on line to make sure */
                                   /*   that it is either GMT or MET (or FET)*/
      if ((token = strtok (linein, " \t")) != NULL)
      {
         if ((    strcmp (strupper(token), "MET") == 0) ||
             (    strcmp (strupper(token), "FET") == 0))
         {
            time_type = TTG_BY_MET ;
            load_param_index [num_load_params++] = TTG_BY_MET ;
            if (num_ttag_params == 0)
               ttag_param_index [num_ttag_params++] = TTG_BY_MET ;
         }
         else if (strcmp (strupper(token), "GMT") == 0)
         {
            time_type = TTG_BY_GMT ;
            load_param_index [num_load_params++] = TTG_BY_GMT ;
            if (num_ttag_params == 0)
               ttag_param_index [num_ttag_params++] = TTG_BY_GMT ;
         }
         else
         {
            fprintf (stderr, "ERROR:  %s\007\n\n",
            "Time tagged parameter file must have GMT or MET in first column") ;
            exit (-1) ;
         }
      }

                                   /* Process rest of column headers         */
      do
      {
         token = strtok (NULL  , " \t") ;

#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "num_ttag_params=%d token={%s}\n",num_ttag_params,token) ;
#endif

         if (token != NULL)
         {
            count     = 0 ;
            new_index = -1 ;
            while (count < num_params && new_index == -1)
            {
               if (strcmp (strupper(token), param_list[count]->token) == 0)
                  new_index = count ;
               else
                  count++ ;
            }

            if (new_index == -1)
            {
               fprintf (stderr, "ERROR:  Invalid parameter name %s %s\007\n\n",
                        strupper (token), "in time tag parameter file") ;
               exit (-1) ; 
            }
            else
            {
               count = 0 ;
               while (count < num_ttag_params &&
                      load_param_index [num_load_params] == -1) 
               {
                  if (ttag_param_index [count] == new_index)
                     load_param_index [num_load_params] = count ;
                  count++ ;
               }

               if (load_param_index [num_load_params] == -1)
               {
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "token={%s} not in list\n", token) ;
#endif
                  if (num_ttag_params < MAX_TTAG_PARAMS)
                  {
                     load_param_index [num_load_params++] = num_ttag_params ; 
                     ttag_param_index [num_ttag_params++] = new_index ;
                  }
                  else
                     fprintf (stderr,
                       "WARNING: Time tagged entries limit reached (%d)\007!\n",
                                 MAX_TTAG_ENTRIES) ;
               }
               else
                  num_load_params++ ;
            }
         }
      }
      while (token != NULL) ;
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "number of columns = %d\n", num_load_params) ;
#endif
                                    /* Read in lines of values now           */
      do
      {
         linelen = read_in_line (param_file, linein) ;

#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "ttag entry = {%s}\n", linein) ;
#endif
         if (linein [0] != '#' && linelen > 0)
         {
            if ((value = strtok (linein, " \t")) != NULL)
            {
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "GMT/MET value [%d][%d] = {%s}\n", 
                     num_ttag_entries, count, value) ;
#endif
               if ((param_ptr = malloc (sizeof(GMT))) == NULL)
               {
                  fprintf (stderr, "Could not allocate memory\n") ;
                  exit (-2) ;
               }

               GMT_FromStr (value, (GMT *) param_ptr) ;

               if (time_type != ttag_param_index [0])
               {
                  if (ttag_param_index [0] == TTG_BY_GMT)
                     param_ptr = GMT_Incr ((GMT *) param_ptr,
                                             &non_year, &met_gmt) ;
                  else
                     param_ptr = GMT_Decr ((GMT *) param_ptr,
                                             &non_year, &met_gmt) ;
               }

               ttag_param_list [num_ttag_entries][0] = param_ptr ;
            }

            count = 1 ;
            do
            {
               value = strtok (NULL  , " \t") ;
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "param value [%d] [%d] = {%s}\n", num_ttag_entries,
                     load_param_index [count], value) ;
#endif
               if (value != NULL)
               {
                  if (strcmp (value, ".") == 0)
                  {
                     param_ptr = NULL ;
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "param value [%d] [%d] = NULL\n", num_ttag_entries,
                     load_param_index [count]) ;
#endif
                  }
                  else
                  {
                     switch (param_list[ttag_param_index [
                                        load_param_index[count]]]->param_type)
                     {
                        case P_REAL : if ((param_ptr = malloc (sizeof(double))) 
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      *((double *) param_ptr) = atof (value) ;
                                      break ;
                        case P_INT  : if ((param_ptr = malloc (sizeof (int)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");

                                         exit (-2) ;
                                      }
                                      *((int *) param_ptr) = atoi (value) ;
                                      break ;
                        case P_DEG  : if ((param_ptr = malloc (sizeof (double)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      *((double *) param_ptr) = 
                                      DEG_TO_RAD (atof (value)) ;
                                      break ;
                        case P_ALT  : if ((param_ptr = malloc (sizeof (double)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      *((double *) param_ptr) = 
                                      MEAN_R + atof (value) ;
                                      break ;
                        case P_GMT  : if ((param_ptr = malloc (sizeof (GMT)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      met_str = index (value, ',') ;
                                      GMT_FromStr (value, (GMT *) param_ptr) ;
                                      if (met_str != NULL)
                                      {
                                         if (strcmp (strupper(met_str), ",MET") == 0)
                                         {
                                            if (met_gmt.d == -1)
                                               fprintf (stderr, "WARNING: %s%s\007\n",
                                                  "Attempting to convert MET to GMT when",
                                                  "MET0_TIME is not set") ;
                                            else
                                               GMT_Incr ((GMT *) param_ptr, &yr, &met_gmt) ;
                                         }
                                         else
                                         {
                                            fprintf (stderr, "WARNING: %s%s\007\n",
                                               "Unknown qualifier for GMT format time",
                                               "entry - ignoring") ;
                                         }
                                      }
                                      break ;
                        case P_STR  : if ((param_ptr = malloc (strlen(value)+1))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      strcpy ((char *) param_ptr, value) ;
                                      break ;
                        case P_BOOL : if ((param_ptr = malloc (sizeof (int)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      *((int *) param_ptr) = 
                                      strcmp(strupper(value),"YES")?FALSE:TRUE;
                                      break ;
                     }
                  }
                  ttag_param_list [num_ttag_entries]
                                  [load_param_index[count++]] = param_ptr ;
               }
            }
            while ((value != NULL) && (count < num_load_params)) ;

            if (++num_ttag_entries == MAX_TTAG_ENTRIES)
               fprintf (stderr,
                       "WARNING: Time tagged entries limit reached (%d)\007!\n",
                                 MAX_TTAG_ENTRIES) ;
         }
         else if (linelen > 0)
         {
            if ((value = strtok (linein, " \t")) != NULL)
            {
               if (strcmp (strupper(value), "#INCLUDE") == 0)
               {
                  incl_time0 = strtok (NULL, " \t") ; /* Base GMT/MET */
                  incl_fname = strtok (NULL, " \t") ; /* Filename     */

                  if ((incl_time0 != NULL) && (incl_fname != NULL))
                  {
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "#INCLUDing file={%s} @ %s GMT/MET\n",
                     incl_fname, incl_time0) ;
#endif
                     load_include_ttag (incl_fname, incl_time0, time_type) ;
                  }
               }
           }
         }
      }
      while ((feof(param_file) == 0) && (num_ttag_entries != MAX_TTAG_ENTRIES));

      fclose (param_file) ;

#if DEBUG
if (show_debug & DEBUG_TTAG)
{
   fprintf (stderr, "%d time-tagged parameter entries loaded with %d parameters\n",
                     num_ttag_entries, num_ttag_params) ;
   for (count = 0 ; count < num_ttag_entries ; count++)
   {
      fprintf (stderr, "Entry %2d @ %s => ", count, 
                        Str_FromGMT (((GMT *) ttag_param_list [count][0]))) ;
      for (count2 = 1 ; count2 < num_ttag_params ; count2++)
         if (param_list[ttag_param_index [count2]]->param_type == P_STR)
            fprintf (stderr, "{%s}=0x%08X ", 
                    (char *) ttag_param_list [count][count2],
                             ttag_param_list [count][count2]);
         else if (param_list[ttag_param_index [count2]]->param_type == P_GMT)
            fprintf (stderr, "%s ",
                     Str_FromGMT ((GMT *) ttag_param_list [count][count2])) ;
         else
            fprintf (stderr, "0x%08X ", ttag_param_list [count][count2]) ;
      
      fprintf (stderr, "\n") ;
   }
}
#endif
   }
   else
      fprintf (stderr,
     "WARNING: File %s not found - time tagged parameters not read in!\007\n\n",
         filename) ;
}



void load_include_ttag (char filename[], char incl_time0[], int time_type) 
{
   FILE *include_file                       ;
   GMT   time0_gmt                          ;
   int   non_year                    = 1900 ;
   int   linelen                            ;
   char  linein [900]                       ;
   char *token                              ;
   char *value                              ;
   void *param_ptr                          ;
   int   count                              ;
   int   num_incl_params             = 1    ;
   int   incl_param_index [MAX_TTAG_PARAMS] ;
   int   new_index                          ;
   char *met_str                            ;
   int   yr                          = 1900 ;

   for (count = 0 ; count < MAX_TTAG_PARAMS ; count++)
      incl_param_index [count] = -1 ;

   if ((include_file = fopen (filename, "r")) == NULL)
   {
      sprintf (linein, "%s/%s", ttp_path, filename) ;
      include_file = fopen (linein, "r") ;
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "file {%s} opened  = 0x%08X\n", linein, include_file) ;
#endif
   }

   if (include_file != NULL)
   {
      GMT_FromStr (incl_time0, &time0_gmt) ;

      if (time_type != ttag_param_index [0])
      {
         if (ttag_param_index [0] == TTG_BY_GMT)
            GMT_Incr (&time0_gmt, &non_year, &met_gmt) ;
         else
            GMT_Decr (&time0_gmt, &non_year, &met_gmt) ;
      }

      fprintf (stderr,
               "Including time tagged parameters from \"%s\" at %s\n",
                       filename, Str_FromGMT (&time0_gmt)) ;

                              /* Read in first non-blank non commented line  */
      do
      {
         linelen = read_in_line (include_file, linein) ;

         if (linein [0] == '#')
            linelen = 0 ;
      }
      while ( feof (include_file) == 0  && linelen == 0) ;

#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "include header = {%s}\n", linein) ;
#endif
                                   /* Check first entry on line to make sure */
                                   /*   that it is MET (or FET)              */
      if ((token = strtok (linein, " \t")) != NULL)
      {
         if ((    strcmp (strupper(token), "MET") != 0) &&
             (    strcmp (strupper(token), "FET") != 0))
         {
            fprintf (stderr, "ERROR:  %s\007\n\n",
                     "Include parameter file must have FET in first column") ;
            exit (-1) ;
         }
      }

                                   /* Process rest of column headers         */
      do
      {
         token = strtok (NULL  , " \t") ;

#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "token={%s}\n",token) ;
#endif

         if (token != NULL)
         {
            count     =  0 ;
            new_index = -1 ;
            while (count < num_params && new_index == -1)
            {
               if (strcmp (strupper(token), param_list[count]->token) == 0)
                  new_index = count ;
               else
                  count++ ;
            }

#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "token={%s} parm_list_index=%d\n",token,new_index) ;
#endif
            if (new_index == -1)
            {
               fprintf (stderr, "ERROR:  Invalid parameter name %s %s\007\n\n",
                        strupper (token), "in include parameter file") ;
               exit (-1) ;
            }
            else
            {
               count = 0 ;
               while (count < num_ttag_params &&
                      incl_param_index [num_incl_params] == -1) 
               {
                  if (ttag_param_index [count] == new_index)
                     incl_param_index [num_incl_params] = count ;
                  count++ ;
               }

               if (incl_param_index [num_incl_params] == -1)
               {
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "token={%s} not in list\n", token) ;
#endif
                  if (num_ttag_params < MAX_TTAG_PARAMS)
                  {
                     incl_param_index [num_incl_params++] = num_ttag_params ; 
                     ttag_param_index [num_ttag_params++] = new_index ;
                  }
                  else
                     fprintf (stderr,
                       "WARNING: Time tagged entries limit reached (%d)\007!\n",
                                 MAX_TTAG_ENTRIES) ;
               }
               else
                  num_incl_params++ ;
            }
         }
      }
      while (token != NULL && num_ttag_params < MAX_TTAG_PARAMS) ;

                                    /* Read in lines of values now           */
      do
      {
         linelen = read_in_line (include_file, linein) ;

#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "include entry = {%s}\n", linein) ;
#endif
         if (linein [0] != '#' && linelen > 0)
         {
            if ((value = strtok (linein, " \t")) != NULL)
            {
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "FET value = {%s}\n", value) ;
#endif
               if ((param_ptr = malloc (sizeof(GMT))) == NULL)
               {
                  fprintf (stderr, "Could not allocate memory\n") ;
                  exit (-2) ;
               }

               GMT_FromStr (value, (GMT *) param_ptr) ;
               param_ptr = GMT_Incr ((GMT *) param_ptr, &non_year,
                                             &time0_gmt) ;

#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "GMT/MET value = {%s}\n", Str_FromGMT ((GMT *) param_ptr)) ;
#endif
               ttag_param_list [num_ttag_entries][0] = param_ptr ;
            }

            count = 1 ;
            do
            {
               value = strtok (NULL  , " \t") ;
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "param value [%d] [%d] = {%s}\n", num_ttag_entries,
                     incl_param_index [count], value) ;
#endif
               if (value != NULL)
               {
                  if (strcmp (value, ".") == 0)
                  {
                     param_ptr = NULL ;
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (stderr, "param value [%d] [%d] = NULL\n", num_ttag_entries,
                   incl_param_index [count]) ;
#endif
                  }
                  else
                  {
                     switch (param_list [ttag_param_index [
                                         incl_param_index [count]]]->param_type)
                     {
                        case P_REAL : if ((param_ptr = malloc (sizeof(double)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      *((double *) param_ptr) = atof (value) ;
                                      break ;
                        case P_INT  : if ((param_ptr = malloc (sizeof (int)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      *((int *) param_ptr) = atoi (value) ;
                                      break ;
                        case P_DEG  : if ((param_ptr = malloc (sizeof (double)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      *((double *) param_ptr) =
                                      DEG_TO_RAD (atof (value)) ;
                                      break ;
                        case P_ALT  : if ((param_ptr = malloc (sizeof (double)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      *((double *) param_ptr) =
                                      MEAN_R + atof (value) ;
                                      break ;
                        case P_GMT  : if ((param_ptr = malloc (sizeof (GMT)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      met_str = index (value, ',') ;
                                      GMT_FromStr (value, (GMT *) param_ptr) ;
                                      if (met_str != NULL)
                                      {
                                         if (strcmp (strupper(met_str), ",MET") == 0)
                                         {
                                            if (met_gmt.d == -1)
                                               fprintf (stderr, "WARNING: %s%s\007\n",
                                                  "Attempting to convert MET to GMT when",
                                                  "MET0_TIME is not set") ;
                                            else
                                               GMT_Incr ((GMT *) param_ptr, &yr, &met_gmt) ;
                                         }
                                         else
                                         {
                                            fprintf (stderr, "WARNING: %s%s\007\n",
                                               "Unknown qualifier for GMT format time",
                                               "entry - ignoring") ;
                                         }
                                      }
                                      break ;
                        case P_STR  : if ((param_ptr = malloc (strlen(value)+1))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      strcpy ((char *) param_ptr, value) ;
                                      break ;
                        case P_BOOL : if ((param_ptr = malloc (sizeof (int)))
                                          == NULL)
                                      {
                                         fprintf (stderr,
                                            "Could not allocate memory\007!\n");
                                         exit (-2) ;
                                      }
                                      *((int *) param_ptr) =
                                      strcmp(strupper(value),"YES")?FALSE:TRUE;
                                      break ;
                     }
                  }
                  ttag_param_list [num_ttag_entries]
                                  [incl_param_index[count++]] = param_ptr ;
               }
            }
            while ((value != NULL) && (count < num_incl_params)) ;

            if (++num_ttag_entries == MAX_TTAG_ENTRIES)
               fprintf (stderr, "WARNING: Time tagged entries limit reached (%d)\007!\n",
                                 MAX_TTAG_ENTRIES) ;
         }
      }
      while ((feof(include_file) == 0) && (num_ttag_entries != MAX_TTAG_ENTRIES));

      fclose (include_file) ;
   }
   else
      fprintf (stderr,
     "WARNING: File %s not found - time tagged parameters not included!\007\n\n",
         filename) ;
}


#define COPY(to,from)   {for(c=0;c<num_ttag_params;to[c++]=from[c]);}
#define TIME(e)         (Day_FromGMT(time_year,(GMT *)e[0])) 
#define SWAP(a,b)       {for(c=0;c<num_ttag_params;temp[c++]=a[c]); \
                         for(c=0;c<num_ttag_params;a[c++]=b[c]);    \
                         for(c=0;c<num_ttag_params;b[c++]=temp[c]);}
#define M        7
#define NSTACK	50

void sort_ttag_param_entries ()
{
   int c, count, count2 ;
   double time_year ;
   unsigned long i,ir,n,j,k,l;
   int jstack=0, *istack;
   void *a[MAX_TTAG_PARAMS];
   void *temp[MAX_TTAG_PARAMS];

   if (num_ttag_entries > 0)
      fprintf (stderr, "Sorting time-tagged entries\n") ;
   else 
      return ;

   if (ttag_param_index [0] == TTG_BY_MET)
      time_year = met_year ;
   else
      time_year = curr_year ;

   ir = n = num_ttag_entries ;
   l  = 1 ;
   istack=malloc (sizeof (int) * NSTACK) ;
   for (;;)
   {
      if (ir-l < M)
      {
         for (j=l+1;j<=ir;j++)
         {
            COPY (a,ttag_param_list[j-1]) ;
            for (i=j-1;i>=1;i--)
            {
               if (TIME(ttag_param_list[i-1]) <= TIME(a)) break;
                  COPY (ttag_param_list[i+1-1],ttag_param_list[i-1]) ;
            }
            COPY(ttag_param_list[i+1-1],a) ;
         }
         if (jstack == 0) break;
         ir=istack[jstack--];
         l=istack[jstack--];
      }
      else
      {
         k=(l+ir) >> 1;
         SWAP(ttag_param_list[k-1],ttag_param_list[l+1-1])
         if (TIME(ttag_param_list[l+1-1]) > TIME(ttag_param_list[ir-1])) {
            SWAP(ttag_param_list[l+1-1],ttag_param_list[ir-1])
         }
         if (TIME(ttag_param_list[l-1]) > TIME(ttag_param_list[ir-1])) {
            SWAP(ttag_param_list[l-1],ttag_param_list[ir-1])
         }
         if (TIME(ttag_param_list[l+1-1]) > TIME(ttag_param_list[l-1])) {
            SWAP(ttag_param_list[l+1-1],ttag_param_list[l-1])
         }
         i=l+1;
         j=ir;
         COPY (a,ttag_param_list[l-1]) ;
         for (;;) {
            do i++; while (TIME(ttag_param_list[i-1]) < TIME(a));
            do j--; while (TIME(ttag_param_list[j-1]) > TIME(a));
            if (j < i) break;
            SWAP(ttag_param_list[i-1],ttag_param_list[j-1])
         }
         COPY(ttag_param_list[l-1],ttag_param_list[j-1]) ;
         COPY(ttag_param_list[j-1],a) ;
         jstack += 2;
         if (jstack > NSTACK)
            fprintf (stderr, "NSTACK too small in sort!!!!\007\n");
         if (ir-i+1 >= j-l) {
            istack[jstack]=ir;
            istack[jstack-1]=i;
            ir=j-1;
         } else {
            istack[jstack]=j-1;
            istack[jstack-1]=l;
            l=i;
         }
      }
   }
   free (istack) ;
#if DEBUG
if (show_debug & DEBUG_TTAG)
{
   fprintf (stderr, "%d ttp entries loaded with %d parameters\n",
                     num_ttag_entries, num_ttag_params) ;
   for (count = 0 ; count < num_ttag_entries ; count++)
   {
      fprintf (stderr, "Entry %2d @ %s => ", count,
                        Str_FromGMT (((GMT *) ttag_param_list [count][0]))) ;
      for (count2 = 1 ; count2 < num_ttag_params ; count2++)
         if (param_list[ttag_param_index [count2]]->param_type == P_STR)
            fprintf (stderr, "{%s}=0x%08X ",
                    (char *) ttag_param_list [count][count2],
                             ttag_param_list [count][count2]);
         else if (param_list[ttag_param_index [count2]]->param_type == P_GMT)
            fprintf (stderr, "%s ",
                     Str_FromGMT ((GMT *) ttag_param_list [count][count2])) ;
         else
            fprintf (stderr, "0x%08X ", ttag_param_list [count][count2]) ;

      fprintf (stderr, "\n") ;
   }
}
#endif
}

#undef NSTACK
#undef M
#undef COPY
#undef SWAP
#undef TIME




void  set_param_ttag_entry   ()
{
   int   count     ;
   void *param_ptr ;

#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (debug_out, "Setting new ttag params with entry %d of %d @ %s\n",
              curr_ttag_entry, num_ttag_entries,
              Str_FromGMT ( (GMT *)ttag_param_list[curr_ttag_entry][0] ) );
#endif
   for (count = 1 ; count < num_ttag_params ; count++)
   {
      param_ptr = param_list[ttag_param_index[count]]->param_ptr;
#if DEBUG
if (show_debug & DEBUG_TTAG)
   fprintf (debug_out, "cnt = %d parm_ndx = %d parm_type = %d parm_ptr=0x%X\n",
            count, ttag_param_index[count],
            param_list[ttag_param_index[count]]->param_type,
            ttag_param_list[curr_ttag_entry][count]) ;
#endif

      if (ttag_param_list[curr_ttag_entry][count] != NULL)
      {
         switch (param_list[ttag_param_index[count]]->param_type)
         {
            case P_REAL : *((double *) param_ptr) =
                          *((double *) ttag_param_list[curr_ttag_entry][count]);
                          break ;
            case P_INT  : *((int *) param_ptr) =
                          *((int *) ttag_param_list [curr_ttag_entry][count]) ;
                          break ;
            case P_DEG  : *((double *) param_ptr) =
                          *((double *) ttag_param_list[curr_ttag_entry][count]);
                          break ;
            case P_ALT  : *((double *) param_ptr) =
                          *((double *) ttag_param_list[curr_ttag_entry][count]);
                          break ;
            case P_GMT  : bcopy ((GMT *)ttag_param_list[curr_ttag_entry][count],
                            (GMT *) param_ptr,
                          sizeof (GMT)) ;
                          break ;
            case P_STR  : strcpy ((char *) param_ptr,
                            (char *) ttag_param_list [curr_ttag_entry][count]) ;
                          break ;
            case P_BOOL : *((int *) param_ptr) =
                          *((int *) ttag_param_list [curr_ttag_entry][count]) ;
                          break ;
         }
      }
   }
                                   /* Initialize modules                     */
   for (count = 0 ; count < NUM_MODULES ; count++)
      if (module_list[count].init_fun != NULL)
         (*module_list[count].init_fun) () ;

                                   /* Compute time from GMT/MET of next entry */
   if (++curr_ttag_entry < num_ttag_entries)
   {
      if (ttag_param_index [0] == TTG_BY_MET)
         curr_ttag_entry_time = Day_FromGMT (met_year ,
                  (GMT *)ttag_param_list [curr_ttag_entry][0]) ;
      else
         curr_ttag_entry_time = Day_FromGMT (curr_year,
                  (GMT *)ttag_param_list [curr_ttag_entry][0]) ;
   }
}



void process_cmdline_params (int argc, char *argv []) 
{
   int count ;

   if (argc > 1)
   {
      for (count = 1 ; count < argc; count++)
      {
         if      (strcmp (strupper (argv[count]), "-HELP"    ) == 0)
         {
            show_simulation_help () ;
            exit (0) ;
         }
         else if (strcmp (strupper (argv[count]), "-DOC"     ) == 0)
         {
            if (count+1 < argc)
            {
               show_simulation_docs (argv[++count]) ;
               exit (0) ;
            }
            else
            {
                fprintf (stderr,"-DOC parameter requires module name\007\n\n") ;
                exit (-1) ;
            }

         }
         else if (strcmp (strupper (argv[count]), "-F"       ) == 0)
         {
            if (count+1 < argc)
               read_in_params (argv[++count]) ;
            else
            {
                fprintf (stderr, "-F parameter requires filename\007\n\n") ;
                exit (-1) ;
            }
         }
         else if (strcmp (strupper (argv[count]), "-NOPSHOW" ) == 0)
         {
            show_params = FALSE ;
         }
         else if (strcmp (strupper (argv[count]), "-NOHEADER") == 0)
         {
            show_header = FALSE ;
         }
#if DEBUG
         else if (strcmp (strupper (argv[count]), "-DEBUG") == 0)
         {
            if (count+1 < argc)
               show_debug = atoi (argv[++count]) ;
            else
            {
                fprintf (stderr, "-DEBUG parameter requires value\007\n\n") ;
                exit (-1) ;
            }
         }
#endif
         else if (strcmp (strupper (argv[count]), "-VAR"     ) == 0)
         {
            if (count+2 < argc)
            {
               set_parameter (argv[count+1], argv[count+2]) ;
               count += 2 ;
            }
            else
            {
               fprintf (stderr,
                  "-VAR parameter requires variable name and value\007\n\n") ;
               exit (-1) ;
            }
         }
         else if (strcmp (strupper (argv[count]), "-SHOW"    ) == 0)
         {
            if (count+1 < argc)
               get_disp_var_list (strupper(argv[++count])) ;
            else
            {
                fprintf (stderr,
                   "SHOW parameter requires variable list\007\n\n") ;
                exit (-1) ;
            }
         }
         else if (strcmp (strupper (argv[count]), "-PLOTL") == 0)
         {
            plotl_labels = TRUE ;
         }
         else if (strcmp (strupper (argv[count]), "-MINMAX") == 0)
         {
            show_min_max = TRUE ;
         }
         else if (strcmp (strupper (argv[count]), "-TDELIM") == 0)
         {
            tab_delim = TRUE ;
         }
         else if (strcmp (strupper (argv[count]), "-REALTIME") == 0)
         {
            delta_out_time = (time_t) 1 ;
         }
         else if (strcmp (strupper (argv[count]), "-REALSYNC") == 0)
         {
            delta_out_time = (time_t) 1 ;
            next_out_time  = (time_t) 1 ;
         }
         else if (strcmp (strupper (argv[count]), "-COMMENTS") == 0)
         {
            show_comments = TRUE ;
         }
         else if (strcmp (strupper (argv[count]), "-BINARIFY") == 0)
         {
            binarify      = TRUE ;
            show_header   = TRUE ; 
            show_comments = TRUE ;
         }
         else if (strcmp (strupper (argv[count]), "-GSETIME") == 0)
         {
            gse_time    = TRUE ;
         }
         else if (strcmp (strupper (argv[count]), "-O"   ) == 0)
         {
            if (count+1 < argc)
               strcpy (simul_fname, argv[++count]) ;
            else
            {
                fprintf (stderr, "-O parameter requires filename\007\n\n") ;
                exit (-1) ;
            }
         }
#if DEBUG
         else if (strcmp (strupper (argv[count]), "-DOUT") == 0)
         {
            if (count+1 < argc)
               strcpy (debug_fname, argv[++count]) ;
            else
            {
                fprintf (stderr, "-DOUT parameter requires filename\007\n\n") ;
                exit (-1) ;
            }
         }
#endif
         else if (strcmp (strupper (argv[count]), "-POUT") == 0)
         {
            if (count+1 < argc)
               strcpy (sparm_fname, argv[++count]) ;
            else
            {
                fprintf (stderr, "-POUT parameter requires filename\007\n\n") ;
                exit (-1) ;
            }
            show_params = TRUE ;
         }
         else if (strcmp (strupper (argv[count]), "-MOUT") == 0)
         {
            if (count+1 < argc)
               strcpy (extre_fname, argv[++count]) ;
            else
            {
                fprintf (stderr, "-MOUT parameter requires filename\007\n\n") ;
                exit (-1) ;
            }
            show_min_max = TRUE ;
         }
         else if (strcmp (strupper (argv[count]), "-LOUT") == 0)
         {
            if (count+1 < argc)
               strcpy (plotl_fname, argv[++count]) ;
            else
            {
                fprintf (stderr, "-LOUT parameter requires filename\007\n\n") ;
                exit (-1) ;
            }
            plotl_labels = TRUE ;
         }
         else
         {
            fprintf (stderr, "\nInvalid paramter %s\007\n\n", argv[count]) ;
            exit (-1) ;
         }
      }
   }
}



void init_file_output ()
{
   if (strlen(debug_fname) > 0)
   {
      if ( (debug_out = fopen (debug_fname, "w")) == NULL)
      {
         fprintf (stderr, "File open for -DOUT %s failed\007\n\n", debug_fname);
         exit (-1) ;
      }
   }
   else
      debug_out = DEF_DEBUG_OUT ;

   if (strlen(sparm_fname) > 0)
   {
      if ( (sparm_out = fopen (sparm_fname, "w")) == NULL)
      {
         fprintf (stderr, "File open for -POUT %s failed\007\n\n", sparm_fname);
         exit (-1) ;
      }
   }
   else
      sparm_out = DEF_SPARM_OUT ;
 
   if (strlen(simul_fname) > 0)
   {
      if ( (simul_out = fopen (simul_fname, "w")) == NULL)
      {
         fprintf (stderr, "File open for -O %s failed\007\n\n", simul_fname);
         exit (-1) ;
      }
   }
   else
      simul_out = DEF_SIMUL_OUT ;

   if (strlen(plotl_fname) > 0)
   {
      if ( (plotl_out = fopen (plotl_fname, "w")) == NULL)
      {
         fprintf (stderr, "File open for -LOUT %s failed\007\n\n", plotl_fname);
         exit (-1) ;
      }
   }
   else
      plotl_out = DEF_PLOTL_OUT ;
 
   if (strlen(extre_fname) > 0)
   {
      if ( (extre_out = fopen (extre_fname, "w")) == NULL)
      {
         fprintf (stderr, "File open for -MOUT %s failed\007\n\n", extre_fname);
         exit (-1) ;
      }
   }
   else
      extre_out = DEF_EXTRE_OUT ;
}



void close_file_output ()
{
   fclose (debug_out) ;

   fclose (sparm_out) ;

   fclose (simul_out) ;

   fclose (plotl_out) ;

   fclose (extre_out) ;
}


void adjust_min_max () 
{
   int     count   ;
   double  val_out ;

   if (first_minmax)
   {
      count = 0 ;
      while (display_list [count] != 0)
      {
         if (outvar_list[display_list[count]-1]->param_type == P_REAL)
         {
            val_out= *((double*)outvar_list[display_list[count]-1]->param_ptr)*
                               outvar_list[display_list[count]-1]->mult_factor ;

            if (isnan (val_out))
            {
               fprintf (stderr, "WARNING: %s !\007\n",
                  "First output value is NaN - min/max may not be correct") ;
               min_val [count] = 0.0 ;
               max_val [count] = 0.0 ;
            }
            else
            {
               min_val [count] = val_out ;
               max_val [count] = val_out ;
            }
         }
         count++ ;
      }
      first_minmax = FALSE ;
   }
   else
   {
      count = 0 ;
      while (display_list [count] != 0)
      {
         if (outvar_list[display_list[count]-1]->param_type == P_REAL)
         {
            val_out= *((double*)outvar_list[display_list[count]-1]->param_ptr)*
                               outvar_list[display_list[count]-1]->mult_factor ;

            if (!isnan (val_out))
            {
               if (val_out < min_val [count])
                  min_val [count] = val_out ;
               if (val_out > max_val [count])
                  max_val [count] = val_out ;
            }
         }
         count++ ;
      }
   }

}



void print_min_max () 
{
   int     count ;

   fprintf (extre_out, "SIMULATION MINIMA:\n") ;
   fprintf (extre_out, "------------------\n") ;

   count = 0 ;
   while (display_list [count] != 0)
   {
      if (outvar_list[display_list[count]-1]->param_type == P_REAL)
      {
         fprintf (extre_out, " %s", outvar_list[display_list[count]-1]->p_hdr) ;

         if (tab_delim)
            fprintf (extre_out, "\t") ;
      }
      count++ ;
   }
   fprintf (extre_out, "\n") ;

   count = 0 ;
   while (display_list [count] != 0)
   {
      if (outvar_list[display_list[count]-1]->param_type == P_REAL)
      {
         fprintf (extre_out, 
                   outvar_list[display_list[count]-1]->p_fmt, min_val[count]) ;

         if (tab_delim)
            fprintf (extre_out, "\t") ;
      }
      count++ ;
   }
   fprintf (extre_out, "\n\n") ;

   fprintf (extre_out, "SIMULATION MAXIMA:\n") ;
   fprintf (extre_out, "------------------\n") ;

   count = 0 ;
   while (display_list [count] != 0)
   {
      if (outvar_list[display_list[count]-1]->param_type == P_REAL)
      {
         fprintf (extre_out, " %s", outvar_list[display_list[count]-1]->p_hdr) ;

         if (tab_delim)
            fprintf (extre_out, "\t") ;
      }
      count++ ;
   }
   fprintf (extre_out, "\n") ;

   count = 0 ;
   while (display_list [count] != 0)
   {
      if (outvar_list[display_list[count]-1]->param_type == P_REAL)
      {
         fprintf (extre_out, 
                   outvar_list[display_list[count]-1]->p_fmt, max_val[count]) ;

      if (tab_delim)
            fprintf (extre_out, "\t") ;
      }
      count++ ;
   }
   fprintf (extre_out, "\n\n") ;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void show_parameters ()
{
   fprintf (sparm_out, "\n\n") ;
   fprintf (sparm_out,
      "---------------------------------------------------------------\n");
   fprintf (sparm_out,
      "Ephemeris Time      = %s %d\n", Str_FromGMT(&ephem_gmt),ephem_year);
   fprintf (sparm_out,
      "---------------------------------------------------------------\n");
   fprintf (sparm_out,
      "Apogee  Altitude    = %9.3f (km)\n",(r_apogee-MEAN_R)/1000.0) ;
   fprintf (sparm_out,
      "Perigee Altitude    = %9.3f (km)\n",(r_perigee-MEAN_R)/1000.0) ;
   fprintf (sparm_out, "\n") ;
   fprintf (sparm_out,
      "Inclination         = %9.4f (deg)\n", RAD_TO_DEG (inclination)) ;
   fprintf (sparm_out,
      "RAAN                = %9.4f (deg)\n", RAD_TO_DEG (raan)) ;
   fprintf (sparm_out,
      "Argument of Perigee = %9.4f (deg)\n", RAD_TO_DEG (arg_perigee)) ;

   if (anomaly_type == MEAN_ANOMALY)
      fprintf (sparm_out,
      "Mean Anomaly        = %9.4f (deg)\n", RAD_TO_DEG (mean_anomaly)) ;
   else
      fprintf (sparm_out,
      "True Anomaly        = %9.4f (deg)\n", RAD_TO_DEG (true_anomaly)) ;

   fprintf (sparm_out, "\n") ;
   fprintf (sparm_out,
      "Simulation Start    = %s %d\n", Str_FromGMT(&start_gmt),start_year);
   fprintf (sparm_out,
      "Simulation Stop     = %s %d\n", Str_FromGMT(&stop_gmt ),stop_year) ;
   fprintf (sparm_out,
      "Time increment      = %s   \n", Str_FromGMT(&incr_gmt)) ;
   fprintf (sparm_out, "\n") ;
   fprintf (sparm_out,
      "Tether Start Pos    = %9.3f (km)\n", tether_start / 1000.0) ;
   fprintf (sparm_out,
      "Tether End   Pos    = %9.3f (km)\n", tether_end   / 1000.0) ;
   fprintf (sparm_out,
      "Ex Boom Length      = %9.3f (m)\n",  efield_boom_x) ;
   fprintf (sparm_out,
      "Ey Boom Length      = %9.3f (m)\n",  efield_boom_y) ;
   fprintf (sparm_out,
      "---------------------------------------------------------------\n");
   fprintf (sparm_out,
      "Perturb Orbit       = %s   \n", orbit_perturb ? "YES" : "NO") ;
   fprintf (sparm_out,
      "Decay Orbit         = %s   \n", orbit_decay   ? "YES" : "NO") ;

   if (orbit_decay)
      fprintf (sparm_out,
      "Ballistic Coeff.    = %9.3f\n", bal_coeff) ;

   fprintf (sparm_out,
      "---------------------------------------------------------------\n");
   fprintf (sparm_out,
      "Radius of Apogee    = %9.3f (km)\n", r_apogee  / 1000.0) ;
   fprintf (sparm_out,
      "Radius of Perigee   = %9.3f (km)\n", r_perigee / 1000.0) ;
   fprintf (sparm_out,
      "Semi-major Axis     = %9.3f (km)\n", semi_major_axis / 1000.0) ;
   fprintf (sparm_out,
      "Eccentricity        = %10.8f    \n", eccentricity) ;
   if (anomaly_type == MEAN_ANOMALY)
      fprintf (sparm_out,
      "True Anomaly        = %10.4f (deg)\n", RAD_TO_DEG (true_anomaly)) ;
   fprintf (sparm_out, "\n") ;
   fprintf (sparm_out,
      "Mean Motion         = %10.7f (rad/s)\n", mean_motion) ;
   fprintf (sparm_out,
      "                    = %10.5f (deg/s)\n", RAD_TO_DEG (mean_motion)) ;
   fprintf (sparm_out,
      "Orbital Period      = %10.4f (min)  \n", orbit_period / 60.0) ;
   fprintf (sparm_out,
      "---------------------------------------------------------------\n");
   fprintf (sparm_out, 
      "Libration Time 0     = %s %d\n", Str_FromGMT (&lib_gmt), lib_year) ;
   fprintf (sparm_out, "\n") ;

   fprintf (sparm_out,
      "IP Lib Magnitude     = %10.4f (meters)\n", libration_mag_ip) ;
   fprintf (sparm_out,
      "OP Lib Magnitude     = %10.4f (meters)\n", libration_mag_op) ;
   fprintf (sparm_out, "Radial Lib Magnitude = %10.4f (%% elongation)\n",
                     libration_mag_ra ) ;
   fprintf (sparm_out, "\n") ;

   fprintf (sparm_out,
      "IP Initial Phase     = %8.3f (deg)\n", RAD_TO_DEG (ip_lib_phase0));
   fprintf (sparm_out,
      "OP Initial Phase     = %8.3f (deg)\n", RAD_TO_DEG (op_lib_phase0));
   fprintf (sparm_out,
      "RA Initial Phase     = %8.3f (deg)\n", RAD_TO_DEG (ra_lib_phase0));
   fprintf (sparm_out, "\n") ;

   fprintf (sparm_out,
      "IP Libration Period  = %10.4f (min)\n", DAY_TO_MIN(ip_lib_period));
   fprintf (sparm_out,
      "OP Libration Period  = %10.4f (min)\n", DAY_TO_MIN(op_lib_period));
   fprintf (sparm_out,
      "EL Libration Period  = %10.4f (min)\n", DAY_TO_MIN(ra_lib_period));
   fprintf (sparm_out,
      "---------------------------------------------------------------\n");

   if (orbit_perturb)
   {
      fprintf (sparm_out,
      "Due to the effects of the Sun, Moon and Earth's oblateness:\n") ;
      fprintf (sparm_out,
      "Precession of RAAN        = %10.6f (deg/day)\n",
               RAD_TO_DEG (d_raan)) ;
      fprintf (sparm_out,
      "Precession of Arg Perigee = %10.6f (deg/day)\n",
               RAD_TO_DEG (d_arg_perigee)) ;
      fprintf (sparm_out,
      "---------------------------------------------------------------\n");
   }

   if (module_list [MOD_TSS_CURRENT].compute_module)
   {
      fprintf (sparm_out,
      "Current Collection Mode = %10s\n", tss_mode_str [tss_current_mode]) ;
      fprintf (sparm_out,
      "Commanded EGA Gun       = %d (Perveance = %8.2e)\n", comm_ega_gun,
                                    ega_perveance[comm_ega_gun]) ;
      fprintf (sparm_out,
      "Commanded EGA Current   = %5.3f (Amps)\n", comm_i_ega) ;
      fprintf (sparm_out,
      "Commanded FPEG Gun      = %2d\n", comm_fpeg_gun) ;
      fprintf (sparm_out,
      "SETS Load Resistor      = %7s = %10.4e (Ohms)\n", 
                                   tcvm_resistor_str   [comm_r_tcvm], Rtcvm) ;
      fprintf (sparm_out,
      "Tether Resistance       = %10.4f (Ohms)\n", Rt) ;
      fprintf (sparm_out,
      "---------------------------------------------------------------\n");
   }

   fprintf (sparm_out, "\n\n") ;
}


void show_headers ()
{
   int    count = 0 ;
   short  num_cols  ;
   short  num_comm  ;
   int    comm_len  ;
   int    comm_rem  ;
   int    index     ;

   if (binarify)
   {
      while (display_list [count] != 0)
      {
         if (outvar_list[display_list[count]-1]->param_type == P_REAL_ARRAY)
            count += *(outvar_list[display_list[count]-1]->array_length) ;
         else
            count++ ;
      }
      num_cols = count ;

      comm_len = strlen (comments) ;
      num_comm = comm_len / (num_cols * 8) ;
      comm_rem = comm_len % (num_cols * 8) ;

      if (comm_rem != 0)
         num_comm++ ;

      fprintf (simul_out, "$P\r\n") ;

      fwrite  (&num_cols, sizeof (short), 1, simul_out) ;
      fwrite  (&num_comm, sizeof (short), 1, simul_out) ;

      for (count = 9 ; count <= num_cols*8 ; count++)
         fprintf (simul_out, "%c", 0) ; 

      if (comm_len > 0)
      {
         fwrite (comments, comm_len, 1, simul_out) ;

         if (comm_rem != 0)
            for (count = comm_rem ; count < num_cols * 8 ; count++)
               fprintf (simul_out, "%c", 0) ;
      }
   }
   else
   {
      if (show_comments)
         fprintf (simul_out, "%s", comments) ;
                                /* Print # sign to comment out header        */
      fprintf (simul_out, "#") ;

      while (display_list [count] != 0)
      {
         if (outvar_list[display_list[count]-1]->param_type == P_REAL_ARRAY)
         {
            for (index = 0 ;
                 index < *(outvar_list[display_list[count]-1]->array_length) ;
                 index++)

               fprintf (simul_out," %s",
                  outvar_list[display_list[count]-1]->p_hdr) ;
               if (tab_delim && (index+1) !=
                     *(outvar_list[display_list[count]-1]->array_length))
                  fprintf (simul_out, "\t") ;
         }
         else
            fprintf (simul_out," %s",outvar_list[display_list[count]-1]->p_hdr);

         if (tab_delim)
            fprintf (simul_out, "\t") ;

         count++ ;
      }
      fprintf (simul_out, "\n") ;
   }
}


void show_plotl_labels () 
{
   int count = 0 ;
   int index = 0 ;

   while (display_list [count] != 0)
   {
      if (outvar_list[display_list[count]-1]->param_type == P_REAL_ARRAY)
      {
         for (index = 0 ;
              index < *(outvar_list[display_list[count]-1]->array_length) ;
              index++)

            fprintf (plotl_out,"%s [%d]\n",
               outvar_list[display_list[count]-1]->plotl_label, index) ;
      }
      else
         fprintf (plotl_out, "%s\n", 
            outvar_list[display_list[count]-1]->plotl_label) ;

      count++ ;
   }
   fflush (plotl_out) ;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


char *strupper (char *string)
{
   int count ;
   int len   ;

   if (string != NULL)
   {
      len = strlen (string) ;

      for (count = 0 ; count < len ; count++)
         string [count] = toupper (string [count]) ;
   }

   return (string) ;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void show_cartesian (FILE *f, char *s1, Cartesian vect, double scale, char *s2) 
{
   fprintf (f,"%s(%11.4f, %11.4f, %11.4f)%s", s1, vect.X/scale, vect.Y/scale,
                                                  vect.Z/scale, s2) ;
}



void show_spherical (FILE *f, char *s1, Spherical vect, double scale, char *s2) 
{
   fprintf (f,"%s(%11.4f, T=%11.4f, P=%11.4f)%s", s1, vect.R/scale,
                          RAD_TO_DEG (vect.Th), RAD_TO_DEG(vect.Ph), s2) ;
}



void show_earth (FILE *f, char *s1, Earth vect, char *s2) 
{
   fprintf (f,"%s(%11.6f, %11.6f) @ %9.2f%s", s1, RAD_TO_DEG(vect.Long),
                                              RAD_TO_DEG(vect.Lat),
                                              vect.Alt / 1000.0, s2) ;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define GMT_Seconds(g)		((g)->s+(g)->m*60+(g)->h*3600+(g)->d*86400)
#define GMT_uSeconds(g)		((g)->ms*1000)

void print_sim_results ()
{
   int     count    = 0 ;
   double  real_out     ;
   int     int_out      ;
   char   *str_out      ;
   char   *gmt_out      ;
   time_t  now_time     ;
   GSETime time_gse     ;
   int     index        ;

   if (delta_out_time > 0)
   {
      while (next_out_time > (now_time = time ((time_t *) NULL))) ; 

      if ((next_out_time-now_time) < delta_out_time)
         next_out_time += delta_out_time ;
   }
   
   if (binarify)
   {
      while (display_list [count] != 0)
      {
         switch (outvar_list[display_list[count]-1]->param_type)
         {
            case P_STR  : real_out = 0.0 ;
                          break ;
            case P_GMT  : if (gse_time)
                          {
                             time_gse.seconds = (UINT) GMT_Seconds ((GMT *)
                                outvar_list[display_list[count]-1]->param_ptr) ;
                             time_gse.microseconds = (UINT) GMT_uSeconds ((GMT *)
                                outvar_list[display_list[count]-1]->param_ptr) ;
                          }
                          else
                          {
                             real_out = 24.0 * Day_FromGMT (1900, (GMT *)
                              outvar_list[display_list[count]-1]->param_ptr) ;
                          }
                          break ;
            case P_INT  : real_out = (double) (*((int *)
                           outvar_list[display_list[count]-1]->param_ptr) *
                          (int)outvar_list[display_list[count]-1]->mult_factor);
                          break ;
            case P_REAL_ARRAY :
               for (index = 0 ;
                    index < *(outvar_list[display_list[count]-1]->array_length);
                    index++) 
               {
                  real_out = (((double *)
                     outvar_list[display_list[count]-1]->param_ptr)[index]) *
                     outvar_list[display_list[count]-1]->mult_factor ;
                  if (isnan (real_out))
                  {
                     fprintf(stderr,"WARNING: NaN value of %s set to 0.0 at %s !\007\n",
                        outvar_list[display_list[count]-1]->p_hdr,
                        Str_FromGMT (&curr_gmt)) ;
                     real_out = 0.0 ;
                  }
                  fwrite (&real_out, sizeof (double) , 1, simul_out) ;
               }
               break ;
            case P_REAL :
            case P_TIME :
            default     : real_out = *((double *)
                           outvar_list[display_list[count]-1]->param_ptr) *
                           outvar_list[display_list[count]-1]->mult_factor ;
                          if (isnan (real_out))
                          {
                             fprintf (stderr, "WARNING: NaN value of %s set to 0.0 at %s !\007\n",
                                outvar_list[display_list[count]-1]->p_hdr,
                                Str_FromGMT (&curr_gmt)) ;
                             real_out = 0.0 ;
                          }
                          break ;
         }

         if (outvar_list[display_list[count]-1]->param_type==P_GMT && gse_time)
            fwrite (&time_gse, sizeof (GSETime), 1, simul_out) ;
         else if(outvar_list[display_list[count]-1]->param_type != P_REAL_ARRAY)
            fwrite (&real_out, sizeof (double) , 1, simul_out) ;

         count++ ;
      }
   }
   else 
   {
      fprintf (simul_out, " ") ;
      while (display_list [count] != 0)
      {
         switch (outvar_list[display_list[count]-1]->param_type)
         {
            case P_STR  : str_out =
                           outvar_list[display_list[count]-1]->param_ptr ;
                          fprintf (simul_out,
                           outvar_list[display_list[count]-1]->p_fmt, str_out) ;
                          break ;
            case P_GMT  : gmt_out = Str_FromGMT (
                           outvar_list[display_list[count]-1]->param_ptr) ;
                          fprintf (simul_out,
                           outvar_list[display_list[count]-1]->p_fmt, gmt_out) ;
                          break ;
            case P_INT  : int_out = *((int *)
                           outvar_list[display_list[count]-1]->param_ptr) *
                           (int)outvar_list[display_list[count]-1]->mult_factor;
                          fprintf (simul_out,
                           outvar_list[display_list[count]-1]->p_fmt, int_out) ;
                          break ;
            case P_REAL_ARRAY :
               for (index = 0 ;
                    index < *(outvar_list[display_list[count]-1]->array_length);
                    index++) 
               {
                  real_out = (((double *)
                     outvar_list[display_list[count]-1]->param_ptr)[index]) *
                     outvar_list[display_list[count]-1]->mult_factor ;
                  if (isnan (real_out))
                  {
                     fprintf(stderr,"WARNING: NaN value of %s set to 0.0 at %s !\007\n",
                        outvar_list[display_list[count]-1]->p_hdr,
                        Str_FromGMT (&curr_gmt)) ;
                     real_out = 0.0 ;
                  }
                  fprintf (simul_out,
                     outvar_list[display_list[count]-1]->p_fmt, real_out);
                  if (tab_delim && (index+1) !=
                         *(outvar_list[display_list[count]-1]->array_length))
                     fprintf (simul_out, "\t") ;
               }
               break ;
            case P_REAL :
            case P_TIME :
            default     : real_out = *((double *)
                           outvar_list[display_list[count]-1]->param_ptr) *
                           outvar_list[display_list[count]-1]->mult_factor ;
                          if (isnan (real_out))
                          {
                             fprintf (stderr, "WARNING: NaN value of %s set to 0.0 at %s !\007\n",
                                outvar_list[display_list[count]-1]->p_hdr,
                                Str_FromGMT (&curr_gmt)) ;
                             real_out = 0.0 ;
                          }
                          fprintf (simul_out,
                           outvar_list[display_list[count]-1]->p_fmt, real_out);
                          break ;
         }
   
         if (tab_delim)
            fprintf (simul_out, "\t") ;

         count++ ;
      }

      if (!global_sim)
         fprintf (simul_out, "\n") ;
   }
}



void print_zero_results ()
{
   int     count    = 0 ;
   double  real_out     ;
   char   *gmt_out      ;
   time_t  now_time     ;
   int     index        ;

   if (delta_out_time > 0)
   {
      while (next_out_time > (now_time = time ((time_t *) NULL))) ;

      if ((next_out_time-now_time) < delta_out_time)
         next_out_time += delta_out_time ;
   }
   
   if (binarify)
   {
      while (display_list [count] != 0)
      {
         switch (outvar_list[display_list[count]-1]->param_type)
         {
            case P_STR  : real_out = 0.0 ;
            case P_INT  : 
            case P_REAL :
            default     :
                          break ;
            case P_TIME : real_out = *((double *)
                           outvar_list[display_list[count]-1]->param_ptr) *
                           outvar_list[display_list[count]-1]->mult_factor ;
            case P_GMT  : real_out = 24.0 * Day_FromGMT (1900, (GMT *)
                           outvar_list[display_list[count]-1]->param_ptr) ;
                          break ;
         }
   
         fwrite (&real_out, sizeof (double), 1, simul_out) ;

         count++ ;
      }
   }
   else 
   {
      fprintf (simul_out, " ") ;
      while (display_list [count] != 0)
      {
         switch (outvar_list[display_list[count]-1]->param_type)
         {
            case P_STR  : 
                          fprintf (simul_out,
                           outvar_list[display_list[count]-1]->p_fmt, "-") ;
                          break ;
            case P_GMT  : gmt_out = Str_FromGMT (
                           outvar_list[display_list[count]-1]->param_ptr) ;
                          fprintf (simul_out,
                           outvar_list[display_list[count]-1]->p_fmt, gmt_out) ;
                          break ;
            case P_INT  : 
                          fprintf (simul_out,
                           outvar_list[display_list[count]-1]->p_fmt, 0) ;
                          break ;
            case P_TIME : real_out = *((double *)
                           outvar_list[display_list[count]-1]->param_ptr) *
                           outvar_list[display_list[count]-1]->mult_factor ;
                          fprintf (simul_out,
                           outvar_list[display_list[count]-1]->p_fmt, real_out);
                          break ;
            case P_REAL :
            default     :
                          fprintf (simul_out,
                           outvar_list[display_list[count]-1]->p_fmt, 0.0) ;
                          break ;
         }
   
         if (tab_delim)
            fprintf (simul_out, "\t") ;

         count++ ;
      }

      if (!global_sim)
         fprintf (simul_out, "\n") ;
   }
}
