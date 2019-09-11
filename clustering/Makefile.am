##############################################
# please add new classes in alphabetical order

AUTOMAKE_OPTIONS = foreign

# List of shared libraries to produce
lib_LTLIBRARIES = \
  libcalo_reco.la 

libcalo_reco_la_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

libcalo_reco_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lgsl \
  -lgslcblas \
  -lcalo_io \
	-lcalo_reco\
  -lcalo_util  \
  -lphparameter

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include 

pkginclude_HEADERS = \
  RawClusterBuilderTemplateSub.h \
	BEmcCluster.h \
	BEmcRec.h \
	BEmcRecCEMC.h

if ! MAKEROOT6
  ROOT5_DICTS = \
  RawClusterBuilderTemplateSub_Dict.cc 
endif

libcalo_reco_la_SOURCES = \
  $(ROOT5_DICTS) \
  RawClusterBuilderTemplateSub.cc \
	BEmcRec.cc \
	BEmcCluster.cc

# Rule for generating table CINT dictionaries.
%_Dict.cc: %.h %LinkDef.h
	rootcint -f $@ @CINTDEFS@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^


################################################
# linking tests

noinst_PROGRAMS = \
  testexternals_calo_reco

BUILT_SOURCES  = testexternals.cc

testexternals_calo_reco_SOURCES = testexternals.cc
testexternals_calo_reco_LDADD = libcalo_reco.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

##############################################
# please add new classes in alphabetical order

clean-local:
	rm -f *Dict* $(BUILT_SOURCES) *.pcm
