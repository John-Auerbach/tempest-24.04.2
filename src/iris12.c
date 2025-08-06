/* iris12.f -- translated by f2c (version 20100827).
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
    real hmf2, nmf2, hmf1;
} block1_;

#define block1_1 block1_

struct {
    real umr;
} const_;

#define const_1 const_

struct {
    real b0, b1, c1;
} block2_;

#define block2_1 block2_

struct {
    real hz, t, hst, str;
} block3_;

#define block3_1 block3_

struct {
    real hme, nme, hef;
} block4_;

#define block4_1 block4_

struct {
    logical night;
    real e[4];
} block5_;

#define block5_1 block5_

struct {
    real hmd, nmd, hdx;
} block6_;

#define block6_1 block6_

struct {
    real d1, xkk, fp30, fp3u, fp1, fp2;
} block7_;

#define block7_1 block7_

struct {
    real hs, tnhs, xsm[4], mm[5], dti[4];
    integer mxsm;
} block8_;

#define block8_1 block8_

struct {
    real xsm1, texos, tlbdh, sigma;
} blotn_;

#define blotn_1 blotn_

struct {
    real ahh[7], ate1, stte[6], dte[5];
} blote_;

#define blote_1 blote_

struct {
    real beta, eta, delta, zeta;
} blo10_;

#define blo10_1 blo10_

struct {
    real argmax;
} argexp_;

#define argexp_1 argexp_

/* Table of constant values */

static real c_b4 = 300.f;
static integer c__1 = 1;
static integer c__0 = 0;
static real c_b8 = 12.f;
static integer c__1976 = 1976;
static integer c__882 = 882;
static integer c__3 = 3;
static real c_b47 = 28.f;
static real c_b48 = 1.f;
static real c_b52 = 81.f;
static real c_b55 = .06f;
static real c_b63 = 4e8f;
static real c_b65 = 88.f;
static real c_b68 = .05f;
static real c_b71 = 4.6f;
static real c_b72 = 4.5f;
static real c_b75 = -11.5f;
static real c_b76 = -4.f;
static real c_b82 = .001f;
static real c_b107 = 0.f;
static doublereal c_b113 = 2.;
static real c_b114 = 1.5f;
static real c_b120 = 3.f;
static real c_b127 = 130.f;
static real c_b128 = 500.f;
static real c_b131 = .01f;
static integer c__12 = 12;
static integer c__4 = 4;
static integer c__2 = 2;
static real c_b153 = 10.f;

/* IRIS12.FOR ---------------------------------------- OCTOBER 1991 */

/* ***************************************************************** */
/* CHANGES FROM  IRIS11.FOR  TO   IRIS12.FOR: */
/*    - CIRA-1986 INSTEAD OF CIRA-1972 FOR NEUTRAL TEMPERATURE */
/*    - 10/30/91 VNER FOR NIGHTTIME LAY-VERSION:  ABS(..) */
/*    - 10/30/91 XNE(..) IN CASE OF LAY-VERSION */
/*    - 10/30/91 CHANGE SSIN=F/T TO IIQU=0,1,2 */
/*    - 10/30/91 Te > Ti > Tn ENFORCED IN FINAL PROFILE */
/*    - 10/30/91 SUB ALL NAMES WITH 6 OR MORE CHARACTERS */
/*    - 10/31/91 CORRECTED HF1 IN HST SEARCH:  NE(HF1)>NME */
/* ----------- not inlcuded on diskette: ---------------------------- */
/*    - 11/14/91 C1=0 IF NO F1-REGION */
/*    - 11/14/91 CORRECTED HHMIN AND HZ FOR LIN. APP. */
/*    -  1/28/92 RZ12=0 included */
/*    -  1/29/92 NEQV instead of NE between URSIF2 and URSIFO */
/*    -  5/ 1/92 CCIR and URSI input as in IRID12 */

/* ***************************************************************** */
/* ********* INTERNATIONAL REFERENCE IONOSPHERE (IRI). ************* */
/* ***************************************************************** */
/* ****************    OCTOBER 1991     **************************** */
/* ****************     SUBROUTINE      **************************** */
/* ***************************************************************** */


