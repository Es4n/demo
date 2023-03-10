#include <pthread.h>
#include <stdio.h>
#include <string.h>

/* 测试获取线程函数的返回值
 * 如果主函数参数输入"return",则线程函数用return退出,返回值为"return",主线程打印
 * 如果主函数参数输入"exit",则线程函数用exit退出,返回值为"exit",主线程打印
 * */

void *print(void *arg)
{
	if(strcmp((char *)arg,"return") == 0)
	{
		return (void *)"return";
	}else if(strcmp((char *)arg,"exit") == 0)
	{
		pthread_exit((void *)"exit");
	}else
	{
		return (void *)"error";
	}
}

int main(int argc,char *argv[])
{
	char *str;
	pthread_t tid;

	pthread_create(&tid,NULL,print,(void *)argv[1]);

	pthread_join(tid,(void **)&str);

	printf("%s\n",str);

	return 0;
}
