AUTOMAKE_OPTIONS = foreign

INCLUDES = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I`root-config --incdir`

libhisubtraction_la_LDFLAGS = \
  -nodefaultlibs \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  `root-config --libs`

lib_LTLIBRARIES = \
  libhisubtraction.la

include_HEADERS = \
  SubtractCEMC.h \
  SubtractCEMCLinkDef.h

if MAKEROOT6
pcmdir = $(libdir)
nobase_dist_pcm_DATA = \
	SubtractCEMC_Dict_rdict.pcm 
endif

libhisubtraction_la_SOURCES = \
  SubtractCEMC.C \
  SubtractCEMC_Dict.C

libhisubtraction_la_LIBADD = \
  -lg4dst \
  -lg4eval \
  -ljetbackground \
  -lphool

BUILT_SOURCES = \
  testexternals.C

noinst_PROGRAMS = \
  testexternals

testexternals_LDADD = \
  libhisubtraction.la

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
