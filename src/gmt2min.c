#include <stdio.h>
#include <stdlib.h>
#include "gmt.h"

void main (int argc, char *argv[])
{
   GMT    gmt1    ;
   double minutes ;

   int curr_year = 0 ;

   if (argc < 2)
   {
      fprintf (stderr, "gmt2min: Needs an argument!\007\n") ;
      exit (-1) ;
   }

   GMT_FromStr (argv [1], &gmt1) ;

   minutes = Day_FromGMT (1900, &gmt1) * 1440.0 ;

   printf ("%f\n", minutes) ;

   exit (0) ;
}
