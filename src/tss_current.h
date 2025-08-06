
/*****************************************************************************/
/*                                                                           */
/*   Module:    tss_current.h                                                */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the TSS_CURRENT Module.   */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   13_Aug_95 NRV   Written.                                     */
/*              26_Sep_95 NRV   Added peak transient voltage and res freq    */
/*                              Revised EGA perveances per MSFC-PROC-2549    */
/*   RCS:       $Id: tss_current.h,v 1.14 1999/04/01 19:41:57 nestorv Exp $                                                        */
/*                                                                           */
/*              $Log: tss_current.h,v $
 * Revision 1.14  1999/04/01  19:41:57  nestorv
 * Don't remember latest revs - it has been a LONG time.
 *
 * Revision 1.13  1997/05/09  13:47:25  nestorv
 * Added NULL array_lengths to OUTPUT_VARS list.
 *
 * Revision 1.12  1996/10/27  01:16:46  nestorv
 * Corrected units on ixB to N/m.
 *
 * Revision 1.11  1996/02/26  11:52:59  nestorv
 * Corrected units for IxB.
 *
 * Revision 1.10  1995/11/03  21:01:53  nestorv
 * Added PEAK_N_VOLT to output.
 *
 * Revision 1.9  1995/11/01  21:32:35  nestorv
 * Added PEAK_VOLT (v_peak_volt) = EMF + Trans parameter to module.
 *
 * Revision 1.8  1995/10/27  21:51:32  nestorv
 * Moved internal computed values to display parameter list.
 *
 * Revision 1.7  1995/10/07  05:59:57  nestorv
 * Updated printing format for IxB to scientific notation.
 *
 * Revision 1.6  1995/10/07  05:24:25  nestorv
 * Corrected IxB variable entry.
 *
 * Revision 1.5  1995/10/07  05:14:41  nestorv
 * Added IxB force variables and output paramters.
 *
 * Revision 1.4  1995/10/04  00:44:01  nestorv
 * Added OPEN to choice of TCVM resistors, and changed it to be the default
 * resistor for the TCVM mode.
 *
 * Revision 1.3  1995/10/02  20:45:12  nestorv
 * Defined TCVM impedance with no load resistors in to be R_TCVM_INF = 1.0e12
 *
 * Revision 1.2  1995/10/02  15:57:06  nestorv
 * Updated length of tether on reel per revised number from Ed Strakota.
 *                                                       */
/*                                                                           */
/*****************************************************************************/


                                   /* TSS system Mode enumeration            */
enum {EGA, TCVM, TCVM_FPEG, FPEG, PASSIVE} ;

#define NUM_TSS_MODES   5
                                  /* SETS load resistores enumeration        */
enum {SHUNT, R25K, R250K, R2500K, OPEN} ;

                                  /* Defined impedance for TCVM open circuit */
#define R_TCVM_INF	1.0e12

#define NUM_SETS_RLOADS  5
                                  /* Fixed physical, circuit & orbital parms */

#define ORB_VELOCITY    7400.0    /* Orbital Velocity (m/s)                  */

#define Q_EL	1.602e-19         /* Fundamental charge of an electron (C)   */
#define M_EL	9.109558e-31      /* Rest Mass of an Electron (kg)           */
#define K_BOLTZ	1.380658e-23      /* Boltzmans constant (J/K)                */

                                  /* Effective radius for orbiter current    */
                                  /*   collectiong surface                   */
#define ORB_EFF_RAD	sqrt (A_E_ORB/(2.0*M_PI))

#ifdef TEMPEST_MAIN

char   tss_mode_str      [] [10] = { {"EGA"}, {"TCVM"}, {"TCVM_FPEG"},
                                     {"FPEG"},{"PASSIVE"} } ;
char   tcvm_resistor_str [] [ 7] = { {"SHUNT"}, {"R25K"}, {"R250K"},
                                     {"R2500K"}, {"OPEN"} } ;

double tcvm_resistor_value   []  = {15.0, 2.5e+4, 2.94e+5, 2.5e6, R_TCVM_INF} ;

char   tss_current_mode_str [10] =   /* System Mode (EGA, TCVM, TCVM_FPEG,   */
                          {"TCVM"} ; /*   FPEG, PASSIVE                      */

int    tss_current_mode   = TCVM   ; /* System mode from enum structure      */

