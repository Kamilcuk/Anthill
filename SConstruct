#!/bin/python3
# Sconstruct file, Kamil Cukrowski

# !	scons anthill (default) builds shared library for python
# !	scons test				builds & runs tests
# 	scons standalone		builds standalone version of anthill (for debug)
# 	scons build_test		builds tests, doesn't run them

# -- init -- #
SetOption('num_jobs', 3)
sources = [ Glob('simulation/*.cpp') ];

# -- Environment() setup -- #
env = Environment();
if env['PLATFORM'] == 'win32':
	env.Append(CCFLAGS = " /D BOOST_PYTHON_STATIC_LIB /O2 /D_USRDLL /D_WINDLL /DOTHER_DEFINES /LD /EHsc ");
	env.Append(LINKFLAGS = " /DLL ");
	# boost and python paths need to be customized depending on user installation
	env.Append(CCFLAGS = "         /I D:/boost_1_60_0/                /I D:/python34/include ");
	env.Append(LINKFLAGS = " /LIBPATH:D:/boost_1_60_0/stage/lib /LIBPATH:D:/Python34/libs     ");
	# we want anthill.pyd, not anthill.lib, we want to link it with python.
	env['SHLIBSUFFIX'] = '.pyd' ; # we want anthill.so, default is libanthill.se
else:
	# linux platform
	env.Append(CCFLAGS = ' -Werror -Wall --std=c++11 -O2 -g -fPIC ');
	env.Append(CCFLAGS = '-Iboost'); # add boost support
	env.Append(LINKFLAGS = '-fPIC -lboost_serialization'); # compile shared # boost serialization
	# add python3 support
	env.ParseConfig('pkg-config --cflags --libs python3')
	# strange library name on debian
	import os.path
	if  os.path.exists('/etc/debian_version'):
		env.Append(LINKFLAGS = '-lboost_python-py34');
	else:
		env.Append(LINKFLAGS = '-lboost_python3');
	env['SHLIBPREFIX'] = ''; # we want anthill.so, default is libanthill.se

# -- shared library for python linking -- #
libanthill = env.SharedLibrary(target = 'anthill', source = sources);
Default(libanthill)

# -- anthill standalone -- #
anthill_standalone = env.Program(target = 'standalone', source = [ sources, 'main_standalone.cpp' ] );

# -- test using shared library that in the same folder that executable -- #
env_test = env.Clone();
env_test.Append(LINKFLAGS=' -lboost_unit_test_framework -L./ -Wl,-rpath -Wl,' + Dir('#').abspath + ' -lanthill ');
env_test.Append(CCFLAGS='--define BOOST_TEST_DYN_LINK')
env_test.VariantDir('_build_test', 'tests', duplicate=0)
anthill_test = env_test.Program(target = 'build_test', source = [ Glob('_build_test/*.cpp'), 'main_tests.cpp' ] );
Depends(anthill_test, libanthill)

# -- build and run tests -- #
import os
def run_tests(target, source, env):
	print "\n\n\t\t\t----- TESTING ----- \n";
	os.system("./build_test --log_level=message --show_progress=yes --report_level=short")
	return None
anthill_test_run = env_test.Command(target = 'test', source = "./build_test", action = run_tests );
Depends(anthill_test_run, anthill_test)

