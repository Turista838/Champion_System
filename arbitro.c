//Sistemas Operativos ISEC LEI - 20/21
//Trabalho Prático Realizado por:
//Gonçalo Ramalho 2019106561
//Rafael Correia 2019131435

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arbitro.h"

int comeca_camp = 0;
int maxpl;

pjogador j = NULL;
pjogos g = NULL; 

pthread_mutex_t trinco;
pthread_t champ;

//Jogos

pjogos adiciona_lista_jogos(char* nome){
    pjogos jogo_novo;
    if ((jogo_novo = malloc(sizeof(pjogos))) == NULL ){
		fprintf(stderr, "Erro na alocacao de memoria.\n");
		exit(14);
    }
    else{
        strcpy(jogo_novo->nome_jogo,nome);
        jogo_novo->prox = g;
        g = jogo_novo;
    }
}

void mostra_total_jogos(){
    pjogos aux = g;
    while(aux != NULL){
        printf("\nNome do jogo: %s", aux->nome_jogo);
		aux = aux->prox;
    }
}

char* randomiza_jogo(){
	pjogos aux = g;
	int random, a = 0, b = 10;
	do{
		while(aux != NULL){
			random = a + rand()%(b-a+1);
			if (random == 5)
    				return aux->nome_jogo;
			aux = aux->prox;
		}
		aux = g;
	}while(1);
}

void liberta_mem_g(){
    pjogos aux;
    while(g != NULL){
        aux = g;
        g = g->prox;
        free(aux);
    }
}

//Jogadores

void liberta_mem_j(){
    pjogador aux;
    while(j != NULL){
        aux = j;
        j = j->prox;
        free(aux);
    }
}

int envia_msg(jogador jgdr){
	int fd_resposta;
	char fifo[40];
	sprintf(fifo, FIFO_CLI, jgdr.id_jogador);
	fd_resposta = open(fifo, O_WRONLY);
	if (fd_resposta == -1){
		return 1;
	}
	write(fd_resposta, &jgdr, sizeof(jogador));
	close(fd_resposta);
	return 0;
}

void adiciona_lista_jogadores(jogador jgdr){
    pjogador jogador_novo, aux = j;
    if ((jogador_novo = malloc(sizeof(jogador))) == NULL ){
        printf("Erro na alocacao de memoria.\n");
		liberta_mem_g();
		liberta_mem_j();
		exit(15);
    }
    else{ //inserção no fim da lista
	strcpy(jogador_novo->nome_jogador,jgdr.nome_jogador);
     	jogador_novo->id_jogador = jgdr.id_jogador;
	strcpy(jogador_novo->jogo_atr,randomiza_jogo());
        jogador_novo->pontuacao = 0;
	jogador_novo->suspenso = 0;
	jogador_novo->casa_do_jogo = jgdr.casa_do_jogo;
	jogador_novo->adicionado = 1;
        jogador_novo->prox = NULL;
	if(j == NULL)
		j = jogador_novo;
	else{
		while(aux->prox != NULL)
			aux = aux->prox;
        	aux->prox = jogador_novo;
	    }
    }
}


void mostra_total_nomes(){
    pjogador aux = j;
    while(aux != NULL){
        printf("\nNome do cliente: %s", aux->nome_jogador);
        printf("\nTem o pid: %d", aux->id_jogador);
		printf("\nJogo atribuido: %s", aux->jogo_atr);
		printf("\nCasa do jogo: %d\n", aux->casa_do_jogo);
        aux = aux->prox;
    }
}

jogador retorna_struct(pjogador aux){
	jogador jgdr;
	strcpy(jgdr.nome_jogador, aux->nome_jogador);
	jgdr.id_jogador = aux->id_jogador;
	jgdr.pid_jogo = aux->pid_jogo;
	jgdr.suspenso = aux->suspenso;
	jgdr.casa_do_jogo = aux->casa_do_jogo;
	strcpy(jgdr.jogo_atr, aux->jogo_atr);
	jgdr.pontuacao = aux->pontuacao;
	jgdr.casa_do_jogo = aux->casa_do_jogo;
	jgdr.adicionado = aux->adicionado;
	return jgdr;
}

