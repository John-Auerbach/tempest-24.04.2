NRLMSIS datafile generator

This repository ships a small helper script `tools/gen_nrlmsis_table.py` that
can generate CSV datafiles usable by TEMPEST when `USE_NRLMSIS` is enabled.

Usage example (generates a small stub file):

python3 tools/gen_nrlmsis_table.py --start 2025-08-17T00:00:00 --end 2025-08-17T00:10:00 --dt 60 --lat 0 --lon 0 --alts 400000 --output /tmp/nrl.csv

Notes:
- If `nrlmsis2` is installed the script will attempt to call it; otherwise it
  writes synthetic placeholder values. Users should adapt the call to their
  installed NRLMSIS wrapper API.
- The CSV columns are: time_iso,lat_deg,lon_deg,alt_m,he,o,n2,o2,ar,h,n,mass,texo,talt
- Set `NRLMSIS_DATAFILE` in your params file to point to the generated CSV and
  set `USE_NRLMSIS = Yes`.
