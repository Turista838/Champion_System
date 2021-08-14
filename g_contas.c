//Sistemas Operativos ISEC LEI - 20/21
//Trabalho Prático Realizado por:
//Gonçalo Ramalho 2019106561
//Rafael Correia 2019131435

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int pontos = 0;

void termina_sinal(int s) {
	exit(pontos);
}

int devolveValor(int a, int b) {
	return a + rand() % (b - a + 1);
}

int main() {
	srand(time(NULL));
	char comando_recebido[99];
	int operacao, resposta = 0, a, b;
	signal(SIGUSR1, termina_sinal);

	printf("\n\n----BEM-VINDO----\n");
	printf("Neste jogo voce tera que responder a varias expressoes aritmeticas de soma,");
	fflush(stdout);
	printf(" subtracao e multiplicacao com numeros que variam de 1 a 10.\nCada resposta certa dara 1 ponto.\n\n");
	fflush(stdout);
	do {
		operacao = devolveValor(1, 3);
		a = devolveValor(1, 10);
		b = devolveValor(1, 10);

		if (operacao == 1) {
			printf("%d + %d = ", a, b);
			fflush(stdout);
			scanf(" %99[^\n]", comando_recebido);
			resposta = atoi(comando_recebido);
			printf("\n");
			if (resposta == a + b) {
				printf("Correto\n");
				pontos++;
			}
			else {
			if( resposta != -11)
				printf("Errado\n");
			}
		}
		if (operacao == 2) {
			printf("%d - %d = ", a, b);
			fflush(stdout);
			scanf(" %99[^\n]", comando_recebido);
			resposta = atoi(comando_recebido);
			printf("\n");
			if (resposta == a - b) {
				printf("Correto\n");
				pontos++;
			}
			else {
				printf("Errado\n");
			}
		}
		if (operacao == 3) {
			printf("%d x %d = ", a, b);
			fflush(stdout);
			scanf(" %99[^\n]", comando_recebido);
			resposta = atoi(comando_recebido);
			printf("\n");
			if (resposta == a * b) {
				printf("Correto\n");
				pontos++;
			}
			else {
				printf("Errado\n");
			}
		}
	} while (1);
}