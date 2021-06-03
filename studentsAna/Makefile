# Filename: Makefile
# Description: Makefile for compiling Bright-Star Analysis Framework. This Makefile is tailored for STAR farm nodes which is set for 32 bit.
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Wed May 15 01:12:25 2019 (-0500)
# URL: jlab.org/~latif

ifndef STAR
  $(error $$STAR environment variable not defined. source setup.[c]sh first)
endif

ifndef STARHOME
  $(error $$STARHOME environment variable not defined. source setup.[c]sh first)
endif

ROOTCINT  = rootcint
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs)
CXXFLAGS  += -fPIC -m32 -std=c++11 -L ${STAR}/.sl73_gcc485/lib -lStMuDSTMaker -L ${STARHOME}/libBrStar/lib -lBrStar -L ${STARHOME}/runMacros/lib -lRunMacros
LIB_INCLUDE  = -I${STARHOME}/StRoot/ -I${STAR}/.${STAR_HOST_SYS}/include/ -I${STAR}/ -I${STAR}/StRoot/ -I${STARHOME}/libBrStar/src/ -I${STARHOME}/runMacros/src/ -I${STARHOME} -I${STARHOME}/include/ -I${STAR}/StRoot/StJetMaker/towers/ -I${STAR}/StRoot/StJetMaker/tracks/ -I${STAR}/StRoot/StJetMaker/mcparticles/

all: libUcrAnalysis

clean: libUcrAnalysis_clean
clear: libUcrAnalysis_clear


libUcrAnalysis: lib/libUcrAnalysis.so
libUcrAnalysis_base= $(wildcard src/*.cxx)  
libUcrAnalysis_inc	= $(patsubst src/%.cxx, ./%.h,$(libUcrAnalysis_base)) 
libUcrAnalysis_obj	= $(patsubst src/%.cxx, src/%.o,$(libUcrAnalysis_base))

lib/libUcrAnalysis.so: $(libUcrAnalysis_obj)  src/libUcrAnalysis_Dict.o
	$(CXX) -o $@ $(CXXFLAGS) -shared -Wl,-soname,libUcrAnalysis.so $^	
	@echo "                                                                        "
	@echo "-----------------------------------------------------------------------"
	@echo "Finished the compilation!"
	@echo "-----------------------------------------------------------------------"
src/%.o:src/%.cxx
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(LIB_INCLUDE) $(ROOTFLAGS) $<

src/libUcrAnalysis_Dict.cxx:
	cd src/; $(ROOTCINT) -f libUcrAnalysis_Dict.cxx -c $(libUcrAnalysis_inc) LinkDef.h

libUcrAnalysis_clean:
	rm -f $(libUcrAnalysis_obj)
	rm -f  src/*_Dict.*
	rm -f lib/libUcrAnalysis.so

libUcrAnalysis_clear:
	rm -f $(libUcrAnalysis_obj)
	rm -f  src/*_Dict.*