int    comm_ega_gun       =     0  ; /* Commanded EGA gun (1 or 2)           */

                                     /* Perveance for EGA guns (0 is science */
                                     /*   simulator value)                   */
double ega_perveance []   = {7.2e-6, 6.41e-6, 7.04e-6} ;

double comm_i_ega         = 0.000  ; /* Commanded EGA current 0.0<=I<=0.8 A  */

int    comm_fpeg_gun      = 0      ; /* Commanded FPEG gun 0, 1, 2 or 3      */

char   cmd_r_str [ 7]     =          /* Commanded TCVM load resistor (SHUNT, */
                          {"OPEN"} ; /*    R25K, R250K, r2500K)              */ 

int    comm_r_tcvm  =       OPEN   ; /* Commanded TCVM load resistor         */

double Rt           =       2000.0 ; /* Tether resistance in ohms            */

double a_e_orb      =         40.0 ; /* Eff. orbiter e collection area m^2   */

double a_i_orb      =         15.0 ; /* Eff. orbiter ion collection area m^2 */

double v_ion_ram    =          5.3 ; /* Ion ram velocity (in eV for O+)      */

double sat_radius   =          0.8 ; /* Satellite radius in meters           */

double teth_on_reel =      21618.3 ; /* Initial meters of tether on reel     */

double Jeo         ;                 /* Electron current density at orbiter  */
double Jio         ;                 /* Ion      current density at orbiter  */
double Ioeorb      ;                 /* Available electron current at orb    */
double Ioiorb      ;                 /* Available ion      current at orb    */
double Ioesat      ;                 /* Available electron current at sat    */

double itcm_sets  ;
double sa_score   ;
double tvmdc_sets ;
double v_tether   ;
double dv_dcore   ;
double vorb_spree ;
double dcbp_rete  ;
double Rtcvm      ;
double Iega       ;
double Ifpeg      ;
double v_sanity   ;

double reel_resfreq =          0.0 ; /* Resonance frequency of tether reel   */

double v_peak_trans =          0.0 ; /* Peak transient voltage (volts)       */

double v_peak_volt  =          0.0 ; /* Peak voltage (EMF+PK_TRANS) (Volts)  */
double v_peak_volt_n=          0.0 ; /* Peak voltage (EMF-PK_TRANS) (Volts)  */

Cartesian ixb_lvlh     ;             /* IxB tether drag in LVLH (Newtons/m)  */
double    ixb_lvlh_mag ;             /* Magnitude of IxB tether drag (N/m)   */
double    ixb_mag_tot  ;             /* Magnitude of IxB tether drag (N)     */

double    tss_power    ;             /* Power generated using TSS (Watts)    */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS tss_current_param_list [] =
{
   {"TSS_MODE"      , P_STR ,  tss_current_mode_str,
    "TSS collection mode (EGA,TCVM,TCVM_FPEG,FPEG,PASSIVE)       "},
   {"EGA_GUN"       , P_INT , &comm_ega_gun,
    "Commanded gun for DCORE EGA (1 or 2 or 0 for sci sim comp)  "},
   {"I_EGA"         , P_REAL, &comm_i_ega,
    "Commanded current for DCORE EGA (Amps) [0 <= Iega <= 0.8]   "},
   {"FPEG_GUN"      , P_INT , &comm_fpeg_gun,
    "Commanded gun for SETS FPEG (Amps) [0-none, 1, 2, 3-both]   "},
   {"R_TCVM"        , P_STR ,  cmd_r_str,
    "Commanded SETS load resistor [SHUNT, R25K, R250K, R2500K]   "},
   {"R_TETHER"      , P_REAL, &Rt,
    "Resistance of the full length of tether (ohms)              "},
   {"A_E_ORB"       , P_REAL, &a_e_orb,
    "Effective orbiter electron collection area (meters^2)       "},
   {"A_I_ORB"       , P_REAL, &a_i_orb,
    "Effective orbiter ion collection area (meters^2)            "},
   {"V_ION_RAM"     , P_REAL, &v_ion_ram,
    "Ion ram velocity (in eV for O+)                             "},
   {"SAT_RADIUS"    , P_REAL, &sat_radius,
    "Satellite radius (meters)                                   "},
   {"TETH_ON_REEL"  , P_REAL, &teth_on_reel,
    "Meters of tether on reel initially (meters)                 "}
} ;

