/*****************************************************************************/
/*                                                                           */
/*   Module:    tempest.h                                                    */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the TEMPEST progra.       */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "gmt.h".                    */
/*                                                                           */
/*   History:   01_Feb_94 NRV   Rewritten.                                   */
/*                                                                           */
/*   RCS:	$Id: tempest.h,v 1.15 2006/11/09 19:30:18 voronka Exp $                                                        */
/*                                                                           */
/*
 *              $Log: tempest.h,v 
 *              Revision 1.13  2006/04/05 00:25:39  voronk
 *              some stuff change
 *
 *              Revision 1.12  2000/12/19 03:16:38  nestorv
 *              Added input param for show_debug.
 *
 *              Revision 1.11  1997/10/21 19:55:57  nestorv
 *              Edited default title.
 *
 * Revision 1.10  1997/10/21  19:39:40  nestorv
 * Added PLOTL_TITLE to input parameters.
 *
 * Revision 1.9  1997/05/09  13:40:39  nestorv
 * Added NULL array_lengths to OUTPUT_VARS list.
 *
 * Revision 1.8  1996/10/27  22:35:20  nestorv
 * Added traps for the Cntrl-C and kill signal.
 *
 * Revision 1.7  1995/11/01  05:41:33  nestorv
 * Upgraded version numnber to 1.93.
 *
 * Revision 1.6  1995/10/24  21:09:22  nestorv
 * Added #include <time.h> to eliminate compilation errors.
 *
 * Revision 1.5  1995/10/24  20:11:30  nestorv
 * Added code for -REALTIME and -REALSYNC options for TEMPEST realtime
 * operations.
 *
 * Revision 1.4  1995/10/07  22:09:10  nestorv
 * Revised version number to match rcsfreeze number of 1.92.
 *
 * Revision 1.3  1995/10/05  13:08:17  nestorv
 * Added RCS information to header and changed version number to 1.90.
 *                                                       */
/*                                                                           */
/*****************************************************************************/

#include <time.h>

#define DEBUG       1              /* Compile in debugging statements?       */

#define VERSION "1.97"             /* Version number of TEMPEST              */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#if TEMPEST_MAIN

GMT    start_gmt  =                /* Starting GMT for simulation            */
             {218, 0, 0, 0, 0} ;
int    start_year =       1992 ;   /* Starting year for simulation           */

GMT    stop_gmt   =                /* Stop GMT for simulation                */
             {219, 0, 0, 0, 0} ;
int    stop_year  =       1992 ;   /* Stop year for simulation               */

GMT    incr_gmt   =                /* Time increment in GMT format           */
             {  0, 0, 1, 0, 0} ;

int    sub_samp_out   =    1   ;   /* Decimation factor for sub-sampling     */

char   plotl_title [80] =          /* Title string for plotl plots           */
             { "TEMPEST simulation results" } ;

int    met_year = 1900         ;   /* Start year for mission elapsed time    */
GMT    met_gmt  =                  /* Mission elapsed time of simulation GMT */
               {-1, -1, -1, -1, -1} ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double start_time              ;   /* Start time in Greenwich solar days     */
                                   /*  from January  0, 1900                 */
double stop_time               ;   /* Stop time in Greenwich solar days from */
                                   /*  January  0, 1900                      */
double incr_time               ;   /* Increment time in Greenwich solar days */

double met_time                ;   /* MET in Greenwich solar days            */

GMT    curr_gmt                ;   /* Current time in mission in GMT         */
int    curr_year               ;   /* Current year in simulation             */
double curr_time               ;   /* Current time in Greenwich solar days   */
                                   /*  from January  0, 1900                 */
double curr_time_year          ;   /* Current time in Greenwich solar days   */
                                   /* From January 1 of current year         */

double time_years              ;   /* Current time in years AD               */

GMT    time1_gmt               ;   /* Time at t0 + 1 * ts                    */
int    time1_year              ;   /* Time at t0 + 1 * ts                    */

double pc_time                 ;   /* Time in seconds from 00:00 01/01/1970  */

int    prog_interrupt = FALSE  ;   /* Terminate at next time step?           */

time_t next_out_time  = 0      ;   /* Time next output is to be printed      */
time_t delta_out_time = 0      ;   /* Difference between successive outputs  */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern int show_debug ;

struct PARAMETERS tempest_param_list [] =
{
   {"START_TIME"    , P_GMT , &start_gmt     ,
    "Time for start of simulation in GMT format                  "},
   {"START_YEAR"    , P_INT , &start_year    ,
    "Year for start of simulation                                "},
   {"STOP_TIME"     , P_GMT , &stop_gmt      ,
    "Time for end of simulation in GMT format                    "},
   {"STOP_YEAR"     , P_INT , &stop_year     ,
    "Year for end of simulation                                  "},
   {"MET0_TIME"     , P_GMT , &met_gmt       ,
     "Time for the MET time 0 in GMT format                      "},
   {"MET0_YEAR"     , P_INT , &met_year      ,
     "Year for the MET time 0                                    "},
   {"TIME_INCR"     , P_GMT , &incr_gmt      ,
    "Increment time in GMT format (DDD/HH:MM:SS.xxx)             "},
   {"SS_OUTPUT"     , P_INT , &sub_samp_out  ,
    "Subsampling to do for displaying simulation output          "},
   {"SHOW_VARS"     , P_VARS, NULL           ,
    "List of variables to display (NO SPACES, commas only!)      "},
   {"TTP_FILE"      , P_FILE, NULL           ,
    "Time tagged paramter file (first column must be GMT or MET!)"},
   {"PLOTL_TITLE"   , P_STR , plotl_title    ,
    "Title string for plotl plots                                "},
   {"SHOW_DEBUG"    , P_INT , &show_debug    ,
    "Bitfield to enable/disable specified debugging outputs      "}
} ;


