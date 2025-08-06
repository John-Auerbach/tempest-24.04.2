
/*****************************************************************************/
/*                                                                           */
/*   Module:    temputil.h                                                   */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the TEMPUTIL rourintes    */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and module header files.        */
/*                                                                           */
/*   History:   01_Feb_94 NRV   Rewritten.                                   */
/*                                                                           */
/*   RCS:       $Id: temputil.h,v 1.26 2000/01/09 17:49:10 nestorv Exp nrv $                                                         */
/*                                                                           */
/*              $Log: temputil.h,v $
 *              Revision 1.26  2000/01/09 17:49:10  nestorv
 *              Increased number of parameters.
 *
 * Revision 1.25  1999/10/20  03:21:12  nestorv
 * Changed values in SOLARMAG module to allow for parameters.
 *
 * Revision 1.24  1999/03/10  05:15:34  nestorv
 * added DEBUG_BARE_TETHER_INTEG
 *
 * Revision 1.23  1999/01/27  04:53:28  nestorv
 * Increased MAX_PARAMS to 120
 *
 * Revision 1.22  1998/06/26  20:32:17  nestorv
 * Added more information to the BARE TETHER desciption section.
 *
 * Revision 1.21  1997/05/09  19:59:10  nestorv
 * Added MOD_BARE_TETHER.
 *
 * Revision 1.20  1997/05/09  15:39:37  nestorv
 * Increased MAX_PARAMS to 100.
 *
 * Revision 1.19  1997/05/09  15:37:18  nestorv
 * Making arrays print
 *
 * Revision 1.18  1997/05/09  15:18:10  nestorv
 * Fixed BARE_TETHER addition.
 *
 * Revision 1.17  1997/05/09  14:13:02  nestorv
 * Added DEBUG_BARE_TETHER.
 *
 * Revision 1.16  1997/05/09  13:54:16  nestorv
 * Fixed adding BARE_TETHER.
 *
 * Revision 1.15  1997/05/09  13:44:59  nestorv
 * Added BARE_TETHER module.
 *
 * Revision 1.14  1996/07/03  20:56:32  nestorv
 * Added init subroutine to EMF module.
 *
 * Revision 1.13  1996/03/29  22:48:44  nestorv
 * Made provisions for comments (binarified and ASCII modes.)
 *
 * Revision 1.12  1995/11/22  00:44:28  nestorv
 * Added variable for -GSETIME command line flag.
 *
 * Revision 1.11  1995/11/06  21:08:40  nestorv
 * Added TEMPEST_TTPS environtal variable for path for TTPs.
 *
 * Revision 1.10  1995/11/02  18:04:02  nestorv
 * Increased number of TTPs.
 *
 * Revision 1.9  1995/11/01  05:24:03  nestorv
 * Increased MAX_OUTPUTS to 200.
 *
 * Revision 1.8  1995/10/27  07:04:26  nestorv
 * Added PLASMA module.
 *
 * Revision 1.7  1995/10/27  05:58:51  nestorv
 * Added declaration for sort_ttag_param_entries ().
 *
 * Revision 1.6  1995/10/09  00:34:08  nestorv
 * Updated to provide for correct min/max adjustment.
 *
 * Revision 1.5  1995/10/07  18:44:48  nestorv
 * Added entry for print_zero_results ().
 *
 * Revision 1.4  1995/10/07  18:12:54  nestorv
 * Added -binarify option to produce binarified output for plotl.
 *
 * Revision 1.3  1995/10/06  04:42:40  nestorv
 * Added boolean for plotl binarify output.
 *
 * Revision 1.2  1995/10/04  19:51:43  nestorv
 * Added time-tagged parameter file capability.
 *                                                        */
/*                                                                           */
/*****************************************************************************/



#define DEF_DEBUG_OUT    stderr
#define DEF_SPARM_OUT    stdout
#define DEF_SIMUL_OUT    stdout
#define DEF_PLOTL_OUT    stdout
#define DEF_EXTRE_OUT    stdout

#define MAX_PARAMS          200
#define MAX_OUTPUTS         250
#define MAX_TTAG_ENTRIES   4096
#define MAX_TTAG_PARAMS      20

