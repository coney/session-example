#!/bin/bash
function send_request {
  echo send request $1+$2
  time echo receive reply $1+$2=`curl -s localhost:8000/$1+$2`
}

COUNT=${1:-5}

for ((i=1;i<=$COUNT;++i)); do
  send_request $i ${i}0 &
  sleep 0.01
done