struct OUTPUT_VARS tss_current_outvar_list [] =
{
   {"JOE_ORB"        , 5, " %10.7f"    , "     J_e_o", &(Jeo), 1.0,
    "Available electron current density at orbiter (Amps/meter^2)",
    "J|-eo|= (A/m|+2|=)", P_REAL, NULL},
   {"JOI_ORB"        , 5, " %10.7f"    , "     J_i_o", &(Jio), 1.0,
    "Available ion      current density at orbiter (Amps/meter^2)",
    "J|-io|= (A/m|+2|=)", P_REAL, NULL},
   {"IOE_ORB"        , 5, " %10.7f"    , "   Ioe_orb", &(Ioeorb), 1.0,
    "Available electron current at orbiter (Amps)",
    "I|-0e_orbiter|= (A)", P_REAL, NULL},
   {"IOI_ORB"        , 5, " %10.7f"    , "   Ioi_orb", &(Ioiorb), 1.0,
    "Available ion      current at orbiter (Amps)",
    "I|-0i_orbiter|= (A)", P_REAL, NULL},
   {"IOE_SAT"        , 5, " %10.7f"    , "   Ioe_sat", &(Ioesat), 1.0,
    "Available electron current at satellite (Amps)",
    "I|-0e_sat|= (A)", P_REAL, NULL},
   {"ITCM_SETS"      , 4, " %10.7f"    , "  Itcm_(A)", &(itcm_sets), 1.0,
    "Tether current as measured by the SETS TCM instrument (Amps)",
    "TCM Current (Amps)", P_REAL, NULL},
   {"SA_SCORE"       , 2, " %10.7f"    , " Sat_I_(A)", &(sa_score), 1.0,
    "Tether current as measured by the SCORE SA instrument (Amps)",
    "SCORE SA Current (Amps)", P_REAL, NULL},
   {"TVMDC_SETS"     , 5, " %10.4f"    , " TVMDC_(V)", &(tvmdc_sets), 1.0,
    "Tether voltage as measured by SETS TVMDC channel (Volts)",
    "TVMDC (Volts)", P_REAL, NULL},
   {"V_TETHER"       , 5, " %10.4f"    , " V_teth_(V)", &(v_tether), 1.0,
    "Voltage drop across tether (Itcvm * Rtether) (Volts)",
    "V|-tether|= (Volts)", P_REAL, NULL},
   {"DV_DCORE"     , 2, " %10.4f"    , "    DV_(V)", &(dv_dcore), 1.0,
    "Tether voltage as measured by DCORE DV instrument (Volts)",
    "DV DCORE (Volts)", P_REAL, NULL},
   {"VORB_SPREE"   , 4, " %10.4f"    , "  Vorb_(V)", &(vorb_spree), 1.0,
    "Orbiter potential as measured by SPREE (Volts)",
    "SPREE Vorb (Volts)", P_REAL, NULL},
   {"DCBP_RETE"    , 4, " %10.4f"    , "  Vsat_(V)", &(dcbp_rete), 1.0,
    "Satellite potential as measured by RETE (Volts)",
    "RETE Vsat (Volts)", P_REAL, NULL},
   {"R_TCVM"   , 5, " %10.4e"    , " Rld_(ohm)", &(Rtcvm), 1.0,
    "Load resistor connected to the tether circuit by SETS (Ohms)",
    "SETS R|-load|= (Ohms)", P_REAL, NULL},
   {"I_EGA"        , 5, " %11.6f"    , "  I_EGA (A)", &(Iega), 1.0,
    "Actual current being emitted by the EGA (Amps)",
    "DCORE I|-EGA|= (Amps)", P_REAL, NULL},
   {"I_FPEG"       , 5, " %11.6f"    , " I_FPEG (A)", &(Ifpeg), 1.0,
    "Actual current being emitted by the FPEG (Amps)",
    "SETS I|-FPEG|= (Amps)", P_REAL, NULL},
   {"V_SANITY"     , 5, " %10.4f"    , "  V_sanity", &(v_sanity), 1.0,
    "This value should be zero for solution to be valid (Volts)",
    "Sanity Check (Volts)", P_REAL, NULL},
   {"V_PK_TRANS"   , 6, " %11.4f"    , " V_pk_trans", &(v_peak_trans), 1.0,
    "Peak Switching Transient Voltage (Volts)",
    "Peak Transient Voltage (Volts)", P_REAL, NULL},
   {"PEAK_VOLT"    , 6, " %11.4f"    , "    V_peak", &(v_peak_volt), 1.0,
    "Peak Switching Voltage (EMF+Transient) (Volts)",
    "Peak Voltage (EMF+Trans) (Volts)", P_REAL, NULL},
   {"PEAK_N_VOLT"  , 6, " %11.4f"    , "  V_n_peak", &(v_peak_volt_n), 1.0,
    "Peak Switching Voltage (EMF-Transient) (Volts)",
    "Peak Voltage (EMF-Trans) (Volts)", P_REAL, NULL},
   {"REEL_RESFREQ" , 7, " %12.4f"    , " ResFreq(Hz)", &(reel_resfreq), 1.0,
    "Tether Reel Resonance Frequence (Hertz)",
    "Reel Resonance Freqency (Hz)", P_REAL, NULL},
   {"IXB_X_LVLH"     , 5, " %13.4e"    , " IxB0_x_(N/m)" , &(ixb_lvlh.X), 1.0,
    "X component of generated IxB force in LVLH (Newtons/meter)",
    "(IxB|-0|=)|-X|= (N/m)", P_REAL, NULL},
   {"IXB_Y_LVLH"     , 5, " %13.4e"    , " IxB0_y_(N/m)" , &(ixb_lvlh.Y), 1.0,
    "Y component of generated IxB force in LVLH (Newtons/meter)",
    "(IxB|-0|=)|-Y|= (N/m)", P_REAL, NULL},
   {"IXB_Z_LVLH"     , 5, " %13.4e"    , " IxB0_z_(N/m)" , &(ixb_lvlh.Z), 1.0,
    "Z component of generated IxB force in LVLH (Newtons/meter)",
    "(IxB|-0|=)|-Z|= (N/m)", P_REAL, NULL},
   {"IXB_MAG"        , 5, " %13.4e"    , " |IxB0|_(N/m)" , &(ixb_lvlh_mag), 1.0,
    "Magnitude of generated IxB force (Newtons/meter)",
    "||IxB|-0|=|| (N/m)", P_REAL, NULL},
   {"TOT_IXB_MAG"    , 5, " %13.4e"    , "   |IxB0|_(N)" , &(ixb_mag_tot), 1.0,
    "Magnitude of generated IxB force (Newtons)",
    "||IxB|-0|=|| (N)", P_REAL, NULL},
   {"TSS_POWER"      , 9, " %13.4e"    , " TSS_Power(W)" , &(tss_power), 1.0,
    "Amount of power generated by tether (Watts)",
    "TSS Power (Watts)", P_REAL, NULL}
} ;

