/* msis86.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Common Block Declarations */

struct {
    integer iumsis, monito, iiee;
} uinr_;

#define uinr_1 uinr_

struct {
    real tlb, s, db04, db16, db28, db32, db40, db48, db01, za, t0, z0, g0, rl,
	     dd, db14;
} gts3c_;

#define gts3c_1 gts3c_

struct {
    real ptm[8], pdm[56]	/* was [8][7] */;
} lower5_;

#define lower5_1 lower5_

union {
    struct {
	real pt[150], pd[1050]	/* was [150][7] */, ps[150], pdl[50]	/* 
		was [25][2] */;
    } _1;
    struct {
	real pt1[50], pt2[50], pt3[50], pa1[50], pa2[50], pa3[50], pb1[50], 
		pb2[50], pb3[50], pc1[50], pc2[50], pc3[50], pd1[50], pd2[50],
		 pd3[50], pe1[50], pe2[50], pe3[50], pf1[50], pf2[50], pf3[50]
		, pg1[50], pg2[50], pg3[50], ph1[50], ph2[50], ph3[50], pi1[
		50];
    } _2;
} parm5_;

#define parm5_1 (parm5_._1)
#define parm5_2 (parm5_._2)

struct {
    real sw[25];
    integer isw;
    real swc[25];
} csw_;

#define csw_1 csw_

union {
    struct {
	real tinfg, gb, rout, tt[15];
    } _1;
    struct {
	real tinf, gb, rout, t[15];
    } _2;
} ttest_;

#define ttest_1 (ttest_._1)
#define ttest_2 (ttest_._2)

union {
    struct {
	integer isdate[3], istime[2], name__;
    } _1;
    struct {
	char isdate[3], istime[8], name__[2];
    } _2;
} datime_;

#define datime_1 (datime_._1)
#define datime_2 (datime_._2)

struct {
    real gsurf, re;
} parmb_;

#define parmb_1 parmb_

struct {
    real taf;
} fit_;

#define fit_1 fit_

struct {
    integer mp, ii, jg, lt;
    real qpb[50];
    integer ierr, ifun, n, j;
    real dv[60];
} lsqv_;

#define lsqv_1 lsqv_

struct {
    real plg[36]	/* was [9][4] */, ctloc, stloc, c2tloc, s2tloc, 
	    c3tloc, s3tloc;
    integer iyr;
    real day, df, dfa, apd, apdf, apt[4];
} lpoly_;

#define lpoly_1 lpoly_

/* Table of constant values */

static integer c__1 = 1;
static real c_b11 = 28.f;
static real c_b12 = 0.f;
static real c_b13 = -1.f;
static real c_b22 = 4.f;
static real c_b23 = -.4f;
static real c_b25 = -1.4f;
static real c_b28 = 16.f;
static real c_b34 = 32.f;
static real c_b40 = 40.f;
static real c_b46 = 1.f;
static real c_b52 = 14.f;
static integer c__4 = 4;
static integer c__3 = 3;
static doublereal c_b100 = .5;
static integer c__50 = 50;
static integer c__8 = 8;
static integer c__56 = 56;

/* MSIS86.FOR	D. BILITZA	10/88 */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* SUBROUTINES AND FUNCTIONS: */
/* 	GTS5, DENSS, GLOBE5, TSELEC, GLOB5L, DNET, CCOR, PRMSG5,GGM */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


/* *********************************************************************** */
/* Subroutine */ int gts5_0_(int n__, char *path, integer *iyd, real *sec, 
	real *alt, real *glat, real *glong, real *stl, real *f107a, real *
	f107, real *ap, integer *mass, real *d__, real *t, logical *meter, 
	ftnlen path_len)
{
    /* Initialized data */

    static integer mt[10] = { 48,0,4,16,28,32,40,1,49,14 };
    static integer ifl = 0;
    static real altl[8] = { 200.f,400.f,150.f,200.f,240.f,450.f,320.f,450.f };
    static integer imr = 0;

    /* Format strings */
    static char fmt_100[] = "(1x,\002MASS\002,i5,\002  NOT VALID\002)";

    /* System generated locals */
    real r__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
    double exp(doublereal), log(doublereal);

    /* Local variables */
    static integer i__, j;
    static real g1, g4, b01, b04, b32, b16, g40, b28, g32, g16, b40, g14, g28,
	     b14, tz, hc04, hc32, hc16, hc40, dm04, dm32, dm16, dm40, dm01, 
	    dm28, rc16, hc01, zc04, zc16, zh04, tr12, xmd, zh16, zh28, yrd, 
	    zh32, xmm, zc32, zh40, zc40, zh01, zc01, rc01, zh14, dm14, hc14, 
	    zc14, rc14, hcc01, hcc14, hcc16, gggg, zcc01, zcc14;
    extern real ccor_(real *, real *, real *, real *);
    static real zcc16, ddum;
    extern real dnet_(real *, real *, real *, real *, real *);
    static real zhm01, tinf, zhm04, zhm32, zhm16, zhm40, zhm14, zhm28;
    extern real denss_(real *, real *, real *, real *, real *, real *, real *,
	     real *, real *, real *, real *, real *, real *), globe5_(real *, 
	    real *, real *, real *, real *, real *, real *, real *, real *), 
	    glob5l_(real *);
    extern /* Subroutine */ int prmsg5_(char *, ftnlen);

    /* Fortran I/O blocks */
    static cilist io___12 = { 0, 6, 0, fmt_100, 0 };


/*        MSIS-86/CIRA 1986 Neutral Thermosphere Model */
/*         A.E.Hedin 3/15/85;2/26/87 (Variable Names Shortened) */
/*         10/14/87 increase altitude limit of O mixing calculation */
/*             ALTL(2) from 300.0 to 400.0 km . */
/*     INPUT: */
/*        IYD - YEAR AND DAY AS YYDDD */
/*        SEC - UT(SEC) */
/*        ALT - ALTITUDE(KM) (GREATER THAN 85 KM) */
/*        GLAT - GEODETIC LATITUDE(DEG) */
/*        GLONG - GEODETIC LONGITUDE(DEG) */
/*        STL - LOCAL APPARENT SOLAR TIME(HRS) */
/*        F107A - 3 MONTH AVERAGE OF F10.7 FLUX */
/*        F107 - DAILY F10.7 FLUX FOR PREVIOUS DAY */
/*        AP - MAGNETIC INDEX(DAILY) OR WHEN SW(9)=-1. : */
/*           - ARRAY CONTAINING: */
/*             (1) DAILY AP */
/*             (2) 3 HR AP INDEX FOR CURRENT TIME */
/*             (3) 3 HR AP INDEX FOR 3 HRS BEFORE CURRENT TIME */
/*             (4) 3 HR AP INDEX FOR 6 HRS BEFORE CURRENT TIME */
/*             (5) 3 HR AP INDEX FOR 9 HRS BEFORE CURRENT TIME */
/*             (6) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 12 TO 33 HRS PRIOR */
/*                    TO CURRENT TIME */
/*             (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 59 HRS PRIOR */
/*                    TO CURRENT TIME */
/*        MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS */
/*                 CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL. */
/*     OUTPUT: */
/*        D(1) - HE NUMBER DENSITY(CM-3) */
/*        D(2) - O NUMBER DENSITY(CM-3) */
/*        D(3) - N2 NUMBER DENSITY(CM-3) */
/*        D(4) - O2 NUMBER DENSITY(CM-3) */
/*        D(5) - AR NUMBER DENSITY(CM-3) */
/*        D(6) - TOTAL MASS DENSITY(GM/CM3) */
/*        D(7) - H NUMBER DENSITY(CM-3) */
/*        D(8) - N NUMBER DENSITY(CM-3) */
/*        T(1) - EXOSPHERIC TEMPERATURE */
/*        T(2) - TEMPERATURE AT ALT */

/*      TO GET OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.) */

/*          ADDITIONAL COMMENTS */
/*           (1) LOWER BOUND QUANTITIES IN COMMON/GTS3C/ */
/*           (2) TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW) */
/*               WHERE SW IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1. */
/*               FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON */
/*               FOR THE FOLLOWING VARIATIONS */
/*               1 - F10.7 EFFECT ON MEAN  2 - TIME INDEPENDENT */
/*               3 - SYMMETRICAL ANNUAL    4 - SYMMETRICAL SEMIANNUAL */
/*               5 - ASYMMETRICAL ANNUAL   6 - ASYMMETRICAL SEMIANNUAL */
/*               7 - DIURNAL               8 - SEMIDIURNAL */
/*               9 - DAILY AP             10 - ALL UT/LONG EFFECTS */
/*              11 - LONGITUDINAL         12 - UT AND MIXED UT/LONG */
/*              13 - MIXED AP/UT/LONG     14 - TERDIURNAL */
/*              15 - DEPARTURES FROM DIFFUSIVE EQUILIBRIUM */
/*              16 - ALL TINF VAR         17 - ALL TLB VAR */
/*              18 - ALL T0 VAR           19 - ALL S VAR */
/*              20 - ALL Z0 VAR           21 - ALL NLB VAR */
/*              22 - ALL TR12 VAR         23 - TURBO SCALE HEIGHT VAR */

/*              To get current values of SW: CALL TRETRV(SW) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!! CHANGES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/*  - NAME,ISD,IST,ISDATE, and ISTIME were changed to character variables */
/*    in GTS5 and PRMSG5 */

/*  - The variable dimension of P and AP in GLOBE5 and GLOBE5L was */
/*    indicted by *, rather than 1; if this does not work on your system */
/*    you may want to use P(150) and AP(7). */

/*  - The large data statement in PRMSG5 is now read in from file */
/*    MSIS86.DAT; some compilers do not allow named commons to be */
/*    initialized in a data statement. */

/*  - The first call to GLOBE5 should come before the common array SW(25) */
/*    is used in GTS5. */

/* Dieter Bilitza !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! March 87 */
/* ********************************************************************** */
/*     CHARACTER NAME(2)*4 */
    /* Parameter adjustments */
    if (ap) {
	--ap;
	}
    if (d__) {
	--d__;
	}
    if (t) {
	--t;
	}

    /* Function Body */
    switch(n__) {
	case 1: goto L_meters;
	}

    if (ifl == 0) {
	prmsg5_(path, path_len);
	if (uinr_1.iiee > 0) {
	    goto L9999;
	}
	ifl = 1;
    }
    yrd = (real) (*iyd);
/*       Eq. A7 */
/* !!OLD!! TINF=PTM(1)*(1.+SW(16)*GLOBE5(YRD,SEC,GLAT,GLONG,STL,F107A,F107, */
/* !!OLD!!$ AP,PT))*PT(1) */
    gggg = globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[1], 
	    parm5_1.pt);
    tinf = lower5_1.ptm[0] * (csw_1.sw[15] * gggg + 1.f) * parm5_1.pt[0];
    gts3c_1.za = lower5_1.ptm[4] * parm5_1.pdl[40];