/* Subroutine */ int iris12_(char *path, logical *jf, integer *jmag, real *
	alati, real *along, real *rz12, integer *mmdd, real *dhour, real *
	heibeg, real *heiend, real *heistp, real *outf, real *oarr, ftnlen 
	path_len)
{
    /* Initialized data */

    static real hoa[3] = { 300.f,400.f,600.f };
    static real xnar[3] = { 0.f,0.f,0.f };
    static real xdels[4] = { 5.f,5.f,5.f,10.f };
    static real dnds[4] = { .016f,.01f,.016f,.016f };
    static integer ddo[4] = { 9,5,5,25 };
    static integer do2[2] = { 5,5 };
    static real b0b1[5] = { .755566f,.778596f,.797332f,.812928f,.826146f };

    /* Format strings */
    static char fmt_104[] = "(a,\002CCIR\002,i2,\002.ASC\002)";
    static char fmt_4689[] = "(1x,4e15.8)";
    static char fmt_1144[] = "(a,\002URSI\002,i2,\002.ASC\002)";
    static char fmt_8449[] = "(1x////,\002 !!!! The file \002,a,\002 is not "
	    "in your directory,\002/\002 !!!!   try a different diskette (ent"
	    "er: 1),\002/\002 !!!!   or exit (enter: 0)\002)";

    /* System generated locals */
    integer i__1;
    real r__1, r__2;
    doublereal d__1, d__2;
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    double atan(doublereal), log(doublereal), sqrt(doublereal), tan(
	    doublereal), exp(doublereal);
    integer s_wsfi(icilist *), do_fio(integer *, char *, ftnlen), e_wsfi(void)
	    , f_open(olist *), s_rsfe(cilist *), e_rsfe(void), f_clos(cllist *
	    ), s_wsfe(cilist *), e_wsfe(void), s_rsle(cilist *), do_lio(
	    integer *, integer *, char *, ftnlen), e_rsle(void);
    double cos(doublereal), pow_dd(doublereal *, doublereal *), r_sign(real *,
	     real *);

    /* Local variables */
    static real d__, f[3], h__;
    static integer i__, j, k, l;
    static real r__, x, y, z__, f2[1976]	/* was [13][76][2] */, z1, z2,
	     z3;
    static integer ki, kk;
    static real ho[4], rg, mo[5];
    static logical dy;
    static real ex;
    extern real ti_(real *), xe_(real *), tn_(real *, real *, real *, real *);
    static real ut, ff0[988];
    static integer ib1;
    static real hf1, fm3[882]	/* was [9][49][2] */, ho2[2], h0o, mo2[3];
    extern real xe2_(real *), xe3_(real *);
    static real xm0[441], rr2, rr1, ex1, xf1, ti1, dec, deh, ate[7], tea[6], 
	    amp[4], rif[4], scl[4], cov;
    extern /* Subroutine */ int ggm_(integer *, real *, real *, real *, real *
	    );
    static real hxl[4], xma;
    static logical ext;
    static real yma, zma, bet, dip, xhi, sax, foe, rgo, sux, flu;
    extern /* Subroutine */ int tal_(real *, real *, real *, real *, logical *
	    , real *);
    static real xdx, hta, hte, sec, ett, tet, ten, ti13, fof2, eta1, fof1, 
	    ted1, nmf1, tid1, tn120, pf1o[12], pg1o[80], pg2o[32], pg3o[80], 
	    pf2o[4], pf3o[12], cos2, xe2h, xe3h, tin1, hv1r, hv2r, ten1, tnn1,
	     ti50, tex, tix, ho05;
    extern real xen_(real *, real *, real *, real *, integer *, real *, real *
	    , real *);
    static real tnh, tih, teh, rox, rhx, rnx, ro2x, dela, hnea;
    extern /* Subroutine */ int teba_(real *, real *, integer *, real *);
    static real hnee;
    extern /* Subroutine */ int moda_(integer *, integer *, integer *, 
	    integer *);
    static real dell;
    extern real tede_(real *, real *, real *);
    static integer iday;
    static real lati, dion[7], mlat;
    static logical old79;
    static real covg, xm3000, epin;
    static logical tcon[3];
    extern /* Subroutine */ int soco_(integer *, real *, real *, real *, real 
	    *, real *, real *, real *);
    static real seax, grat, xdel;
    extern real hpol_(real *, real *, real *, real *, real *, real *, real *);
    static real hour, vner, dxdx;
    static integer iiqu;
    static real alg100;
    extern real elte_(real *);
    static real rrrr, hnia, hnie;
    extern /* Subroutine */ int sufe_(real *, real *, integer *, real *);
    static real zzz1, afof2, ahmf2, delx;
    extern real rpid_(real *, real *, real *, integer *, real *, integer *, 
	    real *);
    static real rhex, xtts, rnox;
    static logical f1reg;
    static real alog2, hhmf2;
    extern real rdno_(real *, real *, real *, real *, real *);
    static real hmf1m;
    static logical gulb0;
    extern real b0pol_(real *, real *, real *, integer *, real *, real *);
    static real nobo2, pnmf1, yfof2, tn1ni, yoh0o;
    extern /* Subroutine */ int f2out_(real *, real *, real *, real *, real *,
	     real *, real *, real *);
    static real stte1, stte2, elede, ymo2z, hhalf, aldo21, hdeep;
    static integer lread;
    static real magbr, dlndh;
    extern /* Subroutine */ int cira86_(integer *, real *, real *, real *, 
	    real *, real *, real *, real *, real *), rdhhe_(real *, real *, 
	    real *, real *, real *, real *, real *, real *);
    extern real xmded_(real *, real *, real *);
    static logical noden;
    extern real teder_(real *);
    static integer idisk;
    static real depth, rathh, longi, modip, mlong;
    static integer daynr;
    static real hhmin, width;
    static logical notem, noion;
    static integer jxnar, month;
    static real secni;
    static logical botto;
    extern /* Subroutine */ int rogul_(integer *, real *, real *, real *);
    static real texni, signi, tlbdn, xnehz, hmaxd;
    static logical topsi;
    static real hmaxn, tmaxd, tmaxn, xxmin;
    extern real fof1ed_(real *, real *, real *), hmf2ed_(real *, real *, real 
	    *, real *);
    extern /* Subroutine */ int regfa1_(real *, real *, real *, real *, real *
	    , real *, R_fp, logical *, real *);
    extern real dtndh_(real *, real *, real *, real *);
    static real xteti;
    static integer msumo;
    static real hfixo, ymaxx, yo2h0o, b0cnew;
    static logical fof2in, hmf2in;
    static real tnahh2;
    extern /* Subroutine */ int koefp1_(real *), koefp2_(real *), koefp3_(
	    real *);
    static real sunde1, hfixo2;
    static logical ursif2;
    static real rdo2mx;
    extern /* Subroutine */ int fieldg_(real *, real *, real *, real *, real *
	    , real *, real *, real *, real *, real *);
    extern real foeedi_(real *, real *, real *, real *);
    static integer iregfa;
    static real dndhbr;
    static integer seaday;
    static char filnam[99];
    static integer icalls;
    static real abslat, absmdp, absmbr, tnahhi;
    static logical belowe, schalt;
    static integer iuccir;
    static real sundec, absmlt, diplat, height;
    static integer numhei;
    extern /* Subroutine */ int inilay_(logical *, real *, real *, real *, 
	    real *, real *, real *, real *, real *, real *, real *, real *, 
	    real *, real *, integer *), ioncom_(real *, real *, real *, real *
	    , integer *, real *);
    static integer season;
    static logical teneop;
    static real rdomax;
    extern real epstep_(real *, real *, real *, real *, real *);
    static logical layver;
    static real xhinon;
    static integer montho, monito, istart;
    static logical ursifo;
    static integer konsol, nseson;
    static real saxnon, rclust, suxnon;

    /* Fortran I/O blocks */
    static icilist io___76 = { 0, filnam, 0, fmt_104, 99, 1 };
    static cilist io___78 = { 0, 0, 0, fmt_4689, 0 };
    static icilist io___81 = { 0, filnam, 0, fmt_1144, 99, 1 };
    static cilist io___82 = { 0, 0, 0, fmt_4689, 0 };
    static cilist io___83 = { 0, 0, 0, fmt_8449, 0 };
    static cilist io___84 = { 0, 5, 0, 0, 0 };


/* ----------------------------------------------------------------- */
/* INTERNATIONAL REFERENCE IONOSPHERE 1991 */

/* INPUT:  JMAG=0/1      GEODETIC/GEOMAGNETIC LATITUDE AND LONGITUDE */
/*         ALATI,ALONG   LATITUDE NORTH AND LONGITUDE EAST IN DEGREES */
/*         RZ12 (-COV)   12-MONTHS-RUNNING MEAN OF SOLAR SUNSPOT NUMBER */
/*                          (OR EQUIVALENT F10.7 SOLAR RADIO FLUX AS */
/*                          NEGATIVE NUMBER) */
/*         MMDD (-DDD)   DATE (OR DAY OF YEAR AS A NEGATIVE NUMBER) */
/*         DHOUR         LOCAL TIME (OR UNIVERSAL TIME + 25) IN DECIMAL */
/*                          HOURS */
/*         HEIBEG,       BEGIN, END, AND STEPWIDTH OF HEIGHT RANGE */
/*          HEIEND,HEISTP   IN KM (MAXIMUM NUMBER OF STEPS IS 50 !!) */
/*         JF(1:12)      TRUE/FALSE FLAGS FOR SEVERAL OPTIONS */
/*          JF(1)=.TRUE.[.FALSE.]   ELECTRON DENSITY IS [NOT] CALCULATED */
/*          JF(2)=T[F]    TEMPERATURES ARE [NOT] CALCULATED */
/*          JF(3)=T[F]    ION COMPOSITION IS [NOT] CALCULATED */
/*          JF(4)=T[F]    B0 FROM TABLE [FROM GULYEAVA 1987] */
/*          JF(5)=T[F]    F2 PEAK FROM CCIR [FROM URSI] */
/*          JF(6)=T[F]    ION COMP. STANDARD [DANILOV-YAICHNIKOV-1985] */
/*          JF(7)=T[F]    STAND. IRI TOPSIDE [IRI-79] */
/*          JF(8)=T[F]    NMF2 PEAK MODEL [INPUT VALUES] */
/*          JF(9)=T[F]    HMF2 PEAK MODEL [INPUT VALUES] */
/*          JF(10)=T[F]   TE MODEL [TE-NE MODEL WITH NE INPUT] */
/*          JF(11)=T[F]   NE STANDARD [LAY-FUNCTIONS VERSION] */
/*          JF(12)=T[F]   MESSAGE ARE WRITTEN TO UNIT=6 [=12] */

/*  JF(1:11)=.TRUE. GENERATES THE STANDARD IRI-90 PARAMETERS. */
/*  IF YOU SET JF(8)=.FALSE., THAN YOU HAVE TO PROVIDE THE F2 PEAK */
/*  NMF2/M-3 OR FOF2/MHZ IN OARR(1). SIMILARLY, IF YOU SET JF(9)= */
/*  .FALSE., THAN YOU HAVE TO PROVIDE THE F2 PEAK HEIGHT HMF2/KM IN */
/*  OARR(2). IF YOU SET JF(10)=.FALSE., THAN YOU HAVE TO PROVIDE THE */
/*  ELECTRON DENSITY IN M-3 AT 300KM AND/OR 400KM AND/OR 600KM IN */
/*  OARR(3), OARR(4), AND OARR(5). IF YOU WANT TO USE THIS OPTION AT */
/*  ONLY ONE OF THE THREE ALTITUDES, THAN SET THE DENSITIES AT THE */
/*  OTHER TWO TO ZERO. */

/*  OUTPUT:  OUTF(1:10,1:50)   IRI PROFILES */
/*              OUTF(1,*)  ELECTRON DENSITY/M-3 */
/*              OUTF(2,*)  NEUTRAL TEMPERATURE/K */
/*              OUTF(3,*)  ION TEMPERATURE/K */
/*              OUTF(4,*)  ELECTRON TEMPERATURE/K */
/*              OUTF(5,*)  PERCENTAGE OF O+ IONS IN % */
/*              OUTF(6,*)  PERCENTAGE OF H+ IONS IN % */
/*              OUTF(7,*)  PERCENTAGE OF HE+ IONS IN % */
/*              OUTF(8,*)  PERCENTAGE OF O2+ IONS IN % */
/*              OUTF(9,*)  PERCENTAGE OF NO+ IONS IN % */
/*                 AND, IF JF(6)=.FALSE.: */
/*              OUTF(10,*)  PERCENTAGE OF CLUSTER IONS IN % */
/*              OUTF(11,*)  PERCENTAGE OF N+ IONS IN % */

/*            OARR(1:30)   ADDITIONAL OUTPUT PARAMETERS */
/*              OARR(1) = NMF2/M-3        OARR(2) = HMF2/KM */
/*              OARR(3) = NMF1/M-3        OARR(4) = HMF1/KM */
/*              OARR(5) = NME/M-3         OARR(6) = HME/KM */
/*              OARR(7) = NMD/M-3         OARR(8) = HMD/KM */
/*              OARR(9) = HHALF/KM        OARR(10) = B0/KM */
/*              OARR(11) =VALLEY-BASE/M-3 OARR(12) = VALLEY-TOP/KM */
/*              OARR(13) = TE-PEAK/K      OARR(14) = TE-PEAK HEIGHT/KM */
/*              OARR(15) = TE-MOD(300KM)  OARR(16) = TE-MOD(400KM)/K */
/*              OARR(17) = TE-MOD(600KM)  OARR(17) = TE-MOD(1400KM)/K */
/*              OARR(18) = TE-MOD(3000KM) OARR(19) = TE(120KM)=TN=TI/K */
/*              OARR(20) = TI-MOD(430KM)  OARR(21) = X/KM, WHERE TE=TI */
/*              OARR(22) = SOLAR ZENITH ANGLE/DEG */
/*              OARR(23) = SUN DECLINATION/DEG */
/*              OARR(24) = DIP            OARR(25) = DIP LATITUDE */
/*              OARR(26) = MODIFIED DIP LATITUDE */
/*              OARR(27:30) FREE */
/* ------------------------------------------------------------------- */
/* *** THIS PROGRAM PRODUCES PROFILES OF                         *** */
/* ***      ELECTRON DENSITY                                     *** */
/* ***      NEUTRAL TEMPERATURE (CIRA 86)                        *** */
/* ***      ELECTRON TEMPERATURE                                 *** */
/* ***      ION TEMPERATURE                                      *** */
/* ***      RELATIVE PERCENTAGE DENSITIES OF THE IONS            *** */
/* ***           ATOMIC OXYGEN, HYDROGEN, HELIUM,                *** */
/* ***           MOLECULAR OXYGEN AND NITROGEN OXYD (NO+)        *** */
/* ***************************************************************** */
/* *** THE ALTITUDE LIMITS ARE:  LOWER (DAY/NIGHT)  UPPER        *** */
/* ***     ELECTRON DENSITY         60/80 KM       1000 KM       *** */
/* ***     TEMPERATURES              120 KM        3000 KM       *** */
/* ***     ION DENSITIES             100 KM        1000 KM       *** */
/* ***************************************************************** */
/* *     --------------------ADDRESSES------------------------     * */
/* *     I  PROF. K. RAWER              DR. D. BILITZA       I     * */
/* *     I  HERRENSTR. 43               GSFC/NSSDC CODE 633  I     * */
/* *     I  D-7801 MARCH                GREENBELT MD 20771   I     * */
/* *     I  F.R.G.                      USA                  I     * */
/* *     -----------------------------------------------------     * */
/* ***************************************************************** */
/* ***************************************************************** */
/* ***************************************************************** */
/* *********       ALL ANGLES ARE IN DEGREE           ************** */
/* *********       ALL DENSITIES ARE IN M-3           ************** */
/* *********       ALL ALTITUDES ARE IN KM            ************** */
/* *********     ALL TEMPERATURES ARE IN KELVIN       ************** */
/* *********     ALL TIMES ARE IN DECIMAL HOURS       ************** */
/* ***************************************************************** */
/* ********************  OPTIONS  ********************************** */
/* ***************************************************************** */
/* * FOR HMF2=0 OR FOF2=0 THE F2 PEAK VALUES ARE CALCULATED WITH   * */
/* * THE CCIR OR URSI MODELS. THE CCIR COEFFICIENT SET FOR THE     * */
/* * MONTH "mm" IS EXPECTED IN THE BINARY FILE "CCIRmm.BIN" AND    * */
/* * THE URSI SET IN "URSImm.BIN". IF YOU USE THE ASCII CODED      * */
/* * FILES "CCIRmm.ASC", YOU HAVE TO INCORPORATE THE CHANGES       * */
/* * INDICTED IN PROGRAM SECTION ENTITLED "READ CCIR COEFFICIENT   * */
/* * SET FOR CHOSEN MONTH."                                        * */
/* ***************************************************************** */
/* ***************************************************************** */
/* ***************************************************************** */
    /* Parameter adjustments */
    --oarr;
    outf -= 12;
    --jf;

    /* Function Body */

/* PROGAM CONSTANTS */

    ++icalls;
    argexp_1.argmax = 88.f;
    const_1.umr = atan(1.f) * 4.f / 180.f;
    alog2 = log(2.f);
    alg100 = log(100.f);
    istart = 1;
    numhei = (integer) ((*heiend - *heibeg) / *heistp) + 1;
    if (numhei > 50) {
	numhei = 50;
    }

/* Code inserted to aleviate block data problem for PC version. */
/* Thus avoiding DATA statement with parameters from COMMON block. */

    blote_1.ahh[0] = 120.f;
    blote_1.ahh[1] = 0.f;
    blote_1.ahh[2] = 300.f;
    blote_1.ahh[3] = 400.f;
    blote_1.ahh[4] = 600.f;
    blote_1.ahh[5] = 1400.f;
    blote_1.ahh[6] = 3e3f;
    blote_1.dte[0] = 5.f;
    blote_1.dte[1] = 5.f;
    blote_1.dte[2] = 10.f;
    blote_1.dte[3] = 20.f;
    blote_1.dte[4] = 20.f;
    block8_1.dti[0] = 10.f;
    block8_1.dti[1] = 10.f;
    block8_1.dti[2] = 20.f;
    block8_1.dti[3] = 20.f;

/* FIRST SPECIFY YOUR COMPUTERS CHANNEL NUMBERS .................... */
/* AGNR=OUTPUT (OUTPUT IS DISPLAYED OR STORED IN FILE OUTPUT.IRI)... */
/* IUCCIR=UNIT NUMBER FOR CCIR COEFFICIENTS ........................ */

    monito = 6;
    iuccir = 10;
    konsol = 6;
    if (! jf[12]) {
	konsol = 12;
    }

/* selection of density and ion composition options .................. */

    noden = ! jf[1];
    notem = ! jf[2];
    noion = ! jf[3];
    dy = ! jf[6];
    layver = ! jf[11];
    old79 = ! jf[7];
    gulb0 = ! jf[4];

/* f peak density .................................................... */

    fof2in = ! jf[8];
    if (fof2in) {
	afof2 = oarr[1];
	if (afof2 > 100.f) {
	    afof2 = sqrt(afof2 / 1.24e10f);
	}
    }
    ursif2 = ! jf[5];

/* f peak altitude .................................................. */

    hmf2in = ! jf[9];
    if (hmf2in) {
	ahmf2 = oarr[2];
    }

/* TE-NE MODEL OPTION .............................................. */

    teneop = ! jf[10];
    if (teneop) {
	for (jxnar = 1; jxnar <= 3; ++jxnar) {
	    xnar[jxnar - 1] = oarr[jxnar + 2];
	    tcon[jxnar - 1] = FALSE_;
/* L8154: */
	    if (xnar[jxnar - 1] > 0.f) {
		tcon[jxnar - 1] = TRUE_;
	    }
	}
    }
/*     if(icalls.gt.1) goto 8201 */
/* 	write(*,*) '*** IRI parameters are being calculated ***' */
/*     if(NODEN) goto 2889 */
/* 	if(LAYVER) write(*,*) 'Ne, E-F: The LAY-Version is ', */
/*    &	  'prelimenary. Erroneous profile features can occur.' */
/* 	if(GULB0) write(*,*) 'Ne, B0: Bottomside thickness is ', */
/*    &	  'obtained with Gulyaeva-1987 model.' */
/* 	if(OLD79) write(*,*) 'Ne: Using IRI-79. Correction', */
/*    &	  ' of equatorial topside is not included.' */
/* 	if(HMF2IN) write(*,*) 'Ne, hmF2: Input values are used.' */
/* 	if(FOF2IN) then */
/* 	  write(*,*) 'Ne, foF2: Input values are used.' */
/* 	  goto 2889 */
/* 	endif */
/* 	if(URSIF2) then */
/* 	  write(*,*) 'Ne, foF2: URSI model is used.' */
/* 	else */
/* 	  write(*,*) 'Ne, foF2: CCIR model is used.' */
/* 	endif */
/* 2889 if((.not.NOION).and.(DY)) */
/*    &	   write(*,*) 'Ion Com.: Using Danilov-Yaichnikov-1985.' */
/*     if((.not.NOTEM).and.(TENEOP)) */
/*    &     write(*,*) 'Te: Temperature-density correlation is used.' */
/* L8201: */

/* CALCULATION OF MEAN F10.7CM SOLAR RADIO FLUX (COV)................ */
/* CALCULATION OF RESTRICTED SOLAR ACTIVITIES (RG,COVG).............. */

    if (*rz12 >= 0.f) {
	r__ = *rz12;
	cov = r__ * (r__ * 8.9e-4f + .728f) + 63.75f;
    } else {
	cov = -(*rz12);
	r__ = (sqrt(cov + 85.12f) - 12.2f) * 33.52f;
    }
    rg = r__;
    covg = cov;
    if (r__ > 150.f) {
	rg = 150.f;
    }
    if (cov > 193.f) {
	covg = 193.f;
    }

/* CALCULATION OF GEOG. OR GEOM. COORDINATES IN DEG.................... */
/* CALCULATION OF MAGNETIC INCLINATION (DIP), DECLINATION (DEC)........ */
/*   DIP LATITUDE (MAGBR) AND MODIFIED DIP (MODIP). ALL IN DEGREE...... */

    if (*jmag > 0) {
	mlat = *alati;
	mlong = *along;
    } else {
	lati = *alati;
	longi = *along;
    }
    ggm_(jmag, &longi, &lati, &mlong, &mlat);
    abslat = abs(lati);
    fieldg_(&lati, &longi, &c_b4, &xma, &yma, &zma, &bet, &dip, &dec, &modip);
    magbr = atan(tan(dip * const_1.umr) * .5f) / const_1.umr;
    absmlt = abs(mlat);
    absmdp = abs(modip);
    absmbr = abs(magbr);

/* CALCULATION OF SEASON (SUMMER=2, WINTER=4).......................... */
/* CALCULATION OF DAY OF YEAR AND SUN DECLINATION...................... */

    if (*mmdd < 0) {
	daynr = -(*mmdd);
	moda_(&c__1, &month, &iday, &daynr);
    } else {
	month = *mmdd / 100;
	iday = *mmdd - month * 100;
	moda_(&c__0, &month, &iday, &daynr);
    }
    season = (integer) ((daynr + 45.f) / 92.f);
    if (season < 1) {
	season = 4;
    }
    nseson = season;
    seaday = daynr;
    if (lati > 0.f) {
	goto L5592;
    }
    season += -2;
    if (season < 1) {
	season += 4;
    }
    seaday = daynr + 183;
    if (seaday > 366) {
	seaday += -366;
    }

/* CALCULATION OF SOLAR ZENITH ANGLE (XHI/DEG)......................... */
/* NOON VALUE (XHINON)................................................. */

L5592:
    if (*dhour > 24.1f) {
	ut = *dhour - 25.f;
	hour = ut + longi / 15.f;
	if (hour > 24.f) {
	    hour += -24.f;
	}
    } else {
	hour = *dhour;
	ut = hour - longi / 15.f;
	if (ut < 0.f) {
	    ut += 24.f;
	}
    }
    soco_(&daynr, &hour, &lati, &longi, &sundec, &xhi, &sax, &sux);
    soco_(&daynr, &c_b8, &lati, &longi, &sunde1, &xhinon, &saxnon, &suxnon);
    block5_1.night = FALSE_;
    if (abs(sax) > 25.f) {
	if (sax < 0.f) {
	    block5_1.night = TRUE_;
	}
	goto L1334;
    }
    if (sax <= sux) {
	goto L1386;
    }
    if (hour > sux && hour < sax) {
	block5_1.night = TRUE_;
    }
    goto L1334;
L1386:
    if (hour > sux || hour < sax) {
	block5_1.night = TRUE_;
    }

/* CALCULATION OF ELECTRON DENSITY PARAMETERS................ */

L1334:
    hnea = 65.f;
    if (block5_1.night) {
	hnea = 80.f;
    }
    hnee = 2e3f;
    if (noden) {
	goto L4933;
    }
    dela = 4.32f;
    if (absmdp >= 18.f) {
	dela = exp(-(absmdp - 30.f) / 10.f) + 1.f;
    }
    dell = exp(-(abslat - 20.f) / 10.f) + 1;
/* !!!!!!! F-REGION PARAMETERS AND E-PEAK !!!!!!!!!!!!!!!!!!!!!!!!!! */
    foe = foeedi_(&cov, &xhi, &xhinon, &abslat);
    block4_1.nme = foe * 1.24e10f * foe;
    block4_1.hme = 105.f;
    if (fof2in && hmf2in) {
	goto L501;
    }
    if (ursif2 != ursifo) {
	goto L7797;
    }
    if (month == montho && rg == rgo) {
	goto L4292;
    }
    if (month == montho) {
	goto L4291;
    }

/* READ CCIR COEFFICIENT SET FOR CHOSEN MONTH.................... */

L7797:
    s_wsfi(&io___76);
    do_fio(&c__1, path, path_len);
    i__1 = month + 10;
    do_fio(&c__1, (char *)&i__1, (ftnlen)sizeof(integer));
    e_wsfi();
    ursifo = ursif2;
L1344:
    lread = 1;
/*        OPEN(IUCCIR,FILE=FILNAM,STATUS='OLD',ERR=8448, */
/*     &		FORM='UNFORMATTED') */
/*        READ(IUCCIR) F2,FM3 */
/* 104     FORMAT('CCIR',I2,'.BIN') */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* !! FOR ASCII CODED CCIR COEFFIECENTS FILES SUBSTITUTE  !! */
/* !! THE LAST THREE STATEMENTS WITH THE FOLLOWING FOUR:  !! */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    o__1.oerr = 1;
    o__1.ounit = iuccir;
    o__1.ofnmlen = 99;
    o__1.ofnm = filnam;
    o__1.orl = 0;
    o__1.osta = "OLD";
    o__1.oacc = 0;
    o__1.ofm = "FORMATTED";
    o__1.oblnk = 0;
    i__1 = f_open(&o__1);
    if (i__1 != 0) {
	goto L8448;
    }
    io___78.ciunit = iuccir;
    s_rsfe(&io___78);
    do_fio(&c__1976, (char *)&f2[0], (ftnlen)sizeof(real));
    do_fio(&c__882, (char *)&fm3[0], (ftnlen)sizeof(real));
    e_rsfe();
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    cl__1.cerr = 0;
    cl__1.cunit = iuccir;
    cl__1.csta = 0;
    f_clos(&cl__1);

/* READ URSI COEFFICIENT SET FOR CHOSEN MONTH.................... */

    if (ursif2) {
	s_wsfi(&io___81);
	do_fio(&c__1, path, path_len);
	i__1 = month + 10;
	do_fio(&c__1, (char *)&i__1, (ftnlen)sizeof(integer));
	e_wsfi();
L1244:
	lread = 2;
/* 1144    FORMAT('URSI',I2,'.BIN') */
/*          OPEN(IUCCIR,FILE=FILNAM,STATUS='OLD',ERR=8448, */
/*     &		FORM='UNFORMATTED') */
/*          READ(IUCCIR) F2 */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* !! FOR ASCII CODED URSI COEFFIECENTS FILES SUBSTITUTE  !! */
/* !! THE LAST THREE STATEMENTS WITH THE FOLLOWING THREE: !! */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	o__1.oerr = 1;
	o__1.ounit = iuccir;
	o__1.ofnmlen = 99;
	o__1.ofnm = filnam;
	o__1.orl = 0;
	o__1.osta = "OLD";
	o__1.oacc = 0;
	o__1.ofm = "FORMATTED";
	o__1.oblnk = 0;
	i__1 = f_open(&o__1);
	if (i__1 != 0) {
	    goto L8448;
	}
	io___82.ciunit = iuccir;
	s_rsfe(&io___82);
	do_fio(&c__1976, (char *)&f2[0], (ftnlen)sizeof(real));
	e_rsfe();
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	cl__1.cerr = 0;
	cl__1.cunit = iuccir;
	cl__1.csta = 0;
	f_clos(&cl__1);
    }
    montho = month;
    goto L4291;
L8448:
    io___83.ciunit = monito;
    s_wsfe(&io___83);
    do_fio(&c__1, filnam, (ftnlen)99);
    e_wsfe();
    s_rsle(&io___84);
    do_lio(&c__3, &c__1, (char *)&idisk, (ftnlen)sizeof(integer));
    e_rsle();
    if (idisk == 1) {
	switch (lread) {
	    case 1:  goto L1344;
	    case 2:  goto L1244;
	}
    }
    goto L3330;

/* LINEAR INTERPOLATION IN SOLAR ACTIVITY */

L4291:
    rr2 = rg / 100.f;
    rr1 = 1.f - rr2;
    for (i__ = 1; i__ <= 76; ++i__) {
	for (j = 1; j <= 13; ++j) {
	    k = j + (i__ - 1) * 13;
/* L20: */
	    ff0[k - 1] = f2[j + (i__ + 76) * 13 - 1002] * rr1 + f2[j + (i__ + 
		    152) * 13 - 1002] * rr2;
	}
    }
    for (i__ = 1; i__ <= 49; ++i__) {
	for (j = 1; j <= 9; ++j) {
	    k = j + (i__ - 1) * 9;
/* L30: */
	    xm0[k - 1] = fm3[j + (i__ + 49) * 9 - 451] * rr1 + fm3[j + (i__ + 
		    98) * 9 - 451] * rr2;
	}
    }
    rgo = rg;
L4292:
    f2out_(&modip, &lati, &longi, ff0, xm0, &ut, &yfof2, &xm3000);
L501:
    if (fof2in) {
	fof2 = afof2;
    } else {
	fof2 = yfof2;
    }
    block1_1.nmf2 = fof2 * 1.24e10f * fof2;
    if (hmf2in) {
	block1_1.hmf2 = ahmf2;
    } else {
	r__1 = fof2 / foe;
	block1_1.hmf2 = hmf2ed_(&magbr, &rg, &r__1, &xm3000);
    }
    topsi = *heiend > block1_1.hmf2;
    botto = *heiend >= block4_1.hme && *heibeg <= block1_1.hmf2;
    belowe = *heibeg < block4_1.hme;

/* topside profile parameters ............................. */

    if (! topsi) {
	goto L1501;
    }
    cos2 = cos(mlat * const_1.umr);
    cos2 *= cos2;
    flu = (covg - 40.f) / 30.f;
    if (old79) {
	eta1 = cos2 * -.0070305f;
    } else {
	ex = exp(-mlat / 15.f);
	ex1 = ex + 1;
	epin = ex * 4.f / (ex1 * ex1);
	eta1 = epin * -.02f;
    }
    blo10_1.eta = eta1 + .058798f + flu * (cos2 * .0069724f - .014065f) + (
	    cos2 * .004281f + .0024287f - fof2 * 1.528e-4f) * fof2;
    blo10_1.zeta = .078922f - cos2 * .0046702f + flu * (cos2 * .0076545f - 
	    .019132f) + (cos2 * .006029f + .0032513f - fof2 * 2.0872e-4f) * 
	    fof2;
    blo10_1.beta = cos2 * 20.253f - 128.03f + flu * (-8.0755f - cos2 * 
	    .65896f) + (cos2 * .71458f + .44041f - fof2 * .042966f) * fof2;
    z__ = exp(94.45f / blo10_1.beta);
    z1 = z__ + 1;
    z2 = z__ / (blo10_1.beta * z1 * z1);
    blo10_1.delta = (blo10_1.eta / z1 - blo10_1.zeta / 2.f) / (blo10_1.eta * 
	    z2 + blo10_1.zeta / 400.f);

/* bottomside profile parameters ............................. */

L1501:
    block1_1.hmf1 = block1_1.hmf2;
    block3_1.hz = block1_1.hmf2;
    block4_1.hef = block4_1.hme;
    if (! botto) {
	goto L2727;
    }
    block2_1.b1 = 3.f;
/* !!!!!!! INTERPOLATION FOR B0 OUT OF ARRAY B0F !!!!!!!!!!!!!!!!!!!!! */
    if (gulb0) {
	rogul_(&seaday, &xhi, &seax, &grat);
	if (block5_1.night) {
	    grat = .91f - block1_1.hmf2 / 4e3f;
	}
	b0cnew = block1_1.hmf2 * (1.f - grat);
	block2_1.b0 = b0cnew / b0b1[0];
    } else {
	block2_1.b0 = b0pol_(&hour, &sax, &sux, &season, &rg, &dela);
    }
/* !!!!!!! F1-REGION PARAMETERS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    f1reg = FALSE_;
    block1_1.hmf1 = 0.f;
    pnmf1 = 0.f;
    block2_1.c1 = 0.f;
    if (block5_1.night || season == 4) {
	goto L150;
    }
    fof1 = fof1ed_(&absmbr, &r__, &xhi);
    if (fof1 < .001f) {
	goto L150;
    }
    f1reg = TRUE_;
    block2_1.c1 = .11f / dela + .09f;
    pnmf1 = fof1 * 1.24e10f * fof1;
L150:
    nmf1 = pnmf1;
/* !!!!!!! PARAMETER FOR E AND VALLEY-REGION !!!!!!!!!!!!!!!!!!!!! */
    xdel = xdels[season - 1] / dela;
    dndhbr = dnds[season - 1] / dela;
    r__1 = 10.5f / dela;
    hdeep = hpol_(&hour, &r__1, &c_b47, &sax, &sux, &c_b48, &c_b48);
    r__1 = 17.8f / dela;
    r__2 = 22.f / dela + 45.f;
    width = hpol_(&hour, &r__1, &r__2, &sax, &sux, &c_b48, &c_b48);
    depth = hpol_(&hour, &xdel, &c_b52, &sax, &sux, &c_b48, &c_b48);
    dlndh = hpol_(&hour, &dndhbr, &c_b55, &sax, &sux, &c_b48, &c_b48);
    if (depth < 1.f) {
	goto L600;
    }
    if (block5_1.night) {
	depth = -depth;
    }
    tal_(&hdeep, &depth, &width, &dlndh, &ext, block5_1.e);
    if (! ext) {
	goto L667;
    }
/*     	  WRITE(KONSOL,650) */
/* L650: */
L600:
    width = 0.f;
L667:
    block4_1.hef = block4_1.hme + width;
    vner = (1.f - abs(depth) / 100.f) * block4_1.nme;

/* Parameters below E  ............................. */

L2727:
    if (! belowe) {
	goto L2726;
    }
/* !!!!!!!D-REGION PARAMETER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    block6_1.nmd = xmded_(&xhi, &r__, &c_b63);
    block6_1.hmd = hpol_(&hour, &c_b52, &c_b65, &sax, &sux, &c_b48, &c_b48);
    r__1 = .03f / dela + .02f;
    f[0] = hpol_(&hour, &r__1, &c_b68, &sax, &sux, &c_b48, &c_b48);
    f[1] = hpol_(&hour, &c_b71, &c_b72, &sax, &sux, &c_b48, &c_b48);
    f[2] = hpol_(&hour, &c_b75, &c_b76, &sax, &sux, &c_b48, &c_b48);
    block7_1.fp1 = f[0];
    block7_1.fp2 = -block7_1.fp1 * block7_1.fp1 / 2.f;
    block7_1.fp30 = (-f[1] * block7_1.fp2 - block7_1.fp1 + 1.f / f[1]) / (f[1]
	     * f[1]);
    block7_1.fp3u = (-f[2] * block7_1.fp2 - block7_1.fp1 - 1.f / f[2]) / (f[2]
	     * f[2]);
    block6_1.hdx = block6_1.hmd + f[1];
    x = block6_1.hdx - block6_1.hmd;
    xdx = block6_1.nmd * exp(x * (block7_1.fp1 + x * (block7_1.fp2 + x * 
	    block7_1.fp30)));
    dxdx = xdx * (block7_1.fp1 + x * (block7_1.fp2 * 2.f + x * 3.f * 
	    block7_1.fp30));
    x = block4_1.hme - block6_1.hdx;
    block7_1.xkk = -dxdx * x / (xdx * log(xdx / block4_1.nme));
    d__1 = (doublereal) x;
    d__2 = (doublereal) (block7_1.xkk - 1.f);
    block7_1.d1 = dxdx / (xdx * block7_1.xkk * pow_dd(&d__1, &d__2));

/* SEARCH FOR HMF1 .................................................. */

L2726:
    if (! botto) {
	goto L4933;
    }
    if (layver) {
	goto L6153;
    }
L924:
    if (! f1reg) {
	goto L380;
    }
    xe2h = xe2_(&block4_1.hef);
    regfa1_(&block4_1.hef, &block1_1.hmf2, &xe2h, &block1_1.nmf2, &c_b82, &
	    nmf1, (R_fp)xe2_, &schalt, &block1_1.hmf1);
    if (! schalt) {
	goto L380;
    }
/* 	  WRITE(KONSOL,11) */
/* L11: */
    iregfa = 1;

/* change B1 and try again .......................................... */

L9244:
    if (block2_1.b1 > 4.5f) {
	switch (iregfa) {
	    case 1:  goto L7398;
	    case 2:  goto L8922;
	}
    }
    block2_1.b1 += .5f;
/* 		WRITE(KONSOL,902) B1-0.5,B1 */
/* L902: */
    if (gulb0) {
	ib1 = (integer) (block2_1.b1 * 2.f - 5.f);
	block2_1.b0 = b0cnew / b0b1[ib1 - 1];
    }
    goto L924;

/* omit F1 feature .................................................... */

L7398:
/* 7398 WRITE(KONSOL,9269) */
/* L9269: */
    block1_1.hmf1 = 0.f;
    nmf1 = 0.f;
    block2_1.c1 = 0.f;
    block2_1.b1 = 3.f;
    f1reg = FALSE_;

/* SEARCH FOR HST [NE3(HST)=NME] .......................................... */

L380:
    rrrr = .5f;
    if (f1reg) {
	hf1 = block1_1.hmf1;
	xf1 = nmf1;
	goto L3972;
    }
    rathh = .5f;
L3973:
    hf1 = block4_1.hef + (block1_1.hmf2 - block4_1.hef) * rathh;
    xf1 = xe3_(&hf1);
    if (xf1 < block4_1.nme) {
	rathh += .1f;
	goto L3973;
    }
L3972:
    h__ = hf1;
    deh = 10.f;
    xxmin = xf1;
    hhmin = hf1;
L3895:
    h__ -= deh;
    if (h__ < block4_1.hef) {
	h__ += deh * 2;
	deh /= 10.f;
	if (deh < 1.f) {
	    goto L3885;
	}
    }
    xe3h = xe3_(&h__);
    if (xe3h < xxmin) {
	xxmin = xe3h;
	hhmin = h__;
    }
    if (xe3h > block4_1.nme) {
	goto L3895;
    }
    regfa1_(&h__, &hf1, &xe3h, &xf1, &c_b82, &block4_1.nme, (R_fp)xe3_, &
	    schalt, &block3_1.hst);
    block3_1.str = block3_1.hst;
    if (! schalt) {
	goto L360;
    }
L3885:
/* 3885	WRITE(KONSOL,100) */
/* L100: */
    iregfa = 2;
    if (xxmin / block4_1.nme < 1.3f) {
	goto L9244;
    }

/* assume linear interpolation between HZ and HEF .................. */

L8922:
    block3_1.hz = hhmin + (hf1 - hhmin) * rrrr;
    xnehz = xe3_(&block3_1.hz);
    if (xnehz - block4_1.nme < .001f) {
	rrrr += .1f;
	goto L8922;
    }
/*       WRITE(KONSOL,901) HZ,HEF */
/* L901: */
    block3_1.t = (xnehz - block4_1.nme) / (block3_1.hz - block4_1.hef);
    block3_1.hst = -333.f;
    goto L4933;

/* calculate HZ, D and T ............................................ */

L360:
    block3_1.hz = (block3_1.hst + hf1) / 2.f;
    d__ = block3_1.hz - block3_1.hst;
    block3_1.t = d__ * d__ / (block3_1.hz - block4_1.hef - d__);
    goto L4933;

/* LAY-functions for middle ionosphere */

L6153:
    hmf1m = xhi * .6428f + 165.f;
    hhalf = grat * block1_1.hmf2;
    hv1r = block4_1.hme + width;
    hv2r = block4_1.hme + hdeep;
    hhmf2 = block1_1.hmf2;
    inilay_(&block5_1.night, &block1_1.nmf2, &nmf1, &block4_1.nme, &vner, &
	    hhmf2, &hmf1m, &block4_1.hme, &hv1r, &hv2r, &hhalf, hxl, scl, amp,
	     &iiqu);
/* 	IF(IIQU.EQ.1) WRITE(KONSOL,7733) */
/* L7733: */
/* 	IF(IIQU.EQ.2) WRITE(KONSOL,7722) */
/* L7722: */
/* ---------- CALCULATION OF NEUTRAL TEMPERATURE PARAMETER------- */
L4933:
    hta = 120.f;
    hte = 3e3f;
    if (notem) {
	goto L240;
    }
    sec = ut * 3600.f;
    cira86_(&daynr, &sec, &lati, &longi, &hour, &cov, &blotn_1.texos, &tn120, 
	    &blotn_1.sigma);
    if (hour != 0.f) {
	secni = (24.f - longi / 15.f) * 3600.f;
	cira86_(&daynr, &secni, &lati, &longi, &c_b107, &cov, &texni, &tn1ni, 
		&signi);
    } else {
	texni = blotn_1.texos;
	tn1ni = tn120;
	signi = blotn_1.sigma;
    }
    blotn_1.tlbdh = blotn_1.texos - tn120;
    tlbdn = texni - tn1ni;

/* --------- CALCULATION OF ELECTRON TEMPERATURE PARAMETER-------- */

/* L881: */
/* !!!!!!!!!! TE(120KM)=TN(120KM) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    ate[0] = tn120;
/* !!!!!!!!!! TE-MAXIMUM (JICAMARCA,ARECIBO) !!!!!!!!!!!!!!!!!!!! */
/* Computing 2nd power */
    r__1 = mlat / 22.41f;
    hmaxd = exp(-(r__1 * r__1)) * 60.f + 210.f;
    hmaxn = 150.f;
    blote_1.ahh[1] = hpol_(&hour, &hmaxd, &hmaxn, &sax, &sux, &c_b48, &c_b48);
/* Computing 2nd power */
    r__1 = mlat / 33.f;
    tmaxd = exp(-(r__1 * r__1)) * 800.f + 1500.f;
    tmaxn = tn_(&hmaxn, &texni, &tlbdn, &signi) + 20;
    ate[1] = hpol_(&hour, &tmaxd, &tmaxn, &sax, &sux, &c_b48, &c_b48);
/* !!!!!!!!!! TE(300,400KM)=TE-AE-C !!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* !!!!!!!!!! TE(1400,3000KM)=TE-ISIS !!!!!!!!!!!!!!!!!!!!!!!!!!! */
    diplat = magbr;
    teba_(&diplat, &hour, &nseson, tea);
    ate[2] = tea[0];
    ate[3] = tea[1];
    ate[5] = tea[2];
    ate[6] = tea[3];
/* !!!!!!!!!! TE(600KM)=TE-AEROS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    ett = exp(-mlat / 11.35f);
    d__1 = (doublereal) (ett + 1);
    tet = 2900.f - ett * 5600.f / pow_dd(&d__1, &c_b113);
    ten = 1161.f / (exp(-(absmlt - 45.f) / 5.f) + 1.f) + 839.f;
    ate[4] = hpol_(&hour, &tet, &ten, &sax, &sux, &c_b114, &c_b114);
/* !!!!!!!!!! OPTION TO USE TE-NE-RELATION !!!!!!!!!!!!!!!!!!!!!! */
/* !!!!!!!!!! AT 300, 400 OR 600 KM  !!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    if (teneop) {
	for (i__ = 1; i__ <= 3; ++i__) {
/* L3395: */
	    if (tcon[i__ - 1]) {
		r__1 = -cov;
		ate[i__ + 1] = tede_(&hoa[i__ - 1], &xnar[i__ - 1], &r__1);
	    }
	}
    }
