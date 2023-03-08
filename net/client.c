#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define MAXLINE 10
#define SERV_PORT 8000

int main(int argc,char *argv[])
{
	struct sockaddr_in servaddr,clientaddr;
	char buf[MAXLINE];
	int sockfd,listenfd,n;
	char *str;

	if(argc != 2)
	{
		fputs("usage: ./client message \n",stderr);
		exit(1);
	}
	str = argv[1];

	/*客户端绑定套接字*/
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&clientaddr,sizeof(clientaddr));
	clientaddr.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&clientaddr.sin_addr);
	clientaddr.sin_port   = htons(SERV_PORT + 1);
	
	bind(listenfd,(struct sockaddr *)&clientaddr,sizeof(clientaddr));  //绑定socket和sockaddr_in结构体,绑定后才能使用socket

	/*建立链接*/
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
	servaddr.sin_port   = htons(SERV_PORT);

	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	write(sockfd,str,strlen(str));

	n = read(sockfd,buf,MAXLINE);
	printf("Response from server:\n");
	printf("%s\n",buf);
	//write(STDOUT_FILENO,buf,n);

	//只要关闭了套接字,就可以重新使用
	close(sockfd);
	close(listenfd);
	return 0;
}
