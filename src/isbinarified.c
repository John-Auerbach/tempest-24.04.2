/*****************************************************************************/
/*                                                                           */
/*   Module:	isbinarified.c                                               */
/*                                                                           */
/*   Purpose:	This program determine if a file is binarified or not and    */
/*		if so, the number of columns of data in the file.            */
/*                                                                           */
/*   Inputs:	Command line options and optionall STDIN.                    */
/*                                                                           */
/*   Outputs:	0 if ASCII, number of columns if binarified.                 */
/*                                                                           */
/*   Uses:	Declarations from: "gmt.h".                                  */
/*                                                                           */
/*   History:	03_Nov_95   Written.                                         */
/*                                                                           */
/*   RCS:	$Id: isbinarified.c,v 1.4 1996/03/14 22:30:12 nestorv Exp $                                                        */
/*                                                                           */
/*		$Log: isbinarified.c,v $
 * Revision 1.4  1996/03/14  22:30:12  nestorv
 * Added provisions for comments.
 *
 * Revision 1.3  1995/11/07  17:36:59  nestorv
 * Fixed inconsistencies.
 *
 * Revision 1.2  1995/11/07  15:56:16  nestorv
 * Can read data from STDIN too.
 *
 * Revision 1.1  1995/11/03  07:53:58  nestorv
 * Initial revision
 *                                                       */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FALSE

#define FALSE 0
#define TRUE  1

#endif

void show_usage ()
{
   fprintf (stderr, "\007\n%s\n\n",
" Usage: isbinarified	-f <input filename>") ;
   fprintf (stderr, "%s\n",
"			This program is used to determine whether or not a");
   fprintf (stderr, "%s\n",
"			given file is binarified or not by looking for a") ;
   fprintf (stderr, "%s\n",
"			$P\\r\\n at the beginning.  If the file is binarified");
   fprintf (stderr, "%s\n",
"			then the number of data columns is returned.  Else") ;
   fprintf (stderr, "%s\n\n",
"			0 is the value returned.") ;
}




void main (int argc, char *argv[])
{
   int     count                           ;
   int     infile_argc  = 0                ;
   FILE   *fin                             ;
   char    firstfour [4]                   ;
   short   num_cols                        ;
   short   num_comm                        ;

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
      else
      {
         fprintf (stderr, "\nERROR: Invalid parameter [%s] specified\007!\n\n",
                           argv[count]) ;
         exit (-1) ;
      }
   }
   
                                   /* Open input file or used STDIN          */
   if (infile_argc == 0)
   {
      fin = stdin ;
   }
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

      printf ("%d\n", num_cols) ;
   }
   else
   {
      printf ("0\n") ;
   }

   fclose (fin) ;
}
