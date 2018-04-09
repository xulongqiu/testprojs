/*************************************************************************
> File: policy_service.cpp
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn
> Time:   2018-03-30 10:18:53
*************************************************************************/

#include<iostream>
#include<iomanip>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<ctime>
#include<cmath>
#include "policy_service.h"
#include "policy_client.h"
using namespace std;


namespace test_libratone
{
#define PS_TAG "PolicyService"

PolicyService::PolicyService()
{
    int i = 0;

    for (i = 0; i < eAudioSourcePriorityMax; i++) {
        mVecSourceLvl[i].clear();
    }
}

const tAudioSourceProperty_t* getSourceProperty(eAudioSourceType_t type)
{
    int i = 0;

    for (i = 0; i < (int)G_AUDIO_SOURCE_TYPE_CNT; i++) {
        if (gAudioSourcePropertyTable[i].self == type) {
            return gAudioSourcePropertyTable + i;
        }
    }

    return NULL;
}
bool PolicyService::isActiveClient(PolicyClient* client)
{
    const tAudioSourceProperty_t* asp = NULL;
    vector<PolicyClient*>::iterator iter;

    if (!client) {
        return false;
    }

    asp = getSourceProperty(client->getSourceType());

    if (!asp) {
        return false;
    }

    for (iter = mVecSourceLvl[asp->lvl].begin(); iter != mVecSourceLvl[asp->lvl].end(); iter++) {
        if (client == *iter) {
            return (client->getSourceStatus() == ePolicyStatusForeground);
        }
    }

    return false;
}

PolicyClient* PolicyService::getForegroundClient()
{
    int i = 0;
    vector<PolicyClient*>::iterator iter;

    for (i = 0; i < eAudioSourcePriorityMax; i++) {
        for (iter = mVecSourceLvl[i].begin(); iter != mVecSourceLvl[i].end(); iter++) {
            if ((*iter)->getSourceStatus() == ePolicyStatusForeground) {
                return *iter;
            }
        }
    }

    return NULL;
}

int PolicyService::checkAddClient(PolicyClient* client)
{
    const tAudioSourceProperty_t* asp = NULL;
    vector<PolicyClient*>::iterator iter;

    if (client == NULL) {
        return -1;
    }

    asp = getSourceProperty(client->getSourceType());

    if (!asp || asp->lvl >= eAudioSourcePriorityMax) {
        return -1;
    }

    for (iter = mVecSourceLvl[asp->lvl].begin(); iter != mVecSourceLvl[asp->lvl].end(); iter++) {
        if (client != *iter) {
            notify(*iter, eRequestRetNotAllow);
        }
    }

    mVecSourceLvl[asp->lvl].clear();
    mVecSourceLvl[asp->lvl].push_back(client);

    return 0;
}

PolicyClient* PolicyService::getSuspendClient()
{
    int i = 0;
    vector<PolicyClient*>::iterator iter;

    for (i = 0; i < eAudioSourcePriorityMax; i++) {
        for (iter = mVecSourceLvl[i].begin(); iter != mVecSourceLvl[i].end(); iter++) {
            if ((*iter)->getSourceStatus() == ePolicyStatusSuspend) {
                return *iter;
            }
        }
    }

    return NULL;
}

PolicyClient* PolicyService::getBackgroundClient()
{
    int i = 0;
    vector<PolicyClient*>::iterator iter;

    for (i = 0; i < eAudioSourcePriorityMax; i++) {
        for (iter = mVecSourceLvl[i].begin(); iter != mVecSourceLvl[i].end(); iter++) {
            if ((*iter)->getSourceStatus() == ePolicyStatusBackground) {
                return *iter;
            }
        }
    }

    return NULL;
}

int PolicyService::checkRemoveClient(PolicyClient* client)
{
    vector<PolicyClient*>::iterator iter;
    const tAudioSourceProperty_t* asp = NULL;

    if (client == NULL) {
        return -1;
    }

    asp = getSourceProperty(client->getSourceType());

    if (!asp || asp->lvl >= eAudioSourcePriorityMax) {
        return -1;
    }

    for (iter = mVecSourceLvl[asp->lvl].begin(); iter != mVecSourceLvl[asp->lvl].end(); iter++) {
        if (client == *iter) {
            mVecSourceLvl[asp->lvl].erase(iter);
            return 0;
        }
    }

    return -1;
}

bool PolicyService::isInNotAllowSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type)
{
    if (!cur_asp) {
        return false;
    }

    return !!(cur_asp->notAllow & type);
}

bool PolicyService::isInSuspendSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type)
{
    if (!cur_asp) {
        return false;
    }

    return !!(cur_asp->suspend & type);
}

bool PolicyService::isInAllowSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type)
{
    if (!cur_asp) {
        return false;
    }

    return !!(cur_asp->allow & type);
}

bool PolicyService::isInNotAllowSelfSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type)
{
    if (!cur_asp) {
        return false;
    }

    return !!(cur_asp->notAllowSelf & type);
}

