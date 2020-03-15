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

	/***************data-generate**************/
	int i;
	const int len = 10;
	int buf[len];
	for (i = 0; i < len; i++)
		buf[i] = i;

	union
	{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];

	}ctrl;

	struct iovec iov[1];
	iov[0].iov_base = buf;
	iov[0].iov_len = len;

	struct msghdr msg;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = ctrl.control;
	msg.msg_controllen = sizeof(ctrl.control);
	msg.msg_flags = 0;


	int count = 0;
	while(1)
	{
		if(-1 == (client_sockfd = socket(AF_INET,SOCK_STREAM,0)))
		{
			return -1;
		}

		if(-1 == (connect(client_sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))))
		{
			return -1;
		}

		sendmsg(client_sockfd,&msg,0);
		close(client_sockfd);//关闭套接字

	 	sleep(1);
	}

}
