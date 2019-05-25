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

# makefile name
MAKE_FILE = Makefile

####### Build rules
first: all

.PHONY: lib ana

all: lib ana
	@echo "done!"	
lib:
	$(MAKE) -C $(LIB_DIR) -f $(MAKE_FILE)

ana: lib
	$(MAKE) -C $(ANA_DIR) -f $(MAKE_FILE)

####### Clean
clean: cleanlib cleanana 

.PHONY: cleanlib cleanana 

cleanlib:
	$(MAKE) -C $(LIB_DIR) -f $(MAKE_FILE) clean

cleanana:
	$(MAKE) -C $(ANA_DIR) -f $(MAKE_FILE) clean

