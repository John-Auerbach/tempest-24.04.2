/*****************************************************************************/
/*                                                                           */
/*   Module:    iri.h                                                        */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the IRI Module.           */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h".                                */
/*                                                                           */
/*   History:   21_Mar_94 NRV   Written.                                     */
/*                                                                           */
/*****************************************************************************/



#ifdef TEMPEST_MAIN

int    iri_b0_from_table = TRUE ;  /* Get B0 from table vs. Gulyeava-1987    */

int    iri_ccir_f2peak   = TRUE ;  /* Get F2 peak from CCIR-67 vs. URSI-89   */

double iri_set_ne  = -1.0 ;        /* Electron density to be set (#/m^3)     */
double iri_set_te  = -1.0 ;        /* Electron temperature to be set (deg K) */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double iri_elect_density       ;   /* Electron density (#/m^3)               */

double iri_elect_temp          ;   /* Electron temperature (degrees Kelvin)  */

double iri_neutral_temp        ;   /* Neutral temperature from CIRA 86 deg K */

double iri_ion_density         ;   /* Total ion density (#/m^3)              */

double iri_ion_temp            ;   /* Ion temperature (degrees Kelvin)       */

double iri_rel_perc_o_ion      ;   /* Relative % of atomic axygen ions       */
double iri_rel_perc_h_ion      ;   /* Relative % of hydrogen      ions       */
double iri_rel_perc_he_ion     ;   /* Relative % of helium        ions       */
double iri_rel_perc_o2_ion     ;   /* Relative % of molec. oxygen ions       */
double iri_rel_perc_no_ion     ;   /* Relative % of nitrous oxide ions       */

double iri_density_o_ion       ;   /* Density of atomic axygen  ions (#/m^3) */
double iri_density_h_ion       ;   /* Density of hydrogen       ions (#/m^3) */
double iri_density_he_ion      ;   /* Density of helium         ions (#/m^3) */
double iri_density_o2_ion      ;   /* Density of molec. oxygen  ions (#/m^3) */
double iri_density_no_ion      ;   /* Density of nitrous oxide  ions (#/m^3) */

double iri_ne_end ;
double iri_te_end ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS iri_param_list [] =
{
   {"IRI_B0_TABLE"  , P_BOOL, &iri_b0_from_table,
    "Get B0 from table (else from Gulyeava 1987)"   },
   {"IRI_CCIR"      , P_BOOL, &iri_ccir_f2peak,
    "Get F2 peak from CCIR-67 (else URSI-89)"       },
   {"IRI_SET_NE"    , P_REAL, &iri_set_ne,
    "Set Ne to this value (do not use IRI to compute!) (#/m^3)"},
   {"IRI_SET_TE"    , P_REAL, &iri_set_te,
    "Set Te to this value (do not use IRI to compute!) (deg K)"},
} ;

