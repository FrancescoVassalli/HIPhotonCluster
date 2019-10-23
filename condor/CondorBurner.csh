#!/bin/csh 
@ p = ( ${1} )
#
set OUT_LOCATION="/sphenix/user/vassalli/idTest/singlesample/"
set OUT_FILE=G_DST_OUT${p}.root
set ANA_FILE=G_ANA${p}.root
#
set SCRATCH_AREA="$_CONDOR_SCRATCH_DIR/fran_HIDST${p}" 
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
#root -b -q Fun4All_G4_sPHENIX.C\(5,\"\",\"$OUT_LOCATION$OUT_FILE\"\) 
root -b -q burnerMacro.C\(\"$OUT_LOCATION$OUT_FILE\",\"$OUT_LOCATION$ANA_FILE\",${p}\)
#
rm -rf $SCRATCH_AREA
#
exit 0
