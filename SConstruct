#!/bin/python3
# Sconstruct file, Kamil Cukrowski

# -- init -- #
SetOption('num_jobs', 8)
sources = [ Glob('simulation/*.cpp') ];

# -- env -- #
env = Environment();
env.Append(CCFLAGS = ' -Wall --std=c++11 -O2 -g -fPIC ');
env.Append(CCFLAGS = '-Iboost'); # add boost support
env.Append(LINKFLAGS = '-fPIC'); # compile shared

# add python3 support
env.ParseConfig('pkg-config --cflags --libs python3')
# strange library name on debian
import os.path
if  os.path.exists('/etc/debian_version'):
	env.Append(LINKFLAGS = '-lboost_python-py34');
else:
	env.Append(LINKFLAGS = '-lboost_python3');

env['SHLIBPREFIX'] = ''; # we want anthill.so, default is libanthill.se

# -- default target - shared library for python linking -- #
libanthill = env.SharedLibrary(target = 'anthill', source = sources);
Default(libanthill);
# -- anthill standalone -- #
anthill = env.Program(target = 'anthill_standalone', source = [ sources, 'main_standalone.cpp' ] );

# -- test ! -- #
env_test = Environment();
env_test.Append(CCFLAGS = env['CCFLAGS'], LINKFLAGS = env['LINKFLAGS']);
env_test.Append(LINKFLAGS=' -lboost_unit_test_framework -L./ -lanthill ');
env_test.Append(CCFLAGS='--define BOOST_TEST_DYN_LINK')
anthill_test = env_test.Program(target = 'test', source = [ Glob('tests/*.cpp'), 'main_tests.cpp' ] );
Depends(anthill_test, libanthill)

