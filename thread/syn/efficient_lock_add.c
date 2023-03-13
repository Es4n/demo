#include <pthread.h>
#include <stdio.h>

#define MAX 10000000
#define LOCAL_MAX 1000  //设置局部阈值为100

/* 使用了局部计数器的多线程计数器
 * 每个线程配备了一个计数器,当线程接受到调度时,现在本线程的局部计数器上面进行增加
 * 但局部计数器的数量超过阈值后,再将局部计数器的数值转移到全局计数器上
 * 这样作可以让大多数情况下被调度到的线程都不会轮空cpu时间片,增加了效率
 * */

int count = 0;  //全局计数器
pthread_mutex_t add_lock = PTHREAD_MUTEX_INITIALIZER;  //静态初始化锁

void *pthread_add(void *arg)
{
	int local_count = 0;
	
    for(int i = 0;i < MAX;i++)
    {
		local_count++;

		if(local_count == LOCAL_MAX)  //当局部计数器达到阈值,将局部计数器的值转移到全局计数器上
		{
			pthread_mutex_lock(&add_lock);
			count += local_count;
			pthread_mutex_unlock(&add_lock);
			local_count = 0;
		}
	}

	return (void *)(long int)count;  //本线程结束时的计数结果
}

int main()
{
	pthread_t thread1,thread2;
	void *re;

    //创建两个线程进行计数
	pthread_create(&thread1,NULL,pthread_add,NULL);
	pthread_create(&thread2,NULL,pthread_add,NULL);

	pthread_join(thread1,&re);
	printf("pthread1:%ld\n",(long int)re);
	pthread_join(thread2,&re);
	printf("pthread2:%ld\n",(long int)re);

    //打印结果
	printf("%d\n",count);

	return 0;
}
