/* PUBLIC HEADER FILE OF EARTH MODELLING ROUTINES.

   Apostolos Lerios - TOLIS@NOVA. */


/* MAGNETIC FIELD MODELLING. */

/* IGRF model year (1985, 1990 currently supported). */

/* #define IGRF_YEAR 1990 */


/* APPROXIMATION PARAMETERS (real values). */

/* Assumed angular deviation of the poles from 0 or 180 deg, in
   radians. PI>=POLES>=0.0. */

#define POLES DTR(1E-4)

/* Accuracy in geocentric to geodetic (and back) latitude conversion.
   A small real number is required, but an extremely small value may
   cause infinite looping because of floating point rounding errors. This
   value is not the error bound of the conversion result. */

#define LATITUDE_CONVERSION_ACCURACY 1E-12