struct OUTPUT_VARS iri_outvar_list [] =
{
   {"ELECT_DENSITY", 7, " %13.5e"   , "   Ne_(#/m^3)",&(iri_elect_density),1.0,
    "Electron density (particles/m^3)",
    "Electron Density N|-e|= (#/m|+3|=)", P_REAL, NULL},
   {"ELECT_TEMP_EV",12, " %10.6f"   , "   Te_(eV)",&(iri_elect_temp),8.62069e-5,
    "Electron temperature (eV)",
    "Electron Temperature T|-e|= (eV)", P_REAL, NULL}, 
   {"ELECT_TEMP"   , 7, " %13.2f"   , "       Te_(K)",&(iri_elect_temp),1.0,
    "Electron temperature (degrees Kelvin)",
    "Electron Temperature T|-e|= (|xCAK)", P_REAL, NULL}, 
   {"NEUTRAL_TEMP" , 9, " %13.2f"   , "       Tn_(K)",&(iri_neutral_temp),1.0,
    "Neutral temperature according to CIRA 86 (degrees Kelvin)",
    "Neutral Temperature T|-n|= (|xCAK)", P_REAL, NULL}, 
   {"ION_DENSITY"  , 5, " %13.5e"   , "   Ni_(#/m^3)",&(iri_ion_density),1.0,
    "Total ion density (particles/m^3)",
    "Ion Density N|-i|= (#/m|+3|=)", P_REAL, NULL}, 
   {"ION_TEMP"     , 5, " %13.2f"   , "       Ti_(K)",&(iri_ion_temp),1.0,
    "Ion temperature (degrees Kelvin)",
    "Ion Temperature T|-i|= (|xCAK)", P_REAL, NULL}, 
   {"O+_REL_DENS"  , 3, " %11.7f"   ," O+_(%ions)",&(iri_rel_perc_o_ion),1.0,
    "Relative density of atomic oxygen ions    (% total ions)",
    "Relative Density of O|++|= (% ions)", P_REAL, NULL}, 
   {"H+_REL_DENS"  , 3, " %11.7f"   ," H+_(%ions)",&(iri_rel_perc_h_ion),1.0,
    "Relative density of hydrogen ions         (% total ions)",
    "Relative Density of H|++|= (% ions)", P_REAL, NULL}, 
   {"HE+_REL_DENS" , 4, " %11.7f"   ,"He+_(%ions)",&(iri_rel_perc_he_ion),1.0,
    "Relative density of helium ions           (% total ions)",
    "Relative Density of He|++|= (% ions)", P_REAL, NULL}, 
   {"O2+_REL_DENS" , 4, " %11.7f"   ,"O2+_(%ions)",&(iri_rel_perc_o2_ion),1.0,
    "Relative density of molecular oxygen ions (% total ions)",
    "Relative Density of O|-2|=|++|= (% ions)", P_REAL, NULL}, 
   {"NO+_REL_DENS" , 4, " %11.7f"   ,"NO+_(%ions)",&(iri_rel_perc_no_ion),1.0,
    "Relative density of nitrous oxide ions    (% total ions)",
    "Relative Density of NO|++|= (% ions)", P_REAL, NULL}, 
   {"O+_DENSITY"   , 3, " %12.5e"   ,"  O+_(#/m^3)",&(iri_density_o_ion),1.0,
    "Density of atomic oxygen ions    (particles/m^3)",
    "Density of O|++|= (#/m|+3|=)", P_REAL, NULL}, 
   {"H+_DENSITY"   , 3, " %12.5e"   ,"  H+_(#/m^3)",&(iri_density_h_ion),1.0,
    "Density of hydrogen ions         (particles/m^3)",
    "Density of H|++|= (#/m|+3|=)", P_REAL, NULL}, 
   {"HE+_DENSITY"  , 4, " %12.5e"   ," He+_(#/m^3)",&(iri_density_he_ion),1.0,
    "Density of helium ions           (particles/m^3)",
    "Density of He|++|= (#/m|+3|=)", P_REAL, NULL}, 
   {"O2+_DENSITY"  , 4, " %12.5e"   ," O2+_(#/m^3)",&(iri_density_o2_ion),1.0,
    "Density of molecular oxygen ions (particles/m^3)",
    "Density of O|-2|=|++|= (#/m|+3|=)", P_REAL, NULL}, 
   {"NO+_DENSITY"  , 4, " %12.5e"   ," NO+_(#/m^3)",&(iri_density_no_ion),1.0,
    "Density of nitrous oxide ions    (particles/m^3)",
    "Density of NO|++|= (#/m|+3|=)", P_REAL, NULL},
   {"NE_END"       , 6, " %14.5e"   ," End_Ne_(#/m^3)",&(iri_ne_end),1.0,
    "Electron density @ tether end (particles/m^3)",
    "Electron Density N|-e|= @ end (#/m|+3|=)", P_REAL, NULL},
   {"TE_EV_END"    , 7, " %12.6f"   , " End_Te_(eV)",&(iri_te_end),8.62069e-5,
    "Electron temperature @ end (eV)",
    "Electron Temperature T|-e|= @ end (eV)", P_REAL, NULL}, 
   {"TE_END"       , 5, " %13.2f"   , "   End_Te_(K)",&(iri_te_end),1.0,
    "Electron temperature @ end (degrees Kelvin)",
    "Electron Temperature T|-e|= @ end (|xCAK)", P_REAL, NULL}
} ;

#else

extern int    iri_b0_from_table   ;
extern int    iri_ccir_f2peak     ;

extern double iri_set_ne          ;
extern double iri_set_te          ;

extern double iri_elect_density   ;
extern double iri_elect_temp      ;

extern double iri_neutral_temp    ;

extern double iri_ion_density     ;
extern double iri_ion_temp        ;

extern double iri_rel_perc_o_ion  ;
extern double iri_rel_perc_h_ion  ;
extern double iri_rel_perc_he_ion ;
extern double iri_rel_perc_o2_ion ;
extern double iri_rel_perc_no_ion ;

extern double iri_density_o_ion   ;
extern double iri_density_h_ion   ;
extern double iri_density_he_ion  ;
extern double iri_density_o2_ion  ;
extern double iri_density_no_ion  ;

extern PARAM  iri_param_list  []  ;
extern OUTVAR iri_outvar_list []  ;

extern double iri_ne_end ;
extern double iri_te_end ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _NO_PROTO_

void init_iri_ionosphere    () ;

void compute_iri_ionosphere () ;

#else

void init_iri_ionosphere    () ;

void compute_iri_ionosphere () ;

#endif

