/*****************************************************************************/
/*                                                                           */
/*   Module:    tss_current.c                                                */
/*                                                                           */
/*   Purpose:	Compute available tether current based on a variety of TSS   */
/*		current collection modes (Passive,TCVM,FPEG,EGA,TCVM+FPEG).  */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      Declarations from: "types.h" and "advmath.h" from the        */
/*              science library.                                             */
/*                                                                           */
/*   History:   13_Aug_95 NRV   Written.                                     */
/*                                                                           */
/*   RCS:	$Id: tss_current.c,v 1.17 1999/04/01 19:41:36 nestorv Exp $                                                        */
/*                                                                           */
/*              $Log: tss_current.c,v $
 * Revision 1.17  1999/04/01  19:41:36  nestorv
 * Don't remember latest revs - it has been a LONG time.
 *
 * Revision 1.16  1996/02/26  12:38:48  nestorv
 * handle negative tether lengths.
 *
 * Revision 1.15  1996/02/10  20:02:11  nestorv
 * Check on tether length for ixb to eliminate divide by 0.0.
 *
 * Revision 1.14  1996/02/01  06:11:37  nestorv
 * Made sure that debugging statements go to debug_out.
 *
 * Revision 1.13  1995/11/03  23:13:00  nestorv
 * Corrected typo.
 *
 * Revision 1.12  1995/11/03  23:10:24  nestorv
 * Added setting of non-mode parameter to appropriate values.
 *
 * Revision 1.11  1995/11/03  21:01:53  nestorv
 * Added PEAK_N_VOLT to output.
 *
 * Revision 1.10  1995/11/01  21:32:35  nestorv
 * Added PEAK_VOLT (v_peak_volt) = EMF + Trans parameter to module.
 *
 * Revision 1.9  1995/10/27  21:51:32  nestorv
 * Moved internal computed values to display parameter list.
 *
 * Revision 1.8  1995/10/18  19:42:30  nestorv
 * In FPEG mode, limit Vorb to 1000.0 Volts.
 *
 * Revision 1.7  1995/10/10  18:24:10  nestorv
 * Added real computations for FPEG case.
 *
 * Revision 1.6  1995/10/07  06:00:52  nestorv
 * Added computation of IxB force.
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
#include "tether.h"
#include "genorbit.h"
#include "bfield.h"
#include "emf.h"
#include "iri.h"
#include "tss_current.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void init_tss_current_params ()
{
   int count ;

   count = 0 ;
   while ( (count < NUM_TSS_MODES) && 
         (strcmp (strupper (tss_current_mode_str), tss_mode_str [count]) != 0) )
      count++ ;

   if (count < NUM_TSS_MODES)
      tss_current_mode = count ;
   else
   {
      fprintf(stderr,"WARNING: Invalid TSS current mode [%s] specified!!\007\n",
                        tss_current_mode_str) ;
      tss_current_mode = PASSIVE ;
   }

   if (comm_i_ega < 0.0 || comm_i_ega > 0.8)
   {
      fprintf (stderr, "WARNING: Commanded EGA Current not valid!!\007\n") ;
      comm_i_ega = 0.0 ;
   }

   switch (comm_fpeg_gun)
   {
      case 0  : Ifpeg = 0.000 ;
                break ;
      case 1  : Ifpeg = 0.100 ;  /* Actually should be 0.09048 (maybe) */
                break ;
      case 2  : Ifpeg = 0.100 ;  /* Actually should be 0.09048 (maybe) */
                break ;
      case 3  : Ifpeg = 0.200 ;  /* Actually should be 0.18095 (maybe) */
                break ;
      default : fprintf (stderr, "WARNING: %s!!\007\n",
                                 "Commanded FPEG Current not valid!!\007\n") ;
                Ifpeg = 0.000 ;
                break ;
   }

   count = 0 ;
   while ( (count < NUM_SETS_RLOADS) && 
         (strcmp (strupper (cmd_r_str), tcvm_resistor_str [count]) != 0) )
      count++ ;

   if (count < NUM_SETS_RLOADS)
   {
      comm_r_tcvm = count ;
      Rtcvm       = tcvm_resistor_value [comm_r_tcvm] ;
   }
   else
   {
      fprintf(stderr,"WARNING: Commanded SETS Load Resistor [%s] not valid!!\007\n",
                        cmd_r_str) ;
      comm_r_tcvm = OPEN ;
      Rtcvm       = tcvm_resistor_value [comm_r_tcvm] ;
   }

}



