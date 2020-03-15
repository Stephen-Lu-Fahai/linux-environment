/*
 ============================================================================
 Name        : msg_queue.c
 Author      : stephen
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/msg.h>


typedef struct {
	long mtype;
	int data;
}msg_t;


void *send_task_proc(void)
{
	int i = 0;

	int msg_id = msgget(1234, IPC_CREAT | 0666);
	if (msg_id != -1)
	{
		msg_t msg_send;
		msg_send.mtype = 1;

		while(1)
		{
			msg_send.data = i++;
			msgsnd(msg_id, &msg_send, sizeof(msg_send.data), 0);
			printf("send %d\n", msg_send.data);
			sleep(3);
		}

	}


}

void *recev_task_proc(void)
{
	int msg_id = msgget(1234, IPC_CREAT | 0666);
	if (msg_id != -1)
	{
		msg_t msg_recev;

		while(1)
		{
			msgrcv(msg_id, &msg_recev, sizeof(msg_recev.data), 1, IPC_NOWAIT);
			printf("receive %d\n", msg_recev.data);
			sleep(2);
		}

	}

}

int main(void) {

	int ret;

	pthread_t send_thread;
	pthread_t recev_thread;


	ret = pthread_create(&send_thread, NULL, (void*)send_task_proc, NULL);
	if (ret)
		printf("failed to create send thread\n");
	pthread_detach(send_thread);

	ret = pthread_create(&recev_thread, NULL, (void*)recev_task_proc, NULL);
	if (ret)
		printf("failed to create receive thread\n");
	pthread_detach(recev_thread);

	while(1)
	{
		sleep(1);
	}

	return EXIT_SUCCESS;
}