jogador retorna_jogador_por_id(int i){
	pjogador aux = j;
	jogador jgdr;
	while(aux != NULL){
        	if(aux->casa_do_jogo == i){
			jgdr = retorna_struct(aux);
			return jgdr;
		}
		aux = aux->prox;
    }
}

jogador suspende_jogador(char* nome){
	pjogador aux = j;
	jogador jgdr;
	while(aux != NULL){
        	if(strcmp(aux->nome_jogador,nome) == 0){
			jgdr = retorna_struct(aux);
			jgdr.suspenso = 1;
			return jgdr;
		}
		aux = aux->prox;
    }
	printf("Jogador não encontrado!\n");
	jgdr.adicionado = 0;
	return jgdr;
}

jogador retoma_jogador(char* nome){
	pjogador aux = j;
	jogador jgdr;
	while(aux != NULL){
        	if(strcmp(aux->nome_jogador,nome) == 0){
			jgdr = retorna_struct(aux);
			jgdr.suspenso = 0;
			return jgdr;
		}
		aux = aux->prox;
    	}
	printf("Jogador não encontrado!\n");
	jgdr.adicionado = 0;
	return jgdr;
}

int verifica_nome(jogador jgdr){
	pjogador aux = j;
	while(aux != NULL){
		if ( strcmp(aux->nome_jogador, jgdr.nome_jogador) == 0){
			printf("Apanhei um nome igual!\n");
			return 0;
		}
		aux = aux->prox;
    	}
	return 1;
}

int apaga_nome(char* cmd){ //apaga e envia sinal kick
	pjogador atual, anterior = NULL;
	int pontos;
	atual = j;
	while(atual != NULL && (strcmp(atual->nome_jogador,cmd)!=0)){
		anterior = atual;
		atual = atual->prox;
	}
	if(atual == NULL){
		printf("Jogador não existente");
		return 0;
	}
	if(anterior == NULL)//primeiro no da lista
		j = atual->prox;
	else
		anterior->prox = atual->prox;
	kill(atual->id_jogador, SIGUSR2);
	if(comeca_camp == 1)
		kill(atual->pid_jogo, SIGUSR1);
	wait (&pontos);
	free(atual);
	return 1;
}

void corrige_casa_do_jogo(){
	pjogador aux = j;
	int i = 0;
	while(aux != NULL){
		aux->casa_do_jogo = i;
		i++;
		aux = aux->prox;
	}
}

void liberta_tudo(int **pipe1, int **pipe2){
	int i;
	unlink(FIFO_ARB);
	liberta_mem_j();
	liberta_mem_g();
	for(i = 0; i < maxpl; i++){
		free(pipe1[i]);
	}
	free(pipe1);
	for(i = 0; i < maxpl; i++){
		free(pipe2[i]);
	}
	free(pipe2);
}