void compute_tss_current ()
{
   double orb_eff_rad ;            /* Effective radius for orbiter e coll.   */
   double Vthe        ;            /* Electron thermal velocity              */
   double foeorb      ;            /* P-M Const for Orbiter Elec Collection  */
   double foesat      ;            /* P-M Const for Satellite Elec Collection*/
   double VegaTest    ;            /* Test EGA voltage                       */
   double Vega        ;            /* EGA voltage                            */
   double Itempa      ;            /* Test tether current                    */
   double It          ;            /* Tether current                         */
   double Vorbtest    ;            /* Test orbiter potential                 */
   double Vorb        ;            /* Orbiter potential                      */
   double Vsat        ;            /* Satellite potential                    */

   Vthe = sqrt (8.0 * K_BOLTZ * iri_elect_temp / (M_PI * M_EL)) ;

   Jeo = 0.25 * Q_EL * iri_elect_density * Vthe ;
   Ioeorb = Jeo * a_e_orb ;
   Ioesat = (2.0 * M_PI * sat_radius * sat_radius) * Jeo ;
   
   Jio = Q_EL * iri_elect_density * ORB_VELOCITY ;

   Ioiorb = Jio * a_i_orb ;

   orb_eff_rad = sqrt (a_e_orb/(2.0*M_PI)) ;

   foeorb = 178.0 * orb_eff_rad * orb_eff_rad *
                   (bfield_loc_mag / 0.45e-4) * (bfield_loc_mag / 0.45e-4) ;

   foesat = 178.0 * sat_radius  * sat_radius  *
                   (bfield_loc_mag / 0.45e-4) * (bfield_loc_mag / 0.45e-4) ;

#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
{
   fprintf (debug_out, "tss_current_mode = %s\nIoeorb=%f Ioesat=%f Ioiorb=%f\n",
                    tss_mode_str[tss_current_mode],Ioeorb,Ioesat,Ioiorb) ;
   fprintf (debug_out, "foeorb=%f foesat=%f\n", foeorb, foesat) ;
}
#endif

   switch (tss_current_mode)
   {
      case EGA       : Iega = comm_i_ega ;
                       VegaTest = (4704.8*Iega-2396.6*Iega*Iega) ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "Iega=%f VegaTest = %f\n", Iega, VegaTest) ; 
#endif

                       if(Iega <= Ioesat)
                       {
                          Vega = vxb_l - Iega*Rt ;
                          Vsat = 0.0 ;

#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Iega <= Ioesat] Vega=%f Vsat=%f\n", Vega, Vsat) ; 
#endif
                          if (Vega >= VegaTest)
                             It = Iega ;
                          else
                          {
                             It   = 0.000041726 * (23524.0 + 5.0 *Rt - 
                                    pow(pow(-23524.0 - 5.0*Rt,2.0) -
                                    239660.*vxb_l,0.5)) ;
                             Vega = 4704.8*It - 2396.6*It*It ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Vega <  VegaTest] Vega=%f Vsat=%f\n", Vega, Vsat) ; 
#endif
                          }
                       }
                       else
                       {
                          Vsat = 0.25*foesat*pow(Iega/Ioesat - 1.0,2.0) ;
                          Vega = vxb_l - Iega*Rt - Vsat ;

#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Iega >  Ioesat] Vega=%f Vsat=%f\n", Vega, Vsat) ;
#endif
                          if (Vega >= VegaTest)
                             It = Iega ;
                          else
                          {
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Vega <  VegaTest] Vega=%f Vsat=%f\n", Vega, Vsat) ;
#endif
                             It = 0.5*(-1.0*Ioesat*
                                  (10.0*foesat-94096.0*Ioesat-20.0*Ioesat*Rt) - 
                                  pow(Ioesat*Ioesat* pow(10.0*foesat -
                                  94096.0*Ioesat - 20.0*Ioesat*Rt,2.0) - 
                                  4.0*Ioesat*Ioesat* (-5.0*foesat + 
                                  47932.0*Ioesat*Ioesat)*
                                  (-5.0*foesat + 20.0*vxb_l),0.5)) /
                                  (-5.0*foesat + 47932.0*Ioesat*Ioesat) ;
                             Vega = 4704.8*It - 2396.6*It*It ;
                             Vsat = 0.25*foesat*pow(It/Ioesat - 1,2.0) ;
                          }
                       }

                       itcm_sets  = sa_score = It ;
                       tvmdc_sets = dv_dcore = -1.0 * Vega ;
                       Rtcvm      = R_TCVM_INF ;
                       v_tether   = It * Rt ;
                       vorb_spree = 0.0 ;
                       dcbp_rete  = Vsat ;
                       v_sanity   = dcbp_rete + v_tether - tvmdc_sets -
                                    vorb_spree - vxb_l ;
                       Ifpeg      =        0.0   ;
                       break ;
      case TCVM      :
                       Itempa = 0.5*(-1.0*Ioesat*Ioiorb*Ioiorb*
                          (-2.0*foesat + 4.0*Ioesat*Rt + 4.0*Ioesat*Rtcvm) + 
                          sqrt(Ioesat*Ioesat*pow(Ioiorb,4.0)*
                          pow(-2.0*foesat+4.0*Ioesat*Rt+4.0*Ioesat*Rtcvm,2.0) 
                          - 4.0*Ioesat*Ioesat*Ioiorb*Ioiorb*
                          (foesat - 4.0*vxb_l - 4.0*v_ion_ram)*
                          (foesat*Ioiorb*Ioiorb + 
                          4.0*Ioesat*Ioesat*v_ion_ram)))/
                          (foesat*Ioiorb*Ioiorb + 4.0*Ioesat*Ioesat*v_ion_ram) ;

                       if (Itempa >= Ioiorb)
                       {
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Itempa >= Ioiorb] Itempa = %f\n", Itempa) ;
#endif
                          if (Itempa >= Ioesat)
                          {
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Itempa >= Ioesat] Itempa = %f\n", Itempa) ;
#endif
                             Vsat      = 0.25 * foesat *
                                         pow ((Itempa/Ioesat-1.0), 2.0) ;
                             Vorb      = v_ion_ram *
                                         (1.0 - pow((Itempa/Ioiorb), 2.0)) ;
                          }
                          else
                          {
                             Itempa = 0.5*(-1.0*Ioiorb*Ioiorb*(Rt + Rtcvm) + 
                                      pow(pow(Ioiorb,4.0)*pow(Rt + Rtcvm,2.0) - 
                                      4.0*Ioiorb*Ioiorb*
                                      (-1.0*vxb_l - v_ion_ram)*
                                      v_ion_ram,0.5))/v_ion_ram ;
   
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Itempa <= Ioesat] Itempa = %f\n", Itempa) ; 
#endif
                             Vsat   = 0.0 ;
                             Vorb   = v_ion_ram * 
                                      (1.0 - pow((Itempa/Ioiorb), 2.0)) ;
                          }
                       }
                       else
                       {
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Itempa < Ioiorb] Itempa = %f\n", Itempa) ;
#endif
                          Itempa = vxb_l / (Rt + Rtcvm) ;
                          Vsat   = 0.0 ;
                          Vorb   = 0.0 ;
                       }

                       itcm_sets  = sa_score = Itempa ;
                       tvmdc_sets = dv_dcore = -1.0 * itcm_sets * Rtcvm ;
                       v_tether   = Itempa * Rt ;
                       vorb_spree = Vorb ;
                       dcbp_rete  = Vsat ;
                       v_sanity   = dcbp_rete + v_tether - tvmdc_sets -
                                    vorb_spree - vxb_l ;
                       Iega       = 0.0 ;
                       Ifpeg      = 0.0 ;
                       break ;
      case FPEG      : if (Ifpeg >= Ioeorb)
                       {
                          Vorb = 0.25*foeorb*
                             pow(Ifpeg/Ioeorb - 1.0,2.0) ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Ifpeg >= Ioeorb] Vorb = %f\n", Vorb) ;