/*       Eq. A9 */
    gts3c_1.t0 = lower5_1.ptm[2] * parm5_1.pd[375] * (csw_1.sw[17] * glob5l_(&
	    parm5_1.pd[375]) + 1.f);
/*       Eq. A8 */
    gts3c_1.tlb = lower5_1.ptm[1] * (csw_1.sw[16] * glob5l_(&parm5_1.pd[325]) 
	    + 1.f) * parm5_1.pd[325];
/*       Eq. A10 */
    gts3c_1.z0 = lower5_1.ptm[6] * (csw_1.sw[19] * glob5l_(&parm5_1.pd[350]) 
	    + 1.f) * parm5_1.pd[350];
/*       Eq. A6 */
    gts3c_1.g0 = lower5_1.ptm[3] * parm5_1.ps[0] * (csw_1.sw[18] * globe5_(&
	    yrd, sec, glat, glong, stl, f107a, f107, &ap[1], parm5_1.ps) + 
	    1.f);
/*       Eq. A5 */
    gts3c_1.s = gts3c_1.g0 / (tinf - gts3c_1.tlb);
/*       Eq. A11 */
    tr12 = parm5_1.pd[400] * (csw_1.sw[21] * glob5l_(&parm5_1.pd[400]) + 1.f);
    t[1] = tinf;
    if (*mass == 0) {
	goto L50;
    }
/*       Eq. A18  N2 */
    g28 = csw_1.sw[20] * glob5l_(&parm5_1.pd[300]);
    yrd = (real) (*iyd);
    t[1] = tinf;
    xmm = lower5_1.pdm[20];
    for (j = 1; j <= 10; ++j) {
	if (*mass == mt[j - 1]) {
	    goto L15;
	}
/* L10: */
    }
    s_wsfe(&io___12);
    do_fio(&c__1, (char *)&(*mass), (ftnlen)sizeof(integer));
    e_wsfe();
    goto L90;
L15:
    if (*alt > altl[5] && *mass != 28 && *mass != 48) {
	goto L17;
    }

/*       **** N2 DENSITY **** */

/*       Eq. A18 */
    gts3c_1.db28 = lower5_1.pdm[16] * exp(g28) * parm5_1.pd[300];
