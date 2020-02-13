#!/bin/csh 
@ p = ( ${1} )
#
set OUT_LOCATION="/sphenix/user/vassalli/idTest/HIsample/"
#
@ n1 = 50 * ($p + 1)
@ n2 = 50 * $p
set s1 = ""
set s2 = ""
if ($n1 < 10000 && $n1 >= 1000) then 
    set s1 = "0"
endif
if ($n1 < 1000 && $n1 >= 100) then 
    set s1 = "00"
endif
if ($n1 < 100 && $n1 >= 10) then 
    set s1 = "000"
endif
if ($n2 < 10000 && $n2 >= 1000) then 
    set s2 = "0"
endif
if ($n2 < 1000 && $n2 >= 100) then 
    set s2 = "00"
endif
if ($n2 < 100 && $n2 >= 10) then 
    set s2 = "000"
endif
if ($n2 < 10 ) then 
    set s2 = "0000"
endif
set IN_FILE="/sphenix/sim/sim01/sphnxpro/sHijing/Geant4-10.02.p02/QGSP_BERT_HP/MinBiasFlow/2019-10-20/fm_0-6.6/G4Hits_sHijing_0-6.6fm_${s2}${n2}_${s1}${n1}.root"
set PYTHIA_FILE="sphenix/user/dvp/gen/newQCD/8/hepmc_QCD25GeV_R04jet40GeV_42p5GeV_eta0p45_seq00000.dat"
set SCRATCH_AREA="$_CONDOR_SCRATCH_DIR/fran_photons${p}"
#
source /phenix/u/vassalli/.cshrc
mkdir $SCRATCH_AREA
cp gen/* $SCRATCH_AREA
cp clustering/* $SCRATCH_AREA
cp subtraction/* $SCRATCH_AREA
cp burners/* $SCRATCH_AREA
cp condor/burnerMacro.C $SCRATCH_AREA
#
cd $SCRATCH_AREA
set OUT_FILE=HI_DVP_PYTH${p}.root
set ANA_FILE=hi_DVP_pytha_ANA${p}.root
#
root -b -q PythiaFun4.C\(10,\"$PYTHIA_FILE\",\"$OUT_LOCATION$OUT_FILE\"\) 
root -b -q burnerMacro.C\(\"$OUT_LOCATION$OUT_FILE\",\"$OUT_LOCATION$ANA_FILE\",${p}\)
#
rm -rf $SCRATCH_AREA
#
exit 0
