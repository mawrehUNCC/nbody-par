# parallel/Makefile
CXX = g++
CXXFLAGS = -O3 -fopenmp -std=c++17

all: nbody_parallel

nbody_parallel: nbody_parallel.cpp
	$(CXX) $(CXXFLAGS) -o nbody_parallel nbody_parallel.cpp

clean:
	rm -f nbody_parallel
