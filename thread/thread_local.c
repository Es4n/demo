#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* 分别打印线程局部变量的地址
 * 循环改变各个线程的局部变量
 * */

static __thread int test;

void *thread_local(void *arg)
{
	//打印线程局部变量的地址
	printf("%ld,%p\n",pthread_self(),&test);

	//循环改变本线程的局部变量的值
	for(int i = 0;i < 50;i++)
	{
		printf("t2:%d\n",test++);
		usleep(100000);
	}
}

int main()
{
	pthread_t tid;
	pthread_create(&tid,NULL,thread_local,NULL);
	
	//打印线程局部变量的地址
	printf("%ld,%p\n",pthread_self(),&test);
	
	//循环改变本线程的局部变量的值
	for(int i = 0;i < 50;i++)
	{
		printf("t1:%d\n",test++);
		usleep(100000);
	}

	pthread_join(tid,NULL);
}
