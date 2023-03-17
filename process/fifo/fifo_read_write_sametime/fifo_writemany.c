#include <error.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define LOG "test1\n"

/* 测试管道边读边写--读端
 *
 * 管道写端有一个进程在写,读端有两个进程在读
 * 读到的数据写进对应的文件,看是否会乱序
 * 并且记录每次读取到的数据的记录,看是否每次读到的数据都和请求的一样
 *
 * 结果:读取不会乱序,每次读取都是4096
 * */

int fifofd;

//捕捉写无读段发出的信号,关闭管道,删除文件
void pwriteerror(int arg)
{
    printf("write no read\n");
    close(fifofd);
    unlink("/home/sen/test_fifo");
}

int main()
{
    //打印实验的次数,看有没有编译成功
    printf(LOG);

    //捕捉写入无读端的管道发出的信号
    signal(SIGPIPE, pwriteerror);

    umask(0000);

    //先判定管道是否存在,不存在则创建
    if(access("/home/sen/test_fifo", F_OK))
    {
        if(mkfifo("/home/sen/test_fifo", 0777) == -1)
        {
            perror("mkfifo");
            return -1;
        }
    }

    //只写打开管道,如果没有其他进程只读打开管道,则阻塞
    fifofd = open("/home/sen/test_fifo", O_WRONLY);
    if(fifofd == -1)
    {
        perror("open fifo");
        return -1;
    }
    printf("write fifo open\n");

    //创建4096大小的数据块,用于跟上读取的速度
    char *wr = (char *)malloc(4096);
    memset(wr,0,4096);
    int readmefd = open("README", O_RDWR | O_CREAT, 0777);
    if(readmefd == -1)
    {
        perror("open README\n");
        return -1;
    }
    for(int i = 0;i < 128;i++)
    {
        write(readmefd, "12345abcdefghijklmnopqrstuvwxyz\n", 32);
    }
    lseek(readmefd,0,SEEK_SET);
    printf("write ready %ld\n",read(readmefd,wr,4096));
    if(close(readmefd) == 0)
        printf("close README\n");
    if(unlink("README") == 0)
        printf("delete README\n");
    
    printf("write wait here\n");

    //循环写入数据,读端两个进程一起读
    for(int i = 0; i < 10000; i++)
    {
        if(i % 1000 == 0)
            printf("%d\n", i);
        if(write(fifofd,wr,4096) == -1)
        {
            perror("write");
            return -1;
        }
    }
    printf("wtire complete\n");

    close(fifofd);
    unlink("/home/sen/test_fifo");

    return 0;
}
