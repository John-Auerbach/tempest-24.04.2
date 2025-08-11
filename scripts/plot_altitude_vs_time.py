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
    atm_drag_n = data[:, 10]     # atmospheric drag in newtons
    
    print(f"loaded {len(time_hours)} data points")
    print(f"time range: {time_hours[0]:.3f} to {time_hours[-1]:.3f} hours")
    print(f"altitude range: {altitude_km.min():.1f} to {altitude_km.max():.1f} km")
    print(f"drag range: {atm_drag_n.min():.3f} to {atm_drag_n.max():.3f} N")
    
    return time_hours, altitude_km, perigee_km, apogee_km, atm_drag_n

def plot_altitude_vs_time(time_hours, altitude_km, perigee_km, apogee_km, atm_drag_n, output_file):
    
    # create figure with two subplots sharing x-axis
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8), sharex=True)
    
    # top plot: altitude
    ax1.plot(time_hours, altitude_km, 'b-', linewidth=2, label='Satellite Altitude')
    
    # perigee and apogee lines
    ax1.axhline(y=perigee_km[0], color='r', linestyle='--', alpha=0.7, 
                label=f'Perigee: {perigee_km[0]:.0f} km')
    ax1.axhline(y=apogee_km[0], color='g', linestyle='--', alpha=0.7, 
                label=f'Apogee: {apogee_km[0]:.0f} km')
    
    # vleo region shading
    ax1.axhspan(150, 400, alpha=0.1, color='orange', label='VLEO Region (150-400 km)')
    
    ax1.set_ylabel('Altitude (km)', fontsize=12)
    ax1.set_title('Altitude and Drag vs Time', fontsize=14, fontweight='bold')
    ax1.grid(True, alpha=0.3)
    ax1.legend(fontsize=10, loc='upper right')
    
    # y-axis limits with margin for altitude
    y_margin = (altitude_km.max() - altitude_km.min()) * 0.1
    ax1.set_ylim(altitude_km.min() - y_margin, altitude_km.max() + y_margin)
    
    # stats box for altitude
    stats_text = f'''Max Altitude: {altitude_km.max():.1f} km\nMin Altitude: {altitude_km.min():.1f} km'''
    ax1.text(0.02, 0.98, stats_text, transform=ax1.transAxes, 
             verticalalignment='top', bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8),
             fontsize=9, fontfamily='monospace')
    
    # bottom plot: atmospheric drag
    ax2.plot(time_hours, atm_drag_n, 'r-', linewidth=2, label='Atmospheric Drag')
    ax2.set_xlabel('Elapsed Time (hours)', fontsize=12)
    ax2.set_ylabel('Drag Force (N)', fontsize=12)
    ax2.grid(True, alpha=0.3)
    ax2.legend(fontsize=10, loc='upper right')
    
    # save plot
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
time_hours, altitude_km, perigee_km, apogee_km, atm_drag_n = read_tempest_output(input_file)
plot_altitude_vs_time(time_hours, altitude_km, perigee_km, apogee_km, atm_drag_n, output_file)
print(f"done: {output_file}")