/*       Eq. A13 - A17 */
    d__[3] = denss_(alt, &gts3c_1.db28, &tinf, &gts3c_1.tlb, &c_b11, &c_b12, &
	    t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[3];
/*       Eq. A19 */
    zh28 = lower5_1.pdm[18];
    zhm28 = lower5_1.pdm[19] * parm5_1.pdl[30];
    xmd = 28.f - xmm;
    b28 = denss_(&zh28, &gts3c_1.db28, &tinf, &gts3c_1.tlb, &xmd, &c_b13, &tz,
	     &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    if (*alt > altl[2] || csw_1.sw[14] == 0.f) {
	goto L17;
    }
    dm28 = denss_(alt, &b28, &tinf, &gts3c_1.tlb, &xmm, &c_b12, &tz, &
	    lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
/*       Eq. A12 */
    d__[3] = dnet_(&d__[3], &dm28, &zhm28, &xmm, &c_b11);
L17:
    switch (j) {
	case 1:  goto L20;
	case 2:  goto L50;
	case 3:  goto L20;
	case 4:  goto L25;
	case 5:  goto L90;
	case 6:  goto L35;
	case 7:  goto L40;
	case 8:  goto L45;
	case 9:  goto L25;
	case 10:  goto L48;
    }
L20:

/*       **** HE DENSITY **** */

/*       Eq. A18 */
    g4 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[
	    1], parm5_1.pd);
    gts3c_1.db04 = lower5_1.pdm[0] * exp(g4) * parm5_1.pd[0];
/*       Eq. A13 - A17 */
    d__[1] = denss_(alt, &gts3c_1.db04, &tinf, &gts3c_1.tlb, &c_b22, &c_b23, &
	    t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[1];
    if (*alt > altl[0] || csw_1.sw[14] == 0.f) {
	goto L24;
    }
/*       Eq. A19 */
    zh04 = lower5_1.pdm[2];
    r__1 = 4.f - xmm;
    b04 = denss_(&zh04, &gts3c_1.db04, &tinf, &gts3c_1.tlb, &r__1, &c_b25, &t[
	    2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    dm04 = denss_(alt, &b04, &tinf, &gts3c_1.tlb, &xmm, &c_b12, &t[2], &
	    lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm04 = zhm28;
    d__[1] = dnet_(&d__[1], &dm04, &zhm04, &xmm, &c_b22);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[1] / b04);
/*       Eq. A20a */
    zc04 = lower5_1.pdm[4] * parm5_1.pdl[25];
    hc04 = lower5_1.pdm[5] * parm5_1.pdl[26];
    d__[1] *= ccor_(alt, &gts3c_1.rl, &hc04, &zc04);
L24:
    if (*mass != 48) {
	goto L90;
    }
L25:

/*      **** O DENSITY **** */

/*       Eq. A18 */
    g16 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm5_1.pd[150]);
    gts3c_1.db16 = lower5_1.pdm[8] * exp(g16) * parm5_1.pd[150];
/*       Eq. A13 - A17 */
    d__[2] = denss_(alt, &gts3c_1.db16, &tinf, &gts3c_1.tlb, &c_b28, &c_b12, &
	    t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[2];
    if (*alt > altl[1] || csw_1.sw[14] == 0.f) {
	goto L34;
    }
/*  Corrected from PDM(3,1) to PDM(3,2)  12/2/85 */
/*       Eq. A19 */
    zh16 = lower5_1.pdm[10];
    r__1 = 16 - xmm;
    b16 = denss_(&zh16, &gts3c_1.db16, &tinf, &gts3c_1.tlb, &r__1, &c_b13, &t[
	    2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    dm16 = denss_(alt, &b16, &tinf, &gts3c_1.tlb, &xmm, &c_b12, &t[2], &
	    lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm16 = zhm28;
    d__[2] = dnet_(&d__[2], &dm16, &zhm16, &xmm, &c_b28);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[9] * abs(parm5_1.pdl[41]) / b16);
/*       Eq. A20a */
    hc16 = lower5_1.pdm[13] * parm5_1.pdl[28];
    zc16 = lower5_1.pdm[12] * parm5_1.pdl[27];
    d__[2] *= ccor_(alt, &gts3c_1.rl, &hc16, &zc16);
/*       Eq. A21 */
    hcc16 = lower5_1.pdm[15] * parm5_1.pdl[38];
    zcc16 = lower5_1.pdm[14] * parm5_1.pdl[37];
    rc16 = lower5_1.pdm[11] * parm5_1.pdl[39];
    d__[2] *= ccor_(alt, &rc16, &hcc16, &zcc16);
L34:
    if (*mass != 48 && *mass != 49) {
	goto L90;
    }
L35:

/*       **** O2 DENSITY **** */

/*       Eq. A18 */
    g32 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm5_1.pd[450]);
    gts3c_1.db32 = lower5_1.pdm[24] * exp(g32) * parm5_1.pd[450];
/*       Eq. A13 - A17 */
    d__[4] = denss_(alt, &gts3c_1.db32, &tinf, &gts3c_1.tlb, &c_b34, &c_b12, &
	    t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    if (*mass == 49) {
	gts3c_1.dd += d__[4] * 2.f;
    } else {
	gts3c_1.dd = d__[4];
    }
    if (*alt > altl[3] || csw_1.sw[14] == 0.f) {
	goto L39;
    }
/*       Eq. A19 */
    zh32 = lower5_1.pdm[26];
    r__1 = 32.f - xmm;
    b32 = denss_(&zh32, &gts3c_1.db32, &tinf, &gts3c_1.tlb, &r__1, &c_b13, &t[
	    2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    dm32 = denss_(alt, &b32, &tinf, &gts3c_1.tlb, &xmm, &c_b12, &t[2], &
	    lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm32 = zhm28;
    d__[4] = dnet_(&d__[4], &dm32, &zhm32, &xmm, &c_b34);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[25] / b32);
/*       Eq. A20a */
    hc32 = lower5_1.pdm[29] * parm5_1.pdl[32];
    zc32 = lower5_1.pdm[28] * parm5_1.pdl[31];
    d__[4] *= ccor_(alt, &gts3c_1.rl, &hc32, &zc32);
L39:
    if (*mass != 48) {
	goto L90;
    }
L40:

/*       **** AR DENSITY **** */

/*       Eq. A18 */
    g40 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm5_1.pd[600]);
    gts3c_1.db40 = lower5_1.pdm[32] * exp(g40) * parm5_1.pd[600];
/*       Eq. A13 - A17 */
    d__[5] = denss_(alt, &gts3c_1.db40, &tinf, &gts3c_1.tlb, &c_b40, &c_b12, &
	    t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[5];
    if (*alt > altl[4] || csw_1.sw[14] == 0.f) {
	goto L44;
    }
/*       Eq. A19 */
    zh40 = lower5_1.pdm[34];
    r__1 = 40.f - xmm;
    b40 = denss_(&zh40, &gts3c_1.db40, &tinf, &gts3c_1.tlb, &r__1, &c_b13, &t[
	    2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    dm40 = denss_(alt, &b40, &tinf, &gts3c_1.tlb, &xmm, &c_b12, &t[2], &
	    lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm40 = zhm28;
    d__[5] = dnet_(&d__[5], &dm40, &zhm40, &xmm, &c_b40);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[33] / b40);
/*       Eq. A20a */
    hc40 = lower5_1.pdm[37] * parm5_1.pdl[34];
    zc40 = lower5_1.pdm[36] * parm5_1.pdl[33];
    d__[5] *= ccor_(alt, &gts3c_1.rl, &hc40, &zc40);
L44:
    if (*mass != 48) {
	goto L90;
    }
L45:

/*        **** HYDROGEN DENSITY **** */

/*       Eq. A18 */
    g1 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &ap[
	    1], &parm5_1.pd[750]);
    gts3c_1.db01 = lower5_1.pdm[40] * exp(g1) * parm5_1.pd[750];
/*       Eq. A13 - A17 */
    d__[7] = denss_(alt, &gts3c_1.db01, &tinf, &gts3c_1.tlb, &c_b46, &c_b23, &
	    t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[7];
    if (*alt > altl[6] || csw_1.sw[14] == 0.f) {
	goto L47;
    }
/*       Eq. A19 */
    zh01 = lower5_1.pdm[42];
    r__1 = 1.f - xmm;
    b01 = denss_(&zh01, &gts3c_1.db01, &tinf, &gts3c_1.tlb, &r__1, &c_b25, &t[
	    2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    dm01 = denss_(alt, &b01, &tinf, &gts3c_1.tlb, &xmm, &c_b12, &t[2], &
	    lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm01 = zhm28;
    d__[7] = dnet_(&d__[7], &dm01, &zhm01, &xmm, &c_b46);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[41] * abs(parm5_1.pdl[42]) / b01);
/*       Eq. A20a */
    hc01 = lower5_1.pdm[45] * parm5_1.pdl[36];
    zc01 = lower5_1.pdm[44] * parm5_1.pdl[35];
    d__[7] *= ccor_(alt, &gts3c_1.rl, &hc01, &zc01);
/*       Eq. A21 */
    hcc01 = lower5_1.pdm[47] * parm5_1.pdl[44];
    zcc01 = lower5_1.pdm[46] * parm5_1.pdl[43];
    rc01 = lower5_1.pdm[43] * parm5_1.pdl[45];
    d__[7] *= ccor_(alt, &rc01, &hcc01, &zcc01);
L47:
L48:

/*        **** ATOMIC NITROGEN DENSITY **** */

/*       Eq. A18 */
    g14 = csw_1.sw[20] * globe5_(&yrd, sec, glat, glong, stl, f107a, f107, &
	    ap[1], &parm5_1.pd[900]);
    gts3c_1.db14 = lower5_1.pdm[48] * exp(g14) * parm5_1.pd[900];
/*       Eq. A13 - A17 */
    d__[8] = denss_(alt, &gts3c_1.db14, &tinf, &gts3c_1.tlb, &c_b52, &c_b12, &
	    t[2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    gts3c_1.dd = d__[8];
    if (*alt > altl[7] || csw_1.sw[14] == 0.f) {
	goto L49;
    }
/*       Eq. A19 */
    zh14 = lower5_1.pdm[50];
    r__1 = 14.f - xmm;
    b14 = denss_(&zh14, &gts3c_1.db14, &tinf, &gts3c_1.tlb, &r__1, &c_b13, &t[
	    2], &lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    dm14 = denss_(alt, &b14, &tinf, &gts3c_1.tlb, &xmm, &c_b12, &t[2], &
	    lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
/*       Eq. A12 */
    zhm14 = zhm28;
    d__[8] = dnet_(&d__[8], &dm14, &zhm14, &xmm, &c_b52);
/*       Eq. A20b */
    gts3c_1.rl = log(b28 * lower5_1.pdm[49] * abs(parm5_1.pdl[2]) / b14);
/*       Eq. A20a */
    hc14 = lower5_1.pdm[53] * parm5_1.pdl[1];
    zc14 = lower5_1.pdm[52] * parm5_1.pdl[0];
    d__[8] *= ccor_(alt, &gts3c_1.rl, &hc14, &zc14);
/*       Eq. A21 */
    hcc14 = lower5_1.pdm[55] * parm5_1.pdl[4];
    zcc14 = lower5_1.pdm[54] * parm5_1.pdl[3];
    rc14 = lower5_1.pdm[51] * parm5_1.pdl[5];
    d__[8] *= ccor_(alt, &rc14, &hcc14, &zcc14);
L49:
    if (*mass != 48) {
	goto L90;
    }

/*       TOTAL MASS DENSITY */

    d__[6] = (d__[1] * 4.f + d__[2] * 16.f + d__[3] * 28.f + d__[4] * 32.f + 
	    d__[5] * 40.f + d__[7] + d__[8] * 14.f) * 1.66e-24f;
    gts3c_1.db48 = (gts3c_1.db04 * 4.f + gts3c_1.db16 * 16.f + gts3c_1.db28 * 
	    28.f + gts3c_1.db32 * 32.f + gts3c_1.db40 * 40.f + gts3c_1.db01 + 
	    gts3c_1.db14 * 14.f) * 1.66e-24f;
    goto L90;
L50:
    ddum = denss_(alt, &c_b46, &tinf, &gts3c_1.tlb, &c_b12, &c_b12, &t[2], &
	    lower5_1.ptm[5], &gts3c_1.s, &gts3c_1.t0, &gts3c_1.za, &
	    gts3c_1.z0, &tr12);
    goto L90;
L90:
    if (imr == 1) {
	for (i__ = 1; i__ <= 8; ++i__) {
	    d__[i__] *= 1e6f;
/* L95: */
	}
	d__[6] /= 1e3f;
    }
    return 0;

L_meters:
    imr = 0;
    if (*meter) {
	imr = 1;
    }
L9999:
    return 0;
} /* gts5_ */

/* Subroutine */ int gts5_(char *path, integer *iyd, real *sec, real *alt, 
	real *glat, real *glong, real *stl, real *f107a, real *f107, real *ap,
	 integer *mass, real *d__, real *t, ftnlen path_len)
{
    return gts5_0_(0, path, iyd, sec, alt, glat, glong, stl, f107a, f107, ap, 
	    mass, d__, t, (logical *)0, path_len);
    }

/* Subroutine */ int meters_(logical *meter)
{
    return gts5_0_(1, (char *)0, (integer *)0, (real *)0, (real *)0, (real *)
	    0, (real *)0, (real *)0, (real *)0, (real *)0, (real *)0, (
	    integer *)0, (real *)0, (real *)0, meter, (ftnint)0);
    }

/* -------------------------------------------------------------------- */
real denss_(real *alt, real *dlb, real *tinf, real *tlb, real *xm, real *
	alpha, real *tz, real *zlb, real *s2, real *t0, real *za, real *z0, 
	real *tr12)
{
    /* Initialized data */

    static real rgas = 831.4f;

    /* System generated locals */
    real ret_val, r__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double exp(doublereal);
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static real x, z__, x2, bb, cc, dd, ta, t12, tt, zg0, zg1, zg2, glb, dta, 
	    gamm, gamma, densa;

    /* Fortran I/O blocks */
    static cilist io___86 = { 0, 6, 0, 0, 0 };


/*       Calculate Temperature and Density Profiles for MSIS models */
    ret_val = 1.f;
    z__ = max(*alt,*za);
/*      Eq. A4a */
    zg2 = (z__ - *zlb) * (parmb_1.re + *zlb) / (parmb_1.re + z__);
/*      Eq. A1a */
    tt = *tinf - (*tinf - *tlb) * exp(-(*s2) * zg2);
    ta = tt;
    *tz = tt;
    ret_val = *tz;
    if (*alt >= *za) {
	goto L10;
    }
/*      Eq. A4b */
    zg0 = (*z0 - *za) * (parmb_1.re + *za) / (parmb_1.re + *z0);
/*      Eq. A2b */
/* Computing 2nd power */
    r__1 = (parmb_1.re + *zlb) / (parmb_1.re + *za);
    dta = (*tinf - ta) * *s2 * (r__1 * r__1);
/*      Eq. A3e */
    t12 = *t0 + *tr12 * (ta - *t0);
/*      Eq. A4b */
    zg1 = (*alt - *za) * (parmb_1.re + *za) / (parmb_1.re + *alt);
/*       CALCULATE TEMPERATURE BELOW ZA */
/*      Eq. A3a */
/* Computing 2nd power */
    r__1 = ta;
    dd = zg0 * .666666f * dta / (r__1 * r__1) - (1.f / ta - 1.f / *t0) * 
	    3.11111f + (1.f / t12 - 1.f / *t0) * 7.11111f;
/*      Eq. A3b */
    cc = zg0 * dta / (ta * 2.f * ta) - (1.f / ta - 1.f / *t0) - dd * 2.f;
/*      Eq. A3c */
    bb = 1.f / ta - 1.f / *t0 - cc - dd;
/*      Eq. A3d */
    x = -(zg1 - zg0) / zg0;
/*      Eq. A1b */
    x2 = x * x;
    *tz = 1.f / (1.f / *t0 + bb * x2 + cc * x2 * x2 + dd * x2 * x2 * x2);
    ret_val = *tz;
    fit_1.taf = (t12 - *t0) / (ta - *t0);
L10:
    if (*xm == 0.f) {
	goto L50;
    }
    if (ta > 0.f && *tz > 0.f) {
	goto L20;
    }
    s_wsle(&io___86);
    do_lio(&c__4, &c__1, (char *)&(*alt), (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&(*xm), (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&(*tinf), (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&(*tlb), (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&(*t0), (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&ta, (ftnlen)sizeof(real));
    do_lio(&c__3, &c__1, (char *)&lsqv_1.ii, (ftnlen)sizeof(integer));
    do_lio(&c__3, &c__1, (char *)&lsqv_1.jg, (ftnlen)sizeof(integer));
    do_lio(&c__3, &c__1, (char *)&lsqv_1.n, (ftnlen)sizeof(integer));
    do_lio(&c__4, &c__1, (char *)&lsqv_1.dv[lsqv_1.j - 1], (ftnlen)sizeof(
	    real));
    do_lio(&c__3, &c__1, (char *)&lsqv_1.ifun, (ftnlen)sizeof(integer));
    do_lio(&c__4, &c__1, (char *)&(*s2), (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&zg0, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&(*tz), (ftnlen)sizeof(real));
    e_wsle();
    tt = *tlb;
    ta = *tlb;
    *tz = *tlb;
L20:
/*      CALCULATE DENSITY ABOVE ZA */
/*      Eq. A17a */
/* Computing 2nd power */
    r__1 = *zlb / parmb_1.re + 1.f;
    glb = parmb_1.gsurf / (r__1 * r__1);
/*      Eq. A16a */
    gamma = *xm * glb / (*s2 * rgas * *tinf);
/*      Eq. A13, A14a, & A15 */
    d__1 = (doublereal) (*tlb / tt);
    d__2 = (doublereal) (*alpha + 1.f + gamma);
    densa = *dlb * pow_dd(&d__1, &d__2) * exp(-(*s2) * gamma * zg2);
    ret_val = densa;
    if (*alt >= *za) {
	goto L50;
    }
/*      CALCULATE DENSITY BELOW ZA */
/*      Eq. A17b */
/* Computing 2nd power */
    r__1 = *za / parmb_1.re + 1.f;
    glb = parmb_1.gsurf / (r__1 * r__1);
/*      Eq. A16b */
    gamm = *xm * glb * zg0 / rgas;
/*      Eq. A13, A14b, & A15 */
    d__1 = (doublereal) (ta / *tz);
    d__2 = (doublereal) (*alpha + 1.f);
    ret_val = densa * pow_dd(&d__1, &d__2) * exp(gamm * ((x - 1) / *t0 + bb * 
	    (x * x2 - 1.f) / 3.f + cc * (x2 * x2 * x - 1.f) / 5.f + dd * (x2 *
	     x2 * x2 * x - 1.f) / 7.f));
L50:
/* CCCCCWRITE(6,100)CXM,ALT,ZA,TINF,TLB,S2,T0,S1,TA,TZ,DLB,DENSA,DENSS */
/* C100 FORMAT(' D',1P13E10.2) */
    return ret_val;
} /* denss_ */

/* -------------------------------------------------------------------- */
real globe5_(real *yrd, real *sec, real *lat, real *long__, real *tloc, real *
	f107a, real *f107, real *ap, real *p)
{
    /* Initialized data */

    static real sr = 7.2722e-5f;
    static real sv[25] = { 1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,
	    1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f };
    static integer nsw = 14;
    static real p39 = -1e3f;
    static real dgtr = .0174533f;
    static real dr = .0172142f;
    static real xl = 1e3f;
    static real tll = 1e3f;
    static real dayl = -1.f;
    static real p14 = -1e3f;
    static real p18 = -1e3f;
    static real p32 = -1e3f;
    static real hr = .2618f;

    /* System generated locals */
    integer i__1;
    real ret_val, r__1, r__2, r__3, r__4, r__5, r__6;
    doublereal d__1;

    /* Builtin functions */
    double sin(doublereal), cos(doublereal), exp(doublereal), pow_dd(
	    doublereal *, doublereal *);

    /* Local variables */
    static real c__;
    static integer i__;
    static real s, c2, c4, f1, f2, s2, p44, p45, t71, t72, t81, t82, cd14, 
	    cd32, cd18, cd39, c2d14, exp1, exp2;
    extern /* Subroutine */ int tselec_(real *);

/*       CALCULATE G(L) FUNCTION FOR MSIS-86/CIRA 1986 */
/*       Upper Thermosphere Parameters */
/* !!OLD!! DIMENSION P(1),SV(25),AP(1) !!!!!!!!!!!!!!!!!!!!!!!!!!! */
    /* Parameter adjustments */
    --p;
    --ap;

    /* Function Body */
/* Eq. A24d */
/* Eq. A24c */
/* Eq. A24a */
    if (csw_1.isw != 64999) {
	tselec_(sv);
    }
    ttest_2.t[9] = 0.f;
    ttest_2.t[10] = 0.f;
    ttest_2.t[11] = 0.f;
    ttest_2.t[12] = 0.f;
/* L10: */
    lpoly_1.iyr = *yrd / 1e3f;
    lpoly_1.day = *yrd - lpoly_1.iyr * 1e3f;
/* Eq. A22 (remainder of code) */
    if (xl == *lat) {
	goto L15;
    }
/* CALCULATE LEGENDRE POLYNOMIALS */
    c__ = sin(*lat * dgtr);
    s = cos(*lat * dgtr);
    c2 = c__ * c__;
    c4 = c2 * c2;
    s2 = s * s;
    lpoly_1.plg[1] = c__;
    lpoly_1.plg[2] = (c2 * 3.f - 1.f) * .5f;
    lpoly_1.plg[3] = (c__ * 5.f * c2 - c__ * 3.f) * .5f;
    lpoly_1.plg[4] = (c4 * 35.f - c2 * 30.f + 3.f) / 8.f;
    lpoly_1.plg[5] = (c2 * 63.f * c2 * c__ - c2 * 70.f * c__ + c__ * 15.f) / 
	    8.f;
    lpoly_1.plg[6] = (c__ * 11.f * lpoly_1.plg[5] - lpoly_1.plg[4] * 5.f) / 
	    6.f;
    lpoly_1.plg[10] = s;
    lpoly_1.plg[11] = c__ * 3.f * s;
    lpoly_1.plg[12] = (c2 * 5.f - 1.f) * 1.5f * s;
    lpoly_1.plg[13] = (c2 * 7.f * c__ - c__ * 3.f) * 2.5f * s;
    lpoly_1.plg[14] = (c4 * 21.f - c2 * 14.f + 1.f) * 1.875f * s;
    lpoly_1.plg[15] = (c__ * 11.f * lpoly_1.plg[14] - lpoly_1.plg[13] * 6.f) /
	     5.f;
    lpoly_1.plg[20] = s2 * 3.f;
    lpoly_1.plg[21] = s2 * 15.f * c__;
    lpoly_1.plg[22] = (c2 * 7.f - 1.f) * 7.5f * s2;
    lpoly_1.plg[23] = c__ * 3.f * lpoly_1.plg[22] - lpoly_1.plg[21] * 2.f;
    lpoly_1.plg[24] = (c__ * 11.f * lpoly_1.plg[23] - lpoly_1.plg[22] * 7.f) /
	     4.f;
    lpoly_1.plg[25] = (c__ * 13.f * lpoly_1.plg[24] - lpoly_1.plg[23] * 8.f) /
	     5.f;
    lpoly_1.plg[30] = s2 * 15.f * s;
    lpoly_1.plg[31] = s2 * 105.f * s * c__;
    lpoly_1.plg[32] = (c__ * 9.f * lpoly_1.plg[31] - lpoly_1.plg[30] * 7.f) / 
	    2.f;
    lpoly_1.plg[33] = (c__ * 11.f * lpoly_1.plg[32] - lpoly_1.plg[31] * 8.f) /
	     3.f;
    xl = *lat;
L15:
    if (tll == *tloc) {
	goto L16;
    }
    lpoly_1.stloc = sin(hr * *tloc);
    lpoly_1.ctloc = cos(hr * *tloc);
    lpoly_1.s2tloc = sin(hr * 2.f * *tloc);
    lpoly_1.c2tloc = cos(hr * 2.f * *tloc);
    lpoly_1.s3tloc = sin(hr * 3.f * *tloc);
    lpoly_1.c3tloc = cos(hr * 3.f * *tloc);
    tll = *tloc;
L16:
    if (lpoly_1.day != dayl || p[14] != p14) {
	cd14 = cos(dr * (lpoly_1.day - p[14]));
    }
    if (lpoly_1.day != dayl || p[14] != p14) {
	c2d14 = cos(dr * 2 * (lpoly_1.day - p[14]));
    }
    if (lpoly_1.day != dayl || p[18] != p18) {
	cd18 = cos(dr * 2.f * (lpoly_1.day - p[18]));
    }
    if (lpoly_1.day != dayl || p[32] != p32) {
	cd32 = cos(dr * (lpoly_1.day - p[32]));
    }
    if (lpoly_1.day != dayl || p[39] != p39) {
	cd39 = cos(dr * 2.f * (lpoly_1.day - p[39]));
    }
    dayl = lpoly_1.day;
    p14 = p[14];
    p18 = p[18];
    p32 = p[32];
    p39 = p[39];
/*         F10.7 EFFECT */
    lpoly_1.df = *f107 - *f107a;
    lpoly_1.dfa = *f107a - 150.f;
/* Computing 2nd power */
    r__1 = lpoly_1.dfa;
    ttest_2.t[0] = p[20] * lpoly_1.df + p[21] * lpoly_1.df * lpoly_1.df + p[
	    22] * lpoly_1.dfa + p[30] * (r__1 * r__1);
    f1 = (p[48] * lpoly_1.dfa + p[20] * lpoly_1.df + p[21] * lpoly_1.df * 
	    lpoly_1.df) * csw_1.swc[0] + 1.f;
    f2 = (p[50] * lpoly_1.dfa + p[20] * lpoly_1.df + p[21] * lpoly_1.df * 
	    lpoly_1.df) * csw_1.swc[0] + 1.f;
/*        TIME INDEPENDENT */
    ttest_2.t[1] = p[2] * lpoly_1.plg[2] + p[3] * lpoly_1.plg[4] + p[23] * 
	    lpoly_1.plg[6] + p[15] * lpoly_1.plg[2] * lpoly_1.dfa * csw_1.swc[
	    0] + p[27] * lpoly_1.plg[1];
/*        SYMMETRICAL ANNUAL */
    ttest_2.t[2] = p[19] * cd32;
/*        SYMMETRICAL SEMIANNUAL */
    ttest_2.t[3] = (p[16] + p[17] * lpoly_1.plg[2]) * cd18;
/*        ASYMMETRICAL ANNUAL */
    ttest_2.t[4] = f1 * (p[10] * lpoly_1.plg[1] + p[11] * lpoly_1.plg[3]) * 
	    cd14;
/*         ASYMMETRICAL SEMIANNUAL */
    ttest_2.t[5] = p[38] * lpoly_1.plg[1] * cd39;
/*        DIURNAL */
    t71 = (p[12] * lpoly_1.plg[11] + p[36] * lpoly_1.plg[10]) * cd14 * 
	    csw_1.swc[4];
    t72 = (p[13] * lpoly_1.plg[11] + p[37] * lpoly_1.plg[10]) * cd14 * 
	    csw_1.swc[4];
    ttest_2.t[6] = f2 * ((p[4] * lpoly_1.plg[10] + p[5] * lpoly_1.plg[12] + p[
	    28] * lpoly_1.plg[14] + t71) * lpoly_1.ctloc + (p[7] * 
	    lpoly_1.plg[10] + p[8] * lpoly_1.plg[12] + p[29] * lpoly_1.plg[14]
	     + t72) * lpoly_1.stloc);
/*        SEMIDIURNAL */
    t81 = p[24] * lpoly_1.plg[21] * cd14 * csw_1.swc[4];
    t82 = p[34] * lpoly_1.plg[21] * cd14 * csw_1.swc[4];
    ttest_2.t[7] = f2 * ((p[6] * lpoly_1.plg[20] + p[42] * lpoly_1.plg[22] + 
	    t81) * lpoly_1.c2tloc + (p[9] * lpoly_1.plg[20] + p[43] * 
	    lpoly_1.plg[22] + t82) * lpoly_1.s2tloc);
/*        TERDIURNAL */
    ttest_2.t[13] = f2 * ((p[40] * lpoly_1.plg[30] + (p[94] * lpoly_1.plg[31] 
	    + p[47] * lpoly_1.plg[33]) * cd14 * csw_1.swc[4]) * 
	    lpoly_1.s3tloc + (p[41] * lpoly_1.plg[30] + (p[95] * lpoly_1.plg[
	    31] + p[49] * lpoly_1.plg[33]) * cd14 * csw_1.swc[4]) * 
	    lpoly_1.c3tloc);
/*          MAGNETIC ACTIVITY BASED ON DAILY AP */
    if (csw_1.sw[8] == -1.f && p[52] != 0.f) {
	goto L30;
    }
    lpoly_1.apd = ap[1] - 4.f;
    p44 = p[44];
    p45 = p[45];
    if (p44 < 0.f) {
	p44 = 1e-5f;
    }
    lpoly_1.apdf = lpoly_1.apd + (p45 - 1.f) * (lpoly_1.apd + (exp(-p44 * 
	    lpoly_1.apd) - 1.f) / p44);
    ttest_2.t[8] = lpoly_1.apdf * (p[33] + p[46] * lpoly_1.plg[2] + p[35] * 
	    lpoly_1.plg[4] + (p[101] * lpoly_1.plg[1] + p[102] * lpoly_1.plg[
	    3] + p[103] * lpoly_1.plg[5]) * cd14 * csw_1.swc[4] + (p[122] * 
	    lpoly_1.plg[10] + p[123] * lpoly_1.plg[12] + p[124] * lpoly_1.plg[
	    14]) * csw_1.swc[6] * cos(hr * (*tloc - p[125])));
    goto L40;
L30:
    exp1 = exp(abs(p[52]) * -10800.f / (p[139] * (45.f - abs(*lat)) + 1.f));
    if (exp1 > .99999f) {
	exp1 = .99999f;
    }
    exp2 = exp(abs(p[54]) * -10800.f);
    if (exp2 > .99999f) {
	exp2 = .99999f;
    }
    if (p[25] < 1e-4f) {
	p[25] = 1e-4f;
    }
/* Computing 3rd power */
    r__1 = exp1;
/* Computing 4th power */
    r__2 = exp1, r__2 *= r__2;
/* Computing 12th power */
    r__3 = exp1, r__3 *= r__3, r__3 *= r__3;
/* Computing 8th power */
    r__4 = exp1, r__4 *= r__4, r__4 *= r__4;
/* Computing 19th power */
    r__5 = exp1, r__6 = r__5, r__5 *= r__5, r__6 *= r__5, r__5 *= r__5, r__5 
	    *= r__5;
    d__1 = (doublereal) exp1;
    lpoly_1.apt[0] = (ap[2] - 4.f + (p[26] - 1.f) * (ap[2] - 4.f + (exp(-abs(
	    p[25]) * (ap[2] - 4.f)) - 1.f) / abs(p[25])) + ((ap[3] - 4.f + (p[
	    26] - 1.f) * (ap[3] - 4.f + (exp(-abs(p[25]) * (ap[3] - 4.f)) - 
	    1.f) / abs(p[25]))) * exp1 + (ap[4] - 4.f + (p[26] - 1.f) * (ap[4]
	     - 4.f + (exp(-abs(p[25]) * (ap[4] - 4.f)) - 1.f) / abs(p[25]))) *
	     exp1 * exp1 + (ap[5] - 4.f + (p[26] - 1.f) * (ap[5] - 4.f + (exp(
	    -abs(p[25]) * (ap[5] - 4.f)) - 1.f) / abs(p[25]))) * (r__1 * (
	    r__1 * r__1)) + ((ap[6] - 4.f + (p[26] - 1.f) * (ap[6] - 4.f + (
	    exp(-abs(p[25]) * (ap[6] - 4.f)) - 1.f) / abs(p[25]))) * (r__2 * 
	    r__2) + (ap[7] - 4.f + (p[26] - 1.f) * (ap[7] - 4.f + (exp(-abs(p[
	    25]) * (ap[7] - 4.f)) - 1.f) / abs(p[25]))) * (r__3 * (r__3 * 
	    r__3))) * (1.f - r__4 * r__4) / (1.f - exp1))) / (1.f + (1.f - 
	    r__6 * (r__5 * r__5)) / (1.f - exp1) * pow_dd(&d__1, &c_b100));
/* Computing 3rd power */
    r__1 = exp2;
/* Computing 4th power */
    r__2 = exp2, r__2 *= r__2;
/* Computing 12th power */
    r__3 = exp2, r__3 *= r__3, r__3 *= r__3;
/* Computing 8th power */
    r__4 = exp2, r__4 *= r__4, r__4 *= r__4;
/* Computing 19th power */
    r__5 = exp2, r__6 = r__5, r__5 *= r__5, r__6 *= r__5, r__5 *= r__5, r__5 
	    *= r__5;
    d__1 = (doublereal) exp2;
    lpoly_1.apt[2] = (ap[2] - 4.f + (p[26] - 1.f) * (ap[2] - 4.f + (exp(-abs(
	    p[25]) * (ap[2] - 4.f)) - 1.f) / abs(p[25])) + ((ap[3] - 4.f + (p[
	    26] - 1.f) * (ap[3] - 4.f + (exp(-abs(p[25]) * (ap[3] - 4.f)) - 
	    1.f) / abs(p[25]))) * exp2 + (ap[4] - 4.f + (p[26] - 1.f) * (ap[4]
	     - 4.f + (exp(-abs(p[25]) * (ap[4] - 4.f)) - 1.f) / abs(p[25]))) *
	     exp2 * exp2 + (ap[5] - 4.f + (p[26] - 1.f) * (ap[5] - 4.f + (exp(
	    -abs(p[25]) * (ap[5] - 4.f)) - 1.f) / abs(p[25]))) * (r__1 * (
	    r__1 * r__1)) + ((ap[6] - 4.f + (p[26] - 1.f) * (ap[6] - 4.f + (
	    exp(-abs(p[25]) * (ap[6] - 4.f)) - 1.f) / abs(p[25]))) * (r__2 * 
	    r__2) + (ap[7] - 4.f + (p[26] - 1.f) * (ap[7] - 4.f + (exp(-abs(p[
	    25]) * (ap[7] - 4.f)) - 1.f) / abs(p[25]))) * (r__3 * (r__3 * 
	    r__3))) * (1.f - r__4 * r__4) / (1.f - exp2))) / (1.f + (1.f - 
	    r__6 * (r__5 * r__5)) / (1.f - exp2) * pow_dd(&d__1, &c_b100));
    ttest_2.t[8] = lpoly_1.apt[0] * (p[51] + p[97] * lpoly_1.plg[2] + p[55] * 
	    lpoly_1.plg[4] + (p[126] * lpoly_1.plg[1] + p[127] * lpoly_1.plg[
	    3] + p[128] * lpoly_1.plg[5]) * cd14 * csw_1.swc[4] + (p[129] * 
	    lpoly_1.plg[10] + p[130] * lpoly_1.plg[12] + p[131] * lpoly_1.plg[
	    14]) * csw_1.swc[6] * cos(hr * (*tloc - p[132])));
L40:
    if (csw_1.sw[9] == 0.f || *long__ <= -1e3f) {
	goto L49;
    }
/*        LONGITUDINAL */
    ttest_2.t[10] = (p[90] * lpoly_1.plg[1] + 1.f) * (p[81] * lpoly_1.dfa * 
	    csw_1.swc[0] + 1.f) * ((p[65] * lpoly_1.plg[11] + p[66] * 
	    lpoly_1.plg[13] + p[67] * lpoly_1.plg[15] + p[104] * lpoly_1.plg[
	    10] + p[105] * lpoly_1.plg[12] + p[106] * lpoly_1.plg[14] + 
	    csw_1.swc[4] * (p[110] * lpoly_1.plg[10] + p[111] * lpoly_1.plg[
	    12] + p[112] * lpoly_1.plg[14]) * cd14) * cos(dgtr * *long__) + (
	    p[91] * lpoly_1.plg[11] + p[92] * lpoly_1.plg[13] + p[93] * 
	    lpoly_1.plg[15] + p[107] * lpoly_1.plg[10] + p[108] * lpoly_1.plg[
	    12] + p[109] * lpoly_1.plg[14] + csw_1.swc[4] * (p[113] * 
	    lpoly_1.plg[10] + p[114] * lpoly_1.plg[12] + p[115] * lpoly_1.plg[
	    14]) * cd14) * sin(dgtr * *long__));
/*        UT AND MIXED UT,LONGITUDE */
    ttest_2.t[11] = (p[96] * lpoly_1.plg[1] + 1.f) * (p[82] * lpoly_1.dfa * 
	    csw_1.swc[0] + 1.f) * (p[120] * lpoly_1.plg[1] * csw_1.swc[4] * 
	    cd14 + 1.f) * ((p[69] * lpoly_1.plg[1] + p[70] * lpoly_1.plg[3] + 
	    p[71] * lpoly_1.plg[5]) * cos(sr * (*sec - p[72])));
    ttest_2.t[11] += csw_1.swc[10] * (p[77] * lpoly_1.plg[21] + p[78] * 
	    lpoly_1.plg[23] + p[79] * lpoly_1.plg[25]) * cos(sr * (*sec - p[
	    80]) + dgtr * 2.f * *long__) * (p[138] * lpoly_1.dfa * csw_1.swc[
	    0] + 1.f);
/*        UT,LONGITUDE MAGNETIC ACTIVITY */
    if (csw_1.sw[8] == -1.f && p[52] != 0.f) {
	goto L45;
    }
    ttest_2.t[12] = lpoly_1.apdf * csw_1.swc[10] * (p[121] * lpoly_1.plg[1] + 
	    1.f) * ((p[61] * lpoly_1.plg[11] + p[62] * lpoly_1.plg[13] + p[63]
	     * lpoly_1.plg[15]) * cos(dgtr * (*long__ - p[64]))) + 
	    lpoly_1.apdf * csw_1.swc[10] * csw_1.swc[4] * (p[116] * 
	    lpoly_1.plg[10] + p[117] * lpoly_1.plg[12] + p[118] * lpoly_1.plg[
	    14]) * cd14 * cos(dgtr * (*long__ - p[119])) + lpoly_1.apdf * 
	    csw_1.swc[11] * (p[84] * lpoly_1.plg[1] + p[85] * lpoly_1.plg[3] 
	    + p[86] * lpoly_1.plg[5]) * cos(sr * (*sec - p[76]));
    goto L48;
L45:
    ttest_2.t[12] = lpoly_1.apt[0] * csw_1.swc[10] * (p[133] * lpoly_1.plg[1] 
	    + 1.f) * ((p[53] * lpoly_1.plg[11] + p[99] * lpoly_1.plg[13] + p[
	    68] * lpoly_1.plg[15]) * cos(dgtr * (*long__ - p[98]))) + 
	    lpoly_1.apt[0] * csw_1.swc[10] * csw_1.swc[4] * (p[134] * 
	    lpoly_1.plg[10] + p[135] * lpoly_1.plg[12] + p[136] * lpoly_1.plg[
	    14]) * cd14 * cos(dgtr * (*long__ - p[137])) + lpoly_1.apt[0] * 
	    csw_1.swc[11] * (p[56] * lpoly_1.plg[1] + p[57] * lpoly_1.plg[3] 
	    + p[58] * lpoly_1.plg[5]) * cos(sr * (*sec - p[59]));
L48:
/*  PARMS NOT USED: 60,83,100,140-150 */
L49:
    ttest_2.tinf = 0.f;
    if (csw_1.sw[8] == -1.f) {
	ttest_2.tinf = p[31];
    }
    i__1 = nsw;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L50: */
	ttest_2.tinf += (r__1 = csw_1.sw[i__ - 1], abs(r__1)) * ttest_2.t[i__ 
		- 1];
    }
    ret_val = ttest_2.tinf;
    return ret_val;
} /* globe5_ */

/* -------------------------------------------------------------------- */
/*     SUBROUTINE TSELEC(SV) */
/*        SET SWITCHES */
/*     DIMENSION SV(1),SAV(25),SVV(1) */
/*     COMMON/CSW/SW(25),ISW,SWC(25) */
/*     DO 100 I = 1,25 */
/*       SAV(I)=SV(I) */
/*       SW(I)=AMOD(SV(I),2.) */
/*       IF(ABS(SV(I)).GT.0.) THEN */
/*         SWC(I)=1. */
/*       ELSE */
/*         SWC(I)=0. */
/*       ENDIF */
/* 100 CONTINUE */
/*     ISW=64999 */
/*     RETURN */
/*     ENTRY TRETRV(SVV) */
/*     DO 200 I=1,25 */
/*       SVV(I)=SAV(I) */
/* 200 CONTINUE */
/*     END */
/* -------------------------------------------------------------------- */
real glob5l_(real *p)
{
    /* Initialized data */

    static real dr = .0172142f;
    static real t[15] = { 0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f };
    static real dayl = -1.f;
    static real p7 = -1e3f;
    static real p9 = -1e3f;
    static real p11 = -1e3f;

    /* System generated locals */
    real ret_val, r__1;

    /* Builtin functions */
    double cos(doublereal);

    /* Local variables */
    static integer i__;
    static real tt, cd7, cd9, cd11;

/*      LIMITED PARAMETER VERSION OF GLOBE 9/2/82 */
/*       CALCULATE G(L) FUNCTION FOR MSIS-86/CIRA 1986 */
/*       Lower Thermosphere Parameters */
/* !!OLD!! DIMENSION P(1),T(15) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    /* Parameter adjustments */
    --p;

    /* Function Body */
    if (lpoly_1.day != dayl || p7 != p[7]) {
	cd7 = cos(dr * (lpoly_1.day - p[7]));
    }
    if (lpoly_1.day != dayl || p9 != p[9]) {
	cd9 = cos(dr * 2.f * (lpoly_1.day - p[9]));
    }
    if (lpoly_1.day != dayl || p11 != p[11]) {
	cd11 = cos(dr * (lpoly_1.day - p[11]));
    }
    dayl = lpoly_1.day;
    p7 = p[7];
    p9 = p[9];
    p11 = p[11];

    t[0] = p[2] * lpoly_1.dfa;
    t[1] = p[4] * lpoly_1.plg[2];
    t[2] = p[6] * cd7;
    t[3] = p[8] * cd9;
    t[4] = (p[10] * lpoly_1.plg[1] + p[22] * lpoly_1.plg[3]) * cd11;
    t[5] = 0.f;
    t[6] = p[14] * lpoly_1.plg[10] * lpoly_1.ctloc + p[15] * lpoly_1.plg[10] *
	     lpoly_1.stloc;
    t[7] = (p[16] * lpoly_1.plg[20] + p[18] * lpoly_1.plg[22] + p[20] * 
	    lpoly_1.plg[23] * cd11 * csw_1.swc[4]) * lpoly_1.c2tloc + (p[17] *
	     lpoly_1.plg[20] + p[19] * lpoly_1.plg[22] + p[21] * lpoly_1.plg[
	    23] * cd11 * csw_1.swc[4]) * lpoly_1.s2tloc;
    t[13] = p[12] * lpoly_1.plg[30] * lpoly_1.c3tloc + p[25] * lpoly_1.plg[30]
	     * lpoly_1.s3tloc;
    if (csw_1.sw[8] == 1.f) {
	t[8] = lpoly_1.apdf * (p[23] + p[24] * lpoly_1.plg[2] * csw_1.swc[1]);
    }
    if (csw_1.sw[8] == -1.f) {
	t[8] = p[3] * lpoly_1.apt[2] + p[5] * lpoly_1.plg[2] * lpoly_1.apt[2] 
		* csw_1.swc[1];
    }
/*       PARMS NOT USED: 13 */
    tt = 0.f;
    for (i__ = 1; i__ <= 14; ++i__) {
/* L50: */
	tt += (r__1 = csw_1.sw[i__ - 1], abs(r__1)) * t[i__ - 1];
    }
    ret_val = tt;
    return ret_val;
} /* glob5l_ */

/* -------------------------------------------------------------------- */
/*     FUNCTION DNET(DD,DM,ZHM,XMM,XM) */
/*       8/20/80 */
/*       TURBOPAUSE CORRECTION FOR MSIS MODELS */
/*       Eq. A12b */
/*     A=ZHM/(XMM-XM) */
/*       Eq. A12a */
/*     YLOG=A*ALOG(DM/DD) */
/*     IF(YLOG.LT.-10.) GO TO 10 */
/*     IF(YLOG.GT.10.)  GO TO 20 */
/*       DNET=DD*(1.+EXP(YLOG))**(1/A) */
/*       GO TO 50 */
/*  10 CONTINUE */
/*       DNET=DD */
/*       GO TO 50 */
/*  20 CONTINUE */
/*       DNET=DM */
/*       GO TO 50 */
/*  50 CONTINUE */
/*     RETURN */
/*     END */
/* -------------------------------------------------------------------- */
/*     FUNCTION  CCOR(ALT, R,H1,ZH) */
/*        CHEMISTRY/DISSOCIATION CORRECTION FOR MSIS MODELS */
/*     Eq. A20a or Eq. A21 */
/*     E=(ALT-ZH)/H1 */
/*     IF(E.GT.70.) GO TO 20 */
/*     IF(E.LT.-70.) GO TO 10 */
/*       EX=EXP(E) */
/*       CCOR=R/(1.+EX) */
/*       GO TO 50 */
/*  10   CCOR=R */
/*       GO TO 50 */
/*  20   CCOR=0. */
/*       GO TO 50 */
/*  50 CONTINUE */
/*     CCOR=EXP(CCOR) */
/*      RETURN */
/*     END */
/* -------------------------------------------------------------------- */
/* Subroutine */ int prmsg5_(char *path, ftnlen path_len)
{
    /* Initialized data */

    static char isd[1*3] = "1" "E" "6";
    static char ist[1*2] = "1" "3";

    /* Format strings */
    static char fmt_11111[] = "(a,\002MSIS86.DAT\002)";
    static char fmt_1290[] = "(1x,5e13.6)";
    static char fmt_2396[] = "(\002 THE MSIS COEFFICIENT FILE (\002,a,\002) "
	    "IS NOT IN\002,\002 YOUR DIRECTORY.\002)";
    static char fmt_2397[] = "(\002 ERROR IN READING THE MSIS COEFFICIENT FI"
	    "LE (\002,a,\002)\002)";

    /* System generated locals */
    integer i__1;
    olist o__1;

    /* Builtin functions */
    integer s_wsfi(icilist *), do_fio(integer *, char *, ftnlen), e_wsfi(void)
	    , f_open(olist *), s_rsfe(cilist *), e_rsfe(void), s_wsfe(cilist *
	    ), e_wsfe(void);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);

    /* Local variables */
    static integer i__;
    static char fname[99];

    /* Fortran I/O blocks */
    static icilist io___139 = { 0, fname, 0, fmt_11111, 99, 1 };
    static cilist io___140 = { 1, 0, 0, fmt_1290, 0 };
    static cilist io___141 = { 0, 6, 0, fmt_2396, 0 };
    static cilist io___142 = { 0, 6, 0, fmt_2397, 0 };


/*          CIRA     11-FEB-86 */
    parmb_1.gsurf = 980.665f;
    parmb_1.re = 6356.77f;
    uinr_1.iiee = 0;
    s_wsfi(&io___139);
    do_fio(&c__1, path, path_len);
    e_wsfi();
/* L2392: */
    o__1.oerr = 1;
    o__1.ounit = uinr_1.iumsis;
    o__1.ofnmlen = 99;
    o__1.ofnm = fname;
    o__1.orl = 0;
    o__1.osta = "OLD";
    o__1.oacc = 0;
    o__1.ofm = "FORMATTED";
    o__1.oblnk = 0;
    i__1 = f_open(&o__1);
    if (i__1 != 0) {
	goto L2399;
    }
    io___140.ciunit = uinr_1.iumsis;
    i__1 = s_rsfe(&io___140);
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pt1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pt2[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pt3[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pa1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pa2[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pa3[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pb1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pb2[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pb3[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pc1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pc2[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pc3[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pd1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pd2[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pd3[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pe1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pe2[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pe3[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pf1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pf2[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pf3[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pg1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pg2[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pg3[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.ph1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.ph2[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.ph3[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__50, (char *)&parm5_2.pi1[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__8, (char *)&lower5_1.ptm[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = do_fio(&c__56, (char *)&lower5_1.pdm[0], (ftnlen)sizeof(real));
    if (i__1 != 0) {
	goto L2390;
    }
    i__1 = e_rsfe();
    if (i__1 != 0) {
	goto L2390;
    }

/* CCCCCCCCCCCC IN PC VERSION: 1290    FORMAT(5E13.6) */

    goto L2395;
L2399:
    s_wsfe(&io___141);
    do_fio(&c__1, fname, (ftnlen)99);
    e_wsfe();
    uinr_1.iiee = 1;
    return 0;
L2390:
    s_wsfe(&io___142);
    do_fio(&c__1, fname, (ftnlen)99);
    e_wsfe();
    uinr_1.iiee = 1;
    return 0;
L2395:
    for (i__ = 1; i__ <= 3; ++i__) {
	*(unsigned char *)&datime_2.isdate[i__ - 1] = *(unsigned char *)&isd[
		i__ - 1];
/* L10: */
    }
    for (i__ = 1; i__ <= 2; ++i__) {
	s_copy(datime_2.istime + (i__ - 1 << 2), ist + (i__ - 1), (ftnlen)4, (
		ftnlen)1);
/* L20: */
    }
    s_copy(datime_2.name__, "CIRA", (ftnlen)1, (ftnlen)4);
    s_copy(datime_2.name__ + 1, "-86 ", (ftnlen)1, (ftnlen)4);
    return 0;
} /* prmsg5_ */

/* ------------------------------------------------------------------ */
/* Subroutine */ int ggm_(integer *art, real *xlg, real *bg, real *xlm, real *
	bm)
{
    /* Builtin functions */
    double cos(doublereal), sin(doublereal), asin(doublereal), r_sign(real *, 
	    real *), acos(doublereal);

    /* Local variables */
    static real ci, si, cbg, cbm, clg, clm, sbg, sbm, slg, slm, ylg, zpi, 
	    faktor;

/* CALCULATES GEOMAGNETIC LONGITUDE (XLM) AND LATITUDE (BM) */
/* FROM GEOGRAFIC LONGITUDE (XLG) AND LATITUDE (BG) FOR ART=0 */
/* AND REVERSE FOR ART=1. ALL ANGLES IN DEGREE. */
/* LATITUDE:-90 TO 90. LONGITUDE:0 TO 360 EAST. */
    faktor = .0174532925f;
    zpi = faktor * 360.f;
    cbg = faktor * 11.4f;
    ci = cos(cbg);
    si = sin(cbg);
    if (*art == 0) {
	goto L10;
    }
    cbm = cos(*bm * faktor);
    sbm = sin(*bm * faktor);
    clm = cos(*xlm * faktor);
    slm = sin(*xlm * faktor);
    sbg = sbm * ci - cbm * clm * si;
    *bg = asin(sbg);
    cbg = cos(*bg);
    slg = cbm * slm / cbg;
    clg = (sbm * si + cbm * clm * ci) / cbg;
    if (abs(clg) > 1.f) {
	clg = r_sign(&c_b46, &clg);
    }
    *xlg = acos(clg);
    if (slg < 0.f) {
	*xlg = zpi - acos(clg);
    }
    *bg /= faktor;
    *xlg /= faktor;
    *xlg += -69.8f;
    if (*xlg < 0.f) {
	*xlg += 360.f;
    }
    return 0;
L10:
    ylg = *xlg + 69.8f;
    cbg = cos(*bg * faktor);
    sbg = sin(*bg * faktor);
    clg = cos(ylg * faktor);
    slg = sin(ylg * faktor);
    sbm = sbg * ci + cbg * clg * si;
    *bm = asin(sbm);
    cbm = cos(*bm);
    slm = cbg * slg / cbm;
    clm = (-sbg * si + cbg * clg * ci) / cbm;
    *xlm = acos(clm);
    if (slm < 0.f) {
	*xlm = zpi - acos(clm);
    }
    *bm /= faktor;
    *xlm /= faktor;
    return 0;
} /* ggm_ */

