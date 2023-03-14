#include <pthread.h>
#include <stdio.h>

#define MAX 100
#define producer_and_consumer_max 5

/* 使用条件变量配合锁实现生产者消费者
 * 在只用锁的版本中,但系统进行了不合理的调度,导致会在资源满仓或空仓
 * 的情况下调度到不应该调度的生产者或消费者,此时因为没有条件变量
 * 所以只能自旋
 * 
 * 当使用了条件变量之后,可以在调度到不合适的生产者或者消费者时让
 * 其进行休眠,干扰了调度顺序,从而提高效率
 * */

pthread_cond_t produce_cond,consume_cond;  //注意这里要使用两个条件变量,非别指向消费者和生产者,这样才能定向唤醒
pthread_mutex_t produce_consume_lock;

int count = 0;

void *producer(void *arg)
{
    //每个生产者进行100次生产,如果资源满了就休眠
	for(int i = 0;i < MAX;i++)
	{
		pthread_mutex_lock(&produce_consume_lock);
		while(count == 5)  //资源满了就休眠,注意这里要用while
			pthread_cond_wait(&produce_cond,&produce_consume_lock);

		count++;
		printf("I am producer %ld,count %d\n",(long int)arg,count);
		pthread_cond_signal(&consume_cond);
		pthread_mutex_unlock(&produce_consume_lock);
	}
}

void *consumer(void *arg)
{
    //每个消费者进行100次消费,如果资源为空就休眠
	for(int i = 0;i < MAX;i++)
	{
		pthread_mutex_lock(&produce_consume_lock);
		while(count == 0)  //资源为空就休眠,注意这里要用while
			pthread_cond_wait(&consume_cond,&produce_consume_lock);

		count--;
		printf("I am consumer %ld,count %d\n",(long int)arg,count);
		pthread_cond_signal(&produce_cond);
		pthread_mutex_unlock(&produce_consume_lock);
	}
}

int main()
{
	pthread_t producer_t[producer_and_consumer_max],
				consumer_t[producer_and_consumer_max];

	pthread_cond_init(&produce_cond,NULL);
	pthread_cond_init(&consume_cond,NULL);
	pthread_mutex_init(&produce_consume_lock,NULL);

	for(long int i = 0;i < producer_and_consumer_max;i++)
	{
		pthread_create(&producer_t[i],NULL,producer,(void *)i);
		pthread_create(&consumer_t[i],NULL,consumer,(void *)i);
	}

	for(long int i = 0;i < producer_and_consumer_max;i++)
	{
		pthread_join(producer_t[i],NULL);
		pthread_join(consumer_t[i],NULL);
	}

	printf("finish %d\n",count);

}

