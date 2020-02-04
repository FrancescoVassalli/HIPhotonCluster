#!/bin/csh 
@ p = ( ${1} )
#
set OUT_LOCATION="/sphenix/user/vassalli/idTest/"
set PYTHIA_FILE="/sphenix/user/dvp/gen/newQCD/8/hepmc_QCD25GeV_R04jet40GeV_42p5GeV_eta0p45_seq00000.dat"
set SCRATCH_AREA="srtch"
##
source /phenix/u/vassalli/.cshrc
mkdir $SCRATCH_AREA
cp ../gen/* $SCRATCH_AREA
cp ../clustering/* $SCRATCH_AREA
cp ../subtraction/* $SCRATCH_AREA
cp ../burners/* $SCRATCH_AREA
cp ../condor/burnerMacro.C $SCRATCH_AREA
#
cd $SCRATCH_AREA
set OUT_FILE=HIPYTH${p}.root
set ANA_FILE=DVPANA${p}.root
#root -b -q PythiaFun.C\(10,\"$PYTHIA_FILE\",\"$OUT_LOCATION$OUT_FILE\"\)#,\"$IN_FILE\"\) 
root -b -q burnerMacro.C\(\"$OUT_LOCATION$OUT_FILE\",\"$OUT_LOCATION$ANA_FILE\",${p},0\)
##
rm -rf $SCRATCH_AREA
##
exit 0
