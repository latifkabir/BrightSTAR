# Filename: Makefile
# Description: Makefile for building EG2 analysis tools
# Author: Latif Kabir < latif@jlab.org >
# Created: Wed Nov 15 01:00:17 2017 (-0500)
# URL: latifkabir.github.io

ifndef STARHOME
  $(error $$STARHOME environment variable not defined. source setup.[c]sh first)
endif

# directories
LIB_DIR = libBrStar
MACRO_DIR = runMacros
EMJANA_DIR = emJetAnalysis
STARSIM_DIR = starSim
# makefile name
MAKE_FILE = Makefile

####### Build rules
first: all

.PHONY: lib ana cons macro diff emj sim 

all: lib cons macro emj sim
	@echo "done!"	
lib:
	$(MAKE) -C $(LIB_DIR) -f $(MAKE_FILE)
	@cp -ur $(LIB_DIR)/lib/*.so lib
macro: cons
	$(MAKE) -C $(MACRO_DIR) -f $(MAKE_FILE)
	@cp -ur $(MACRO_DIR)/lib/*.so lib
emj: #cons
	$(MAKE) -C $(EMJANA_DIR) -f $(MAKE_FILE)
	@cp -ur $(EMJANA_DIR)/lib/*.so lib
	@cp -ur $(STDANA_DIR)/lib/*.so lib
sim: 
	$(MAKE) -C $(STARSIM_DIR) -f $(MAKE_FILE)
	@cp -ur $(STARSIM_DIR)/lib/*.so lib
cons:
	cons

####### Clean
clean: cleanlib cleanmacro cleancons cleansim 

.PHONY: cleanlib cleanana cleanmacro cleanemj cleansim

cleanlib:
	$(MAKE) -C $(LIB_DIR) -f $(MAKE_FILE) clean

cleanmacro:
	$(MAKE) -C $(MACRO_DIR) -f $(MAKE_FILE) clean

cleanemj:
	$(MAKE) -C $(EMJANA_DIR) -f $(MAKE_FILE) clean

cleansim:
	$(MAKE) -C $(STARSIM_DIR) -f $(MAKE_FILE) clean

cleancons:
	rm -r .${STAR_HOST_SYS}/*
