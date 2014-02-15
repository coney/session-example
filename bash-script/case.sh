#!/bin/bash

CODE=${1:?no http code specified}
case $CODE in
    200)
        echo "OK";;
    404)
        echo "Not Found";;
    502)
        echo "Bad Gateway";;
    *)
        echo "Undefined";;
    301)
        echo "Redirect";;
esac



