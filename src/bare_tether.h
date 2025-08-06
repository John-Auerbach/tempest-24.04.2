/*****************************************************************************/
/*                                                                           */
/*   Module:    bare_tether.h                                                */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the BARE_TETHER Module.   */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   05_May_97 NRV   Written.                                     */
/*                                                                           */
/*   RCS:       $Id: bare_tether.h,v 1.21 2008/04/17 09:54:40 nrv Exp $                                                        */
/*                                                                           *
 *              $Log: bare_tether.h,v 
 *              Revision 1.17  2005/05/26 21:06:05  voronk
 *              *** empty log message **
 *
 *              Revision 1.16  2001/06/22 02:07:34  nestorv
 *              Save before latest changes.
 *
 *              Revision 1.15  2000/10/25 19:37:17  nestorv
 *              Added V0_POSITION output.
 *
 *              Revision 1.14  2000/09/27 03:56:14  nestorv
 *              make down DN vs. DW
 *
 *              Revision 1.13  2000/09/27 03:47:02  nestorv
 *              Added up & down contactor models
 *
 * Revision 1.12  1999/09/30  04:47:13  nestorv
 * Added altitude control mode.
 *
 * Revision 1.12  1999/09/30  04:47:13  nestorv
 * Added altitude control mode.
 *
 * Revision 1.11  1999/09/30  04:14:55  nestorv
 * Check in after many updates
 *
 * Revision 1.10  1999/04/01  19:41:10  nestorv
 * Added vector based control mode.
 *
 * Revision 1.9  1999/02/09  02:59:30  nestorv
 * Changed errors in documentation.
 *
 * Revision 1.8  1999/02/09  02:50:11  nestorv
 * Added constant power supply model.
 *
 * Revision 1.7  1999/01/27  04:56:22  nestorv
 * Updated.
 *
 * Revision 1.6  1999/01/17  18:02:55  nestorv
 * *** empty log message ***
 *
 * Revision 1.5  1997/08/17  21:41:19  nestorv
 * Corrected iLxB computation and added output in Newtons
 *
 * Revision 1.4  1997/07/13  22:11:50  nestorv
 * Save.
 *
 * Revision 1.3  1997/06/11  18:06:25  nestorv
 * Seems to work.
 *
 * Revision 1.2  1997/06/06  00:43:28  nestorv
 * code that works but needs cleaning up.,
 *
 * Revision 1.1  1997/05/09  15:36:31  nestorv
 * Initial revision
 *
 * Revision 1.1  1997/05/09  15:35:45  nestorv
 * Initial revision
 *                                                       */
/*                                                                           */
/*****************************************************************************/

                                  /* Fixed physical, circuit & orbital parms */

#define Q_EL	1.602e-19         /* Fundamental charge of an electron (C)   */
#define M_EL	9.109558e-31      /* Rest Mass of an Electron (kg)           */
#define K_BOLTZ	1.380658e-23      /* Boltzmans constant (J/K)                */

#define MAX_BARE_T_SEG     1000   /* Maximum number of bare tether segments  */

enum {FIXED=0, FEA=1, PM=2, MODEL3=3, MODEL4=4, BARE=5, CYLIND=6, HCPC=7 } ;

enum {BATTERY=0, DOUBLE_EMF, CONST_PWR} ;

enum {VECT_CNTRL_OFF=0, VECT_CNTRL_CONE} ;

enum {POS_CNTRL_OFF=0, POS_CNTRL_CYL, POS_CNTRL_ALT} ;

#ifdef TEMPEST_MAIN
                                   /* V(I) model for hollow cathode          */
int    bare_contact_model_up = FIXED ;
int    bare_contact_model_down = FIXED ;
                                   /* Parameters for V(I) models             */
double bare_contact_p1_up = 0.0  ;
double bare_contact_p2_up = 0.0  ;
double bare_contact_p3_up = 0.0  ;
double bare_contact_p4_up = 0.0  ;
                                   /* Parameters for V(I) models             */
double bare_contact_p1_down = 0.0  ;
double bare_contact_p2_down = 0.0  ;
double bare_contact_p3_down = 0.0  ;
double bare_contact_p4_down = 0.0  ;

double bare_anode_bias    = 0.0  ; /* Bias for anode in an upward deployment */
double bare_up_load       = 0.0  ; /* Load resistance for upware deployment  */

