# TEMPEST - Tethered Satellite Simulation

TEMPEST is a space tether simulation program designed to model the electrodynamics of tethered satellite systems in Earth orbit. The simulation includes orbital mechanics, electromagnetic fields, plasma physics, atmospheric models, and simple tether dynamics. Recent work has involved updating legacy code to run on modern Ubuntu distributions and running sample simulations.

## Quick Start

To run a complete atmospheric drag simulation with real NRLMSIS data:

### 1. Generate Atmospheric Data
```bash
python3 tools/generate_nrlmsis_data.py
```
This generates real atmospheric density data using the NRLMSIS model for altitudes from 0 km to 1000 km.

### 2. Run Simulation
```bash
tempest -f params/vleo_eccentric.params -o test1.out
```
This runs the orbital simulation with atmospheric drag using the eccentric VLEO orbit parameters.

### 3. Plot Results
```bash
python3 scripts/plot_altitude_vs_time.py test1.out
```
This creates plots showing altitude vs time and atmospheric drag effects.

## Output

The simulation outputs orbital data including:
- Altitude variations
- Atmospheric drag forces
- Orbital decay effects
- Atmospheric model used at each datapoint (NRLMSIS, MSIS-86, or MSISE-90)

## Requirements

- Python 3 with pymsis library for atmospheric data generation
- Matplotlib for plotting (install with: `pip install matplotlib`)
- TEMPEST simulation binary (compiled from source) 

## Updates

The simulation has been modified to run on Ubuntu 24.04.2 and use up-to-date NRLMSIS atmospheric data when `USE_NRLMSIS = Yes` is set in the parameter file. The software initially failed to compile on post-20.04.6 versions of Ubuntu due to issues with variable handling introduced in GCC 10. Much of the original code was written in Fortran, then adapted to C using the f2c translator. Fortran uses COMMON blocks to allow multiple routines to share the same memory block. However, the translator converted these into repeated declarations of global variables across different header files. In past versions of Ubuntu, the GCC (pre-10) defaulted to -fcommon, which allowed multiple definitions of the same global variable across object files, so this was never an issue. However, starting with GCC 10, the default changed to -fno-common, which led to a build error. Adding -fcommon flags to the compilation lines in the Makefile allowed the identical global variable declarations to be merged. The program now compiles and successfully outputs data from test files. 

We developed a sample script using NRLMSIS data to demonstrate the eccentric orbit of a 1-km tether dipping into the VLEO range, with a 150 km perigee and 600 km apogee at 28.5° inclination, 0 RAAN, and 0 argument of perigee, ignoring electrodynamic effects (for now...)

