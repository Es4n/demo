#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

//可以用ps -e看到，确实有两个进程在运行

int main()
{
	fork();

	for(int i = 0;i < 10;i++)
	{
		printf("%d\n",getpid());
		sleep(1);
	}

	return 0;
}
