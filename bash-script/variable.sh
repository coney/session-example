#!/bin/bash
#set -u

echo $UNDEFINED

# return hello if var1 is undefined
echo show var1: ${var1:-hello}
echo var1: $var1

# set var2 to hello and return var2 if var2 is undefined
echo show var2: ${var2:=hello}
echo var2: $var2

# return hello if var3 is defined
var3=dddddddd
echo show var3: ${var3:+hello}
echo var3: $var3

# show error message and exit if var3 is undefined
echo show var4: ${var4:?var4 is undefined}
echo var4: $var4