#define TTG_BY_MET		16384
#define TTG_BY_GMT		32768

#define DEBUG_MODULES		0x0001
#define DEBUG_GENERAL		0x0002
#define DEBUG_GENORBIT		0x0004
#define DEBUG_TETHER		0x0008
#define DEBUG_BFIELD		0x0010
#define DEBUG_EMF		0x0020
#define DEBUG_SOLARMAG		0x0040
#define DEBUG_IRI		0x0080
#define DEBUG_NEUTDENS		0x0100
#define DEBUG_TSS_CURRENT	0x0200
#define DEBUG_PLASMA		0x0400
#define DEBUG_BARE_TETHER	0x0800
#define DEBUG_BARE_TETHER_INTEG 0x0800
#define DEBUG_HCPC		0x1000
#define DEBUG_SPARE5		0x2000
#define DEBUG_TEMPUTIL		0x4000
#define DEBUG_TTAG		0x8000

enum {	MOD_GENERAL , MOD_GLOBAL  , MOD_GENORBIT, MOD_TETHER  , MOD_BFIELD  , 
 	MOD_EMF     , MOD_SOLARMAG, MOD_IRI     , MOD_NEUTDENS,
	MOD_TSS_CURRENT, MOD_PLASMA, MOD_BARE_TETHER } ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef TEMPEST_MAIN 

char  *data_path       = NULL  ;    /* Path from environmental variable      */
                                    /*   TEMPEST_DATA for data files etc.    */
char  *ttp_path        = NULL  ;    /* Path from environmental variable      */
                                    /*   TEMPEST_TTPS for TTP files          */

FILE  *debug_out       = NULL  ;    /* Default file for debugging output     */
FILE  *sparm_out       = NULL  ;    /* Default file for parameter display    */
FILE  *simul_out       = NULL  ;    /* Default file for simulation output    */
FILE  *plotl_out       = NULL  ;    /* Default file for plotl lable output   */
FILE  *extre_out       = NULL  ;    /* Default file for extrema output       */

char  *comments        = NULL  ;    /* Comments for binarified output        */

char   debug_fname [80] = {""} ;    /* Filename for debugging output         */
char   sparm_fname [80] = {""} ;    /* Filename for parameter output         */
char   simul_fname [80] = {""} ;    /* Filename for simulation output        */
char   plotl_fname [80] = {""} ;    /* Filename for plotl label output       */
char   extre_fname [80] = {""} ;    /* Filename for min/max output           */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int    show_params     = TRUE  ;    /* Boolean for parameter output          */
int    show_header     = TRUE  ;    /* Boolean for column header output      */
int    show_comments   = FALSE ;    /* Boolean to show comments              */
int    show_debug      = FALSE ;    /* Boolean for debugging output output   */
int    plotl_labels    = FALSE ;    /* Boolean for plotl label output        */
int    tab_delim       = FALSE ;    /* Boolean for tab or space delimiting   */
int    show_min_max    = FALSE ;    /* Boolean for extrema output            */
int    binarify        = FALSE ;    /* Boolean for plotl binarified output   */
int    gse_time        = FALSE ;    /* Boolean for GSETime output selection  */
int    first_minmax    = TRUE  ;    /* Boolean for first min/max compare     */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double min_val [MAX_OUTPUTS]   ;    /* Array containing minimum values       */
double max_val [MAX_OUTPUTS]   ;    /* Array containing maximum values       */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

