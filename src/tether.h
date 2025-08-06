/*****************************************************************************/
/*                                                                           */
/*   Module:    tether.h                                                     */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the TETHER Module.        */
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
/*   RCS:	$Id: tether.h,v 1.15 2005/06/07 00:13:13 voronka Exp $                                                        */
/*                                                                           */
/*		$Log: tether.h,v $
 *		Revision 1.15  2005/06/07 00:13:13  voronka
 *		*** empty log message ***
 *
 *		Revision 1.14  2004/09/29 05:44:34  voronka
 *		unit changes
 *
 * Revision 1.13  1999/12/08  05:15:05  nestorv
 * Commented out tether temp stuff.
 *
 * Revision 1.12  1999/12/08  03:54:06  nestorv
 * Added temperature stuff.
 *
 * Revision 1.11  1997/05/09  21:46:27  nestorv
 * Changed BARE_LENGTH to BARE_END.
 *
 * Revision 1.10  1997/05/09  20:19:35  nestorv
 * Added radii of tether.
 *
 * Revision 1.9  1997/05/09  13:45:53  nestorv
 * Added NULL array_lengths to OUTPUT_VARS list.
 *
 * Revision 1.8  1997/05/09  12:57:05  nestorv
 * Added BARE_START and BARE_LENGTH.
 *
 * Revision 1.7  1996/02/18  06:53:37  nestorv
 * Changed DPLY_MET0 to DPLY_GMT0.
 *
 * Revision 1.6  1996/02/18  06:38:52  nestorv
 * Changed dply time from MET to GMT.
 *
 * Revision 1.5  1995/11/22  00:38:33  nestorv
 * Changed initial tether length from 0.0 to 20700.0.
 *                                                       */
/*                                                                           */
/*****************************************************************************/

extern double compute_tether_temp (double current) ;
extern double compute_tether_res  (double temp   ) ;

#define MAX_LEN_POLY_ORDER	4

#define MAX_TEMP_ITER		25
#define	RES_CONV_THRESH		0.1

#define tether_end		(length_poly[0])

#ifdef TEMPEST_MAIN

double tether_start =      0.0 ;   /* Tether starting point from orbiting    */
                                   /*    body position (r) +up - down        */

double libration_mag_ip =  0.0 ;   /* Magnitude of in-plane libration (m)    */

double libration_mag_op =  0.0 ;   /* Magnitude of out-of-plane libration (m)*/

double libration_mag_ra =  0.0 ;   /* Magnitude of axial libration (% elong) */

Cartesian tether_orient_lvlh =     /* Vector describing the tethers initial  */
              {-1, -1, -1} ;       /*  orientation in LVLH (X=V, Z=+R)       */

double ip_lib_phase0    =  0.0 ;   /* Initial phase for in-plane     lib     */
double op_lib_phase0    =  0.0 ;   /* Initial phase for out-of-plane lib     */
double ra_lib_phase0    =  0.0 ;   /* Initial phase for radial       lib     */
                                   /*    (specified in deg - stored in rad)  */

int    lib_year = -1           ;   /* Year when initial phase was specified  */
GMT    lib_gmt  =                  /* GMT  when initial phase was specified  */
               {-1, -1, -1, -1, -1} ;
double lib_time                ;   /* Time when initial phase was specified  */

int    dply_year = -1          ;   /* Year when current dply step specified */
GMT    dply_gmt  =                 /* GMT  when current dply step specified */
               {-1, -1, -1, -1, -1} ;
double dply_time               ;   /* Time when current dply step specified */

double length_poly [MAX_LEN_POLY_ORDER] =
                   {20700.0, 0.0, 0.0, 0.0} ;

double t_bare_start     =  0.0 ;   /* Start of bare tether section           */
double t_bare_end       =  0.0 ;   /* End of bare tether section             */

double tether_outer_radius = 0.0 ; /* Outer radius of tether (meters)        */
double tether_cond_radius  = 0.0 ; /* Radius of conducter of tether (meters) */

double tether_resistivity  = 0.0 ; /* Resistivity of conductor @ Tr ohm-cm   */
double tether_resist_temp  = 0.0 ; /* Temperature at which resistivity of    */
                                   /*   tether is specified deg C            */
double tether_temp_coeff   = 0.0 ; /* Temperature resistance coefficient in  */
                                   /*    ppm/deg C                           */
double t_insul_emissivity  = 0.0 ; /* Solar emissivity  of insulated tether  */
double t_insul_absorbtivity= 0.0 ; /* Solar absorbtivity of insulated tether */

double t_bare_emissivity   = 0.0 ; /* Solar emissivity  of bare tether       */
double t_bare_absorbtivity = 0.0 ; /* Solar absorbtivity of bare tether      */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double    tether_len     ;         /* Non radially librating tether length   */
double    tether_l_dot   ;         /* Non radially librating tether l_dot    */

double    ip_lib_period  ;         /* Period of in-plane tether libration    */
double    op_lib_period  ;         /* Period of out-of-plan tether libration */
double    ra_lib_period  ;         /* Period of radial tether libration      */
                                   /*    (periods in Greenwich solar days)   */

