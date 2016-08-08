
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)
CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS)

CC= g++ 
CCFLAGS = -g -fPIC -I include 
VPATH = src include 
SRC = lumi_plane.cxx

.SUFFIXES  : .o .cxx

.cxx.o:
	$(CC) $(CCFLAGS) $(CXXFLAGS) -c  $<


lumi_plane : lumi_plane.o
	
	$(CC) $(CCFLAGS) $(ROOTCFLAGS) $(ROOTGLIBS) \
        -o $@ $^ $(ROOTGLIBS)


clean:
	rm *.o 


all: lumi_plane
