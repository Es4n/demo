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

/* 观察两个进程共用一个file结构体同时read一个文件,看数据是否会乱序
 *
 * 结果:不会乱序,而且根据log文件里面的结果分析,每次读到的都是4096字节
 * */

int main()
{
    printf(LOG);

    char *read_buf = (char *)malloc(4096);
    memset(read_buf, 0, 4096);
    int re = 0;

    /*创建并写入文件*/
    //创建文件
    umask(0000);
    int fd = open("README", O_RDWR | O_CREAT, 0777);
    printf("create file complete,fd: %d\n", fd);

    //先写入128次,也就是4096字节
    for(int i = 0; i < 128; i++)
    {
        write(fd, "12345abcdefghijklmnopqrstuvwxyz\n", 32);
    }

    //读取4096字节,再写入文件,减少write的调用次数
    lseek(fd, 0, SEEK_SET);
    read(fd, read_buf, 4096);
    for(int i = 0; i < 10000000 / 128 - 1; i++)
    {
        if(i % 100 == 0)
            printf("write %d\n", i);
        write(fd, read_buf, 4096);
    }
    printf("write complete\n");

    /*创建两个进程读取文件*/
    //将输出的信息输出到log文件,分析结果
    dup2(open("log", O_RDWR | O_CREAT, 0777), STDOUT_FILENO);
    //重置README的偏移
    lseek(fd,0,SEEK_SET);

    //两个进程共用一个file结构体,以4096为单位读取数据,看是否会乱序
    int pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return -1;
    } else if(pid == 0)
    {
        int log_child = open("log_child", O_RDWR | O_CREAT, 0777);

        //先读后写
        for(int i = 0; i < 10000; i++)
        {
            re = read(fd, read_buf, 4096);
            if(re == 0)
                break;
            //标准输入已经被重定向到了log文件中
            printf("chile read %d\n", re);
            //将读到的数据写入到本进程的记录文件中,分析结果
            write(log_child, read_buf, re);
        }

        //关闭
        close(log_child);
        close(fd);
        free(read_buf);

        return 0;
    } else
    {
        int log_parent = open("log_parent", O_RDWR | O_CREAT, 0777);

        //先读后写
        for(int i = 0; i < 10000; i++)
        {
            re = read(fd, read_buf, 4096);
            if(re == 0)
                break;
            //标准输入已经被重定向到了log文件中
            printf("parent read %d\n", re);
            //将读到的数据写入到本进程的记录文件中,分析结果
            write(log_parent, read_buf, re);
        }

        //关闭
        close(log_parent);
        close(fd);
        free(read_buf);
        wait(NULL);

        return 0;
    }
}
