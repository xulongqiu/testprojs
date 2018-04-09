/*************************************************************************
> File: atoi.c
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn 
> Time:   2018-03-28 13:43:03
*************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

void main(void)
{
    const char* str = "ffffffff";
    int32_t num = 0; 
    
    sscanf(str, "%x", &num);
    printf("num=%d, sizeof(int)=%ld\n", num, sizeof(int));
}
