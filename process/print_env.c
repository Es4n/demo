#include <stdio.h>
#include <stdlib.h>

/* 测试环境变量的例程
 * 先打印所有环境变量
 * 再打印SHELL的路径
 * */

int main()
{
	extern char **environ;
	printf("打印环境变量列表:\n");
	for(int i = 0;environ[i] != NULL;i++)
	{
		printf("%s\n",environ[i]);
	}
	printf("-------------------------------------\n");
	
	printf("搜索环境变量SHELL的路径:\n")
	char *str = getenv("SHELL");
	printf("%s\n",str);

	return 0;
}