double bare_cathode_bias  = 0.0  ; /* Cathode bias for downard deployment    */
double bare_down_voltage  = 0.0  ; /* Supply voltage for downard deployment  */
double bare_down_power    = 0.0  ; /* Supply power for downard deployment    */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

                                   /* Specifies IV characteristics of power  */
                                   /*   supply for thrust mode               */
int    thrust_control     = DOUBLE_EMF ;

                                   /* Specifies control mode based on the    */
                                   /*    direction of the thrust vector      */
int    vector_control     = VECT_CNTRL_OFF ;
                                   /* Vector indicating desired thrust       */
                                   /*    direction in LVLH (unitless)        */
Cartesian control_lvlh    = {1.0, 0.0, 0.0} ;
                                   /* Maximum Permitted deviation in radians */
double max_cone_angle         = 0.0 ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

                                   /* Specifies control mode based on the    */
                                   /*    position of the satellite           */
int    position_control   = POS_CNTRL_OFF ;
                                   /* Latitude, Longitude and altitude for   */
                                   /*    position based control              */
Earth     pos_cntrl_lla      =
                               {0.0, 0.0, 0.0} ;
                                   /* Location in ECI for position control   */
Cartesian pos_cntrl_eci             ;
                                   /* Magnitude of ECI position vector       */
double pos_cntrl_eci_mag            ;
                                   /* Radius for position based control      */
double pos_cntrl_radius      = 0.0  ;
                                   /* Altitude deviation for position contrl */
double pos_cntrl_d_alt       = 0.0  ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double dRt                = 0.05 ; /* Tether resistance per unit length      */

double bare_ion_mass = 2.678e-26 ; /* Mass of the ion (O+ default) in kg     */

int    bare_tether_seg    = 100  ; /* Number of bare tether segments         */

double di_dy_scale        = 1.0  ; /* Arbitrary tether current scaling       */
double di_dy_beta         = 0.5  ; /* Arbitrary exponent for sqrt(v)         */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double fea_n     = 1.0e6  ;        /* Number of field emitter tips           */

double fea_a     = 8.0e-7 ;        /* Fowler-Nordheim parameter a A/V^2/tip  */

double fea_b     = 800.0  ;        /* Fowler-Nordheim parameter b            */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double pm_r_sat  = 1.6    ;        /* Satellite radius for Parker-Murphy (m) */

double pm_alpha  = 2.5    ;        /* Alpha parameter for Parker-Murphy      */

double pm_beta   = 0.52   ;        /* Beta parameter for Parker-Murphy       */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double i_bare_end                ;
double i_bare_max                ; 
double i_bare_ave                ; 

double imax_position             ;
double v0_position               ;

double v_anode_out               ;
double v_cathode_out             ;
double v_power_supply            ;
double p_power_supply            ;
double p_load                    ;
double v_bare_res                ;

Cartesian bare_ixb_lvlh          ;
double    bare_ixb_lvlh_mag      ;

Cartesian bare_ilxb_lvlh         ;
double    bare_ilxb_lvlh_mag     ;

double    thrust_cone_angle      ;
int       vect_thrust_out        ;