#else

extern char   tss_mode_str      [] [10] ;
extern char   tcvm_resistor_str [] [ 7] ;
extern double tcvm_resistor_value    [] ;

extern char   tss_current_mode_str [  ] ;

extern int    tss_current_mode          ;

extern int    comm_ega_gun              ;

extern double ega_perveance        [  ] ;
extern double comm_i_ega                ;

extern int    comm_fpeg_gun             ;

extern char   cmd_r_str            [  ] ;

extern int    comm_r_tcvm               ;

extern double Rt                        ;

extern double a_e_orb                   ;

extern double a_i_orb                   ;

extern double v_ion_ram                 ;

extern double sat_radius                ;

extern double teth_on_reel              ;

extern double Jeo         ;
extern double Jio         ;
extern double Ioeorb      ;
extern double Ioiorb      ;
extern double Ioesat      ;

extern double itcm_sets  ;
extern double sa_score   ;  
extern double tvmdc_sets ;
extern double v_tether   ;
extern double dv_dcore   ;
extern double vorb_spree ;
extern double dcbp_rete  ;
extern double Rtcvm      ;
extern double Iega       ;
extern double Ifpeg      ;
extern double v_sanity   ;
extern double v_peak_trans ;
extern double v_peak_volt  ;
extern double v_peak_volt_n;
extern double reel_resfreq ;

extern Cartesian ixb_lvlh     ;
extern double    ixb_lvlh_mag ;
extern double    ixb_mag_tot  ;

extern double    tss_power    ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if _NO_PROTO_

void init_tss_current_params () ;

void compute_tss_current () ;

#else

void init_tss_current_params () ;

void compute_tss_current () ;

#endif
