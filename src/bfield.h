/*****************************************************************************/
/*                                                                           */
/*   Module:	bfield.h                                                     */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the BFIELD Module.        */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   01_Feb_94 NRV	Rewritten.                                   */
/*                                                                           */
/*****************************************************************************/



#ifdef TEMPEST_MAIN

int       b_igrf_order   = 10   ;  /* Order of spherical harmonics to use    */
                                   /*    for the IGRF model                  */

int       b_igrf_secvar  = FALSE;  /* At each step update date for IGRF      */

double    b_trace_step   =  0.0 ;  /* Step in meters for field tracing       */
double    b_trace_alt    =  0.0 ;  /* Altitude to which to trace field lines */

Cartesian bfield_loc_gc  ;         /* Magnetic field in local geocentric     */
                                   /*  coordinates and in units of Tesla     */
double    bfield_loc_mag ;         /* Magnitude of magnetic field            */

double    bfield_horiz   ;         /* Horizontal component of local field    */
double    bfield_incl    ;         /* Inclination of magnetic field vector   */
double    bfield_decl    ;         /* Inclination of magnetic field vector   */

Cartesian bfield_eci     ;         /* Magnetic field in ECI coordinates      */
Cartesian bfield_lvlh    ;         /* Total magnetic field (lvlh)            */

Cartesian bfield_start   ;         /* Magnetic field at the start of tether  */
double    bfield_sta_mag ;         /* Magnitude of the magnetic field at the */
                                   /*   start of the tether                  */

Cartesian bfield_end     ;         /* Magnetic field at the end of tether    */
Cartesian bfield_end_lvlh;         /* Mag field at end in LVLH               */
double    bfield_end_mag ;         /* Magnitude of the magnetic field at the */
                                   /*   end of the tether                    */

Cartesian b_tracep_cart  ;         /* Position to which field line is traced */
Earth     b_tracep_lla   ;         /* Lat,Long to which field line is traced */

Cartesian b_tracem_cart  ;         /* Position to which field line is traced */
Earth     b_tracem_lla   ;         /* Lat,Long to which field line is traced */

Cartesian b_traceeq_cart ;         /* Position to which field line is traced */
Earth     b_traceeq_lla  ;         /* Lat,Long to which field line is traced */

double    b_angle_ip     ;         /* Inclination of field line in  plane    */
double    b_angle_op     ;         /* Inclination of field line out of plane */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct PARAMETERS bfield_param_list [] =
{
   {"IGRF_ORDER"   , P_INT  , &b_igrf_order,
    "Order to be used for IGRF magnetic field model (default=10) "},
   {"IGRF_SECVAR"  , P_BOOL , &b_igrf_secvar,
    "Continuously update IGRF for secular variation (default NO) "},
   {"TRACE_ALT"    , P_REAL , &b_trace_alt,
    "Altitude to which field lines are to be traced (meters)     "},
   {"TRACE_STEP"   , P_REAL , &b_trace_step,
    "Step distance in meters for magnetic field line traceing    "}
} ;

