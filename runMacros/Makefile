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

ROOTCINT  = rootcint
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs)
CXXFLAGS  += -fPIC -m32 -std=c++11 -L ${STAR}/.sl73_gcc485/lib -lStMuDSTMaker -L ${STARHOME}/libBrStar/lib -L ${STARHOME}/.sl73_gcc485/lib -lBrStar -lBrEEmcMaker -lBrTemplateMaker -lBrRpMaker
MODULES = .part4/include .part8/include .part9/include 
SRC_DIR   = $(addprefix ${STARDEV}/,$(MODULES))
LIB_INCLUDE  = $(addprefix -I,$(SRC_DIR)) -I${STAR}/ -I${STAR}/StRoot/ -I${STARHOME}/StRoot -I${STARHOME}/libBrStar/src/ -I${STARHOME}/include/ -I${STARHOME}/StRoot/ -I${STAR}/StRoot/StJetMaker/towers/ -I${STAR}/StRoot/StJetMaker/tracks/ -I${STAR}/StRoot/StJetMaker/mcparticles/

all: libRunMacros

clean: libRunMacros_clean
clear: libRunMacros_clear


libRunMacros: lib/libRunMacros.so
libRunMacros_base= $(wildcard src/*.cxx)  
libRunMacros_inc	= $(patsubst src/%.cxx, ./%.h,$(libRunMacros_base)) 
libRunMacros_obj	= $(patsubst src/%.cxx, src/%.o,$(libRunMacros_base))

lib/libRunMacros.so: $(libRunMacros_obj)  src/libRunMacros_Dict.o
	$(CXX) -o $@ $(CXXFLAGS) -shared -Wl,-soname,libRunMacros.so $^	
	@echo "                                                                        "
	@echo "-----------------------------------------------------------------------"
	@echo "Finished the compilation!"
	@echo "-----------------------------------------------------------------------"
src/%.o:src/%.cxx
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(LIB_INCLUDE) $(ROOTFLAGS) $<

src/libRunMacros_Dict.cxx:
	cd src/; $(ROOTCINT) -f libRunMacros_Dict.cxx -c $(libRunMacros_inc) LinkDef.h

libRunMacros_clean:
	rm -f $(libRunMacros_obj)
	rm -f  src/*_Dict.*
	rm -f lib/libRunMacros.so

libRunMacros_clear:
	rm -f $(libRunMacros_obj)
	rm -f  src/*_Dict.*


