#!/bin/csh 
@ p = ( ${1} )
#
set OUT_LOCATION="/sphenix/user/vassalli/idTest/HIsample/"
#
set SCRATCH_AREA="$_CONDOR_SCRATCH_DIR/fran_HIDST${p}" 
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
set OUT_FILE=HIPYTH${p}.root
set ANA_FILE=hipytha_ANA${p}.root

root -b -q burnerMacro.C\(\"$OUT_LOCATION$OUT_FILE\",\"$OUT_LOCATION$ANA_FILE\",${p},1\)
#
rm -rf $SCRATCH_AREA
#
exit 0
