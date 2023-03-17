#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

#define LOG "test1\n"

/* 测试管道边读边写--读端
 *
 * 管道写端有一个进程在写,读端有两个进程在读
 * 读到的数据写进对应的文件,看是否会乱序
 * 并且记录每次读取到的数据的记录,看是否每次读到的数据都和请求的一样
 *
 * 结果:读取不会乱序,每次读取都是4096
 * */

int main()
{
    //打印实验的次数,看有没有编译成功
    printf(LOG);

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

    //只读打开管道,如果没有其他进程只写打开管道,则阻塞
    int fifofd = open("/home/sen/test_fifo", O_RDONLY);
    if(fifofd == -1)
    {
        perror("open fifo");
        return -1;
    }

    //创建用于分析记录的文件
    int log1fd = open("log1", O_WRONLY | O_CREAT);
    int log2fd = open("log2", O_WRONLY | O_CREAT);
    printf("read wait here\n");
    //将输出重定向到log文件,用于分析
    dup2(open("log",O_RDWR | O_CREAT),STDOUT_FILENO);

    //创建两个进程进行读取,观察读取结果是否会乱序
    char *re = (char *)malloc(4096);
    memset(re, 0, 4096);
    int pid = fork();
    if(pid < 0)
    {
        perror("fork\n");
        return -1;
    } else if(pid == 0)
    {
        //从管道读取数据,然后将读取到的数据写入log文件,同时将读取到的数量写入
        for(int i = 0; i < 5100; i++)
        {
            memset(re, 0, 4096);
            int flag = read(fifofd, re, 4096);
            printf("chile %d\n", flag);
            if(flag == -1)
            {
                perror("read -1\n");
                return -1;
            } else if(flag == 0)
            {
                printf("read 0\n");
                break;
            }
            //将读到的数据写入log文件
            write(log1fd, re, flag);
        }
        printf("read complete chile\n");

        //关闭
        close(fifofd);
        close(log1fd);
    } else
    {
        //从管道读取数据,然后将读取到的数据写入log文件,同时将读取到的数量写入
        for(int i = 0; i < 5100; i++)
        {
            memset(re, 0, 4096);
            int flag = read(fifofd, re, 4096);
            printf("parent %d\n", flag);
            if(flag == -1)
            {
                perror("read -1\n");
                return -1;
            } else if(flag == 0)
            {
                printf("read 0\n");
                break;
            }
            //将读到的数据写入log文件
            write(log2fd, re, flag);
        }
        printf("read complete parent\n");

        //关闭
        close(fifofd);
        close(log2fd);

        //等子进程结束
        wait(NULL);
    }

    unlink("/home/sen/test_fifo");

    return 0;
}
