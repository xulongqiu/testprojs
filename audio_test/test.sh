#########################################################################
# File Name: nandtest.sh
# Author: eric.xu
# mail: eric.xu@libratone.com.cn
# Created Time: 2017-07-27 17:30:19
#########################################################################
#!/bin/bash

while [ 1 ];
do
    echo "adb player 1"
    adb shell setprop libratone.dsp.bypass 1
    adb shell auto_test.sh
    sleep 2
    echo "adb pull file"
    mydata=$(date +%Y%m%d%H%M%S)
    echo ${mydata}
    adb pull  /mnt/asec/pcm_out.wav ${mydata}.wav
    adb shell sync
    ((cnt += 1))
    echo "======finish test $cnt times======"    
    
    echo "reboot target "
    #adb shell reboot
    sleep 30
done
