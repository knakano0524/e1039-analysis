#!/bin/bash
DIR_SCRIPT=$(dirname $(readlink -f $BASH_SOURCE))
source $DIR_SCRIPT/config_common.sh || exit

##
## Parse command-line options
##
VERBOSE=no
DO_OVERWRITE=no
USE_GRID=no
LINE_MIN=1
LINE_MAX=0

OPTIND=1
while getopts ":voga:l:" OPT ; do
    case $OPT in
        v ) VERBOSE=yes
            ;;
        o ) DO_OVERWRITE=yes
            ;;
        g ) USE_GRID=yes
            ;;
        a ) FN_ARG_LIST=$OPTARG
            ;;
        l ) LINE_MAX=${OPTARG#*-} # eg. '20' or '11-20'
	    test $LINE_MAX != $OPTARG && LINE_MIN=${OPTARG%-*}
            ;;
    esac
done
shift $((OPTIND - 1))

if [ ! -f $FN_ARG_LIST ] ; then
    echo "!ERROR!  Cannot find the argument-list file '$FN_ARG_LIST'.  Abort."
    exit
fi

##
## Set up the working directory
##
mkdir -p $DIR_WORK
#chmod -R 01755 $DIR_WORK
test -e dir_$JOB_NAME || ln -s $DIR_WORK dir_$JOB_NAME

if [ $USE_GRID = yes ] ; then
    if ! which jobsub_submit &>/dev/null ; then
	echo "Command 'jobsub_submit' not found."
	echo "Forget 'source /e906/app/software/script/setup-jobsub-spinquest.sh'?"
	exit
    fi
fi

rm -rf   $DIR_WORK/input
mkdir -p $DIR_WORK/input
cd $DIR_INPUT
cp -a $DIR_SCRIPT/$FN_EXE_SIN $DIR_WORK/input
echo "---- input.tar.gz ------------"
tar czvf $DIR_WORK/input/input.tar.gz $LIST_INPUT_FILES
echo "------------------------------"

##
## Work on each argument
##
test $LINE_MAX -le 0 && LINE_MAX=$(cat $FN_ARG_LIST | wc -l)
I_LINE=0 # To be incremented
while read -a VARS ; do
    if [ $(( ++I_LINE )) -lt $LINE_MIN -o $I_LINE -gt $LINE_MAX ] ; then
	test $VERBOSE = yes && echo "Line $I_LINE:  Skip."
	continue
    fi
    echo "----------------------------------------------------------------"
    echo "Line $I_LINE: ${VARS[*]}."
    ID=${VARS[0]}
    if [ -z "$ID" ] ; then
	echo "Line $I_LINE seems empty.  Abort."
	exit
    fi
    if [ $DO_OVERWRITE = no -a -e $DIR_WORK/$ID ] ; then
	echo "Working directory exists.  Skip."
	continue
    fi

    rm -rf   $DIR_WORK/$ID
    mkdir -p $DIR_WORK/$ID/out
    #chmod -R 01755 $DIR_WORK/$ID

    if [ $USE_GRID = yes ] ; then
	CMD="jobsub_submit"
	CMD+=" -g --OS=SL7 --use_gftp --resource-provides=usage_model=$USAGE_MODEL -e IFDHC_VERSION --expected-lifetime='$LIFE_TIME'"
	CMD+=" --mail_never"
	CMD+=" -L $DIR_WORK/$ID/log_job.txt"
	CMD+=" -f $DIR_WORK/input/input.tar.gz"
	for (( II = 0 ; II < ${#VARS[*]} ; II++ )) ; do
	    if [ ${VARS[$II]:0:6} = '/pnfs/' ] ; then
		CMD+=" -f ${VARS[$II]}"
		VARS[$II]=$(basename ${VARS[$II]})
	    fi
	done
	CMD+=" -d OUTPUT $DIR_WORK/$ID/out"
	if [ -e $FN_CONF_NG_SITE ] ; then
	    while read SITE ; do
		CMD+=" --append_condor_requirements='(TARGET.GLIDEIN_Site isnt \"$SITE\")'"
	    done <$FN_CONF_NG_SITE
	fi
	
	CMD+=" file://$DIR_WORK/input/$FN_EXE_SIN ${VARS[*]}"
	test $VERBOSE = yes && echo "Jobsub command: $CMD"
	$CMD
	RET=$?
	if [ $RET -ne 0 ] ; then
	    echo "Jobsub returned $RET.  Abort."
	    exit
	fi
    else ## $USE_GRID = no
	mkdir -p $DIR_WORK/$ID/local
	cd       $DIR_WORK/$ID/local
	export  CONDOR_DIR_INPUT=$DIR_WORK/input
	export CONDOR_DIR_OUTPUT=$DIR_WORK/$ID/out
	$DIR_WORK/input/$FN_EXE_SIN ${VARS[*]} 2>&1 | tee $DIR_WORK/$ID/log_job.txt
    fi
done <$FN_ARG_LIST 2>&1 | tee log_execute_multiple.txt
