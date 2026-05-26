#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define N 10
void* foo(void *args) {
	int current = *((int *)args);
	int arg = current + 1;
	if(arg < N) {
		pthread_t childThread;
		int result = pthread_create(&childThread, NULL, foo, (void *) &arg);
		if(result != 0) {
			char buff[35];
			sprintf(buff, "Falha ao criar thread filha n %d", arg);
			perror(buff);
			return NULL;
		}
		pthread_join(childThread, NULL);
	}
	printf("Executou Thread %d\n", current);
	return NULL;
}
int main() {
	pthread_t thread;
	int arg = 1;
	int result = pthread_create(&thread, NULL, foo, (void *) &arg);
	if(result != 0) {
		perror("Thread não foi criada");
		return EXIT_FAILURE;
	}

	result = pthread_join(thread, NULL);
	if(result != 0) {
		perror("Join falhou");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}