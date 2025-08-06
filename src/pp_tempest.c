/*****************************************************************************/
/*                                                                           */
/*   Module:    pp_tempest.c                                                 */
/*                                                                           */
/*   Purpose:	This is the TEMPEST post-processor program.                  */
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
/* #include <malloc.h> */

#include "types.h"
#include "gmt.h"

#ifndef TRUE
typedef enum {FALSE, TRUE} BOOLEAN ;
#endif

#define DEBUG                 1

#define VERSION          "0.901"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int     show_debug     = FALSE ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

char pp_linein [400]           ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define DEF_INPUT_IN      stdin
#define DEF_OUTPUT_OUT    stdout
#define DEF_DEBUG_OUT     stderr

FILE  *input_in        = NULL   ;   /* Default file for program input        */
FILE  *output_out      = NULL   ;   /* Default file for program output       */
FILE  *debug_out       = NULL   ;   /* Default file for debugging output     */

char   input_fname  [80] = {""} ;   /* Filename for post-processor input     */
char   debug_fname  [80] = {""} ;   /* Filename for debugging output         */
char   output_fname [80] = {""} ;   /* Filename for post-processor output    */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int    adist_col         =   0   ;  /* Compute distribution of angles in     */
                                    /*   in this column (first is 0)         */

int    num_adist_bins    =  45   ;  /* Number of bins in distribution        */

double adist_max         =  90.0 ;  /* Maximum angle for distribution        */

char   adist_col_fmt [40]        ;  /* Format to extract angle for sscanf    */

double adist_quant               ;  /* Quantization factor for distribution  */

int   *adist_bins        = NULL  ;  /* Actual bins for distribution          */

int    num_points        =    0  ;  /* Number of points processed            */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int    lat_col           =    0  ;  /* Column number for latitude  (degrees) */
int    lon_col           =    0  ;  /* Column number for longitude (degrees) */

double lat_cent          =   0.0 ;  /* Latitude  of center of circle         */
double lon_cent          =   0.0 ;  /* Longitude of center of circle         */
double pos_rads          =   5.0 ;  /* Radius of circle (degrees)            */

char   locate_col_fmt [40]       ;  /* Format to extract time, lat & long    */

/*===========================================================================*/