int       pos_thrust_out         ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS bare_tether_param_list [] =
{
   {"ANODE_BIAS"    , P_REAL, &bare_anode_bias,
    "Anode bias voltage for upward deployed bare tether          "},
   {"BARE_LOAD"     , P_REAL, &bare_up_load,
    "Load resistor at bottom for upward deployed bare tether     "},
   {"CATHODE_BIAS"  , P_REAL, &bare_cathode_bias,
    "Cathode bias voltage for downward deployed bare tether      "},
   {"THRUST_VOLTAGE", P_REAL, &bare_down_voltage,
    "Supply voltage for downward deployed thrust-mode tether      "},
   {"THRUST_POWER"  , P_REAL, &bare_down_power,
    "Supply voltage for downward deployed thrust-mode tether      "},
   {"THRUST_CONTROL", P_INT, &thrust_control,
    "Controls supply V(i) for thrust (0=batt,1=2*EMF,2=const pwr)"},
   {"VECTOR_CONTROL" , P_INT , &vector_control,
    "Vector based bare tether control mode (0=OFF, 1=CONE)       "},
   {"VECT_CONE_ANGLE", P_DEG , &max_cone_angle,
    "Maximum Cone angle for thrust enabling (degrees)            "},
   {"CONTROL_X"      , P_REAL, &control_lvlh.X,
    "X component of desired thrust for vector based control      "},
   {"CONTROL_Y"      , P_REAL, &control_lvlh.Y,
    "Y component of desired thrust for vector based control      "},
   {"CONTROL_Z"      , P_REAL, &control_lvlh.Z,
    "Z component of desired thrust for vector based control      "},
   {"POS_CONTROL"    , P_INT , &position_control,
    "Position based bare tether control mode (0=OFF,1=CYL,2=ALT)"},
   {"POS_CNTRL_LAT"  , P_DEG  , &(pos_cntrl_lla.Lat),
    "Position based bare tether control mode Latitude (degrees)  "},
   {"POS_CNTRL_LONG" , P_DEG  , &(pos_cntrl_lla.Long),
    "Position based bare tether control mode Longitude (degrees) "},
   {"POS_CNTRL_ALT"  , P_REAL , &(pos_cntrl_lla.Alt),
    "Position based bare tether control mode Altitude (meters)   "},
   {"POS_CNTRL_RAD"  , P_REAL , &pos_cntrl_radius,
    "Position based bare tether control mode Radius (meters)     "},
   {"POS_CNTRL_DALT" , P_REAL , &pos_cntrl_d_alt,
    "Position based bare tether control mode Altitude Delta (m)  "},
   {"DR_TETHER"     , P_REAL, &dRt,
    "Resistance of the tether (ohms/meter) (default is 0.05)     "},
   {"B_ION_MASS"    , P_REAL, &bare_ion_mass,
    "Ion mass for tether current calculations (kg) [O+ default]  "},
   {"CONTACTOR_UP" , P_INT , &bare_contact_model_up,
    "Model for upper end plasma contactor (0=fixed V,1=FEA,2=P-M)"},
   {"CONTACTOR_P1_UP", P_REAL, &bare_contact_p1_up,
    "Parameter for contactor model (CONTACT_MODEL!=0)            "},
   {"CONTACTOR_P2_UP", P_REAL, &bare_contact_p2_up,
    "Parameter for contactor model (CONTACT_MODEL!=0)            "},
   {"CONTACTOR_P3_UP", P_REAL, &bare_contact_p3_up,
    "Parameter for contactor model (CONTACT_MODEL!=0)            "},
   {"CONTACTOR_P4_UP", P_REAL, &bare_contact_p4_up,
    "Parameter for contactor model (CONTACT_MODEL!=0)            "},
   {"CONTACTOR_DOWN", P_INT , &bare_contact_model_down,
    "Model for lower end plasma contactor (0=fixed V,1=FEA,2=P-M)"},
   {"CONTACTOR_P1_DN", P_REAL, &bare_contact_p1_down,
    "Parameter for contactor model (CONTACT_MODEL!=0)            "},
   {"CONTACTOR_P2_DN", P_REAL, &bare_contact_p2_down,
    "Parameter for contactor model (CONTACT_MODEL!=0)            "},
   {"CONTACTOR_P3_DN", P_REAL, &bare_contact_p3_down,
    "Parameter for contactor model (CONTACT_MODEL!=0)            "},
   {"CONTACTOR_P4_DN", P_REAL, &bare_contact_p4_down,
    "Parameter for contactor model (CONTACT_MODEL!=0)            "},
   {"BARE_SEGMENTS" , P_INT , &bare_tether_seg,
    "Number of tether segments for integration (default 100)     "},
   {"FEA_N"         , P_REAL, &fea_n,
    "Number of tips in Field Emitter Array (FEA)                 "},
   {"FEA_A"         , P_REAL, &fea_a,
    "Fowler-Nordheim parameter a of FEA (Amps/Volts^2/tip)       "},
   {"FEA_B"         , P_REAL, &fea_b,
    "Fowler-Nordheim parameter b of FEA (Volts)                  "},
   {"PM_R_SAT"      , P_REAL, &pm_r_sat,
    "Satellite radius for Parker-Muprhy contactor model (meters) "},
   {"PM_ALPHA"      , P_REAL, &pm_alpha,
    "Alpha parameter for Parker-Murphy contactor model           "},
   {"PM_BETA"       , P_REAL, &pm_beta,
    "Beta parameter for Parker-Murphy contactor model            "},
   {"DI_DY_SCALE"   , P_REAL, &di_dy_scale, 
    "Scaling of di_dy tether segment current (default 1.0)       "},
   {"DI_DY_BETA"    , P_REAL, &di_dy_beta,
    "Alternate exponent for sqrt(V) in di_dy calculation         "}
} ;