double    cur_ip_lib     ;         /* Current in-plane tether libration      */
double    cur_op_lib     ;         /* Current out-of-plane tether libration  */
double    cur_ra_elong   ;         /* Current radial elongation (libration)  */
                                   /*    (stored in radians)                 */

Cartesian tether_gg_lvlh ;         /* Vector describing the gravity gradient */
                                   /*  oriented tether (lvlh)                */
Cartesian tether_lib_lvlh;         /* Vector describing the lilrating tether */
                                   /*  in LVLH coordinates (X=V, Z=+R)       */
double    tether_lib_len ;         /* Length of librating tether             */

Cartesian tether_start_r_eci ;     /* Position of tether start in ECI        */
Cartesian tether_end_r_eci   ;     /* Position of tether end   in ECI        */

Earth     tether_start_r_lla ;     /* Position of tether start (lat,long,alt)*/
Earth     tether_end_r_lla   ;     /* Position of tether end   (lat,long,alt)*/

double    tether_temp        ;     /* Tether temperature (deg C)             */
double    tether_resist      ;     /* Tether resistance (ohms)               */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS tether_param_list [] =
{
   {"TETHER_END"    , P_REAL, &tether_end,
    "Distance to tether end   (m) (same as TETHER_LEN)           "},
   {"BARE_START"    , P_REAL, &t_bare_start,
    "Distance along tether to start of bare section (m)          "},
   {"BARE_END"      , P_REAL, &t_bare_end,
    "Distance along tether to end of bare section (m)            "},
   {"TETHER_LEN"    , P_REAL, &(length_poly[0]),
    "Constant value for tether length polynomial (m +=up -=down) "},
   {"TETHER_LEN_1"  , P_REAL, &(length_poly[1]),
    "1st order value for tether length polynomial (m/sec)        "},
   {"TETHER_LEN_2"  , P_REAL, &(length_poly[2]),
    "2nd order value for tether length polynomial (m/sec^2)      "},
   {"TETHER_LEN_3"  , P_REAL, &(length_poly[3]),
    "3rd order value for tether length polynomial (m/sec^3)      "},
   {"T_OUTER_RADIUS", P_REAL, &tether_outer_radius,
    "Outer radius of tether (m)                                  "},
   {"T_COND_RADIUS" , P_REAL, &tether_cond_radius,
    "Radius of conductor in/of tether (m)                        "},
   {"T_RESISTIVITY" , P_REAL, &tether_resistivity,
    "Resistivity of tether material (ohm-cm) at given temperature"},
   {"T_RESIST_TEMP" , P_REAL, &tether_resist_temp,
    "Temperature at which resisitivity is specified (deg C)      "},
   {"T_RES_TEMPCO"  , P_REAL, &tether_temp_coeff,
    "Temperature resistance coefficient of tether (per deg C)    "},
   {"I_EMISSIVITY"  , P_REAL, &t_insul_emissivity,
    "Infrared emissivity of insulated tether segment             "},
   {"B_EMISSIVITY"  , P_REAL, &t_bare_emissivity,
    "Infrared emissivity of bare tether segment                  "},
   {"I_ABSORBTIVITY", P_REAL, &t_insul_absorbtivity,
    "Solar absorbtivity of insulated tether segment              "},
   {"B_ABSORBTIVITY", P_REAL, &t_bare_absorbtivity,
    "Solar absorbtivity of bare tether segment                   "},
   {"T_ORIENT_LVLH_X", P_REAL, &tether_orient_lvlh.X,
    "Initial X Tether Orientation in LVLH (will be normalized)     "},
   {"T_ORIENT_LVLH_Y", P_REAL, &tether_orient_lvlh.Y,
    "Initial Y Tether Orientation in LVLH (will be normalized)     "},
   {"T_ORIENT_LVLH_Z", P_REAL, &tether_orient_lvlh.Z,
    "Initial Z Tether Orientation in LVLH (will be normalized)     "},
   {"INPLANE_LIB"   , P_REAL, &libration_mag_ip,
    "Magnitude of in-plane libration (meters)                    "},
   {"OUTOFPLANE_LIB", P_REAL, &libration_mag_op,
    "Magnitude of out-of-plane libration (meters)                "},
   {"RADIAL_LIB"    , P_REAL, &libration_mag_ra,
    "Magnitude of radial libration (% elongation)                "},
   {"IP_PHASE"      , P_DEG , &ip_lib_phase0 ,
    "Initial phase angle for in-plane libration (degrees)        "},
   {"OP_PHASE"      , P_DEG , &op_lib_phase0 ,
    "Initial phase angle for in-plane libration (degrees)        "},
   {"RAD_PHASE"     , P_DEG , &ra_lib_phase0 ,
    "Initial phase angle for in-plane libration (degrees)        "},
   {"LIB_GMT0"      , P_GMT , &lib_gmt ,
    "GMT of time when initial phase angle was measured           "},
   {"LIB_YEAR0"     , P_INT , &lib_year,
    "GMT year of time when initial phase angle was measured      "},
   {"DPLY_GMT0"     , P_GMT , &dply_gmt ,
    "GMT of time when initial deployment values specified        "},
   {"DPLY_YEAR0"    , P_INT , &dply_year,
    "GMT year of time when initial deployment values specified   "}
} ;

