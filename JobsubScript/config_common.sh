# JOB_NAME:  Unique name for a set of jobs.
# Used as the output directory name, cf. $DIR_WORK.
JOB_NAME=job_20210208

# FN_ARG_LIST:  Name of the argument list.
# 
FN_ARG_LIST=list_argument.txt

# DIR_WORK: Name of the working directory for job submission.
# User need not change usually.  Must be a pnfs directory.
DIR_WORK=/pnfs/e1039/persistent/users/$USER/jobsub_script/$JOB_NAME

# DIR_INPUT: Directory that contains a set of input files.
# An absolute path or a relative path to "JobsubScript/".
DIR_INPUT=../AnaRealDst/macro

# LIST_INPUT_FILES: List of input files and directories.
# Relative to $DIR_INPUT.  Directories upper than $DIR_INPUT can be included but moved under $DIR_INPUT (e.g. "../inst" -> "$DIR_INPUT/inst").
LIST_INPUT_FILES='*.C list_spill_good.txt ../inst'

# USAGE_MODEL: The usage model in jobsub_submit.
# DEDICATED, OPPORTUNISTIC and/or OFFSITE
USAGE_MODEL=DEDICATED,OPPORTUNISTIC,OFFSITE

# LIFE_TIME: The life time in jobsub_submit.
# short (3h), medium (8h) or long (23h)
LIFE_TIME=short 

# FN_EXE_MAC:  Name of the script that runs ROOT macro with a single 
# parameter set.  User need not change usually.
FN_EXE_MAC=execute_macro.sh
