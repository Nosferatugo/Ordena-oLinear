#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <limits.h>

#define VETOR_TAM 100000
#define ARQVETOR_TAM 12

#define ARQ_DIR "inst"

typedef int vetor[VETOR_TAM];
typedef char arqvetor[ARQVETOR_TAM][256];


void counting_sort(vetor in, vetor out, int max, int tam);
void radix_sort(int** arranjos, int tam, int d);

int busca_max(vetor vet, int tam);
void copia_vetor( vetor v1, vetor v2, int tam );

void vetor_para_arranjos( int*** arranjos, vetor vet, int tam, int* d );
void arranjos_para_vetor( int** arranjos, vetor vet, int tam, int d );

long long get_ms();

int le_amostras_vet(char *arq, vetor vet, int *tam);
void imprime_amostras_vet(vetor vet, int tam);

void carrega_arqnome(char *arqnome, int i, int vettam);
void carrega_arqnome_vet(arqvetor arqvet, int *cont);
void lista_arqnome_vet(arqvetor arqvet, int tam);
void carrega_arqpath(char *path, char *arqnome, char *basedir);

void menu();
void menu_ordenamento();

int main() {
	vetor vet;
	vetor auxvet;
	int** arranjos;
	
	int tam, d;
	int min, max, i;
	long long ms1, ms2;

	arqvetor arqvet;
	int arqvet_tam;
	int leu;

	char op = '1';
	char op2 = '0';
	int arqnum = 0;

	char path[256];
			
	carrega_arqnome_vet(arqvet, &arqvet_tam);
	do {
		menu();
		printf("\nInforme a opcao desejada: ");

		fflush(stdin);
		fflush(stdout);
		op = getchar();

		switch (op) {			
			case '1':				
				lista_arqnome_vet(arqvet, arqvet_tam);
				printf("\nInforme um numero do arquivo para carregar a amostra: ");
				scanf("%d", &arqnum);

				if (arqnum < 1 || arqnum > arqvet_tam) {
					printf("\nEste numero nao consta na lista de amostras.");
				} else {
					carrega_arqpath(path, arqvet[arqnum - 1], ARQ_DIR);
					leu = le_amostras_vet(path, vet, &tam);
					if (leu) {
						printf("\nAs amostras foram carregadas com sucesso!");
					} else {
						printf("\nHouve uma falha na leitura do arquivo: %s", path);
					}
				}
				break;
				
			case '2':
				imprime_amostras_vet(vet, tam);
				break;
												
			case '3':
				menu_ordenamento();
				printf("\nInforme uma opcao desejada: ");

				fflush(stdin);
				fflush(stdout);
				op2 = getchar();

				switch (op2) {
					case '1':
						max = busca_max( vet, tam );
						ms1 = get_ms();
						counting_sort( vet, auxvet, max, tam );
						ms2 = get_ms();						
						copia_vetor( vet, auxvet, tam );
						printf("\nO VETOR FOI ORDENADO COM SUCESSO EM: %lldms", (ms2 - ms1));
						break;	
											
					case '2':
						vetor_para_arranjos( &arranjos, vet, tam, &d );						
						ms1 = get_ms();
						radix_sort( arranjos, tam, d );
						ms2 = get_ms();						
						arranjos_para_vetor( arranjos, vet, tam, d );
						printf("\nO VETOR FOI ORDENADO COM SUCESSO EM: %lldms", (ms2 - ms1));
						break;
						
					case '0':
						break;
					default:
						printf("\nEsta opcao e invalida!");
				}

				if (op2 != '0')	{
					printf("\nPrecione a tecla Enter para continuar...");
					fflush(stdin);
					fflush(stdout);
					getc(stdin);
				}
				break;
								
			case '0':
				printf("\nPrecione a tecla Enter para fechar a aplicação...\n");
				break;
			default:
				printf("\nA opcao e invalida!");
		}

		if (op != '0' && op != '4') {
			if (op != '0')
				printf("\nPrecione a tecla Enter para continuar...");
			fflush(stdin);
			fflush(stdout);
			getc(stdin);
		}

	} while (op != '0');

	return 0;
}

void menu() {
	printf("\n|*************** MENU DE ACOES PRINCIPAIS *****************|");
	printf("\n                                                            ");
	printf("\n  (1) Selecionar um arquivo para leitura");
	printf("\n  (2) Listar o conjunto de amostras");
	printf("\n  (3) Ordenar as amostras");
	printf("\n  (0) Sair");
	
	printf("\n                                                             ");
	printf("\n|*********************************************************|\n");
}

void menu_ordenamento() {
	printf("\n|********* APLICAR ORDENACAO *********|");
	printf("\n                                       ");
	printf("\n  (1) Counting Sort                    ");
	printf("\n  (2) Radix Sort                       ");
	printf("\n  (0) Voltar                           ");
	printf("\n                                       ");
	printf("\n|***********************************|\n");
}

void carrega_arqpath(char *path, char *arqnome, char *basedir) {
	strcpy(path, basedir);
	strcat(path, "/");
	strcat(path, arqnome);
}

void lista_arqnome_vet(arqvetor arqvet, int tam) {
	int i;

	printf("\n| *** Lista de arquivos disponiveis *** |\n\n");
	for (i = 0; i < tam; i++)
		printf(" (%d) %s\n", (i + 1), arqvet[i]);
	printf("\n");
}

