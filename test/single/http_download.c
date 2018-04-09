/*************************************************************************
> File: http_download.c
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn 
> Time:   2017年01月09日 星期一 18时06分30秒
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
//http://isure.stream.qqmusic.qq.com/M500004crIQJ1c3EEn.mp3?vkey=5C5DE08E8425C4DEDE3F88208ADDD62399A7B96037BC842F2164ABBC658D6A6AD1484FEE33CFD4599E8C258D9C92AD103455B56877BB2FFD&guid=ffffffff9cd3337cffffffffb5cece77&continfo=D5A9231D04B9C36AD920DA9262E990FDC3F95840085A4BC7&uin=1152921504621981642&fromtag=57
//下载目标文件 : http://ftp.gnu.org/gnu/bc/bc-1.03.tar.gz
//#define HOST_ADDR "ftp.gnu.org"
//#define FILE_PATH "/gnu/bc/bc-1.03.tar.gz"
//#define FILE_NAME "bc-1.03.tar.gz"

//#define HOST_ADDR "isure.stream.qqmusic.qq.com"
//#define FILE_PATH "/M500004crIQJ1c3EEn.mp3?vkey=5C5DE08E8425C4DEDE3F88208ADDD62399A7B96037BC842F2164ABBC658D6A6AD1484FEE33CFD4599E8C258D9C92AD103455B56877BB2FFD&guid=ffffffff9cd3337cffffffffb5cece77&continfo=D5A9231D04B9C36AD920DA9262E990FDC3F95840085A4BC7&uin=1152921504621981642&fromtag=57"
//#define FILE_NAME "M500004crIQJ1c3EEn.mp3"

static char HOST_ADDR[100] = {0};
static char* FILE_PATH = 0;
static char* FILE_NAME = 0;

void write_to_socket(int socketfd, const char* str)
{
    write(socketfd, str, strlen(str));
}

int main(int argc, char* argv[])
{
    char* pstr = NULL;
    int sk_fd = 0;
    int size = 0;
    int sum = 0;
    unsigned long filesize = 0;
    FILE* fp = NULL;
    FILE* newfp = NULL;
    struct sockaddr_in sk_addr;
    struct hostent *phost = NULL;
    char buf[1024*32] = {0};
    float processbar = 0.0f;
	unsigned char last_progress = 0;

    if (argc != 3)
    {   
        printf("Usage:%s http_addr file_name", argv[0]);
        exit(1);
    }
    else
    {
       pstr = strstr(argv[1], "://");
       if(pstr > 0)
       {
           pstr += 3;
       }
       else
       {
           printf("invalid http_addr:%s", argv[1]);
           exit(1);
       }
       FILE_PATH = strchr(pstr, '/');
       if(FILE_PATH > pstr)
       {
           memcpy(HOST_ADDR, pstr, FILE_PATH - pstr);
       }    
       else
       {
           printf("invalid http_addr:%s", argv[1]);
           exit(1);
       }
       FILE_NAME = argv[2];

    }
    
    // create TCP socket
    sk_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sk_fd < 0)
    {
        perror("socket failed");
        exit(1);
    }
    
 
    phost = gethostbyname(HOST_ADDR);
    printf("Step0: parser args:\nFILE_PATH=%s\nHOST_ADDR=%s\n", FILE_PATH, HOST_ADDR);
    
    memset(&sk_addr, 0x00, sizeof(sk_addr));
    sk_addr.sin_family = AF_INET;
    sk_addr.sin_port = htons(80);
    sk_addr.sin_addr.s_addr = ((struct in_addr *)phost->h_addr)->s_addr;
    if (connect(sk_fd, (struct sockaddr *)&sk_addr, sizeof(sk_addr)) < 0)
    {
        perror("connect failed");
        close(sk_fd);
        exit(1);
    }
    
    sprintf(buf, "GET %s HTTP/1.1\r\nAccept: */*\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n\0", FILE_PATH, HOST_ADDR);
    printf("\nStep1: send get str to server:\n%s", buf);
    write_to_socket(sk_fd, buf);
    shutdown(sk_fd, SHUT_WR);    
    
    fp = fdopen(sk_fd, "r");
    if(fp <=0){
        printf("open sk_fd(%d) error=%d\n", errno);
    }
    newfp = fopen(FILE_NAME, "w+");
    
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, 1023, fp))
    {
        printf("line=%s", buf);
        
        if (filesize != 0) /* find CRLF */
        {
            if (2 == strlen(buf))
            {
                break;
            }
        }
        else /* get file size */
        {
            pstr = strstr(buf, "Content-Length:");
            if (pstr != NULL)
            {
                pstr = strchr(pstr, ':');
                pstr++;
                filesize = strtoul(pstr, NULL, 10);
            }
        }
        memset(buf, 0x00, sizeof(buf));
    }
    if(filesize == 0){
        printf("fgets error=%d\n", ferror(fp));
    }
    printf("\nStep2: start get file, total_size=%d:\n", filesize);
    
    while (filesize)
    {
        size = fread(buf, 1, 1024*32, fp);
        sum += size;
        if (0 == size)
        {   if(ferror(fp) != 0)
            {
                printf("read file error\n");
            }
            break;
        }
        size = fwrite(buf, 1, size, newfp);
        processbar = (float)sum / (float)filesize;
		if ((unsigned char)(processbar * 100) >= (last_progress + 1))
		{
			printf("%6d/%lu[%2.1f]\n", sum, filesize, (processbar * 100));
			last_progress = (unsigned char)(processbar * 100);
		}
        filesize -= size;
        sleep(1);
    }
    fclose(fp);
    fclose(newfp);
    
    return 0;
}
