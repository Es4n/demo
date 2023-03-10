#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX 40

/* 程序思路：
 * 子进程等待信号，父进程发送信号后，子进程会进入信号处理函数
 * 信号处理函数是一个循环，在循环结束以前，父进程给子进程发送第二个信号
 * 
 * 观察：子进程完成信号处理函数之后从内核返回，是否会处理正在处理第一个信号时接受到的第二个信号
 * 
 * 现象：会处理,但只会处理一个待处理信号,也就是说,如果在信号处理的过程中,接受到了多个同类型的信号,等信号处理结束后,只会处理一个
 * */

void wait_signal(int sig)
{
	int i = MAX;
	printf("recive signal\n");
	while(i)
	{
		printf("%d\n",i--);
		usleep(100000);
	}
}

int main()
{
	printf("test 6\n");

	signal(SIGUSR1,wait_signal);

	int pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return -1;
	}else if(pid == 0)
	{
		//等待信号
		pause();
		printf("no signal\n");
		return 0;
	}else
	{
		//发送三个同类信号,子进程会处理一个,待处理一个,其他丢弃
		for(int i = 0;i < 3;i++)
		{
			printf("send signal %d\n",i);
			kill(pid,SIGUSR1);
			usleep(100000);
		}
		printf("send signal complete\n");
		wait(NULL);
	}
	return 0;
}
