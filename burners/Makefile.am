AUTOMAKE_OPTIONS = foreign

INCLUDES = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I`root-config --incdir`

libsubtractionburner_la_LDFLAGS = \
  -nodefaultlibs \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  `root-config --libs`

lib_LTLIBRARIES = \
  libsubtractionburner.la

include_HEADERS = \
	GammaClusBurner.h\
	GammaClusBurnerLinkDef.h\
	GammaLocal.h\
	GammaLocalLinkDef.h\
	ChaseTower.h\
	ChaseTowerLinkDef.h\
	TowerMap.h\
	TowerMapLinkDef.h\
	IDBurner.h\
	IDBurnerLinkDef.h\
	AllClusBurner.h\
	AllClusBurnerLinkDef.h

if MAKEROOT6
pcmdir = $(libdir)
nobase_dist_pcm_DATA = \
	GammaClusBurner_Dict_rdict.pcm \
	GammaLocal_Dict_rdict.pcm \
	AllClusBurner_Dict_rdict.pcm \
	ChaseTower_Dict_rdict.pcm \
	TowerMap_Dict_rdict.pcm \
	IDBurner_Dict_rdict.pcm 
endif

libsubtractionburner_la_SOURCES = \
  AllClusBurner.C \
	AllClusBurner_Dict.C\
  GammaLocal.C \
	GammaLocal_Dict.C\
  ChaseTower.C \
	ChaseTower_Dict.C\
  TowerMap.C \
	TowerMap_Dict.C\
  IDBurner.C \
	IDBurner_Dict.C

libsubtractionburner_la_LIBADD = \
  -lg4dst \
  -lg4eval \
  -ljetbackground \
  -lphool

BUILT_SOURCES = \
  testexternals.C

noinst_PROGRAMS = \
  testexternals

testexternals_LDADD = \
  libsubtractionburner.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

##############################################
# please add new classes in alphabetical order
# Rule for generating CINT dictionaries from class headers.
%_Dict.C: %.h %LinkDef.h
	rootcint -f $@ -c $(CINTFLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict* testexternals.C

testexternals_SOURCES = testexternals.C
