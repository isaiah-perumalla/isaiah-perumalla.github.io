#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#define REPS 1000 * 1 
#define CACHE_LINE_SIZE 64
#define PING_OFFSET CACHE_LINE_SIZE * 2
#define PONG_OFFSET PING_OFFSET + CACHE_LINE_SIZE
#define SEMAPHORE_OFFSET PONG_OFFSET + CACHE_LINE_SIZE 
#define PAGE_SIZE 4096

struct args_struct {
	sem_t* sem;
	int* ping_val;
	int* pong_val;
};


void pinger(struct args_struct *args ) {

	getchar();
	int* cnt = malloc(sizeof(int));
	int* ping_val = args->ping_val;
	int* pong_val = args->pong_val;
	sem_t* sem = args->sem;
	*cnt = 0;
	for (int i = 0; i < REPS; ++i)
	{
		sem_wait(sem);
		*ping_val = i;
		sem_post(sem);

		do {
			asm volatile("pause\n": : :"memory"); //pause cpu

		}while(*pong_val != i);
		*cnt = *cnt + 1;
	}
	free(cnt);
	printf("pinger finished , pong_val=%i, ping_val=%i\n", *pong_val, *ping_val );
}

void ponger(struct args_struct *args ) {
	getchar();
	int* cnt = malloc(sizeof(int));
	int* ping_val = args->ping_val;
	int* pong_val = args->pong_val;
	sem_t* sem = args->sem;
	*cnt = 0;
	for (int i = 0; i < REPS; ++i)
	{
		do {
			asm volatile("pause\n": : :"memory"); //pause cpu

		}while(*ping_val != i);

		sem_wait(sem);
		*pong_val = i;
		sem_post(sem);
		*cnt = *cnt + 1;
	}
	free(cnt);
	printf("ponger finished , pong_val=%i, ping_val=%i\n", *pong_val, *ping_val );
}


void* init_shared_mem(int fd) {
	
	char* shared_mem = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	if(shared_mem == MAP_FAILED) {
		fprintf(stderr, "failed to allocated shared mem\n");
		return NULL;
	}
	memset(shared_mem, 0, PAGE_SIZE);
	return shared_mem;
}


int fork_ping_pong(struct args_struct args) {
	pid_t pid = fork();
	if(pid == 0) {

		pinger(&args);

	}
	else if (pid > 0) {
		
		ponger(&args);
	}
	else {
		fprintf(stderr, "error creating child process \n" );
		return -1;
	}
	return 0;
}

int main(int argc, char **argv) {

	int shared_mem_fd = shm_open("/pingpong", O_CREAT| O_RDWR, 0);
	if(shared_mem_fd == -1) {
		fprintf(stderr, "could not open shared_mem file descriptor , errorno=%i \n", errno);
		return -1;
	}
	else {
		ftruncate(shared_mem_fd, PAGE_SIZE);
	}
	
	void* shared_mem = init_shared_mem(shared_mem_fd);
	
	sem_t* semaphore = (sem_t*)(shared_mem + SEMAPHORE_OFFSET);
	
	struct args_struct args;
	args.ping_val = (int*)(shared_mem + PING_OFFSET);
	args.pong_val = (int*)(shared_mem + PONG_OFFSET);
	args.sem = semaphore;
	
	
	
	if(argc == 2 && strcmp("--thread", argv[1]) == 0) {
		if(sem_init(semaphore, 0, 1) == -1 ) {
			fprintf(stderr, "failed to init semaphore, errno=%i\n", errno );
			return -1;
		}
		printf("ping pong with threads \n");
		pthread_t pinger_thread;
		pthread_create( &pinger_thread, NULL, (void*)&pinger, &args);	
		ponger(&args);
		
	}
	else {
		if(sem_init(semaphore, 1, 1) == -1 ) {
			fprintf(stderr, "failed to init semaphore, errno=%i\n", errno);
			return -1;
		}
		printf("ping pong interprocess pid= %i \n", getpid());	
		fork_ping_pong(args);
	}

	
	return 0;
}