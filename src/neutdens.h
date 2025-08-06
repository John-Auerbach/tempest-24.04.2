/*****************************************************************************/
/*                                                                           */
/*   Module:    neutdens.h                                                   */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the NEUTDENS Module.      */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h".                                */
/*                                                                           */
/*   History:   26_Mar_93 NRV   Written.                                     */
/*                                                                           */
/*****************************************************************************/



#ifdef TEMPEST_MAIN

                                   /* Where this is an option, use MSISE-90  */
int neutdens_prefer_msise_90 = FALSE ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double neutdens_numb_he        ;   /* Number density of neutral He    #/m^3  */

double neutdens_numb_o         ;   /* Number density of neutral O     #/m^3  */

double neutdens_numb_n2        ;   /* Number density of neutral N2    #/m^3  */

double neutdens_numb_o2        ;   /* Number density of neutral O2    #/m^3  */

double neutdens_numb_ar        ;   /* Number density of neutral Ar    #/m^3  */

double neutdens_numb_h         ;   /* Number density of neutral H     #/m^3  */

double neutdens_numb_n         ;   /* Number density of neutral N     #/m^3  */

double neutdens_tot_mass       ;   /* Total neutrals mass densigy kg/m^3     */

double cum_flux_ao             ;   /* Cumulative fluence of neutral O #/m^2  */

double neutdens_temp_exos      ;   /* Exospheric temperature  (deg Kelvin)   */

double neutdens_temp_atalt     ;   /* Temperature at altitude (deg Kelvin)   */

char   neutdens_model      [8] ;   /* Which neutral density model was used?  */

char   neutdens_msis86      [] = " MSIS-86" ;
char   neutdens_msise90     [] = "MSISE-90" ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS neutdens_param_list [] =
{
   {"PREFER_MSISE90", P_BOOL , &neutdens_prefer_msise_90,
    "Use MSISE-90 where possible (altitude < 400km)? else MSIS-86"}
} ;

struct OUTPUT_VARS neutdens_outvar_list [] =
{
   {"TEMP_EXOS"    , 6, " %13.4f"   ,"     Texo_(K)",&(neutdens_temp_exos),1.0,
    "Exospheric temperature (degrees Kelvin)",
    "Exospheric Temperature T|-exosph|= (|xCAK)", P_REAL, NULL},
   {"TEMP_NEUTRAL" , 6, " %13.4f"   ,"       Tn_(K)",&(neutdens_temp_atalt),1.0,
    "Neutral temperature (degrees Kelvin)",
    "Neutral Temperature T|-n|= (|xCAK)", P_REAL, NULL},
   {"HE_DENSITY"   , 4, " %12.4e"   ,"  He_(#/m^3)",&(neutdens_numb_he),1.0,
    "Density of neutral helium (particles/m^3)",
    "Density of He (#/m|+3|=)", P_REAL, NULL},
   {"O_DENSITY"    , 3, " %12.4e"   ,"   O_(#/m^3)",&(neutdens_numb_o ),1.0,
    "Density of neutral atomic oxygen (particles/m^3)",
    "Density of O (#/m|+3|=)", P_REAL, NULL},
   {"N2_DENSITY"   , 4, " %12.4e"   ,"  N2_(#/m^3)",&(neutdens_numb_n2),1.0,
    "Density of neutral nitrogen (particles/m^3)",
    "Density of N|-2|= (#/m|+3|=)", P_REAL, NULL},
   {"O2_DENSITY"   , 4, " %12.4e"   ,"  O2_(#/m^3)",&(neutdens_numb_o2),1.0,
    "Density of neutral oxygen (particles/m^3)",
    "Density of O|-2|= (#/m|+3|=)", P_REAL, NULL},
   {"AR_DENSITY"   , 4, " %12.4e"   ,"  Ar_(#/m^3)",&(neutdens_numb_ar),1.0,
    "Density of argon (particles/m^3)",
    "Density of Ar (#/m|+3|=)", P_REAL, NULL},
   {"H_DENSITY"    , 3, " %12.4e"   ,"   H_(#/m^3)",&(neutdens_numb_h ),1.0,
    "Density of neutral atomic hydrogen (particles/m^3)",
    "Density of H (#/m|+3|=)", P_REAL, NULL},
   {"N_DENSITY"    , 3, " %12.4e"   ,"   N_(#/m^3)",&(neutdens_numb_n ),1.0,
    "Density of neutral atomic nitrogen (particles/m^3)",
    "Density of N (#/m|+3|=)", P_REAL, NULL},
   {"AO_FLUX" ,      4, " %12.4e"   ,"  AO_(#/m^2)",&(cum_flux_ao),1.0,
    "Cumulative Atomic Oxygen Flux (particles/m^2)",
    "Cumulative AO Flux (#/m|+2|=)", P_REAL, NULL},
   {"MASS_DENSITY" , 6, " %14.4e"   ," Neut_(kg/m^3)",&(neutdens_tot_mass),1.0,
    "Total mass density of neutrals (kg/m^3)",
    "Density of Neutrals (kg/m|+3|=)", P_REAL, NULL},
   {"MODEL_NEUTD" , 7,    " %12s"   ,"  Neut_model", neutdens_model,1.0,
    "Which neutral densuty model was used",
    "Neutral density model", P_STR, NULL},
} ;

#else

extern int neutdens_prefer_msise_90   ;

extern double neutdens_numb_he        ;
extern double neutdens_numb_o         ;
extern double neutdens_numb_n2        ;
extern double neutdens_numb_o2        ;
extern double neutdens_numb_ar        ;
extern double neutdens_numb_h         ;
extern double neutdens_numb_n         ;

extern double cum_flux_ao             ;

extern double neutdens_tot_mass       ;

extern double neutdens_temp_exos      ;
extern double neutdens_temp_atalt     ;

extern char   neutdens_model       [] ;

extern char   neutdens_msis86      [] ;
extern char   neutdens_msise90     [] ;

extern PARAM  neutdens_param_list  [] ;
extern OUTVAR neutdens_outvar_list [] ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _NO_PROTO_

void init_neutral_densities    () ;

void compute_neutral_densities () ;

#else

void init_neutral_densities    () ;

void compute_neutral_densities () ;

#endif
