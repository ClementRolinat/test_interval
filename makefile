SRCS=$(wildcard *.cpp)
BINS=$(SRCS:.cpp=)



CXX = g++
CXXFLAGS = $(shell pkg-config --cflags ibex) 
LIBS	 = $(shell pkg-config --libs  ibex)


ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall
else
CXXFLAGS := $(CXXFLAGS)
endif

ibexring: ibexring.o vibes.o tools.o
	$(CXX) $^ -o ibexring $(CXXFLAGS) $(LIBS)

ibexSLAM: ibexSLAM.o vibes.o tools.o
	$(CXX) $^ -o ibexSLAM $(CXXFLAGS) $(LIBS)

projection: projection.o sepProj.o vibes.o tools.o
	$(CXX) $^ -o projection $(CXXFLAGS) $(LIBS)

vibes.o: vibes.h

tools.o: tools.h

sepProj.o: sepProj.h

%.o: %.c
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(LIBS)

clean:
	rm -rf *.bak rm -rf *.o ibexring ibexSLAM
	
