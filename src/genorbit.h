
/*****************************************************************************/
/*                                                                           */
/*   Module:    genorbit.h                                                   */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the GENORBIT Module.      */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   01_Feb_94 NRV   Rewritten.                                   */
/*                                                                           */
/*   RCS:	$Id: genorbit.h,v 1.16 2006/06/16 21:37:55 voronka Exp voronka $                                                        */
/*                                                                           */
/*              $Log: genorbit.h,v 
 *              Revision 1.13  1999/12/08 03:55:21  nestor
 *              Added solar angle computation.
 *
 * Revision 1.12  1997/08/20  22:52:41  nestorv
 * Added ZERO_OUT variable.
 *
 * Revision 1.11  1997/08/17  21:10:27  nestorv
 * Added ATM_DRAG_FORCE variables.
 *
 * Revision 1.10  1997/08/09  22:16:23  nestorv
 * Added bare tether thrust forces to RK propagation algorithm.
 *
 * Revision 1.9  1997/05/09  13:32:57  nestorv
 *  Added NULL array_lengths to OUTPUT_VARS list.
 *
 * Revision 1.8  1996/10/27  23:05:26  nestorv
 * Added parameter to recompute ephemeri in precision orbit.
 *
 * Revision 1.7  1996/10/27  00:09:21  nestorv
 * Added RK4(5) propagator.
 *
 * Revision 1.6  1996/02/10  17:55:24  nestorv
 * Corrected inclination and removed BU values.
 *
 * Revision 1.5  1996/02/08  21:01:48  nestorv
 * Added inclination to output list.
 *
 * Revision 1.4  1995/10/02  23:05:38  nestorv
 * Changed RCS Log entry to be correct.
 *                                                       */
/*                                                                           */
/*****************************************************************************/



                                   /* Anomaly type enumeration               */
enum {TRUE_ANOMALY, MEAN_ANOMALY} ;

#ifdef TEMPEST_MAIN

int    ephem_year  =     1992 ;    /* Year at which ephemeri are defined     */
GMT    ephem_gmt   =               /* Time at which ephemeri are defined     */
            {218, 0, 0, 0, 0} ;

double r_circular_alt = -1    ;    /* Circular orbital altitude (meters)     */
double sun_sync_ltan  = -1    ;    /* Local time of the ascending node for   */
                                   /*  sun synchronous orbit (hours)         */

double r_apogee    = MEAN_R +      /* Radius of orbit at apogee  (meters)    */
                     300000.0 ;
double r_perigee   = MEAN_R +      /* Radius of orbit at perigee (meters)    */
                     300000.0 ;

double inclination =               /* Angle between the orbital and the      */
             DEG_TO_RAD(28.5) ;    /*  equatorial plane (radians)            */
double raan             = 0.0 ;    /* Angle between vernal equinox and the   */
                                   /*  ascending node (where the satellite   */
                                   /*  crosses equatorial plane when moving  */
                                   /*  from south to north                   */
double arg_perigee      = 0.0 ;    /* Angle between ascending node & e (rad) */

double true_anomaly     = 0.0 ;    /* Angle between eccentricity vector &    */
                                   /*  satellite position vector measured in */
                                   /*  direction of satellite motion (rad)   */
double mean_anomaly     = 0.0 ;    /* Relates position of the satellite in   */
                                   /*  the orbit to elapsed time (radians)   */
char   anomaly_str [10] =          /* Anomaly type (TRUE or MEAN)            */
                     {"TRUE"} ;

int    anomaly_type    =           /* Anomaly type from enum structure -     */
                 TRUE_ANOMALY ;    /*   either TRUE_ANOMALY or MEAN_ANOMALY  */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int    orbit_perturb   = FALSE ;   /* Boolean indicating whether or not to   */
                                   /*  perturb orbit with the gravitational  */
                                   /*  effects of the sun, moon, and the     */
                                   /*  earths oblateness (J2)                */
int    orbit_decay     = FALSE ;   /* Boolean indicating whether or not to   */
                                   /*  perturb orbit with atmospheric drag   */