/* !!!!!!!!!! TE'S ARE CORRECTED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* !!!!!!!!!! ALSO TE > TN ENFORCED !!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    tnahh2 = tn_(&blote_1.ahh[1], &blotn_1.texos, &blotn_1.tlbdh, &
	    blotn_1.sigma);
    if (ate[1] < tnahh2) {
	ate[1] = tnahh2;
    }
    stte1 = (ate[1] - ate[0]) / (blote_1.ahh[1] - blote_1.ahh[0]);
    for (i__ = 2; i__ <= 6; ++i__) {
	tnahhi = tn_(&blote_1.ahh[i__], &blotn_1.texos, &blotn_1.tlbdh, &
		blotn_1.sigma);
	if (ate[i__] < tnahhi) {
	    ate[i__] = tnahhi;
	}
	stte2 = (ate[i__] - ate[i__ - 1]) / (blote_1.ahh[i__] - blote_1.ahh[
		i__ - 1]);
	ate[i__ - 1] -= (stte2 - stte1) * blote_1.dte[i__ - 2] * alog2;
/* L1901: */
	stte1 = stte2;
    }
/* !!!!!!!!!! GRADIENTS ARE CALCULATED WITH !!!!!!!!!!!!!!!!!!!! */
/* !!!!!!!!!! CORRECTED REGION BOUNDARIES !!!!!!!!!!!!!!!!!!!!!! */
    for (i__ = 1; i__ <= 6; ++i__) {
/* L1902: */
	blote_1.stte[i__ - 1] = (ate[i__] - ate[i__ - 1]) / (blote_1.ahh[i__] 
		- blote_1.ahh[i__ - 1]);
    }
    blote_1.ate1 = ate[0];
