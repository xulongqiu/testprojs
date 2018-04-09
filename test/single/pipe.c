#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(void){
  int fd[2];
  int ret = 0;
  int *read_fd = &fd[0]; 
  int *write_fd = &fd[1]; 
  char string[] = "pipe test\n";
  char read_buf[100] = {0};

  ret = pipe(fd);
  if(-1 == ret)
  {
    printf("create pipe fail\n");
	return -1;
  }

  write(*write_fd, string, strlen(string));

  sleep(1);

  read(*read_fd, read_buf, sizeof(read_buf));
  printf("%s", read_buf);

  close(*write_fd);
  close(*read_fd);
  return 0;
}
