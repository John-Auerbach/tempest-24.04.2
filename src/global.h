/*****************************************************************************/
/*                                                                           */
/*   Module:    global.h                                                     */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the GLOBAL Module.        */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h".                                */
/*                                                                           */
/*   History:   01_Feb_94 NRV   Rewritten.                                   */
/*                                                                           */
/*****************************************************************************/



#ifdef TEMPEST_MAIN

int    global_sim  = FALSE ;       /* Global sim or a function of time?      */

double g_lat_start =               /* Start latitude for global simulation   */
                     DEG_TO_RAD( -90.0) ;
double g_lat_stop  =               /* Stop  latitude for global simulation   */
                     DEG_TO_RAD(  90.0) ;

double g_lon_start =               /* Start longitude for global simulation  */
                     DEG_TO_RAD(-180.0) ;
double g_lon_stop  =               /* Stop  longitude for global simulation  */
                     DEG_TO_RAD( 180.0) ;

double g_lat_incr  =               /* Increment in latitude  for global sim  */
                     DEG_TO_RAD(   1.0) ;
double g_lon_incr  =               /* Increment in longitude for global sim  */
                     DEG_TO_RAD(   1.0) ;

double global_alt  =   6400.0 ;    /* Altitude for global simulation         */

double g_loc_time  =     -1.0 ;    /* Local time for entire simulation       */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS global_param_list [] =
{
   {"GLOBAL_SIM"    , P_BOOL, &global_sim    ,
    "Perform global simulation (NO/yes)                          "},
   {"LAT_START"     ,  P_DEG, &g_lat_start   ,
    "Starting latitude for global simulation                     "},
   {"LAT_STOP"      ,  P_DEG, &g_lat_stop    ,
    "Stopping latitude for global simulation                     "},
   {"LAT_INCR"      ,  P_DEG, &g_lat_incr    ,
    "Latitude increment for global simulation                    "},
   {"LON_START"     ,  P_DEG, &g_lon_start   ,
    "Starting longitude for global simulation                    "},
   {"LON_STOP"      ,  P_DEG, &g_lon_stop    ,
    "Stopping longitude for global simulation                    "},
   {"LON_INCR"      ,  P_DEG, &g_lon_incr    ,
    "Longitude increment for global simulation                   "},
   {"GLOBAL_ALT"    , P_REAL, &global_alt    ,
    "Altitude for global simulation over mean earth              "},
   {"LOCAL_TIME"    , P_REAL, &g_loc_time    ,
    "Local time for global simulation over mean earth (hours)    "}
} ;

struct OUTPUT_VARS global_outvar_list [] =
{
   {"GLOBAL_NONE"    , 2, " %9.2f"    , "         ", NULL, 1.0,
    "                                                           ",
    "                                ",  P_REAL, NULL}
} ;

#else

extern int    global_sim  ;

extern double g_lat_start ;
extern double g_lat_stop  ;

extern double g_lon_start ;
extern double g_lon_stop  ;

extern double g_lat_incr  ;
extern double g_lon_incr  ;

extern double global_alt  ;
extern double g_loc_time  ;

extern PARAM  global_param_list  [] ;
extern OUTVAR global_outvar_list [] ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if _NO_PROTO_

void init_global_sim     () ;

void perform_global_sim  () ;

#else

void init_global_sim     () ;

void perform_global_sim  () ;

#endif

