#!/bin/bash

# requires google-perftools, libgoogle-perftools-dev

LD_PRELOAD=/usr/lib/libprofiler.so.0 CPUPROFILE=/tmp/prof.out python3 main.py
echo "web" | google-pprof /usr/bin/python3 /tmp/prof.out #--svg  > pprof.svg
