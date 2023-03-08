#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

//可以用cat test_fork发现，两个进程输入的数据不会乱，可以并发执行
//因为共用一个file结构体

int main()
{
	int fd = open("test_fork",O_RDWR | O_CREAT | O_TRUNC,0777);

	int pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return -1;
	}else if(pid == 0)
	{
		for(int i = 0;i < 100;i++)
		{
			printf("i am a chile\n");
			write(fd,"i am a child,abndefghijklmno\n",
					sizeof("i am a child,abndefghijklmno\n"));
		}
	}else
	{
		for(int i = 0;i < 100;i++)
		{
			printf("i am a parent\n");
			write(fd,"i am a parent,pqrstuvwxyz\n",
					sizeof("i am a parent,pqrstuvwxyz\n"));
		}
	}

	return 0;
}
