//Sistemas Operativos ISEC LEI - 20/21
//Trabalho Prático Realizado por:
//Gonçalo Ramalho 2019106561
//Rafael Correia 2019131435

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
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
	int ale;
	char comando_recebido[99];
	signal(SIGUSR1, termina_sinal);

	printf("\n\n----BEM-VINDO----\n");
	printf("Neste jogo voce tera que traduzir varias palavras de varias linguas diferentes,");
	printf(" a resposta deve ser dada em letras mnusculas.\nCada resposta certa dara 1 ponto.\n\n");
	fflush(stdout);

	do
	{
		ale = devolveValor(1, 5);

		if (ale == 1) {
			printf("Traduza a palavra Nazionale\nResposta: ");
			fflush(stdout);
			scanf(" %99[^\n]", comando_recebido);
			printf("\n");
			if (strcmp(comando_recebido, "nacional") == 0){
				printf("Correto\n\n");
				pontos++;
			}
			else
				printf("Errado\n\n");
		}
		else if (ale == 2) {
			printf("Traduza a palavra Ladder\nResposta: ");
			fflush(stdout);
			scanf(" %99[^\n]", comando_recebido);
			printf("\n");
			if (strcmp(comando_recebido, "escada") == 0) {
				printf("Correto\n\n");
				pontos++;
			}
			else
				printf("Errado\n\n");
		}

		 else if (ale == 3) {
			printf("Traduza a palavra Taub\nResposta: ");
			fflush(stdout);
			scanf(" %99[^\n]", comando_recebido);
			printf("\n");
			if (strcmp(comando_recebido, "surdo") == 0) {
				printf("Correto\n\n");
				pontos++;
			}
			else
				printf("Errado\n\n");
		}

		else if (ale == 4) {
			printf("Traduza a palavra Sauver\nResposta: ");
			fflush(stdout);
			scanf(" %99[^\n]", comando_recebido);
			printf("\n");
			if (strcmp(comando_recebido, "salvar") == 0) {
				printf("Correto\n\n");
				pontos++;
			}
			else
				printf("Errado\n\n");
		}

		else if (ale == 5) {
			printf("Traduza a palavra Pegamento\nResposta: ");
			fflush(stdout);
			scanf(" %99[^\n]", comando_recebido);
			printf("\n");
			if (strcmp(comando_recebido, "cola") == 0) {
				printf("Correto\n\n");
				pontos++;
			}
			else
				printf("Errado\n\n");
		}
	} while (1);
}