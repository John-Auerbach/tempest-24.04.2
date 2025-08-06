/*****************************************************************************/
/*                                                                           */
/*   Module:	chop.c                                                       */
/*                                                                           */
/*   Purpose:	This program 'chop's out a section of a file based on an     */
/*		in range GMT (time) in either an ASCII of binarified file.   */
/*                                                                           */
/*   Inputs:	Command line options and optionall STDIN.                    */
/*                                                                           */
/*   Outputs:	Chopped data to file or STDOUT.                              */
/*                                                                           */
/*   Uses:	Declarations from: "gmt.h".                                  */
/*                                                                           */
/*   History:	03_Nov_95   Written.                                         */
/*                                                                           */
/*   RCS:	$Id: chop.c,v 1.4 1996/07/23 20:33:45 nestorv Exp $                                                        */
/*                                                                           */
/*		$Log: chop.c,v $
 * Revision 1.4  1996/07/23  20:33:45  nestorv
 * Allowed chop to deal with binarified comments.
 *
 * Revision 1.3  1995/11/07  16:46:18  nestorv
 * Changed DAY_FromGMT to Day_FromGMT.
 *
 * Revision 1.2  1995/11/03  07:24:01  nestorv
 * Fixed formatting in usage message.
 *
 * Revision 1.1  1995/11/03  06:40:48  nestorv
 * Initial revision
 *                                                       */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmt.h"

#ifndef FALSE

#define FALSE 0
#define TRUE  1

#endif

#define MAX_COLS	50

void show_usage ()
{
   fprintf (stderr, "\007\n%s\n",
"	Usage:	chop	-start <time> (-end <time>)|(-dur <time>)");
   fprintf (stderr, "%s\n",
"			-f <input filename> [-o <output filename>") ;
   fprintf (stderr, "%s\n\n",
"			[-col <column containing time in GMT format>]") ;
   fprintf (stderr, "%s\n",
"			All times are in GMT format ddd/hh:mm:ss.xxx") ;
   fprintf (stderr, "%s\n",
"			Column 1 is the default time column and for text") ;
   fprintf (stderr, "%s\n",
"			files, space and tab are valid delimeters and lines") ;
   fprintf (stderr, "%s\n",
"			with a # in the first column are ignored.  Files") ;
   fprintf (stderr, "%s\n\n",
"			that start with a $P\\r\\n are considered binarified.");
}




