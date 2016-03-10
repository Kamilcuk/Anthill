#!/bin/python3
# Sconstruct file, Kamil Cukrowski
env = Environment();
env.Append(CCFLAGS = ' -Wall -Werror --std=c++11 -O2 -g -fPIC ');

SetOption('num_jobs', 5)

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
anthill = env.SharedLibrary(target = 'anthill', source = sources);
Default(anthill);