double bal_coeff       = 100.0 ;   /* Ballistic coefficient used for the     */
                                   /*  decay due to atmospheric drag model   */
                                   /*  (mass[kg]/(drag_coeff*area[m^2])      */

double system_mass     = 200.0 ;   /* Mass of entire tethered system         */

int    orbit_precise   = FALSE ;   /* When true, compute orbital trajectory  */
                                   /*   using Runge-Kutta integrator         */

int    ephem_from_rv   = FALSE ;   /* When true, compute ephemeri from the   */
                                   /*   R&V generated by RK4(5)              */

Cartesian thrust_lvlh           ;  /* Manual Thrust Force (Newtons)          */

double    gs_lat = -1.0  ;         /* Latitude of Ground Station             */
double    gs_lon = -1.0  ;         /* Longitude of Ground Station            */
double    gs_elev_mask = 0.0 ;     /* Elevation Mask for Satellite View      */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double fast_in_time0_pc   = 0.0   ;
double fast_in_period     = 0.0   ;
double fast_in_delta_per  = 0.0   ;
double fast_in_delta_raan = 0.0   ;
double fast_in_inclin     = 0.0   ;
double fast_in_raan0      = 0.0   ;
double fast_in_phi0       = 0.0   ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double semi_major_axis         ;   /* Semi-major axis of orbit   (meters)    */
double eccentricity            ;   /* Length of eccentricity vector (points  */
                                   /*  from center of earth to perigee)      */

double alt_apogee              ;   /* Apogee  altitude over mean earth       */
double alt_perigee             ;   /* Perigee altitude over mean earth       */

double mean_motion             ;   /* Average angular velocity (rad/sec)     */

double orbit_period            ;   /* Period for one orbit (seconds)         */ 

double curr_true_anomaly       ;   /* True anomaly for computing current     */
                                   /*   satellite R and V                    */
double curr_mean_anomaly       ;   /* Mean anomaly for computing current     */
                                   /*   satellite R and V                    */

int    orbit_decayed = FALSE   ;   /* Has orbit decayed due to atmospheric   */
                                   /*   drag? (sat_r_lla.Alt <= 0.0)         */

double d_raan                  ;   /* Precession of RAAN in deg/day          */
double d_arg_perigee           ;   /* Precession of Arg Perigee in deg/day   */

double ephem_time              ;   /* Ephemeris time in Greenwich solar days */
                                   /*  from January  0, 1900                 */

Cartesian oblate_earth   =         /* Acceleration due to nonspherical earth */
               {0.0, 0.0, 0.0} ;
Cartesian solar_gravity  =         /* Acceleration due to solar gravity      */
               {0.0, 0.0, 0.0} ;
Cartesian lunar_gravity  =         /* Acceleration doe to lunar gravity      */
               {0.0, 0.0, 0.0} ;
Cartesian solar_pressure =         /* Acceleration due to solar pressure     */
               {0.0, 0.0, 0.0} ;
Cartesian atmos_drag     =         /* Acceleration due to atmospheric drag   */
               {0.0, 0.0, 0.0} ;
Cartesian ilxb_accel     =         /* Acceleration due to iLxB force         */
               {0.0, 0.0, 0.0} ;
Cartesian b_ilxb_accel   =         /* Acceleration due to bare iLxB force    */
               {0.0, 0.0, 0.0} ;
Cartesian thrust_accel   =         /* Acceleration due to 'manual' force     */
               {0.0, 0.0, 0.0} ;

Cartesian total_perturb        ;   /* Sum of accelerations due to perturbs   */

Cartesian atmos_drag_force     ;   /* Force due to atmospheric drag (N)      */

double    atmos_drag_force_m   ;   /* Magnitude of atmospherid drag force (N)*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

Cartesian sat_v_eci      ;         /* Satellite's velocity in ECI coordinates*/
double    sat_v_eci_mag  ;         /* Magnitude of satellite's velocity      */
Cartesian sat_r_eci      ;         /* Satellite's position in ECI coordinates*/
double    sat_r_eci_mag  ;         /* Magnitude of satellite's radius vector */
Earth     sat_r_lla      ;         /* Satellite's position in Lat, Long coord*/
double    altitude_mer   ;         /* Altitude above IGRF90 mean earth rad   */
double    sat_ra         ;         /* Satellites RA in ECI                   */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

