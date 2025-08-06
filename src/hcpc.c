// ===========================================================================
//	HCPC.c
// ===========================================================================
//	©2007 Tethers Unlimited Inc. 
//	©2008 Nestor Voronka
//			
//	Routines for modeling electron emission and collection by HCPC, model
//	originally from SAIC (aka Maxwell) Electronics Workbench, as interpreted
//	by Keith Fuhrhop (U.Mich)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "advmath.h"

#include "types.h"
#include "gmt.h"
#include "tempest.h"
#include "temputil.h"

#include	"hcpc.h"

#pragma mark ------Local Defines-------

#define		d_PI				3.1415926535

#define		d_electron_charge		1.6e-19		// coulombs
#define		d_permittivity_of_free_space	8.85e-12	// F/m
#define		d_me				9.11e-31	// kg		electron mass
#define		d_ProtonMass			1.67e-27	// kg		proton mass
#define		d_e_me				1.7563e11	// C/kg
#define		d_e_mi				5.998e6		// C/kg   assume it's mostly oxygen atoms

#define 	d_Vanode			26.5		// Hollow Cathode Anode Potential {V]
#define 	d_HC_OrificeEmittedIonCurrent	0.12774		// what Fuhrhop used, the percentage of the input neutral xenon is being ionized
#define		d_HC_OrificeDensity		2e20		// m^-3
#define		d_HC_OrificeRadius		1.375e-3	// m
#define		d_KeeperDiameter		4.675e-3	// m
#define		d_KeeperThickness		2.4e-4		// m
#define		d_KeeperDistance		7.4e-4		// m
#define		d_XenonMolecularWeight		131.29


#define MAX_BISECTIONS		100
#define SOLVER_ACCURACY_PERCENT	1.0