/* L887: */

/* ------------ CALCULATION OF ION TEMPERATURE PARAMETERS-------- */

/* !!!!!!!!!! TI(430KM,DAY)=TI-AEROS !!!!!!!!!!!!!!!!!!!!!!!!!!! */
    blotn_1.xsm1 = 430.f;
    block8_1.xsm[0] = blotn_1.xsm1;
    z1 = exp(mlat * -.09f);
    z2 = z1 + 1.f;
    tid1 = 1240.f - z1 * 1400.f / (z2 * z2);
    block8_1.mm[1] = hpol_(&hour, &c_b120, &c_b107, &sax, &sux, &c_b48, &
	    c_b48);
/* !!!!!!!!!!  TI < TE   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    ted1 = tea[5] + 30.f;
    if (tid1 > ted1) {
	tid1 = ted1;
    }
/* !!!!!!!!!! TI(430KM,NIGHT)=TI-AEROS !!!!!!!!!!!!!!!!!!!!!!!!! */
    z1 = absmlt;
    z2 = z1 * (z1 * .024f + .47f) * const_1.umr;
    z3 = cos(z2);
    tin1 = 1200.f - r_sign(&c_b48, &z3) * 300.f * sqrt((abs(z3)));
/* !!!!!!!!!! TN < TI < TE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    ten1 = tea[4];
    tnn1 = tn_(&blotn_1.xsm1, &texni, &tlbdn, &signi);
    if (ten1 < tnn1) {
	ten1 = tnn1;
    }
    if (tin1 > ten1) {
	tin1 = ten1;
    }
    if (tin1 < tnn1) {
	tin1 = tnn1;
    }
/* !!!!!!!!!! TI(430KM,LT) FROM STEP FUNCTION !!!!!!!!!!!!!!!!!! */
    ti1 = tin1;
    if (tid1 > tin1) {
	ti1 = hpol_(&hour, &tid1, &tin1, &sax, &sux, &c_b48, &c_b48);
    }
