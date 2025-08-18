/*****************************************************************************/
/*                                                                           */
/*   Module:    neutdens.h                                                   */
/*                                                                           */
/*   Purpose:	This module interfaces to the MSIS-86 and MSISE90 models     */
/*		that were provided by the NSSDC.                             */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   26_Mar_94 NRV   Rewritten.                                   */
/*                                                                           */
/*   RCS:       $Id: neutdens.c,v 1.4 2005/06/07 00:11:52 voronka Exp nrv $                                                         */
/*                                                                           */
/*              $Log: neutdens.c,v 
 *              Revision 1.3  1997/08/09 21:38:14  nestor
 *              Added debugging in effort to determine drag problem.
 *
 * Revision 1.2  1995/09/28  21:42:17  nestorv
 *  Added && DEBUG_NEUTDENS to show_debug conditionals and added RCS info to
 * header.
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
#include "genorbit.h"
#include "solarmag.h"
#include "neutdens.h"

#include "f2c.h"

integer    yyddd         ;
real       utsec         ;
real       altitude      ;
real       geod_lat      ;
real       geod_long     ;
real       loc_sol_time  ;
real       f107_3ma      ;
real       f107_d        ;
real       daily_ap [7]  ;
integer    msis_mass     ;
real       dens_out [8]  ;
real       temp_out [2]  ;
ftnlen     data_path_len ;

/* Simple in-memory store for NRLMSIS CSV file */
typedef struct {
   double epoch; /* seconds since epoch (UTC) */
   double lat;   /* degrees */
   double lon;   /* degrees */
   double alt;   /* meters */
   double dens[8]; /* He,O,N2,O2,Ar,unused,mass,H,N -> follow dens_out order */
   double temps[2]; /* exo, atalt */
} nrl_row_t;

static nrl_row_t *nrl_table = NULL;
static size_t nrl_table_len = 0;
static int nrl_table_loaded = 0;

/* Parse a CSV line. Very permissive; expects numeric columns in known order. */
static int load_nrl_csv (const char *path)
{
   FILE *f = fopen (path, "r") ;
   char line[2048];
   size_t cap = 0;
   size_t idx = 0;
   if (!f) return 0;
   /* Free previous table */
   if (nrl_table) { free(nrl_table); nrl_table = NULL; nrl_table_len = 0; }

   while (fgets(line, sizeof(line), f))
   {
      /* Skip header lines starting with # or non-digit */
      char *s = line;
      while (*s == ' ' || *s == '\t') s++;
      if (*s == '#' || *s == '\0' || *s == '\n') continue;

      /* columns expected: time_iso, lat, lon, alt_m, he, o, n2, o2, ar, h, n, mass, texo, talt */
      double t_lat, t_lon, t_alt;
      double he,o,n2,o2,ar,h,n,mass,texo,talt;
      char time_iso[64];
      int got = sscanf(line, "%63[^,],%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                       time_iso, &t_lat, &t_lon, &t_alt,
                       &he,&o,&n2,&o2,&ar,&h,&n,&mass,&texo,&talt);
      if (got < 14) continue;

      /* convert time_iso (ISO 8601) to epoch seconds if possible; fallback to 0 */
      double epoch = 0.0;
      struct tm tmv; memset(&tmv,0,sizeof(tmv));
      if (strptime(time_iso, "%Y-%m-%dT%H:%M:%S", &tmv) != NULL) {
          epoch = (double) timegm(&tmv);
      }

      nrl_row_t r;
      r.epoch = epoch;
      r.lat = t_lat;
      r.lon = t_lon;
      r.alt = t_alt;
      r.dens[0] = he;
      r.dens[1] = o;
      r.dens[2] = n2;
      r.dens[3] = o2;
      r.dens[4] = ar;
      r.dens[5] = 0.0; /* placeholder for unused slot to match dens_out */
      r.dens[6] = mass; /* mass placed into dens[6] to map to dens_out[5] later */
      r.dens[7] = h;
      /* note: N placed separately for dens[7] is not perfect mapping, keep n in temps[0] spare */
      r.temps[0] = texo;
      r.temps[1] = talt;

      nrl_table = (nrl_row_t*) realloc (nrl_table, (cap+1) * sizeof(nrl_row_t));
      if (!nrl_table) { fclose(f); return 0; }
      nrl_table[cap] = r;
      cap++;
   }
   fclose(f);
   nrl_table_len = cap;
   nrl_table_loaded = (nrl_table_len>0);
   return nrl_table_loaded;
}

