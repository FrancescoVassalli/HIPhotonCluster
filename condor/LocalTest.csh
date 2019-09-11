#!/bin/csh 
@ p = ( ${1} )
#
set OUT_LOCATION="/sphenix/user/vassalli/idTest/"
set OUT_FILE=${OUT_LOCATION}out${p}.root
set SCRATCH_AREA="/sphenix/user/vassalli/srtch/"
set IN_FILE=""
#
set SOURCE_GEN="/direct/phenix+u/vassalli/sphenix/id/gen/*"
#
source /phenix/u/vassalli/.cshrc
mkdir $SCRATCH_AREA
cp  $SOURCE_GEN $SCRATCH_AREA
#cp $BURNER $SCRATCH_AREA
cp ../clustering/* $SCRATCH_AREA
cp ../subtraction/* $SCRATCH_AREA
#
cd $SCRATCH_AREA
root -b -q Fun4All_G4_sPHENIX.C\(1,\"$IN_FILE\",\"$OUT_FILE\",\"\"\) 
#root -b -q cluster_burner.C\(\"$IN_FILE\",\"$OUT_FILE\",$p\)
#
rm -rf $SCRATCH_AREA
#
exit 0
