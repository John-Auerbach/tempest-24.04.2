#include <stdio.h>
#include <stdlib.h>
#include "gmt.h"

void main (int argc, char *argv[])
{
   double days1 ;
   double days2 ;
   GMT gmt1 ;
   GMT gmt2 ;

   int year = 1900 ;

   if (argc < 3)
   {
      fprintf (stderr, "diffgmt: Need two arguments!\007\n") ;
      exit (-1) ;
   }

   GMT_FromStr (argv [1], &gmt1) ;
   GMT_FromStr (argv [2], &gmt2) ;

   GMT_Decr (&gmt1, &year, &gmt2) ;
   
   printf ("%s\n", Str_FromGMT (&gmt1)) ;

   exit (0) ;
}