/* Simple nearest-row lookup by time and altitude. If no rows loaded return 0. */
static int lookup_nrl_values (double epoch, double lat, double lon, double alt_m,
                              double out_dens[8], double out_temps[2])
{
   if (!nrl_table_loaded) return 0;
   size_t best = 0;
   double best_score = 1e308;
   for (size_t i=0;i<nrl_table_len;i++)
   {
      double dt = nrl_table[i].epoch - epoch;
      if (nrl_table[i].epoch == 0.0) dt = 0.0; /* unknown times match equally */
      double da = (nrl_table[i].alt - alt_m);
      double score = fabs(dt) + fabs(da)/1000.0; /* simple combined metric */
      if (score < best_score) { best_score = score; best = i; }
   }
   /* map values back to expected dens_out order: dens_out[0]=He, [1]=O, [2]=N2, [3]=O2, [4]=Ar, [5]=mass, [6]=H, [7]=N */
   out_dens[0] = nrl_table[best].dens[0];
   out_dens[1] = nrl_table[best].dens[1];
   out_dens[2] = nrl_table[best].dens[2];
   out_dens[3] = nrl_table[best].dens[3];
   out_dens[4] = nrl_table[best].dens[4];
   out_dens[5] = nrl_table[best].dens[6]; /* mass */
   out_dens[6] = nrl_table[best].dens[7]; /* H */
   out_dens[7] = 0.0; /* N not parsed separately */
   out_temps[0] = nrl_table[best].temps[0];
   out_temps[1] = nrl_table[best].temps[1];
   return 1;
}



void init_neutral_densities    ()
{
   integer meter_true ;

   msis_mass = 48 ;                 /* Compute all densities and mass        */

   meter_true = TRUE_ ;
   meters_ (&meter_true) ;          /* Results from MSIS86  will be in KG&M  */

   meter_true = TRUE_ ;
   meter6_ (&meter_true) ;          /* Results from MSISE90 will be in KG&M  */

   daily_ap [0] = (real) mag_ind_ap ;
   daily_ap [1] = (real) mag_ind_ap ;
   daily_ap [2] = (real) mag_ind_ap ;
   daily_ap [3] = (real) mag_ind_ap ;
   daily_ap [4] = (real) mag_ind_ap ;
   daily_ap [5] = (real) mag_ind_ap ;
   daily_ap [6] = (real) mag_ind_ap ;

   data_path_len = (ftnlen) strlen (data_path) ;

   cum_flux_ao = 0.0 ;
}



