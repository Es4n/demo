#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*线程自己将自己设置为detach(分离属性)*/
void *detach_thread(void *arg)
{
	printf("this thread will detach1\n");
	pthread_detach(pthread_self()); 
	return (void *)1;
}

int main()
{
	pthread_t tid;

	pthread_create(&tid,NULL,detach_thread,NULL);
	sleep(2);
	
	if(pthread_join(tid,NULL) != 0)  //join一个detach的线程
		printf("can't join a detach thread\n");  //提示错误
	
	return 0;
}
