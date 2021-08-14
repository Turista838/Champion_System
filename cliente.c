//Sistemas Operativos ISEC LEI - 20/21
//Trabalho Prático Realizado por:
//Gonçalo Ramalho 2019106561
//Rafael Correia 2019131435

#include <stdio.h>
#include <stdlib.h>
#include "arbitro.h"

int fd_resposta, flag;
jogador jgdr;
char fifo[40];

void champion_concluido(int s){
	flag = -1;
	printf("Acabou o Campeonato!\n");
}

void kickado(int s){
	printf("Jogador desistiu!!!\n");
	close(fd_resposta);
	unlink(fifo);
	exit(0);
}

int main (int argc, char *argv[]){
	int fd_pedido, num, res;
	jogador jgdr;
	fd_set sel;

	signal(SIGUSR1, champion_concluido);
	signal(SIGUSR2, kickado);
	
	jgdr.id_jogador = getpid();
	jgdr.adicionado = 0;	

	sprintf(fifo, FIFO_CLI, jgdr.id_jogador);
	mkfifo(fifo, 0600);

	fd_resposta = open(fifo, O_RDWR);
	if (fd_resposta == -1){
		fprintf(stderr, "ERRO! Abertura do pipe cliente sem sucesso\n");
		exit(1);
	}	
	
	printf("[CLIENTE]Introduza o seu nome: ");
	fflush(stdout);

	do {
		fd_pedido = open(FIFO_ARB, O_WRONLY);
		if (fd_pedido == -1)
			fprintf(stderr, "ERRO arbitro nao existe\n");
		FD_ZERO(&sel);
		FD_SET(0, &sel); //tomar atenção ao teclado
		FD_SET(fd_resposta, &sel); //tomar atenção ao named pipe fdP
		res = select(fd_resposta+1, &sel, NULL, NULL, NULL);
		if (res > 0 && FD_ISSET(0, &sel)){
			scanf(" %99[^\n]", jgdr.comando);
			fflush(stdout);//no arbitro está igual, testar isto
			if((strcmp(jgdr.comando,"#quit")==0) && jgdr.adicionado == 0){
				close(fd_resposta);
				unlink(fifo);
				exit(5);
			}
			if((strcmp(jgdr.comando,"info")==0)){
				printf("Sou o jogador com o PID: %d\n", jgdr.id_jogador);
				printf("Sou tenho a casa do jogo: %d\n", jgdr.casa_do_jogo);
			}
			if (access(FIFO_ARB, F_OK) == 0 )
				num = write(fd_pedido, &jgdr, sizeof(jogador));
			else{
				jgdr.adicionado = 0;
			}
		}
		else if (res > 0 && FD_ISSET(fd_resposta, &sel)){
			if(jgdr.adicionado == 0){
				num = read(fd_resposta, &jgdr, sizeof(jogador));
				if(num == sizeof(jogador))
					printf("\n'%s'\n", jgdr.comando);
				if(jgdr.adicionado != 0)
					printf("[CLIENTE]Introduza o comando: ");
				else
					printf("[CLIENTE]Introduza o seu nome: ");
			}
			else{
				num = read(fd_resposta, &jgdr, sizeof(jogador));
				if(num == sizeof(jogador)){
					if(flag == -1){
						printf("Pontuacao: %d\n", jgdr.pontuacao);
						printf("%s", jgdr.comando);
						printf("O Campeonato terminou. Para continuar introduza novo nome ou '#quit' para sair\n");
						flag = 0;
					}
					else{
						printf("%s\n", jgdr.comando);
					}
				fflush(stdout);
				}
			}
			fflush(stdout);
		}
		close(fd_pedido);
	}while(1);
}
