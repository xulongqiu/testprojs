/*************************************************************************
> File: policy_client.h
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn
> Time:   2018-03-29 19:05:43
*************************************************************************/
#ifndef __POLICY_CLIENT_H__
#define __POLICY_CLIENT_H__
#include<iostream>
#include<iomanip>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<ctime>
#include<cmath>
#include "policy.h"
using namespace std;

namespace test_libratone
{
class PolicyService;
class IPolicyClient
{
public:
    eAudioSourceType_t mSelf;
    const char* mName;
    ePolicyStatus_t mStat;
    virtual void notify(eRequestRet_t msg, eAudioSourceType_t old, eAudioSourceType_t now, 
            int ext1, int ext2, void* pri_vate) = 0;
    virtual ~IPolicyClient() {}
};
class PolicyClient: public IPolicyClient
{
public:
    PolicyClient(const char* name, eAudioSourceType_t source);
    void setPs(PolicyService* ps){
        mPs = ps;
    }
    void notify(eRequestRet_t msg, \
                eAudioSourceType_t old = eAudioSourceTypeNull, \
                eAudioSourceType_t now = eAudioSourceTypeNull, \
                int ext1 = 0, int ext2 = 0, void* pri_vate = NULL);
    int requestStart();
    int requestFinish();
    const char* getSourceName(){
        return mName;
    }
    eAudioSourceType_t getSourceType(){
        return mSelf;
    }
    ePolicyStatus_t getSourceStatus(){
        return mStat;
    }
    void setSourceStatus(ePolicyStatus_t st){
        mStat = st;
    }
    ~PolicyClient();
private:
    PolicyService* mPs;
};
}

#endif
