#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void* foo(void *args) {
	int *vals = (int*) args;
	int s = vals[0];
	int qnt = vals[1];
	char c = vals[2];
	sleep(s);
	for(int i=0;i<qnt;i++) putc(c, stdout);
	return NULL;
}
int main() {
	pthread_t thread[3];
	int args[3][3];
	for(int i=0;i<3;i++) {
		args[i][0] = i;
		args[i][1] = 5;
		args[i][2] = 'A'+i;
		int result = pthread_create(&thread[i], NULL, foo, (void *) &args[i]);
		if(result != 0) {
			perror("Thread não foi criada");
			return EXIT_FAILURE;
		}
	}
	for(int i=0;i<3;i++) {
		int result = pthread_join(thread[i], NULL);
		if(result != 0) {
			perror("Join falhou");
			return EXIT_FAILURE;
		}
	}
	printf("\n");

	return EXIT_SUCCESS;
}