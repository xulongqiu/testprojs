/*************************************************************************
> File: abstructClass.cpp
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn 
> Time:   2016年10月25日 星期二 19时34分35秒
*************************************************************************/

#include<iostream>
#include <stdlib.h>
using namespace std;

namespace android{

    
    class listener {
        public:
            virtual void battery_changed(int value);
    };
    class Mylistener: public listener {
        public:
            void battery_changed(int value){
                //printf("battery_changed:%d\n", value);
            }
    };

    class producer{
        public:
            void power_changed_notify(listener& listener){
                listener.battery_changed(5);
            }
    };
}

int main(int argc, char *argv[])
{
    if(argc && argv != NULL)
        ;
    android::producer *pro = new android::producer();
    android::Mylistener *cus = new android::Mylistener();

    while(1){
    
        pro->power_changed_notify(*cus);
        //sleep(1);
    }
}

