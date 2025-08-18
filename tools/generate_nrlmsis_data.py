#!/usr/bin/env python3
"""
NRLMSIS CSV generator using pymsis library.
Generates CSV rows: time_iso,lat,lon,alt_m,he,o,n2,o2,ar,h,n,mass,texo,talt

This script uses the pymsis library to generate atmospheric data. configure parameters below.
"""
import argparse
from datetime import datetime, timedelta
import csv
import numpy as np

try:
    import pymsis
    HAVE_PYMSIS = True
    print("pymsis library loaded successfully")
except ImportError:
    HAVE_PYMSIS = False
    print("ERROR: pymsis library is not installed!")
    print("Install it with: pip install pymsis")
    print("Cannot generate atmospheric data without real NRLMSIS library.")
except Exception as e:
    HAVE_PYMSIS = False
    print(f"ERROR: Failed to import pymsis: {e}")
    print("Cannot generate atmospheric data without real NRLMSIS library.")

# Generate more altitudes with higher resolution
import numpy as np
alt_range_km = np.linspace(0, 1000, 500)  # 1000 altitude points
alt_range_m = alt_range_km * 1000  # Convert to meters
alt_string = ','.join([f'{int(alt)}' for alt in alt_range_m])

# Configuration parameters - modify these as needed
DEFAULT_CONFIG = {
    'start': '2025-08-17T00:00:00',
    'end': '2025-08-18T00:00:00',  # 24 hours
    'dt': '1800',  # 30 minute time step
    'lat': 0.0,  # latitude in degrees
    'lon': 0.0,  # longitude in degrees
    'alts': alt_string,  # 1000 altitudes from 100km to 1000km
    'f107': 150.0,  # solar flux index
    'ap': 4.0,  # geomagnetic index
    'output': 'nrlmsis_output.csv'
}


def run_with_custom_config(**kwargs):
    """
    Run the generator with custom configuration parameters.
    
    Example usage:
        run_with_custom_config(
            start='2025-08-17T00:00:00',
            end='2025-08-17T12:00:00',
            lat=45.0,
            lon=-75.0,
            alts='300000,400000,500000',
            output='custom_output.csv'
        )
    """
    config_dict = DEFAULT_CONFIG.copy()
    config_dict.update(kwargs)
    
    class ConfigObject:
        def __init__(self, config_dict):
            for k, v in config_dict.items():
                setattr(self, k, v)
    
    config = ConfigObject(config_dict)
    generate(config)
    return config.output


def generate(config=None):
    """Generate NRLMSIS data using either provided config or command line args"""
    
    # Fail if NRLMSIS library is not available
    if not HAVE_PYMSIS:
        raise RuntimeError(
            "pymsis library not available! Install the library with: pip install pymsis\n"
        )
    
    if config is None:
        # If no config provided, use command line arguments (backward compatibility)
        import sys
        if len(sys.argv) > 1:
            # Command line mode
            p = argparse.ArgumentParser()
            p.add_argument('--start', required=True, help='ISO start time e.g. 2025-08-17T00:00:00')
            p.add_argument('--end', required=True, help='ISO end time')
            p.add_argument('--dt', default='60', help='time step seconds')
            p.add_argument('--lat', type=float, default=0.0)
            p.add_argument('--lon', type=float, default=0.0)
            p.add_argument('--alts', default='400000', help='comma separated altitudes in meters')
            p.add_argument('--f107', type=float, default=150.0)
            p.add_argument('--ap', type=float, default=4.0)
            p.add_argument('--output', required=True)
            args = p.parse_args()
            config = args
        else:
            # No command line args, use default config
            class ConfigObject:
                def __init__(self, config_dict):
                    for k, v in config_dict.items():
                        setattr(self, k, v)
            config = ConfigObject(DEFAULT_CONFIG)
    
    start = datetime.fromisoformat(config.start)
    end = datetime.fromisoformat(config.end)
    dt = int(config.dt)
    alt_list = [float(x) for x in config.alts.split(',')]
    alt_km_list = [alt/1000.0 for alt in alt_list]  # Convert to km for pymsis

    # Construct output path - handle both running from tools/ and tempest/ directories
    import os
    current_dir = os.getcwd()
    
    if current_dir.endswith('tools'):
        # Running from tools directory
        output_path = f"../data/{config.output}"
    elif 'tempest' in current_dir:
        # Running from tempest directory or subdirectory
        output_path = f"data/{config.output}"
    else:
        # Fallback - use absolute path
        output_path = f"/home/scien/tempest/data/{config.output}"
    
    print(f"Writing to: {os.path.abspath(output_path)}")
    
    # Create data directory if it doesn't exist
    os.makedirs(os.path.dirname(os.path.abspath(output_path)), exist_ok=True)
    
    with open(output_path,'w',newline='') as f:
        w = csv.writer(f)
        w.writerow(['# time_iso','lat_deg','lon_deg','alt_m','he_#/m3','o_#/m3','n2_#/m3','o2_#/m3','ar_#/m3','h_#/m3','n_#/m3','mass_kg_m3','texo_k','talt_k'])
        
        t = start
        while t <= end:
            # Convert datetime to numpy datetime64 for pymsis
            time_np = np.array([t], dtype='datetime64')
            
            # Calculate atmospheric data for all altitudes at once
            atmosphere = pymsis.calculate(
                time_np, 
                [config.lon], 
                [config.lat], 
                alt_km_list
            )
            
            # Extract data for each altitude
            for i, alt in enumerate(alt_list):
                # Get species number densities (convert from m^-3 to match expected units)
                he = atmosphere[0, 0, 0, i, pymsis.Variable.HE]  # He number density
                o  = atmosphere[0, 0, 0, i, pymsis.Variable.O]   # O number density
                n2 = atmosphere[0, 0, 0, i, pymsis.Variable.N2]  # N2 number density
                o2 = atmosphere[0, 0, 0, i, pymsis.Variable.O2]  # O2 number density
                ar = atmosphere[0, 0, 0, i, pymsis.Variable.AR]  # Ar number density
                h_ = atmosphere[0, 0, 0, i, pymsis.Variable.H]   # H number density
                n_ = atmosphere[0, 0, 0, i, pymsis.Variable.N]   # N number density
                
                # Get mass density and temperature
                mass = atmosphere[0, 0, 0, i, pymsis.Variable.MASS_DENSITY]  # kg/m^3
                texo = atmosphere[0, 0, 0, i, pymsis.Variable.TEMPERATURE]  # K (use same temp for both)
                talt = atmosphere[0, 0, 0, i, pymsis.Variable.TEMPERATURE]  # K
                
                w.writerow([
                    t.strftime('%Y-%m-%dT%H:%M:%S'), 
                    config.lat, 
                    config.lon, 
                    alt, 
                    he, o, n2, o2, ar, h_, n_, 
                    mass, texo, talt
                ])
            
            t += timedelta(seconds=dt)
    
    print(f"Generated NRLMSIS atmospheric data with {len(alt_list)} altitudes")


if __name__ == '__main__':
    # Run with default configuration (no command line arguments needed)
    generate()
    print(f"NRLMSIS data generated successfully! Output file: {DEFAULT_CONFIG['output']}")
