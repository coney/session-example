TMPFILE=/tmp/accessiable
BADFILE=/oh/my/god
if touch $TMPFILE
then
    echo created $TMPFILE
fi

if ! touch $BADFILE &>/dev/null
then
    echo can\'t create $BADFILE
fi

if (echo "hello world" | grep "hello" &>/dev/null)
then 
    echo true
fi

echo "hello world" | grep "hello" &>/dev/null
if [ $? -eq 0 ]; then echo true; fi

if [[ -n `echo "hello world" | grep "hello"` ]]; then echo true; fi

[[ -n `echo "hello world" | grep "hello"` ]] && echo true

if brew update && brew upgrade 
then
    echo brew updated
fi


