#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

/* 用信号和自旋作为进程同步工具
 * 如果在打开文件后再创建子进程,父进程的文件描述符会复制到子进程
 * 因此父子进程共享同一个File结构体,也就是共享文件的偏移量
 * 因此父子进程向同一个文件同时输入数据不会互相践踏
 *
 * 该程序为创建了子进程后,在父子进程内分别打开同一份文件,不共享file文件夹
 * 因此如果同时写入文件会产生数据践踏
 *
 * 此时使用信号和自旋进行进程间同步的工具,验证效果
 * */

int flag = 0;  //阻塞位，0代表不可打印，1代表可以打印

//收到信号将flag设置为1,代表可以打印
void set_flag(int sig)
{
	flag = 1;
}

int main()
{
	printf("test3\n");

    //注册信号处理函数
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
        //在进程内打开文件,也就是不与父进程共享文件描述符
		int fd = open("test_fork",O_RDWR | O_CREAT,0777);
		char str[] = "I am a child,abndefghijklmno\n";

        //循环与子进程向同一个文件打印信息,用信号和自旋同步,防止互相践踏
		for(int i = 0;i < 100;i++)
		{
            //如果flag为0,也就是没有受到信号,自旋
			while(flag == 0);
            //结束自旋后设置flag为0,准备下一次自旋
			flag = 0;

            //向同一个文件打印数据
			lseek(fd,strlen("I am a parent,pqrstuvwxyz\n"),SEEK_CUR);
			printf("%s",str);  //printf到log文件
			if(write(fd,str,strlen(str)) != strlen(str))
			{
				perror("parent write\n");
				return -1;
			}

            //打印结束后向另一个进程发送信号,自身自旋
			kill(getppid(),SIGUSR1);
		}
	}else
	{
        //在进程内打开文件,也就是不与父进程共享文件描述符
		int fd = open("test_fork",O_RDWR | O_CREAT,0777);
		char str[] = "I am a parent,pqrstuvwxyz\n";
		flag = 1;  //父进程flag位设置为1,也就是父进程先走

        //循环与子进程向同一个文件打印信息,用信号和自旋同步,防止互相践踏
		for(int i = 0;i < 100;i++)
		{
            //如果flag为0,也就是没有受到信号,自旋
			while(flag == 0);
            //结束自旋后设置flag为0,准备下一次自旋
			flag = 0;

            //向同一个文件打印数据
			printf("%s",str); //printf到log文件
			if(write(fd,str,strlen(str)) != strlen(str))
			{
				perror("parent write\n");
				return -1;
			}
			lseek(fd,strlen("I am a child,abndefghijklmno\n"),SEEK_CUR);

            //打印结束后向另一个进程发送信号,自身自旋
			kill(pid,SIGUSR1);
		}
	}

	return 0;
}
