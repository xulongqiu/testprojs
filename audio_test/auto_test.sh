#! /system/bin/sh

audio_test /data/Power_On.wav -D 0 -d 0 -P -p 4096 -r 48000 -f 16 -c 2 -n 6&
sleep 2
audio_test /mnt/asec/pcm_out.wav -D 0 -d 1 -C -p 512 -r 16000 -f 16 -c 8 -n 8&
sleep 10
pkill audio_test
sleep 2
sync