Cartesian sun_r_eci      ;         /* Sun's position in ECI coordinates      */

Earth     sun_r_lla      ;         /* Sun's position in Earth coordinates    */

Cartesian sun_pos_lvlh   ;         /* Sun's position in LVLH coordinates     */

double    local_time_h   ;         /* Satellite's local time in hours        */

double    solar_angle    ;         /* Angle of sun in degrees from vertical  */
double    solar_az       ;
double    solar_el       ;

int       sat_in_shadow  ;         /* True if satellite is in Earth's shadow */

double    zero_out       ;         /* Dummy output variable = 0.0            */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double fast_out_delta_t ;
double fast_out_period ;
double fast_out_ra ;
double fast_out_dec ;
double fast_out_lat ;
double fast_out_long ;
double fast_out_dlat ;
double fast_out_dlong ;
double fast_out_gs_sat_elev ;
int    fast_out_gs_sat_in_view = FALSE ;;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double gs_sat_elev = 0.0   ;
int    gs_sat_in_view   = FALSE ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
struct PARAMETERS genorbit_param_list [] =
{
   {"EPHEMERIS_YEAR", P_INT , &ephem_year    ,
    "Year at which ephemeri are being given                      "},
   {"EPHEMERIS_TIME", P_GMT , &ephem_gmt     ,  
    "Time at which ephemeri are being given                      "},
   {"CIRCULAR_ALT"  , P_ALT , &r_circular_alt,
    "Initial circular altitude above mean earth radis (m)        "},
   {"APOGEE_ALT"    , P_ALT , &r_apogee      ,
    "Apogee  altitude above mean earth radius (m)                "},
   {"PERIGEE_ALT"   , P_ALT , &r_perigee     ,
    "Perigee altitude above mean earth radius (m)                "},
   {"INCLINATION"   , P_DEG , &inclination   ,
    "Inclination of orbital plane (degrees)                      "},
   {"SUN_SYNC_LTAN" , P_REAL , &sun_sync_ltan,
    "Local time of the ascending node for a sun sync orbit(hours)"},
   {"RAAN"          , P_DEG , &raan          ,
    "Right ascension of Ascending Node (degrees)                 "},
   {"ARG_PERIGEE"   , P_DEG , &arg_perigee   ,
    "Argument of perigee (degrees)                               "},
   {"TRUE_ANOMALY"  , P_DEG , &true_anomaly  ,
    "True anomaly (degrees)                                      "},
   {"MEAN_ANOMALY"  , P_DEG , &mean_anomaly  ,
    "Mean anomaly (degrees)                                      "},
   {"ANOMALY_TYPE"  , P_STR ,  anomaly_str   ,
    "Which anomaly type initially specified (TRUE/MEAN)          "},
   {"ORBIT_PERTURB" , P_BOOL, &orbit_perturb ,
    "Include perturbations due to the sun, moon, and oblateness  "},
   {"ORBIT_DECAY"   , P_BOOL, &orbit_decay   ,
    "Include orbital perturbations due to atmospheric drag       "},
   {"BAL_COEFF"     , P_REAL, &bal_coeff     ,
    "Ballistic Coefficient (mass[kg]/(drag_coefficient*area[m^2])"},
   {"SYSTEM_MASS"   , P_REAL, &system_mass   ,
    "Mass of entire tethered system (kg)                         "},
   {"ORBIT_PRECISE" , P_BOOL, &orbit_precise ,
    "Compute orbital trajectory using Runge-Kutta 4(5) propagator"},
   {"EPHEM_FROM_RV" , P_BOOL, &ephem_from_rv ,
    "Compute ephemeri from R&V during RK4(5) propagation (def F) "},
   {"THRUST_X"      , P_REAL, &thrust_lvlh.X,
    "X component of thrust force vector to be applied (Newtons)  "},
   {"THRUST_Y"      , P_REAL, &thrust_lvlh.Y,
    "Y component of thrust force vector to be applied (Newtons)  "},
   {"THRUST_Z"      , P_REAL, &thrust_lvlh.Z,
    "Z component of thrust force vector to be applied (Newtons)  "},
   {"FAST_TIME0"      , P_REAL, &fast_in_time0_pc,
    "Fast propagator time 0 in PC time (seconds)"},
   {"FAST_PERIOD"     , P_REAL, &fast_in_period,
    "Fast propagator orbital period (seconds)"},
   {"FAST_DELTA_PER"     , P_REAL,&fast_in_delta_per,
    "Chang in fast propagators orbital period (sec/sec)"},
   {"FAST_DRAAN"      , P_REAL, &fast_in_delta_raan,
    "Fast propagator delta raan (rad/sec)"},
   {"FAST_INCLIN"     , P_REAL, &fast_in_inclin,
    "Fast propagator inclination (rad/sec)"},
   {"FAST_RAAN0"     , P_REAL, &fast_in_raan0,
    "Fast propagator RAAN 0 (rad)"},
   {"FAST_PHI0"     , P_REAL, &fast_in_phi0,
    "Fast propagator Phi 0 (rad)"},
   {"GS_LAT"        , P_DEG , &gs_lat,
    "Latitude of Ground Station (deg)"},
   {"GS_LONG"       , P_DEG , &gs_lon,
    "Longitude of Ground Station (deg)"},
   {"GS_ELEV_MASK"  , P_DEG , &gs_elev_mask,
    "Elevation Mask for Satellite to be in View of GS"}
} ;

