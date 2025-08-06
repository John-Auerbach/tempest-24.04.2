// ===========================================================================
//	HCPC.h			
// ===========================================================================
//	©2007 Tethers Unlimited Inc.
//	@2008 Nestor Voronka
//			
//	Routines for modeling electron emission and collection by HCPC, model
//	originally from SAIC (aka Maxwell) Electronics Workbench

#ifndef	_HCPC__
	#define _HCPC_

double f_HCPC_Current (	double	num_Voltage,	//Hollow Cathode Anode Potential wrt. Plasma [V]
			double	num_IonMass,
			double	num_B,			//Magnetic Flux Density [T]
			double	num_Te,			//Ambient Plasma Electron temparature [eV]
			double	num_ne,			//Ambient Plasma Density [particles/m^3] : quasineutral
			double	num_FlowRate_sccm,	//Gas FLow num_FlowRate_sccm [sccm]
			double	num_DoubleLayerPotentialDrop,//Double layer potential drop [V] Fuhrhop uses 4V
			double	num_Source_Te	//Source Electron Temperature [eV]  Fuhrhop uses 3.889
);
								
double f_HCPC_Voltage (	double	num_Current,    //Hollow Cathode Anode Potential wrt. Plasma [V] as a function of emitted current
			double	num_IonMass,
			double	num_B,			//Magnetic Flux Density [T]
			double	num_Te,			//Ambient Plasma Electron temparature [eV]
			double	num_ne,			//Ambient Plasma Density [particles/m^3] : quasineutral
			double	num_FlowRate_sccm,	//Gas FLow num_FlowRate_sccm [sccm]
			double	num_DoubleLayerPotentialDrop,//Double layer potential drop [V] Fuhrhop uses 4V
			double	num_Source_Te,	//Source Electron Temperature [eV]  Fuhrhop uses 3.889
			double        min_Voltage,
			double        max_Voltage
);

#endif