MODULES module_list [] = {
  { "GENERAL"   , "General TEMPEST simulation parameters",
    "\
     This module contains variables for general top-level simulation control\n\
     including simulation type, simulation start/stop time and increments as\n\
     well as simulation output control.  This module allows one to output\n\
     mission elapsed time (MET), in addition to Greenwich Mean Time (GMT).\n",
    NULL, NULL,
    tempest_param_list,   sizeof (tempest_param_list )/sizeof (PARAM),
    tempest_outvar_list , sizeof (tempest_outvar_list)/sizeof(OUTVAR), TRUE },
  { "GLOBAL"    , "TEMPEST global simulation parameters",
    "\
     This module is used to generate simulations on a global scale.  This\n\
     is accomplished by varying latitude and longitude over the specified\n\
     ranges while holding all other input paramters fixed.\n",
     NULL, NULL,
     global_param_list , sizeof( global_param_list)/sizeof (PARAM),
     NULL, 0, FALSE },
  { "GENORBIT"  , "Generate position & velocity using Keplerian propagator",
    "Generate position & velocity using Keplerian orbit propagator\n",
    init_orbital_params, compute_sat_position,
    genorbit_param_list,   sizeof (genorbit_param_list )/sizeof (PARAM),
    genorbit_outvar_list , sizeof (genorbit_outvar_list)/sizeof(OUTVAR), TRUE },
  { "TETHER"    , "Compute tether libration using simple harmonic model",
    "Compute tether libration using simple harmonic model of tether\n\
     libration.  The in-plane period of oscillation is 1/sqrt(3) times the\n\
     orbital period and the out-of-plane libration is 1/2 of the orbital\n\
     period.  The tether L vector is displaced about the tether's 0.\n",
    init_libration_params, compute_tether_libration,
    tether_param_list,     sizeof (tether_param_list )/sizeof (PARAM),
    tether_outvar_list,    sizeof (tether_outvar_list)/sizeof(OUTVAR), FALSE },
  { "BFIELD"    , "Compute near-earth magnetic fields using IGRF-91",
    "\
     The International Geomagnetic Reference Field (IGRF) model is the\n\
     empirical representation of the Earth's magnetic field recommended for\n\
     scientific use by the International Association of Geomagnetism and\n\
     Aeronomy (IAGA).  The IGRF model represents the main (core) field\n\
     without external sources.  They employ the usual spherical harmonics\n\
     expansion of the scalar potential in geocentric coordinates.  The IGRF\n\
     model coefficients are based on all available data sources including\n\
     geomagnetic measurements from observatories, ships, aircraft and\n\
     satellites.\n\n\
     The IGRF-91 model consists of coefficient sets for the epochs 1945 to\n\
     1990 in steps of 5 years and the first time derivatives of the coeff-\n\
     icients for the time period of 1990 to 1995.  For the 5 year period in\n\
     between the models, linear interpolation is used.\n",
    init_bfield, compute_bfields,
    bfield_param_list,     sizeof (bfield_param_list)/sizeof(PARAM),
    bfield_outvar_list,    sizeof (bfield_outvar_list)/sizeof(OUTVAR), FALSE },
  { "EMF"       , "Compute induced EMF in the tether and double probes",
    "Compute the motionally induced EMF (using magnetic fields from IGRF)\n\
     in the tether and electic field double probes.\n",
    init_emfs, compute_emfs,
    emf_param_list,        sizeof (emf_param_list )/sizeof (PARAM),
    emf_outvar_list,       sizeof (emf_outvar_list)/sizeof(OUTVAR), FALSE },
  { "SOLARMAG"  , "Compute daily and average solar and magnetic indices",
    "\
     Compute daily and average solar and magnetic indices based on predicted\n\
     solar data generated by Kenneth H. Schatten of NASA/GSFC.  The values\n\
     generated are based on monthly mean smoothed estimates of future solar\n\
     radio flux (for references to prediction technique see Schatten and\n\
     Sofia Geophys. Res. Lett., 14, 632, 1987, or Schatten et al., Geophys.\n\
     Geophys. Res. Lett., 5, 411, 1978.  In addition, a measure of a weighted\n\
     planetary geomagnetic index ap is provided.  The data file used here\n\
     was generated on January 11, 1994 and has values from 1/94 through\n\
     11/2015.  The data  was interpolated using linear interpolation.\n",
    init_solarmag_indices, compute_solarmag_indices,
    solarmag_param_list , sizeof (solarmag_param_list )/sizeof(PARAM),
    solarmag_outvar_list, sizeof (solarmag_outvar_list)/sizeof(OUTVAR), FALSE },
  { "IRI"       , "Compute ionospheric parameters using the IRI-90 model",
    "\
     IRI-90 is the empirical reference model of ionospheric densities and\n\
     temperatures (electrons and ions) recommended for international use\n\
     by the Committee on Space Research (COSPAR) and the International Union\n\
     of Radio Science (URSI).  It was established in a world-wide collab-\n\
     oration starting in the late sixties.  IRI is updated bi-yearly during\n\
     special IRI workshops to incorporate newly available measurements.\n\
     IRI is based on all the important ionospheric data sources including\n\
     ground-based (ionosonde, absorption, incoherent scatter) and spacecraft\n\
     (Alouette, ISIS, AE, AEROS, DE, rockets) measurements.  IRI provides\n\
     monthly mean values for magnetically quiet conditions at non-auroral\n\
     latitudes in the altitude range 50k to 2000km.\n",
    init_iri_ionosphere, compute_iri_ionosphere, 
    iri_param_list,        sizeof (iri_param_list )/sizeof (PARAM),
    iri_outvar_list,       sizeof (iri_outvar_list)/sizeof(OUTVAR), FALSE },
  {"NEUTDENS"   , "Compute neutral atmosphere parameters using MSIS models",
    "\
     The Mass-Spectrometer-Incoherent-Scatter-1986 (MSIS-86) neutral\n\
     atmosphere model describes the neutral temperature and the densities\n\
     of He, O, N2, O2, Ar, H, and N. The MSIS model is based on the extensive\n\
     data compilation and analysis work of A. E. Hedin and his collaborators\n\
     [A. E. Hedin et al., J.  Geophys. Res. 82, 2139-2156, 1977; A. E.\n\
     Hedin, J. Geophys. Res. 88, 10170- 10188, 1983; A. E. Hedin, J. Geophys.\n\
     Res. 92,  4649, 1987]. MSIS-86 constitutes the upper part of the COSPAR\n\
     International Reference Atmosphere (CIRA-86).  MSIS is the primary model\n\
     used for oxygen calculations and is accurate in the altitude range of\n\
     85km through 1000km.\n\n\
     The MSISE-90 empirical model is named for the in situ Mass-Spectrometer-\n\
     Incoherent-Scatter measurements of atmospheric composition and\n\
     temperature.  These data were obtained from satellites, rocket probes,\n\
     and ground-based stations.  This sub-model version of MSIS is similar\n\
     to the model MSIS-86 except the data used by the model covers the\n\
     altitude of 0 km (sea level) to 400 km.\n",
    init_neutral_densities   , compute_neutral_densities,
    neutdens_param_list , sizeof (neutdens_param_list )/sizeof (PARAM),
    neutdens_outvar_list, sizeof (neutdens_outvar_list)/sizeof(OUTVAR), FALSE },
  { "TSS_CURRENT"  , "Compute current collection capabilities for TSS-1R",
    "\
     This module computes the current collection capabilitie of the TSS-1R\n\
     system based on the physical enviroment and the configuration of the\n\
     TSS-1R system.\n",
    init_tss_current_params, compute_tss_current,
    tss_current_param_list,   sizeof (tss_current_param_list )/sizeof (PARAM),
    tss_current_outvar_list , sizeof (tss_current_outvar_list)/sizeof(OUTVAR),
    FALSE },
  { "PLASMA"       , "Computes plasma parameters based on geophysical values",
    "\
     This module computes various plasma parameters based on the predicted\n\
     values of plasma density and temperatures from IRI, magnetic field\n\
     strenght from IGRF, and neutral atmosphere parameters from MSIS.\n",
     init_plasma_params, compute_plasma_params,
     plasma_param_list , sizeof (plasma_param_list )/sizeof (PARAM),
     plasma_outvar_list, sizeof (plasma_outvar_list)/sizeof(OUTVAR), FALSE },
  {"BARE_TETHER"   , "Computes current collection capabilities of bare tethers",
   "\
    This module computes the current coleection of both an upward and\n\
    downward bare or partially bare tether based on the physical environment\n\
    and end electrical properties.\n\
    There are multiple contactor models available that can be selected with\n\
    a model number: 0 - fixed cathode/anode bias, 1 - field emissive array,\n\
    2 - P-M sphere, 3 - v=P1*exp(P2*i)+P3, 4 - v=P1*I^P2+P3,\n\
    5 - bare tehter, 6 - thin cylinder, 7 - hollow cathode.\n",
    init_bare_tether_params, compute_bare_tether,
    bare_tether_param_list , sizeof (bare_tether_param_list)/sizeof (PARAM),
    bare_tether_outvar_list, sizeof (bare_tether_outvar_list)/sizeof(OUTVAR), 
    FALSE }
} ;

