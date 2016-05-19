Anthill
=======

Anthill implements a simulation of a ant colony. It is a project made for ZPR classes.

Dependencies
------------

To build&run the application you need:
- c++ compiler (gcc on linux, msvc2015 for windows)
- scons
- python3 with PyQt5
- boost
- lcov (for profiling and test coverage, works only with gcc)
- google-perftools, libgoogle-perftools-dev for profiling

Project was succesfuly build, run and tested on following platforms:
- Arch linux
- Debian linux
- Windows 7 with msvc2015, python3.4, boost1.60

Build & run
-----------
Executing:
- scons anthill (default) - builds shared library for python
- scons test	- 			builds & runs tests, shows test coverage report
- scons standalone	-	builds standalone version of anthill (for debug)
- scons build_test	-	builds tests, doesn't run them
- ./runProfiler.sh  -   attaches profiler and shows profiler output

To run the project execute file 'main.py' with the python3 interpreter.

Authors
--------

- Kamil Cukrowski  
- Michał Kostrzewa  
- Michał Sypetkowski  
