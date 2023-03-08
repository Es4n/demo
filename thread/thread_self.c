#include <pthread.h>
#include <stdio.h>

int a;

void *printf_SelfTid(void *arg)
{
	a = 1234; 
	printf("thread tid:%lu\n",pthread_self());
	return (void *)&a;  //不能返回栈上的空间
}

int main()
{
	void *thread_re;
	pthread_t thread;

	printf("main tid:%lu\n",pthread_self());
	pthread_create(&thread,NULL,printf_SelfTid,NULL);  //需要研究线程的属性
	pthread_join(thread,&thread_re);
	printf("thread return:%d\n",*(int *)thread_re);

	return 0;
}