double f_HCPC_Voltage(double	num_Current,	//Hollow Cathode Anode Potential wrt. Plasma [V] as a function of emitted current
					  double	num_IonMass,
					  double	num_B,			//Magnetic Flux Density [T]
					  double	num_Te,			//Ambient Plasma Electron temparature [eV]
					  double	num_ne,			//Ambient Plasma Density [particles/m^3] : quasineutral
					  double	num_FlowRate_sccm,	//Gas FLow num_FlowRate_sccm [sccm]
					  double	num_DoubleLayerPotentialDrop,//Double layer potential drop [V] Fuhrhop uses 4V
					  double	num_Source_Te,	//Source Electron Temperature [eV]  Fuhrhop uses 3.889
                                          double	min_Voltage,
                                          double	max_Voltage
					  )			
{
   double	solver_accuracy ;
   double	func_out, func_mid ;
   double	delta_V, V_mid, bisect_V ;
   int		iteration ;

#if DEBUG
if (show_debug & DEBUG_HCPC)
{
   fprintf (debug_out, "HCPC V(I=%f) ionmass=%f B=%e Te=%f Ne=%e sccm=%f Vdl=%f HC_Te=%f\n",
                        num_Current, num_IonMass, num_B, num_Te, num_ne, num_FlowRate_sccm,
                        num_DoubleLayerPotentialDrop, num_Source_Te) ;
}
#endif


   solver_accuracy = SOLVER_ACCURACY_PERCENT * 0.01 * num_Source_Te ;

   func_out = f_HCPC_Current (min_Voltage, num_IonMass, num_B, num_Te, num_ne, num_FlowRate_sccm, num_DoubleLayerPotentialDrop, num_Source_Te) - num_Current ;
   func_mid = f_HCPC_Current (max_Voltage, num_IonMass, num_B, num_Te, num_ne, num_FlowRate_sccm, num_DoubleLayerPotentialDrop, num_Source_Te) - num_Current ;

#if DEBUG
if (show_debug & DEBUG_HCPC)
{
   fprintf (debug_out, "HCPC I(Vmin) = %f (%f)\n", func_out+num_Current, min_Voltage) ;
   fprintf (debug_out, "HCPC I(Vmax) = %f (%f)\n", func_mid+num_Current, max_Voltage) ;
}
#endif

   if (func_out * func_mid >= 0.0) 
   {
      fprintf (stderr, "ERROR: @ %s, HCPC V of I solver has inadequate bounds [%f, %f] for convergence!\n",
                        Str_FromGMT (&curr_gmt), min_Voltage, max_Voltage) ;
      fprintf (stderr, "HCPC V(I=%f) ionmass=%f B=%e Te=%f Ne=%e sccm=%f Vdl=%f HC_Te=%f\n",
                        num_Current, num_IonMass, num_B, num_Te, num_ne, num_FlowRate_sccm,
                        num_DoubleLayerPotentialDrop, num_Source_Te) ;
      fprintf (stderr, "HCPC I(Vmin) = %f (%f)\n", func_out+num_Current, min_Voltage) ;
      fprintf (stderr, "HCPC I(Vmax) = %f (%f)\n", func_mid+num_Current, max_Voltage) ;
#if DEBUG
if (show_debug & DEBUG_HCPC)
{
      fprintf (debug_out, "ERROR: @ %s, HCPC V of I solver has inadequate bounds [%f, %f] for convergence!\n",
                        Str_FromGMT (&curr_gmt), min_Voltage, max_Voltage) ;
}
#endif
      return (-99999.0) ;
   }

   if (func_out < 0.0)
   {
     bisect_V = min_Voltage ;
     delta_V  = max_Voltage - min_Voltage ;
   }
   else
   {
     bisect_V = max_Voltage ;
     delta_V  = min_Voltage - max_Voltage ;
   }


   for (iteration = 0 ; iteration <= MAX_BISECTIONS ; iteration++)
   {
      delta_V *= 0.5 ;
      V_mid    = bisect_V + delta_V ;

      func_mid = f_HCPC_Current (V_mid, num_IonMass, num_B, num_Te, num_ne, num_FlowRate_sccm, num_DoubleLayerPotentialDrop, num_Source_Te) - num_Current ;

      if (func_mid <= 0.0)
         bisect_V = V_mid ;

      if ((fabs (delta_V) < solver_accuracy) || (func_mid = 0.0))
      {
#if DEBUG
if (show_debug & DEBUG_HCPC)
{
   fprintf (debug_out, "HCPC V(I) = %fV = V(%fI)\n", bisect_V, num_Current) ;
   fprintf (debug_out, "HCPC V of I solver needed %d iterations\n", iteration) ;
}
#endif
         return (bisect_V) ;
      }
   }

   fprintf (stderr, "ERROR: @ %s, HCPC V of I solver has inadequate bounds [%f, %f] for convergence!\n",
                        Str_FromGMT (&curr_gmt), min_Voltage, max_Voltage) ;
   fprintf (stderr, "BAD: HCPC solver used %d iterations (error=%f)and needed more!!\n",
            iteration, delta_V) ;
   fprintf (stderr, "HCPC I(Vmin) = %f (%f)\n", func_out+num_Current, min_Voltage) ;
   fprintf (stderr, "HCPC I(Vmax) = %f (%f)\n", func_mid+num_Current, max_Voltage) ;
#if DEBUG
if (show_debug & DEBUG_HCPC)
{
   fprintf (debug_out, "BAD: HCPC solver used %d iterations (error=%f)and needed more!!\n",
            iteration, delta_V) ;
}
#endif
   return (-99999.0) ;
}

//*****************************************************************
// Function definitions
//*****************************************************************

