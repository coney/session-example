#!/bin/bash
function send_request {
  echo send request $1+$2
  time echo receive reply $1+$2=`curl -s localhost:8000/$1+$2`
}
for ((i=1;i<=5;++i)); do
  send_request $i ${i}0 &
done
