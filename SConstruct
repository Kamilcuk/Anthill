#!/bin/python3
# Sconstruct file, Kamil Cukrowski
env = Environment();
env.Append(CCFLAGS = ' -Wall --std=c++11 -O2 -g -fPIC ');
#env.Append(CCFLAGS = ' -w -Wno-deprecated --std=c++11 ');

SetOption('num_jobs', 8)

# add boost support
env.Append(CCFLAGS = '-Iboost');
#add shared support
env.Append(LINKFLAGS = '-fPIC');

# add python3 support
env.ParseConfig('pkg-config --cflags --libs python3')
# strange library name on debian
import os.path
if  os.path.exists('/etc/debian_version'):
	env.Append(LINKFLAGS = '-lboost_python-py34');
else:
	env.Append(LINKFLAGS = '-lboost_python3');


## SROUCES! :D
sources = [ Glob('simulation/*.cpp') ];

env['SHLIBPREFIX'] = ''; # anthill.so, default is libanthill.se
libanthill = env.SharedLibrary(target = 'anthill', source = sources);
Default(libanthill);

anthill = env.Program(target = 'anthill_standalone', source = [ sources, 'main.cpp' ] );
