#include <stdio.h>
#include <stdlib.h>
#include "gmt.h"

int main (int argc, char *argv[])
{
   int year1 ;
   int year2 ;
   GMT gmt1 ;
   GMT gmt2 ;


   if (argc < 5)
   {
      fprintf (stderr, "diffgmt: Need four arguments!\007\n") ;
      exit (-1) ;
   }

   GMT_FromStr (argv [1], &gmt1) ;
   year1 = atoi (argv[2]) ;
   GMT_FromStr (argv [3], &gmt2) ;
   year2 = atoi (argv[4]) ;

   year1 = year1 - year2 ;

   GMT_Decr (&gmt1, &year1, &gmt2) ;
   
   printf ("%s %d \n", Str_FromGMT (&gmt1), year1) ;

   exit (0) ;
}
