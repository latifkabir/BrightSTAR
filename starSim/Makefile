# Filename: Makefile
# Description: Makefile for compiling Bright Star Analysis Framework. This Makefile is tailored for STAR farm nodes which is set for 32 bit.
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Wed May 15 01:12:25 2019 (-0500)
# URL: jlab.org/~latif

ifndef STAR
  $(error $$STAR environment variable not defined. source setup.[c]sh first)
endif

ifndef STARHOME
  $(error $$STARHOME environment variable not defined. source setup.[c]sh first)
endif

# ifndef STARSIMHOME
#   $(error $$STARSIMHOME environment variable not defined. source setup.[c]sh first)
# endif


ROOTCINT  = rootcint
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs)
CXXFLAGS  += -fPIC -m32 -std=c++11 -L ${STAR}/.sl73_gcc485/lib -lStMuDSTMaker -L ${STARHOME}/libBrStar/lib -L ${STARHOME}/.sl73_gcc485/lib -lBrStar -lBrEEmcMaker -lBrTemplateMaker -lBrRpMaker
LIB_INCLUDE  = -I${STARHOME}/StRoot/ -I${STAR}/.${STAR_HOST_SYS}/include/ -I${STAR}/ -I${STAR}/StRoot/ -I${STARHOME}/StRoot -I${STARHOME}/libBrStar/src/ -I${STARHOME}/include/ -I${STAR}/StRoot/StJetMaker/towers/ -I${STAR}/StRoot/StJetMaker/tracks/ -I${STAR}/StRoot/StJetMaker/mcparticles/

all: libStarSim

clean: libStarSim_clean
clear: libStarSim_clear


libStarSim: lib/libStarSim.so
libStarSim_base= $(wildcard src/*.cxx)  
libStarSim_inc	= $(patsubst src/%.cxx, ./%.h,$(libStarSim_base)) 
libStarSim_obj	= $(patsubst src/%.cxx, src/%.o,$(libStarSim_base))

lib/libStarSim.so: $(libStarSim_obj)  src/libStarSim_Dict.o
	$(CXX) -o $@ $(CXXFLAGS) -shared -Wl,-soname,libStarSim.so $^	
	@echo "                                                                        "
	@echo "-----------------------------------------------------------------------"
	@echo "Finished the compilation!"
	@echo "-----------------------------------------------------------------------"
src/%.o:src/%.cxx
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(LIB_INCLUDE) $(ROOTFLAGS) $<

src/libStarSim_Dict.cxx:
	cd src/; $(ROOTCINT) -f libStarSim_Dict.cxx -c $(libStarSim_inc) LinkDef.h

libStarSim_clean:
	rm -f $(libStarSim_obj)
	rm -f  src/*_Dict.*
	rm -f lib/libStarSim.so

libStarSim_clear:
	rm -f $(libStarSim_obj)
	rm -f  src/*_Dict.*



