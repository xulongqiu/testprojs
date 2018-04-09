#define LOG_TAG "TestWavFile"

#include <cutils/log.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include "audio_file.h"
#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1


/**
 * @brief  create wave file and seek to the data zone of the file
 *
 *
 * @param pcm_config  the param of the pcm stream
 * @param file_name the file_name to create
 *
 * @return struct wav_file
 */
struct wav_file * create_wav_file(struct pcm_config pcm_config, char * file_name)
{
	struct wav_file *pfile = NULL;
	struct wav_header *pheader = NULL;

	pheader = (struct wav_header *)malloc(sizeof(struct wav_header));
	if(!pheader)
	{
		ALOGE("malloc error --> create wav file fail");
		return 0;
	}
	pfile = (struct wav_file *)malloc(sizeof(struct wav_file));
	if(!pfile)
	{
		free(pheader);
		ALOGE("malloc error --> create wav file fail");
		return 0;
	}

	pheader->riff_id = ID_RIFF;
	pheader->riff_sz = 0;
	pheader->riff_fmt = ID_WAVE;
	pheader->fmt_id = ID_FMT;
	pheader->fmt_sz = 16; //must default 0x00000010
	pheader->audio_format = FORMAT_PCM;
	pheader->num_channels = pcm_config.channels;
	pheader->sample_rate = pcm_config.rate;
	if(pcm_config.format == PCM_FORMAT_S16_LE)
		pheader->bits_per_sample = 16;
	else if(pcm_config.format == PCM_FORMAT_S32_LE)
		pheader->bits_per_sample = 32;
	pheader->byte_rate = (pheader->bits_per_sample / 8) * pheader->num_channels * pheader->sample_rate;
	pheader->block_align = pheader->num_channels * (pheader->bits_per_sample / 8);
	pheader->data_id = ID_DATA;
	
	switch(pcm_config.rate) {
	case 8000:
	case 11025:
	case 16000:
	case 22050:
	case 24000:
	case 32000:
	case 44100:
	case 48000:
	case 96000:
	case 192000:
		break;
	default:
		ALOGE("sample_rate error --> create wav file fail");
		return 0;
	}


	pfile->file = fopen(file_name,"wb");
	if(!pfile->file)
	{
		free(pheader);
		free(pfile);
		ALOGE("fopen error --> create wav file fail");
		return 0;
	}

	pfile->header = pheader;
	pfile->file_frames = 0;

	fseek(pfile->file, sizeof(struct wav_header), SEEK_SET);
	return pfile;
}

uint32_t write_wav_file(struct wav_file *pfile, void * buffer, ssize_t frames)
{
	ssize_t frames_w;

	if(!pfile || !buffer)
	{
		ALOGE("wav_file is NULL");
		return 0;
	}

	frames_w = fwrite(buffer,pfile->header->block_align, frames, pfile->file);
	if(frames != frames_w)
	{
		ALOGE("fwrite error!!");

	}
	pfile->file_frames += frames_w;
	return frames_w;
}


static void add_wav_header(FILE *file, struct wav_header *pheader, uint32_t frames_num)
{

	//pheader->data_sz = size * pheader->block_align;
	//pheader->riff_sz = pheader->data_sz + sizeof(resultHeader) - 8;
	pheader->data_sz = frames_num * pheader->block_align;
	pheader->riff_sz = pheader->data_sz + sizeof(pheader) - 8;
#if 1	
	ALOGD("pheader->riff_id = 0x%08x\n", pheader->riff_id);
	ALOGD("pheader->riff_sz = 0x%08x\n", pheader->riff_sz);
	ALOGD("pheader->riff_fmt = 0x%08x\n", pheader->riff_fmt);
	ALOGD("pheader->fmt_id = 0x%08x\n", pheader->fmt_id);
	ALOGD("pheader->fmt_sz = 0x%08x\n", pheader->fmt_sz);
	ALOGD("pheader->audio_format = 0x%04x\n", pheader->audio_format);
	ALOGD("pheader->num_channels = 0x%04x\n", pheader->num_channels);
	ALOGD("pheader->sample_rate = 0x%08x\n", pheader->sample_rate);
	ALOGD("pheader->byte_rate = 0x%08x\n", pheader->byte_rate);
	ALOGD("pheader->block_align = 0x%08x\n", pheader->block_align);
	ALOGD("pheader->bits_per_sample = 0x%04x\n", pheader->bits_per_sample);
	ALOGD("pheader->data_id = 0x%08x\n", pheader->data_id);
	ALOGD("pheader->data_sz = 0x%08x\n", pheader->data_sz);
#endif
	fseek(file, 0, SEEK_SET);
	fwrite(pheader, sizeof(struct wav_header), 1, file);
}

void release_wav_file(struct wav_file *pfile)
{
	if(pfile)
	{
		if(pfile->file && pfile->header)
		{
			ALOGD("pfile->file_frames = %d\n", pfile->file_frames);
			add_wav_header(pfile->file, pfile->header, pfile->file_frames);
			fclose(pfile->file);
			free(pfile->header);
			free(pfile);
		}
	}
}