int NUM_MODULES = (sizeof (module_list) / sizeof (MODULES)) ; 

PARAM  *param_list   [MAX_PARAMS ] ;
int     num_params                 ;

OUTVAR *outvar_list  [MAX_OUTPUTS] ;
int     outvar_mod   [MAX_OUTPUTS] ;
int     num_outvars                ;

int     display_list [MAX_OUTPUTS] ;

int     num_ttag_entries = 0         ;
int     curr_ttag_entry  = 0         ;
int     num_ttag_params  = 0         ;
double  curr_ttag_entry_time         ;
int     ttag_param_index [MAX_TTAG_PARAMS] ;
void   *ttag_param_list  [MAX_TTAG_ENTRIES] [MAX_TTAG_PARAMS] ;

#else

extern char  *data_path        ;
extern char  *ttp_path         ;

extern FILE  *debug_out        ;
extern FILE  *sparm_out        ;
extern FILE  *simul_out        ;
extern FILE  *plotl_out        ;
extern FILE  *extre_out        ;

extern char   debug_fname []   ;
extern char   sparm_fname []   ;
extern char   simul_fname []   ;
extern char   plotl_fname []   ;
extern char   extre_fname []   ;

extern char  *comments         ;

extern int    show_params      ;
extern int    show_header      ;
extern int    show_comments    ;
extern int    show_debug       ;
extern int    plotl_labels     ;
extern int    tab_delim        ;
extern int    show_min_max     ;
extern int    binarify         ;
extern int    gse_time         ;
extern int    first_minmax     ;

