/*****************************************************************************/
/*                                                                           */
/*   Module:    att.h                                                        */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the ATTITUDE Module.      */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h".                                */
/*                                                                           */
/*   RCS:	$Id: att.h,v 1.1 1997/10/21 19:53:35 nestorv Exp $                                                        */
/*                                                                           */
/*		$Log: att.h,v $
 * Revision 1.1  1997/10/21  19:53:35  nestorv
 * Initial revision
 *                                                       */
/*                                                                           */
/*****************************************************************************/

                                   /* Attitude coordinate systems            */
enum {LVLH_COORD, INERTIAL_COORD} ;

#ifdef TEMPEST_MAIN

int    att_year = -1           ;   /* Year when attitude command is given    */
GMT    att_gmt  =                  /* GMT  when attitude command is given    */
               {-1, -1, -1, -1, -1} ;

double comm_att_roll    = 0.0  ;   /* Commanded attitude - roll  (degrees)   */
double comm_att_pitch   = 0.0  ;   /* Commanded attitude - pitch (degrees)   */
double comm_att_yaw     = 0.0  ;   /* Commanded attitude - yaw   (degrees)   */

double comm_att_omega_x = 0.0  ;   /* Commanded omega about X axis (deg/sec) */
double comm_att_omega_y = 0.0  ;   /* Commanded omega about Y axis (deg/sec) */
double comm_att_omega_z = 0.0  ;   /* Commanded omega about Z axis (deg/sec) */

char   att_coord_str [10] =
          "LVLH"               ;   /* Reference frame for commanded attitude */
int    att_coord_val      =
          LVLH_COORD           ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double attitude_lvlh_roll      ;   /* Actual attitude in LVLH   - roll       */
double attitude_lvlh_pitch     ;   /* Actual attitude in LVLH   - pitch      */
double attitude_lvlh_yaw       ;   /* Actual attitude in LVLH   - yaw        */

double attitude_inrt_roll      ;   /* Actual INERTIAL attitude - roll        */
double attitude_inrt_pitch     ;   /* Actual INERTIAL attitude - pitch       */
double attitude_inrt_yaw       ;   /* Actual INERTIAL attitude - yaw         */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS attitude_param_list [] =
{
   {"ATT_YEAR"  , P_INT, &att_year,
    "Year when attitude is commanded"},
   {"ATT_GMT"   , P_GMT, &att_gmt,
    "GMT  when attitude is commanded"},
   {"ROLL"      , P_DEG, &att_comm_roll,
    "Commanded body attitude angle - roll  (degrees)"},
   {"PITCH"     , P_DEG, &att_comm_pitch,
    "Commanded body attitude angle - pitch (degrees)"},
   {"YAW"       , P_DEG, &att_comm_yaw,
    "Commanded body attitude angle - yaw   (degrees)"},
   {"OMEGA_X"   , P_DEG, &comm_omega_x,
    "Commanded body rate - Omega X (degrees/second)"},
   {"OMEGA_Y"   , P_DEG, &comm_omega_y,
    "Commanded body rate - Omega Y (degrees/second)"},
   {"OMEGA_Z"   , P_DEG, &comm_omega_z,
    "Commanded body rate - Omega Z (degrees/second)"},
   {"ATT_COORD"  ,P_STR,  att_coord_str,
    "Coordinate system of commanded attitude [LVLH,inertial]"},
} ;

struct OUTPUT_VARS attitude_outvar_list [] =
{
   {"LVLH_ROLL"  ,6," %13.7f", " LVLH_R_(deg)",&(attitude_lvlh_roll) ,R_D_CONST,
    "Body attitude in LVLH coordinates - roll  (degrees)",
    "Roll (degrees)"},
   {"LVLH_PITCH" ,6," %13.7f", " LVLH_P_(deg)",&(attitude_lvlh_pitch),R_D_CONST,
    "Body attitude in LVLH coordinates - pitch (degrees)",
    "Pitch (degrees)"},
   {"LVLH_YAW"   ,6," %13.7f", " LVLH_Y_(deg)",&(attitude_lvlh_yaw)  ,R_D_CONST,
    "Body attitude in LVLH coordinates - yaw   (degrees)",
    "Yaw (degrees)"},
   {"INRT_ROLL"  ,6," %13.7f", " INRT_R_(deg)",&(attitude_inrt_roll) ,R_D_CONST,
    "Body attitude in inertial coordinates - roll  (degrees)",
    "Roll (degrees)"},
   {"INRT_PITCH" ,6," %13.7f", " INRT_P_(deg)",&(attitude_inrt_pitch),R_D_CONST,
    "Body attitude in inertial coordinates - pitch (degrees)",
    "Pitch (degrees)"},
   {"INRT_YAW"   ,6," %13.7f", " INRT_Y_(deg)",&(attitude_inrt_yaw)  ,R_D_CONST,
    "Body attitude in inertial coordinates - yaw   (degrees)",
    "Yaw (degrees)"}
} ;

#else

extern PARAM  attitude_param_list  []  ;
extern OUTVAR attitude_outvar_list []  ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _NO_PROTO_

void init_attitude    () ;

void compute_attitude () ;

#else

void init_attitude    () ;

void compute_attitude () ;

#endif

