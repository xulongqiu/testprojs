/*************************************************************************
> File: policy_service.h
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn
> Time:   2018-03-29 21:15:38
*************************************************************************/
#ifndef __POLICY_SERVICE_H__
#define __POLICY_SERVICE_H__
#include<iostream>
#include<iomanip>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<ctime>
#include<cmath>
#include<vector>
#include"policy.h"
using namespace std;

namespace test_libratone
{
class PolicyClient;
class PolicyService
{
public:
    PolicyService();
    int requestAudioPlay(PolicyClient* client);
    int requestAudioFinish(PolicyClient* client);
private:
    int notify(PolicyClient* client, eRequestRet_t msg, \
               eAudioSourceType_t old = eAudioSourceTypeNull, \
               eAudioSourceType_t now = eAudioSourceTypeNull,
               int ext1 = 0, int ext2 = 0, void* pri_vate = NULL);
    PolicyClient* getForegroundClient();
    bool isActiveClient(PolicyClient* client);
    int checkAddClient(PolicyClient* client);
    int checkRemoveClient(PolicyClient* client);
    PolicyClient* getSuspendClient();
    PolicyClient* getBackgroundClient();
    bool isInNotAllowSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type);
    bool isInSuspendSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type);
    bool isInAllowSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type);
    bool isInNotAllowSelfSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type);
    bool isInSuspendSelfSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type);
    bool isInBackgroundSelfSources(const tAudioSourceProperty_t* cur_asp, eAudioSourceType_t type);
    bool compareSources(PolicyClient* foreground, PolicyClient* client);
    vector<PolicyClient*> mVecSourceLvl[eAudioSourcePriorityMax];
};
}

#endif
