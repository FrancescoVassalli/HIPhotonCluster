SHELL=/usr/bin/env bash
CXX_COMMON=-O2  -pedantic -W -fPIC
PREFIX_INCLUDE=/phenix/u/vassalli/pythia8230/include
PREFIX_LIB=/phenix/u/vassalli/pythia8230/lib
CXX_COMMON:=-I$(PREFIX_INCLUDE) $(CXX_COMMON)
CXX_COMMON+= -L$(PREFIX_LIB) -Wl,-rpath,$(PREFIX_LIB) -lpythia8 -ldl
CXX=g++
HEPMC2_INCLUDE=/direct/phenix+u/vassalli/HEPBuild/include
HEPMC2_LIB=/direct/phenix+u/vassalli/HEPBuild/lib
all: photonJetGenerator minBiasGen

photonJetGenerator: photonJetGenerator.cc $(PREFIX_LIB)/libpythia8.a
		$(CXX) $< -o photonJetGenerator -std=c++11 -I$(HEPMC2_INCLUDE) $(CXX_COMMON) -\
L$(HEPMC2_LIB) -Wl,-rpath,$(HEPMC2_LIB) -lHepMC `root-config --libs --cflags`

minBiasGen: minBiasGen.cc $(PREFIX_LIB)/libpythia8.a
		$(CXX) $< -o minBiasGen -std=c++11 -I$(PYTHIA8) $(CXX_COMMON) -\
L -Wl,-rpath `root-config --libs --cflags`
