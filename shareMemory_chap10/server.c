#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#define KEY_NAME_SEND "receive"
#define KEY_NAME_RECEIVE "send"
#define KEY_NAME_GET_ID "getId"
#define KEY_NAME_SET_ID "setId"

const int size = 8182;
int shm_fd_send = -1;
int shm_fd_receive = -1;
void *ptr_send = NULL;
void *ptr_receive = NULL;
char name[256];

int shm_fd_get_pid = -1;
int shm_fd_send_pid = -1;
void *ptr_send_pid = NULL;
void *ptr_receive_pid = NULL;
char pid_client[10];

int flag = 0;
int pid = 0;
int flagReceive = 0;
int flagSend = 0;
void siguser_handel()
{
	flagReceive = 1;
	flagSend = 1;
	printf("\nreceive from client:\n");
}

void *threadSend(void *arg)
{
	while (1)
	{
		while (flagSend)
		{
			shm_fd_send = shm_open(KEY_NAME_SEND, O_CREAT | O_RDWR, 0666);
			ftruncate(shm_fd_send, size);
			ptr_send = mmap(0, size, PROT_WRITE, MAP_SHARED, shm_fd_send, 0);
			memset(name, 0, sizeof(name));
			printf("\nsend to client:");
			scanf("%s", name);
			strcpy((char *)ptr_send, name);
			flag = 1;
			printf("pid of client: %d\n", pid);
			kill(pid, SIGUSR2);
			flagSend = 0;
		}
	}
}

void *threadReceive(void *arg)
{

	while (1)
	{
		if (flagReceive == 1)
		{
			/* open the shared memory object */
			shm_fd_receive = shm_open(KEY_NAME_RECEIVE, O_RDONLY, 0666);
			/* memory map the shared memory object */
			ptr_receive = mmap(0, size, PROT_READ, MAP_SHARED, shm_fd_receive, 0);

			/* read from the shared memory object */
			printf("\nserver: %s\n", (char *)ptr_receive);
			if (strlen((char *)ptr_receive) > 1)
			{
				/* remove the shared memory object */
				shm_unlink(KEY_NAME_RECEIVE);
				munmap(ptr_receive, size);
				ptr_receive = NULL;
				shm_fd_receive = -1;
				flagReceive = 0;
			//	kill(pid, SIGUSR2);
			}
		}
	}
}

void main()
{
	signal(SIGUSR1, siguser_handel);
	shm_fd_send_pid = shm_open(KEY_NAME_SET_ID, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd_send_pid, size);
	ptr_send_pid = mmap(0, size, PROT_WRITE, MAP_SHARED, shm_fd_send_pid, 0);
	sprintf(pid_client, "%d", getpid());
	printf("\npid of server%s", pid_client);
	strcpy((char *)ptr_send_pid, pid_client);
	shm_fd_get_pid = shm_open(KEY_NAME_GET_ID, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd_get_pid, size);
	while (1)
	{

		ptr_receive_pid = mmap(0, size, PROT_WRITE, MAP_SHARED, shm_fd_get_pid, 0);
		if (strlen((char *)ptr_receive_pid) > 1)
		{
			pid = atoi(ptr_receive_pid);
			printf("\npid of client %s", (char *)ptr_receive_pid);
			shm_unlink(KEY_NAME_GET_ID);
			break;
		}
		sleep(5);
	}
	// while (1)
	// 	;
	pthread_t thread1;
	pthread_t thread2;
	pthread_create(&thread1, NULL, threadSend, NULL);
	pthread_create(&thread2, NULL, threadReceive, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
}
