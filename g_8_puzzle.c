//Sistemas Operativos ISEC LEI - 20/21
//Trabalho Prático Realizado por:
//Gonçalo Ramalho 2019106561
//Rafael Correia 2019131435

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

int pontos = 0;

void termina_sinal(int s){
	exit(pontos);
}

int devolveValor(int a, int b){
    return a + rand()%(b-a+1);
}

int main() {
  int mat[3][3];
  int var[9];
  int i,j,a;
  int b,k = 0,c;
  int lin0,col0;
  char comando_recebido[99];
  int cont_inv;
  srand(time(NULL));
  signal(SIGUSR1, termina_sinal);

  printf("\n\n----BEM-VINDO----\n");
  printf("Este jogo consiste de um puzzle chamado 8-puzzle, o objetivo do jogo e ordenar os");
  printf(" numeros do puzzle de 1 a 8 e depois 0 como mostrado a seguir:\n\n"); 
  fflush(stdout);
  printf("\t1 | 2 | 3\n");
  printf("\t---------\n");
  printf("\t4 | 5 | 6\n");
  printf("\t---------\n");
  printf("\t7 | 8 | 0\n");
  printf("\t---------\n");
  fflush(stdout);
  printf("O 0 e a peca principal no movimento.\n");
  fflush(stdout);
  printf("Para mover a peca com o 0 deve usar as teclas 'w' 'a' 's' 'd'");
  fflush(stdout);
  printf("\nAdicionalmente pode usar a tecla p para desistir do puzzle atual.\n");
  fflush(stdout);
  printf("Cada puzzle completo adiciona 1 ponto a pontuacao total.\n");
  printf("\n--Boa Sorte--");
  fflush(stdout);

do{
  printf("\n\n--------NOVO PUZZLE------\n\n");
  fflush(stdout);
do{
   c = 0;
   for (b = 0; b < 9; b++) {
     var[b] = -1;
   }

   while (k == 0 || c < 9) {
     k = 0;
     a = devolveValor(0,8);
     for (b = 0; b < 9; b++) {
       if (a == var[b]) {
         k = 1;
       }
     }
     if(k == 0){
       var[c] = a;
       c++;
     }
   }

     cont_inv = 0;
     for (i = 0; i < 9 - 1; i++)
         for (j = i+1; j < 9; j++)
              if (var[j] && var[i] &&  var[i] > var[j])
                   cont_inv++;
    }while (cont_inv%2 != 0);

    b = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
          mat[i][j] = var[b];
          b++;
          if (mat[i][j] == 0) {
            lin0 = i;
            col0 = j;
          }
    }
}

do {
  printf("\t%d | %d | %d\n", mat[0][0], mat[0][1], mat[0][2]);
  printf("\t---------\n");
  printf("\t%d | %d | %d\n", mat[1][0], mat[1][1], mat[1][2]);
  printf("\t---------\n");
  printf("\t%d | %d | %d\n", mat[2][0], mat[2][1], mat[2][2]);
  printf("\t---------\n");
  fflush(stdout);

  scanf(" %99[^\n]", comando_recebido);

  switch (comando_recebido[0])
  {
     case 'w':
       if(lin0 != 0){
         mat[lin0][col0] = mat[lin0-1][col0];
         lin0 = lin0 - 1;
         mat[lin0][col0] = 0;
       }
     break;

     case 's':
        if (lin0 != 2) {
          mat[lin0][col0] = mat[lin0+1][col0];
          lin0 = lin0 + 1;
          mat[lin0][col0] = 0;
        }
     break;

     case 'a':
        if (col0 != 0) {
          mat[lin0][col0] = mat[lin0][col0-1];
          col0 = col0 - 1;
          mat[lin0][col0] = 0;
        }
     break;

     case 'd':
        if (col0 != 2) {
          mat[lin0][col0] = mat[lin0][col0+1];
          col0 = col0 + 1;
          mat[lin0][col0] = 0;
        }
     break;

     default:
       break;
  }

  if (mat[0][0] == 1 && mat[0][1] == 2 && mat[0][2] == 3 && mat[1][0] == 4 && mat[1][1] == 5 && mat[1][2] == 6 && mat[2][0] == 7 && mat[2][1] == 8 && mat[2][2] == 0) {
    printf("\t%d | %d | %d\n", mat[0][0], mat[0][1], mat[0][2]);
    printf("\t---------\n");
    printf("\t%d | %d | %d\n", mat[1][0], mat[1][1], mat[1][2]);
    printf("\t---------\n");
    printf("\t%d | %d | %d\n", mat[2][0], mat[2][1], mat[2][2]);
    printf("\t---------\n");
    printf("\nGanhou 1 ponto\n");
    fflush(stdout);
    pontos++;
    comando_recebido[0] = 'p';
  }

}while (comando_recebido[0] != 'p');

}while (1);

}