/* !!!!!!!!!! TANGENT ON TN DETERMINES HS !!!!!!!!!!!!!!!!!!!!!! */
    ti13 = teder_(&c_b127);
    ti50 = teder_(&c_b128);
    regfa1_(&c_b127, &c_b128, &ti13, &ti50, &c_b131, &ti1, (R_fp)teder_, &
	    schalt, &block8_1.hs);
    if (schalt) {
	block8_1.hs = 200.f;
    }
    block8_1.tnhs = tn_(&block8_1.hs, &blotn_1.texos, &blotn_1.tlbdh, &
	    blotn_1.sigma);
    block8_1.mm[0] = dtndh_(&block8_1.hs, &blotn_1.texos, &blotn_1.tlbdh, &
	    blotn_1.sigma);
    if (schalt) {
	block8_1.mm[0] = (ti1 - block8_1.tnhs) / (blotn_1.xsm1 - block8_1.hs);
    }
    block8_1.mxsm = 2;
/* !!!!!!!!!! XTETI ALTITTUDE WHERE TE=TI !!!!!!!!!!!!!!!!!!!!!! */
/* L2391: */
    xtts = 500.f;
    x = 500.f;
L2390:
    x += xtts;
    if (x >= blote_1.ahh[6]) {
	goto L240;
    }
    tex = elte_(&x);
    tix = ti_(&x);
    if (tix < tex) {
	goto L2390;
    }
    x -= xtts;
    xtts /= 10.f;
    if (xtts > .1f) {
	goto L2390;
    }
    xteti = x + xtts * 5.f;
