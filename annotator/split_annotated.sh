#!/bin/bash

audio=$1
timeline=$2

start=0
nb=1
curclass=0
cat $timeline |
while read first second; do
    if [ $nb -ne 1 ]
    then
        avconv -i $audio -ss $start -t `expr $first - $start` $curclass-$nb-$audio;
    fi;
    start=$first;
    curclass=$second;
    nb=$((nb + 1))
done;


