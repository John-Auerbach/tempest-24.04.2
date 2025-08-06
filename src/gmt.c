/*****************************************************************************/
/*                                                                           */
/*   Module:    gmt.h                                                        */
/*                                                                           */
/*   Purpose:  	This file contains routines to do computations, comparisons  */
/*              and conversions with time in GMT format.                     */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   12_Sep_93 NRV   Rewritten.                                   */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "gmt.h"

#define DEBUG 0

/* Returns the day number for year Year, day Day, hour Hour, minute Minute, 
   and seconds Sec. The reference time point is the beginning of January 1,
   1900 (i.e. one day AFTER January 0, 1900). Year should be a number
   between 1901 and 2099, both endpoints included. */


double Day_FromGMT (unsigned int year, GMT *gmt_in) 
{
   double result ;

   result = (year - 1900.0) * 365.0
    +  gmt_in->d 
    +  gmt_in->h  /  24.0 
    +  gmt_in->m  / (24.0 * 60.0) 
    +  gmt_in->s  / (24.0 * 60.0 * 60.0)
    +  gmt_in->ms / (24.0 * 60.0 * 60.0 * 1000.0) ;

    if ( (year >= 1901) && ((year - 1901) / 4 > 0) )
       result += (double) ( (year - 1901) / 4 ) ;

    return (result) ;
}



void GMT_FromDay (double days_in, int *year, GMT *gmt_out)
{
   int whole_days  ;
   int partial_day ;

   whole_days  = (int) (days_in) ;

   *year = whole_days / 365 + 1900 ;
   gmt_out->d = whole_days % 365 - (*year - 1900) / 4 + 1;

   partial_day  = (int) ((days_in - whole_days) 
                          * 24.0 * 60.0 * 60.0 * 1000.0) ;
#if DEBUG
printf ("%f %d %d\n", days_in, whole_days, partial_day) ;
#endif
   gmt_out->h   = partial_day / (60*60*1000) ;
   partial_day %= (60*60*1000) ;
#if DEBUG
printf ("%f %d %d\n", days_in, whole_days, partial_day) ;
#endif

   gmt_out->m   = partial_day / (60*1000) ;
   partial_day %= (60*1000) ;
#if DEBUG
printf ("%f %d %d\n", days_in, whole_days, partial_day) ;
#endif

   gmt_out->s   = partial_day / 1000 ;
   partial_day %= 1000 ;
#if DEBUG
printf ("%f %d %d\n", days_in, whole_days, partial_day) ;
#endif

   gmt_out->ms  = partial_day ;
}




GMT *GMT_Incr    (GMT *gmt_in, int *curr_year, GMT *gmt_incr) 
{
   int carry = 0 ;

   gmt_in->ms += gmt_incr->ms ;
   if (gmt_in->ms >= 1000) 
   {
      gmt_in->ms -= 1000 ;
      carry = 1 ;
   }
   else
      carry = 0 ;

   gmt_in->s  += gmt_incr->s  + carry ;
   if (gmt_in->s  >= 60  ) 
   {
      gmt_in->s -= 60 ;
      carry = 1 ;
   }
   else
      carry = 0 ;

   gmt_in->m  += gmt_incr->m  + carry ;
   if (gmt_in->m  >= 60  ) 
   {
      gmt_in->m  -= 60 ;
      carry = 1 ;
   }
   else
      carry = 0 ;

   gmt_in->h  += gmt_incr->h  + carry ;
   if (gmt_in->h  >= 24  ) 
   {
      gmt_in->h  -= 24 ;
      carry = 1 ;
   }
   else
      carry = 0 ;

   gmt_in->d  += gmt_incr->d  + carry ;
   if (gmt_in->d  >= 366 ) 
   {
      gmt_in->d = 1;
      carry = 1 ;
   }
   else
      carry = 0 ;

   *curr_year += carry ;

   return (gmt_in) ;
}



GMT *GMT_Decr    (GMT *gmt_in, int *curr_year, GMT *gmt_decr) 
{
   int carry = 0 ;

   gmt_in->ms -= gmt_decr->ms ;
   if (gmt_in->ms < 0) 
   {
      gmt_in->ms += 1000 ;
      carry = 1 ;
   }
   else
      carry = 0 ;

   gmt_in->s  -= (gmt_decr->s  + carry) ;
   if (gmt_in->s  < 0) 
   {
      gmt_in->s += 60 ;
      carry = 1 ;
   }
   else
      carry = 0 ;

   gmt_in->m  -= (gmt_decr->m  + carry) ;
   if (gmt_in->m  < 0)
   {
      gmt_in->m  += 60 ;
      carry = 1 ;
   }
   else
      carry = 0 ;

   gmt_in->h  -= (gmt_decr->h  + carry) ;
   if (gmt_in->h  < 0) 
   {
      gmt_in->h  += 24 ;
      carry = 1 ;
   }
   else
      carry = 0 ;

   gmt_in->d  -= (gmt_decr->d  + carry) ;
   if (gmt_in->d  < 0)
   {
      gmt_in->d  += 365 ;
      carry = 1 ;
   }
   else
      carry = 0 ;

   *curr_year -= carry ;

   return (gmt_in) ;
}



char *Str_FromGMT (GMT *gmt_in) 
{
   static char out_str [17] ;

   if (gmt_in != (GMT *) NULL)
      sprintf(out_str,"%03d/%02d:%02d:%02d.%03d",gmt_in->d,
                       gmt_in->h, gmt_in->m, gmt_in->s, gmt_in->ms );
   else
      sprintf(out_str, "000/00:00:00.000") ;

   return (out_str) ;
}



void GMT_FromStr (char text_in[], GMT *gmt_out)
{
   sscanf (text_in, "%3d/%2d:%2d:%2d.%3d", &(gmt_out->d), &(gmt_out->h),
                                           &(gmt_out->m), &(gmt_out->s),
                                           &(gmt_out->ms)) ;

   if (strlen (text_in) < 16)
      gmt_out->ms = 0 ;
}


#define SGN(a)		((a>0) ? 1 : -1)

int GMT_Comp (int year1, GMT *gmt1, int year2, GMT *gmt2)
{
   int    delta_t ;               /* Time difference                        */

   if ((delta_t = year1 - year2) == 0)
      if ((delta_t = gmt1->d - gmt2->d) == 0)
         if ((delta_t = gmt1->h - gmt2->h) == 0)
            if ((delta_t = gmt1->m - gmt2->m) == 0)
               if ((delta_t = gmt1->s - gmt2->s) == 0)
                  if ((delta_t = gmt1->ms - gmt2->ms) == 0)
                     return (0) ;
                  else
                     return (SGN(delta_t)) ;
               else
                  return (SGN(delta_t)) ;
            else
               return (SGN(delta_t)) ;
         else
            return (SGN(delta_t)) ;
      else
         return (SGN(delta_t)) ;
   else
      return (SGN(delta_t)) ;
}



double GMT_Secs (GMT *gmt)
{
   double secs ;

   secs = gmt->ms / 1000.0 + gmt->s           +
          gmt->m  *   60.0 + gmt->h  * 3600.0 ;

   return (secs) ;
}



void   GMT_MonthDay (GMT *gmt, int year, int *month, int *day)
{
   int yearday ;
   int leap    ;
   int count   ;
   int daytable [2] [13] = {
       {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
       {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} } ;

   yearday = gmt->d ; 
   leap = (year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0) ;

   for (count = 1 ; yearday > daytable [leap][count] ; count++)
      yearday -= daytable [leap] [count] ;

   *month = count   ;
   *day   = yearday ;
}
