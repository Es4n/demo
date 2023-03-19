#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/* 演示死锁
 * 设置两个锁lock1和lock2
 * thread1先获取lock1后获取lock2
 * thread2先获取lock2后获取lock1
 *
 * 当thread1只获取了lock1,thread2只获取了lock2,那么死锁就产生了
 * 如果两个线程完整获取了两个锁,就不会产生死锁
 * */

pthread_mutex_t lock1, lock2;

void *thread1(void *arg)
{
    for(int i = 0; i < 100000000; i++)
    {
        pthread_mutex_lock(&lock1);

        printf("thread1 take lock1\n");

        pthread_mutex_lock(&lock2);

        printf("thread1 take lock2\n");

        pthread_mutex_unlock(&lock1);

        printf("thread1 release lock1\n");

        pthread_mutex_unlock(&lock2);

        printf("thread1 release lock2\n");
    }
}

void *thread2(void *arg)
{
    for(int i = 0; i < 100000000; i++)
    {
        pthread_mutex_lock(&lock2);

        printf("thread2 take lock2\n");

        pthread_mutex_lock(&lock1);

        printf("thread2 take lock1\n");

        pthread_mutex_unlock(&lock2);

        printf("thread2 release lock2\n");

        pthread_mutex_unlock(&lock1);

        printf("thread2 release lock1\n");
    }
}

int main()
{
    pthread_t tid1, tid2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&tid1,NULL,thread1,NULL);
    pthread_create(&tid2,NULL,thread2,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    printf("no detach lock\n");

    return 0;
}
