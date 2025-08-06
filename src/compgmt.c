#include <stdio.h>
#include <stdlib.h>
#include "gmt.h"

void main (int argc, char *argv[])
{
   GMT gmt1 ;
   GMT gmt2 ;

   int curr_year = 0 ;
   int delta ;

   if (argc < 3)
   {
      fprintf (stderr, "compgmt: Need two arguments!\007\n") ;
      exit (-1) ;
   }

   GMT_FromStr (argv [1], &gmt1) ;
   GMT_FromStr (argv [2], &gmt2) ;

   delta = GMT_Comp (curr_year, &gmt1, curr_year, &gmt2) ;

   printf ("%d\n", delta) ;

   exit (0) ;
}
