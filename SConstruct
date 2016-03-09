# Sconstruct file, Kamil Cukrowski
env = Environment();
env.Append(CCFLAGS = '-Wall --std=c++11 -O2 -g');

## SROUCES! :D
sources = [ Glob('*.cpp') ];

anthill = env.Program(target = 'anthill', source = sources);
Default(anthill);
