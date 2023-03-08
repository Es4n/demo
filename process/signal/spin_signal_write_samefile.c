#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int flag = 0;  //阻塞位，0代表不可打印，1代表可以打印

void set_flag(int sig)
{
	flag = 1;
}

int main()
{
	printf("test3\n");

	signal(SIGUSR1,set_flag);

	//重定位标准输出到log文件，方便分析
	int log_fd = open("log",O_RDWR | O_CREAT | O_TRUNC,0777);
	dup2(log_fd,STDOUT_FILENO);

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
			while(flag == 0);
			flag = 0;
			lseek(fd,strlen("I am a parent,pqrstuvwxyz\n"),SEEK_CUR);
			printf("%s",str);  //printf到log文件
			if(write(fd,str,strlen(str)) != strlen(str))
			{
				perror("parent write\n");
				return -1;
			}
			kill(getppid(),SIGUSR1);
		}
	}else
	{
		int fd = open("test_fork",O_RDWR | O_CREAT,0777);
		char str[] = "I am a parent,pqrstuvwxyz\n";
		flag = 1;

		for(int i = 0;i < 100;i++)
		{
			while(flag == 0);
			flag = 0;
			printf("%s",str); //printf到log文件
			if(write(fd,str,strlen(str)) != strlen(str))
			{
				perror("parent write\n");
				return -1;
			}
			lseek(fd,strlen("I am a child,abndefghijklmno\n"),SEEK_CUR);
			kill(pid,SIGUSR1);
		}
	}

	return 0;
}