/* !!!!!!!!!! TI=TE ABOVE XTETI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    block8_1.mxsm = 3;
    block8_1.mm[2] = blote_1.stte[5];
    block8_1.xsm[1] = xteti;
    if (xteti > blote_1.ahh[5]) {
	goto L240;
    }
    block8_1.mxsm = 4;
    block8_1.mm[2] = blote_1.stte[4];
    block8_1.mm[3] = blote_1.stte[5];
    block8_1.xsm[2] = blote_1.ahh[5];
    if (xteti > blote_1.ahh[4]) {
	goto L240;
    }
    block8_1.mxsm = 5;
    block8_1.dti[0] = 5.f;
    block8_1.dti[1] = 5.f;
    block8_1.mm[2] = blote_1.stte[3];
    block8_1.mm[3] = blote_1.stte[4];
    block8_1.mm[4] = blote_1.stte[5];
    block8_1.xsm[2] = blote_1.ahh[4];
    block8_1.xsm[3] = blote_1.ahh[5];

/* CALCULATION OF ION DENSITY PARAMETER.................. */

L240:
    if (noion) {
	goto L141;
    }
    hnia = 100.f;
    hnie = 2e3f;
    if (dy) {
	goto L141;
    }

/* INPUT OF THE ION DENSITY PARAMETER ARRAYS PF1O,PF2O AND PF3O...... */

    rif[0] = 2.f;
    if (abslat < 30.f) {
	rif[0] = 1.f;
    }
    rif[1] = 2.f;
    if (cov < 100.f) {
	rif[1] = 1.f;
    }
    rif[2] = (real) season;
    if (season == 1) {
	rif[2] = 3.f;
    }
    rif[3] = 1.f;
    if (block5_1.night) {
	rif[3] = 2.f;
    }
    koefp1_(pg1o);
    koefp2_(pg2o);
    koefp3_(pg3o);
    sufe_(pg1o, rif, &c__12, pf1o);
    sufe_(pg2o, rif, &c__4, pf2o);
    sufe_(pg3o, rif, &c__12, pf3o);

