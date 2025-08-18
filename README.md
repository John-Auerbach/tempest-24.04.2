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

## Configuration

The simulation has been modified to use up-to-date NRLMSIS atmospheric data when `USE_NRLMSIS = Yes` is set in the parameter file. 

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

