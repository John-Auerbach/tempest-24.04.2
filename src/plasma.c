/*****************************************************************************/
/*                                                                           */
/*   Module:    plasma.c                                                     */
/*                                                                           */
/*   Purpose: 	This module computes plasma parameters based on value from   */
/*              geophysical models like IRI, IGRF, and MSIS.                 */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   27_Oct_95 NRV   Written.                                     */
/*                                                                           */
/*   RCS:       $Id: plasma.c,v 1.1 1995/11/07 16:56:04 nestorv Exp $                                                        */
/*                                                                           */
/*              $Log: plasma.c,v $
 * Revision 1.1  1995/11/07  16:56:04  nestorv
 * Initial revision
 *
 * Revision 1.1  1995/11/07  16:56:04  nestorv
 * Initial revision
 *                                                       */
/*                                                                           */
/*****************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "advmath.h"
#include "earth.h"
#include "orbit.h"

#include "types.h"
#include "gmt.h"
#include "tempest.h"
#include "temputil.h"
#include "iri.h"
#include "bfield.h"
#include "plasma.h"

void init_plasma_params ()
{
}



void compute_plasma_params ()
{
   gyro_radius_electron = ELECT_MASS * perp_velocity / 
                          (Q_CHARGE * bfield_loc_mag) ;
   gyro_radius_ion      = ion_mass * perp_velocity /
                          (Q_CHARGE * bfield_loc_mag) ;

   debye_length   = sqrt ((EPSILON_0 * K_BOLTZ * iri_elect_temp) / 
                           (Q_CHARGE * Q_CHARGE * iri_elect_density)) ;
   mean_free_path = 100.0 ;

   mean_speed_electron = sqrt ((8.0 * K_BOLTZ * iri_elect_temp) /
                               (M_PI * ELECT_MASS)) ;

   mean_speed_ion      = sqrt ((8.0 * K_BOLTZ * iri_ion_temp  ) /
                               (M_PI * ion_mass)) ;

   sound_speed_ion     = (K_BOLTZ * (iri_elect_temp + 3.0 * iri_ion_temp)) /
                                ion_mass ;

   plasma_freq_electron = Q_CHARGE * sqrt (iri_elect_density / 
                                                    (EPSILON_0 * ELECT_MASS)) ;
   plasma_freq_ion      = Q_CHARGE * sqrt (iri_ion_density /
                                                    (EPSILON_0 * ion_mass  )) ;

   gyro_freq_electron = Q_CHARGE * bfield_loc_mag / ELECT_MASS ;
   gyro_freq_ion      = Q_CHARGE * bfield_loc_mag / ion_mass ;

   coll_freq_elect_ion = 1735.0 ;
}