#endif
                          if (Vorb > 1000.0)
                          {
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Vorb > 1000.0] Vorb = %f\n", Vorb) ;
#endif
                             Vorb = 1000.0 ;
                          }
                       }
                       else
                       {
                          Vorb = 0.0 ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Ifpeg < Ioeorb] Vorb = %f\n", Vorb) ;
#endif
                       }
                       itcm_sets  = 0.0                 ;
                       sa_score   = 0.0                 ;
                       tvmdc_sets = -1.0 * vxb_l - Vorb ;
                       v_tether   = 0.0                 ;
                       dv_dcore   = -1.0 * vxb_l - Vorb ;
                       vorb_spree = Vorb                ;
                       dcbp_rete  = 0.0                 ;
                       Rtcvm      = R_TCVM_INF          ;
                       Iega       = 0.0                 ;
                       v_sanity   = dcbp_rete + v_tether - tvmdc_sets -
                                    vorb_spree - vxb_l ;
                       break ;
      case TCVM_FPEG :
                       It = (0.5*(-1.0 * Ioesat*(-2.0*foesat*Ioeorb*Ioeorb + 
                          2.0*foeorb*Ifpeg*Ioesat - 2.0*foeorb*Ioeorb*Ioesat + 
                          4.0*Ioeorb*Ioeorb*Ioesat*Rt + 
                          4.0*Ioeorb*Ioeorb*Ioesat*Rtcvm) + 
                          sqrt(Ioesat*Ioesat*
                          pow(-2.*foesat*Ioeorb*Ioeorb + 
                          2.0*foeorb*Ifpeg*Ioesat - 2.0*foeorb*Ioeorb*Ioesat + 
                          4.0*Ioeorb*Ioeorb*Ioesat*Rt + 
                          4.0*Ioeorb*Ioeorb*Ioesat*Rtcvm,2.0) - 
                          4.0*Ioesat*Ioesat*
                          (foesat*Ioeorb*Ioeorb - foeorb*Ioesat*Ioesat)*
                          (-1.0*foeorb*Ifpeg*Ifpeg + 2.0*foeorb*Ifpeg*Ioeorb - 
                          foeorb*Ioeorb*Ioeorb + foesat*Ioeorb*Ioeorb - 
                          4.0*Ioeorb*Ioeorb*vxb_l))))/
                          (foesat*Ioeorb*Ioeorb - foeorb*Ioesat*Ioesat) ;

