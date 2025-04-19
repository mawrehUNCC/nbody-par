CXX = g++
CXXFLAGS = -std=c++11 -fopenmp -O2
SOURCES = main.cpp nbody.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = nbody

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJECTS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(EXEC)
