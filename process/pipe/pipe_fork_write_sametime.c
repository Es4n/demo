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

/* 用管道作为进程同步工具
 * 如果在打开文件后再创建子进程,父进程的文件描述符会复制到子进程
 * 因此父子进程共享同一个File结构体,也就是共享文件的偏移量
 * 因此父子进程向同一个文件同时输入数据不会互相践踏
 *
 * 该程序为创建了子进程后,在父子进程内分别打开同一份文件,不共享file文件夹
 * 因此如果同时写入文件会产生数据践踏
 *
 * 此时使用pipe进行进程间同步的工具,验证效果
 * */

int main()
{
	printf("test 5\n");

	//重定位标准输出到log文件，方便分析
	int log_fd = open("log",O_RDWR | O_CREAT | O_TRUNC,0777);
	dup2(log_fd,STDOUT_FILENO);

	//创建管道,需要两条管道,因为一条管道只能向一方通信
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
        //在进程内打开文件,也就是不与父进程共享文件描述符
		int fd = open("test_fork",O_RDWR | O_CREAT,0777);
		char *str = "I am a child,abndefghijklmno\n";
		char re_read;

		//写管道关闭读端，读管道关系写端
		close(c_for_p[0]);
		close(p_for_c[1]);

		lseek(fd,strlen("I am a parent,pqrstuvwxyz\n"),SEEK_CUR);

        //循环与子进程向同一个文件打印信息,用管道同步,防止互相践踏
		for(int i = 0;i < MAX;i++)
		{
            //用管道作为同步工具,从管道接受到消息才能接触阻塞
			read(p_for_c[0],&re_read,1); //子进程先读，阻塞
			if(re_read != 'p')
			{
				printf("chile read fail\n");
				return -1;
			}
			re_read = '0';  //读取完成后重置,以防不能更新

			printf("%s",str);  //printf到log文件
			write(fd,str,strlen(str));  //write到test_fork文件
			lseek(fd,strlen("I am a parent,pqrstuvwxyz\n"),SEEK_CUR);

			write(c_for_p[1],"c",1);
		}
	}else
	{
        //在进程内打开文件,也就是不与子进程共享文件描述符
		int fd = open("test_fork",O_RDWR | O_CREAT,0777);
		char *str = "I am a parent,pqrstuvwxyz\n";
		char re_read;

		//写管道关闭读端，读管道关系写端
		close(p_for_c[0]);
		close(c_for_p[1]);

        //循环与子进程向同一个文件打印信息,用管道同步,防止互相践踏
		for(int i = 0;i < MAX;i++)
		{
			printf("%s",str);  //printf到log文件
			write(fd,str,strlen(str));  //write到test_fork文件
			lseek(fd,strlen("I am a child,abndefghijklmno\n"),SEEK_CUR);

            //用管道作为同步工具,从管道接受到消息才能接触阻塞
			write(p_for_c[1],"p",1); 
			read(c_for_p[0],&re_read,1);
			if(re_read != 'c')
			{
				printf("parent read fail\n");
				return -1;
			}
			re_read = '0'; //读取完成后重置,以防不能更新
		}

		wait(NULL);
	}

	return 0;
}
