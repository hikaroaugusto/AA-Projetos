/*
 * Projeto 2 - Programa��o Din�mica - Word Wrap
 *
 * SCC218 - Algoritmos Avan�ados e Aplica��es
 *
 * Autoria:
 * H�karo Augusto de Oliveira - NUSP: 9066487
 *    
 */

#include <limits.h>
#include <stdio.h>
#include <string.h>

#define LIMITE_CHARS 81
#define LIMITE_PALAVRAS 1000

/* Funcao auxiliar para remover quebras de linha ('\t' ou '\n') da string s */
void strip(char *s) {

    char *p2 = s;

    while (*s != '\0') {
        if (*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            s++;
        }
    }

    *p2 = '\0';
}

/* Esta funcao imprime o resultado obtido pela fun��o solucao, e armazenado no vetor sol[]. 
   J� imprime com a sa�da formatada, e indexa a impress�o do vetor sol com a matriz de palavras entradas.

   Parametros:
   sol[]: vetor que indica a solu��o para o problema de word wrap. 
          Cada posi��o indica qual palavra iniciar� a preencher a linha,
   n_palavras: n�mero total de palavras entradas,
   matriz[][]: matriz que obt�m todas as strings entradas, os par�metros indicam a 
               posi��o que � encontrada a palavra e o tamanho desta.
*/
int imprime_solucao(int sol[], int n_palavras, char matriz[LIMITE_PALAVRAS][LIMITE_CHARS]) {

    int i_linha, j;

    if (sol[n_palavras] == 1) {
        i_linha = 1;
    } else {
        i_linha = 1 + imprime_solucao(sol, sol[n_palavras] - 1, matriz);
    }

	for (j = sol[n_palavras]-1; j < n_palavras; j++){
    	printf("%s ", matriz[j]);
	}

	printf("\n");
	return i_linha;
}
 
/* Esta funcao calcula e balanceia o codigo de forma a termos soma dos espa�os 
   remanescentes das palavras inseridas minimizada. Para isso, se baseia na relacao 
   de recorrencia. Primeiramente, constroi e preenche uma matriz de memoizacao, a qual, 
   na iteracao vai ser percorrida e ent�o poderemos preencher o vetor solucao, 
   ao indicar qual palavra come�a com qual linha.

   Parametros:
   tamanhos[]: recebe o tamanho de cada string entrada, para cada posi��o;
   n_palavras: n�mero de palavras entradas;
   max_chars: n�mero m�ximo de caracteres por linha;
   matriz[][]: matriz que obt�m todas as strings entradas, 
               os par�metros indicam a posi��o que � encontrada a palavra e o tamanho desta.

   OBS: os vetores da funcao s�o utilizados a partir do indice 1. 
*/ 
void solucao(int tamanhos[], int n_palavras, int max_chars, char matriz[LIMITE_PALAVRAS][LIMITE_CHARS]) {

    int i, j;

    /* Matriz que, na posicao (i, j), guarda o n�mero de espacos 
       remanescentes caso palavras de i a j sejam alocadas na mesma linha */
    int esp_remanescentes[n_palavras + 1][n_palavras + 1];  
 
    /* Vetor contendo a solucao a ser imprimida */  
    int sol[n_palavras + 1]; 
 
    /* Matriz que, na posicao (i, j), guarda o custo da linha 
       com palavras de i a j */
    int custo_linha[n_palavras + 1][n_palavras + 1];
  
    /* Vetor que, na i-�sima posicao, guarda o custo total do arranjo 
       �timo de palavras de 1 a i */
    int custo[n_palavras + 1];
  
    /* Obtendo espacos remanescentes para cada arranjo de palavras nas linhas */
    for (i = 1; i <= n_palavras; i++) {
        esp_remanescentes[i][i] = max_chars - tamanhos[i-1];

        for (j = i+1; j <= n_palavras; j++) {
            esp_remanescentes[i][j] = esp_remanescentes[i][j - 1] - tamanhos[j - 1] - 1;
        }
    }

    /* Obtendo valores para custo_linha relativos 
       aos espacos remanescentes previamente calculados */
    for (i = 1; i <= n_palavras; i++) {

        for (j = i; j <= n_palavras; j++) {

            if (esp_remanescentes[i][j] < 0) {
                custo_linha[i][j] = INT_MAX;
            } else if (j == n_palavras && esp_remanescentes[i][j] >= 0) {
                custo_linha[i][j] = 0;
            } else {
                custo_linha[i][j] = esp_remanescentes[i][j] * esp_remanescentes[i][j];
            }
        }
    }
 
    /* Obtendo arranjo �timo de palavras al�m de seu custo (custo minimo) */
    custo[0] = 0;
    for (j = 1; j <= n_palavras; j++) {

        custo[j] = INT_MAX;

        for (i = 1; i <= j; i++) {

            if (custo[i-1] != INT_MAX 
                && (custo_linha[i][j] != INT_MAX) 
                && (custo[i-1] + custo_linha[i][j] < custo[j])) {
                
                custo[j] = custo[i-1] + custo_linha[i][j];
                sol[j] = i;
            }
        }
    }
 
    imprime_solucao(sol, n_palavras, matriz);
}

int main() {

	int max_chars, n_palavras, i;
	int tamanhos[LIMITE_CHARS];

    char matrix[LIMITE_PALAVRAS][LIMITE_CHARS];
    char aux[10];
    
	scanf("%d", &max_chars);
	scanf("%d", &n_palavras);
	
	fgets(aux, 10, stdin);
	
	for (i = 0; i < n_palavras; i++) {
		fgets(matrix[i], LIMITE_CHARS, stdin);
		strip(matrix[i]);
		tamanhos[i] = strlen(matrix[i]);
	}
	
	printf("\n");
	
    solucao(tamanhos, n_palavras, max_chars, matrix);
    
    return 0;
}
