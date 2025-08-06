/* PUBLIC HEADER FILE OF ELECTRON BEAM TRACER.

   Apostolos Lerios - TOLIS@NOVA. */


/* APPROXIMATION PARAMETERS (real values). */

/* The margin of error (in terms of meters of distance) in (helical)
   beam intersection reporting. The error vector lies in a direction
   perpendicular to the intersected or missed surface.
   0.0<LENGTH_ACCCURACY. */

#define LENGTH_ACCURACY 0.00001

/* The maximum (normal) distance of the generator emergence point from
   a surface plane, in meters, such that the point is considered to lie
   on the plane. 0.0<=PLANE_DISTANCE<0.1. */

#define PLANE_DISTANCE 0.0000001

/* Beam tracing (Beam_Trace() function):
   (i) The length of each helix segment, in meters, which will be
      approximated by a line segment during tracing: real such that
      0.0<SEGMENT_LENGTH.
   (ii) The length, in meters, of the linear beam which will be traced
      in ROTATIONS tracing mode (in which case, the concept of electron
      rotations does not apply): real such that 0.0<LINEAR_LENGTH. */

#define SEGMENT_LENGTH 0.30
#define LINEAR_LENGTH  40.0
