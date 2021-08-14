//Sistemas Operativos ISEC LEI - 20/21
//Trabalho Prático Realizado por:
//Gonçalo Ramalho 2019106561
//Rafael Correia 2019131435

#ifndef ARBITRO_H
#define ARBITRO_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

#define FIFO_ARB "f_arbitro"
#define FIFO_CLI "f_cli%d"

#define MAXJ 30

typedef struct lista jogador, *pjogador;
struct lista{
	char nome_jogador[50]; //nome do jogador
	char comando[99]; //comando enviado para arbitro/jogo
	char jogo_atr[30]; //jogo atribuido
	int id_jogador; //recebe pid jogador
	int pid_jogo; //recebe pid jogo
	int casa_do_jogo; //responder ao jogo certo
	int pontuacao; //pontuação final
	int suspenso; //1 se sim, 0 se não
	int adicionado; //1 se sim, 0 se não
	pjogador prox;
};

typedef struct games jogos, *pjogos;
struct games{
	char nome_jogo[30]; //nome do jogo
	char comando_j[99];
	pjogos prox;
};

typedef struct critical_variables critical, *pcritical;
struct critical_variables{
	int tempo_espera; //tempo de espera do campeonato
	int duracao_campeonato; //duração do campeonato
	int nr_jogadores; //numero total de jogadores
	int nr_jogo; //numero do jogo a ser atribuído
	char* diretorio; //directorio que contém os jogos
	int** jogo_out; //Ponteiro de ponteiro para a comunicação pipe jogo -> arbitro
	int** jogo_in; //Ponteiro de ponteiro para a comunicação pipe arbitro -> jogo
};

#endif

