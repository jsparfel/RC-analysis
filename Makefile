CC = gcc
CXX = g++
CCFLAGS = -std=c++11 -g -W -Wall -Wno-unused-parameter -Wno-ignored-qualifiers #-pedantic -fPIC

##### ROOT
# REQUIRE ROOT
# (Assuming a defined ROOTSYS guarantees all ROOT-related stuff infra works...) 
ifeq ($(ROOTSYS),)
  $(error Environment ROOTSYS not set)
endif
ROOTFLAGS = `root-config --cflags --glibs`
ROOTVERSION = -D ROOT5   # Probably useless

##### PHAST
ifeq ($(PHAST),)
  $(error Environment PHAST not set)
endif
PHAST_LIBS =
PHAST_INCL =

ifeq ($(SITE),BW)
CCFLAGS = -DBW
endif

ifeq ($(DEBUG),1)
CCFLAGS += -DDEBUG
endif

all : RC
RC : RC_split RC_collect RC_plot

%.o: %.cc %.h
	@$(CXX) $(CCFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -c -o $@ $<

RC_split: RC_split.cc RC_split.h
	@echo 'Building radiative corrections analysis package..'
	@$(CXX) $(CCFLAGS) -Wno-ignored-qualifiers $(ROOTFLAGS) $(ROOTVERSION) -o $@ $< $(PHAST_LIBS) $(PHAST_INCL)

RC_collect: RC_collect.cc RC_collect.h
	@$(CXX) $(CCFLAGS) -Wno-ignored-qualifiers $(ROOTFLAGS) $(ROOTVERSION) -o $@ $< $(PHAST_LIBS) $(PHAST_INCL)

RC_plot: RC_plot.cc RC_plot.h
	@$(CXX) $(CCFLAGS) -Wno-ignored-qualifiers $(ROOTFLAGS) $(ROOTVERSION) -o $@ $< $(PHAST_LIBS) $(PHAST_INCL)
	@echo 'Radiative corrections analysis package built !'

clean :
	@rm -rf *.o RC_split RC_collect RC_plot
