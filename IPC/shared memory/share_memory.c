/*
 ============================================================================
 Name        : share_memory.c
 Author      : stephen
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <pthread.h>
#include <unistd.h>

#define SHM_SIZE	2048
#define SHM_MODE	0666

typedef struct {
	char name[20];
	int age;
	char gender;
}person_t;

void *send_task_proc(void)
{
	int shm_id = shmget((key_t)2345, sizeof(person_t), IPC_CREAT | SHM_MODE);

	person_t person;
	person.age = 24;
	person.gender = 'M';
	strcpy(person.name, "Stephen_Lu");


	if (shm_id != -1)
	{
		void *shm = shmat(shm_id, NULL, 0);

		if (shm != (void*)-1)
		{
			memcpy(shm, &person, sizeof(person));
			shmdt(shm);
		}
	}


	while(1)
	{
		sleep(3);
	}


}

void *recev_task_proc(void)
{

	int shm_id = shmget((key_t)2345, sizeof(person_t), IPC_CREAT | SHM_MODE);

	person_t person;

	while(1)
	{

		if (shm_id != -1)
		{
			void *shm = shmat(shm_id, NULL, 0);

			if (shm != (void*)-1)
			{

				memcpy(&person, shm, sizeof(person));
				printf("name: %s\n", person.name);
				printf("age: %d\n", person.age);
				printf("gender: %c\n", person.gender);
				printf("\n");
				shmdt(shm);
			}

		}

		sleep(2);
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
