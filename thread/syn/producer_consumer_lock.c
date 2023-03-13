#include <stdio.h>
#include <pthread.h>

#define COUNT_MAX 5
#define producer_and_consumer_max 5

/* 使用锁实现生产者消费者
 * 分别创建3个生产者和3个消费者,分别进行300次生产和300次消费
 * 最终应该打印600条记录
 * 因此如果生产在或消费者轮空之后要退回,否则打印不足600条记录
 * */

pthread_mutex_t count_lock;

int count = 0;  //资源

void *producer(void *arg)
{
	for(int i = 0;i < 100;i++)
	{
		while(1)
		{
			pthread_mutex_lock(&count_lock);
			if(count == COUNT_MAX)  //资源最大值设为5,超过则不能再生产
			{
				pthread_mutex_unlock(&count_lock);
				continue;
			}
			count++;
			printf("producer %ld : %d\n",(long int)arg,count);
			pthread_mutex_unlock(&count_lock);
			break;
		}
	}
}

void *consumer(void *arg)
{
	for(int i = 0;i < 100;i++)
	{
		while(1)
		{
			pthread_mutex_lock(&count_lock);
			if(count == 0)  //资源等于0不能消费
			{
				pthread_mutex_unlock(&count_lock);
				continue;
			}
			count--;
			printf("consumer %ld : %d\n",(long int)arg,count);
			pthread_mutex_unlock(&count_lock);
			break;
		}
	}
}

int main()
{
	pthread_t producer_t[producer_and_consumer_max];
	pthread_t consumer_t[producer_and_consumer_max];

	pthread_mutex_init(&count_lock,NULL);
	
	//多个生产者消费者
	for(long int i = 0;i < producer_and_consumer_max;i++) 
	{
		pthread_create(&producer_t[i],NULL,producer,(void *)(i + 1));
		pthread_create(&consumer_t[i],NULL,consumer,(void *)(i + 1));
	}

	//等待所有线程结束
	for(int i = 0;i < 5;i++)  
	{
		pthread_join(producer_t[i],NULL);
		pthread_join(consumer_t[i],NULL);
	}

	pthread_mutex_destroy(&count_lock);

	printf("finish count : %d\n",count);  //打印最终的资源,预期应该是0

	return 0;
}
