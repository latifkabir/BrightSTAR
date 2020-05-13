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
CXXFLAGS  += -fPIC -m32 -std=c++11 -L ${STAR}/.sl73_gcc485/lib -lStMuDSTMaker -L ${STARHOME}/libBrStar/lib -lBrStar -L ${STARHOME}/runMacros/lib -lRunMacros
MODULES = .part4/include .part8/include .part9/include 
SRC_DIR   =   $(addprefix ${STARDEV}/,$(MODULES)) 
LIB_INCLUDE  = -I${STARHOME}/StRoot/ $(addprefix -I,$(SRC_DIR)) -I${STAR}/ -I${STAR}/StRoot/ -I${STARHOME}/libBrStar/src/ -I${STARHOME}/runMacros/src/ -I${STARHOME} -I${STARHOME}/include/ -I${STAR}/StRoot/StJetMaker/towers/ -I${STAR}/StRoot/StJetMaker/tracks/ -I${STAR}/StRoot/StJetMaker/mcparticles/

all: libEmJetAnalysis

clean: libEmJetAnalysis_clean
clear: libEmJetAnalysis_clear


libEmJetAnalysis: lib/libEmJetAnalysis.so
libEmJetAnalysis_base= $(wildcard src/*.cxx)  
libEmJetAnalysis_inc	= $(patsubst src/%.cxx, ./%.h,$(libEmJetAnalysis_base)) 
libEmJetAnalysis_obj	= $(patsubst src/%.cxx, src/%.o,$(libEmJetAnalysis_base))

lib/libEmJetAnalysis.so: $(libEmJetAnalysis_obj)  src/libEmJetAnalysis_Dict.o
	$(CXX) -o $@ $(CXXFLAGS) -shared -Wl,-soname,libEmJetAnalysis.so $^	
	@echo "                                                                        "
	@echo "-----------------------------------------------------------------------"
	@echo "Finished the compilation!"
	@echo "-----------------------------------------------------------------------"
src/%.o:src/%.cxx
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(LIB_INCLUDE) $(ROOTFLAGS) $<

src/libEmJetAnalysis_Dict.cxx:
	cd src/; $(ROOTCINT) -f libEmJetAnalysis_Dict.cxx -c $(libEmJetAnalysis_inc) LinkDef.h

libEmJetAnalysis_clean:
	rm -f $(libEmJetAnalysis_obj)
	rm -f  src/*_Dict.*
	rm -f lib/libEmJetAnalysis.so

libEmJetAnalysis_clear:
	rm -f $(libEmJetAnalysis_obj)
	rm -f  src/*_Dict.*


