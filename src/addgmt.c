#include <stdio.h>
#include <stdlib.h>
#include "gmt.h"

void main (int argc, char *argv[])
{
   GMT gmt1 ;
   GMT gmt2 ;

   int curr_year = 0 ;

   if (argc < 3)
   {
      fprintf (stderr, "addgmt: Need two arguments!\007\n") ;
      exit (-1) ;
   }

   GMT_FromStr (argv [1], &gmt1) ;
   GMT_FromStr (argv [2], &gmt2) ;

   GMT_Incr (&gmt2, &curr_year, &gmt1) ;

   printf ("%s\n", Str_FromGMT (&gmt2)) ;

   exit (0) ;
}
