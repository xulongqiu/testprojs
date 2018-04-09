/*************************************************************************
> File: audio_generator.c
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn
> Time:   2017-12-01 14:22:02
*************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

struct wav_header {
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t riff_fmt;
    uint32_t fmt_id;
    uint32_t fmt_sz;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_id;
    uint32_t data_sz;
};

static int get_wav_header(const char* filename, struct wav_header* header)
{
    FILE* fd = NULL;
    int len = 0;

    if(NULL == filename || NULL == header){
        return -1;
    }
    
    if ((fd = fopen(filename, "r")) == NULL) {
        return -1;
    }

    len = fread((void*)header, 1, sizeof(struct wav_header), fd);
    if(len == sizeof(struct wav_header)){
        return 0;
    }
    fclose(fd);
    return -1;
}

int main(int argc, char* argv[])
{
    struct wav_header header;

    if(argc < 2){
        printf("Usage: %s filename\n", argv[0]);
        return 0;
    }
    
    if(get_wav_header(argv[1], &header) == 0){
        printf("This file smaple_rate=%d, ch=%d\n", header.sample_rate, header.num_channels);
    }
    
    return 1;
}
