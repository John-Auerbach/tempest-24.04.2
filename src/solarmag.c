/*****************************************************************************/
/*                                                                           */
/*   Module:    solarmag.h                                                   */
/*                                                                           */
/*   Purpose:	This module predicts of solar and magnetic indices.          */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   12_Mar_94 NRV   Rewritten.                                   */
/*                                                                           */
/*   RCS:       $Id: solarmag.c,v 1.7 2004/11/01 20:00:53 voronka Exp $                                                         */
/*                                                                           */
/*              $Log: solarmag.c,v $
 *              Revision 1.7  2004/11/01 20:00:53  voronka
 *              Fixed reading in of schatten predictions - blank line was causing the system to barf.
 *
 *              Revision 1.6  2000/11/03 01:56:59  nestorv
 *              Added historical data inputs in addition to reading in predictions from file.
 *
 *              Revision 1.5  2000/10/25 19:35:09  nestorv
 *              Added SET_F107_DAILY, SET_F107_3M_AVE, SET_MAG_IND_AP.
 *
 *              Revision 1.4  1999/12/08 03:56:01  nestorv
 *              Added solar constant calculation.
 *
 * Revision 1.3  1995/09/28  22:06:04  nestorv
 * Changed && to & in show_debug conditional.
 *
 * Revision 1.2  1995/09/28  21:46:10  nestorv
 * Added && DEBUG_SOLARMAG to show_debug conditionals and RCS info to header.
 *                                                        */
