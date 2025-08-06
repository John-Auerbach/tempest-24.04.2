/*****************************************************************************/
/*                                                                           */
/*   Module:    solarmag.h                                                   */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the SOLARMAG Module.      */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h".                                */
/*                                                                           */
/*   History:   12_Mar_94 NRV   Written.                                     */
/*                                                                           */
/*****************************************************************************/

typedef struct SOLAR_DAT_STRUCT
{
   int   month        ;
   int   year         ;
   int   nomi_f_mean  ;
   int   nomi_f_p2sig ;
   int   nomi_f_m2sig ;
   int   nomi_ap      ;
} SOLAR_DAT ;

static int days_in_month [2] [13] = {
            { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
            { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}} ;

                                   /* Based on equation (4) from             */
                                   /*    A.C. Layden et. al. in              */
                                   /*    Solar Physics 132:1-40, 1991.       */
                                   /*  F10.7 = 60.743 (+/-0.315) +           */
                                   /*    0.88042 (+/-0.00687) * Rz           */
                                   /*  rms scatter=7.95 and rho=0.987        */
#define RZ_FROM_F107(f)     ((f-60.743)/0.88042)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef TEMPEST_MAIN

SOLAR_DAT *schatten_data       ;   /* Array of predicted solar magnitude     */
int        size_schatten_data  ;   /* Number of elements of predicted data   */

SOLAR_DAT *historical_data      ;  /* Array of predicted solar magnitude     */
int        size_historical_data ;  /* Number of elements of predicted data   */

double solar_const_in = 1358.0 ;   /* Average solar constant (W/m^2)         */

double earth_albedo   =    0.3 ;   /* Earth reflected solar albedo           */

double earth_ir_flux  =  237.0 ;   /* Earth emitted infrared flux (W/m^2)    */

double set_f107_daily   = -1.0 ;   /* Forced F10.7 daily flux value          */
double set_f107_3mo_ave = -1.0 ;   /* Forced F10.7 3 month average flux val  */
double set_mag_ind_ap   = -1.0 ;   /* Forced value for magnetic index Ap     */

int    solar_data_hist  =  0   ;   /* Is solar data historical? T/F = 1/0    */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double zurich_ssn_r   = 100.0  ;   /* Daily Zurich Sunspot Number R          */

double f107_daily     = 150.0  ;   /* Daily f10.7 flux                       */

double f107_3mo_ave   = 150.0  ;   /* Three month running average of F10.7   */

double mag_ind_ap     =   4.0  ;   /* Daily magnetic index Ap                */

double solar_constant =   0.0  ;   /* Solar constant for analyses (W/m^2)    */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS  solarmag_param_list [] =
{
   {"SET_F107_DAILY" , P_REAL, &set_f107_daily,
    "Forced ouput of daily F10.7 solar flux value            "},
   {"SET_F107_3M_AVE", P_REAL, &set_f107_3mo_ave,
    "Forced ouput of daily F10.7 solar flux value            "},
   {"SET_MAG_IND_AP" , P_REAL, &set_mag_ind_ap,
    "Forced ouput of daily magnetic index Ap                 "},
   {"SOLAR_CONSTANT" , P_REAL, &solar_const_in,
    "Mean solar intensity observed at Earth (dflt=1358 W/m^2)"},
   {"EARTH_ALBEDO"   , P_REAL, &earth_albedo,
    "Fraction of Earth reflected solar flux-albedo (dflt=0.3)"},
   {"EARTH_IR"       , P_REAL, &earth_ir_flux,
    "Infrared energy flux emitted from Earth (dflt=237 W/m^2)"}
} ;

struct OUTPUT_VARS solarmag_outvar_list [] =
{
   {"F107_DAILY"   , 4, " %14.3f"   ,"    F10.7_curr", &(f107_daily), 1.0,
    "Solar F10.7 flux of current day",
    "Daily F|-10.7|=", P_REAL, NULL},
   {"F107_AVE_3MO" , 6, " %14.3f"   ," F10.7_3mo_Ave", &(f107_3mo_ave), 1.0,
    "Three month running average of solar F10.7 flux",
    "F|-10.7|= 3 month ave", P_REAL, NULL},
   {"SSN_RZ"       , 3, " %13.3f"   , " Zurich_SSN_R", &(zurich_ssn_r), 1.0,
    "Zurich Sunspot Number R",
    "Zurich SSN R|-z|=", P_REAL, NULL},
   {"AP"           , 2, " %13.3f"   , "   Mag_Ind_Ap", &(mag_ind_ap), 1.0,
    "Daily average planetary magnetic index Ap (linear)",
    "Magnetic Index A|-p|=", P_REAL, NULL},
   {"SOLAR_FLUX"   , 7, " %13.3f"   , "   Solar_Flux", &(solar_constant),1.0,
    "Direct solar flux, also known as the solar constant (W/m^2)",
    "Solar Flux (W/m|+2|=)", P_REAL, NULL},
   {"SOLAR_DATA_H" , 8, " %18d"   , " Solar_Data_Hist?", &(solar_data_hist),1.0,
    "Is solar output data historical? (Boolean 1=YES, 0=NO)",
    "Solar Data Historical?", P_INT, NULL}
} ;

#else

extern SOLAR_DAT *schatten_data       ;
extern int        size_schatten_data  ;

extern SOLAR_DAT *historical_data      ;
extern int        size_historical_data ;

extern double solar_const_in ;
extern double earth_albedo   ;
extern double earth_ir_flux  ;

extern double set_f107_daily   ;
extern double set_f107_3mo_ave ;
extern double set_mag_ind_ap   ;

extern int solar_data_hist ;

extern double f107_daily     ;
extern double f107_3mo_ave   ;
extern double zurich_ssn_r   ;
extern double mag_ind_ap     ;

extern double solar_constant ;

extern PARAM  solarmag_param_list  []  ;
extern OUTVAR solarmag_outvar_list []  ;


#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _NO_PROTO_

void init_solarmag_indices    () ;

void compute_solarmag_indices () ;

void md_from_day () ;

#else

void init_solarmag_indices    () ;

void compute_solarmag_indices () ;

void md_from_day (int day, int year, int *month, int *dom, int *dim) ;

#endif
