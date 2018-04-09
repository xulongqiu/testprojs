#define LOG_NDEBUG 0
#define LOG_TAG "AlarmUtilsJson"

#include "AlarmUtilsJson.h"
#include <string.h>
#include <time.h>
#include <string>

using namespace std;

namespace android
{
#define ALOGV printf
#define ALOGE printf

AlarmUtilsJson::AlarmUtilsJson(const char* jsonString)
{
    if (jsonString != NULL) {
        mJsonP = cJSON_Parse(jsonString);
    } else {
        ALOGV("AlarmUtilsJson, jsonString=NULL");
    }

    ALOGV("AlarmUtilsJson created\n");
}

AlarmUtilsJson::AlarmUtilsJson()
{
    mJsonP = cJSON_CreateObject();

    ALOGV("AlarmUtilsJson created\n");
}

AlarmUtilsJson::~AlarmUtilsJson(void)
{
    if (mJsonP != NULL) {
        cJSON_Delete(mJsonP);
        mJsonP = NULL;
    }

    ALOGV("AlarmUtilsJson destroyed\n");
}

void AlarmUtilsJson::onFirstRef()
{
    ALOGV("AlarmUtilsJson::onFirstRef(%p)\n", this);
}

void AlarmUtilsJson::jsonRelease(void)
{
    if (mJsonP != NULL) {
        cJSON_Delete(mJsonP);
        mJsonP = NULL;
    }
}

#ifdef SUPPORT_TENCENT_INTERFACE

typedef struct slotNote {
    string ori_txt;
    string txt;
} slotNoteT;

typedef struct slotBasicDt {
    string date;
    string time;
    int year;
    char mon;
    char day;
    char hour;
    char min;
    char sec;
    char week;
    string ori_txt;
    char peroidOfDay;

} slotBasicDtT;

typedef struct slotInterval {
    slotBasicDtT end;
    slotBasicDtT start;
} slotIntervalT;

typedef enum slotRepeatType {
    repeatNone,
    repeatYear,
    repeatMon,
    repeatWeek,
    repeatDay,
    repeatHour,
    repeatMin,
    repeatWorkday,
    repeatWeekend

} slotRepeatTypeT;

typedef struct slotRepeat {
    slotIntervalT interval;
    slotRepeatTypeT type;
} slotRepeatT;

typedef enum slotDtType {
    typeNone,
    typeInterval,
    typeDatatime,
    typeDuration,
    typeRepeatDt

} slotDtTypeT;

typedef struct slotDataTime {
    slotBasicDtT datetime;
    slotIntervalT interval;
    slotRepeatT repeat;
    string ori_txt;
    slotDtTypeT type;

} slotDataTimeT;

typedef struct slot {
    string alarmOper;
    slotNoteT note;
    slotDataTime date;
    slotDataTime time;
    string sceneName;
    string spk;
    string tts;
} slotT;

struct action2Oper {
    uint8_t oper;
    const char* str;
};

struct action2Oper oper_table[ALARM_OPER_MAX] = {
    {.oper = ALARM_ADD, .str = "new"},
    {.oper = ALARM_REMOVE, .str = "delete"},
    {.oper = ALARM_MODIFY, .str = "modify"},
    {.oper = ALARM_CHECK, .str = "check"},
    {.oper = ALARM_CANCEL, .str = "cancel"},
    {.oper = ALARM_TURNON, .str = "on"},
    {.oper = ALARM_TURNOFF, .str = "off"},
};

typedef enum {
    tencentErrOK,
    tencentNoSupport,
    tencentJsonStrNull,
    tencentJsonParseErr,
    tencentSceneDataNull,
    tencentJsonSlotsNull,
    tencentJsonSlotsZero,
    tencentJsonSlotStringNull,
    tencentOPerTypeErr,
    tencentNoStartDate,
    tencentNoStartTime,
    tencentNoEndDate,
    tencentNoEndTime,
    tencentParseDtErr,
    tencentParseSlotErr,
} tencentErrCodeT;

static int jsonParseBasicDt(cJSON* subJsonP, slotBasicDtT* datetime)
{
    cJSON* obj;

    if (subJsonP == NULL || datetime == NULL) {
        return -1;
    }

    obj = cJSON_GetObjectItem(subJsonP, "date");

    if (obj && strlen(obj->valuestring)) {
        datetime->date.append(obj->valuestring);
    }

    obj = cJSON_GetObjectItem(subJsonP, "time");

    if (obj && strlen(obj->valuestring)) {
        datetime->time.append(obj->valuestring);
    }

    obj = cJSON_GetObjectItem(subJsonP, "year");
    datetime->year = obj ? obj->valueint : 0;
    obj = cJSON_GetObjectItem(subJsonP, "week");
    datetime->week = obj ? obj->valueint : 0;
    obj = cJSON_GetObjectItem(subJsonP, "mon");
    datetime->mon = obj ? obj->valueint : 0;
    obj = cJSON_GetObjectItem(subJsonP, "day");
    datetime->day = obj ? obj->valueint : 0;
    obj = cJSON_GetObjectItem(subJsonP, "hour");
    datetime->hour = obj ? obj->valueint : 0;
    obj = cJSON_GetObjectItem(subJsonP, "min");
    datetime->min = obj ? obj->valueint : 0;
    obj = cJSON_GetObjectItem(subJsonP, "sec");
    datetime->sec = obj ? obj->valueint : 0;
    obj = cJSON_GetObjectItem(subJsonP, "original_text");

    if (obj && strlen(obj->valuestring)) {
        datetime->ori_txt.append(obj->valuestring);
    }

    obj = cJSON_GetObjectItem(subJsonP, "period_of_day");
    datetime->peroidOfDay = obj ? obj->valueint : 0;

    return 0;
}

static int jsonParseSlot(const char* str, slotT* slot, const char* slot_name)
{
    cJSON* jsonP, *subJsonP, *obj, *subInterval;
    int ret = 0;
    slotDataTimeT* dtP = NULL;

    if (str == NULL || slot == NULL || slot_name == NULL) {
        return -1;
    }

    if ((jsonP = cJSON_Parse(str)) == NULL) {
        return -2;
    }
    
    printf("[%s]:%s\n\n", slot_name, cJSON_Print(jsonP));
    if (!strcmp(slot_name, "alarm")) {
        obj = cJSON_GetObjectItem(jsonP, "original_text");

        if (obj && strlen(obj->valuestring)) {
            slot->note.ori_txt.append(obj->valuestring);
        }

        obj = cJSON_GetObjectItem(jsonP, "text");

        if (obj && strlen(obj->valuestring)) {
            slot->note.txt.append(obj->valuestring);
        }

        cJSON_Delete(jsonP);
        return 0;
    }

    if (!strcmp(slot_name, "date")) {
        dtP = &slot->date;
    } else if (!strcmp(slot_name, "time")) {
        dtP = &slot->time;
    }

    if (dtP == NULL) {
        ret = -3;
        goto err;
    }
    //datetime
    subJsonP = cJSON_GetObjectItem(jsonP, "datetime");
    ret = jsonParseBasicDt(subJsonP, &dtP->datetime);

    if (ret) {
        goto err;
    }

    //interval
    subInterval = cJSON_GetObjectItem(jsonP, "interval");

    if (subJsonP == NULL) {
        ret = -4;
        goto err;
    }

    obj = cJSON_GetObjectItem(subInterval, "end");
    ret = jsonParseBasicDt(obj, &dtP->interval.end);

    if (ret) {
        goto err;
    }

    obj = cJSON_GetObjectItem(subInterval, "start");
    ret = jsonParseBasicDt(obj, &dtP->interval.start);

    if (ret) {
        goto err;
    }

    //original_text
    obj = cJSON_GetObjectItem(jsonP, "original_text");
    if(obj && strlen(obj->valuestring)){
        dtP->ori_txt.append(obj->valuestring);
    }

    //repeat
    subJsonP = cJSON_GetObjectItem(jsonP, "repeat");

    if (subJsonP == NULL) {
        ret = -4;
        goto err;
    }

    subInterval = cJSON_GetObjectItem(subJsonP, "interval");

    if (subJsonP == NULL) {
        ret = -4;
        goto err;
    }

    obj = cJSON_GetObjectItem(subInterval, "end");
    ret = jsonParseBasicDt(obj, &dtP->repeat.interval.end);

    if (ret) {
        goto err;
    }

    obj = cJSON_GetObjectItem(subInterval, "start");
    ret = jsonParseBasicDt(obj, &dtP->repeat.interval.start);

    if (ret) {
        goto err;
    }

    obj = cJSON_GetObjectItem(subJsonP, "repeat_datetime_type");
    dtP->repeat.type = (slotRepeatTypeT)(obj ? obj->valueint : 0);

    //type
    obj = cJSON_GetObjectItem(jsonP, "type");
    dtP->type = (slotDtTypeT)(obj ? obj->valueint : 0);


err:
    cJSON_Delete(jsonP);
    return ret;
}

static int jsonParseSemantic(const char* str, slotT* slot)
{
    cJSON* jsonP, *subJsonP, *obj, *subSlots, *slotItem;
    const char* slotName = NULL;
    int slotCnt = 0, i = 0;
    int ret = 0;

    if (str == NULL || slot == NULL) {
        return -1;
    }

    if ((jsonP = cJSON_Parse(str)) == NULL) {
        return -2;
    }

    subJsonP = cJSON_GetObjectItem(jsonP, "semantic");
    printf("semantic=%s\n", cJSON_Print(subJsonP));

    if (subJsonP == NULL) {
        ret = -3;
        goto err;
    }

    subSlots = cJSON_GetObjectItem(subJsonP, "slots");

    if (subSlots == NULL) {
        ret = -3;
        goto err;
    }

    slotCnt = cJSON_GetArraySize(subSlots);

    for (i = 0; i < slotCnt; i++) {
        slotItem = cJSON_GetArrayItem(subSlots, i);

        if (slotItem == NULL) {
            ret = -3;
            goto err;
        }

        obj = cJSON_GetObjectItem(slotItem, "name");
        slotName = obj ? obj->valuestring : NULL;

        obj = cJSON_GetObjectItem(slotItem, "slot_string");

        if (obj == NULL) {
            ret = -4;
            goto err;
        }

        ret = jsonParseSlot(obj->valuestring, slot, slotName);
    }

err:
    cJSON_Delete(jsonP);
    return ret;
}


static unsigned long str2SecTime(const char* szTime)
{
    tm tm_;
    time_t t_;

    strptime(szTime, "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间
    tm_.tm_isdst = -1;
    t_  = mktime(&tm_);                         //将tm时间转换为秒时间
    return (unsigned long)t_;
}

uint8_t getWeekFromSecs(unsigned long secs)
{
    struct timeval tv;
    struct tm* local;

    tv.tv_sec = secs;
    local = localtime(&tv.tv_sec);
    return (local->tm_wday == 6? 0: (local->tm_wday - 1));
}

static int slot2AlarmParaT(slot* slot, AlarmParaT* alarm)
{
    string timeOri;
    int i = 0;
    string startDate, startTime, endDate, endTime;
    string firstTime, lastTime;

    memset(alarm, 0, sizeof(AlarmParaT));

    //alarmName
    if (!slot->note.ori_txt.empty()) {
        strncpy(alarm->alarmName, slot->note.ori_txt.data(), ALARM_NAME_LEN);
    } else if (!slot->note.txt.empty()) {
        strncpy(alarm->alarmName, slot->note.txt.data(), ALARM_NAME_LEN);
    }

    //alarmOperation
    if (!slot->alarmOper.empty()) {
        for (i = 0; i < ALARM_OPER_MAX; i++) {
            if (!strcmp(slot->alarmOper.data(), oper_table[i].str)) {
                alarm->alarmOperation = oper_table[i].oper;
                ALOGV("[%s]:alarmOPeration=%d, aciton:%s\n", __FUNCTION__, oper_table[i].oper, oper_table[i].str);
                break;
            }
        }
    }

    if (i >= ALARM_OPER_MAX) {
        ALOGV("[%s]:alarmOperation can't parse, action:%s\n", __FUNCTION__, slot->alarmOper.data());
        return 0;
    }

    //alarmTimeOri
    timeOri.append(slot->date.ori_txt);
    timeOri.append(slot->time.ori_txt);

    if (!timeOri.empty()) {
        strncpy(alarm->alarmTimeOri, timeOri.data(), ALARM_TIMEORI_LEN);
    }

    //alarmTTS
    if(!slot->tts.empty()){
        strncpy(alarm->alarmTTS, slot->tts.data(), ALARM_TTS_LEN_MAX);
    }

    //alarmTime
    //firstTime
    if (!slot->date.datetime.date.empty()) {
        startDate.append(slot->date.datetime.date.data());
    }  else if (!slot->date.repeat.interval.start.date.empty()) {
        startDate.append(slot->date.repeat.interval.start.date.data());
    } else if (!slot->date.interval.start.date.empty()) {
        startDate.append(slot->date.interval.start.date.data());
    } else {
        ALOGV("[%s]:start date is empty\n", __FUNCTION__);
        return -tencentNoStartDate;
    }
    
    if ((!startDate.empty()) && strlen(startDate.data()) > 10) {
        ALOGV("startDate format error:%s\n", startDate.data());
        return -tencentNoStartDate;
    }

    firstTime.append(startDate);
    firstTime.append(" ");

    if (!slot->time.datetime.time.empty()) {
        startTime.append(slot->time.datetime.time.data());
    } else if (!slot->time.repeat.interval.start.date.empty()) {
        startTime.append(slot->time.repeat.interval.start.date.data());
    } else if (!slot->time.interval.start.time.empty()) {
        startTime.append(slot->time.interval.start.time.data());
    } else {
        ALOGV("[%s]:start time is empty\n", __FUNCTION__);
        return -tencentNoStartTime;
    }

    if (! startTime.empty() && strlen(startTime.data()) > 8) {
        ALOGV("startTime format error:%s\n", startTime.data());
        return -tencentNoStartTime;
    }

    firstTime.append(startTime);
    ALOGV("[%s]:start datetime:%s\n", __FUNCTION__, firstTime.data());
    alarm->alarmTime.firstTime = str2SecTime(firstTime.data());

    //endTime
    if (!slot->date.interval.end.date.empty()) {
        endDate.append(slot->date.interval.end.date.data());
    } else if (!slot->date.repeat.interval.end.date.empty()) {
        endDate.append(slot->date.repeat.interval.end.date.data());
    } else {
        endDate.append("2500-12-31");
        ALOGV("[%s]:end date is empty\n", __FUNCTION__);
    }

    if (!endDate.empty() && strlen(endDate.data()) > 10) {
        ALOGV("endDate format error:%s\n", endDate.data());
        return -tencentNoEndDate;
    }

    lastTime.append(endDate);
    lastTime.append(" ");

    if (!slot->time.interval.end.time.empty()) {
        endTime.append(slot->time.interval.end.time.data());
    } else if (!slot->time.repeat.interval.end.time.empty()) {
        endTime.append(slot->time.repeat.interval.end.time.data());
    } else {
        endTime.append("23:59:59");
        ALOGV("[%s]:end time is empty\n", __FUNCTION__);
    }

    if (!endTime.empty() && strlen(endTime.data()) > 8) {
        ALOGV("endTime format error:%s\n", endTime.data());
        return -tencentNoEndTime;
    }

    lastTime.append(endTime);
    ALOGV("[%s]:end datetime:%s\n", __FUNCTION__, lastTime.data());
    alarm->alarmTime.endTime = str2SecTime(lastTime.data());

    //repeat
    alarm->alarmTime.repeatInfo = 0;

    if (slot->date.repeat.type == repeatYear) {
        alarm->alarmTime.repeatInfo = 0;//ALARM_REPEATE_YEAR;
    } else if (slot->date.repeat.type == repeatMon) {
        alarm->alarmTime.repeatInfo |= ALARM_REPEATE_MONTH;
    } else if (slot->date.repeat.type == repeatWeek) {
        alarm->alarmTime.repeatInfo = (1 << getWeekFromSecs(alarm->alarmTime.firstTime));
    } else if (slot->date.repeat.type == repeatDay) {
        if(slot->date.repeat.interval.start.peroidOfDay == 5){
            alarm->alarmTime.repeatInfo = ALARM_REPEATE_WORKDAY;
        }else{
            alarm->alarmTime.repeatInfo |= ALARM_REPEAT_DAY;
        }
    } else if (slot->date.repeat.type == repeatWorkday) {
        alarm->alarmTime.repeatInfo = ALARM_REPEATE_WORKDAY;
    } else if (slot->date.repeat.type == repeatWeekend) {
        alarm->alarmTime.repeatInfo = ALARM_REPEATE_WORKEND;
    }

    if (slot->time.repeat.type == repeatHour || slot->time.repeat.type == repeatMin) {
        alarm->alarmTime.repeatInfo = 0;
    }

    //handle alarm time
    alarm->alarmTime.firstTime *= 1000L;
    alarm->alarmTime.endTime *= 1000L;
    alarm->alarmTime.interval = ALARM_INTERVAL_DEFAULT * 1000L;
    alarm->alarmTime.repeatCnt = ALARM_REPEAT_CNT_DEFAULT;

    //intent
    if(!slot->sceneName.empty()){
        strncpy(alarm->alarmIntent[0].name, slot->sceneName.data(), ALARM_INTENT_NAME_LEN);
    }
    alarm->alarmIntent[0].code = ALARM_INTENT_PLAY;
    alarm->alarmIntent[0].subCode = ALARM_PLAY_LOCAL_FILE;
    strncpy(alarm->alarmIntent[0].content, ALARM_AUDIO_DEFULAT, ALARM_INTENT_CONTENT_LEN);

    //alarmID
    if (alarm->alarmID == 0) {
        alarm->alarmID = ALARM_ID(alarm->alarmTime.firstTime, alarm->alarmTime.repeatInfo);
    }

    return tencentErrOK;
}

int AlarmUtilsJson::jsonToAlarmsTencent(AlarmParaT* alarm)
{
    slotT slot;
    cJSON* subJsonP, *obj;
    int ret = 0;

    if (mJsonP == NULL) {
        ALOGE("mJsonP is NULL \n");
        return -1;
    }

    memset(&slot, 0, sizeof(slotT));

    subJsonP = cJSON_GetObjectItem(mJsonP, "res");

    if (subJsonP == NULL) {
        return -2;
    }

    subJsonP = cJSON_GetObjectItem(subJsonP, "scene");

    if (subJsonP == NULL) {
        return -2;
    }

    obj = cJSON_GetObjectItem(subJsonP, "action");

    if (obj == NULL) {
        return -2;
    }

    if (strlen(obj->valuestring)) {
        slot.alarmOper.append(obj->valuestring);
    } else {
        return 0;
    }

    obj = cJSON_GetObjectItem(subJsonP, "scene_data");

    //printf("scene_data:%s\n", obj? obj->valuestring : "none");
    if (obj != NULL) {
        ret = jsonParseSemantic(obj->valuestring, &slot);

        if (ret != 0) {
            return ret;
        }
    } else {
        return -2;
    }
    obj = cJSON_GetObjectItem(subJsonP, "scene_name");
    printf("scene_name:%s\n", obj ? obj->valuestring : "none");
    if(obj && strlen(obj->valuestring)){
    	slot.sceneName.append(obj->valuestring);
    }


    obj = cJSON_GetObjectItem(subJsonP, "speak");
    printf("speak:%s\n", obj ? obj->valuestring : "none");
    if(obj && strlen(obj->valuestring)){
    	slot.spk.append(obj->valuestring);
    }

    return slot2AlarmParaT(&slot, alarm);
}

#endif

int AlarmUtilsJson::jsonToAlarmsLibratone(AlarmParaT* vecAlarm)
{
    cJSON* subJsonP, *subAlarm, *subAlarmTimeInfo, *subAlarmRepeatTimes, *subAlarmIntent, *intentObj, *obj;
    AlarmParaT alarm;
    int alarmCnt = 0, intentCnt = 0, repeatTimesCnt = 0;
    int i = 0, j = 0;
    char* jsonString;

    if (mJsonP == NULL) {
        ALOGE("mJsonP is NULL \n");
        return -1;
    }

    subJsonP = cJSON_GetObjectItem(mJsonP, "vecCalendarInfo");

    if (subJsonP == NULL) {
        return 0;
    }

    alarmCnt = cJSON_GetArraySize(subJsonP);
    jsonString = cJSON_Print(subJsonP);
    ALOGV("vecCalendarInfo=%s\n", jsonString);
    free(jsonString);

    for (i = 0; i < alarmCnt; i++) {
        subAlarm = cJSON_GetArrayItem(subJsonP, i);
        //jsonString = cJSON_Print(subAlarm);
        //ALOGV("alarmPara[%d]=%s\n", i, jsonString);
        //free(jsonString);
        memset(&alarm, 0, sizeof(alarm));
        obj = cJSON_GetObjectItem(subAlarm, "lAlarmID");
        alarm.alarmID = obj ? obj->valuedouble : 0;
        obj = cJSON_GetObjectItem(subAlarm, "iAlarmType");
        alarm.alarmType = obj ? obj->valueint : 0;
        obj = cJSON_GetObjectItem(subAlarm, "eAlarmOperation");
        alarm.alarmOperation = obj ? obj->valueint : 0;
        obj = cJSON_GetObjectItem(subAlarm, "bAlarmOff");
        alarm.alarmOff = obj ? obj->valueint : 0;

        obj = cJSON_GetObjectItem(subAlarm, "sAlarmName");

        if (obj != NULL) {
            strncpy(alarm.alarmName, obj->valuestring, ALARM_NAME_LEN);
        }

        obj = cJSON_GetObjectItem(subAlarm, "sAlarmTimeOri");

        if (obj != NULL) {
            strncpy(alarm.alarmTimeOri, obj->valuestring, ALARM_TIMEORI_LEN);
        }

        obj = cJSON_GetObjectItem(subAlarm, "sAlarmTTS");

        if (obj != NULL) {
            strncpy(alarm.alarmTTS, obj->valuestring, ALARM_TTS_LEN_MAX);
        }

        subAlarmIntent = cJSON_GetObjectItem(subAlarm, "vecAlarmIntent");

        if (subAlarmIntent != NULL) {
            intentCnt = cJSON_GetArraySize(subAlarmIntent);

            if (intentCnt > ALARM_INTENT_CNT_MAX) {
                intentCnt = ALARM_INTENT_CNT_MAX;
            }

            for (j = 0; j < intentCnt; j++) {
                intentObj = cJSON_GetArrayItem(subAlarmIntent, j);
                obj = cJSON_GetObjectItem(intentObj, "sIntentName");

                if (obj != NULL) {
                    strncpy(alarm.alarmIntent[j].name, obj->valuestring, ALARM_INTENT_NAME_LEN);
                }

                obj = cJSON_GetObjectItem(intentObj, "eIntentCode");
                alarm.alarmIntent[j].code = (alarm_intent_code_t)(obj ? obj->valueint : 0);
                obj = cJSON_GetObjectItem(intentObj, "eIntentSubCode");
                alarm.alarmIntent[j].subCode = obj ? obj->valueint : 0;

                obj = cJSON_GetObjectItem(intentObj, "sIntentContent");

                if (obj != NULL) {
                    strncpy(alarm.alarmIntent[j].content, obj->valuestring, ALARM_INTENT_CONTENT_LEN);
                }

                if (alarm.alarmIntent[j].code <= ALARM_INTENT_PLAY
                    && alarm.alarmIntent[j].subCode <= ALARM_PLAY_LOCAL_FILE
                    && strlen(alarm.alarmIntent[j].content) == 0) {
                    strncpy(alarm.alarmIntent[j].content, (char*)ALARM_AUDIO_DEFULAT, ALARM_INTENT_CONTENT_LEN);
                }
            }
        }

        subAlarmTimeInfo = cJSON_GetObjectItem(subAlarm, "stTimeInfo");

        if (subAlarmTimeInfo != NULL) {
            obj = cJSON_GetObjectItem(subAlarmTimeInfo, "lFirstTime");
            alarm.alarmTime.firstTime = obj ? obj->valuedouble * 1000LL : 0;
            obj = cJSON_GetObjectItem(subAlarmTimeInfo, "lInterval");
            alarm.alarmTime.interval = obj ? obj->valueint : 0;

            if (alarm.alarmTime.interval < ALARM_INTERVAL_MIN || alarm.alarmTime.interval > ALARM_INTERVAL_MAX) {
                alarm.alarmTime.interval = ALARM_INTERVAL_DEFAULT;
            }

            alarm.alarmTime.interval *= 1000L;
            obj = cJSON_GetObjectItem(subAlarmTimeInfo, "lRepeatCnt");
            alarm.alarmTime.repeatCnt = obj ? obj->valueint : 0;

            if (alarm.alarmTime.repeatCnt < ALARM_REPEAT_CNT_MIN  || alarm.alarmTime.repeatCnt > ALARM_REPEAT_CNT_MAX) {
                alarm.alarmTime.repeatCnt = ALARM_REPEAT_CNT_DEFAULT;
            }

            obj = cJSON_GetObjectItem(subAlarmTimeInfo, "eRepeatInfo");
            alarm.alarmTime.repeatInfo = obj ? obj->valueint : 0;

            subAlarmRepeatTimes = cJSON_GetObjectItem(subAlarmTimeInfo, "vecRepeatTimes");

            if (subAlarmRepeatTimes != NULL) {
                repeatTimesCnt = cJSON_GetArraySize(subAlarmRepeatTimes);

                if (repeatTimesCnt > ALARM_REPEAT_TIMESTAMP_CNT) {
                    repeatTimesCnt = ALARM_REPEAT_TIMESTAMP_CNT;
                }

                for (j = 0; j < repeatTimesCnt; j++) {
                    alarm.alarmTime.repeatTimes[j] = cJSON_GetArrayItem(subAlarmRepeatTimes, j)->valueint;
                }
            }
        }

        if (alarm.alarmID == 0) {
            alarm.alarmID = ALARM_ID(alarm.alarmTime.firstTime, alarm.alarmTime.repeatInfo);
        }
    }

    return alarmCnt;
}

#ifdef SUPPORT_BAIDU_INTERFACE
int AlarmUtilsJson::jsonToAlarmsBaidu(AlarmParaT* vecAlarm)
{
    return 0;
}
#endif

//"{ \"iRet\": 0, \"sError\": \"\", \"vecCalendarInfo\": [ { \"eAction\": 2, \"sContent\": \"起床\", \"stTimeInfo\": { \"lFirstTime\": 1484697600, \"nRepeatInfo\": 31, \"sTimeOri\": \"八点\", \"vecRepeatTimes\": [  ]}} ]}"
char* AlarmUtilsJson::alarmsToJsonString(AlarmParaT* vecAlarms)
{
    cJSON* vecCalendarInfo, *alarmItem, *alarmInfo, *vecRepeatTimes, *vecIntent, *intent;
    //char* out;
    int i = 0;
    AlarmParaT* iter = NULL;

    if(mJsonP){
	jsonRelease();
    }
	
    if (mJsonP == NULL) {
        mJsonP = cJSON_CreateObject();
    }

    cJSON_AddNumberToObject(mJsonP, "iRet", 0);
    cJSON_AddStringToObject(mJsonP, "sError", "");

    vecCalendarInfo = cJSON_CreateArray();
    cJSON_AddItemToObject(mJsonP, "vecCalendarInfo", vecCalendarInfo);

    for (iter = vecAlarms; iter != NULL; iter=NULL) {
        alarmItem = cJSON_CreateObject();
        cJSON_AddNumberToObject(alarmItem, "lAlarmID", iter->alarmID);
        cJSON_AddNumberToObject(alarmItem, "iAlarmType", iter->alarmType);
        cJSON_AddNumberToObject(alarmItem, "eAlarmOperation", iter->alarmOperation);
        cJSON_AddBoolToObject(alarmItem, "bAlarmOff", iter->alarmOff);
        cJSON_AddStringToObject(alarmItem, "sAlarmName", iter->alarmName);
        cJSON_AddStringToObject(alarmItem, "sAlarmTimeOri", iter->alarmTimeOri);
        cJSON_AddStringToObject(alarmItem, "sAlarmTTS", iter->alarmTTS);
        vecIntent = cJSON_CreateArray();
        cJSON_AddItemToObject(alarmItem, "vecAlarmIntent", vecIntent);

        for (i = 0; i < ALARM_INTENT_CNT_MAX; i++) {
            if (iter->alarmIntent[i].name[0] == '\0') {
                break;
            }

            intent = cJSON_CreateObject();
            cJSON_AddStringToObject(intent, "sIntentName", iter->alarmIntent[i].name);
            cJSON_AddNumberToObject(intent, "eIntentCode", iter->alarmIntent[i].code);
            cJSON_AddNumberToObject(intent, "eIntentSubCode", iter->alarmIntent[i].subCode);
            cJSON_AddStringToObject(intent, "sIntentContent", iter->alarmIntent[i].content);
            cJSON_AddItemToArray(vecIntent, intent);
        }

        alarmInfo = cJSON_CreateObject();
        cJSON_AddItemToObject(alarmItem, "stTimeInfo", alarmInfo);
        cJSON_AddNumberToObject(alarmInfo, "lFirstTime", iter->alarmTime.firstTime/1000L);
        cJSON_AddNumberToObject(alarmInfo, "lInterval", iter->alarmTime.interval/1000L);
        cJSON_AddNumberToObject(alarmInfo, "lRepeatCnt", iter->alarmTime.repeatCnt);
        cJSON_AddNumberToObject(alarmInfo, "eRepeatInfo", iter->alarmTime.repeatInfo);

        for (i = 0; i < ALARM_REPEAT_TIMESTAMP_CNT; i++) {
            if (iter->alarmTime.repeatTimes[i] == 0) {
                break;
            }
        }

        vecRepeatTimes = cJSON_CreateIntArray((int*)iter->alarmTime.repeatTimes, i);
        cJSON_AddItemToObject(alarmInfo, "vecRepeatTimes", vecRepeatTimes);
        cJSON_AddItemToArray(vecCalendarInfo, alarmItem);
    }

    return cJSON_Print(mJsonP);
}

}
