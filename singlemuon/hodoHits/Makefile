
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

DPSIMCFLAGS = -I$(DPSIM_ROOT)/inc
#DPSIMLIBS =  -L$(DPSIM_LIB) -lktracker
DPSIMLIBS =  -L$(DPSIM_ROOT)/build/lib

CXXFLAGS     += $(ROOTCFLAGS)
CXXFLAGS     += $(DPSIMCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS)

#get individual libs
DPSIM       = $(DPSIM_ROOT)/build/lib/libDPMCRawEvent.so
GEOMSVC       = $(DPSIM_ROOT)/GeomSvc.o
SRAWEVT       = $(DPSIM_ROOT)/SRawEvent.o	
SRECEVT       = $(DPSIM_ROOT)/SRecEvent.o
FASTTRCK      = $(DPSIM_ROOT)/FastTracklet.o
ALLFILES      = $(DPSIM_ROOT)/GeomSvc.o $(DPSIM_ROOT)/SRawEvent.o $(DPSIM_ROOT)/SRecEvent.o $(DPSIM_ROOT)/FastTracklet.o


CC= g++ 
CCFLAGS = -g -fPIC -I include 
#VPATH = src include 
SRC = getHodoHits_trackID.C 

.SUFFIXES  : .o .C

.C.o:
	$(CC) $(CCFLAGS) $(CXXFLAGS) -c  $<


getHodoHits_trackID : getHodoHits_trackID.o
	
	$(CC) $(CCFLAGS) $(ROOTCFLAGS) $(DPSIMCFLAGS) $(ROOTGLIBS) \
	-o $@ $^ $(DPSIM) $(ROOTGLIBS) 

clean:
	rm *.o 


all: getHodoHits_trackID
