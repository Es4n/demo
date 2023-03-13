#include <pthread.h>
#include <stdio.h>

#define MAX 10000000


/* 不用锁的多线程计数器,会使计算结果混乱 */

int count = 0;

void *pthread_add(void *arg)
{
    for(int i = 0;i < MAX;i++)
    {
        count++;
    }

	return (void *)(long int)count;
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
