#########################################################################
# File Name: autoloopplayer.sh
# Author: eric.xu
# mail: eric.xu@libratone.com.cn
# Created Time: 2017-10-12 13:10:58
#########################################################################
#!/system/bin/sh

cnt=0

while [ $cnt -le 200 ]; do
    echo "autotest $cnt times....."
    testloopplayer /data/dingdang.wav &
    pid=$!
    sleep 30
    kill -9 $pid
    ((cnt += 1))
done

