#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

/* 该例程用来验证select是否可以进行流式读取
 *
 * 流式读取:
 * 输入方将数据输入到流中,读取方每次可以读取任意数量的数据,以任意次数将数据提取完
 *
 * 实验流程:
 * 创建管道后,创建两个进程:父进程先向管道中写入数据,子进程后从管道读取数据
 *
 * 两个进程用信号进行同步
 * 父进程先写入数据,写入完成后唤醒子进程读取数据
 * 子进程读取数据分多次读取,看select是否能解除多次阻塞
 * */

int flag = 1; // 控制子进程自旋等待
// 信号捕捉函数,子进程用来捕捉父进程的唤醒信号
void fun(int sig)
{
    printf("start to read\n");
    flag = 0;
}

int main()
{
    // 创建管道
    if(mkfifo("/home/sen/test_fifo", 0666) < 0)
    {
        perror("mkfifo");
        return -1;
    }

    // 创建进程
    int pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return 0;
    } else if(pid == 0) // 子进程
    {
        // 注册函数等待唤醒
        signal(SIGUSR1, fun);

        // 子进程读打开管道
        int fd = open("/home/sen/test_fifo", O_RDONLY);
        if(fd < 0)
        {
            perror("open fifo");
            return -1;
        }
        printf("child open success\n");

        // 自旋等待父进程写入信息
        while(flag)
            ;

        // 开始用select进行阻塞读取
        fd_set listen;
        fd_set listen_copy;
        FD_ZERO(&listen);
        FD_SET(fd, &listen); // 监听输入

        char str[10] = {0};        // 存放读取到的数据的buf
        for(int i = 0; i < 3; i++) // 分3次读取
        {
            listen_copy = listen;        // 每次重设监听集合
            memset(str, 0, sizeof(str)); // 清空上次的输入

            // 用select监听
            select(fd + 1, &listen_copy, NULL, NULL, NULL);

            // 解除阻塞,读取数据
            if(FD_ISSET(fd, &listen_copy))
            {
                read(fd, str, sizeof("aaa"));
                // 打印读取到的数据
                printf("%s\n", str);
            }
            printf("read %d\n", i + 1);
        }

        // 结束,关闭文件描述符
        close(fd);

    } else // 父进程
    {
        // 父进写
        int fd = open("/home/sen/test_fifo", O_WRONLY);
        if(fd < 0)
        {
            perror("open fifo");
            return -1;
        }
        printf("parent open success\n");

        // 写入三次
        for(int i = 0; i < 3; i++)
        {
            if(write(fd, "aaa", sizeof("aaa")) < 4)
            {
                perror("write");
            }
        }
        printf("write complete\n");

        // 发送信号唤醒子进程读取
        kill(pid, SIGUSR1);

        // 等待子进程结束
        wait(NULL);

        // 关闭文件描述符,删除管道
        close(fd);
        unlink("/home/sen/test_fifo");
    }
}
