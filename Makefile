arbitro: arbitro.c arbitro.h
	gcc arbitro.c -c

cliente: cliente.c arbitro.h
	gcc cliente.c -c

g_8_puzzle: g_8_puzzle.c
	gcc g_8_puzzle.c -c

g_adivinha_num: g_adivinha_num.c
	gcc g_adivinha_num.c -c

g_traduzir: g_traduzir.c
	gcc g_traduzir.c -c

g_contas: g_contas.c
	gcc g_contas.c -c

all: arbitro.o cliente.o g_8_puzzle.o g_adivinha_num.o g_traduzir.o g_contas.o arbitro.h
	gcc arbitro.c -o arbitro -lpthread
	gcc cliente.c -o cliente
	gcc g_8_puzzle.c -o g_8_puzzle
	gcc g_adivinha_num.c -o g_adivinha_num
	gcc g_traduzir.c -o g_traduzir
	gcc g_contas.c -o g_contas
clean:
	rm *.o
	rm arbitro
	rm cliente
	rm g_8_puzzle
	rm g_adivinha_num
	rm g_traduzir
	rm g_contas
	
