#!/bin/csh 
@ p =  ${1}
#
set OUT_LOCATION="/sphenix/user/vassalli/minBiasPythia/"
set PYTHIA_FILE=${OUT_LOCATION}pJet${p}
#
set SCRATCH_AREA="$_CONDOR_SCRATCH_DIR/fran_photons${p}"
#
set GENNAME="photonJetGenerator"
#
source /phenix/u/vassalli/.cshrc
mkdir $SCRATCH_AREA
cp ./pythia/$GENNAME $SCRATCH_AREA
#
cd $SCRATCH_AREA
./$GENNAME $PYTHIA_FILE 3
#
rm -r $SCRATCH_AREA
#
exit 0
