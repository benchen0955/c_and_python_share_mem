#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define QUEUE_SIZE 512
#define Key_write 10000
#define Key_read 10001

void *read_queue(void* param_ptr)
{
	int shmid_r;
	char *shared_memory_r;
	
	char from_web_str[QUEUE_SIZE];
	int key;
	
//	queue web_to_stb(10001);
    key=Key_read;
	shmid_r = shmget(key, QUEUE_SIZE, IPC_CREAT | 0666);	
	if(shmid_r < 0)
	{
		printf("Error getting shared memory id [%d]", errno);
	}	
	shared_memory_r = (char *)shmat(shmid_r, NULL, 0);	
	if(shared_memory_r == (char *) -1)
	{
		printf("Error attaching shared memory id");
	}
	
	while(1)
	{
		
//		web_to_stb.read_queue(from_web_str);
		
		if(shared_memory_r != (char *) -1)
		memcpy(from_web_str, shared_memory_r, QUEUE_SIZE);
		
		
		if(strlen(from_web_str) > 0)
		{
			printf("Got string from queue : %s\n", from_web_str);
			
//			web_to_stb.write_queue((char* )"");
            if(shared_memory_r != (char *) -1)
		    memcpy(shared_memory_r, "", strlen("")+1);
			
			if( strcmp(from_web_str, "EXIT") == 0)
			{
				printf("Quit read_queue thread.\n");
				break;
			}
		}
		
		usleep(500);
	}

	//Detach the shared memory
	shmdt(shared_memory_r);

	//Deallocate the shared memory
	shmctl(shmid_r, IPC_RMID, 0);
	
//	web_to_stb.deinit();
	pthread_exit(0);
}




void *write_queue(void* param_ptr)
{
	
	int shmid_w;
	char *shared_memory_w;	
	
	char input_str[QUEUE_SIZE];
	int key;
	
//	queue stb_to_web(10000);	
	
	key=Key_write;
	
	shmid_w = shmget(key, QUEUE_SIZE, IPC_CREAT | 0666);
	
	if(shmid_w < 0)
	{
		printf("Error getting shared memory id [%d]", errno);
	}
	
	shared_memory_w = (char *)shmat(shmid_w, NULL, 0);
	
	if(shared_memory_w == (char *) -1)
	{
		printf("Error attaching shared memory id");
	}
	
	strcpy(input_str, "");
	
//	stb_to_web.write_queue(input_str);	
	if(shared_memory_w != (char *) -1)
		memcpy(shared_memory_w, input_str, strlen(input_str)+1);

	while(1)
	{
		//Get string from stdin
		printf("Input string to send to python (EXIT to quit): ");
		fgets(input_str, QUEUE_SIZE, stdin);

		//replace \n to \0 at end of string
		input_str[strlen(input_str)-1] = '\0';

		printf("%s(%d)\n", input_str, strlen(input_str));
		
//		stb_to_web.write_queue(input_str);
		if(shared_memory_w != (char *) -1)
		memcpy(shared_memory_w, input_str, strlen(input_str)+1);

		if( strcmp(input_str, "EXIT") == 0)
		{
			printf("Quit write_queue thread.\n");
			break;
		}
	}
	//Detach the shared memory.
	shmdt(shared_memory_w);

	//Deallocate the shared memory
	shmctl(shmid_w, IPC_RMID, 0);

//	stb_to_web.deinit();
	pthread_exit(0);
}

int main(int argc, const char **argv)
{
	pthread_t read_queue_thread;
	pthread_t write_queue_thread;
	
	printf("stb_test for c\n");
	
	pthread_create(&read_queue_thread, NULL , read_queue , NULL);
	pthread_create(&write_queue_thread, NULL , write_queue , NULL);
	
	pthread_join(read_queue_thread, NULL);
	pthread_join(write_queue_thread, NULL);
	
	
	
	while(1);
}
