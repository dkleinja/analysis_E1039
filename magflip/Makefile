
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)
CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS)

CC= g++ 
CCFLAGS = -g -fPIC -I include 
VPATH = src include 
SRC = getphi_dpsim.C

.SUFFIXES  : .o .C

.C.o:
	$(CC) $(CCFLAGS) $(CXXFLAGS) -c  $<


getphi_dpsim : getphi_dpsim.o
	
	$(CC) $(CCFLAGS) $(ROOTCFLAGS) $(ROOTGLIBS) \
        -o $@ $^ $(ROOTGLIBS)


clean:
	rm *.o 


all: getphi_dpsim
