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

int main(void)
{
	struct sockaddr_in servaddr,cliaddr;  //sockaddr_in说明是IPV4通信用的结构体，问题：里面装的是什么？
	socklen_t cliaddr_len;
	int listenfd,connfd;  //套接字的“”文件描述符"
	char buf[MAXLINE];  //用来写入要传输的数据
	char str[INET_ADDRSTRLEN];  //用来接受网络上传过来的信息
	int i,n;

	//创建套接字,该套接字用于监听
	listenfd = socket(AF_INET,SOCK_STREAM,0);

	/*在地址结构体里面装入*/
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family      = AF_INET;  //说明是用IPV4通信
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //自动设置监听IP,小端转大端
	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");   //监听的ip地址，问题：为什么要设置监听地址，一个服务器可以监听多个地址吗?
	servaddr.sin_port        = htons(SERV_PORT);  //监听端口,小端转大端

	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));  //绑定socket和sockaddr_in结构体,绑定后才能使用socket

	listen(listenfd,20);  //用socket监听

	printf("Accepting connections ...\n");
	while(1)
	{
		cliaddr_len = sizeof(cliaddr);
		connfd      = accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddr_len);  //建立链接成功返回socket，相当于文件描述符使用

		n = read(connfd,buf,MAXLINE);  //像读取文件一样写入内容
		printf("received from %s at PORT %d\n",
				inet_ntop(AF_INET,&cliaddr.sin_addr,str,sizeof(str)),  //大端转小端，因为这个信息是从网络上传输过来的，是大端存放的数据
				ntohs(cliaddr.sin_port));  //大端转小端，因为这个信息是从网络上传输过来的，是大端存放的数据

		//进行大小写装换
		for(i = 0;i < n;i++)
		{
			buf[i] = toupper(buf[i]);
		}
		write(connfd,buf,n); //像读取文件一样写入内容
		close(connfd);
	}
}
