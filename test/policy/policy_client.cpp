/*************************************************************************
> File: policy_client.cpp
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn 
> Time:   2018-03-29 21:44:08
*************************************************************************/

#include<iostream>
#include<iomanip>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<ctime>
#include<cmath>
#include "policy_client.h"
#include "policy_service.h"

using namespace std;
namespace test_libratone{
PolicyClient::PolicyClient(const char* name, eAudioSourceType_t source)
{
    mName = name;
    mSelf = source;
    mPs = NULL;
    printf("[%p]%s: name=%s, source=%d\n", this, __func__, name, source);
}

PolicyClient::~PolicyClient()
{
    printf("[%p]%s\n", this, __func__);
}

void PolicyClient::notify(eRequestRet_t msg, eAudioSourceType_t old, eAudioSourceType_t now, \
        int ext1, int ext2, void* pri_vate)
{
    const char* msg2str=NULL;

    printf("[%p]%s: msg=%d, old=%d, new=%d, ext1=%d, ext2=%d, pri_vate=%p\n", \
            this, __func__, msg, old, now, ext1, ext2, pri_vate);
    
    switch(msg){
        case eRequestRetNotAllow:
            msg2str = "eRequestRetNotAllow";
            break;
        case eRequestRetSuspend:
            msg2str = "eRequestRetSuspend";
            break;
        case eRequestRetAllow:
            msg2str = "eRequestRetAllow";
            break;
        case eRequestRetResume:
            msg2str = "eRequestRetResume";
            break;
        case eRequestRetBackground:
            msg2str = "eRequestRetBackground";
            break;
        case eRequestRetForeground:
            msg2str = "eRequestRetForeground";
            break;
        default:
            break;
    }
    printf("[%p]%s: %s.%s\n", this, __func__, mName, msg2str);
}

int PolicyClient::requestStart()
{
    printf("[%p]%s:%s.request\n", this, __func__, mName);
    if(mPs){
        mPs->requestAudioPlay(this);
    }
    return 0;
}

int PolicyClient::requestFinish()
{
    printf("[%p]%s:%s.request\n", this, __func__, mName);
    if(mPs){
        mPs->requestAudioFinish(this);
    }
    return 0;
}
}


