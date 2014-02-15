#!/bin/bash

DIR=${1:-.}

for file in $DIR/*.sh
do
    echo add x for $file
    chmod +x $file
done
