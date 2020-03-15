#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
 
#define BUFFERSIZE 1024
int main(int argc,char **argv)
{
	int listenfd,connfd;
	
	struct sockaddr_in sockaddr;
	char buff[BUFFERSIZE];
	int n;
 
	memset(&sockaddr,0,sizeof(sockaddr));
 
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //绑定本机网卡
	sockaddr.sin_port = htons(8000);
 
	listenfd = socket(AF_INET,SOCK_STREAM,0);
 
	if(-1 == bind(listenfd,(struct sockaddr *) &sockaddr,sizeof(sockaddr)))
	{
		printf("bind error");
	}
	listen(listenfd,1024);
 
	while(1)
	{
		 if((connfd = accept(listenfd,(struct sockaddr*)NULL,NULL))==-1)
		 {
			 printf("accpet socket error: %s errno :%d\n",strerror(errno),errno);
			 continue;
		 }
		 
			 n = recv(connfd,buff,BUFFERSIZE,0);
			 buff[n] = '\0';
			 printf("recv msg from client:  %s\n",buff);
			 close(connfd);
	}
	 close(listenfd);
	
}
 
 
 
 
 