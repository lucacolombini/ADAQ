######################################################################
#
# name: Makefile
# date: 24 Sep 15
# auth: Zach Hartwig
# mail: hartwig@psfc.mit.edu
#
# desc: This GNUmakefile controls the building two ASIM libraries the
#       readout and analysis of Geant4 simulated data within the ADAQ
#       framework. libASIMReadout.so contains a powerful set of
#       classes that can dropped into a user's Geant4 simulation for
#       readout of scintillator and photodetector data;
#       libASIMStorage.so contains classes for persistent storage of
#       the data that is provided by libASIMReadout into ASIM
#       files. The libASIMStorage.so library, unlike the
#       libASIMReadout.so library, intentionally does *not* require
#       the Geant4 libraries/headers and so may be utilized into a
#       user's code for the analysis of ASIM files on without
#       necessitating a Geant4 install.
#
#       Both libraries are built locally within the build/ directory
#       when 'make' is called and then copied - along with the header
#       files -into the top-level $ADAQ/lib/$ARCH and $ADAQ/include
#       when a 'make install' for general purpose access by other
#       codes. Note that $ARCH specifies the system architecture (32-
#       or 64-bit)
#
# dpnd: libADAQReadout.so requires:
#       -- The ROOT toolkit (v5.34.30 latest tested)
#
#       libADAQStorage.so requires
#       -- The ROOT toolkit (v5.34.30 latest tested)
#       -- The Geant4 toolkit (v10.01 latest tested)
#
# 2run: To build the libraries locally
#       $ make
# 
#       To install library and headers in $ADAQ/lib and $ADAQ/include
#       $ make install
#
#       To clean up transient build files and delete local library
#       $ make clean
#    
######################################################################

#***************************#
#**** MACRO DEFINITIONS ****#
#***************************#

# Include the Makefile for ROOT-based projects
RC:=root-config
ROOTSYS:=$(shell $(RC) --prefix)
ROOTMAKE:=$(ROOTSYS)/etc/Makefile.arch
include $(ROOTMAKE)

# Note that at this point in the makefile CXXFLAGS now includes
# ROOT-based flags so we must ensure to use "+=" to add more flags

# Obtain CPU architecture: 32-bit ("i686") or 64-bit ("x86_64") (Must
# be set after the ROOT makefile is included)
ARCH=$(shell uname -m)

# Include the Geant4 header files and libraries
GC:=geant4-config
G4SYS:=$(shell $(GC) --prefix)
G4INCLDIR=$(G4SYS)/include/Geant4
G4LIBS=$(shell $(GC) --libs)
CXXFLAGS += -I$(G4INCLDIR)

# Specify the top-level ADAQ destination directories for the built
# libraries and headers during installation
DESTINCL = $(ADAQHOME)/include
DESTLIB = $(ADAQHOME)/lib/$(ARCH)

# Specify header files directory. Note that this must be an absolute
# path to ensure the ROOT dictionary files can find the headers
INCLDIR = $(PWD)/include
CXXFLAGS += -I$(INCLDIR)/

# Specify the build and source directories
BUILDDIR = build
SRCDIR = src

# List all of the ADAQ header files
INCLS = $(wildcard $(INCLDIR)/*.hh)

# List of headers necessary for ROOT dictionary generation
ROOTDICTINCLS = $(INCLDIR)/ASIMEvent.hh $(INCLDIR)/ASIMRun.hh $(INCLDIR)/ASIMStorageManager.hh

# Specify necessary files for the R(eadout) library

R_SRCS = $(SRCDIR)/ASIMPhotodetectorSD.cc \
         $(SRCDIR)/ASIMPhotodetectorSDHit.cc \
         $(SRCDIR)/ASIMReadoutManager.cc \
         $(SRCDIR)/ASIMReadoutMessenger.cc \
         $(SRCDIR)/ASIMScintillatorSD.cc \
         $(SRCDIR)/ASIMScintillatorSDHit.cc 
R_TMP = $(patsubst %.cc,%.o,$(R_SRCS))
R_OBJS = $(subst src/,build/,$(R_TMP))
R_TARGET =$(BUILDDIR)/libASIMReadout.so

# Specifcy necessary files for the S(torage) library

S_SRCS = $(SRCDIR)/ASIMEvent.cc \
         $(SRCDIR)/ASIMRun.cc \
         $(SRCDIR)/ASIMStorageManager.cc
S_TMP = $(patsubst %.cc,%.o,$(S_SRCS))
S_OBJS = $(subst src/,build/,$(S_TMP))
S_OBJS += $(BUILDDIR)/ASIMStorageDict.o
S_TARGET = $(BUILDDIR)/libASIMStorage.so


#***************#
#**** RULES ****#
#***************#

all: $(R_TARGET) $(S_TARGET)

# Build the libASIMReadout.so library

$(R_TARGET) : $(R_OBJS) $(INCLS)
	@echo -e "\nBuilding $@ ..."
	$(CXX) $(CXXFLAGS) -g $(R_OBJS) $(ROOTLIBS) -shared -o $@ 
	@echo -e "\n$@ build is complete!\n"

$(BUILDDIR)/%.o : $(SRCDIR)/%.cc $(INCLS)
	@echo -e "\nBuilding object file '$@' ..."
	$(CXX) $(CXXFLAGS) -g -c -o $@ $<

# Build the libASIMStorage.so library

$(S_TARGET) : $(S_OBJS) $(INCLS)
	@echo -e "\nBuilding $@ ..."
	$(CXX) $(CXXFLAGS) -g $(S_OBJS) $(ROOTLIBS) -shared -o $@ 
	@echo -e "\n$@ build is complete!\n"

$(BUILDDIR)/%.o : $(SRCDIR)/%.cc $(INCLS)
	@echo -e "\nBuilding object file '$@' ..."
	$(CXX) $(CXXFLAGS) -g -c -o $@ $<

$(BUILDDIR)/ASIMStorageDict.o : $(BUILDDIR)/ASIMStorageDict.cc
	@echo -e "\nBuilding '$@' ..."
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/ASIMStorageDict.cc : $(ROOTDICTINCLS) $(INCLDIR)/RootLinkDef.h
	@echo -e "\nGenerating ROOT dictionary '$@' ..."
	rootcint -f $@ -c $^ 

# PHONY rules

.PHONY:

# Clean up build files and libraries
clean:
	@echo -e "\nCleaning up the build files and libraries ..."
	@rm -f $(BUILDDIR)/*
	@echo -e ""

# Install libs and headers into top-level ADAQ directory
install:
	@echo -e "\nInstalling the ASIM libraries into $(ADAQ)/lib/$(ARCH) ..."
	@cp -v $(R_TARGET) $(S_TARGET) $(DESTLIB)
	@echo -e "\nInstalling the ASIM headers into $(ADAQ)/include ..."
	@cp -v include/*.hh $(DESTINCL)
	@echo -e ""