void* champion(void* dados){//thread iniciadora do campeonato
	critical* c;
	c = (critical *) dados;

	sleep(c->tempo_espera);

	struct timespec timeout;
	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += (c->duracao_campeonato)*60;
	int fork_control, pontos, best_pts = 0;
	char best_player[50], d[200];
	jogador jg;
	pjogador aux_jogador = j;

	for(c->nr_jogo = 0; c->nr_jogo < c->nr_jogadores; c->nr_jogo++){
		sprintf(d, "%s/%s", c->diretorio, aux_jogador->jogo_atr);
		pipe(c->jogo_out[c->nr_jogo]); //jogo -> arbitro
		pipe(c->jogo_in[c->nr_jogo]); //arbitro -> jogo
		fork_control = fork();
		if(fork_control == -1){
			liberta_tudo(c->jogo_in, c->jogo_out);
			fprintf(stderr, "[Thread] ERRO! Criacao do processo filho sem sucesso\n");
			exit(16);
		}
		aux_jogador->pid_jogo = fork_control;
		aux_jogador=aux_jogador->prox;
		if (fork_control > 1){
			comeca_camp = 1;
		}	
		if (fork_control == 0){ //filho
			close(1);
			dup(c->jogo_out[c->nr_jogo][1]);
			close(c->jogo_out[c->nr_jogo][1]);
			close(c->jogo_out[c->nr_jogo][0]);					
			close(0);
			dup(c->jogo_in[c->nr_jogo][0]);
			close(c->jogo_in[c->nr_jogo][0]);			
			close(c->jogo_in[c->nr_jogo][1]);
			execl(d, d, NULL);					
		}
	}
	
	int fd_proprio_pipe = open (FIFO_ARB, O_RDWR);
	strcpy(jg.comando,"#begin");
	write(fd_proprio_pipe, &jg, sizeof(jogador));
	close(fd_proprio_pipe);

	pthread_mutex_lock(&trinco);

	pthread_mutex_timedlock(&trinco, &timeout);

	aux_jogador = j;
	while(aux_jogador != NULL){
		kill(aux_jogador->pid_jogo, SIGUSR1);
		wait (&pontos);
		if(WIFEXITED(pontos)){
			aux_jogador->pontuacao = WEXITSTATUS(pontos);
			if(aux_jogador->pontuacao > best_pts){
				best_pts = aux_jogador->pontuacao;
				strcpy(best_player,aux_jogador->nome_jogador);
			}
		}
		aux_jogador = aux_jogador->prox;
    }
	aux_jogador = j;
	sprintf(jg.comando, "O jogador com mais pontos foi: %s com %d pontos!\n", best_player, best_pts);
	while(aux_jogador != NULL){
		kill(aux_jogador->id_jogador, SIGUSR1);
		jg.adicionado = 0;
		jg.suspenso = 0;
		jg.pontuacao = aux_jogador->pontuacao;
		jg.id_jogador = aux_jogador->id_jogador;
		if(envia_msg(jg)){
			liberta_tudo(c->jogo_in, c->jogo_out);
			exit(8);
		}
		aux_jogador = aux_jogador->prox;
	}
	liberta_mem_j(j);
	j = NULL;
	c->nr_jogadores = 0; //reset ao nº de jogadores
	comeca_camp = 0; //reset ao campeonato
	pthread_mutex_unlock(&trinco);
	pthread_exit(NULL);
}

int retira_jogador(int* nr_jgdr){
	int num = 0;

	if((*nr_jgdr) == 2 && comeca_camp == 0){
		num = pthread_cancel(champ);
		if (num != 0) {
			fprintf(stderr, "ERRO! Término da thread champion sem sucesso\n");
			return 1;
		}
	}
	(*nr_jgdr)--;

	if(comeca_camp != 1) {
		corrige_casa_do_jogo();
	}
	else{
		if((*nr_jgdr) == 1){
			num = pthread_mutex_unlock(&trinco);
			if (num != 0) {
				fprintf(stderr, "ERRO! Desbloqueio do mutex trinco sem sucesso\n");
				return 1;
			}
		}
	}
	return 0;
}

