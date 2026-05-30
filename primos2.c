#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define QNT 10
#define BLOCKS 1
#define min(a,b) (a<b?a:b)
int verifica_se_primo(long int);

int ppread[QNT][2];
int ppwrite[QNT][2];

long int readFrom(int fd, char *buff) {
	char last=-1;
	int indx = 0;
	char *c = malloc(1);
	do {
		read(fd, c, 1);
		buff[indx++] = *c;
	}while(*c>='0' && *c<='9');
	free(c);

	return atol(buff);
}

void parentFun(long int numero, bool imprimir) {
	long int atual = 1;
	char buff[20];
	bool leitura = false;
	int block = numero/QNT;
	for(int i=0;i<QNT;i++) {
		long int fim = i==QNT-1 ? numero: min(atual + block, numero);
		int siz = sprintf(buff, "%ld %ld ", atual, fim);
		atual = fim + 1;
		write(ppwrite[i][1], buff, siz);
	}
	sprintf(buff, "0 ");
	for(int i = 0;i<QNT;i++) {
		long int value;
		while(true) {
			value = readFrom(ppread[i][0], buff);
			if(value == 0) break;
			if(imprimir) {
				printf("%ld eh primo.\n", value);
			}
		}
		write(ppwrite[i][1], buff, 2);
		close(ppwrite[i][1]);
		close(ppread[i][0]);
	}
}
void childFun(int childNum, int imprimir) {
	char buff[20];
	while(true) {
		long int ini = readFrom(ppwrite[childNum][0], buff);
		if(ini == 0) break;
		long int fim = readFrom(ppwrite[childNum][0], buff);

		bool first = true;
		for (long int num_int = ini; num_int <= fim; num_int++){
			bool eh = verifica_se_primo(num_int);
			if(!eh) continue;
			if (imprimir == 1){
				int siz = sprintf(buff, "%ld ", num_int);
				write(ppread[childNum][1], buff, siz);
			}
		}
		sprintf(buff, "0 ");
		write(ppread[childNum][1], buff, 2);
	}
	close(ppwrite[childNum][0]);
	close(ppread[childNum][1]);
}

int main(int argc, char *argv[])
{
	long int numero = 0;
	short int result, imprimir;
	if (argc != 3)
	{
		printf("Uso correto: %s <numero> <imprimir=1,nao_imprimir=0>\n\n", argv[0]);
		return 1;
	}
	numero = atol(argv[1]);
	imprimir = atoi(argv[2]);
	bool parent = true;
	int whi = 0;

	for(int i=0;i<QNT;i++) {
		pipe(ppread[i]);
		pipe(ppwrite[i]);
		
		if(fork() == 0) {
			parent = false;
			whi = i;
			close(ppwrite[i][1]);
			close(ppread[i][0]);
			break;
		}
		close(ppread[i][1]);
		close(ppwrite[i][0]);
	}

	if(parent) {
		parentFun(numero, imprimir);
	}
	else {
		childFun(whi, imprimir);
	}
	return 0;
}
int verifica_se_primo(long int numero)
{
	long int ant;
	for (ant = 2; ant <= (long int)sqrt(numero); ant++)
	{
		if (numero % ant == 0)
			return 0;
	}
	if (ant * ant >= numero)
		return 1;
}