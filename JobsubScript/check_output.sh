#!/bin/bash
DIR_SCRIPT=$(dirname $(readlink -f $BASH_SOURCE))
cd $DIR_SCRIPT
source config_common.sh || exit

##
## Parse command-line options
##
OPTIND=1
while getopts ":a:" OPT ; do
    case $OPT in
        v ) VERBOSE=yes
            ;;
        a ) FN_ARG_LIST=$OPTARG
            ;;
    esac
done
shift $((OPTIND - 1))

if [ ! -f $FN_ARG_LIST ] ; then
    echo "!ERROR!  Cannot find the argument-list file '$FN_ARG_LIST'.  Abort."
    exit
fi

##
## Main
##
FN_RESULT=result_check_output.txt
{
echo "Checking each job output..."

N_ALL=0
N_NO_LOG=0
N_BAD_LOG=0
N_NO_ROOT=0
N_BAD_ROOT=0
while read -a VARS ; do
    ID=${VARS[0]}
    FN_LOG=$DIR_WORK/$ID/log_job.txt
    FN_ROOT=$DIR_WORK/$ID/out/output.root

    (( N_ALL++ ))
    if [ ! -e $FN_LOG ] ; then
	(( N_NO_LOG++ ))
	echo "No log file: $ID"
    elif ! grep -q 'execute_single.sh finished!' $FN_LOG ; then
	(( N_BAD_LOG++ ))
	echo "Bad log file: $ID"
    elif [ ! -e $FN_ROOT ] ; then
	(( N_NO_ROOT++ ))
	echo "No ROOT file: $ID"
    elif [ $(stat --printf='%s' $FN_ROOT) -lt 100 ] ; then
	(( N_BAD_ROOT++ ))
	echo "Bad ROOT file: $ID"
    fi
done <$DIR_SCRIPT/$FN_ARG_LIST

echo "...done."
echo
echo "Statistics: "
echo "  All           = $N_ALL"
echo "  No  log file  = $N_NO_LOG"
echo "  Bad log file  = $N_BAD_LOG"
echo "  No  ROOT file = $N_NO_ROOT"
echo "  Bad ROOT file = $N_BAD_ROOT"

} | tee $FN_RESULT

echo "Finished.  Reported also in '$FN_RESULT'."
