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

# -- test2 -- #
env_test = env.Clone();
env_test.Append(LINKFLAGS=' -lboost_unit_test_framework ')
env_test.Append(CCFLAGS='--define BOOST_TEST_DYN_LINK')
env_test.VariantDir('_build_test/simulation', 'simulation', duplicate=0)
env_test.VariantDir('_build_test/', './', duplicate=0)
anthill_test = env_test.Program(target = 'test', source = [ Glob('_build_test/simulation/*.cpp'), Glob('tests/*.cpp'), '_build_test/main_tests.cpp' ] );

# -- test using shared library that in the same folder that executable -- #
env_test2 = env.Clone();
env_test2.Append(LINKFLAGS=' -lboost_unit_test_framework -L./ -Wl,-rpath -Wl,' + Dir('#').abspath + ' -lanthill ');
env_test2.Append(CCFLAGS='--define BOOST_TEST_DYN_LINK')
env_test.VariantDir('_build_test2', 'tests', duplicate=0)
anthill_test2 = env_test2.Program(target = 'test2', source = [ Glob('_build_test2/*.cpp'), 'main_tests.cpp' ] );
Depends(anthill_test2, libanthill)