bool PolicyService::isInSuspendSelfSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type)
{
    if (!cur_asp) {
        return false;
    }

    return !!(cur_asp->suspendSelf & type);
}

bool PolicyService::isInBackgroundSelfSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type)
{
    if (!cur_asp) {
        return false;
    }

    return !!(cur_asp->backgroundSelf & type);
}

int PolicyService::notify(PolicyClient* client, eRequestRet_t msg, \
                          eAudioSourceType_t old, eAudioSourceType_t now, \
                          int ext1, int ext2, void* pri_vate)
{
    if (!client || msg >= eRequestRetMax) {
        return -1;
    }

    switch (msg) {
    case eRequestRetAllow:
    case eRequestRetResume:
    case eRequestRetForeground:
        client->setSourceStatus(ePolicyStatusForeground);
        break;

    case eRequestRetSuspend:
        client->setSourceStatus(ePolicyStatusSuspend);
        break;

    case eRequestRetNotAllow:
        client->setSourceStatus(ePolicyStatusInit);
        break;

    case eRequestRetBackground:
        client->setSourceStatus(ePolicyStatusBackground);

    default:
        break;
    }

    client->notify(msg, old, now, ext1, ext2, pri_vate);
    return 0;
}

bool PolicyService::compareSources(PolicyClient* foreground, PolicyClient* client)
{
    eAudioSourceType_t source = eAudioSourceTypeNull;
    const tAudioSourceProperty_t* asp = NULL;

    if (!foreground || !client) {
        return false;
    }
    
    asp = getSourceProperty(foreground->getSourceType());
    if(!asp){
        return false;
    }
    
    source = client->getSourceType();

    if (isInNotAllowSources(asp, source)) {
        notify(client, eRequestRetNotAllow);
        return false;
    } else if (isInSuspendSources(asp, source)) {
        checkAddClient(client);
        notify(client, eRequestRetSuspend);
        return false;
    } else if (isInAllowSources(asp, source)) {
        if (isInNotAllowSelfSources(asp, source)) {
            notify(foreground, eRequestRetNotAllow);
            checkRemoveClient(foreground);
            foreground = getSuspendClient();
            if(foreground){
                if(compareSources(foreground, client) == false){
                    notify(foreground, eRequestRetResume);
                }
            }else{
                checkAddClient(client);
                notify(client, eRequestRetAllow);
            }
        } else if (isInSuspendSelfSources(asp, source)) {
            notify(foreground, eRequestRetSuspend);
            checkAddClient(client);
            notify(client, eRequestRetAllow);
        } else if (isInBackgroundSelfSources(asp, source)) {
            notify(foreground, eRequestRetBackground, foreground->getSourceType(), source, asp->backVolumeLvl);
            checkAddClient(client);
            notify(client, eRequestRetAllow);
        }
        return true;
    }
    notify(client, eRequestRetNotAllow);
    return false;
}

int PolicyService::requestAudioPlay(PolicyClient* client)
{
    eAudioSourceType_t source = eAudioSourceTypeNull;
    const char* name = NULL;
    PolicyClient* forground = getForegroundClient();
    PolicyClient* background = getBackgroundClient();
    PolicyClient* cmp = forground;
    const tAudioSourceProperty_t* asp = NULL;

    if (!client) {
        return -1;
    }

    source = client->getSourceType();
    name = client->getSourceName();
    printf("[%s]%s: client.name=%s, client.eSourceType=%d\n", PS_TAG, __func__, name, source);

    if (forground == NULL) {
        checkAddClient(client);
        notify(client, eRequestRetAllow);
        return 0;
    } else if (forground == client) {
        notify(client, eRequestRetAllow);
        return 0;
    }

    if (background) {
        asp = getSourceProperty(background->getSourceType());

        if (background == client) {
            notify(client, eRequestRetBackground);
            return 0;
        }

        cmp = background;
    } else {
        asp = getSourceProperty(forground->getSourceType());
    }

    if (!asp) {
        return -1;
    }
    compareSources(cmp, client);
    return 0;
}

int PolicyService::requestAudioFinish(PolicyClient* client)
{
    eAudioSourceType_t source = eAudioSourceTypeNull;
    const char* name = NULL;
    PolicyClient* resume_c = NULL;

    if (!client) {
        return -1;
    }

    source = client->getSourceType();
    name = client->getSourceName();
    printf("[%s]%s: client.name=%s, client.eSourceType=%d\n", PS_TAG, __func__, name, source);

    notify(client, eRequestRetNotAllow);

    if (checkRemoveClient(client) != 0) {
        return 0;
    }

    if (getForegroundClient()) {
        return 0;
    }

    resume_c = getBackgroundClient();

    if (resume_c) {
        notify(resume_c, eRequestRetForeground, source, resume_c->getSourceType(), \
               getSourceProperty(resume_c->getSourceType())->backVolumeLvl);
    } else {
        resume_c = getSuspendClient();

        if (resume_c) {
            notify(resume_c, eRequestRetResume);
        }
    }

    return 0;
}

}
