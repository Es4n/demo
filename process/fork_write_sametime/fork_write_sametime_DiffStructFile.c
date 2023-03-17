#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	/* 思路：
	 * 两个进程进行循环，先printf后write
	 * 重定向标准输出，printf到log文件，write到test_fork文件
	 * 比较两个文件的内容
	 * 
	 * 前提条件：可知printf是线程安全的
	 *
	 * 结果：write的数量比printf的数量少，说明相互write被互相覆盖数据了
	 * log中有200条信息，但test_fork文件中只有100条信息
     *
     * 可以观察到,在进程切换的时候,输出的消息就会乱,通过实验,多线程反而没有这种情况
     * 猜测是在进程切换的时候,会像进程退出一样,要求内核将对未写入的数据作一个处理
	 * */

	//重定位标准输出到log文件，方便分析
	int log_fd = open("log",O_RDWR | O_CREAT | O_TRUNC,0777);
	dup2(log_fd,STDOUT_FILENO);

	//创建子进程，两个进程同时向两个文件写入，但printf是线程安全的，write未知
	int pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return -1;
	}else if(pid == 0)
	{
		int fd = open("test_fork",O_RDWR | O_CREAT,0777);
		char str[] = "I am a child,abndefghijklmno\n";

		for(int i = 0;i < 100;i++)
		{
			printf("%s",str);  //printf到log文件
			write(fd,str,strlen(str));  //write到test_fork文件
		}
	}else
	{
		int fd = open("test_fork",O_RDWR | O_CREAT,0777);
		char str[] = "I am a parent,pqrstuvwxyz\n";

		for(int i = 0;i < 100;i++)
		{
			printf("%s",str); //printf到log文件
			write(fd,str,strlen(str)); //write到test_fork文件
		}
	}

	return 0;
}
