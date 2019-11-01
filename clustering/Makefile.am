AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include \
  -I`root-config --incdir`

libcalo_reco_la_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib\
 `root-config --libs`

lib_LTLIBRARIES = \
  libcalo_reco.la 

pkginclude_HEADERS = \
  RawClusterBuilderGraph.h \
  RawClusterBuilderFwd.h \
  RawClusterBuilderTemplate.h \
  RawClusterBuilderTemplateSub.h \
  RawClusterBuilderTemplateFEMC.h \
  RawClusterPositionCorrection.h \
  RawTowerCalibration.h 

libcalo_reco_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lgsl \
  -lgslcblas \
  -lcalo_io \
  -lcalo_util  \
  -lphparameter

if ! MAKEROOT6
  ROOT5_DICTS = \
  RawClusterBuilderGraph_Dict.cc \
  RawClusterBuilderFwd_Dict.cc \
  RawClusterBuilderTemplate_Dict.cc \
  RawClusterBuilderTemplateSub_Dict.cc \
  RawClusterBuilderTemplateFEMC_Dict.cc \
  RawClusterPositionCorrection_Dict.cc \
  RawTowerCalibration_Dict.cc 
endif


libcalo_reco_la_SOURCES = \
  $(ROOT5_DICTS) \
  BEmcCluster.cc \
  BEmcRec.cc \
  BEmcRecCEMC.cc \
  BEmcRecFEMC.cc \
	BEmcProfile.cc \
  RawClusterBuilderGraph.cc \
  RawClusterBuilderFwd.cc \
  RawClusterBuilderTemplate.cc \
  RawClusterBuilderTemplateSub.cc \
  RawClusterBuilderTemplateFEMC.cc \
  RawClusterPositionCorrection.cc \
  RawTowerCalibration.cc 

# Rule for generating table CINT dictionaries.
%_Dict.cc: %.h %LinkDef.h
	rootcint -f $@ -c $(CINTFLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^


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