/* calculate O+ profile parameters */

    if (abs(xhi) <= 90.f) {
	zzz1 = cos(xhi * const_1.umr);
    } else {
	zzz1 = 0.f;
    }
    msumo = 4;
    rdomax = 100.f;
    mo[0] = epstep_(pf1o, &pf1o[1], &pf1o[2], &pf1o[3], &zzz1);
    mo[1] = epstep_(&pf1o[4], &pf1o[5], &pf1o[6], &pf1o[7], &zzz1);
    mo[2] = 0.f;
    ho[0] = epstep_(&pf1o[8], &pf1o[9], &pf1o[10], &pf1o[11], &zzz1);
    ho[1] = 290.f;
    if (rif[1] == 2.f && rif[2] == 2.f) {
	ho[1] = 237.f;
    }
    ho[3] = pf2o[0];
    ho05 = pf2o[3];
    mo[3] = pf2o[1];
    mo[4] = pf2o[2];

/* adjust gradient MO(4) of O+ profile segment above F peak */

L7100:
    ho[2] = (alg100 - mo[4] * (ho[3] - ho05)) / mo[3] + ho[3];
    if (ho[2] <= ho[1] + 20.f) {
	mo[3] += -.001f;
	goto L7100;
    }
    hfixo = (ho[1] + ho[2]) / 2.f;

