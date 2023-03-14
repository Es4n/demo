#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define COUNT_MAX 5  //缓冲区大小为5
#define PRODUCER_CONSUMER_MAX 5  //生产者消费者数量
#define WORK_MAX 100  //生产和消费的次数

sem_t product,consume;
pthread_mutex_t count_lock;

int count = 0; // 资源数量

void *producer(void *arg)
{
    for(int i = 0;i < WORK_MAX;i++)
    {
        sem_wait(&product);  //用条件变量互斥等待操作,这是消费者与消费者之间的互斥 
        pthread_mutex_lock(&count_lock);  //用锁互斥操作资源,这是所有生产者与消费者之间的互斥
        count++;
        printf("I am producer %ld,do %d,count %d\n",(long int)arg,i,count);
        pthread_mutex_unlock(&count_lock);
        sem_post(&consume);
    }
}

void *consumer(void *arg)
{
    for(int i = 0;i < WORK_MAX;i++)
    {
        sem_wait(&consume); //用条件变量互斥等待操作,这是消费者与消费者之间的互斥 
        pthread_mutex_lock(&count_lock);  //用锁互斥操作资源,这是所有生产者与消费者之间的互斥
        count--;
        printf("I am consumer %ld,do %d,count %d\n",(long int)arg,i,count);
        pthread_mutex_unlock(&count_lock);
        sem_post(&product);
    }
}

int main()
{
    pthread_t producer_t[PRODUCER_CONSUMER_MAX],consumer_t[PRODUCER_CONSUMER_MAX];

    pthread_mutex_init(&count_lock, NULL);
    sem_init(&consume,0,0);  //可消费资源初始化为0
    sem_init(&product,0,COUNT_MAX);  //先生产5个资源

    for(long int i = 0;i < PRODUCER_CONSUMER_MAX;i++)
    {
        pthread_create(&producer_t[i],NULL,producer,(void *)i);
        pthread_create(&consumer_t[i],NULL,consumer,(void *)i);
    }

    for(int i = 0;i < PRODUCER_CONSUMER_MAX;i++)
    {
        pthread_join(producer_t[i],NULL);
        pthread_join(consumer_t[i],NULL);
    }

    printf("finish count %d\n",count);

    return 0;
}


