#ifndef __GOER_AUDIO_TEST_H__
#define __GOER_AUDIO_TEST_H__

#include <system/audio.h>
#include <hardware/hardware.h>
#include <system/audio.h>
#include <hardware/audio.h>

#include <tinyalsa/asoundlib.h>
#include <audio_utils/resampler.h>


#ifdef __cplusplus
extern "C" {
#endif

#define LOCAL_FAR_PIPE 1
#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

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
	
struct riff_wave_header {
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t wave_id;
};

struct chunk_header {
    uint32_t id;
    uint32_t sz;
};

struct chunk_fmt {
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
};
	

struct wav_file{
   FILE *file;
   struct wav_header *header;
   uint32_t file_frames;
};
	
struct wav_file * create_wav_file(struct pcm_config pcm_config, char * file_name);
uint32_t write_wav_file(struct wav_file *, void * buffer, ssize_t frames);
void release_wav_file(struct wav_file *pfile);
	
#ifdef __cplusplus
}
#endif
#endif

