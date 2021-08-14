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
	int a, resposta = 0;
	signal(SIGUSR1, termina_sinal);

	printf("\n\n----BEM-VINDO----\n");
	printf("Neste jogo voce tera que adivinhar o numero que o computador escolheu,");
	printf(" os numeros vao de 0 a 10.\nCada resposta certa dara 1 ponto.\n\n");
	fflush(stdout);
	do
	{
		a = devolveValor(0, 10);

		printf("Adivinhe o numero: ");
		fflush(stdout);
		scanf(" %99[^\n]", comando_recebido);
		printf("\n");
		resposta = atoi(comando_recebido);
		if (resposta == a) {
			printf("Correto\n\n");
			pontos++;
		}
		else
			printf("Errado, resposta correta %d\n\n", a);
		
	} while (1);
}