#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "It = %f\n", It) ;
#endif
                       if (It <= Ifpeg)
                       {
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It <= Ifpeg]\n") ;
#endif
                          if ((Ifpeg - It) >= Ioeorb)
                          {
                             Vorbtest = 0.25*foeorb*
                                pow((Ifpeg - It)/Ioeorb - 1.0,2.0) ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Ifpeg - It >= Ioeorb] Vorbtest = %f\n", Vorbtest) ;
#endif
                             if (Vorbtest <= 1000.0)
                             {
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Vorbtest <= 1000.0]\n") ;
#endif
                                if (It >= Ioesat)
                                {
                                   Vorb = 0.25*foeorb*
                                      pow((Ifpeg - It)/Ioeorb - 1.0,2.0) ;
                                   Vsat = 0.25*foesat*
                                      pow(It/Ioesat - 1.0,2.0) ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It >= Ioesat] Vorb=%f Vsat=%f\n", Vorb,Vsat) ;
#endif
                                }
                                else
                                {
                                   It = 0.5*(2.0*foeorb*Ifpeg -
                                        2.0*foeorb*Ioeorb + 
                                        4.0*Ioeorb*Ioeorb*Rt + 
                                        4.0*Ioeorb*Ioeorb*Rtcvm - 
                                        pow(pow(-2.0*foeorb*Ifpeg + 
                                        2.0*foeorb*Ioeorb - 
                                        4.0*Ioeorb*Ioeorb*Rt - 
                                        4.0*Ioeorb*Ioeorb*Rtcvm,2.0) - 
                                        4.0*foeorb*
                                        (foeorb*Ifpeg*Ifpeg - 
                                        2.0*foeorb*Ifpeg*Ioeorb + 
                                        foeorb*Ioeorb*Ioeorb + 
                                        4.0*Ioeorb*Ioeorb*vxb_l),0.5)) / 
                                        foeorb ;
                                   Vorb = 0.25*foeorb*
                                          pow((Ifpeg-It)/Ioeorb - 1.0,2.0) ;
                                   Vsat = 0.0 ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It < Ioesat] It=%f Vorb=%f Vsat=%f\n", It, Vorb,Vsat) ;
