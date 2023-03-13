#include <pthread.h>
#include <stdio.h>

#define MAX 100
#define producer_and_consumer_max 5

pthread_cond_t produce_cond,consume_cond;
pthread_mutex_t produce_consume_lock;

int count = 0;

void *producer(void *arg)
{
	for(int i = 0;i < MAX;i++)
	{
		pthread_mutex_lock(&produce_consume_lock);
		while(count == 5)
			pthread_cond_wait(&produce_cond,&produce_consume_lock);
		count++;
		printf("I am producer %ld,count %d\n",(long int)arg,count);
		pthread_cond_signal(&consume_cond);
		pthread_mutex_unlock(&produce_consume_lock);
	}
}

void *consumer(void *arg)
{
	for(int i = 0;i < MAX;i++)
	{
		pthread_mutex_lock(&produce_consume_lock);
		while(count == 0)
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

