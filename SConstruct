
# Sconstruct file, Kamil Cukrowski
env = Environment();
env.Append(CCFLAGS = ' -Wall --std=c++11 -O2 -g -fPIC ');

SetOption('num_jobs', 5)

# add boost support
env.Append(CCFLAGS = '-Iboost');

env.Append(LINKFLAGS = '-fPIC -lboost_python3');
# add python3 support
env.ParseConfig('pkg-config --cflags --libs python3')


## SROUCES! :D
sources = [ Glob('simulation/*.cpp') ];

env['SHLIBPREFIX'] = ''; # anthill.so, default is libanthill.se
anthill = env.SharedLibrary(target = 'anthill', source = sources);
Default(anthill);