void compute_neutral_densities ()
{
   yyddd = (integer) (1000 * (curr_year % 100) + curr_gmt.d) ;

   utsec = (real) GMT_Secs (&curr_gmt) ;

   altitude  = (real) (sat_r_lla.Alt  / 1000.0   ) ;
   geod_lat  = (real) (sat_r_lla.Lat  * R_D_CONST) ;
   if (sat_r_lla.Long < 0.0)
      geod_long = (real) (sat_r_lla.Long * R_D_CONST + 360.0) ;
   else
      geod_long = (real) (sat_r_lla.Long * R_D_CONST) ;

   loc_sol_time = (real) local_time_h            ;
   f107_3ma     = (real) f107_3mo_ave            ;
   f107_d       = (real) f107_daily              ;
#if DEBUG
if (show_debug && DEBUG_NEUTDENS)
{
   fprintf (debug_out, "data_path_len={%s}\n", data_path) ;
   fprintf (debug_out, "yyddd=%d utsec=%f\n", (int) yyddd, utsec) ;
   fprintf (debug_out, "altitude=%f geod_lat=%f geod_long=%f\n",
                        altitude, geod_lat, geod_long) ;
   fprintf (debug_out, "loc_sol_time=%f f107_3ma=%f f107_d=%f\n",
                        loc_sol_time, f107_3ma, f107_d) ;
}
#endif
   if (altitude < 85.0)
   {                                /* Need to use MSISE-90 (0 to 400km)     */
      if (use_nrlmsis) {
         /* load table lazily */
         if (!nrl_table_loaded && strlen(nrlmsis_datafile) > 0) {
            load_nrl_csv (nrlmsis_datafile) ;
            if (nrl_table_loaded) {
               printf("DEBUG: NRLMSIS table loaded with %zu rows\n", nrl_table_len);
            } else {
               printf("DEBUG: Failed to load NRLMSIS table from %s\n", nrlmsis_datafile);
            }
         }
         double epoch_seconds = (double) GMT_Secs (&curr_gmt) ;
         double outd[8], outt[2];
         printf("DEBUG: Looking up NRLMSIS for alt=%.1f, epoch=%.1f\n", altitude, epoch_seconds);
         if (lookup_nrl_values (epoch_seconds, (double) sat_r_lla.Lat * R_D_CONST,
                                (double) sat_r_lla.Long < 0 ? (double)(sat_r_lla.Long*R_D_CONST+360.0) : (double)(sat_r_lla.Long*R_D_CONST),
                                sat_r_lla.Alt, outd, outt))
         {
            for (int i=0;i<8;i++) dens_out[i] = (real) outd[i];
            temp_out[0] = (real) outt[0]; temp_out[1] = (real) outt[1];
            strcpy (neutdens_model, "NRLMSIS") ;
            printf("DEBUG: Using NRLMSIS data\n");
         } else {
            gts6_ (&yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time, 
                   &f107_3ma, &f107_d, daily_ap, &msis_mass,
                   dens_out, temp_out) ;
            strcpy (neutdens_model, neutdens_msise90) ;
            printf("DEBUG: NRLMSIS lookup failed, using MSISE-90\n");
         }
      } else {
         gts6_ (&yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time, 
                &f107_3ma, &f107_d, daily_ap, &msis_mass,
                dens_out, temp_out) ;
         strcpy (neutdens_model, neutdens_msise90) ;
      }
   }
   else if (altitude > 400.0)
   {                                /* Need to use MSIS-86 (85 to 1000km) - but try NRLMSIS first */
      if (use_nrlmsis) {
         /* load table lazily */
         if (!nrl_table_loaded && strlen(nrlmsis_datafile) > 0) {
            load_nrl_csv (nrlmsis_datafile) ;
            if (nrl_table_loaded) {
               printf("DEBUG: NRLMSIS table loaded with %zu rows\n", nrl_table_len);
            } else {
               printf("DEBUG: Failed to load NRLMSIS table from %s\n", nrlmsis_datafile);
            }
         }
         double epoch_seconds = (double) GMT_Secs (&curr_gmt) ;
         double outd[8], outt[2];
         printf("DEBUG: Looking up NRLMSIS for alt=%.1f, epoch=%.1f\n", altitude, epoch_seconds);
         if (lookup_nrl_values (epoch_seconds, (double) sat_r_lla.Lat * R_D_CONST,
                                (double) sat_r_lla.Long < 0 ? (double)(sat_r_lla.Long*R_D_CONST+360.0) : (double)(sat_r_lla.Long*R_D_CONST),
                                sat_r_lla.Alt, outd, outt))
         {
            for (int i=0;i<8;i++) dens_out[i] = (real) outd[i];
            temp_out[0] = (real) outt[0]; temp_out[1] = (real) outt[1];
            strcpy (neutdens_model, "NRLMSIS") ;
            printf("DEBUG: Using NRLMSIS data\n");
         } else {
            gts5_ (data_path, 
                   &yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time,
                   &f107_3ma, &f107_d, daily_ap, &msis_mass,
                   dens_out, temp_out,
                   data_path_len) ; 
            strcpy (neutdens_model, neutdens_msis86) ;
            printf("DEBUG: NRLMSIS lookup failed, using MSIS-86\n");
         }
      } else {
         gts5_ (data_path, 
                &yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time,
                &f107_3ma, &f107_d, daily_ap, &msis_mass,
                dens_out, temp_out,
                data_path_len) ; 
         strcpy (neutdens_model, neutdens_msis86) ;
      }
   }
   else
   {
      if (neutdens_prefer_msise_90)
      {
         if (use_nrlmsis) {
            if (!nrl_table_loaded && strlen(nrlmsis_datafile) > 0) load_nrl_csv(nrlmsis_datafile);
            double epoch_seconds = (double) GMT_Secs (&curr_gmt) ;
            double outd[8], outt[2];
            if (lookup_nrl_values (epoch_seconds, (double) sat_r_lla.Lat * R_D_CONST,
                                   (double) sat_r_lla.Long < 0 ? (double)(sat_r_lla.Long*R_D_CONST+360.0) : (double)(sat_r_lla.Long*R_D_CONST),
                                   sat_r_lla.Alt, outd, outt))
            {
               for (int i=0;i<8;i++) dens_out[i] = (real) outd[i];
               temp_out[0] = (real) outt[0]; temp_out[1] = (real) outt[1];
               strcpy (neutdens_model, "NRLMSIS") ;
            } else {
               gts6_ (&yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time,
                     &f107_3ma, &f107_d, daily_ap, &msis_mass,
                     dens_out, temp_out) ;
               strcpy (neutdens_model, neutdens_msise90) ;
            }
         } else {
            gts6_ (&yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time,
                   &f107_3ma, &f107_d, daily_ap, &msis_mass,
                   dens_out, temp_out) ;
            strcpy (neutdens_model, neutdens_msise90) ;
         }
      }
      else
      {
         gts5_ (data_path,
                &yyddd, &utsec, &altitude, &geod_lat, &geod_long, &loc_sol_time,
                &f107_3ma, &f107_d, daily_ap, &msis_mass,
                dens_out, temp_out,
                data_path_len) ;
         strcpy (neutdens_model, neutdens_msis86) ;
      }
   }

                                    /* Put vals from output array to globals */
   neutdens_numb_he    = (double) dens_out [0] ;
   neutdens_numb_o     = (double) dens_out [1] ;
   neutdens_numb_n2    = (double) dens_out [2] ;
   neutdens_numb_o2    = (double) dens_out [3] ;
   neutdens_numb_ar    = (double) dens_out [4] ;
   neutdens_numb_h     = (double) dens_out [6] ;
   neutdens_numb_n     = (double) dens_out [7] ;

   neutdens_tot_mass   = (double) dens_out [5] ;

   neutdens_temp_exos  = (double) temp_out [0] ;

   neutdens_temp_atalt = (double) temp_out [1] ;

                                    /* Integrate AO density over time to get flux */
   cum_flux_ao += neutdens_numb_o * sat_v_eci_mag * incr_time * (24.0 * 60.0 * 60.0) ;

#if DEBUG
if (show_debug && DEBUG_NEUTDENS)
{
   fprintf (debug_out, "neutdens_numb_he  = %e\n", neutdens_numb_he) ;
   fprintf (debug_out, "neutdens_numb_o   = %e\n", neutdens_numb_o ) ;
   fprintf (debug_out, "neutdens_numb_n2  = %e\n", neutdens_numb_n2) ;
   fprintf (debug_out, "neutdens_numb_o2  = %e\n", neutdens_numb_o2) ;
   fprintf (debug_out, "neutdens_numb_ar  = %e\n", neutdens_numb_ar) ;
   fprintf (debug_out, "neutdens_numb_h   = %e\n", neutdens_numb_h ) ;
   fprintf (debug_out, "neutdens_numb_n   = %e\n", neutdens_numb_n ) ;
   fprintf (debug_out, "neutdens_tot_mass = %e\n", neutdens_tot_mass) ;
}
#endif
}
