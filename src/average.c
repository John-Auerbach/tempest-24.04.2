/*****************************************************************************/
/*                                                                           */
/*   Module:	average.c                                                    */
/*                                                                           */
/*   Purpose:	This program computes the average of a particular output     */
/*		column.                                                      */
/*                                                                           */
/*   Inputs:	Command line options and optionall STDIN.                    */
/*                                                                           */
/*   Outputs:	average value to STDOUT.      .                              */
/*                                                                           */
/*   Uses:	Declarations from: "gmt.h".                                  */
/*                                                                           */
/*   History:	09_Apr_06  Written.                                          */
/*                                                                           */
/*   RCS:	$Id:$                                                        */
/*                                                                           */
/*		$Log:$ *                                                     */
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
"	Usage:	average	-f <input filename> [-o <output filename>") ;
   fprintf (stderr, "%s\n\n",
"			[-col <column>]") ;
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



int main (int argc, char *argv[])
{
   int     count                           ;
   int     col          = -1               ;
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
   double  average      = 0.0              ;
   double  ave_sqr      = 0.0              ;
   double  minimum      = 0.0              ;
   double  maximum      = 0.0              ;

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
      else if (strcasecmp (argv[count], "-col"  ) == 0)
      {
         if (count+1 < argc)
         {
            col = atoi (argv[++count]) ;
            if (col < 1)
               col = 1 ;
         }
         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-COL parameter requires column number") ;
            exit (-1) ;
         }
      }
   }
   
                                   /* Make sure that column is specified     */
   if (col == -1)
   {
      fprintf (stderr,"\nERROR: %s\007!\n\n",
               "Must specify a column to average") ;
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
      
   if (verbose)
   {
      fprintf (stderr, "Averaging column # %d\n", col) ;
      if (infile_argc == 0)
         fprintf (stderr, "Input source  = STDIN\n") ;
      else
         fprintf (stderr, "Input source  = %s\n", argv [infile_argc]) ;
   }

                                   /* Read first 4 bytes to check binarify   */
   fread  (firstfour, 1, 4, fin) ;

                                   /* Do averaging                           */
   if (bcmp (firstfour, "$P\r\n", 4) == 0)
   {
                                   /* Convert time column to array index     */
      col-- ;

      if (verbose)
         fprintf (stderr, "Processing binarified input file.\n") ;

                                   /* Read in number of columns in data      */
      fread (&num_cols, sizeof (short), 1, fin) ;
      fread (&num_comm, sizeof (short), 1, fin) ;
                                   /* Read in the rest of the header         */
      fread (values_bin, sizeof (double), num_cols-1, fin) ;

                                 /* Read in comment headers                  */
      for (count = 1 ; count <= num_comm ; count++)
      {
         fread  (buffer, sizeof (double), num_cols, fin ) ;
      }

      if (verbose)
      {
         fprintf (stderr, "Binarified file has %d columns of data\n", num_cols);
         fprintf (stderr, "Binarified file has %d records of comments\n", num_comm) ;
      }

      if (col > num_cols)
      {
         fprintf (stderr, "ERROR: Column number too big. Output set to -1\007!!\n\n") ;
         printf ("-1.0\n") ;
         exit (-1) ;
      }
         

      do
      {
         if (fread (values_bin, sizeof(double), num_cols, fin) == num_cols)
         {
            average += values_bin [col] ;
            ave_sqr += (values_bin [col] * values_bin [col]) ;
            
            if (lines == 0)
            {
               minimum = values_bin [col] ;
               maximum = values_bin [col] ;
            }
            else
            {
               if (values_bin [col] < minimum) 
                  minimum = values_bin [col] ;
               if (values_bin [col] > maximum)
                  maximum = values_bin [col] ;
            }

            lines++ ;
         }
         else
            break ;
      }
      while (!feof (fin)) ; 

   }
   else
   {
      fprintf (stderr,"WARNING:  May be erroneous!!\n\n") ;
      if (verbose)
         fprintf (stderr, "Processing ASCII input file.\n") ;
      rewind (fin) ;

                                   /* Create format string for col      */
      fmt_str [0] = '\0' ;
      for (count = 1 ; count < col ; count++)
         strcat (fmt_str, "%*s ") ;
      strcat (gmt_str, "%s") ;

      do
      {
         linelen = read_in_line (fin, line_in, 512) ;

         if ((linelen > 0) && (line_in[0] != '#'))
         {
            sscanf (line_in, fmt_str, gmt_str) ;

            average += atof (gmt_str) ;

            lines++ ;
         }
      }
      while (!feof(fin)) ;
   }

   average = average / ((double) lines);
   ave_sqr = sqrt (ave_sqr / ((double) lines)) ;

   printf ("%f %f %f %f\n", minimum, maximum, average, ave_sqr) ;

   fclose (fin ) ;
}
