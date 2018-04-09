/*************************************************************************
> File: policy.h
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn
> Time:   2018-03-29 19:11:10
*************************************************************************/
#ifndef __POLICY_H__
#define __POLICY_H__
#include<stdint.h>
#include<stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    eAudioSourceTypeNull = 0,
    /*sources: airply, udiskplayer, qplay, kaishu
     *point: Music can not intterupt wakeup, vrMusic can
     */
    eAudioSourceTypeLikeMusic = 0x01,
    /*vrMusic, news, long tts*/
    eAudioSourceTypeLikeVrMusic = 0x02,
    /*wakeup, short tts*/
    eAudioSourceTypeLikeWakeUp = 0x04,
    /*alarm*/
    eAudioSourceTypeLikeAlarm = 0x08,
    /*ring tone*/
    eAudioSourceTypeLikeRingTone = 0x10,
    /*call*/
    eAudioSourceTypeLikeCall = 0x20,
    /*tone, voice propmt*/
    eAudioSourceTypeLikeTone = 0x40,
    eAudioSourceTypeAll = 0x7f,
    eAudioSourceTypeMax
} eAudioSourceType_t;

typedef enum {
    eRequestRetAllow,
    eRequestRetSuspend,
    eRequestRetNotAllow,
    eRequestRetResume,
    eRequestRetBackground,
    eRequestRetForeground,
    eRequestRetMax
} eRequestRet_t;

typedef enum {
    eSourceStatusInit = 0,
    eSourceStatusForeground,
    eSourceStatusBackground,
    eSourceStatusSuspend,
    eSourceStatusMax
} eAudioSourceStatus_t;

typedef enum {
    eAudioSourcePriorityOne,
    eAudioSourcePriorityTwo,
    eAudioSourcePriorityThree,
    eAudioSourcePriorityFour,
    eAudioSourcePriorityFive,
    eAudioSourcePrioritySix,
    eAudioSourcePriorityMax
} eAudioSourcePriority_t;

typedef enum {
    eBackgroundVolumeZero = 0, // cur_volume -= cur_volume * 0
    eBackgroundVolumeFiveOne = 1, // cur_volume -= cur_volume / 1
    eBackgroundVolumeFiveTwo = 2,
    eBackgroundVolumeFiveThree = 3,
    eBackgroundVolumeFiveFour = 4,
    eBackgroundVolumeFull,
} eBackgroundVolume_t;

typedef struct {
    eAudioSourceType_t self;
    eAudioSourcePriority_t lvl;
    uint32_t notAllow;
    uint32_t suspend;
    uint32_t allow;
    uint32_t notAllowSelf;
    uint32_t suspendSelf;
    uint32_t backgroundSelf;
    eBackgroundVolume_t backVolumeLvl;
} tAudioSourceProperty_t;

#define G_AUDIO_SOURCE_TYPE_CNT (sizeof(gAudioSourcePropertyTable)/sizeof(gAudioSourcePropertyTable[0]))

const tAudioSourceProperty_t gAudioSourcePropertyTable[] = {
    [0] = {
        .lvl = eAudioSourcePrioritySix,
        .self = eAudioSourceTypeLikeMusic,
        .notAllow = eAudioSourceTypeNull,
        .suspend = eAudioSourceTypeNull,
        .allow = eAudioSourceTypeAll,
        .notAllowSelf = eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic,
        .suspendSelf = eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeAlarm,
        .backgroundSelf = eAudioSourceTypeLikeTone,
        .backVolumeLvl = eBackgroundVolumeFull,
        .suspendType = eSourceSuspendTypeSuspendAi
    },
    [1] = {
        .lvl = eAudioSourcePrioritySix,
        .self = eAudioSourceTypeLikeVrMusic,
        .notAllow = eAudioSourceTypeNull,
        .suspend = eAudioSourceTypeNull,
        .allow = eAudioSourceTypeAll,
        .notAllowSelf = eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic,
        .suspendSelf = eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeAlarm,
        .backgroundSelf = eAudioSourceTypeLikeTone,
        .backVolumeLvl = eBackgroundVolumeFull,
        .suspendType = eSourceSuspendTypeSuspend
    },
    [2] = {
        .lvl = eAudioSourcePrioritySix,
        .self = eAudioSourceTypeLikeWakeUp,
        .notAllow = eAudioSourceTypeNull,
        .suspend = eAudioSourceTypeLikeAlarm,
        .allow = eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeTone \
        | eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic,
        .notAllowSelf = eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeVrMusic \
        | eAudioSourceTypeLikeRingTone | eAudioSourceTypeLikeCall \
        | eAudioSourceTypeLikeMusic,
        .suspendSelf =  eAudioSourceTypeNull,
        .backgroundSelf = eAudioSourceTypeLikeTone,
        .backVolumeLvl = eBackgroundVolumeFull,
        .suspendType = eSourceSuspendTypeSuspendAi
    },
    [3] = {
        .lvl = eAudioSourcePriorityFour,
        .self = eAudioSourceTypeLikeAlarm,
        .notAllow = eAudioSourceTypeNull,
        .suspend = eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic,
        .allow = eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeTone,
        .notAllowSelf = eAudioSourceTypeLikeAlarm,
        .suspendSelf =  eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall,
        .backgroundSelf = eAudioSourceTypeLikeTone,
        .backVolumeLvl = eBackgroundVolumeFull,
        .suspendType = eSourceSuspendTypeSuspend
    },
    [4] = {
        .lvl = eAudioSourcePriorityThree,
        .self = eAudioSourceTypeLikeRingTone,
        .notAllow = eAudioSourceTypeNull,
        .suspend = eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic \
        | eAudioSourceTypeLikeAlarm,
        .allow = eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeTone,
        .notAllowSelf = eAudioSourceTypeLikeRingTone | eAudioSourceTypeLikeCall,
        .suspendSelf =  eAudioSourceTypeLikeWakeUp,
        .backgroundSelf = eAudioSourceTypeLikeTone,
        .backVolumeLvl = eBackgroundVolumeFull,
        .suspendType = eSourceSuspendTypeSuspend
    },
    [5] = {
        .lvl = eAudioSourcePriorityTwo,
        .self = eAudioSourceTypeLikeCall,
        .notAllow = eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone | eAudioSourceTypeLikeCall,
        .suspend = eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic \
        | eAudioSourceTypeLikeAlarm,
        .allow = eAudioSourceTypeLikeTone,
        .notAllowSelf = eAudioSourceTypeLikeCall,
        .suspendSelf =  eAudioSourceTypeNull,
        .backgroundSelf = eAudioSourceTypeLikeTone,
        .backVolumeLvl = eBackgroundVolumeFull,
        .suspendType = eSourceSuspendTypeSuspend
    },
    [6] = {
        .lvl = eAudioSourcePrioritySix,
        .self = eAudioSourceTypeLikeTone,
        .notAllow = eAudioSourceTypeNull,
        .suspend = eAudioSourceTypeNull,
        .allow = eAudioSourceTypeAll,
        .notAllowSelf = eAudioSourceTypeNull,
        .suspendSelf =  eAudioSourceTypeNull,
        .backgroundSelf = eAudioSourceTypeNull,
        .backVolumeLvl = eBackgroundVolumeFull,
        .suspendType = eSourceSuspendTypeSuspendAi
    }
};

#ifdef __cplusplus
}
#endif

#endif