struct OUTPUT_VARS bare_tether_outvar_list [] =
{
   {"I_BARE_END"   , 8, " %12.7f"    , " I_b_end_(A)", &(i_bare_end), 1.0,
    "Tether current at end of bare tether (Amps)",
    "End Tether Current (Amps)", P_REAL, NULL},
   {"I_BARE_MAX"   , 8, " %12.7f"    , " I_b_max_(A)", &(i_bare_max), 1.0,
    "Maximum tether current along bare tether (Amps)",
    "Max Tether Current (Amps)", P_REAL, NULL},
   {"I_BARE_AVE"   , 8, " %12.7f"    , " I_b_ave_(A)", &(i_bare_ave), 1.0,
    "Average tether current along bare tether (Amps)",
    "Average Tether Current (Amps)", P_REAL, NULL},
   {"IMAX_POSITION", 6, " %12.4f"    , " Imax_loc(m)", &(imax_position), 1.0,
    "Location along tether of maximum current (meters)",
    "I|-max|= position (meters)", P_REAL, NULL}, 
   {"V0_POSITION"  , 6, " %12.4f"    , "   V0_loc(m)", &(v0_position), 1.0,
    "Location along tether of zero potential (meters)",
    "V|-0|= position (meters)", P_REAL, NULL}, 
   {"V_CATHODE"    , 5, " %10.4f"    , " V_cathode", &(v_cathode_out), 1.0,
    "For upward deployed tether, cathode voltage (at bottom) (Volts)",
    "Cathode Voltage (Volts)", P_REAL, NULL},
   {"V_ANODE"      , 5, " %10.4f"    , "   V_anode", &(v_anode_out), 1.0,
    "For downard deployed tether, anode voltage (at bottom) (Volts)",
    "Anode Voltage (Volts)", P_REAL, NULL},
   {"V_SUPPLY"     , 5, " %10.4f"    , "  V_supply", &(v_power_supply), 1.0,
    "For downard deployed tether, power supply voltage (at top) (Volts)",
    "Power Supply Voltage (Volts)", P_REAL, NULL},
   {"P_SUPPLY"     , 5, " %10.4f"    , "  P_supply", &(p_power_supply), 1.0,
    "For downard deployed tether, power supply output (at top) (Watts)",
    "Power Supply Output (W)", P_REAL, NULL},
   {"P_LOAD"      , 5, " %10.4f"    , "  P_load", &(p_load), 1.0,
    "Power dissipated in load resistor (Watts)", 
    "Power Delivered to Load R (W)", P_REAL, NULL},
   {"BARE_RESIDUAL" , 6, " %10.5f"    , "   V_b_res", &(v_bare_res), 1.0,
    "Residual term from numerical solution (volts)",
    "Bare Tether Residual (Volts)", P_REAL, NULL},
   {"B_IXB_X_LVLH"  , 7, " %13.4e"   , " IxB0_x_(N/m)",&(bare_ixb_lvlh.X), 1.0,
    "X component of generated IxB force in LVLH (Newtons/meter)",
    "(IxB|-0|=)|-X|= (N/m)", P_REAL, NULL},
   {"B_IXB_Y_LVLH"  , 7, " %13.4e"   , " IxB0_y_(N/m)",&(bare_ixb_lvlh.Y), 1.0,
    "Y component of generated IxB force in LVLH (Newtons/meter)",
    "(IxB|-0|=)|-Y|= (N/m)", P_REAL, NULL},
   {"B_IXB_Z_LVLH"  , 7, " %13.4e"   , " IxB0_z_(N/m)",&(bare_ixb_lvlh.Z), 1.0,
    "Z component of generated IxB force in LVLH (Newtons/meter)",
    "(IxB|-0|=)|-Z|= (N/m)", P_REAL, NULL},
   {"B_IXB_MAG"     , 7, " %13.4e"   , " |IxB0|_(N/m)",&(bare_ixb_lvlh_mag),1.0,
    "Magnitude of generated IxB force (Newtons/meter)",
    "||IxB|-0|=|| (N/m)", P_REAL, NULL},
   {"B_ILXB_X_LVLH" , 8, " %13.4e"  , "  iLxB0_x_(N)",&(bare_ilxb_lvlh.X), 1.0,
    "X component of generated iLxB force in LVLH (Newtons)",
    "(iLxB|-0|=)|-X|= (N)", P_REAL, NULL},
   {"B_ILXB_Y_LVLH" , 8, " %13.4e"  , "  iLxB0_y_(N)",&(bare_ilxb_lvlh.Y), 1.0,
    "Y component of generated iLxB force in LVLH (Newtons)",
    "(iLxB|-0|=)|-Y|= (N)", P_REAL, NULL},
   {"B_ILXB_Z_LVLH" , 8, " %13.4e"  , "  iLxB0_z_(N)",&(bare_ilxb_lvlh.Z), 1.0,
    "Z component of generated iLxB force in LVLH (Newtons)",
    "(iLxB|-0|=)|-Z|= (N)", P_REAL, NULL},
   {"B_ILXB_MAG"    , 8, " %13.4e"  , "  |iLxB0|_(N)",&(bare_ilxb_lvlh_mag),1.0,
    "Magnitude of generated iLxB force (Newtons)",
    "||iLxB|-0|=|| (N)", P_REAL, NULL},
   {"THRUST_ANGLE"  , 8, " %18.4f"  , " thrust_angl_(deg)",&(thrust_cone_angle),
    R_D_CONST, "Angle between actual and desired thrust vectors (degrees)",
    "Deviation from desired thrust (degrees)", P_REAL, NULL},
   {"VECTOR_CONTROL", 8, " %9d"  , " vect_cntrl",&(vect_thrust_out), 1.0, 
    "Vector thrust control on/off",
    "Vector control", P_INT, NULL},
   {"POS_CONTROL"   , 8, " %9d"  , "  pos_cntrl",&(pos_thrust_out), 1.0, 
    "Position based thrust control on/off",
    "Position control", P_INT, NULL}
} ;

