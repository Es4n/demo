#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

/* 用管道观察select用于write的阻塞情况
 * 在父进程中,建立一个线程往管道中写数据,直到写满为之
 * 然后父进程主线程再对管道的读端进行write的select,观察是否阻塞
 * 父进程调用select同时,向子进程发送信号SIGUSR1
 * 子进程捕捉SIGUSR1并在3秒后向管道读取数据,让管道腾出空间
 * 当管道中有数据后,父进程的selcet就会解除阻塞
 * */

int pipefd[2];

void read_pipe(int sig)
{
    // 此时父进程的线程已经将管道写满了
    // 延迟3秒后从管道读取数据,解除select的阻塞
    for(int i = 0; i < 3; i++)
    {
        printf("read after %d\n", i);
        sleep(1);
    }
    char *re = (char *)malloc(9192);
    read(pipefd[0], re, 9192);
    printf("read complete\n");
}

void *write_pipe(void *arg)
{
    printf("in thread\n");
    char *full = (char *)malloc(4096);
    memset(full, 1, 4096);
    printf("malloc success\n");

    // 重复写入数据直到管道塞满
    for(int i = 0; i < 17; i++)
    {
        if(write(pipefd[1], full, 4096) == 4096)
            printf("write 4096 %d\n", i);
    }

    // 管道数据写满后write会阻塞,该语句不会执行
    printf("write complete\n");
}

int main()
{
    if(pipe(pipefd) < 0)
    {
        perror("pipe");
        return -1;
    }

    int pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return 0;
    } else if(pid == 0)
    {
        //捕捉父进程发来的信号
        signal(SIGUSR1, read_pipe);
        printf("set signal complete\n");
        pause();
    } else
    {
        // 创建线程往管道中写入数据
        pthread_t tid;
        pthread_create(&tid, NULL, write_pipe, NULL);

        // 等写入线程写满管道后,也就是不打印数据后,主线程进行select,观察是否阻塞
        printf("wait here\n");
        getchar();
        //向子进程写入数据,3秒后子进程读取数据,解除select的阻塞
        kill(pid, SIGUSR1);

        // 设置监听集合
        fd_set listen;
        char re[10];
        FD_ZERO(&listen);
        FD_SET(pipefd[1], &listen);
        printf("wait write\n");

        // 用select监听管道写入端,观察是否阻塞
        select(pipefd[1] + 1, NULL, &listen, NULL, NULL);

        //等子进程读取数据后,就会解除阻塞
        printf("write ready\n");
    }
}
