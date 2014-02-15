#!/bin/sh
for pid in `ps aux | grep sleep |grep -v grep | awk '{print $2}'`
do
    echo killing $pid
    kill $pid
done

sleep 1
ps aux | grep sleep | grep -v grep
