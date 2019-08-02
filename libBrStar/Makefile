# Filename: Makefile
# Description: Makefile for compiling Bright Star Analysis Framework. This Makefile is tailored for STAR farm nodes which is set for 32 bit.
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Wed Nov 15 01:12:25 2017 (-0500)
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
CXXFLAGS  += -fPIC -m32 -std=c++11 -L ${STAR}/.sl73_gcc485/lib -lStMuDSTMaker
MODULES = .part4/include .part8/include .part9/include 
SRC_DIR   = $(addprefix ${STAR}/,$(MODULES))
LIB_INCLUDE  = $(addprefix -I,$(SRC_DIR)) -I${STAR}/StRoot/ -I${STAR}/

all: libBrStar

clean: libBrStar_clean
clear: libBrStar_clear


libBrStar: lib/libBrStar.so
libBrStar_base= $(wildcard src/*.cxx)  
libBrStar_inc	= $(patsubst src/%.cxx, ./%.h,$(libBrStar_base)) 
libBrStar_obj	= $(patsubst src/%.cxx, src/%.o,$(libBrStar_base))

lib/libBrStar.so: $(libBrStar_obj)  src/libBrStar_Dict.o
	$(CXX) -o $@ $(CXXFLAGS) -shared -Wl,-soname,libBrStar.so $^	
	@echo "                                                                        "
	@echo "-----------------------------------------------------------------------"
	@echo "Finished the compilation!"
	@echo "-----------------------------------------------------------------------"
src/%.o:src/%.cxx
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(LIB_INCLUDE) $(ROOTFLAGS) $<

src/libBrStar_Dict.cxx:
	cd src/; $(ROOTCINT) -f libBrStar_Dict.cxx -c $(libBrStar_inc) LinkDef.h

libBrStar_clean:
	rm -f $(libBrStar_obj)
	rm -f  src/*_Dict.*
	rm -f lib/libBrStar.so

libBrStar_clear:
	rm -f $(libBrStar_obj)
	rm -f  src/*_Dict.*


