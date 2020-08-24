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
ANA_DIR = analysis
MACRO_DIR = runMacros
DIFFANA_DIR = diffAnalysis
EMJANA_DIR = emJetAnalysis
CEP_DIR = cepAnalysis
STARSIM_DIR = starSim
# makefile name
MAKE_FILE = Makefile

####### Build rules
first: all

.PHONY: lib ana cons macro diff emj sim 

all: lib ana cons macro diff emj cep sim
	@echo "done!"	
lib:
	$(MAKE) -C $(LIB_DIR) -f $(MAKE_FILE)
	@cp -ur $(LIB_DIR)/lib/*.so lib
ana: lib
	$(MAKE) -C $(ANA_DIR) -f $(MAKE_FILE)
	@cp -ur $(ANA_DIR)/lib/*.so lib	
diff: lib
	$(MAKE) -C $(DIFFANA_DIR) -f $(MAKE_FILE)
	@cp -ur $(DIFFANA_DIR)/lib/*.so lib
macro: cons
	$(MAKE) -C $(MACRO_DIR) -f $(MAKE_FILE)
	@cp -ur $(MACRO_DIR)/lib/*.so lib
emj: #cons
	$(MAKE) -C $(EMJANA_DIR) -f $(MAKE_FILE)
	@cp -ur $(EMJANA_DIR)/lib/*.so lib
cep: 
	$(MAKE) -C $(CEP_DIR) -f $(MAKE_FILE)
	@cp -ur $(CEP_DIR)/lib/*.so lib
sim: 
	$(MAKE) -C $(STARSIM_DIR) -f $(MAKE_FILE)
	@cp -ur $(STARSIM_DIR)/lib/*.so lib
cons:
	cons

####### Clean
clean: cleanlib cleanana cleanmacro cleancons cleansim cleancep

.PHONY: cleanlib cleanana cleandiff cleanmacro cleanemj cleansim

cleanlib:
	$(MAKE) -C $(LIB_DIR) -f $(MAKE_FILE) clean

cleanana:
	$(MAKE) -C $(ANA_DIR) -f $(MAKE_FILE) clean

cleandiff:
	$(MAKE) -C $(DIFFANA_DIR) -f $(MAKE_FILE) clean

cleanmacro:
	$(MAKE) -C $(MACRO_DIR) -f $(MAKE_FILE) clean

cleanemj:
	$(MAKE) -C $(EMJANA_DIR) -f $(MAKE_FILE) clean

cleancep:
	$(MAKE) -C $(CEP_DIR) -f $(MAKE_FILE) clean

cleansim:
	$(MAKE) -C $(STARSIM_DIR) -f $(MAKE_FILE) clean

cleancons:
	rm -r .$STAR_HOST_SYS/*
