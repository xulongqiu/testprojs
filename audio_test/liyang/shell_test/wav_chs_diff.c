/*************************************************************************
> File: mic_file_check.c
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn 
> Time:   2017-08-03 22:09:08
*************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<fcntl.h>

static int my_abs(int a, int b)
{
    return (a > b? a - b : b - a);
}


int main(int argc, const char* argv[])
{
    uint16_t pcm[256] = {0}; 
    int cnt = 0;

    FILE* fin = fopen(argv[1], "r");
    if(!fin){
        printf("open error\n");
        return 0;
    }
    //printf("open fin succ\n");
    if(fread(pcm, 2, 22, fin) <= 0){
        printf("read hearder error \n");
        fclose(fin);
        return 0;
    }
    for(int n = 0; n < 10000; n++){
        int all = 0;
        cnt = fread(pcm, 2, 256, fin);
        while(cnt > 0){
           if(my_abs(pcm[cnt -1], 0xffff) > 500){
            all = 1;
           }
          cnt -= 1; 
        }
        if(all == 0){
            printf("checked silience, read bytes=%d\n", n * 256 * 2);
            break;
        }
    }
    uint64_t sum = 0;
    do{
        cnt = fread(pcm, 2, 256, fin);
        if(cnt > 0){
            sum = 0;
            for(int i = 0; i < cnt; i++){
                if(i % 8 == 0 && sum != 0){
                    sum /= 6;
                    for(int j = (i - 8); j < i; j++){
                        if(j % 8 != 1 && j % 8 != 5){
                            if(my_abs(pcm[j], sum) > 300){
                                printf("%s, channel:%d is diff:average=%lu, ch_val=%u, i=%d, j=%d\n", argv[1], (j % 8) + 1, sum, pcm[j], i, j);
                                goto fail;
                            }
                        }
                    } 
                    sum = 0;
                }
                if(i % 8 != 1 && i % 8 != 5){
                    sum += pcm[i];
                }
            }
        } 
    }while(cnt != 0);
    printf("%s, all channels are same\n", argv[1]);
fail: 
    fclose(fin);
    return 0;
}