struct OUTPUT_VARS genorbit_outvar_list [] =
{
   {"RX_ECI"         , 2, " %10.3f"    , "   Rx_(km)", &(sat_r_eci.X), 0.001,
    "X component of the satellite's Position in ECI (km)",
    "R|-x|= ECI (km)", P_REAL, NULL},
   {"RY_ECI"         , 2, " %10.3f"    , "   Ry_(km)", &(sat_r_eci.Y), 0.001,
    "Y component of the satellite's Position in ECI (km)",
    "R|-y|= ECI (km)", P_REAL, NULL},
   {"RZ_ECI"         , 2, " %10.3f"    , "   Rz_(km)", &(sat_r_eci.Z), 0.001,
    "Z component of the satellite's Position in ECI (km)",
    "R|-z|= ECI (km)", P_REAL, NULL},
   {"R_ECI"          , 1, " %10.3f"    , "   R__(km)", &(sat_r_eci_mag), 0.001,
    "Magnitude of the satellite's position vector |R| in ECI (km)",
    "||R|| ECI (km)", P_REAL, NULL},
   {"VX_ECI"         , 2, " %10.3f"    , "  Vx_(m/s)", &(sat_v_eci.X), 1.0,
    "X component of the satellite's velocity in ECI (m/s)",
    "V|-x|= ECI (m/s)", P_REAL, NULL},
   {"VY_ECI"         , 2, " %10.3f"    , "  Vy_(m/s)", &(sat_v_eci.Y), 1.0,
    "Y component of the satellite's velocity in ECI (m/s)",
    "V|-y|= ECI (m/s)", P_REAL, NULL},
   {"VZ_ECI"         , 2, " %10.3f"    , "  Vz_(m/s)", &(sat_v_eci.Z), 1.0,
    "Z component of the satellite's velocity in ECI (m/s)",
    "V|-z|= ECI (m/s)", P_REAL, NULL},
   {"V_ECI"          , 1, " %10.3f"    , "  V__(m/s)", &(sat_v_eci_mag), 1.0,
    "Magnitude of the satellite's velocity vector |V| in ECI (m/s)",
    "||V|| ECI (m/s)", P_REAL, NULL},
   {"LATITUDE"     , 3,  " %9.5f",  " Latitude", &(sat_r_lla.Lat), R_D_CONST,
    "Latitude  of current ground track position (deg +N)",
    "Geodetic Latitude (+N)", P_REAL, NULL},
   {"LONGITUDE"    , 4, " %10.5f", " Longitude", &(sat_r_lla.Long), R_D_CONST,
    "Longitude of current ground track position (deg +E)",
    "Geodetic Longitude (+E)", P_REAL, NULL},
   {"ALTITUDE_J2"  , 3, "  %11.4f", " Alt_J2_(km)", &(sat_r_lla.Alt), 0.001,
    "Altitude of satellite above oblate spheroidal earth (J2) (km)",
    "Altitude J2 (km)", P_REAL, NULL},
   {"ALT_MER"      , 5, "  %11.4f", " Alt_mer(km)",  &(altitude_mer), .001,
    "Altitude of satellite above IGRF90 mean earth radius (km)",
    "Altitude MER (km)", P_REAL, NULL},
   {"SAT_RA"      , 6, "  %11.4f", " Sat_RA (deg)",  &(sat_ra), R_D_CONST,
    "RA of Satellite in ECI (deg)", 
    "Satellite RA (deg)", P_REAL, NULL},
   {"RAAN"         , 4, " %13.5f"   , "   RAAN_(deg)",&(raan), R_D_CONST,
    "Right ascension of Ascending Node (degrees)         ",
    "RAAN (deg)", P_REAL, NULL},
   {"ARG_PERIGEE"  , 5, " %13.5f"   , "  Arg_Perigee",&(arg_perigee), R_D_CONST,
    "Argument of perigee (degrees)                       ",
    "Arg Perigee (deg)", P_REAL, NULL},
   {"ECCENTRICITY" , 3, " %13.9f"   , " Eccentricity",&(eccentricity), 1.0,
    "Instantaneous eccentricity of orbit                 ",
    "Eccentricity", P_REAL, NULL},
   {"INCLINATION"  , 6, " %13.7f"   , " Inclin_(deg)",&(inclination), R_D_CONST,
    "Inclination of orbital plane (degrees)",
    "Inclination (deg)", P_REAL, NULL},
   {"SEMI_MAJ_AXIS", 7, " %14.4f"   ," SemiMajorAxis",&(semi_major_axis),0.001,
    "Length of Semi-major axis (kilometers)              ",
    "Semi-major Axis (km)", P_REAL, NULL},
   {"TRUE_ANOMALY ", 7," %12.7f" ,"  TrueAnomly",&(curr_true_anomaly),R_D_CONST,
    "Current True Anomaly (deg)                          ",
    "True Anomaly (deg)", P_REAL, NULL},
   {"MEAN_ANOMALY ", 7," %12.7f" ,"  MeanAnomly",&(curr_mean_anomaly),R_D_CONST,
    "Current Mean Anomaly (deg)                          ",
    "Mean Anomaly (deg)", P_REAL, NULL},
   {"APOGEE_ALT"   , 4, " %13.4f"   ,"  Apogee_(km)", &( alt_apogee), 0.001,
    "Apogee  altitude above mean earth radius (m)        ",
    "Apogee Alt (km)", P_REAL, NULL},
   {"PERIGEE_ALT"  , 4, " %13.4f"   ," Perigee_(km)", &(alt_perigee), 0.001,
    "Perigee altitude above mean earth radius (m)        ",
    "Perigee Alt (km)", P_REAL, NULL},
   {"MEAN_MOTION"  , 7, " %14.9f"   ," MeanMo(deg/s)", &(mean_motion), R_D_CONST,
    "Mean Motion - average angular velocity (deg/sec)",
    "Mean Motion (deg/s)", P_REAL, NULL},
   {"ORB_PER_SEC"  , 9, " %12.7f"   ," OrbPer(sec)", &(orbit_period), 1.0,
    "Period for one orbit (seconds)",
    "Orbital Period (seconds)", P_REAL, NULL},
   {"ORB_PERIOD"   , 8, " %12.7f"   ," OrbPer(min)", &(orbit_period),(1.0/60.0),
    "Period for one orbit (minutes)",
    "Orbital Period (minutes)", P_REAL, NULL},
   {"SUN_RX_ECI"   , 6, " %13.5e"   ,"  SUN_Rx_(km)", &(sun_r_eci.X), 0.001,
    "X component of the sun's position vector in ECI (km)",
    "Sun's R|-x|= ECI (km)", P_REAL, NULL},
   {"SUN_RY_ECI"   , 6, " %13.5e"   ,"  SUN_Ry_(km)", &(sun_r_eci.Y), 0.001,
    "Y component of the sun's position vector in ECI (km)",
    "Sun's R|-y|= ECI (km)", P_REAL, NULL},
   {"SUN_RZ_ECI"   , 6, " %13.5e"   ,"  SUN_Rz_(km)", &(sun_r_eci.Z), 0.001,
    "Z component of the sun's position vector in ECI (km)",
    "Sun's R|-z|= ECI (km)", P_REAL, NULL},
   {"SAT_IN_SHADOW", 7, " %8d  "    ," In_Shade?", &(sat_in_shadow), 1.0,
    "Is the satellite in the shadow of the Earth?",
    "Sat in Shade ?", P_INT, NULL},
   {"SOLAR_ANGLE",   8, " %12.7f" ,"  SolarAngle", &(solar_angle), R_D_CONST,
    "Angle of sun in degrees from vertical (deg)",
    "Solar Angle (deg)", P_REAL, NULL},
   {"SOLAR_AZ",   8, " %12.7f" ,"   SolarAzim", &(solar_az), R_D_CONST,
    "Azimuth of sun direction in LVLH (deg)", 
    "Azimuth to Sun (deg)", P_REAL, NULL},
   {"SOLAR_EL",   8, " %12.7f" ,"   SolarElev", &(solar_el), R_D_CONST,
    "Elevation of sun direction in LVLH (deg)", 
    "Elevation to Sun (deg)", P_REAL, NULL},
   {"LOC_TIME"     , 5, " %13.8f"   ," Loc_Time_(h)", &(local_time_h), 1.0,
    "Local Apparent Solar Time (hours)",
    "Local Time (hours)", P_REAL, NULL},
   {"ATM_DRAG"     , 5, " %13.4e"   ," Atm_Drag_(N)", &(atmos_drag_force_m),1.0,
    "Atmospheric Drag Force (N)",
    "Atmos Drag (N)", P_REAL, NULL},
   {"FAST_DELTA_T", 8, " %13.4e"   ," F_DeltaT(orbits)", &(fast_out_delta_t),1.0,
    "Fast Delta_T (orbits)",
    "F_DeltaT(orbits)", P_REAL, NULL},
   {"FAST_PERIOD", 8, " %13.4e"   ," F_Period(sec)", &(fast_out_period),1.0,
    "Fast Computed Orbital Periode (sec)",
    "F_OrbPer(sec)", P_REAL, NULL},
   {"FAST_RA"  , 7, " %13.4e"   ," F_RA(deg)", &(fast_out_ra),R_D_CONST,
    "Fast Right Ascension (deg)",
    "Fast RA (deg)", P_REAL, NULL},
   {"FAST_DEC" , 8, " %13.4e"   ," F_Dec(deg)", &(fast_out_dec),R_D_CONST,
    "Fast Declination (deg)",
    "Fast Dec (deg)", P_REAL, NULL},
   {"FAST_LAT" , 8, " %13.4e"   ," F_Lat(deg)", &(fast_out_lat), R_D_CONST,
    "Fast Latitude (deg)",
    "Fast Lat (deg)", P_REAL, NULL},
   {"FAST_LONG" , 8, " %13.4e"   ," F_Long(deg)", &(fast_out_long), R_D_CONST,
    "Fast Longitude (deg)",
    "Fast Long (deg)", P_REAL, NULL},
   {"FAST_DLAT" , 8, " %13.4e"   ," F_DLat(deg)", &(fast_out_dlat), R_D_CONST,
    "Fast Delta Latitude (deg)",
    "Fast DLat (deg)", P_REAL, NULL},
   {"FAST_DLONG" , 8, " %13.4e"   ," F_DLong(deg)", &(fast_out_dlong), R_D_CONST,
    "Fast Delta Longitude (deg)",
    "Fast DLong (deg)", P_REAL, NULL},
   {"FAST_SAT_IN_VIEW" ,10, " %13.4e"   ," Fast_Sat_In_view", &(fast_out_gs_sat_in_view), 1.0,
    "Fast Propagator GS Sat In View?",
    "Fast GS Sat In View?", P_INT, NULL},
   {"GS_SAT_ELEV" , 10, " %13.4e"   ," GS_Sat_Elev(deg)", &(gs_sat_elev), R_D_CONST,
    "Ground Station Elevation to Satellite (deg)",
    "GS Sat Elev (deg)", P_REAL, NULL},
   {"GS_SAT_IN_VIEW" ,10, " %13.4e"   ," GS_Sat_In_view", &(gs_sat_in_view), 1.0,
    "GS Sat In View?",
    "GS Sat In View?", P_INT, NULL},
   {"ZERO"         , 4, " %13.4e"   ,"      Zero_()", &(zero_out),0.0,
    "Zero ()",
    "Zero ()", P_REAL, NULL}

} ;

