#!/usr/bin/env python3
# tempest vleo altitude plotter

import numpy as np
import matplotlib.pyplot as plt
import sys
import os

def read_tempest_output(filename):
    print(f"reading tempest output from: {filename}")
    
    data = np.loadtxt(filename, comments='#')
    
    # extract columns
    time_hours = data[:, 1]      # hours
    altitude_km = data[:, 2]     # j2 corrected altitude
    perigee_km = data[:, 4]      # perigee
    apogee_km = data[:, 5]       # apogee
    
    print(f"loaded {len(time_hours)} data points")
    print(f"time range: {time_hours[0]:.3f} to {time_hours[-1]:.3f} hours")
    print(f"altitude range: {altitude_km.min():.1f} to {altitude_km.max():.1f} km")
    
    return time_hours, altitude_km, perigee_km, apogee_km

def plot_altitude_vs_time(time_hours, altitude_km, perigee_km, apogee_km, output_file):
    
    plt.figure(figsize=(12, 8))
    plt.plot(time_hours, altitude_km, 'b-', linewidth=2, label='Satellite Altitude')
    
    # lines for perigee and apogee (initial values)
    plt.axhline(y=perigee_km[0], color='r', linestyle='--', alpha=0.7, 
                label=f'Perigee: {perigee_km[0]:.0f} km')
    plt.axhline(y=apogee_km[0], color='g', linestyle='--', alpha=0.7, 
                label=f'Apogee: {apogee_km[0]:.0f} km')
    
    # highlight VLEO region
    plt.axhspan(150, 400, alpha=0.1, color='orange', label='VLEO Region (150-400 km)')
    
    plt.xlabel('Mission Elapsed Time (hours)', fontsize=12)
    plt.ylabel('Altitude (km)', fontsize=12)
    plt.title('TEMPEST VLEO Eccentric Orbit Simulation\nAltitude vs Time', fontsize=14, fontweight='bold')
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=10)
    
    # y-axis limits with some margin
    y_margin = (altitude_km.max() - altitude_km.min()) * 0.1
    plt.ylim(altitude_km.min() - y_margin, altitude_km.max() + y_margin)
    
    # statistics text box
    stats_text = f'''    Min Altitude: {altitude_km.min():.1f} km
    Max Altitude: {altitude_km.max():.1f} km'''
    
    plt.text(0.02, 0.98, stats_text, transform=plt.gca().transAxes, 
             verticalalignment='top', bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8),
             fontsize=9, fontfamily='monospace')
    
    # Save plot
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Plot saved to: {output_file}")
    
# just run it
input_file = "/home/scien/tempest/vleo_eccentric.out"
output_file = "/home/scien/tempest/scripts/vleo_altitude_plot.png"

# use command line args if provided
if len(sys.argv) > 1:
    input_file = sys.argv[1]
if len(sys.argv) > 2:
    output_file = sys.argv[2]

# check file exists
if not os.path.exists(input_file):
    print(f"error: {input_file} not found!")
    print("run tempest simulation first")
    sys.exit(1)

# read data and make plot
time_hours, altitude_km, perigee_km, apogee_km = read_tempest_output(input_file)
plot_altitude_vs_time(time_hours, altitude_km, perigee_km, apogee_km, output_file)
print(f"done: {output_file}")
