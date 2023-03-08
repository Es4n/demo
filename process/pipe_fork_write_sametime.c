#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX 100

int main()
{
	printf("test 5\n");

	//重定位标准输出到log文件，方便分析
	int log_fd = open("log",O_RDWR | O_CREAT | O_TRUNC,0777);
	dup2(log_fd,STDOUT_FILENO);

	//创建管道
	int p_for_c[2];
	int c_for_p[2];
	pipe(p_for_c);
	pipe(c_for_p);

	//创建子进程，两个进程同时向两个文件写入，但printf是线程安全的，write未知
	int pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return -1;
	}else if(pid == 0)
	{
		int fd = open("test_fork",O_RDWR | O_CREAT,0777);
		char *str = "I am a child,abndefghijklmno\n";
		char re_read;

		//写管道关闭读端，读管道关系写端
		close(c_for_p[0]);
		close(p_for_c[1]);

		lseek(fd,strlen("I am a parent,pqrstuvwxyz\n"),SEEK_CUR);
		for(int i = 0;i < MAX;i++)
		{
			read(p_for_c[0],&re_read,1); //子进程先读，阻塞
			if(re_read != 'p')
			{
				printf("chile read fail\n");
				return -1;
			}
			re_read = '0';
			printf("%s",str);  //printf到log文件
			write(fd,str,strlen(str));  //write到test_fork文件
			lseek(fd,strlen("I am a parent,pqrstuvwxyz\n"),SEEK_CUR);
			write(c_for_p[1],"c",1);
		}
	}else
	{
		int fd = open("test_fork",O_RDWR | O_CREAT,0777);
		char *str = "I am a parent,pqrstuvwxyz\n";
		char re_read;

		//写管道关闭读端，读管道关系写端
		close(p_for_c[0]);
		close(c_for_p[1]);

		for(int i = 0;i < MAX;i++)
		{
			printf("%s",str);  //printf到log文件
			write(fd,str,strlen(str));  //write到test_fork文件
			lseek(fd,strlen("I am a child,abndefghijklmno\n"),SEEK_CUR);
			write(p_for_c[1],"p",1); 
			read(c_for_p[0],&re_read,1);
			if(re_read != 'c')
			{
				printf("parent read fail\n");
				return -1;
			}
			re_read = '0';
		}

		wait(NULL);
	}

	return 0;
}
