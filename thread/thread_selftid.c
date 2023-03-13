#include <pthread.h>
#include <stdio.h>

/* 测试pthread_self函数,并测试线程函数的返回
 * */

int a;

void *printf_SelfTid(void *arg)
{
	a = 1234; 
	printf("thread tid:%lu\n",pthread_self());  //打印自身线程ID
	return (void *)&a;  //不能返回栈上的空间
}

int main()
{
	void *thread_re;
	pthread_t thread;

	printf("main tid:%lu\n",pthread_self());  //打印自身线程ID
	pthread_create(&thread,NULL,printf_SelfTid,NULL);  //需要研究线程的属性
	pthread_join(thread,&thread_re);
	printf("thread return:%d\n",*(int *)thread_re);

	return 0;
}
