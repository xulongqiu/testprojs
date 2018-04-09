#########################################################################
# File Name: netadb.sh
# Author: eric.xu
# mail: eric.xu@libratone.com.cn
# Created Time: 2017-08-09 16:11:50
#########################################################################
#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 ip"
    exit 1
fi

g_ip=$1

function shellcmd(){
    adb disconnect $g_ip
    adb connect $g_ip
    sleep 1
    adb shell $1
}
shellcmd "ls /data"
