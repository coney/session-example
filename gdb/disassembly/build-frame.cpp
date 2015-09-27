#include <unistd.h>
#include <pthread.h>
#include <stdio.h>


#define 	ARRAY_SIZE	10

unsigned int arr[ARRAY_SIZE] = {0};

void *thread_proc(void *arg) {
	size_t index = (size_t)arg;
	while (true) {
		arr[index]++;
		sleep(1);
	}
}

int main(int argc, char **argv) {

	for (size_t i = 0; i < ARRAY_SIZE; i++) {
		pthread_t thread;
		pthread_create(&thread, NULL, thread_proc, (void *)i);
	}

	while (true) {
		for (size_t i = 0; i < ARRAY_SIZE; i++) {
			printf("%u ", arr[i]);
		}
		puts("\n");
		sleep(5);
	}
	return 0;
}
