#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

/* 观察多线程的进程调用fork
 *
 * 预期结果:两个进程各打印100条记录
 *
 * 结果:除了调用fork的线程,进程中的其他线程不会被复制到子进程
 * */

pid_t ppid;

void *test_pthread(void *arg)
{
    /* 主线程可能在该线程的任意地方打断,然后调用fork,所以要做好应对
     * 
     * 具体措施为:
     * 1.在每次循环中加入判断,判断是否与原进程ID一致
     *   但每次都循环都调用getpid函数会降低效率,所以使用标志位flag
     *   运用短路原理使得子进程不用每次循环都调用函数
     * 2.但flag为1,就会退出里层循环,此时i < 1000,在外层循环会执行!(i = 0)重设i,再进行1000次循环
     *   而如果是里层循环1000次执行完毕返回到外层循环,则不会执行!(i = 0)
     * */

    for(int flag = 0,i = 0;i < 1000 && !(i = 0);)  //对于循环足够1000次的可以正常退出,否则重设i进行1000次循环
    {
        for(;i < 1000;i++)
        {
            if(flag == 0 && getpid() != ppid)  //如果当前pid和原pid不同则退出循环
            {
                printf("change flag\n");
                flag = 1;
                break;  //在外层循环重设i
            }

            printf("pid %d,tid %ld,i %d\n",getpid(),pthread_self(),i);
            usleep(100);
        }
    }
}

int main()
{
    pthread_t tid;

    ppid = getpid();  //fork之前记录原pid

    pthread_create(&tid,NULL,test_pthread,NULL);  //创建计数线程

    pid_t pid = fork(); 
    if(pid < 0)
    {
        perror("fork");
        return -1;
    }else if(pid == 0)
    {
        printf("I am chile\n");
    }else 
    {
        printf("I am parent\n");
    }

    pthread_join(tid,NULL);

    printf("pid %d complete\n",getpid());

    return 0;
}
