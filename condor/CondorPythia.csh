#!/bin/csh 
@ p =  ${1}
#
set OUT_LOCATION="/sphenix/user/vassalli/minBiasPythia/"
set PYTHIA_FILE=${OUT_LOCATION}out${p}.root
#
set SCRATCH_AREA="$_CONDOR_SCRATCH_DIR/fran_photons${p}"
#
set GENNAME="minBiasGen"
#
source /phenix/u/vassalli/.cshrc
mkdir $SCRATCH_AREA
cp ./pythia/$GENNAME $SCRATCH_AREA
#
cd $SCRATCH_AREA
./$GENNAME $PYTHIA_FILE 1000000
#
rm -r $SCRATCH_AREA
#
exit 0
