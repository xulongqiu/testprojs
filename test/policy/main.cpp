/*************************************************************************
> File: main.cpp
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn
> Time:   2018-03-30 13:42:15
*************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "policy_client.h"
#include "policy_service.h"
#include "policy.h"

using namespace std;
using namespace test_libratone;

void my_sleep(int sec)
{
    //sleep(sec);
}

int main(int argc, const char* argv[])
{
    PolicyClient* mWakeup = new PolicyClient("WakeUp", eAudioSourceTypeLikeWakeUp);
    PolicyClient* mTTS = new PolicyClient("TTS", eAudioSourceTypeLikeWakeUp);
    PolicyClient* mVrMusic = new PolicyClient("vrMusic", eAudioSourceTypeLikeVrMusic);
    PolicyClient* mBtCall = new PolicyClient("BtCall", eAudioSourceTypeLikeCall);
    PolicyClient* mRingtone = new PolicyClient("Ringtone", eAudioSourceTypeLikeRingTone);
    PolicyClient* mUdisk = new PolicyClient("UdiskPlayer", eAudioSourceTypeLikeMusic);
    PolicyClient* mAlarm = new PolicyClient("Alarm", eAudioSourceTypeLikeAlarm);
    PolicyClient* mTone = new PolicyClient("Tone", eAudioSourceTypeLikeTone);
    PolicyService* mMainPs = new PolicyService();

    if (!mWakeup || !mTTS || !mVrMusic || !mBtCall || !mRingtone \
        || !mUdisk || !mAlarm || !mTone || !mMainPs) {
        return -0;
    }

    mWakeup->setPs(mMainPs);
    mTTS->setPs(mMainPs);
    mVrMusic->setPs(mMainPs);
    mRingtone->setPs(mMainPs);
    mBtCall->setPs(mMainPs);
    mUdisk->setPs(mMainPs);
    mAlarm->setPs(mMainPs);
    mTone->setPs(mMainPs);
#if 1    
    mWakeup->requestStart();
    mTone->requestStart();
    my_sleep(2);
    mTone->requestFinish();
    mUdisk->requestStart();
    my_sleep(1);
    mAlarm->requestStart();
    mTone->requestStart();
    mTone->requestFinish();
    my_sleep(1);
    mTTS->requestStart();
    my_sleep(1);
    mRingtone->requestStart();
    my_sleep(2);
    mVrMusic->requestStart();
    my_sleep(1);
    mWakeup->requestStart();
    my_sleep(1);
    mBtCall->requestStart();
    my_sleep(2);
    mRingtone->requestStart();
    mWakeup->requestStart();
    mRingtone->requestFinish();
    mBtCall->requestStart();
    mBtCall->requestFinish();
    my_sleep(5);
    mBtCall->requestFinish();
    mTone->requestStart();
    my_sleep(1);
    mTone->requestFinish();
    my_sleep(5);
    mAlarm->requestFinish();
    mTone->requestStart();
    mAlarm->requestStart();
    mTone->requestFinish();
    my_sleep(3);
    printf("----------------------------------\n");
    mRingtone->requestStart();
    my_sleep(2);
    mWakeup->requestStart();
    my_sleep(1);
    mTTS->requestStart();
    my_sleep(1);
    mUdisk->requestStart();
    my_sleep(1);
    mTTS->requestFinish();
    my_sleep(1);
    mRingtone->requestFinish();
    mTone->requestStart();
    mTone->requestFinish();
    my_sleep(1);
    mAlarm->requestFinish();
    my_sleep(10);

    mWakeup->requestFinish();
    mVrMusic->requestFinish();
    mTTS->requestFinish();
    mRingtone->requestFinish();
    mUdisk->requestFinish();
#else
    mVrMusic->requestStart();
    mTone->requestStart();
    mAlarm->requestStart();
    mTone->requestFinish();
    mAlarm->requestFinish();
    mVrMusic->requestFinish();
#endif
    delete mWakeup;
    delete mTTS;
    delete mVrMusic;
    delete mTone;
    delete mRingtone;
    delete mBtCall;
    delete mUdisk;
    delete mAlarm;
    delete mMainPs;

    return 0;
}