/* find height H0O of maximum O+ relative density */

    delx = 5.f;
    x = ho[1];
    ymaxx = 0.f;
L7102:
    x += delx;
    y = rpid_(&x, &hfixo, &rdomax, &msumo, mo, ddo, ho);
    if (y <= ymaxx) {
	if (delx <= .1f) {
	    goto L7104;
	}
	x -= delx;
	delx /= 5.f;
    } else {
	ymaxx = y;
    }
    goto L7102;
L7104:
    h0o = x - delx / 2.f;
L7101:
    if (y < 100.f) {
	goto L7103;
    }
    rdomax += -.01f;
    y = rpid_(&h0o, &hfixo, &rdomax, &msumo, mo, ddo, ho);
    goto L7101;
L7103:
    yo2h0o = 100.f - y;
    yoh0o = y;

/* calculate parameters for O2+ profile */

    hfixo2 = pf3o[0];
    rdo2mx = pf3o[1];
    for (l = 1; l <= 2; ++l) {
	i__ = l << 1;
	ho2[l - 1] = pf3o[i__] + pf3o[i__ + 1] * zzz1;
/* L7105: */
	mo2[l] = pf3o[i__ + 6] + pf3o[i__ + 7] * zzz1;
    }
    mo2[0] = pf3o[6] + pf3o[7] * zzz1;
    if (hfixo2 > ho2[0]) {
	ymo2z = mo2[1];
    } else {
	ymo2z = mo2[0];
    }
    aldo21 = log(rdo2mx) + ymo2z * (ho2[0] - hfixo2);
    hfixo2 = (ho2[1] + ho2[0]) / 2.f;
    rdo2mx = exp(aldo21 + mo2[1] * (hfixo2 - ho2[0]));

/* make sure that rd(O2+) is less or equal 100-rd(O+) at O+ maximum */

L7106:
    y = rpid_(&h0o, &hfixo2, &rdo2mx, &c__2, mo2, do2, ho2);
    if (y > yo2h0o) {
	mo2[2] += -.02f;
	goto L7106;
    }

/* use ratio of NO+ to O2+ density at O+ maximum to calculate */
/* NO+ density above the O+ maximum (H0O) */

    if (y < 1.f) {
	nobo2 = 0.f;
    } else {
	nobo2 = (yo2h0o - y) / y;
    }

/* CALCULATION FOR THE REQUIRED HEIGHT RANGE....................... */

L141:
    if (! f1reg) {
	block1_1.hmf1 = block3_1.hz;
    }
    for (ki = 1; ki <= 11; ++ki) {
	for (kk = 1; kk <= 50; ++kk) {
/* L7397: */
	    outf[ki + kk * 11] = -1.f;
	}
    }
    height = *heibeg;
    kk = 1;
L300:
    if (noden) {
	goto L330;
    }
    if (height > hnee || height < hnea) {
	goto L330;
    }
    if (layver) {
	elede = -9.f;
	if (iiqu < 2) {
	    elede = xen_(&height, &block1_1.hmf2, &block1_1.nmf2, &
		    block4_1.hme, &c__4, hxl, scl, amp);
	}
    } else {
	elede = xe_(&height);
    }
    outf[kk * 11 + 1] = elede;
L330:
    if (notem) {
	goto L7108;
    }
    if (height > hte || height < hta) {
	goto L7108;
    }
    tnh = tn_(&height, &blotn_1.texos, &blotn_1.tlbdh, &blotn_1.sigma);
    tih = tnh;
    if (height >= block8_1.hs) {
	tih = ti_(&height);
    }
    teh = elte_(&height);
    if (tih < tnh) {
	tih = tnh;
    }
    if (teh < tih) {
	teh = tih;
    }
    outf[kk * 11 + 2] = tnh;
    outf[kk * 11 + 3] = tih;
    outf[kk * 11 + 4] = teh;
L7108:
    if (noion) {
	goto L7118;
    }
    if (height > hnie || height < hnia) {
	goto L7118;
    }
    if (dy) {
	r__1 = xhi * const_1.umr;
	r__2 = lati * const_1.umr;
	ioncom_(&height, &r__1, &r__2, &cov, &month, dion);
	rox = dion[0];
	rhx = dion[1];
	rnx = dion[2];
	rhex = dion[3];
	rnox = dion[4];
	ro2x = dion[5];
	rclust = dion[6];
    } else {
	rox = rpid_(&height, &hfixo, &rdomax, &msumo, mo, ddo, ho);
	ro2x = rpid_(&height, &hfixo2, &rdo2mx, &c__2, mo2, do2, ho2);
	rdhhe_(&height, &h0o, &rox, &ro2x, &nobo2, &c_b153, &rhx, &rhex);
	rnox = rdno_(&height, &h0o, &ro2x, &rox, &nobo2);
	rnx = -1.f;
	rclust = -1.f;
    }
    outf[kk * 11 + 5] = rox;
    outf[kk * 11 + 6] = rhx;
    outf[kk * 11 + 7] = rhex;
    outf[kk * 11 + 8] = ro2x;
    outf[kk * 11 + 9] = rnox;
    outf[kk * 11 + 10] = rnx;
    outf[kk * 11 + 11] = rclust;
L7118:
    height += *heistp;
    ++kk;
    if (kk <= numhei) {
	goto L300;
    }

/* ADDITIONAL PARAMETER FIELD OARR */

    if (noden) {
	goto L6192;
    }
    oarr[1] = block1_1.nmf2;
    oarr[2] = block1_1.hmf2;
    oarr[3] = nmf1;
    oarr[4] = block1_1.hmf1;
    oarr[5] = block4_1.nme;
    oarr[6] = block4_1.hme;
    oarr[7] = block6_1.nmd;
    oarr[8] = block6_1.hmd;
    oarr[9] = hhalf;
    oarr[10] = block2_1.b0;
    oarr[11] = vner;
    oarr[12] = block4_1.hef;
L6192:
    if (notem) {
	goto L6092;
    }
    oarr[13] = ate[1];
    oarr[14] = blote_1.ahh[1];
    oarr[15] = ate[2];
    oarr[16] = ate[3];
    oarr[17] = ate[4];
    oarr[18] = ate[5];
    oarr[19] = ate[6];
    oarr[20] = ate[0];
    oarr[21] = ti1;
    oarr[22] = xteti;
L6092:
    oarr[23] = xhi;
    oarr[24] = sundec;
    oarr[25] = dip;
    oarr[26] = magbr;
    oarr[27] = modip;
L3330:
    return 0;
} /* iris12_ */

