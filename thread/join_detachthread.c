#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *detach_thread(void *arg)
{
	printf("this thread will detach1\n");
	//pthread_detach(tid);  //可以自己detach自己吗?
	return (void *)1;
}

int main()
{
	pthread_t tid;

	pthread_create(&tid,NULL,detach_thread,NULL);
	sleep(2);
	
	pthread_detach(tid);  //先调用一次detach
	if(pthread_join(tid,NULL) != 0)  //join一个detach的线程
		printf("can't join a detach thread\n");  //提示错误
	
	return 0;
}