struct OUTPUT_VARS tempest_outvar_list [] =
{
   {"GMT"         , 3,    " %16s",  "             GMT", &(curr_gmt), 1.0,
    "Current simulation GMT in ddd/hh:mm:ss.xxx format",
    "GMT", P_GMT, NULL},
   {"YEAR"        , 4,  "  %4d",  " Year", &(curr_year), 1.0,
    "Current simulation year",
    "Year", P_INT, NULL},
   {"GMT_YEARS"   , 5,  " %15.9f",  " Year", &(time_years), 1.0,
    "Time (Years)",
    "Time (Years)", P_REAL, NULL},
   {"GMT_DAYS"    , 5,  " %15.9f",  "     GMT_(days)", &(curr_time_year), 1.0,
    "Current simulation time in Greenwich solar days",
    "GMT (days)", P_TIME, NULL},
   {"GMT_HOURS"   , 5,  " %15.7f",  "    GMT_(hours)", &(curr_time_year), 24.0,
    "Current simulation time in hours   of a Greenwich solar day",
    "GMT (hours)", P_TIME, NULL},
   {"GMT_MINUTES" , 5,  " %15.5f",  "  GMT_(minutes)", &(curr_time_year), 1440.0,
    "Current simulation time in minutes of a Greenwich solar day",
    "GMT (minutes)", P_TIME, NULL},
   {"GMT_SECONDS" , 5,  " %15.3f",  "  GMT_(seconds)", &(curr_time_year), 86400.0,
    "Current simulation time in seconds of a Greenwich solar day",
    "GMT (seconds)", P_TIME, NULL},
   {"MET"         , 3,    " %16s",  "             MET", &(met_gmt), 1.0,
    "Current mission elapsed time in ddd/hh:mm:ss.xxx format",
    "MET", P_GMT, NULL},
   {"MET_YEAR"    , 8,  " %4d",     "   MET_YEAR(AD)", &(met_year), 1.0,
    "Mission elapsed time year",
    "MET YEAR", P_INT, NULL},
   {"MET_YEARS"   , 9,  " %15.9f",  "     MET_(days)", &(met_time), 1.0/365.25,
    "Total Mission Elapsed Time in Greenwich solar years",
    "MET (years)", P_TIME, NULL},
   {"MET_DAYS"    , 5,  " %15.9f",  "     MET_(days)", &(met_time), 1.0,
    "Mission elapsed time in Greenwich solar days",
    "MET (days)", P_TIME, NULL},
   {"MET_HOURS"   , 5,  " %15.7f",  "    MET_(hours)", &(met_time), 24.0,
    "Total Mission elapsed time in hours of a Greenwich solar day",
    "MET (hours)", P_TIME, NULL},
   {"MET_MINUTES" , 5,  " %15.5f",  "  MET_(minutes)", &(met_time), 1440.0,
    "Total Mission elapsed time in minutes of a Greenwich solar day",
    "MET (minutes)", P_TIME, NULL},
   {"MET_SECONDS" , 5,  " %15.3f",  "  MET_(seconds)", &(met_time), 86400.0,
    "Total Mission elapsed time in seconds of a Greenwich solar day",
    "MET (seconds)", P_TIME, NULL},
   {"PC_SECONDS" , 5,  " %15.3f",  " PC_Time_(sec)", &(pc_time), 86400.0,
    "PC Time (seconds since 00:00 01-Jan-1970)",
    "PC Time (seconds)", P_TIME, NULL}
} ;

#else

extern GMT    start_gmt  ;
extern int    start_year ;

extern GMT    stop_gmt   ;
extern int    stop_year  ;

extern GMT    incr_gmt   ;

extern int    sub_samp_out ;

extern char   plotl_title [80] ;

extern int    met_year   ;
extern GMT    met_gmt    ;

extern double start_time ;   
extern double stop_time  ;   
extern double incr_time  ;   
extern double met_time   ;   

extern GMT    curr_gmt   ;
extern int    curr_year  ;
extern double curr_time  ;
extern double curr_time_year ;
extern double time_years ;

extern int    prog_interrupt ;

extern GMT    time1_gmt  ;
extern int    time1_year ; 

extern double pc_time    ;

extern time_t next_out_time  ;
extern time_t delta_out_time ;

extern PARAM  tempest_param_list  [] ;
extern OUTVAR tempest_outvar_list [] ;

#endif

