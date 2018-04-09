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
extern "C"{
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
    eRequestRetSuspendAi,
    eRequestRetNotAllow,
    eRequestRetResume,
    eRequestRetResumeAi,
    eRequestRetBackground,
    eRequestRetForeground,
    eRequestRetMax
} eRequestRet_t;

typedef enum {
    ePolicyStatusInit = 0,
    ePolicyStatusForeground,
    ePolicyStatusBackground,
    ePolicyStatusSuspendAi,
    ePolicyStatusResumeAi,
    ePolicyStatusSuspend,
    ePolicyStatusResume,
    ePolicyStatusNotAllow,
    ePolicyStatusMax
} ePolicyStatus_t;

typedef enum {
    eAudioSourcePriorityOne,
    eAudioSourcePriorityTwo,
    eAudioSourcePriorityThree,
    eAudioSourcePriorityFour,
    eAudioSourcePriorityFive,
    eAudioSourcePrioritySix,
    eAudioSourcePriorityMax
} eAudioSourcePriority_t;

typedef enum{
    eBackgroundVolumeZero = 0, // cur_volume -= cur_volume * 0
    eBackgroundVolumeFiveOne = 1, // cur_volume -= cur_volume / 1
    eBackgroundVolumeFiveTwo = 2,
    eBackgroundVolumeFiveThree = 3,
    eBackgroundVolumeFiveFour = 4,
    eBackgroundVolumeFull,
}eBackgroundVolume_t;

typedef enum{
    eSourceSuspendTypeSuspend = eRequestRetSuspend,
    eSourceSuspendTypeSuspendAi = eRequestRetSuspendAi
}eSourceSuspendType_t;

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
    eSourceSuspendType_t suspendType;
} tAudioSourceProperty_t;



#if 0
#define G_AUDIO_SOURCE_TYPE_CNT (7)
extern const tAudioSourceProperty_t gAudioSourcePropertyTable[G_AUDIO_SOURCE_TYPE_CNT];
#else
#define G_AUDIO_SOURCE_TYPE_CNT (sizeof(gAudioSourcePropertyTable)/sizeof(gAudioSourcePropertyTable[0]))

const tAudioSourceProperty_t gAudioSourcePropertyTable[] = {
    [0] = {
        eAudioSourceTypeLikeMusic,
        eAudioSourcePrioritySix,
        eAudioSourceTypeNull,
        eAudioSourceTypeNull,
        eAudioSourceTypeAll,
        eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic,
        eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeAlarm,
        eAudioSourceTypeLikeTone,
        eBackgroundVolumeFull,
        eSourceSuspendTypeSuspendAi
    },
    [1] = {
        eAudioSourceTypeLikeVrMusic,
        eAudioSourcePrioritySix,
        eAudioSourceTypeNull,
        eAudioSourceTypeNull,
        eAudioSourceTypeAll,
        eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic,
        eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeAlarm,
        eAudioSourceTypeLikeTone,
        eBackgroundVolumeFull,
        eSourceSuspendTypeSuspendAi
    },
    [2] = {
        eAudioSourceTypeLikeWakeUp,
        eAudioSourcePriorityFive,
        eAudioSourceTypeNull,
        eAudioSourceTypeLikeAlarm,
        eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeTone \
        | eAudioSourceTypeLikeVrMusic | eAudioSourceTypeLikeMusic,
        eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeVrMusic \
        | eAudioSourceTypeLikeRingTone | eAudioSourceTypeLikeCall \
        | eAudioSourceTypeLikeMusic,
        eAudioSourceTypeNull,
        eAudioSourceTypeLikeTone,
        eBackgroundVolumeFull,
        eSourceSuspendTypeSuspendAi
    },
    [3] = {
        eAudioSourceTypeLikeAlarm,
        eAudioSourcePriorityFour,
        eAudioSourceTypeNull,
        eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic,
        eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeTone,
        eAudioSourceTypeLikeAlarm,
        eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall,
        eAudioSourceTypeLikeTone,
        eBackgroundVolumeFull,
        eSourceSuspendTypeSuspend,
    },
    [4] = {
        eAudioSourceTypeLikeRingTone,
        eAudioSourcePriorityTwo,
        eAudioSourceTypeNull,
        eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic \
        | eAudioSourceTypeLikeAlarm,
        eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone \
        | eAudioSourceTypeLikeCall | eAudioSourceTypeLikeTone,
        eAudioSourceTypeLikeRingTone | eAudioSourceTypeLikeCall,
        eAudioSourceTypeLikeWakeUp,
        eAudioSourceTypeLikeTone,
        eBackgroundVolumeFull,
        eSourceSuspendTypeSuspend,
    },
    [5] = {
        eAudioSourceTypeLikeCall,
        eAudioSourcePriorityTwo,
        eAudioSourceTypeLikeWakeUp | eAudioSourceTypeLikeRingTone | eAudioSourceTypeLikeCall,
        eAudioSourceTypeLikeMusic | eAudioSourceTypeLikeVrMusic \
        | eAudioSourceTypeLikeAlarm,
        eAudioSourceTypeLikeTone,
        eAudioSourceTypeLikeCall,
        eAudioSourceTypeNull,
        eAudioSourceTypeLikeTone,
        eBackgroundVolumeFull,
        eSourceSuspendTypeSuspend,
    },
    [6] = {
        eAudioSourceTypeLikeTone,
        eAudioSourcePriorityOne,
        eAudioSourceTypeNull,
        eAudioSourceTypeNull,
        eAudioSourceTypeAll,
        eAudioSourceTypeNull,
        eAudioSourceTypeNull,
        eAudioSourceTypeNull,
        eBackgroundVolumeFull,
        eSourceSuspendTypeSuspendAi,
    }
};

static inline eAudioSourceType_t  mediaPolicySource2SourceType(int source)
{
    if(source == 0){
        return eAudioSourceTypeNull;
    }else{
        return eAudioSourceTypeMax;
    }
}


#endif

#ifdef __cplusplus
}
#endif

#endif
