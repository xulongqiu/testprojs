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

int main(int argc, const char* argv[])
{
    int16_t pcm[256] = {0}; 
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
    FILE* fout = fopen(argv[2], "w");
    //printf("open fout succ\n");
    if(!fout){
        printf("open out file error\n");
        fclose(fin);
        return 0;
    }
    if(fwrite(pcm, 2, 22, fout) > 0){
        //printf("write header succ\n");
    }
    do{
        cnt = fread(pcm, 2, 256, fin);
        if(cnt > 0){
            for(int i = 0; i < cnt; i++){
                pcm[i] *= 6;
            }
            fwrite(pcm, 2, cnt, fout);
        } 
    }while(cnt != 0);
    
    fclose(fin);
    fclose(fout); 
    return 0;
}
