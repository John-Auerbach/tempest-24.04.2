/* PUBLIC HEADER FILE OF ADVANCED MATH ROUTINES.

   Apostolos Lerios - TOLIS@NOVA. */


/* APPROXIMATION PARAMETERS (real values). */

/* For two vectors to be considered perpendicular, the absolute value
   of the cosine of their angle should be smaller than
   PERPENDICULAR_COSINE. 0.0<PERPENDICULAR_COSINE<=1.0. */

#define PERPENDICULAR_COSINE 0.01

/* For two vectors to be considered parallel, the sine of their angle
   should be smaller than PARALLEL_SINE. 0.0<PARALLEL_SINE<=1.0. */

#define PARALLEL_SINE PERPENDICULAR_COSINE

/* This approximation is used in Eqn_NextSolution(), to determine the
   relative weight of the alpha*cos(NewOmega*t) and beta*sin(NewOmega*t)
   terms of the equation; if fabs(beta/alpha)>RELATIVE_IMPORTANCE, then
   fabs(alpha)<<fabs(beta), and thus alpha is set to 0. */

#define RELATIVE_IMPORTANCE 1E4
