all:
	g++ -shared -fPIC -Iboost -I/usr/include/python3.5m -std=c++11 -lpython3.5m -lboost_python3 simulation/*.cpp -o anthill.so
