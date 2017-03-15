
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

KTRACKERCFLAGS = -I$(KTRACKER_ROOT)
#KTRACKERLIBS =  -L$(KTRACKER_LIB) -lktracker
KTRACKERLIBS =  -L$(KTRACKER_LIB)

CXXFLAGS     += $(ROOTCFLAGS)
CXXFLAGS     += $(KTRACKERCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS)

#get individual libs
KTRACK       = $(KTRACKER_LIB)/libkTracker.so
GEOMSVC       = $(KTRACKER_ROOT)/GeomSvc.o
SRAWEVT       = $(KTRACKER_ROOT)/SRawEvent.o	
SRECEVT       = $(KTRACKER_ROOT)/SRecEvent.o
FASTTRCK      = $(KTRACKER_ROOT)/FastTracklet.o
ALLFILES      = $(KTRACKER_ROOT)/GeomSvc.o $(KTRACKER_ROOT)/SRawEvent.o $(KTRACKER_ROOT)/SRecEvent.o $(KTRACKER_ROOT)/FastTracklet.o


CC= g++ 
CCFLAGS = -g -fPIC -I include 
#VPATH = src include 
SRC = getHodoHits.C

.SUFFIXES  : .o .C

.C.o:
	$(CC) $(CCFLAGS) $(CXXFLAGS) -c  $<


getHodoHits : getHodoHits.o
	
	$(CC) $(CCFLAGS) $(ROOTCFLAGS) $(KTRACKERCFLAGS) $(ROOTGLIBS) \
	-o $@ $^ $(KTRACK) $(ROOTGLIBS) 

clean:
	rm *.o 


all: getHodoHits
