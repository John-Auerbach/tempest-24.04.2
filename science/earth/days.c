/* DATE CONVERSION.

   Apostolos Lerios - TOLIS@NOVA. */


#include <math.h>
#include "advmath.h"

#include "earth.h"


/* Returns the day number for year Year, month Month, day Day, and
   seconds Sec. The reference time point is the beginning of January 1,
   1900 (i.e. one day AFTER January 0, 1900). Year should be a number
   between 1901 and 2099, both endpoints included. */

double DAY_FromYMDS(Year,Month,Day,Sec)
unsigned int  Year;
unsigned int  Month;
unsigned int  Day;
unsigned long Sec;
{
  static int MonthDays[]={0,31,59,90,120,151,181,212,243,273,304,334};

  double Result;

  Result=(Year-1900.0)*365.0+(Year-1901)/4+
    MonthDays[Month-1]+(Day-1)+Sec/(24.0*60.0*60.0);
  if (!(Year%4) && Month>2)
    Result++;
  return Result;
}
