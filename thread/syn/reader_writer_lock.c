#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define max_reader 1000  //读的次数
#define max_writer 1000 //写的次数
#define readernum 5  //读者的数量
#define writernum 2  //写者的数量
#define read_write_num 5

pthread_mutex_t Reader_Writer_lock,  //读写锁
				Reader_count_lock,  //读取读者数量锁
				writer_wait_lock;  //写者等待标志锁

int writer_wait = 0; //写者等待数量
int readingnum = 0;  //现有读者数量

//上读者锁
void reader_lock()
{
	while(1)  //自旋等待写者完成写入,因为没有条件变量或信号量,所以只能自旋
	{
		pthread_mutex_lock(&writer_wait_lock);
		if(writer_wait == 0)
		{
			pthread_mutex_unlock(&writer_wait_lock);
			break;
		}
		pthread_mutex_unlock(&writer_wait_lock);
	}

	pthread_mutex_lock(&Reader_count_lock);
	if(readingnum == 0)  //第一个读者获得读写锁
	{
		pthread_mutex_lock(&Reader_Writer_lock);
	}
	readingnum++;
	pthread_mutex_unlock(&Reader_count_lock);
}

//释放读者锁
void reader_unlock()
{
	pthread_mutex_lock(&Reader_count_lock);
	if(readingnum == 1)  //最后一个读者释放读写锁
	{
		pthread_mutex_unlock(&Reader_Writer_lock);
	}
	readingnum--;
	pthread_mutex_unlock(&Reader_count_lock);
}

//上写者锁
void writer_lock()
{
	pthread_mutex_lock(&writer_wait_lock);
	writer_wait++;
	pthread_mutex_unlock(&writer_wait_lock);

	printf("a writer wait\n");
	pthread_mutex_lock(&Reader_Writer_lock);
}

//释放写者锁
void writer_unlock()
{
	pthread_mutex_lock(&writer_wait_lock);
	writer_wait--;
	pthread_mutex_unlock(&writer_wait_lock);

	pthread_mutex_unlock(&Reader_Writer_lock);
	printf("a writer complete\n");
}

//读者线程
void *reader(void *arg)
{
	for(int i = 0;i < max_reader;i++)
	{
		reader_lock();
		printf("I am reader %ld,read %d\n",(long int)arg,i);
		reader_unlock();
	}
}

//写者线程
void *writer(void *arg)
{
	srand(time(NULL));
	
	for(int i = 0;i < max_writer;i++)
	{
		writer_lock();
		printf("I am writer %ld,write %d\n",(long int)arg,i);
		writer_unlock();
		usleep((rand()%10) * 100);
	}
}

int main()
{
	printf("test1\n");
	pthread_t reader_t[readernum];
	pthread_t writer_t[writernum];

	//创建锁
	pthread_mutex_init(&Reader_Writer_lock,NULL);
	pthread_mutex_init(&Reader_count_lock,NULL);
	pthread_mutex_init(&writer_wait_lock,NULL);

	////创建多个读者写者
	//for(long int i = 0;i < readernum;i++)
	//{
	//	pthread_create(&reader_t[i],NULL,reader,(void *)(i + 1));
	//}
	//for(long int i = 0;i < writernum;i++)
	//{
	//	pthread_create(&writer_t[i],NULL,writer,(void *)(i + 1));
	//}
	
	//一边创建读者,一边创建写者
	for(long int i = 0;i < read_write_num;i++)
	{
		pthread_create(&reader_t[i],NULL,reader,(void *)(i + 1));
		pthread_create(&writer_t[i],NULL,writer,(void *)(i + 1));
	}

	//等待读者写者完成
	for(int i = 0;i < writernum;i++)
	{
		pthread_join(writer_t[i],NULL);
	}
	for(int i = 0;i < readernum;i++)
	{
		pthread_join(reader_t[i],NULL);
	}

	//摧毁锁
	pthread_mutex_destroy(&Reader_Writer_lock);
	pthread_mutex_destroy(&Reader_count_lock);
	pthread_mutex_destroy(&writer_wait_lock);
}
