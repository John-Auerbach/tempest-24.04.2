/*****************************************************************************/
/*                                                                           */
/*   Module:	binarifyhead.c                                               */
/*                                                                           */
/*   Purpose:	This program determine if a file is binarified or not and    */
/*		if so, prints the desired value from the first entry.        */
/*                                                                           */
/*   Inputs:	Command line options.                                        */
/*                                                                           */
/*   Outputs:	Desired value if possible, else 0.0 or 000/00:00:00.000.     */
/*                                                                           */
/*   Uses:	Declarations from: "gmt.h".                                  */
/*                                                                           */
/*   History:	03_Nov_95   Written.                                         */
/*                                                                           */
/*   RCS:	$Id: binarifyhead.c,v 1.5 1996/03/14 22:55:39 nestorv Exp $                                                        */
/*                                                                           */
/*		$Log: binarifyhead.c,v $
 * Revision 1.5  1996/03/14  22:55:39  nestorv
 * Added code to deal with comments.
 *
 * Revision 1.4  1996/01/30  19:00:14  nestorv
 * Added -fmt option.
 *
 * Revision 1.3  1995/11/07  17:35:32  nestorv
 * Fixed inconsistencies.
 *
 * Revision 1.2  1995/11/07  15:56:35  nestorv
 *  Can read data from STDIN too.
 *
 * Revision 1.1  1995/11/03  07:54:21  nestorv
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

void show_usage ()
{
   fprintf (stderr, "\007\n%s\n",
" Usage: binarifyhead	-f <input filename> -col <desired column of value>") ;
   fprintf (stderr, "%s\n\n",
"			[-gmt] [-fmt <printf format>]") ;
   fprintf (stderr, "%s\n",
"			This program is used to determine whether or not a");
   fprintf (stderr, "%s\n",
"			given file is binarified or not by looking for a") ;
   fprintf (stderr, "%s\n",
"			$P\\r\\n at the beginning.  If the file is binarified");
   fprintf (stderr, "%s\n",
"			then the number in the specified column is printed") ;
   fprintf (stderr, "%s\n\n",
"			in double or GMT format from the first entry.") ;
}




void main (int argc, char *argv[])
{
   int     count                           ;
   int     infile_argc  = 0                ;
   FILE   *fin                             ;
   char    firstfour [4]                   ;
   short   num_cols                        ;
   short   num_comm                        ;
   int     val_col      = 1                ;
   int     gmt_out      = FALSE            ;
   int     year                            ;
   GMT     gmt_val                         ;
   int     printf_fmt   = 0                ;
   double  values_bin [50]                 ;
   char    buffer [500*8]                  ;

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
            val_col = atoi (argv[++count]) ;
            if (val_col < 1)
               val_col = 1 ;
         }
         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-GMT parameter requires column number") ;
            exit (-1) ;
         }
      }
      else if (strcasecmp (argv[count], "-gmt"  ) == 0)
      {
         gmt_out = TRUE ;
      }
      else if (strcasecmp (argv[count], "-fmt"  ) == 0)
      {
         if (count+1 < argc)
            printf_fmt = ++count ;
         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-FMT parameter requires format string") ;
            exit (-1) ;
         }
      }
      else
      {
         fprintf (stderr, "\nERROR: Invalid parameter [%s] specified\007!\n\n",
                           argv[count]) ;
         exit (-1) ;
      }
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
                                   /* Read first 4 bytes to check binarify   */
   fread  (firstfour, 1, 4, fin) ;

                                   /* If file binarified                     */
   if (bcmp (firstfour, "$P\r\n", 4) == 0)
   {
                                   /* Read in number of columns in data      */
      fread (&num_cols, sizeof (short), 1, fin) ;
      fread (&num_comm, sizeof (short), 1, fin) ;
                                   /* Read in the rest of the header         */
      fread (values_bin, sizeof (double), num_cols-1, fin) ;
                                   /* Read in comment headers                */
      for (count = 1 ; count <= num_comm ; count++)
         fread (buffer, sizeof (double), num_cols, fin) ;

                                   /* Read in first record                   */
      fread (values_bin, sizeof (double), num_cols, fin) ;

      if (gmt_out)
      {
         GMT_FromDay (values_bin [val_col-1]/24.0, &year, &gmt_val) ;
         gmt_val.d -- ;

         printf ("%s\n", Str_FromGMT (&gmt_val)) ;
      }
      else
         if (printf_fmt == 0)
            printf ("%f\n", values_bin [val_col-1]) ;
         else
         {
            printf (argv[printf_fmt], values_bin [val_col-1]) ;
            printf ("\n") ;
         }
   }
   else
   {
      fprintf (stderr, "WARNING:  File not binarified - null outputs\007!\n");
      if (gmt_out)
         printf ("000/00:00:00.000\n") ;
      else
         printf ("0.0\n") ;
   }

   fclose (fin) ;
}