#endif
                                }
                             }
                             else
                             {
                                It = 0.5*(-1.0*Ioesat*(-2.0*foesat +
                                     4.0*Ioesat*Rt + 
                                     4.0*Ioesat*Rtcvm) + 
                                     pow(Ioesat*Ioesat*
                                     pow(-2.0*foesat + 4.0*Ioesat*Rt + 
                                     4.0*Ioesat*Rtcvm,2.0) - 
                                     4.0*foesat*Ioesat*Ioesat*
                                     (-4000.0+foesat-4.0*vxb_l),0.5))/foesat;

#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Vorbtest > 1000.0] It=%f\n", It) ;
#endif
                                if (It >= Ioesat)
                                {
                                   Vorb = 1000.0 ;
                                   Vsat = 0.25*foesat*
                                          pow(It/Ioesat - 1.0,2.0) ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It >= Ioesat] It=%f Vorb=%f Vsat=%f\n", It, Vorb, Vsat) ;
#endif
                                }
                                else
                                {
                                   It   = (vxb_l + 1000.0)/(Rt + Rtcvm) ;
                                   Vorb = 1000.0 ;
                                   Vsat = 0.0    ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It <  Ioesat] It=%f Vorb=%f Vsat=%f\n", It, Vorb, Vsat) ;
#endif
                                }
                             }
                          }
                          else
                          {
                             It = 0.5*(-(Ioesat*(-2.0*foesat +
                                  4.0*Ioesat*Rt + 4.0*Ioesat*Rtcvm)) + 
                                  pow(Ioesat*Ioesat*
                                  pow(-2.0*foesat + 4.0*Ioesat*Rt + 
                                  4.0*Ioesat*Rtcvm,2.0) - 
                                  4.0*foesat*Ioesat*Ioesat*
                                  (foesat - 4.0*vxb_l),0.5)) / foesat ;

#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[Ifpeg - It < Ioeorb] It=%f\n", It) ;
#endif
                             if (It >= Ioesat)
                             {
                                Vorb = 0.0 ;
                                Vsat = 0.25*foesat*
                                       pow(It/Ioesat - 1.0,2.0) ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It >= Ioesat] Vorb=%f Vsat=%f\n", Vorb, Vsat) ;
#endif
                             }
                             else
                             {
                                It   = vxb_l / (Rt + Rtcvm) ;
                                Vorb = 0.0 ;
                                Vsat = 0.0 ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It <  Ioesat] It=%f Vorb=%f Vsat=%f\n", It, Vorb, Vsat) ;
#endif
                             }
                          }
                       }
                       else
                       {
                          It = 0.5*(-1.0*Ioesat*(-2.*foesat*Ioiorb*Ioiorb + 
                               4.0*Ioesat*Ioiorb*Ioiorb*Rt + 
                               4.0*Ioesat*Ioiorb*Ioiorb*Rtcvm - 
                               8.0*Ifpeg*Ioesat*v_ion_ram) + 
                               pow(Ioesat*Ioesat*
                               pow(-2.0*foesat*Ioiorb*Ioiorb + 
                               4.0*Ioesat*Ioiorb*Ioiorb*Rt + 
                               4.0*Ioesat*Ioiorb*Ioiorb*Rtcvm - 
                               8.0*Ifpeg*Ioesat*v_ion_ram,2.0) - 
                               4.0*Ioesat*Ioesat*
                               (foesat*Ioiorb*Ioiorb + 
                               4.0*Ioesat*Ioesat*v_ion_ram)*
                               (1.0*foesat*Ioiorb*Ioiorb - 
                               4.0*Ioiorb*Ioiorb*vxb_l + 
                               4.0*Ifpeg*Ifpeg*v_ion_ram - 
                               4.0*Ioiorb*Ioiorb*v_ion_ram),0.5))/
                               (foesat*Ioiorb*Ioiorb +
                               4.0*Ioesat*Ioesat*v_ion_ram) ;

#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It > Ifpeg] It = %f\n", It) ;
#endif
                          if ((It - Ifpeg) >= Ioiorb)
                          {
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It - Ifpeg >= Ioiorb]\n") ;
#endif
                             if (It >= Ioesat)
                             {
                                Vorb = v_ion_ram*(1.0 - 
                                    pow((It - Ifpeg)/Ioiorb,2.0)) ;
                                Vsat = 0.25*foesat*pow(It/Ioesat - 1.0,2.0);
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It >= Ioesat] Vorb=%f Vsat=%f\n", Vorb, Vsat) ;
#endif
                             }
                             else
                             {
                                It = (-(Ioiorb*Ioiorb*Rt) - 
                                     Ioiorb*Ioiorb*Rtcvm + 
                                     2.0*Ifpeg*v_ion_ram + 
                                     pow(pow(Ioiorb*Ioiorb*Rt + 
                                     Ioiorb*Ioiorb*Rtcvm -
                                     2.0*Ifpeg*v_ion_ram,2.0) - 
                                     4.0*v_ion_ram*(-(Ioiorb*Ioiorb*vxb_l) + 
                                     Ifpeg*Ifpeg*v_ion_ram - 
                                     Ioiorb*Ioiorb*v_ion_ram),0.5)) /
                                     (2.0*v_ion_ram) ;
                                Vorb = v_ion_ram*(1.0 -
                                       pow((It - Ifpeg)/Ioiorb,2.0)) ;
                                Vsat = 0.0 ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It <  Ioesat] It=%f Vorb=%f Vsat=%f\n", It, Vorb, Vsat) ;
