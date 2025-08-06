/*****************************************************************************/
/*                                                                           */
/*   Module:    plasma.h                                                     */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the PLASMA Module.        */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h".                                */
/*                                                                           */
/*   History:   27_Oct_95 NRV   Written.                                     */
/*                                                                           */
/*****************************************************************************/


#define K_BOLTZ 1.380658e-23       /* Boltzmann's constant k (Joules/Kelvin) */

#define Q_CHARGE  1.6023e-19       /* Charge of electron at rest (Coulombs)  */

#define EPSILON_0  8.864e-12       /* Permittivity of free space (Farad/mtr) */

#define ELECT_MASS 9.111e-31       /* Rest mass of an electron               */

#define RAD_TO_HZ (0.5/M_PI)       /* Conversion factor from rad/sec to Hz   */

#ifdef TEMPEST_MAIN

double ion_mass  = 2.678e-26  ;    /* Mass of the ion (O+ default) in kg     */

double perp_velocity = 1234.2018 ; 

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double gyro_radius_electron ;      /* Gyro radius for an electron (meters)   */
double gyro_radius_ion      ;      /* Gyro radius for an ion      (meters)   */

double debye_length         ;      /* Debye length (meters)                  */
double mean_free_path       ;      /* Mean free path (meters)                */

double mean_speed_electron  ;      /* Mean electron speed (meters/sec)       */
double mean_speed_ion       ;      /* Mean ion      speed (meters/sec)       */

double sound_speed_ion      ;      /* Ion sound speed in a plasma (meters/s) */

double plasma_freq_electron ;      /* Electron plasma frequency (rad/sec)    */
double plasma_freq_ion      ;      /* Ion      plasma frequence (rad/sec)    */

double gyro_freq_electron   ;      /* Electron gyro frequency   (rad/sec)    */
double gyro_freq_ion        ;      /* Ion      gyro frequency   (rad/sec)    */

double coll_freq_elect_ion  ;      /* Electron-Ion collision frequency       */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS plasma_param_list [] =
{

   {"ION_MASS"  , P_REAL, &ion_mass,
    "Ion mass for calculations (kg) [O+ default]"   }
} ;

struct OUTPUT_VARS plasma_outvar_list [] =
{
   {"ELECT_GYRO_RAD", 7, " %11.7f", "  E_gyr_rad",&(gyro_radius_electron),1.0,
    "Electron gyro radius (meters)",
    "Electron R|-L|= (meters)", P_REAL, NULL},
   {"ION_GYRO_RAD"  , 5, " %11.7f", "  I_gyr_rad",&(gyro_radius_ion), 1.0,
    "Ion gyro radius (meters)",
    "Ion R|-L|= (meters)", P_REAL, NULL},
   {"DEBYE_LEN"     , 5, " %11.8f", "  Debye_leni", &(debye_length), 1.0,
    "Debye length (meters)", 
    "Debye Length (m)", P_REAL, NULL},
   {"MEAN_FR_PATH",   7, " %11.5f", " Mean_fr_pth", &(mean_free_path), 1.0,
    "Mean Free Path (meters)",
    "Mean Free Path (m)", P_REAL, NULL},
   {"ELECT_SPD_MEAN", 8, " %11.5f", "  E_mean_spd", &(mean_speed_electron), 1.0,
    "Mean Electron Speed (meters/second)",
    "Mean Elect Speed (m/s)", P_REAL, NULL},
   {"ION_SPD_MEAN"  , 6, " %11.5f", "  I_mean_spd", &(mean_speed_ion), 1.0,
    "Mean Ion Speed (meters/second)",
    "Mean Ion Speed (m/s)", P_REAL, NULL},
   {"ION_SPD_SOUND" , 9, " %11.5f", " I_sound_spd", &(sound_speed_ion), 1.0,
    "Ion Sound Speed (meters/second)",
    "Ion Sound Speed (m/s)", P_REAL, NULL},
   {"ELECT_PL_FREQ" , 8, " %11.5e", " E_plasma_fr", &(plasma_freq_electron),
    0.5/PI,
    "Electron Plasma Frequency (Hz)",
    "Elect Plasma Freq (Hz)", P_REAL, NULL},
   {"ION_PL_FREQ"   , 6, " %11.5e", " I_plasma_fr", &(plasma_freq_ion),
    0.5/PI,
    "Ion Plasma Frequency (Hz)",
    "Ion Plasma Freq (Hz)", P_REAL, NULL},
   {"ELECT_GYR_FREQ", 8, " %11.5e", "   E_gyro_fr", &(gyro_freq_electron),
    0.5/PI,
    "Electron Gyro Frequency (Hz)",
    "Elect Gyro Freq (Hz)", P_REAL, NULL},
   {"ION_GYR_FREQ"  , 6, " %11.5e", "   I_gyro_fr", &(gyro_freq_ion),
    0.5/PI,
    "Ion Gyro Frequency (Hz)",
    "Ion Gyro Freq (Hz)", P_REAL, NULL},
   {"EI_COLL_FREQ"  , 7, " %11.5f", "  EI_coll_fr", &(coll_freq_elect_ion),
     0.5/PI,
    "Electron-Ion Collision Frequency (Hz)",
    "Elect-Ion Coll Freq (Hz)", P_REAL, NULL}
} ;

#else

extern double ion_mass             ;
extern double perp_velocity        ;

extern double gyro_radius_electron ;
extern double gyro_radius_ion      ;

extern double debye_length         ;
extern double mean_free_path       ;

extern double mean_speed_electron  ;
extern double mean_speed_ion       ;

extern double sound_speed_ion      ;

extern double plasma_freq_electron ;
extern double plasma_freq_ion      ;

extern double gyro_freq_electron   ;
extern double gyro_freq_ion        ;

extern double coll_freq_elect_ion  ;

extern PARAM  plasma_param_list  []  ;
extern OUTVAR plasma_outvar_list []  ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _NO_PROTO_

void init_plasma_params    () ;

void compute_plasma_params () ;

#else

void init_plasma_params    () ;

void compute_plasma_params () ;

#endif

