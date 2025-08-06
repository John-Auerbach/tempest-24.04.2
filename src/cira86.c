/* cira86.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int cira86_(integer *iday, real *sec, real *glat, real *
	glong, real *stl, real *f107a, real *tinf, real *tlb, real *sigma)
{
    /* Initialized data */

    static real dr = .0172142f;
    static real dr2 = .0344284f;
    static real hr = .2618f;
    static real sr = 7.2722e-5f;
    static real xl = 1e3f;
    static real tll = 1e3f;
    static real dgtr = .0174533f;

    /* Builtin functions */
    double sin(doublereal), cos(doublereal);

    /* Local variables */
    static real c__, s, c2, c4, f1, f2, f3, g0, s2, t1, t2, t3, t5, t4, t7, 
	    t8, z1, z2, t11, t12, t14, t71, t72, t81, t82, zz, cd9, cd11, dfa,
	     cd14, cd32, cd18, cd39, plg[36]	/* was [9][4] */, t7814, 
	    ctloc, stloc, c2tloc, c3tloc, s2tloc, s3tloc;

/* ******************************************************************* */
/*   Calculates neutral temperature parameters for IRI using the */
/*   MSIS-86/CIRA 1986 Neutral Thermosphere Model. The subroutines */
/*   GTS5, GLOBE5 and GLOBL5 developed by A.E. Hedin (2/26/87) were */
/*   modified for use in IRI --------- D. Bilitza -------- March 1991 */

/*     INPUT: */
/*        IDAY - DAY OF YEAR */
/*        SEC - UT(SEC) */
/*        GLAT - GEODETIC LATITUDE(DEG) */
/*        GLONG - GEODETIC LONGITUDE(DEG) */
/*        STL - LOCAL APPARENT SOLAR TIME(HRS) */
/*        F107A - 3 MONTH AVERAGE OF F10.7 FLUX */

/*     OUTPUT: */
/*        TINF - EXOSPHERIC TEMPERATURE (K) */
/*        TLB - TEMPERATURE AT LOWER BOUNDARY (K) */
/*        SIGMA - SHAPE PARAMETER FOR TEMPERATURE PROFILE */

/* ********************************************************************** */

/* CALCULATE LEGENDRE POLYNOMIALS */

    if (xl == *glat) {
	goto L15;
    }
    c__ = sin(*glat * dgtr);
    s = cos(*glat * dgtr);
    c2 = c__ * c__;
    c4 = c2 * c2;
    s2 = s * s;
    plg[1] = c__;
    plg[2] = (c2 * 3.f - 1.f) * .5f;
    plg[3] = (c__ * 5.f * c2 - c__ * 3.f) * .5f;
    plg[4] = (c4 * 35.f - c2 * 30.f + 3.f) / 8.f;
    plg[5] = (c2 * 63.f * c2 * c__ - c2 * 70.f * c__ + c__ * 15.f) / 8.f;
    plg[10] = s;
    plg[11] = c__ * 3.f * s;
    plg[12] = (c2 * 5.f - 1.f) * 1.5f * s;
    plg[13] = (c2 * 7.f * c__ - c__ * 3.f) * 2.5f * s;
    plg[14] = (c4 * 21.f - c2 * 14.f + 1.f) * 1.875f * s;
    plg[15] = (c__ * 11.f * plg[14] - plg[13] * 6.f) / 5.f;
    plg[20] = s2 * 3.f;
    plg[21] = s2 * 15.f * c__;
    plg[22] = (c2 * 7.f - 1.f) * 7.5f * s2;
    plg[23] = c__ * 3.f * plg[22] - plg[21] * 2.f;
    plg[30] = s2 * 15.f * s;
    plg[31] = s2 * 105.f * s * c__;
    plg[32] = (c__ * 9.f * plg[31] - plg[30] * 7.f) / 2.f;
    plg[33] = (c__ * 11.f * plg[32] - plg[31] * 8.f) / 3.f;
    xl = *glat;
L15:
    if (tll == *stl) {
	goto L16;
    }
    stloc = sin(hr * *stl);
    ctloc = cos(hr * *stl);
    s2tloc = sin(hr * 2.f * *stl);
    c2tloc = cos(hr * 2.f * *stl);
    s3tloc = sin(hr * 3.f * *stl);
    c3tloc = cos(hr * 3.f * *stl);
    tll = *stl;
L16:

    dfa = *f107a - 150.f;

/* EXOSPHERIC TEMPERATURE */

/*         F10.7 EFFECT */
    t1 = (.00311701f - dfa * 6.4111e-6f) * dfa;
    f1 = dfa * .00426385f + 1.f;
    f2 = dfa * .00511819f + 1.f;
    f3 = dfa * .00292246f + 1.f;
/*        TIME INDEPENDENT */
    t2 = plg[2] * .0385528f + plg[4] * .00303445f;
/*        SYMMETRICAL ANNUAL AND SEMIANNUAL */
    cd14 = cos(dr * (*iday + 8.45398f));
    cd18 = cos(dr2 * (*iday - 125.818f));
    cd32 = cos(dr * (*iday - 30.015f));
    cd39 = cos(dr2 * (*iday - 2.75905f));
    t3 = cd32 * .00805486f + cd18 * .014237f;
/*        ASYMMETRICAL ANNUAL AND SEMIANNUAL */
    t5 = f1 * (plg[1] * -.127371f - plg[3] * .0302449f) * cd14 - plg[1] * 
	    .0192645f * cd39;
