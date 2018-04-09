#########################################################################
# File Name: check.sh
# Author: eric.xu
# mail: eric.xu@libratone.com.cn
# Created Time: 2017-08-03 22:52:10
#########################################################################
#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 inpath outpath"
    exit 1
fi

inpath=$1
outpath=$2

i=0

for file in `ls $inpath/*.wav`;
do
    ((i+=1))
    filename=${file#*/}
    #echo $filename
    #if [ $i -eq 35 ]; then
        echo $filename 
        ./check.o $file $outpath/$filename
        i=0
    #fi
done