double f_HCPC_Current(double	num_Voltage,	//Hollow Cathode Anode Potential wrt. Plasma [V]
					  double	num_IonMass,
					  double	num_B,			//Magnetic Flux Density [T]
					  double	num_Te,			//Ambient Plasma Electron temparature [eV]
					  double	num_ne,			//Ambient Plasma Density [particles/m^3] : quasineutral
					  double	num_FlowRate_sccm,	//Gas FLow num_FlowRate_sccm [sccm]
					  double	num_DoubleLayerPotentialDrop,//Double layer potential drop [V] Fuhrhop uses 4V
					  double	num_Source_Te	//Source Electron Temperature [eV]  Fuhrhop uses 3.889
					  )			
{	
		
	//Ambient Constants
	double	
		alpha = 0.1,                        //Scattering coefficient
		ionMass, 
		cathodeOrificeArea,orificeEmittedCurrent,maxEscapingCurrent,
		orificeCurrent,
		f, 
		Iepc, 
		ionDensity,delphimag,rmag,Vmag,
		BohmCorrectedIonEnergy, 
		totalEmittedCurrent,
		ionVelocity,
		cyclotronFrequency,
		nemag,
		Rdl,delxcl,Vcl,delphi,Idlamb,Ieamb,Iambmin,
		jthi, je, Imagamb;
				
#if DEBUG
if (show_debug & DEBUG_HCPC)
{
   fprintf (debug_out, "HCPC I(V=%f) ionmass=%f B=%e Te=%f Ne=%e sccm=%f Vdl=%f HC_Te=%f\n",
                        num_Voltage, num_IonMass, num_B, num_Te, num_ne, num_FlowRate_sccm,
                        num_DoubleLayerPotentialDrop, num_Source_Te) ;
}
#endif
		
	//Calculations
	
	//Neutral Molecular Gas Flow [Amps] - for just Xenon?
	orificeCurrent= 0.07165 * num_FlowRate_sccm;
	
	//Cross sectional Area of Hollow Cathode Orifice [m^2]
	cathodeOrificeArea = d_PI * d_HC_OrificeRadius*d_HC_OrificeRadius;
	
	//ionMass is the ion mass [kg]
	ionMass = d_XenonMolecularWeight * d_ProtonMass;
	
	//Hollow Cathode Orifice Ion Thermal Current Density [A/m^2]
	 jthi = d_electron_charge*d_HC_OrificeDensity * sqrt(d_electron_charge*num_Source_Te/(2*d_PI*ionMass));
	
	//Hollow Cathode Orifice Emitted Ion Current [A]
	if(jthi*cathodeOrificeArea<orificeCurrent)
		orificeEmittedCurrent = jthi*cathodeOrificeArea;
	else
		orificeEmittedCurrent=orificeCurrent;
	
	f = 1;
	
	//The maximum escaping electron current
	maxEscapingCurrent = f * sqrt(ionMass/(2*d_PI*d_me)) * orificeEmittedCurrent;
		
	//If the anode potential is positive with respect to the plasma the current
	//is reduced by an exponential of the potential.
	if ((d_Vanode + num_Voltage) < 0)
	{		Iepc = maxEscapingCurrent;
	}
	else
	{		Iepc = maxEscapingCurrent * exp(-(num_Voltage + d_Vanode) / num_Source_Te);
	}
	
			
	//Bohm Corrected Ion Energy [eV]
	BohmCorrectedIonEnergy = num_Source_Te/2;
	
	//Ion Velocity Near the Plasma Contactor [m/s]
	ionVelocity = sqrt(2*d_electron_charge * BohmCorrectedIonEnergy/ionMass);
	
	//Ion density near the Plasma Contactor [particles / m^3]
	ionDensity = orificeEmittedCurrent/(2 * d_PI * d_electron_charge * ionVelocity * pow(d_KeeperDistance,2));
	
	//Cyclotron Frequency [rad/s]
	cyclotronFrequency = d_electron_charge*num_B/d_me;
	
	//Density at which Electrons can be Scattered Across the Magnetic Field
	//Lines - Scattering Frequrncy = Cyclotron Frequency [particles / m^3]
	nemag = (d_permittivity_of_free_space*d_me
			 /(d_electron_charge*d_electron_charge) * pow(cyclotronFrequency/alpha,2));
	
	//Potential Drop across the above Scattering Density (Barometric law) - 
	//assume ambient electrons do not contribute much at the above density [V]
	delphimag = num_Source_Te*log(ionDensity/nemag);
	
	//The radius at which the Ion Density reaches the above Scattering Density [m]
	rmag = sqrt(orificeEmittedCurrent/(2*d_PI*d_electron_charge*nemag*ionVelocity*sqrt(1+(delphimag/BohmCorrectedIonEnergy))));
	
	//Extra Voltage at the Magnetic Radius [V]
	Vmag = 0.0;
	if (d_Vanode + num_Voltage - delphimag > 0.0)
	{Vmag =(d_Vanode + num_Voltage - delphimag);
	}
	
	
	//Ambient Electron Current Density [A/m^2]
	je = d_electron_charge*num_ne*sqrt(d_electron_charge*num_Te/(2*d_PI*d_me));
	
	//Magnetic Limited Current Collected [A]
	if(d_Vanode+ num_Voltage -delphimag < 0)
	{	Imagamb = (je*2*d_PI*rmag*rmag*(1+sqrt(8*d_electron_charge*Vmag/(d_me*pow(cyclotronFrequency*rmag,2)))) 
				   * exp(d_Vanode+ num_Voltage -delphimag));
	}else
	{
		Imagamb = (je*2*d_PI*rmag*rmag*(1+sqrt(8*d_electron_charge*Vmag/(d_me*pow(cyclotronFrequency*rmag,2)))) );

	}
	//----------------------
	
	//Potential Difference for a Double Sheath Layer with a Potential Drop
		//Equivalent to the Source Electron Temperature. [V]
		delphi = num_Source_Te*log((ionDensity/num_ne)*sqrt(num_Te/num_Source_Te));
	
	//Current of Double Layer from Electron Collection of Ambient Plasma [A]
	Idlamb = orificeEmittedCurrent*(sqrt(ionMass/d_me)
									* 0.5*sqrt(num_DoubleLayerPotentialDrop/delphi));
	
	//Double layer radius [m]
	Rdl = sqrt(Idlamb/(2*d_PI*je));
	
	//Potential used in Child Langmuir estimate [V]
	Vcl = 0;
	if ((d_Vanode+num_Voltage-delphi)>0)
		Vcl = d_Vanode+num_Voltage-delphi;
	
	//Child Layer estimate of double layer thickness [m]
	delxcl = 1.255 * sqrt(d_permittivity_of_free_space) 
		* pow((d_electron_charge/(2*d_PI*d_me)),0.25) 
		* pow(Vcl,0.75) * sqrt(1/je);
	
	//Current Due to the Flow of Electrons from the Ambient Plasma [A]
	if( d_Vanode+num_Voltage-delphi < 0)
		Ieamb = Idlamb*(1+2*delxcl/Rdl)	*exp((d_Vanode+num_Voltage-delphi)/num_Te);
	else
		Ieamb = Idlamb*(1+2*delxcl/Rdl);
	
	// Note: The Ieamb is heavily dependant upon the Distance from orifice exit to beginning of keeper
	//------------------------
	
	//Find the min between Ieamb and Imagamb at every point so it can be plotted
	if(Ieamb<Imagamb)
	{	Iambmin = Ieamb;
	}else
	{
		Iambmin = Imagamb;
	}
	
	//The Total Current th The Plasma Contactor [A]
	//Negative because electrons being emitted is negative current.
	totalEmittedCurrent = -(Iepc-orificeEmittedCurrent-Iambmin);
		
#if DEBUG
if (show_debug & DEBUG_HCPC)
{
   fprintf (debug_out, "HCPC I(V) = %fA = I(%fV)\n", totalEmittedCurrent, num_Voltage) ;
}
#endif
	return(totalEmittedCurrent);
}
