#!/bin/bash
if [ -z "$CONDOR_DIR_INPUT" -o -z "$CONDOR_DIR_OUTPUT" ] ; then
    echo "!ERROR!  CONDOR_DIR_INPUT/OUTPUT is invalid.  Abort."
    exit 1
fi
echo "INPUT  = $CONDOR_DIR_INPUT"
echo "OUTPUT = $CONDOR_DIR_OUTPUT"
echo "HOST   = $HOSTNAME"

ID=$1
FN_DATA=$2
test ${FN_DATA:0:1} = '/' || FN_DATA=$CONDOR_DIR_INPUT/$FN_DATA

tar xzf $CONDOR_DIR_INPUT/input.tar.gz
echo "--- ls -lh -------------"
ls -lh
echo "------------------------"
echo "--- ls -lh /usr/lib64 --"
ls -lh /usr/lib64
echo "------------------------"

FN_SETUP=/e906/app/software/osg/software/e1039/this-e1039.sh
if [ ! -e $FN_SETUP ] ; then # On grid
    FN_SETUP=/cvmfs/seaquest.opensciencegrid.org/seaquest/${FN_SETUP#/e906/app/software/osg/}
fi
source $FN_SETUP
export LD_LIBRARY_PATH=inst/lib:$LD_LIBRARY_PATH

time root -b -q "Fun4RealDst.C($ID, \"$FN_DATA\")" 2>&1 | tee $CONDOR_DIR_OUTPUT/log_macro.txt
RET=${PIPESTATUS[0]} # $?
if [ $RET -ne 0 ] ; then
    echo "ROOT returned $RET."
    exit $RET
fi

mv output.root  $CONDOR_DIR_OUTPUT

echo "execute_macro.sh finished!"
