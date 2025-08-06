/*****************************************************************************/
/*                                                                           */
/*   Module:	unbinarify.c                                                 */
/*                                                                           */
/*   Purpose:	This program determine if a file is binarified or not and    */
/*		if so, prints out the data in text format.                   */
/*                                                                           */
/*   Inputs:	Command line options and optionall STDIN.                    */
/*                                                                           */
/*   Outputs:	none if ASCII, data if binarified.                           */
/*                                                                           */
/*   Uses:	Declarations from: "gmt.h".                                  */
/*                                                                           */
/*   History:	03_Nov_95   Written.                                         */
/*                                                                           */
/*   RCS:	$Id: unbinarify.c,v 1.2 1996/03/14 22:54:54 nestorv Exp nestorv $                                                        */
/*                                                                           */
/*		$Log: unbinarify.c,v $
 *		Revision 1.2  1996/03/14 22:54:54  nestorv
 *		Added code to deal with comments.
 *
 * Revision 1.1  1995/11/07  17:38:53  nestorv
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
" Usage: unbinarify	-f <input filename> -float -gmt <column number>") ;
   fprintf (stderr, "%s\n\n",
"			-nocomment -commas -flip") ;
   fprintf (stderr, "%s\n",
"			This program is used to print in ASCII the data from");
   fprintf (stderr, "%s\n",
"			a binarified file (which has $P\\r\\n at the beginning.)") ;
   fprintf (stderr, "%s\n",
"			The default output format is %12.5e or %15.5f with");
   fprintf (stderr, "%s\n",
"			the -float option.  The comments will be printed as") ;
   fprintf (stderr, "%s\n\n",
"			well unless the -nocomment options is used.") ;
   fprintf (stderr, "%s\n",
"			The output is in fixed columns by default but can be");
   fprintf (stderr, "%s\n\n",
"			comma delimited with the -commas option.") ;
   fprintf (stderr, "%s\n\n",
"			The -flip option flips the endian of the binary numbers.") ;
}




double d_flipendian (double din)
{
   int     i   ;
   int     size ;
   double  dout ;
   char   *p_di, *p_do ;

   size = sizeof (double) ;
   p_di = (char *) &din ;
   p_do = (char *) &dout ;

   for (i = 0 ; i < size ; i++)
      *(p_do+i) = *(p_di+size-1-i) ;

   return (dout) ;
}



short s_flipendian (short sin)
{
   int     i   ;
   int     size ;
   short   sout ;
   char   *p_si, *p_so ;

   size = sizeof (short) ;
   p_si = (char *) &sin ;
   p_so = (char *) &sout ;

   for (i = 0 ; i < size ; i++)
      *(p_so+i) = *(p_si+size-1-i) ;

   return (sout) ;
}



int main (int argc, char *argv[])
{
   int     count                           ;
   int     infile_argc  = 0                ;
   FILE   *fin                             ;
   char    firstfour [4]                   ;
   short   num_cols                        ;
   short   num_comm                        ;
   int     is_gmt [MAX_COLS]               ;
   int     floatout              = FALSE   ;
   int     showcomment           = TRUE    ;
   int     use_commas            = FALSE   ;
   int     flip_endian           = FALSE   ;
   double  values_bin [MAX_COLS]           ;
   int     year                            ;
   GMT     gmt_val                         ;
   int     column                          ;
   char    buffer [MAX_COLS*8]             ;
   char    fmt_string [20]                 ;

   for (count = 0 ; count < MAX_COLS ; count++)
      is_gmt [count] = FALSE ;

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
      else if (strcasecmp (argv[count], "-nocomment") == 0)
         showcomment = FALSE ;
      else if (strcasecmp (argv[count], "-commas") == 0)
         use_commas = TRUE ;
      else if (strcasecmp (argv[count], "-float") == 0)
         floatout = TRUE ;
      else if (strcasecmp (argv[count], "-flip") == 0)
         flip_endian = TRUE ;
      else if (strcasecmp (argv[count], "-gmt"  ) == 0)
      {
         if (count+1 < argc)
         {
            column = atoi (argv[++count]) ;
            if ((column > 0) && (column < MAX_COLS))
               is_gmt [column-1] = TRUE ;
            else
            {
               fprintf(stderr,"\nERROR: %s\007!\n\n",
                  "Column number not in range (1<=column<=50)") ;
            }
         }
         else
         {
            fprintf(stderr,"\nERROR: %s\007!\n\n",
              "-GMT parameter requires column number") ;
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

      if (flip_endian)
      {
         num_cols = s_flipendian (num_cols) ;
         num_comm = s_flipendian (num_comm) ;
      }
                                   /* Read in the rest of the header         */
      fread (values_bin, sizeof (double), num_cols-1, fin) ;

      sprintf (fmt_string, "%%%ds", num_cols*8) ;
                                   /* Read in comment headers                */
      for (count = 1 ; count <= num_comm ; count++)
      {
         fread (buffer, sizeof (double), num_cols, fin) ;

         if (showcomment)
            printf (fmt_string, buffer) ;
      }

      do
      {
         if (fread (values_bin, sizeof(double), num_cols, fin) == num_cols)
         {
            if (flip_endian)
            {
               for (count = 0 ; count < num_cols ; count++)
                  values_bin [count] = d_flipendian (values_bin [count]) ;
            }

            for (count = 0 ; count < num_cols ; count++)
            {
               if (is_gmt [count])
               {
                  GMT_FromDay (values_bin [count]/24.0, &year, &gmt_val) ;
                  gmt_val.d -- ;

                  if (!use_commas || (use_commas && (count == num_cols-1)))
                     printf ("%s ", Str_FromGMT (&gmt_val)) ;
                  else
                     printf ("%s, ", Str_FromGMT (&gmt_val)) ;
                 
               }
               else
               {
                  if (!use_commas || (use_commas && (count == num_cols-1)))
                  {
                     if (floatout)
                        printf ("%15.5f ", values_bin [count]) ;
                     else
                        printf ("%12.5e ", values_bin [count]) ;
                  }
                  else
                  {
                     if (floatout)
                        printf ("%15.5f, ", values_bin [count]) ;
                     else
                        printf ("%12.5e, ", values_bin [count]) ;
                  }
               }
            }
            printf ("\n") ;
         }
      }
      while (!feof (fin)) ; 

   }

   fclose (fin) ;

   exit (0) ;
}