#else

extern int    ephem_year        ;
extern GMT    ephem_gmt         ;

extern double r_circular_alt    ;
extern double r_apogee          ;
extern double r_perigee         ;

extern double inclination       ;
extern double sun_sync_ltan     ;
extern double raan              ;
extern double arg_perigee       ;

extern double true_anomaly      ;
extern double mean_anomaly      ;
extern char   anomaly_str [10]  ;
extern int    anomaly_type      ;

extern int    orbit_perturb     ;
extern int    orbit_decay       ;
extern double bal_coeff         ;
extern double system_mass	;
extern int    orbit_precise     ;
extern int    ephem_from_rv     ;
extern Cartesian thrust_lvlh    ;

extern double semi_major_axis   ;
extern double eccentricity      ;

extern double alt_apogee        ; 
extern double alt_perigee       ;

extern double mean_motion       ;

extern double orbit_period      ;

extern double curr_true_anomaly ; 
extern double curr_mean_anomaly ;

extern int    orbit_decayed     ;

extern double d_raan            ; 
extern double d_arg_perigee     ;

extern double ephem_time        ;

extern Cartesian oblate_earth   ;
extern Cartesian solar_gravity  ;   
extern Cartesian lunar_gravity  ;
extern Cartesian solar_pressure ;
extern Cartesian atmos_drag     ;
extern Cartesian ilxb_accel     ;
extern Cartesian b_ilxb_accel   ;
extern Cartesian thrust_accel   ;