#endif
                             }
                          }
                          else
                          {
                             It = 0.5*(-(Ioesat*(-2.0*foesat + 
                                  4.0*Ioesat*Rt + 4.0*Ioesat*Rtcvm)) + 
                                  pow(Ioesat*Ioesat*
                                  pow(-2.0*foesat + 4.0*Ioesat*Rt + 
                                  4.0*Ioesat*Rtcvm,2.0) - 
                                  4.0*foesat*Ioesat*Ioesat*
                                  (foesat - 4.0*vxb_l),0.5)) / foesat ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It - Ifpeg <  Ioiorb] It=%f\n", It) ;
#endif
                             if (It >= Ioesat)
                             {
                                Vorb = 0.0 ;
                                Vsat = 0.25*foesat*
                                       pow(It/Ioesat - 1.0,2.0) ;
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It >= Ioesat] Vorb=%f Vsat=%f\n", Vorb, Vsat) ;
#endif
                             }
                             else
                             {
#if DEBUG
if (show_debug & DEBUG_TSS_CURRENT)
   fprintf (debug_out, "[It <  Ioesat] It=%f Vorb=%f Vsat=%f\n", It, Vorb, Vsat) ;
#endif
                                It   = vxb_l/(Rt + Rtcvm) ;
                                Vorb = 0.0 ;
                                Vsat = 0.0 ;
                             }
                          }
                       }

                       itcm_sets  = sa_score = It ;
                       tvmdc_sets = dv_dcore = -1.0 * It * Rtcvm ;
                       v_tether   = It * Rt ;
                       vorb_spree = Vorb ;
                       dcbp_rete  = Vsat ;
                       v_sanity   = dcbp_rete + v_tether - tvmdc_sets -
                                    vorb_spree - vxb_l ;
                       Iega       =        0.0   ;
                       break ;
      case PASSIVE   :
      default        :
                       itcm_sets  =        0.0   ;
                       sa_score   =        0.0   ;
                       tvmdc_sets = -1.0 * vxb_l ;
                       v_tether   =        0.0   ;
                       dv_dcore   = -1.0 * vxb_l ;
                       vorb_spree =        0.0   ;
                       dcbp_rete  =        0.0   ;
                       Rtcvm      = R_TCVM_INF   ;
                       Iega       =        0.0   ;
                       Ifpeg      =        0.0   ;
                       v_sanity   = dcbp_rete + v_tether - tvmdc_sets -
                                    vorb_spree - vxb_l ;
                       break ;
   }

                                   /* Compute tether reel paramters          */
   v_peak_trans =  itcm_sets * 
                   pow ((teth_on_reel - tether_lib_len), 1.04518) / 1.15145 ;

   v_peak_volt   = vxb_l + v_peak_trans ;
   v_peak_volt_n = vxb_l - v_peak_trans ;

   reel_resfreq = pow ((1.2527e6 / (teth_on_reel - tether_lib_len)), 1.35787) ;

                                  /* Compute tether drag/thrust IxB force    */
                                  /* Compute L x B                           */
   V_Cross (&tether_lib_lvlh, &bfield_lvlh, &ixb_lvlh) ;
                                  /* Scale by I / L                          */
   if (fabs(tether_lib_len) > 0.0)
      V_Mult (&ixb_lvlh, itcm_sets / tether_lib_len, &ixb_lvlh) ;
   else
      V_Mult (&ixb_lvlh, 0.0, &ixb_lvlh) ;

                                  /* Compute magnitude of force              */
   ixb_lvlh_mag = V_Mag (&ixb_lvlh) ;
   ixb_mag_tot  = ixb_lvlh_mag * tether_lib_len ;
                                  /* Compute aboumt of power generated       */
   tss_power = itcm_sets * vxb_l ;
}
