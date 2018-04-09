#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/select.h>
#include <time.h>
#include <errno.h>

void milliseconds_sleep(unsigned long mSec){
  struct timeval tv;
  int err;

  tv.tv_sec = mSec/1000;
  tv.tv_usec = (mSec%1000)*1000;

  do{
    err = select(0, NULL, NULL, NULL, &tv);
  }while(err<0 && errno == EINTR);
}


void thread(void)
{
  int i = 0;

  while(1){
   // milliseconds_sleep(20);
    usleep(20000);
    printf("This is a pthread:%d\n", i++);
  }
}

int main(void){
  pthread_t id;
  int i, ret;

  ret = pthread_create(&id, NULL, (void*)thread, NULL);
  if(ret != 0){
    printf("create pthread error\n");
	exit (1);
  }

  while(1){
    sleep(5);
	printf("This is main thread\n");
  }
  return 0;
}
