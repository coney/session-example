#!/bin/bash
EXPIRE=`date -v+5S +%s`
while [[ `date +%s` < $EXPIRE ]]
do
    echo left $(($EXPIRE - `date +%s`)) seconds
    sleep 1
done

echo timeout