/*        DIURNAL */
    t71 = plg[11] * .0123512f * cd14;
    t72 = plg[11] * -.00526277f * cd14;
    t7 = (plg[10] * -.105531f - plg[12] * .00607134f + t71) * ctloc + (plg[10]
	     * -.115622f + plg[12] * .0020224f + t72) * stloc;
/*        SEMIDIURNAL */
    t81 = plg[21] * .00386578f * cd14;
    t82 = plg[21] * .00389146f * cd14;
    t8 = (plg[20] * -5.16278e-4f - plg[22] * .00117388f + t81) * c2tloc + (
	    plg[20] * .00990156f - plg[22] * 3.54589e-4f + t82) * s2tloc;
/*        TERDIURNAL */
    z1 = plg[31] * cd14;
    z2 = plg[33] * cd14;
    t14 = (plg[30] * .00147284f - z1 * 1.73933e-4f + z2 * 3.65016e-5f) * 
	    s3tloc + (plg[30] * 3.41345e-4f - z1 * 1.53218e-4f + z2 * 
	    1.15102e-4f) * c3tloc;
    t7814 = f2 * (t7 + t8 + t14);
/*        LONGITUDINAL */
    t11 = f3 * ((plg[11] * .00562606f + plg[13] * .00594053f + plg[15] * 
	    .00109358f - plg[10] * .00301801f - plg[12] * .00423564f - plg[14]
	     * .00248289f + (plg[10] * .00189689f + plg[12] * .00415654f) * 
	    cd14) * cos(dgtr * *glong) + (plg[11] * -.011654f - plg[13] * 
	    .00449173f - plg[15] * 3.53189e-4f + plg[10] * 9.19286e-4f + plg[
	    12] * .00216372f + plg[14] * 8.63968e-4f + (plg[10] * .0118068f + 
	    plg[12] * .0033119f) * cd14) * sin(dgtr * *glong));
/*        UT AND MIXED UT,LONGITUDE */
    t12 = (1.f - plg[1] * .565411f) * cos(sr * (*sec - 31137.f)) * (plg[1] * 
	    -.013341f - plg[3] * .0243409f - plg[5] * .0135688f) + (plg[21] * 
	    8.45583e-4f + plg[23] * 5.38706e-4f) * cos(sr * (*sec - 247.956f) 
	    + dgtr * 2.f * *glong);
/*  Exospheric temperature TINF/K  [Eq. A7] */
    *tinf = (t1 + 1.f + t2 + t3 + t5 + t7814 + t11 + t12) * 1041.3f * .99604f;

/* TEMPERATURE DERIVATIVE AT LOWER BOUNDARY */

/*         F10.7 EFFECT */
    t1 = dfa * .00252317f;
/*        TIME INDEPENDENT */
    t2 = plg[2] * -.0467542f + plg[4] * .12026f;
/*        ASYMMETRICAL ANNUAL */
    cd14 = cos(dr * (*iday + 8.45398f));
    t5 = plg[1] * -.13324f * cd14;
/*        SEMIDIURNAL */
    zz = plg[21] * cd14;
    t81 = zz * -.00973404f;
    t82 = zz * -7.18482e-4f;
    t8 = (plg[20] * .0191357f + plg[22] * .00787683f + t81) * c2tloc + (plg[
	    20] * .00125429f - plg[22] * .00233698f + t82) * s2tloc;
/*  dTn/dh at lower boundary  [Eq. A6] */
    g0 = (t1 + 1.f + t2 + t5 + t8) * 16.6728f * .951363f;

/* NEUTRAL TEMPERATURE AT LOWER BOUNDARY 120KM */

    cd9 = cos(dr2 * (*iday - 89.382f));
    cd11 = cos(dr * (*iday + 8.45398f));
    t1 = dfa * 5.68478e-4f;
    t4 = cd9 * .0107674f;
    t5 = plg[1] * -.0192414f * cd11;
    t7 = plg[10] * -.02002f * ctloc - plg[10] * .00195833f * stloc;
    t8 = (plg[20] * -.00938391f - plg[22] * .00260147f + plg[23] * 
	    5.11651e-5f * cd11) * c2tloc + (plg[20] * .013148f - plg[22] * 
	    8.08556e-4f + plg[23] * .00255717f * cd11) * s2tloc;
/*  Tn at lower boundary 120km   [Eq. A8] */
    *tlb = (t1 + 1.f + t4 + t5 + t7 + t8) * 386.f * .976619f;
/*  Sigma      [Eq. A5] */
    *sigma = g0 / (*tinf - *tlb);
    return 0;
} /* cira86_ */



real tn_(real *h__, real *tinf, real *tlbd, real *s)
{
    /* System generated locals */
    real ret_val;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    static real zg2;

/* -------------------------------------------------------------------- */
/*       Calculate Temperature for MSIS/CIRA-86 model */
/* -------------------------------------------------------------------- */
    zg2 = (*h__ - 120.f) * 6476.77f / (*h__ + 6356.77f);
    ret_val = *tinf - *tlbd * exp(-(*s) * zg2);
    return ret_val;
} /* tn_ */



real dtndh_(real *h__, real *tinf, real *tlbd, real *s)
{
    /* System generated locals */
    real ret_val;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    static real zg1, zg2, zg3;

/* --------------------------------------------------------------------- */
    zg1 = *h__ + 6356.77f;
    zg2 = 6476.77f / zg1;
    zg3 = (*h__ - 120.f) * zg2;
    ret_val = -(*tlbd) * exp(-(*s) * zg3) * (*s / zg1 * (zg3 - zg2));
    return ret_val;
} /* dtndh_ */