#else

extern int    bare_contact_model_up ;
extern double bare_contact_p1_up    ;
extern double bare_contact_p2_up    ;
extern double bare_contact_p3_up    ;
extern double bare_contact_p4_up    ;

extern int    bare_contact_model_down ;
extern double bare_contact_p1_down    ;
extern double bare_contact_p2_down    ;
extern double bare_contact_p3_down    ;
extern double bare_contact_p4_down    ;

extern double bare_anode_bias    ;
extern double bare_up_load       ;

extern double bare_cathode_bias  ;
extern double bare_down_voltage  ;
extern double bare_down_power    ;

extern int    thrust_control     ;

extern int    vector_control     ;
extern Cartesian control_lvlh    ;
extern double max_cone_angle     ;

extern int       position_control   ;
extern Earth     pos_cntrl_lla      ;
extern Cartesian pos_cntrl_eci      ;
extern double    pos_cntrl_eci_mag  ;
extern double    pos_cntrl_radius   ;
extern double    pos_cntrl_d_alt    ;

extern double dRt                ;

extern double bare_ion_mass      ;

extern int    bare_tether_seg    ;

extern double di_dy_scale        ;
extern double di_dy_beta         ;

extern double fea_n              ;
extern double fea_a              ;
extern double fea_b              ;

extern double pm_r_sat           ;
extern double pm_alpha           ;
extern double pm_beta            ;

extern double i_bare_end         ; 
extern double i_bare_max         ; 
extern double i_bare_ave         ; 

extern double imax_position      ;
extern double v0_position               ;

extern double v_cathode_out      ;
extern double v_anode_out        ;
extern double v_power_supply     ;
extern double p_power_supply     ;
extern double p_load             ;
extern double v_bare_res         ;

extern Cartesian bare_ixb_lvlh     ;
extern double    bare_ixb_lvlh_mag ;

extern Cartesian bare_ilxb_lvlh     ;
extern double    bare_ilxb_lvlh_mag ;

extern double    thrust_cone_angle  ;
extern int       vect_thrust_out    ;

extern int       pos_thrust_out  ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if _NO_PROTO_

void init_bare_tether_params () ;

void compute_bare_tether     () ;

#else

void init_bare_tether_params () ;

void compute_bare_tether     () ;

#endif
