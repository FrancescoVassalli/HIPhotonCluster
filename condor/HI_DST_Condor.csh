#!/bin/csh 
@ p = ( ${1} )
#
set OUT_LOCATION="/sphenix/user/vassalli/idTest/HIsample/"
set OUT_FILE=HI_DST_OUT${p}.root
@ Nfile = ($p + 3000)
set IN_FILE="/sphenix/data/data02/jet-production-hijing-AuAu200-2018-11/Iteration2-innerHCalAl/MidCentral-0-6.6fm-Iteration2-innerHCalAl/MidCentral-0-6.6fm-Iteration2-innerHCalAl_${Nfile}.xml.root"
#
set SCRATCH_AREA="$_CONDOR_SCRATCH_DIR/fran_HIDST${p}" 
#
source /phenix/u/vassalli/.cshrc
mkdir $SCRATCH_AREA
cp /gen/* $SCRATCH_AREA
cp /clustering/* $SCRATCH_AREA
cp /subtraction/* $SCRATCH_AREA
#
cd $SCRATCH_AREA
root -b -q Fun4All_G4_sPHENIX.C\(20,\"\",\"$OUT_LOCATION$OUT_FILE\",\"$IN_FILE\"\) 
#
rm -rf $SCRATCH_AREA
#
exit 0
