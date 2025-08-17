#!/usr/bin/env python3
"""
Simple NRLMSIS CSV generator skeleton.
Generates CSV rows: time_iso,lat,lon,alt_m,he,o,n2,o2,ar,h,n,mass,texo,talt

This script uses the nrlmsis2 library if available; otherwise it writes a stub.
"""
import argparse
from datetime import datetime, timedelta
import csv

try:
    import nrlmsis2
    HAVE_NRL = True
except Exception:
    HAVE_NRL = False


def generate(args):
    start = datetime.fromisoformat(args.start)
    end = datetime.fromisoformat(args.end)
    dt = int(args.dt)
    alt_list = [float(x) for x in args.alts.split(',')]

    with open(args.output,'w',newline='') as f:
        w = csv.writer(f)
        w.writerow(['# time_iso','lat_deg','lon_deg','alt_m','he_#/m3','o_#/m3','n2_#/m3','o2_#/m3','ar_#/m3','h_#/m3','n_#/m3','mass_kg_m3','texo_k','talt_k'])
        t = start
        while t <= end:
            for alt in alt_list:
                if HAVE_NRL:
                    # Call into nrlmsis2 - this is a placeholder; users must adapt to api
                    out = nrlmsis2.run(t, args.lat, args.lon, alt/1000.0, f107=args.f107, ap=args.ap)
                    # out should provide species number densities and temps; adapt as needed
                    he = out.get('he',0.0)
                    o  = out.get('o',0.0)
                    n2 = out.get('n2',0.0)
                    o2 = out.get('o2',0.0)
                    ar = out.get('ar',0.0)
                    h_ = out.get('h',0.0)
                    n_ = out.get('n',0.0)
                    mass = out.get('mass',0.0)
                    texo = out.get('texo',0.0)
                    talt = out.get('talt',0.0)
                else:
                    # stub/demo values
                    he = 1e5
                    o  = 1e10
                    n2 = 1e11
                    o2 = 1e10
                    ar = 1e9
                    h_ = 1e7
                    n_ = 1e8
                    mass = 1e-12
                    texo = 800.0
                    talt = 250.0
                w.writerow([t.strftime('%Y-%m-%dT%H:%M:%S'), args.lat, args.lon, alt, he, o, n2, o2, ar, h_, n_, mass, texo, talt])
            t += timedelta(seconds=dt)


if __name__ == '__main__':
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
    generate(args)
