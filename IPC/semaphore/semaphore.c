/*
 ============================================================================
 Name        : semaphore.c
 Author      : stephen
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <pthread.h>

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
};

static int sem_id = 0;

static int set_semvalue()
{
	//用于初始化信号量，在使用信号量前必须这样做
	union semun sem_union;

	sem_union.val = 1;
	if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
		return 0;
	return 1;
}

static void del_semvalue()
{
	//删除信号量
	union semun sem_union;

	if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore\n");
}

static int semaphore_p()
{
	//对信号量做减1操作，即等待P（sv）
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;//P()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return 0;
	}
	return 1;
}

static int semaphore_v()
{
	//这是一个释放操作，它使信号量变为可用，即发送信号V（sv）
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;//V()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_v failed\n");
		return 0;
	}
	return 1;
}


void *send_task_proc(void)
{
	char msg_data = 'X';
	int flag = 1;
	int i;
	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);

	if(flag == 1)
	{
		//程序第一次被调用，初始化信号量
		if(!set_semvalue())
		{
			fprintf(stderr, "Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}

		sleep(2);
	}
	for(i = 0; i < 10; ++i)
	{
		//进入临界区
		if(!semaphore_p())
			exit(EXIT_FAILURE);
		//向屏幕中输出数据
		printf("%c", msg_data);
		//清理缓冲区，然后休眠随机时间
		fflush(stdout);
		sleep(rand() % 3);
		//离开临界区前再一次向屏幕输出数据
		printf("%c", msg_data);
		fflush(stdout);
		//离开临界区，休眠随机时间后继续循环
		if(!semaphore_v())
			exit(EXIT_FAILURE);
		sleep(rand() % 2);
	}

	sleep(10);
	printf("\nsend thread - finished\n");

	return 0;
}

void *recev_task_proc(void)
{

	char msg_data = 'O';
	int flag = 1;
	int i;
	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);

	if(flag == 1)
	{
		//程序第一次被调用，初始化信号量
		if(!set_semvalue())
		{
			fprintf(stderr, "Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}

		sleep(2);
	}
	for(i = 0; i < 10; ++i)
	{
		//进入临界区
		if(!semaphore_p())
			exit(EXIT_FAILURE);
		//向屏幕中输出数据
		printf("%c", msg_data);
		//清理缓冲区，然后休眠随机时间
		fflush(stdout);
		sleep(rand() % 3);
		//离开临界区前再一次向屏幕输出数据
		printf("%c", msg_data);
		fflush(stdout);
		//离开临界区，休眠随机时间后继续循环
		if(!semaphore_v())
			exit(EXIT_FAILURE);
		sleep(rand() % 2);
	}

	sleep(10);
	printf("\nreceive thread - finished\n");

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