extern Cartesian total_perturb  ;

extern Cartesian atmos_drag_force   ;
extern double    atmos_drag_force_m ;

extern double fast_in_period     ;
extern double fast_in_delta_per  ;
extern double fast_in_time0_pc   ;
extern double fast_in_delta_raan ;
extern double fast_in_inclin     ;
extern double fast_in_ra0        ;
extern double fast_in_dec0       ;
extern double fast_in_raan0      ;
extern double fast_in_phi0       ;

extern double    gs_lat ;
extern double    gs_lon ;
extern double    gs_elev_mask ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
extern Cartesian sat_v_eci      ; 
extern double    sat_v_eci_mag  ;
extern Cartesian sat_r_eci      ;
extern double    sat_r_eci_mag  ;
extern Earth     sat_r_lla      ;
extern double    altitude_mer   ;
extern double    sat_ra         ;

extern Cartesian sun_r_eci      ;
extern Cartesian sun_pos_lvlh   ;
extern Earth     sun_r_lla      ;
extern double    local_time_h   ;
extern double    solar_angle    ;
extern double    solar_az       ;
extern double    solar_el       ;
extern int       sat_in_shadow  ;
extern double    zero_out       ;

extern double fast_out_delta_t ;
extern double fast_out_period ;
extern double fast_out_ra ;
extern double fast_out_dec ;
extern double fast_out_lat ;
extern double fast_out_long ;
extern double fast_out_dlat ;
extern double fast_out_dlong ;
extern double fast_out_gs_sat_elev ;
extern int    fast_out_gs_sat_in_view ;

extern double gs_sat_elev ;
extern int    gs_sat_in_view   ;

extern PARAM  orbit_param_list  [] ;
extern OUTVAR orbit_outvar_list [] ;

#endif
#if _NO_PROTO_

void init_orbital_params () ;

void compute_sat_position () ;

#else

void init_orbital_params () ;

void compute_sat_position () ;

#endif