int read_in_line (FILE *file, char *linein, int maxlen)
{
   int linelen = 0 ;

   while (((linein [linelen] = (char) fgetc (file)) != '\n') &&
           (linein [linelen] != EOF) && (linelen <= maxlen))
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



void main (int argc, char *argv[])
{
   GMT     start_gmt    = {-1,-1,-1,-1,-1} ;
   double  start_time                      ;
   GMT     stop_gmt     = {-1,-1,-1,-1,-1} ;
   double  stop_time                       ;
   GMT     dur_gmt      = {-1,-1,-1,-1,-1} ;
   int     timecol      = 1                ;
   int     count                           ;
   int     year         = 1900             ;
   int     verbose      = FALSE            ;
   int     infile_argc  = 0                ;
   int     outfile_argc = 0                ;
   FILE   *fin                             ;
   FILE   *fout                            ;
   char    firstfour [4]                   ;
   short   num_cols                        ;
   short   num_comm                        ;
   double  values_bin [50]                 ;
   unsigned long lines  = 0                ;
   char    line_in [513]                   ;
   int     linelen                         ;
   char    fmt_str [200]                   ;
   char    gmt_str [ 20]                   ;
   char    buffer  [MAX_COLS*8]            ;
   GMT     curr_gmt                        ;

   if (argc < 2)
   {
      show_usage () ;
      exit (-1) ;
   }

   for (count = 1 ; count < argc ; count++)
   {
      if     ((strcasecmp (argv[count], "-help" ) == 0) ||
              (strcasecmp (argv[count], "-doc"  ) == 0))
      {
         show_usage () ;
         exit (-1) ;
      }
      else if (strcasecmp (argv[count], "-v"    ) == 0)
      {
         verbose = TRUE ;
      }
      else if (strcasecmp (argv[count], "-start") == 0)
      {
         if (count+1 < argc)
            GMT_FromStr (argv[++count], &start_gmt) ;
         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-START parameter requires time in GMT format ddd/hh:mm:ss.xxx") ;
            exit (-1) ;
         }
      }
      else if (strcasecmp (argv[count], "-stop" ) == 0)
      {
         if (count+1 < argc)
            GMT_FromStr (argv[++count], &stop_gmt) ;

         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-STOP parameter requires time in GMT format ddd/hh:mm:ss.xxx") ;
            exit (-1) ;
         }
      }
      else if (strcasecmp (argv[count], "-dur"  ) == 0)
      {
         if (count+1 < argc)
            GMT_FromStr (argv[++count], &dur_gmt) ;
         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-DUR parameter requires time in GMT format ddd/hh:mm:ss.xxx") ;
            exit (-1) ;
         }
      }
      else if (strcasecmp (argv[count], "-f"    ) == 0)
      {
         if (count+1 < argc)
            infile_argc = ++count ;
         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-F parameter requires filename") ;
            exit (-1) ;
         }
      }
      else if (strcasecmp (argv[count], "-o"    ) == 0)
      {
         if (count+1 < argc)
            outfile_argc = ++count ;
         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-O parameter requires filename") ;
            exit (-1) ;
         }
      }
      else if (strcasecmp (argv[count], "-col"  ) == 0)
      {
         if (count+1 < argc)
         {
            timecol = atoi (argv[++count]) ;
            if (timecol < 1)
               timecol = 1 ;
         }
         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-COL parameter requires column number") ;
            exit (-1) ;
         }
      }
   }
   
                                   /* Make sure that start time specified    */
   if (start_gmt.d == -1)
   {
      fprintf (stderr,"\nERROR: %s\007!\n\n",
               "Must specify a start time for chop") ;
      exit (-1) ;
   }

                                   /* Make sure that stop or dur specified   */
   if ((stop_gmt.d == -1) && (dur_gmt.d == -1))
   {
      fprintf (stderr,"\nERROR: %s\007!\n\n",
               "Must specify either a stop time or a duration for chop") ;
      exit (-1) ;
   }

                                   /* Open input file or used STDIN          */
   if (infile_argc == 0)
      fin = stdin ;
   else
   {
      if ((fin = fopen (argv [infile_argc], "r")) == NULL)
      {
         fprintf(stderr,"\nERROR: Unable to open input file \"%s\"\007!\n\n",
                         argv [infile_argc]) ;
         exit (-1) ;
      }
   } 
      
                                   /* Open output file or used STDOUT        */
   if (outfile_argc == 0)
      fout = stdout ;
   else
   {
      if ((fout = fopen (argv [outfile_argc], "w")) == NULL)
      {
         fprintf(stderr,"\nERROR: Unable to open output file \"%s\"\007!\n\n",
                         argv [outfile_argc]) ;
         exit (-1) ;
       }
   } 
     
                                   /* Compute stop time = start + dur        */
   if (stop_gmt.d == -1)
   {
      bcopy (&start_gmt, &stop_gmt, sizeof (GMT)) ;
      GMT_Incr (&stop_gmt, &year, &dur_gmt) ;
   }

   if (verbose)
   {
      fprintf (stderr, "Chopping from %s to ",  Str_FromGMT (&start_gmt)) ;
      fprintf (stderr, "%s\n", Str_FromGMT (&stop_gmt)) ;
      if (infile_argc == 0)
         fprintf (stderr, "Input source  = STDIN\n") ;
      else
         fprintf (stderr, "Input source  = %s\n", argv [infile_argc]) ;
      if (outfile_argc == 0)
         fprintf (stderr, "Output source = STDOUT\n") ;
      else
         fprintf (stderr, "Output source = %s\n", argv [outfile_argc]) ;
      fprintf (stderr, "Time column = %d\n", timecol) ;
   }

                                   /* Read first 4 bytes to check binarify   */
   fread  (firstfour, 1, 4, fin) ;

                                   /* Do chopping                            */
   if (bcmp (firstfour, "$P\r\n", 4) == 0)
   {
                                   /* Convert time column to array index     */
      timecol-- ;

      if (verbose)
         fprintf (stderr, "Processing binarified input file.\n") ;

                                   /* Read in number of columns in data      */
      fread (&num_cols, sizeof (short), 1, fin) ;
      fread (&num_comm, sizeof (short), 1, fin) ;
                                   /* Read in the rest of the header         */
      fread (values_bin, sizeof (double), num_cols-1, fin) ;

      if (verbose)
         fprintf (stderr, "Binarified file has %d columns of data\n", num_cols);

                                   /* Write out binarify header              */
      fprintf (fout,  "$P\r\n") ;
      fwrite  (&num_cols, sizeof (short), 1, fout) ;
      fwrite  (&num_comm, sizeof (short), 1, fout) ;
      for (count = 9 ; count <= num_cols*8 ; count++)
         fprintf (fout, "%c", 0) ;
                                   /* Read & Write comment headers           */
      for (count = 1 ; count <= num_comm ; count++)
      {
         fread  (buffer, sizeof (double), num_cols, fin ) ;
         fwrite (buffer, sizeof (double), num_cols, fout) ;
      }
                                   /* Compute time in hours                  */
      start_time = 24.0 * Day_FromGMT (1900, &start_gmt) ;
      stop_time  = 24.0 * Day_FromGMT (1900, &stop_gmt ) ;

      do
      {
         if (fread (values_bin, sizeof(double), num_cols, fin) == num_cols)
         {
            if (values_bin [timecol] >  stop_time )
                     break ;

            if (values_bin [timecol] >= start_time)
            {
               fwrite (values_bin, sizeof (double), num_cols, fout) ;
               lines++ ;
            }
         }
         else
            break ;
      }
      while (!feof (fin)) ; 
   }
   else
   {
      if (verbose)
         fprintf (stderr, "Processing ASCII input file.\n") ;
      rewind (fin) ;

                                   /* Create format string for timecol      */
      fmt_str [0] = '\0' ;
      for (count = 1 ; count < timecol ; count++)
         strcat (fmt_str, "%*s ") ;
      strcat (fmt_str, "%s") ;

      do
      {
         linelen = read_in_line (fin, line_in, 512) ;

         if ((linelen > 0) && (line_in[0] != '#'))
         {
            sscanf (line_in, fmt_str, gmt_str) ;

            GMT_FromStr (gmt_str, &curr_gmt) ;

            if (GMT_Comp (1900, &curr_gmt, 1900, &stop_gmt ) >  0)
               break ;

            if (GMT_Comp (1900, &curr_gmt, 1900, &start_gmt) >= 0)
            {
               fprintf (fout, "%s\n", line_in) ;
               lines++ ;
            }
         }
      }
      while (!feof(fin)) ;
   }

   if (verbose)
      fprintf (stderr, "%lu lines written\n", lines) ;

   fclose (fin ) ;
   fclose (fout) ;
}
