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
  SubtractedClusterBurner.h \
	GammaClusBurner.h\
	GammaClusLinkDef.h\
  SubtractedClusterBurnerLinkDef.h
if MAKEROOT6
pcmdir = $(libdir)
nobase_dist_pcm_DATA = \
	SubtractedClusterBurner_Dict_rdict.pcm 
	GammaClusBurner_Dict_rdict.pcm 
endif

libsubtractionburner_la_SOURCES = \
  SubtractedClusterBurner.C \
  GammaClusBurner.C \
  SubtractedClusterBurner_Dict.C

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
