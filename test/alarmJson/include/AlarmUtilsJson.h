#ifndef ANDROID_ALARMUTILSJSON_H
#define ANDROID_ALARMUTILSJSON_H

#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include "cJSON.h"
#include "AlarmParas.h"

namespace android
{
#define SUPPORT_TENCENT_INTERFACE 1
#define SUPPORT_BAIDU_INTERFACE 1

#ifdef SUPPORT_TENCENT_INTERFACE
enum DOBBY_CALENDAR_ACTION {
    DOBBY_CALENDAR_ACTION_ADD_CALENDAR   =   2,
    DOBBY_CALENDAR_ACTION_DEL_CALENDAR   =   3,
    DOBBY_CALENDAR_ACTION_CHECK_CALENDAR =   4,
    DOBBY_CALENDAR_ACTION_MODIFY_CALENDAR =  5
};

enum enumTimeRepeateBit {
    ALARM_REPEATE_BIT_MONDAY    =   0x0001,     // ÿ��һ������
    ALARM_REPEATE_BIT_TUESDAY   =   0x0002,
    ALARM_REPEATE_BIT_WEDNESDAY =   0x0004,
    ALARM_REPEATE_BIT_THURSDAY  =   0x0008,
    ALARM_REPEATE_BIT_FRIDAY    =   0x0010,
    ALARM_REPEATE_BIT_SATURDAY  =   0x0020,
    ALARM_REPEATE_BIT_SUNDAY    =   0x0040,
    ALARM_REPEATE_PER_MONTH     =   0x0080,     // �ӵ�һ������ʱ�俪ʼ���������������ظ�
    ALARM_REPEATE_PER_YEAR      =   0x0100      // �ӵ�һ������ʱ�俪ʼ���������������ظ�
};
#endif

class AlarmUtilsJson
{
public:
    AlarmUtilsJson(const char* jsonString);
    AlarmUtilsJson();
    ~AlarmUtilsJson();
    void onFirstRef();
#ifdef SUPPORT_TENCENT_INTERFACE
    int jsonToAlarmsTencent(AlarmParaT* vecAlarm);
#endif
#ifdef SUPPORT_BAIDU_INTERFACE
    int jsonToAlarmsBaidu(AlarmParaT* vecAlarm);
#endif
    int jsonToAlarmsLibratone(AlarmParaT* vecAlarm);
    void jsonRelease(void);
    char* alarmsToJsonString(AlarmParaT* vecAlarms);

private:
    cJSON* mJsonP;
};

};

#endif// ANDROID_ALARMUTILSJSON_H
