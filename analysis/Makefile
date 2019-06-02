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
CXXFLAGS  += -fPIC -m32 -std=c++11 -L ${STAR}/.sl73_gcc485/lib -lStMuDSTMaker -L ${STARHOME}/libBrStar/lib -lBrStar
MODULES   = StMuDSTMaker/COMMON StChain St_base Star2Root StEvent StarClassLibrary 
SRC_DIR   = $(addprefix ${STAR}/StRoot/,$(MODULES))
LIB_INCLUDE  = $(addprefix -I,$(SRC_DIR)) -I${STAR}/StRoot/ -I${STARHOME}/libBrStar/src/ 

all: libStAnalysis

clean: libStAnalysis_clean
clear: libStAnalysis_clear


libStAnalysis: lib/libStAnalysis.so
libStAnalysis_base= $(wildcard src/*.cxx)  
libStAnalysis_inc	= $(patsubst src/%.cxx, ./%.h,$(libStAnalysis_base)) 
libStAnalysis_obj	= $(patsubst src/%.cxx, src/%.o,$(libStAnalysis_base))

lib/libStAnalysis.so: $(libStAnalysis_obj)  src/libStAnalysis_Dict.o
	$(CXX) -o $@ $(CXXFLAGS) -shared -Wl,-soname,libStAnalysis.so $^	
	@echo "                                                                        "
	@echo "-----------------------------------------------------------------------"
	@echo "Finished the compilation!"
	@echo "-----------------------------------------------------------------------"
src/%.o:src/%.cxx
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(LIB_INCLUDE) $(ROOTFLAGS) $<

src/libStAnalysis_Dict.cxx:
	cd src/; $(ROOTCINT) -f libStAnalysis_Dict.cxx -c $(libStAnalysis_inc) LinkDef.h

libStAnalysis_clean:
	rm -f $(libStAnalysis_obj)
	rm -f  src/*_Dict.*
	rm -f lib/libStAnalysis.so

libStAnalysis_clear:
	rm -f $(libStAnalysis_obj)
	rm -f  src/*_Dict.*


