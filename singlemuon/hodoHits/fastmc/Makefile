
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS)

#get individual libs


CC= g++ 
CCFLAGS = -g -fPIC -I include 
#VPATH = src include 
SRC = getHodoHits.C  

.SUFFIXES  : .o .C

.C.o:
	$(CC) $(CCFLAGS) $(CXXFLAGS) -c  $<


getHodoHits : getHodoHits.o
	
	$(CC) $(CCFLAGS) $(ROOTCFLAGS) $(ROOTGLIBS) \
	-o $@ $^ $(ROOTGLIBS) 

clean:
	rm *.o 


all: getHodoHits
