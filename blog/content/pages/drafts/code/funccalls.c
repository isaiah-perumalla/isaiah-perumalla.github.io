#include <stdio.h>
#include <stdlib.h>


void count_to(int start, int count) {
	if(start == count) {
		printf("%i calls reached, any key to exit \n", count);
		getchar();

	}
	else{
		count_to(start + 1, count);
	}
}

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("funcalls <count> \n");
		return 0;
	}
	int count = atoi(argv[1]);
	// pthread_t pinger_thread;
	//pthread_create( &pinger_thread, NULL, (void*)&pinger, &args);
	count_to(0, count);
	return 0;

}