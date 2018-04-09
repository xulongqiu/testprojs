/*************************************************************************
> File: symlink.c
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn 
> Time:   2018-03-28 21:09:52
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <errno.h>
#include <stdarg.h>
#include <mtd/mtd-user.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <selinux/selinux.h>
#include <selinux/label.h>
#include <selinux/android.h>

#include <libgen.h>

#include <cutils/list.h>
#include <cutils/android_reboot.h>
#include <cutils/sockets.h>
#include <cutils/iosched_policy.h>
#include <cutils/fs.h>
#include <private/android_filesystem_config.h>
#include <termios.h>

#include "devices.h"
#include "init.h"
#include "log.h"
#include "property_service.h"
#include "bootchart.h"
#include "signal_handler.h"
#include "keychords.h"
#include "init_parser.h"
#include "util.h"
#include "ueventd.h"
#include "watchdogd.h"



#define ERROR printf

static void symlink_users(void)
{
    int ret = 0;
    int is_zipp = 0;
    int mfi_flag = 0;
    char tmp[PROP_VALUE_MAX];
    #define libvdsp_path "/system/lib/libvdsp"
    #define libalg_path "/system/lib/libfdma6micthr"

    is_zipp = 1;
    mfi_flag = 1;

    unlink(libvdsp_path".so");
    unlink(libalg_path"1demo_lib.so");
    unlink(libalg_path"2demo_lib.so");
    ERROR("%s: is_zipp=%d, mfi_flag=%d\n", __func__, is_zipp, mfi_flag);
    if(is_zipp != 0){
        ret = symlink(libalg_path"1demo_lib_zipp3.so", libalg_path"1demo_lib.so");
        if(ret < 0){
            ERROR("%s: link %s to %s failed:%s\n", __func__, libalg_path"1demo_lib_zipp3.so", 
                    libalg_path"1demo_lib.so", strerror(errno));    
        }
        ret = symlink(libalg_path"2demo_lib_zipp3.so", libalg_path"2demo_lib.so");
        if(ret < 0){
            ERROR("%s: link %s to %s failed:%s\n", __func__, libalg_path"2demo_lib_zipp3.so", 
                    libalg_path"2demo_lib.so", strerror(errno));
        }
        if(mfi_flag != 0){
            ret = symlink(libvdsp_path"_zipp3_480.so", libvdsp_path".so");
            if(ret < 0){
                ERROR("%s: link %s to %s failed:%s\n", __func__, libvdsp_path"_zipp3_480.so", 
                        libvdsp_path".so", strerror(errno));    
            }
        }else{
            ret = symlink(libvdsp_path"_zipp3.so", libvdsp_path".so");
            if(ret < 0){
                ERROR("%s: link %s to %s failed:%s\n", __func__, libvdsp_path"_zipp3.so", 
                        libvdsp_path".so", strerror(errno));    
            }
        }
    }else{
        symlink(libalg_path"1demo_lib_egg2.so", libalg_path"1demo_lib.so");
        symlink(libalg_path"2demo_lib_egg2.so", libalg_path"2demo_lib.so");
        if(mfi_flag != 0){
            symlink(libvdsp_path"_egg2_480.so", libvdsp_path".so");
        }else{
            symlink(libvdsp_path"_egg2.so", libvdsp_path".so");
        }
    }
}

int main(int argc, const char* argv[])
{
    symlink_users();
    return 1;
}
