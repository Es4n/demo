#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

/* 用管道测试select监听读取
 * 父进程用select监听管道读端
 * 子进程延时3秒后往管道中写入数据
 *
 * 父进程会阻塞到子进程写入数据
 * */

int main()
{
    int pipefd[2];

    if(pipe(pipefd) < 0)
    {
        perror("pipe");
        return -1;
    }

    int pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return -1;
    } else if(pid == 0)
    {
        printf("in child\n");
        // 子进程写,关闭读端
        close(pipefd[0]);

        // 延时3秒,往管道中写入数据,观察selcet是否会阻塞到数据写入
        for(int i = 0; i < 3; i++)
        {
            printf("%d\n", i);
            sleep(1);
        }
        write(pipefd[1], "123456789", 10);
        printf("write complete\n");

        // 关闭
        close(pipefd[1]);
    } else
    {
        printf("in parent\n");
        // 父进程读,关闭写端
        close(pipefd[1]);

        // 设置监听集合
        fd_set listen;
        char re[10];
        FD_ZERO(&listen);
        FD_SET(pipefd[0], &listen);

        // 监听管道读端,等待子进程写入
        printf("wait read");
        select(pipefd[0] + 1, &listen, NULL, NULL, NULL);
        printf("read ready\n");
        printf("read %ld,%s\n", read(pipefd[0], re, 10), re);

        // 关闭
        close(pipefd[0]);
    }
}