struct OUTPUT_VARS tether_outvar_list [] =
{
   {"L_MAG"        , 1, " %13.5f"   , "  Lib_T_L_(m)", &(tether_lib_len), 1.0,
    "Length of tether vector (w/  libration) (m)",
    "Librating Tether Length (m)", P_REAL, NULL},
   {"LENGTH"       , 3, " %13.5f"   , " Tether_L_(m)", &(tether_len), 1.0,
    "Length of tether vector (w/o libration) (m)",
    "Tether length (m)", P_REAL, NULL},
   {"L_DOT"        , 3, " %13.5f"   , "  L_dot_(m/s)", &(tether_l_dot), 1.0,
    "First derivative of the tether length (w/o libration) (m/sec)",
    "LI|-dot|= (m)", P_REAL, NULL},
   {"LX_LVLH"      , 2, " %13.5f"   , "  Lx_LVLH_(m)", &(tether_lib_lvlh.X),1.0,
    "X component of tether orientation vector in LVLH (m)",
    "Tether Orientation L|-x|= (m)", P_REAL, NULL},
   {"LY_LVLH"      , 2, " %13.5f"   , "  Ly_LVLH_(m)", &(tether_lib_lvlh.Y),1.0,
    "Y component of tether orientation vector in LVLH (m)",
    "Tether Orientation L|-y|= (m)", P_REAL, NULL},
   {"LZ_LVLH"      , 2, " %13.5f"   , "  Lz_LVLH_(m)", &(tether_lib_lvlh.Z),1.0,
    "Z component of tether orientation vector in LVLH (m)",
    "Tether Orientation L|-z|= (m)", P_REAL, NULL},
   {"IP_LIBRATION" , 6, " %13.5f"   , " IP_lib_(deg)", &(cur_ip_lib), R_D_CONST,
    "Current in-plane tether libration angle (degrees)",
    "In-plane libration (deg)", P_REAL, NULL},
   {"OP_LIBRATION" , 6, " %13.5f"   , " OP_lib_(deg)", &(cur_op_lib), R_D_CONST,
    "Current out-of-plane tether libration angle (degrees)",
    "Out-of-plane libration (deg)", P_REAL, NULL},
   {"RA_LIBRATION" , 6, " %15.5f"   , " RA_elong_(%el)", &(cur_ra_elong), 100.0,
    "Current radial tether libration length (% elongation)",
    "Radial tether libration (% elongation)", P_REAL, NULL},
   {"T_TEMPERATURE" , 6, " %15.5f"  , " T_temp_C_(ohm)", &(tether_temp), 1.0,
    "Tether temperature (deg C)",
    "Tether temperature (deg C)", P_REAL, NULL},
   {"T_RESISTANCE" , 5, " %15.5f"   , " T_resist_(ohm)", &(tether_resist), 1.0,
    "Tether resistance (ohms)",
    "Tether resistance (ohms)", P_REAL, NULL}
} ;

#else

extern double tether_start       ;

extern double libration_mag_ip   ;
extern double libration_mag_op   ;
extern double libration_mag_ra   ;

extern Cartesian tether_orient_lvlh ;

extern double ip_lib_phase0      ;
extern double op_lib_phase0      ;
extern double ra_lib_phase0      ;

extern int    lib_year           ;
extern GMT    lib_gmt            ;
extern double lib_time           ;

extern int    dply_year          ;
extern GMT    dply_gmt           ;
extern double dply_time          ;

extern double length_poly [MAX_LEN_POLY_ORDER] ;

extern double t_bare_start        ;
extern double t_bare_end          ;

extern double tether_outer_radius ;
extern double tether_cond_radius  ;

extern double tether_resistivity  ;
extern double tether_resist_temp  ;
extern double tether_temp_coeff   ;

extern double t_insul_emissivity   ;
extern double t_insul_absorbtivity ;

extern double t_bare_emissivity   ;
extern double t_bare_absorbtivity ;

extern double    tether_len      ;
extern double    tether_l_dot    ;

extern double    ip_lib_period   ;
extern double    op_lib_period   ;
extern double    ra_lib_period   ;

extern double    cur_ip_lib      ;
extern double    cur_op_lib      ;
extern double    cur_ra_elong    ;

extern Cartesian tether_gg_lvlh  ;

extern Cartesian tether_lib_lvlh ;

extern double    tether_lib_len  ;

extern Cartesian tether_start_r_eci ;
extern Cartesian tether_end_r_eci   ;

extern Earth     tether_start_r_lla ;

extern Earth     tether_end_r_lla   ;

extern double tether_temp   ;
extern double tether_resist ;

extern PARAM  tether_param_list  [] ;
extern OUTVAR tether_outvar_list [] ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _NO_PROTO_

void init_libration_params    () ;

void compute_tether_libration () ;

#else

void init_libration_params    () ;

void compute_tether_libration () ;

#endif
