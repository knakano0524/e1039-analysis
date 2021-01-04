#!/bin/bash

N65=02
N43=$1

ls /pnfs/e906/production/digit/R009/$N65/$N43/digit_*_009.root | \
while read FNAME ; do
    ID=$(basename $FNAME | cut -c 8-12)
    echo "$ID $FNAME"
done >list_argument_${N65}${N43}00.txt

# seq $SEQ_ID_MIN $SEQ_ID_MAX >$FN_ARG
#cut -c 49-53 list_input_file.txt | paste - list_input_file.txt >list_argument.txt