struct OUTPUT_VARS bfield_outvar_list [] =
{
   {"BX_LOC"         , 5, " %9.2f"    , " B0x_(nT)", &(bfield_loc_gc.X), 1.0e9,
    "X comp. (EAST)  of magnetic field at tether 0 (loc gc) (nT)",
    "B|-0x|= (Local Geocentric) (nT)", P_REAL, NULL},
   {"BY_LOC"         , 5, " %9.2f"    , " B0y_(nT)", &(bfield_loc_gc.Y), 1.0e9,
    "Y comp. (NORTH) of magnetic field at tether 0 (loc gc) (nT)",
    "B|-0y|= (Local Geocentric) (nT)", P_REAL, NULL},
   {"BZ_LOC"         , 5, " %9.2f"    , " B0z_(nT)", &(bfield_loc_gc.Z), 1.0e9,
    "Z comp. (UP)    of magnetic field at tether 0 (loc gc) (nT)",
    "B|-0z|= (Local Geocentric) (nT)", P_REAL, NULL},
   {"B_MAG_GAUSS"    , 7, " %9.7f"    , "   B0_(G)", &(bfield_loc_mag), 1.0e4,
    "Magnitude       of magnetic field at tether 0 (Gauss)",
    "||B|-0|=|| (nT)", P_REAL, NULL},
   {"B_MAG"          , 1, " %9.2f"    , "  B0_(nT)", &(bfield_loc_mag), 1.0e9,
    "Magnitude       of magnetic field at tether 0 (nT)",
    "||B|-0|=|| (nT)", P_REAL, NULL},
   {"B_HORIZ"        , 3, " %9.2f"    , " B0h_(nT)", &(bfield_horiz)  , 1.0e9,
    "Horizontal Component of magnetic field at tether 0 (nT)",
    "B|-0 horiz|= (nT)", P_REAL, NULL},
   {"B_INCL"         , 3, "%10.5f"    , " B0_incl", &(bfield_incl), R_D_CONST,
    "Inclination of magnetic field at tether 0 (deg)",
    "B|-0 incl|= (deg)", P_REAL, NULL},
   {"B_DECL"         , 3, "%10.5f"    , " B0_decl", &(bfield_decl), R_D_CONST,
    "Declination of magnetic field at tether 0 (deg)",
    "B|-0 decl|= (deg)", P_REAL, NULL},
   {"BX_LVLH"        , 4, " %9.2f"    , " B0xL(nT)", &(bfield_lvlh.X), 1.0e9,
    "X component of magnetic field in LVLH at tether 0 (nT)",
    "B|-0x|= LVLH (nT)", P_REAL, NULL},
   {"BY_LVLH"        , 4, " %9.2f"    , " B0yL(nT)", &(bfield_lvlh.Y), 1.0e9,
    "Y component of magnetic field in LVLH at tether 0 (nT)",
    "B|-0y|= LVLH (nT)", P_REAL, NULL},
   {"BZ_LVLH"        , 4, " %9.2f"    , " B0zL(nT)", &(bfield_lvlh.Z), 1.0e9,
    "Z component of magnetic field in LVLH at tether 0 (nT)",
    "B|-0z|= LVLH (nT)", P_REAL, NULL},
   {"B_IP_ANGLE"     , 4, " %10.5f"    , " Bip_(deg)", &(b_angle_ip), R_D_CONST,
    "Inclination of magnetic field in the in-plane direction",
    "In-plane inclination of B|-0|= (deg)", P_REAL, NULL},
   {"B_OP_ANGLE"     , 4, " %10.5f"    , " Bop_(deg)", &(b_angle_op), R_D_CONST,
    "Inclination of magnetic field in the out-oF-plane direction",
    "Out-of-plane inclination of B|-0|= (deg)", P_REAL, NULL},
   {"BX_ECI"         , 4, " %9.2f"    , " B0xE(nT)", &(bfield_eci.X), 1.0e9,
    "X component of magnetic field in ECI at tether 0 (nT)",
    "B|-0x|= ECI (nT)", P_REAL, NULL},
   {"BY_ECI"         , 4, " %9.2f"    , " B0yE(nT)", &(bfield_eci.Y), 1.0e9,
    "Y component of magnetic field in ECI at tether 0 (nT)",
    "B|-0y|= ECI (nT)", P_REAL, NULL},
   {"BZ_ECI"         , 4, " %9.2f"    , " B0zE(nT)", &(bfield_eci.Z), 1.0e9,
    "Z component of magnetic field in ECI at tether 0 (nT)",
    "B|-0z|= ECI (nT)", P_REAL, NULL},
   {"B_STA_X"        , 7, " %9.2f"    , " Bs_x(nT)", &(bfield_start.X), 1.0e9,
    "X component of magnetic field at tether start (loc gc) (nT)",
    "B|-Sx|= ECI (nT)", P_REAL, NULL},
   {"B_STA_Y"        , 7, " %9.2f"    , " Bs_y(nT)", &(bfield_start.Y), 1.0e9,
    "Y component of magnetic field at tether start (loc gc) (nT)",
    "B|-Sy|= ECI (nT)", P_REAL, NULL},
   {"B_STA_Z"        , 7, " %9.2f"    , " Bs_z(nT)", &(bfield_start.Z), 1.0e9,
    "Z component of magnetic field at tether start (loc gc) (nT)",
    "B|-Sz|= ECI (nT)", P_REAL, NULL},
   {"B_START"        , 3, " %9.2f"    , "  Bs_(nT)", &(bfield_sta_mag), 1.0e9,
    "Magnitude   of magnetic field at tether start          (nT)",
    "start ||B|-S|=|| ECI (nT)", P_REAL, NULL},
   {"B_END_X"        , 7, " %9.2f"    , " Be_x(nT)", &(bfield_end.X), 1.0e9,
    "X component of magnetic field at tether end (loc gc) (nT)",
    "B|-Ex|= ECI (nT)", P_REAL, NULL},
   {"B_END_Y"        , 7, " %9.2f"    , " Be_y(nT)", &(bfield_end.Y), 1.0e9,
    "Y component of magnetic field at tether end (loc gc) (nT)",
    "B|-Ey|= ECI (nT)", P_REAL, NULL},
   {"B_END_Z"        , 7, " %9.2f"    , " Be_z(nT)", &(bfield_end.Z), 1.0e9,
    "Z component of magnetic field at tether end (loc gc) (nT)",
    "B|-Ez|= ECI (nT)", P_REAL, NULL},
   {"B_END"          , 3, " %9.2f"    , "  Be_(nT)", &(bfield_end_mag), 1.0e9,
    "Magnitude   of magnetic field at tether end          (nT)",
    "||B|-E|=|| ECI (nT)", P_REAL, NULL},
   {"BX_END_LVLH"    , 4, " %9.2f"    , " BexL(nT)",&(bfield_end_lvlh.X), 1.0e9,
    "X component of magnetic field in LVLH at tether end (nT)",
    "B|-ex|= LVLH (nT)", P_REAL, NULL},
   {"BY_END_LVLH"    , 4, " %9.2f"    , " BeyL(nT)",&(bfield_end_lvlh.Y), 1.0e9,
    "Y component of magnetic field in LVLH at tether end (nT)",
    "B|-ey|= LVLH (nT)", P_REAL, NULL},
   {"BZ_END_LVLH"    , 4, " %9.2f"    , " BezL(nT)",&(bfield_end_lvlh.Z), 1.0e9,
    "Z component of magnetic field in LVLH at tether end (nT)",
    "B|-ez|= LVLH (nT)", P_REAL, NULL},
   {"TRACE_P_LAT"  ,10,"   %11.5f","  B_Trace+_Lat",&(b_tracep_lla.Lat) ,R_D_CONST,
    "Lat  at which traced field line in +|B| hits ground (deg +N)",
    "Latitude  of field line trace in +B (+N)", P_REAL, NULL},
   {"TRACE_P_LONG" ,10,"   %12.5f","  B_Trace+_Long",&(b_tracep_lla.Long),R_D_CONST,
    "Long at which traced field line in +|B| hits ground (deg +E)",
    "Longitude of field line trace in +B (+E)", P_REAL, NULL},
   {"TRACE_M_LAT"  ,10,"   %11.5f","  B_Trace-_Lat",&(b_tracem_lla.Lat) ,R_D_CONST,
    "Lat  at which traced field line in -|B| hits ground (deg +N)",
    "Latitude  of field line trace in -B (+N)", P_REAL, NULL},
   {"TRACE_M_LONG" ,10,"   %12.5f","  B_Trace-_Long",&(b_tracem_lla.Long),R_D_CONST,
    "Long at which traced field line in -|B| hits ground (deg +E)",
    "Longitude of field line trace in -B (+E)", P_REAL, NULL},
   {"TRACE_EQ_LON", 10,"   %12.5f"," B_TraceEQ_Long",&(b_traceeq_lla.Long),R_D_CONST,
    "Long at which traced field line intersects geographical equator (deg +E)",
    "Longitude of field line trace to equator (+E)", P_REAL, NULL},
   {"TRACE_EQ_ALT", 10,"   %12.5f","  B_TraceEQ_Alt",&(b_traceeq_lla.Alt), 0.001,
    "Altitude at which traced field line intersects geographical equator (km)",
    "Altitude of field line trace to equator (km)", P_REAL, NULL},
} ;

#else

extern int       b_igrf_order   ;

extern int       b_igrf_secvar  ;

extern double    b_trace_step   ;
extern double    b_trace_alt    ;

extern Cartesian bfield_loc_gc  ;
extern double    bfield_loc_mag ;

extern double    bfield_horiz   ;
extern double    bfield_incl    ;
extern double    bfield_decl    ;

extern Cartesian bfield_eci     ;
extern Cartesian bfield_lvlh    ;

extern Cartesian bfield_start   ;
extern double    bfield_sta_mag ;

extern Cartesian bfield_end     ;
extern Cartesian bfield_end_lvlh;
extern double    bfield_end_mag ;

extern Cartesian b_tracep_cart  ;
extern Earth     b_tracep_lla   ;
extern Cartesian b_tracem_cart  ;
extern Earth     b_tracem_lla   ;
extern Cartesian b_traceeq_cart ;
extern Earth     b_traceeq_lla  ;

extern double    b_angle_ip     ;
extern double    b_angle_op     ;

extern PARAM     bfield_param_list  [] ;
extern OUTVAR    bfield_outvar_list [] ;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _NO_PROTO_

void init_bfield () ;

void compute_bfields () ;

#else

void init_bfield () ;

void compute_bfields () ;

#endif
