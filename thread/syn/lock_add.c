#include <pthread.h>
#include <stdio.h>

#define MAX 10000000

/* 不使用局部计数器的多线程计数器
 * */

int count = 0;  //全局计数器
pthread_mutex_t add_lock = PTHREAD_MUTEX_INITIALIZER;

void *pthread_add(void *arg)
{
    for(int i = 0;i < MAX;i++)  //循环增加全局计数器
    {
		pthread_mutex_lock(&add_lock);
        count++;
		pthread_mutex_unlock(&add_lock);
    }

	return (void *)(long int)count;  //返回本线程的计算结果
}

int main()
{
    pthread_t thread1,thread2;
	void *re;

    pthread_create(&thread1,NULL,pthread_add,NULL);
    pthread_create(&thread2,NULL,pthread_add,NULL);

	pthread_join(thread1,&re);
	printf("pthread1:%ld\n",(long int)re);
	pthread_join(thread2,&re);
	printf("pthread2:%ld\n",(long int)re);

	printf("%d\n",count);

	return 0;
}
