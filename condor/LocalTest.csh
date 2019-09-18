#!/bin/csh 
@ p = ( ${1} )
#
set OUT_LOCATION="/sphenix/user/vassalli/idTest/"
set OUT_FILE="out${p}.root"
set SCRATCH_AREA="/sphenix/user/vassalli/srtch/"
set ANA_FILE="ana${p}.root"
#
set SOURCE_GEN="/direct/phenix+u/vassalli/sphenix/id/gen/*"
set BURNER="burnerMacro.C"
#
source /phenix/u/vassalli/.cshrc
mkdir $SCRATCH_AREA
cp  $SOURCE_GEN $SCRATCH_AREA
cp $BURNER $SCRATCH_AREA
cp ../clustering/* $SCRATCH_AREA
cp ../subtraction/* $SCRATCH_AREA
cp ../burners/* $SCRATCH_AREA
#
cd $SCRATCH_AREA
root -b -q Fun4All_G4_sPHENIX.C\(1,\"\",\"$OUT_FILE\",\"\"\) 
cp $OUT_FILE $OUT_LOCATION$OUT_FILE
root -b -q $BURNER\(\"$OUT_LOCATION$OUT_FILE\",\"$OUT_LOCATION$ANA_FILE\",$p\)
#
rm -rf $SCRATCH_AREA
#
exit 0