int main (int argc, char *argv[]){
	char *dr_camp = NULL, *tmp_espera = NULL, *j_maximo;
	char cmd[99], buf [99];
	critical c;
	c.tempo_espera = 60;
	c.duracao_campeonato = 10;
	c.nr_jogadores = 0;

	int cont = 0, maxfd = 0;
	int opt, fd_pedido, num, res, i, temp;
	struct dirent *d;
	DIR* dir;
	jogador jgdr;
	fd_set sel;

	while((opt = getopt (argc, argv, "d:t:")) != -1)
		switch (opt)
			{
			case'd':
				dr_camp = optarg;
				if(dr_camp != NULL)
					c.duracao_campeonato = atoi(dr_camp);
				if(c.duracao_campeonato <=0)
				{
					fprintf(stderr, "ERRO! Duracao do campeonato não válido.\n");
					exit(2);
				}
				break;
			case't':
				tmp_espera = optarg;
				if(tmp_espera != NULL)
					c.tempo_espera = atoi(tmp_espera);
				if(c.tempo_espera <=0)
				{
					fprintf(stderr, "ERRO! Tempo de espera não válido.\n");
					exit(3);
				}
				break;
			default:
				fprintf(stderr, "Erro ao iniciar arbitro!\n");
				exit(1);
			}

	if(dr_camp == NULL)
		printf("Tempo de duracao do campeonato nao inserido. Assumir duracao default (10 minutos).\n");
	printf("Duracao campeonato: %d minutos.\n", c.duracao_campeonato);

	if(tmp_espera == NULL)
		printf("Tempo de espera de jogadores nao inserido. Assumir tempo de espera default (60 segundos).\n");
	printf("Tempo espera: %d segundos.\n", c.tempo_espera);

	c.diretorio = getenv("GAMEDIR");
	if(c.diretorio == NULL)
		c.diretorio = getenv("PWD");

	dir = opendir(c.diretorio); //Verifica se directorio existe
	if (dir) {
    		printf("Diretorio para ir buscar os jogos: %s\n", c.diretorio);
	} else if (ENOENT == errno) {
    		c.diretorio = getenv("PWD");
		dir = opendir(c.diretorio);
		printf("Diretorio GAMEDIR nao existe! Utilizar directorio base: %s\n", c.diretorio);
	}

	while( (d = readdir(dir)) != NULL){
		if((d->d_name[0] == 'g') && (d->d_name[1] == '_')){ //se for 'g_'
			if(access(d->d_name, X_OK) != -1){ //se for executável
				adiciona_lista_jogos(d->d_name);
			}
		}
	}
	closedir(dir);

	if(g == NULL){
		fprintf(stderr, "ERRO! Sem jogos instalados na pasta %s\n", c.diretorio);
		exit(9);
	}

	j_maximo = getenv("MAXPLAYER");
	if(j_maximo != NULL)
		maxpl = atoi(j_maximo);
	else
	{
		fprintf(stderr, "ERRO! Variavel MAXPLAYER nao encontrada\n");
		exit(4);
	}

	if(maxpl > MAXJ || maxpl < 2)
	{
		fprintf(stderr, "ERRO! Numero de jogadores definidos invalido\n");
		exit(5);
	}
	else
		printf("Numero de jogadores: %d \n", maxpl);

	if (access(FIFO_ARB, F_OK) != 0 ){
	mkfifo(FIFO_ARB, 0600);
	}
	else{
		fprintf(stderr, "ERRO! Ja existe um pipe arbitro criado\n");
		exit(6);
	}

	printf("Criei o fifo ARB\n");
	fd_pedido = open (FIFO_ARB, O_RDWR);
	if (fd_pedido == -1){
		fprintf(stderr, "ERRO! Abertura do pipe arbitro sem sucesso\n");
		exit(7);
	}

	c.jogo_in = (int**)malloc(maxpl * sizeof(int*));
	for(i = 0; i < maxpl; i++){
		c.jogo_in[i] = (int*)malloc(2 * sizeof(int));
	}
	c.jogo_out = (int**)malloc(maxpl * sizeof(int*));
	for(i = 0; i < maxpl; i++){
		c.jogo_out[i] = (int*)malloc(2 * sizeof(int));
	}	

	do{

		printf("\n[ARBITRO]Introduza um comando: ");		
		fflush(stdout);
		FD_ZERO(&sel);
		FD_SET(0, &sel);
		FD_SET(fd_pedido, &sel);
		maxfd = fd_pedido;
		if(comeca_camp == 1){
			for(i=0; i<c.nr_jogo; i++){
				FD_SET(c.jogo_out[i][0], &sel);
				if(c.jogo_out[i][0] > maxfd)
					maxfd = c.jogo_out[i][0];
			}
		}

		res = select(maxfd+1, &sel, NULL, NULL, NULL);
					
		if (res > 0 && FD_ISSET(0, &sel)){	
			scanf(" %99[^\n]", cmd);
			if(strcmp(cmd,"exit")==0){
				close(fd_pedido);
				liberta_tudo(c.jogo_in, c.jogo_out);
				exit(0);
			}
			if(strcmp(cmd,"players")==0){
				printf("\n[LISTA DOS JOGADORES]");
				mostra_total_nomes();
			}
			if (strcmp(cmd, "games") == 0) {
				printf("\n[LISTA DOS JOGOS]");
				mostra_total_jogos(g);
			}
			if(strcmp(cmd,"end")==0){ //terminar campeonato, jogadores recebem pontuação mas o arbitro fica online
				if(comeca_camp != 1){
					printf("Campeonato não iniciado!\n");
				}
				else{
					num = pthread_mutex_unlock(&trinco);
					if (num != 0){
						close(fd_pedido);
						liberta_tudo(c.jogo_in, c.jogo_out);
						fprintf(stderr, "ERRO! Desbloqueio do mutex trinco sem sucesso\n");
						exit(12);
					}
				}
			}
			if(cmd[0] == 's'){
				for(i=0; cmd[i]!= '\0'; i++)
					cmd[i] = cmd[i+1];
				cmd[i]='\0';
				if(comeca_camp != 1){
					printf("Campeonato não iniciado!\n");
				}
				else{
					jgdr = suspende_jogador(cmd);
					if(jgdr.adicionado != 0){
						strcpy(jgdr.comando,"Jogador suspenso! Aguarde pela retoma das comunicações.\n");
						if (envia_msg(jgdr)) {
							close(fd_pedido);
							liberta_tudo(c.jogo_in, c.jogo_out);
							exit(8);
						}
					}		
				}			
			}
			if(cmd[0] == 'r'){
				for(i=0; cmd[i]!= '\0'; i++)
					cmd[i] = cmd[i+1];
				cmd[i]='\0';
				if(comeca_camp != 1){
					printf("Campeonato não iniciado!\n");
				}
				else{
					jgdr = retoma_jogador(cmd);
					if(jgdr.adicionado != 0){
						strcpy(jgdr.comando,"Comunicações retomadas!\n");
						if (envia_msg(jgdr)) {
							close(fd_pedido);
							liberta_tudo(c.jogo_in, c.jogo_out);
							exit(8);
						}
					}
				}
			}
			if(cmd[0] == 'k'){
				for(i=0; cmd[i]!= '\0'; i++)
					cmd[i] = cmd[i+1];
				cmd[i]='\0';
				if(apaga_nome(cmd)){
					if(retira_jogador(&c.nr_jogadores)){
						close(fd_pedido);
						liberta_tudo(c.jogo_in, c.jogo_out);
						exit(13);
					}		
				}
			}
		}
		else if (res > 0 && FD_ISSET(fd_pedido, &sel)){
			num = read(fd_pedido, &jgdr, sizeof(jogador));
			
        	if(num == sizeof(jogador)){
				if(jgdr.comando[0] == '#'){
					if(strcmp(jgdr.comando,"#begin")==0)
						printf("Campeonato iniciado!\n");
					if(strcmp(jgdr.comando,"#quit")==0){
						if(apaga_nome(jgdr.nome_jogador)){
							if(retira_jogador(&c.nr_jogadores)){
								close(fd_pedido);
								liberta_tudo(c.jogo_in, c.jogo_out);
								exit(13);
							}					
						}
					}
					if(strcmp(jgdr.comando,"#mygame")==0){
						if(jgdr.adicionado == 1){
							jgdr = retorna_jogador_por_id(jgdr.casa_do_jogo);
							strcpy(jgdr.comando,jgdr.jogo_atr);
							if (envia_msg(jgdr)) {
								close(fd_pedido);
								liberta_tudo(c.jogo_in, c.jogo_out);
								exit(8);
							}
						}						
					}
				}
				else{
					if(comeca_camp != 1) {
						if(jgdr.adicionado == 0){
							strcpy(jgdr.nome_jogador,jgdr.comando);
							if(verifica_nome(jgdr)){
								jgdr.adicionado = 1;
								jgdr.suspenso = 0;
								jgdr.casa_do_jogo = c.nr_jogadores;
								adiciona_lista_jogadores(jgdr);
								c.nr_jogadores++;
								if(c.nr_jogadores == 2){
									if(c.tempo_espera == 0)
										c.tempo_espera = temp;
									num = pthread_create(&champ, NULL, champion, (void*)&c);
									if (num != 0){
										close(fd_pedido);
										liberta_tudo(c.jogo_in, c.jogo_out);
										fprintf(stderr, "ERRO! Criacao da thread champion sem sucesso\n");
										exit(10);
									}
								}
								if(c.nr_jogadores == maxpl){ //Começa logo o campeonato assim que atingir o nº máximo de jogadores								
									num = pthread_cancel(champ);
									if (num != 0){
										close(fd_pedido);
										liberta_tudo(c.jogo_in, c.jogo_out);
										fprintf(stderr, "ERRO! Término da thread champion sem sucesso\n");
										exit(11);
									}
									temp = c.tempo_espera;
									c.tempo_espera = 0;
									num = pthread_create(&champ, NULL, champion, (void*)&c);
									if (num != 0){
										close(fd_pedido);
										liberta_tudo(c.jogo_in, c.jogo_out);
										fprintf(stderr, "ERRO! Criacao da thread champion sem sucesso\n");
										exit(10);
									}
									
								}
								strcpy(jgdr.comando,"Jogador Adicionado!");
							}
							else{
								strcpy(jgdr.comando,"Jogador NAO Adicionado! Introduza novo nome.");
							}
						}
						else{
							strcpy(jgdr.comando,"Espere pelo comeco do campeonato!");
						}
						if(envia_msg(jgdr)){
							close(fd_pedido);
							liberta_tudo(c.jogo_in, c.jogo_out);
							exit(8);
						}
					}
					if(comeca_camp == 1){
						if(jgdr.adicionado == 0){ //segurança para o caso de um cliente querer entrar a meio do campeonato
							strcpy(jgdr.comando,"Campeonato em curso! Aguarde que ele termine");
							if(envia_msg(jgdr)){
								close(fd_pedido);
								liberta_tudo(c.jogo_in, c.jogo_out);
								exit(8);
							}
						}
						else{
							if(jgdr.suspenso == 1){
								strcpy(jgdr.comando,"Comunicações suspensas!");
								if(envia_msg(jgdr)){
									close(fd_pedido);
									liberta_tudo(c.jogo_in, c.jogo_out);
									exit(8);
								}
							}
							else{
								strcat(jgdr.comando,"\n");
								num = write(c.jogo_in[jgdr.casa_do_jogo][1], &jgdr.comando, strlen(jgdr.comando));								
							}						
						}				
					}
				}				
			}
		}

		else if (comeca_camp == 1){
			for(i=0; i<c.nr_jogo; i++){
				if (res > 0 && FD_ISSET(c.jogo_out[i][0], &sel)){		
					num = read(c.jogo_out[i][0], &buf,99);
					buf[num]='\0';
					jgdr = retorna_jogador_por_id(i);
					strcpy(jgdr.comando,buf);
					if(envia_msg(jgdr)){
						close(fd_pedido);
						liberta_tudo(c.jogo_in, c.jogo_out);
						exit(8);
					}
				}
			}
		}
	}while(1);
}





























