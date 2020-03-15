#include<stdio.h>
#include <stdio.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <stdlib.h>
#include <stdint.h>
 
int main()
{
 
    int client_sockfd = -1;                           //创建套接字
	
	struct sockaddr_in server_addr; 				 //服务器端网络地址结构体  
	memset(&server_addr,0,sizeof(server_addr)); 	 //数据初始化--清零  
	server_addr.sin_family = AF_INET;				 
	server_addr.sin_addr.s_addr = inet_addr("192.168.0.18");  	 //服务器IP地址 
	server_addr.sin_port = htons(8000);
	
	if(-1 == (client_sockfd = socket(AF_INET,SOCK_STREAM,0)))  
	{    
		return -1;  
	}  
	if(-1 == (connect(client_sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))))
	{
		return -1;
	}

	send(client_sockfd,"hello_stephen",sizeof("hello_stephen"),0);

	send(client_sockfd,"goodbye",sizeof("goodbye"),0);
	close(client_sockfd);//关闭套接字
		
}