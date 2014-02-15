#!/bin/bash

URL=http://w5cdn.ranktv.cn/dotnet/20130418/ueditor/dialogs/emotion/images/ac/%02d.gif

for ((i=1;i<=40;++i))
do
    wget `printf $URL $i` -O gif/$i.gif &
done
