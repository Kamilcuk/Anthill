
# Sconstruct file, Kamil Cukrowski
env = Environment();
env.Append(CCFLAGS = ' -Wall --std=c++11 -O2 -g ');
env.Append(CCFLAGS = '-I/usr/include/python3.5m -Iboost');
env.Append(LINKFLAGS = '-fPIC -lpython3.5m -lboost_python3 ');

## SROUCES! :D
sources = [ Glob('simulation/*.cpp') ];

env['SHLIBPREFIX'] = ''; # anthill.so, default is libanthill.se
anthill = env.SharedLibrary(target = 'anthill', source = sources);
Default(anthill);
