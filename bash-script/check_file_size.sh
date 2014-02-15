#!/bin/bash

FILE=${1:?need filename}

if [ -f $FILE ]
then
    COUNT=`wc -l $FILE | awk '{print $1}'`
    if [ $COUNT -lt 100 ]
    then
        echo $FILE is a small file
    elif [ $COUNT -lt 1000 ]
    then
        echo $FILE is a normal file
    else
        echo $FILE is a big file
    fi
else
    echo $FILE doesn\'t exist
fi