/*                                                                           */
/*****************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "advmath.h"
#include "earth.h"
#include "orbit.h"

#include "types.h"
#include "gmt.h"
#include "tempest.h"
#include "temputil.h"
#include "solarmag.h"

void init_solarmag_indices    ()
{
   char  filename [200] ;
   FILE *fp ;
   int   linelen   ;
   int   linenum ;
   char  linein [800] ;
   int   p_2sig, m_2sig ;
   int   ym_date, Ap ;

   sprintf (filename, "%s/%s", data_path, "solarmag_schatten.dat") ;
   if ((fp = fopen (filename, "r")) != NULL)
   {
      fprintf (stderr, "Reading in solar activity predicions from \"%s\"\n",
                       filename) ;

      size_schatten_data = 0 ;
      do
      {
         linelen = read_in_line (fp, linein) ;

         if (linein[0] != '#' && linelen > 0)
            size_schatten_data++ ;
      }
      while (!feof (fp)) ;

      schatten_data = (SOLAR_DAT *) 
                        calloc (size_schatten_data, sizeof (SOLAR_DAT)) ;

      rewind (fp) ;
      size_schatten_data = 0 ;
      do
      {
         linelen = read_in_line (fp, linein) ;

         if (linein[0] != '#' && linelen > 0)
         {
            sscanf (linein, "%d %d %d\n", 
                            &(schatten_data[size_schatten_data].month),
                            &(schatten_data[size_schatten_data].year),
                            &(schatten_data[size_schatten_data].nomi_f_mean),
                            &p_2sig, &m_2sig,
                            &(schatten_data[size_schatten_data].nomi_ap)) ;
            size_schatten_data++ ;
         }
      }
      while (!feof (fp)) ;
      fclose (fp) ;
   }
   else
   {
      fprintf (stderr,
         "ERROR: Unable to read solar activity prediction data from %s!\007\n",
         filename) ;
   }

   sprintf (filename, "%s/%s", data_path, "F10.7_monthly.abs") ;
   if ((fp = fopen (filename, "r")) != NULL)
   {
      fprintf (stderr,"Reading in solar activity historical data from \"%s\"\n",
                       filename) ;

      size_historical_data = 0 ;
      do
      {
         linelen = read_in_line (fp, linein) ;

         if (linein[0] != '#' && linelen > 0)
            size_historical_data++ ;
      }
      while (!feof (fp)) ;

      historical_data = (SOLAR_DAT *) 
                        calloc (size_historical_data, sizeof (SOLAR_DAT)) ;

      rewind (fp) ;
      size_historical_data = 0 ;
      do
      {
         linelen = read_in_line (fp, linein) ;

         if (linein[0] != '#' && linelen > 0)
         {
            sscanf (linein, "%d %d %d\n", 
                        &(historical_data[size_historical_data].year),
                        &(historical_data[size_historical_data].month),
                        &(historical_data[size_historical_data].nomi_f_mean)) ;
            size_historical_data++ ;
         }
      }
      while (!feof (fp)) ;
      fclose (fp) ;
   }
   else
   {
      fprintf (stderr,
         "ERROR: Unable to read solar activity prediction data from %s!\007\n",
         filename) ;
   }

   sprintf (filename, "%s/%s", data_path, "Kp_Ap_monthly.dat") ;
   if ((fp = fopen (filename, "r")) != NULL)
   {
      fprintf (stderr,"Reading in historical geomagnetic indices from \"%s\"\n",
                       filename) ;

      linenum = 0 ;
      do
      {
         linelen = read_in_line (fp, linein) ;

         if (linein[0] != '#' && linelen > 0)
         {
            sscanf (linein, "%d %d\n", &ym_date, &Ap) ;
            if ((historical_data[linenum].year  == (ym_date/100)) &&
                (historical_data[linenum].month == (ym_date%100)))
                  historical_data[linenum].nomi_ap = Ap ;
            else
            {
               fprintf (stderr, "ERROR: %s\007\n",
                  "Dates for historical data of F10.7 and Ap do not match!") ;
            }
            linenum++ ;
         }
      }
      while (!feof (fp) && (linenum < size_historical_data)) ;
      fclose (fp) ;
   }
   else
   {
      fprintf (stderr,
         "ERROR: Unable to read solar activity prediction data from %s!\007\n",
         filename) ;
   }

   if ((set_f107_daily<0.0 && (set_f107_3mo_ave>=0.0 || set_mag_ind_ap>=0.0))||
       (set_f107_3mo_ave<0.0 && (set_f107_daily>=0.0 || set_mag_ind_ap>=0.0))||
       (set_mag_ind_ap<0.0 && (set_f107_daily>=0.0 || set_f107_3mo_ave>=0.0)))
      fprintf (stderr, "\n%s\007\n",
               "BAD:  Need to set F10.7 daily, F10.7 3mo ave, and Ap!") ;
}



void compute_solarmag_indices ()
{
   int    curr_month  ;
   int    curr_day    ;
   int    index       ;
   int    pts_to_ave  ;
   int    count       ;
   int    days_inmo   ;

   solar_data_hist = 1 ;

   if (set_f107_daily < 0.0 && set_f107_3mo_ave < 0.0 && set_mag_ind_ap < 0.0)
   {
      md_from_day (curr_gmt.d, curr_year, &curr_month, &curr_day, &days_inmo) ;

      if (curr_year < schatten_data [                   0].year           ||
          curr_year > schatten_data [size_schatten_data-1].year)
      {
         fprintf (stderr, "Out of range in solar prediction indices\n") ;
      }
      else
      {
         index = (curr_month - 1) + 12 * (curr_year - schatten_data[0].year) ;


         f107_daily = (double)  schatten_data [index  ].nomi_f_mean + 
                      (double) (curr_day-1) * 
                      (double) (schatten_data [index+1].nomi_f_mean - 
                                schatten_data [index  ].nomi_f_mean) / 
                      (double)  days_inmo ;
#if DEBUG
if (show_debug & DEBUG_SOLARMAG)
   fprintf (debug_out, "m=%d y=%d index = %d daily = %f\n",
                       curr_month, curr_year, index, f107_daily) ;
#endif

         zurich_ssn_r = RZ_FROM_F107 (f107_daily) ;

         pts_to_ave   =   0 ;
         f107_3mo_ave = 0.0 ;
         for (count = -1 ; count <=1 ; count++)
         {
            if (index+count >=0 && index+count <= size_schatten_data-1)
            {
               f107_3mo_ave += (double)schatten_data [index+count].nomi_f_mean;
               pts_to_ave ++ ;
            }
         }
         f107_3mo_ave /= (double) pts_to_ave ;

         mag_ind_ap = (double)  schatten_data [index  ].nomi_ap + 
                      (double) (curr_day-1) *
                      (double) (schatten_data [index+1].nomi_ap - 
                                schatten_data [index  ].nomi_ap) /
                      (double) days_inmo ;
      }
   }
   else
   {
      f107_daily   = set_f107_daily ;
      f107_3mo_ave = set_f107_3mo_ave ;
      mag_ind_ap   = set_mag_ind_ap ;

      zurich_ssn_r = RZ_FROM_F107 (f107_daily) ;
   }
                                   /* Approximation of observed solar        */
                                   /*   radiation flux in Earth's orbit from */
                                   /*   AIAA "Space Vehicle Design", pg. 128 */
   solar_constant = solar_const_in * (1.0004 + 0.0334 * 
                       cos (curr_time_year-184.0)) ;

}



void md_from_day (int day, int year, int *month, int *dom, int *dim) 
{
   int count, leap_year ;

   leap_year = (year % 4 == 0) && (year %100 != 0) || (year % 400 == 0) ;

   for (count = 1 ; day > days_in_month [leap_year][count] ; count++)
      day -= days_in_month [leap_year][count] ;

   *month = count                             ;
   *dom   = day                               ;
   *dim   = days_in_month [leap_year][count]  ;
}
