#!/bin/csh 
@ p = ( ${1} )
#
set OUT_LOCATION="/sphenix/user/vassalli/idTest/HIsample/"
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
set OUT_FILE=Signal_DST_OUT${p}.root
set ANA_FILE=Signal_ANA${p}.root
#
set SCRATCH_AREA="$_CONDOR_SCRATCH_DIR/fran_HIDST${p}" 
#set SCRATCH_AREA="srtch" 
#
source /phenix/u/vassalli/.cshrc
mkdir $SCRATCH_AREA
cp gen/* $SCRATCH_AREA
cp clustering/* $SCRATCH_AREA
cp subtraction/* $SCRATCH_AREA
cp burners/* $SCRATCH_AREA
cp condor/* $SCRATCH_AREA
#
cd $SCRATCH_AREA
root -b -q FunSignal.C\(3,\"\",\"$OUT_LOCATION$OUT_FILE\",\"$IN_FILE\"\) 
root -b -q burnerMacro.C\(\"$OUT_LOCATION$OUT_FILE\",\"$OUT_LOCATION$ANA_FILE\",${p},1\)
#
rm -rf $SCRATCH_AREA
#
exit 0