char *strupper (char *string)
{
   int count ;
   int len   ;

   len = strlen (string) ;

   for (count = 0 ; count < len ; count++)
      string [count] = toupper (string [count]) ;

   return (string) ;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



void show_help ()
{
printf ("\n") ;
printf ("Usage:   pp_tempest [-help] [-doc]\n") ;
printf ("                    [-f <filename>] [-o <filename>]\n") ;
printf ("                    [-adist <column #>] [-abins <# of bins>]\n") ;
printf ("                    [-ahalf] [-afull]\n") ;
printf ("                    [-latlon <lat col #> <lon col #>]\n") ;
printf ("                    [-locat <lat> <long> <radius>]\n") ;
printf ("\n") ;
}



void show_doc ()
{
   show_help () ;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void process_cmdline_params (int argc, char *argv []) 
{
   int count ;

   if (argc > 1)
   {
      for (count = 1 ; count < argc; count++)
      {
         if      (strcmp (strupper (argv[count]), "-HELP"    ) == 0)
         {
            show_help () ;
            exit (0) ;
         }
         else if (strcmp (strupper (argv[count]), "-DOC"     ) == 0)
         {
            show_doc () ;
            exit (0) ;
         }
         else if (strcmp (strupper (argv[count]), "-F"    ) == 0)
         {
            if (count+1 < argc)
               strcpy (input_fname, argv[++count]) ;
            else
            {
                fprintf (stderr, "-F parameter requires filename\007\n\n") ;
                exit (-1) ;
            }
         }
         else if (strcmp (strupper (argv[count]), "-DEBUG") == 0)
         {
            show_debug = TRUE ;
         }
         else if (strcmp (strupper (argv[count]), "-DOUT" ) == 0)
         {
            if (count+1 < argc)
               strcpy (debug_fname, argv[++count]) ;
            else
            {
                fprintf (stderr, "-DEBUG parameter requires filename\007\n\n") ;
                exit (-1) ;
            }
         }
         else if (strcmp (strupper (argv[count]), "-O"    ) == 0)
         {
            if (count+1 < argc)
               strcpy (output_fname, argv[++count]) ;
            else
            {
                fprintf (stderr, "-O parameter requires filename\007\n\n") ;
                exit (-1) ;
            }
         }
         else if (strcmp (strupper (argv[count]), "-ADIST") == 0)
         {
            if (count+1 < argc)
               adist_col = atoi (argv[++count]) ;
            else
            {
                fprintf (stderr, "-ADIST parameter requires column #\007\n\n") ;
                exit (-1) ;
            }
         }
         else if (strcmp (strupper (argv[count]), "-ABINS") == 0)
         {
            if (count+1 < argc)
               num_adist_bins = atoi (argv[++count]) ;
            else
            {
                fprintf (stderr, "-ABINS parameter requires number\007\n\n") ;
                exit (-1) ;
            }
         }
         else if (strcmp (strupper (argv[count]), "-AHALF") == 0)
         {
            adist_max = 180.0 ;
         }
         else if (strcmp (strupper (argv[count]), "-AFULL") == 0)
         {
            adist_max = 360.0 ;
         }
         else if (strcmp (strupper (argv[count]), "-LATLON") == 0)
         {
            if (count+2 < argc)
            {
               lat_col = atoi (argv[++count]) ;
               lon_col = atoi (argv[++count]) ;
            }
            else
            {
               fprintf(stderr,"-LATLON parameter requires two numbers\007\n\n");
               exit (-1) ;
            }
         }
         else if (strcmp (strupper (argv[count]), "-LOCAT") == 0)
         {
            if (count+3 < argc)
            {
               lat_cent = atof (argv[++count]) ;
               lon_cent = atof (argv[++count]) ;
               pos_rads = atof (argv[++count]) ;
            }
         }
         else
         {
            fprintf (stderr, "\nInvalid paramter %s\007\n\n", argv[count]) ;
            exit (-1) ;
         }
      }
   }
}



void init_file_access ()
{
   if (strlen(input_fname) > 0)
   {
      if ( (input_in = fopen (input_fname, "r")) == NULL)
      {
         fprintf (stderr, "File open for %s failed\007\n\n", input_fname ) ;
         exit (-1) ;
      }
   }
   else
      input_in  = DEF_INPUT_IN ;

   if (strlen(output_fname) > 0)
   {
      if ( (output_out = fopen (output_fname, "w")) == NULL)
      {
         fprintf (stderr, "File open for -O %s failed\007\n\n", output_fname);
         exit (-1) ;
      }
   }
   else
      output_out = DEF_OUTPUT_OUT ;

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
}



void end_file_access ()
{
   fclose (input_in  ) ;
   fclose (output_out) ;
   fclose (debug_out ) ;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



void init_adist () 
{
   int count ;

   fprintf (stderr, "Computing distribution from column %d with range of ",
                    adist_col) ;

   if      (adist_max == 360.0)
      fprintf (stderr, "(-180, 180)") ;
   else if (adist_max == 180.0)
      fprintf (stderr, "(0, 180)") ;
   else if (adist_max ==  90.0)
      fprintf (stderr, "(0, 90)") ;
   
   fprintf (stderr, " degrees\n") ;

                                   /* Allocate space for distribution bins   */
   adist_bins = (int *) malloc (num_adist_bins * sizeof (int)) ;
                                   /* Initialize the counts in the bins to 0 */
   for (count = 0 ; count < num_adist_bins ; count++)
      adist_bins [count] = 0 ;
                                   /* Compute quantization factor for bins   */
   adist_quant = (double ) (num_adist_bins) / adist_max ;

                                   /* Construct sscanf format for our column */
   strcpy (adist_col_fmt, "%*s") ;
   for (count = 1 ; count < adist_col ; count++) 
      strcat (adist_col_fmt, " %*s") ;
   strcat (adist_col_fmt, " %lf %*s\\n") ;

   fprintf (stderr, "Placing results into %d bins %7.3f degrees wide\n",
                     num_adist_bins, 1.0/adist_quant) ;
}



void adist_process (char *line_in)
{
   double input_angle ;
   int    bin         ;

                                  /* Extract column from entire line of data */
   sscanf (line_in, adist_col_fmt, &input_angle) ;

                                  /* Convert angle in degrees to bin #       */
   if      (adist_max == 360.0)
      bin = (int) ((180.0 + input_angle) * adist_quant) ;
   else if (adist_max == 180.0)
      bin = (int) (    fabs(input_angle) * adist_quant) ;
   else if (adist_max ==  90.0)
   {
      input_angle = RAD_TO_DEG (asin (sin (DEG_TO_RAD (input_angle)))) ;
      bin = (int) (    fabs(input_angle) * adist_quant) ;
   }

   if (bin > num_adist_bins - 1)
      bin = num_adist_bins -1  ;

                                  /* Increment count in computed bin         */
   adist_bins[bin] ++ ;
}



void adist_results_out ()
{
   double start_theta  ;
   double center_theta ;
   double end_theta    ;
   double tot_count    ;
   int count           ;

                                  /* Get ready to produce output             */
   tot_count = 0 ;
   if (adist_max == 360.0)
      start_theta = -180.0  ;
   else
      start_theta =    0.0  ;

                                  /* Print angle distribution header         */
   fprintf (output_out,
            "#  Start    Center       End     Count   %% Total    %% Upto\n") ;

                                  /* Print distribution computation results  */
   for (count = 0 ; count < num_adist_bins; count++)
   {
      end_theta    = start_theta + 1.0 / adist_quant ;
      center_theta = start_theta + 0.5 / adist_quant ;

      tot_count += adist_bins [count] ;
      fprintf (output_out, "%8.3f  %8.3f  %8.3f  %8d  %8.4f  %8.4f\n",
               start_theta, center_theta, end_theta, adist_bins [count],
               (double) adist_bins [count] / num_points * 100.0,
               (double)         tot_count  / num_points * 100.0) ;

      start_theta += 1.0 / adist_quant ;
   }
                                   /* Free distribution bins space           */
   if (adist_bins != NULL)
      free (adist_bins) ;
}


 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void init_locate ()
{
   int count ;
                                   /* Construct sscanf format for our column */
   strcpy (locate_col_fmt, "%s") ;

   if (lat_col < lon_col)
   {
      for (count = 1 ; count < lat_col ; count++)
         strcat (locate_col_fmt, " %*s") ;
      strcat (locate_col_fmt, " %lf") ;
      for (count = lat_col+1 ; count < lon_col ; count++)
         strcat (locate_col_fmt, " %*s") ;
      strcat (locate_col_fmt, " %lf %*s\\n") ;
   }
   else
   {
      for (count = 1 ; count < lon_col ; count++)
         strcat (locate_col_fmt, " %*s") ;
      strcat (locate_col_fmt, " %lf") ;
      for (count = lat_col+1 ; count < lat_col ; count++)
         strcat (locate_col_fmt, " %*s") ;
      strcat (locate_col_fmt, " %lf %*s\\n") ;
   }
   fprintf (output_out, "Satellite is withing %f degrees of (%f, %f) at:\n", 
             pos_rads, lat_cent, lon_cent) ;
}



void locate_process (char *line_in)
{
   char   time_str [25] ;
   double latitude      ;
   double longitude     ;
   double cur_dist      ;
                                  /* Extract column from entire line of data */
   if (lat_col < lon_col)
      sscanf (line_in, locate_col_fmt, time_str, &latitude, &longitude) ;
   else
      sscanf (line_in, locate_col_fmt, time_str, &longitude, &latitude) ;

   cur_dist = sqrt ((latitude  - lat_cent) * (latitude  - lat_cent) +
                    (longitude - lon_cent) * (longitude - lon_cent)) ;

   if (cur_dist <= pos_rads)
      fprintf (output_out, "%s\n", time_str) ;
}



void locate_process_out ()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



main (int argc, char *argv [])
{
   int rc    ;
   int count ;

   fprintf (stderr,
      "\nTEMPEST Post-processor V%s (compiled on %s %s)\n", 
         VERSION, __DATE__, __TIME__) ;
   fprintf (stderr, "%s\n", 
    "========================================================================");

                                  /* Process command line parameters         */
   process_cmdline_params (argc, argv) ;

                                  /* Open specified files for input/output   */
   init_file_access () ;

                                  /* If needed initialize angle distribution */
   if (adist_col != 0)
      init_adist () ;
                                  /* If needed initialize location overpass  */
   if (lat_col   != 0)
      init_locate () ;
                                  /* Read in first line from input file      */
   fgets (pp_linein, 400, input_in) ;

   do
   {
      if (adist_col != 0)         /* Update angle from current data set      */
         adist_process (pp_linein) ;
      if (lat_col   != 0)         /* Check to see if in given position       */
         locate_process (pp_linein) ;
                                  /* Read in another line from input file    */
      fgets (pp_linein, 400, input_in) ;

      num_points++ ;
   }
   while (!feof (input_in)) ;

   fprintf (stderr, "%d data points processed\n", num_points) ;
   fprintf (stderr, "%s\n", 
    "========================================================================");

   if (adist_col != 0)
      adist_results_out () ; 

   end_file_access () ;

   exit (0) ;
}