extern double min_val []       ;
extern double max_val []       ;

extern MODULES module_list  [] ;
extern int     NUM_MODULES     ;

extern PARAM  *param_list   [] ;
extern int     num_params      ;

extern OUTVAR *outvar_list  [] ;
extern int     outvar_mod   [] ;
extern int     num_outvars     ;

extern int     display_list [] ;

extern int     num_ttag_entries             ;
extern int     curr_ttag_entry              ;
extern int     num_ttag_params              ;
extern double  curr_ttag_entry_time         ;
extern int     ttag_param_index []          ;
extern void   *ttag_param_list  [MAX_TTAG_ENTRIES] [MAX_TTAG_PARAMS] ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _NO_PROTO_

void  show_simulation_help   () ;
void  show_simulation_docs   () ;

void  make_lists             () ;

void  process_cmdline_params () ;
void  read_in_params         () ;
void  load_ttag_params       () ;
void  load_include_ttag      () ;
void  sort_ttag_param_entries () ;
void  set_param_ttag_entry   () ;
void  get_disp_var_list      () ;
void  set_parameter          () ;

void  init_file_output       () ;
void  close_file_output      () ;

void  adjust_min_max         () ;
void  print_min_max          () ;

void  show_parameters        () ;
void  show_headers           () ;
void  show_plotl_labels      () ;

char *strupper               () ;

void  print_sim_results      () ;
void  print_zero_results     () ;

#else

void  show_simulation_help   () ;
void  show_simulation_docs   (char *module_name) ;

void  make_lists             () ;

void  process_cmdline_params (int argc, char *argv []) ;
void  read_in_params         (char filename[]) ;
void  load_ttag_params       (char filename[]) ;
void  load_include_ttag      (char filename[],char incl_time0[],int time_type) ;
void  sort_ttag_param_entries () ;
void  set_param_ttag_entry   () ;
void  get_disp_var_list      (char *var_list) ;
void  set_parameter          (char *token, char *value) ;

void  init_file_output       () ;
void  close_file_output      () ;

void  adjust_min_max         () ;
void  print_min_max          () ;

void  show_parameters        () ;
void  show_headers           () ;
void  show_plotl_labels      () ;

char *strupper               (char *string) ;

void  print_sim_results      () ;
void  print_zero_results     () ;

#endif