void carrega_arqnome_vet(arqvetor arqvet, int *cont) {
	int i, j;

	*cont = 0;
	for (i = 1; i <= 4; i++)
		for (j = 1000; j <= 100000; j *= 10)
			carrega_arqnome(arqvet[(*cont)++], i, j);
}

void carrega_arqnome(char *arqnome, int i, int vettam) {
	sprintf(arqnome, "%s%d%s%d%s", "num.", vettam, ".", i, ".in");
}

int le_amostras_vet(char *arqnome, vetor vet, int *tam) {
	int i;
	FILE *arq;
	char linha[10];

	arq = fopen(arqnome, "r");
	if (arq == NULL)
		return 0;

	fgets(linha, sizeof(linha), arq);
	*tam = atoi(linha);

	for (i = 0; i < *tam; i++) {
		fgets(linha, sizeof(linha), arq);
		vet[i] = atoi(linha);
	}

	fclose(arq);

	return 1;
}

void imprime_amostras_vet(vetor vet, int tam) {
	int i;

	printf("\nVetor=\n");
	for (i = 0; i < tam; i++)
	{
		printf("%11d", vet[i]);
		if ((i + 1) % 7 == 0)
			printf("\n");
	}
	printf("\n");
}


int particiona_tradicional(vetor vet, int p, int r ) {
	int pivor = vet[ r ];
	int i = p-1;
	int j, aux;
	for( j = p; j < r; j++ ) {
		if( vet[j] <= pivor ){
			i++;
			aux = vet[ i ];
			vet[ i ] = vet[ j ];
			vet[ j ] = aux;
		}
	}
	vet[ r ] = vet[ i+1 ];
	vet[ i+1 ] = pivor;			
	return i+1;		
}


int particiona(vetor vet, int p, int r ) {
	int pivor = vet[p];
	int a = p+1;
	int b = r;
	int aux;
	while( 1 ) {
		if ( a < r )
			while( vet[a] <= pivor && a < r )
				a++;
		if ( b > 0 )
			while( vet[b] > pivor && b > 0 )
				b--;
		
		if ( vet[a] == vet[b] ) {
			a++;			
		} else if ( a <= b ) {
			aux = vet[a];
			vet[a] = vet[b];
			vet[b] = aux;
		} else {
			vet[p] = vet[ b ];
			vet[b] = pivor;
			return b;
		}
	}
}


void counting_sort(vetor in, vetor out, int max, int tam) {
	vetor aux;
	int i;
	
	for( i = 0; i <= max; i++ )
		aux[ i ] = 0;
	
	for( i = 0; i < tam; i++ ) 
		aux[ in[ i ] ]++;
	
	for( i = 1; i <= max; i++ )
		aux[ i ] += aux[ i-1 ];
	
	for( i = 0; i < tam; i++ ) {
		out[ aux[ in[ i ] ] ] = in[ i ];
		aux[ in[ i ] ]--;
	}
}

void radix_sort(int** arranjos, int tam, int d) {
	int i, j, k;
	int aux;
	int ordenado;	
	for( k = d-1; k >= 0; k-- ) {
		do {
			ordenado = 1;
			for( i = 0; i < tam-1; i++ ) {
				if ( arranjos[ i ][ k ] > arranjos[ i+1 ][ k ] ) {					
					for( j = 0; j < d; j++ ) {
						aux = arranjos[ i ][ j ];
						arranjos[ i ][ j ] = arranjos[ i+1 ][ j ];
						arranjos[ i+1 ][ j ] = aux;
					}					
					ordenado = 0;
				}
			}
		} while( !ordenado );
	}
}

void vetor_para_arranjos( int*** arranjos, vetor vet, int tam, int* d) {
	int i, j;
	int num, potencia10, pot10;
	int max;
	
	max = busca_max( vet, tam );
	*d = ((int)log10( abs( max ) ))+1;
	
	*arranjos = (int**)malloc( tam * sizeof(int*) );
	for( i = 0; i < tam; i++ )
		(*arranjos)[ i ] = (int*)malloc( (*d) * sizeof(int) );
		
	potencia10 = (int)round(pow( 10.0, (*d)-1 ));
	for( i = 0; i < tam; i++ ) {
		num = vet[ i ];
		
		pot10 = potencia10;
		for( j = 0; j < *d; j++ ) {			
			(*arranjos)[ i ][ j ] = num / pot10;
			num -= (*arranjos)[ i ][ j ] * pot10;
			pot10 /= 10;
		}
	}	
}

void arranjos_para_vetor( int** arranjos, vetor vet, int tam, int d ) {
	int i, j;
	int num, potencia10, pot10;
	int naozero;

	potencia10 = (int)round(pow( 10.0, d-1 ));
	for( i = 0; i < tam; i++ ) {
		num = 0;
		
		pot10 = potencia10;
		naozero = 0;
		for( j = 0; j < d; j++ ) {
			if ( !naozero || ( naozero && arranjos[ i ][ j ] != 0 ) ) {			
				num += arranjos[ i ][ j ] * pot10;
				naozero = 1;
			}
			pot10 /= 10;
		}			
		vet[ i ] = num;
	}
}

int busca_max(vetor vet, int tam) {
	int max = INT_MIN;
	int i;
	for( i = 0; i < tam; i++ )
		if ( vet[ i ] > max )
			max = vet[ i ];
	return max;
}

void copia_vetor( vetor v1, vetor v2, int tam ) {
	int i;
	for( i = 0; i < tam; i++ )
		v1[ i ] = v2[ i ];
}

long long get_ms() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return (((long long)(t.tv_sec) * 1000)) + (t.tv_usec / 1000);
}
