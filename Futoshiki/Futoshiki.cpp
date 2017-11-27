/*Nome: Híkaro A. de Oliveira	N°USP:9066487*/

#include <stdio.h>
#include <stdlib.h>
 
#define UNASSIGNED 0
#define N 9

bool FindUnassignedLocation(int grid[N][N], int &row, int &col, int dim);

bool FindUnassignedLocation2(int grid[N][N], int matrix[N][81], int &row, int &col, int dim);

bool isSafe(int grid[N][N], int *v, int row, int col, int num, int dim, int tam);
 
bool isSafe2(int grid[N][N], int matrix[N][81], int *v, int row, int col, int num, int dim, int tam);

/*Quando entramos com um número na matriz, invalidamos todas as outras posições da linha de comportar esse número. Isso na matriz auxiliar*/
void eliminateRow(int linha, int coluna, int mOriginal[N][N], int matrix[N][81], int dim){
	int k, l;
		
	for(k = coluna*dim; k<(coluna*dim)+dim; k++)
		matrix[linha][k] = -1;	//-1 = preenchido
				
	for(k = 0; k<dim*dim; k = k+dim){
					
		if(matrix[linha][k] != -1){
			for(l = k; l < k+dim; l++){
				if(matrix[linha][l] == mOriginal[linha][coluna]){
					matrix[linha][l] = -2; // -2 indica valor não possível
				}
			}
		}
	}
}

/*Quando entramos com um número na matriz, invalidamos todas as outras posições da coluna de comportar esse número. Isso na matriz auxiliar*/
void eliminateColumn(int linha, int coluna, int mOriginal[N][N], int matrix[N][81], int dim){
	int k, col, l; //k para movimentar nas linhas
	
	col = coluna*dim;
	
	for(k=0; k<dim; k++){
		if(matrix[k][col]!= -1){
			for(l=col; l<col+dim; l++){
				if(matrix[k][l] == mOriginal[linha][coluna]){
					matrix[k][l] = -2;
				}
			}
		}
	}
}

/*Aqui, quando entramos com um valor na matriz, e a posição em que entramos esse valor tem algum condição de maior/menor e a posição
afetada está desocupada, então, na matriz auxiliar, tiramos as opções que invalidam a restrição na posição afetada*/
void eliminateRestriction(int linha1, int coluna1, int linha2, int coluna2, int mOriginal[N][N], int matrix[N][81], int dim){
	int aux, i, col;
	
	if(mOriginal[linha1][coluna1] != 0 && mOriginal[linha2][coluna2] == 0){ //tenho que garantir que os valores sejam maiores para m[linha2][coluna2]
		aux = mOriginal[linha1][coluna1];
		
		col = coluna2*dim;
		
		for(i=col; i<col+dim; i++){
			if(matrix[linha2][i] < aux)
				matrix[linha2][i] = -2;
		}
	}
	
	if(mOriginal[linha1][coluna1] == 0 && mOriginal[linha2][coluna2] != 0){ //tenho que garantir que os valores sejam menores para m[linha1][coluna1]
		aux = mOriginal[linha2][coluna2];
		
		col = coluna1*dim;
		
		for(i=col; i<col+dim; i++){
			if(matrix[linha1][i] > aux)
				matrix[linha1][i] = -2;
		}	
	}	
}

bool SolveFutoshiki(int grid[N][N], int *v, int dim, int tam){
    int i, j, row, col;
 
    // Se não tiver nenhum valor em branco, acabamos
    if (!FindUnassignedLocation(grid, row, col, dim))
       return true; // successo
 
    // Considerar dígitos de 1 a dimensão do jogo
    for (int num = 1; num <= dim; num++){
        // Se parecer promissor
        if (isSafe(grid, v, row, col, num, dim, tam)){
            // tentamos escrever
            grid[row][col] = num;
 			
            // Retorna se obtiver sucesso
            if (SolveFutoshiki(grid, v, dim, tam))
                return true;
 
            // falha, desfaz e faz novamente
            grid[row][col] = UNASSIGNED;
        }
    }
    return false;
}

bool SolveFutoshiki2(int grid[N][N], int matrix[N][81], int *v, int dim, int tam){
    int row, col, i, j, matrixAux[N][81];
    int lin1, lin2, col1, col2;
 
    // Se não tiver nenhum valor em branco, acabamos
    if (!FindUnassignedLocation(grid, row, col, dim))
       return true; // success!
 
    // Considerar dígitos de 1 a dimensão do jogo
    for (int num = 1; num <= dim; num++){
        // Se parecer promissor
        if (isSafe2(grid, matrix, v, row, col, num, dim, tam)){
        	
        	// tentamos escrever
			grid[row][col] = num;
            
            for(i=0; i<dim; i++){
				for(j=0; j<dim*dim; j++){
					matrixAux[i][j] = matrix[i][j];
				}
			}
			
			grid[row][col] = num;
			
 
            // retorna, se obtiver sucesso
            if (SolveFutoshiki2(grid, matrix, v, dim, tam))
                return true;
 
            // falha, desfaz e refaz
            grid[row][col] = UNASSIGNED;
            
				for(i=0; i<dim; i++){
				for(j=0; j<dim*dim; j++){
					matrix[i][j] = matrixAux[i][j];
				}
			}
        }
    }
    return false;
}

bool SolveFutoshiki3(int grid[N][N], int matrix[N][81], int *v, int dim, int tam){
    int row, col, i, j, matrixAux[N][81];
    int lin1, lin2, col1, col2, atrib=0;
 
    // Se não tiver nenhum valor em branco, acabamos
    if (!FindUnassignedLocation2(grid, matrix, row, col, dim))
       return true; // success!
 
    // Considerar dígitos de 1 a dimensão do jogo
    for (int num = 1; num <= dim; num++){
        // Se parecer promissor
        if (isSafe2(grid, matrix, v, row, col, num, dim, tam)){
            // tentamos escrever
            for(i=0; i<dim; i++){
				for(j=0; j<dim*dim; j++){
					matrixAux[i][j] = matrix[i][j];
				}
			}
			
			grid[row][col] = num;
			
			eliminateRow(row, col, grid, matrix, dim);
			eliminateColumn(row, col, grid, matrix, dim);
			
			for(i = 0; i < tam; i = i+4){
			
				lin1 = v[i];
				col1 = v[i+1];
				lin2 = v[i+2];
				col2 = v[i+3];
				
				if(lin1 == row && col1 == col){
					if(grid[lin2][col2] == 0)
						eliminateRestriction(lin1, col1, lin2, col2, grid, matrix, dim);
				}
				
				else if(lin2 == row && col2 == col){
					if(grid[lin1][col1] == 0)
						eliminateRestriction(lin1, col1, lin2, col2, grid, matrix, dim);
				}
			}
			
 
            // retorna,se obtiver sucesso
            if (SolveFutoshiki3(grid, matrix, v, dim, tam))
                return true;
 
            // falha, desfaz e refaz
            grid[row][col] = UNASSIGNED;
            
            for(i=0; i<dim; i++){
				for(j=0; j<dim*dim; j++){
					matrix[i][j] = matrixAux[i][j];
				}
			}
        }
    }
    return false;
}

/* Essa função acha uma posição no jogo a qual ainda não foi preenchida, faz isso
de forma linear*/
bool FindUnassignedLocation(int grid[N][N], int &row, int &col, int dim){
    for (row = 0; row < dim; row++)
        for (col = 0; col < dim; col++)
            if (grid[row][col] == UNASSIGNED)
                return true;
    return false;
} 

/*Aqui, achamos uma posição na matriz que tenha menor número de possibilidades de valores. Para isso,
percorremos a matriz auxiliar e pegamos o primeiro valor que tiver o menos posibilidades em relação aos
outros*/
bool FindUnassignedLocation2(int grid[N][N], int matrix[N][81], int &row, int &col, int dim){
    int i, j, k, count = 0;
	int maxNum = 0, linMaior = 0, colMaior = 0;
	
	for(k=0; k<dim; k++){			//Movemos as linhas
		for(i=0; i<dim*dim; i=i+dim){	//Move nas linhas, sempre vai da posição até posição mais dimensão
			for(j=i; j<i+dim; j++){		//Na posição referente a matriz original, andamos por suas possibilidades
				if(matrix[k][j] == -2){
					count++;
				}
			}
			
			if(count == dim-1){
				row = k;
				col = i/dim;	//col identifica a correspondência da matriz de possibilidades na original
				return true;
			}
						
			if(maxNum<count){
				maxNum=count;
				linMaior = k;
				colMaior = i/dim;
			}
			
			count = 0;
		}
	}
	
	if(maxNum != 0){
		row = linMaior;
		col = colMaior;
		return true;
	}
	
	/*Se não tiver nenhuma posição com menos possibilidades,
	fazemos uma busca linear pela matriz até achar valor nulo*/
	for (row = 0; row < dim; row++)
        for (col = 0; col < dim; col++)
            if (grid[row][col] == UNASSIGNED)
                return true;
    return false;
	
}
 
 /*Retorna um valor booleano que indica se qualquer entrada preenchida
 na linha especificada tem correspondência com o número dado*/
bool UsedInRow(int grid[N][N], int row, int num, int dim){
    for (int col = 0; col < dim; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}
 
/*Retorna um valor booleano que indica se qualquer entrada preenchida
 na linha especificada tem correspondência com o número dado*/
bool UsedInCol(int grid[N][N], int col, int num, int dim)
{
    for (int row = 0; row < dim; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}
/* Retorna um valor booleano que indica se a restrição de menor/maior está sendo obedecida
com a entrada do valor no campo especificado na matriz*/
bool MajorMinorSat(int grid[N][N], int *v, int row, int col, int num, int dim, int tam)
{
    int i, restri = 0;
    int row1, col1, row2, col2;
    
    for (i = 0; i < tam; i = i+2){
    	if(v[i] == row && v[i+1] == col){
    		restri++;
			//printf("(%d %d)", row, col);
		}
	}
	
	if(restri == 0)
		return false;
	
	else{
		
		for(i = 0; i < tam; i = i+4){
			
			row1 = v[i];
			col1 = v[i+1];
			row2 = v[i+2];
			col2 = v[i+3];
				
			if(row1 == row && col1 == col){
				if(num > grid[row2][col2] && grid[row2][col2] != 0)
					return true;
			}
				
			else if(row2 == row && col2 == col){
				if(num < grid[row1][col1] && grid[row1][col1] != 0)
					return true;
			}
				
		}
	return false;	
	}
}

/*Verifica se o valor escalado para ocupar dada posição, não invalida
o preenchimento por qualuquer outro número em outra posição na linha*/
bool verifyRow(int linha, int matrix[N][81], int dim){
	int i, j, count = 0;
	
	for(i=0; i<dim*dim; i=i+dim){
		for(j=i*dim; j<(i*dim)+dim; j++){
			if(matrix[linha][j] == -2)
				count++;
		}
		if(count == dim)
			return false;
		
		else
			count=0;
	}
	return true;	
}

/*Verifica se o valor escalado para ocupar dada posição, não invalida
o preenchimento por qualuquer outro número em outra posição na coluna*/
bool verifyColumn(int coluna, int matrix[N][81], int dim){
	int k, col, l, count=0; //k para movimentar nas linhas
	
	col = coluna*dim;
	
	for(k=0; k<dim; k++){
		for(l=col; l<col+dim; l++){
			if(matrix[k][l] == -2){
				count++;
			}
		}
		
		if(count == dim)
			return false;
		
		else
			count = 0;
	}
	return true;
}

/*Verifica se o valor escalado para ocupar dada posição, não invalida o preenchimento por qualuquer outro número em outra
 posição na linha, coluna ou restrições de maior/menor*/
bool PossibleResult(int mOriginal[N][N], int matrix[N][81], int *v, int linha, int coluna, int num, int dim, int tam){
	int matrixAux[N][81], gridAux[N][N];
	int i, j, lin1, lin2, col1, col2;
	
	for(i=0; i<dim; i++){
		for(j=0; j<dim*dim; j++){
			matrixAux[i][j] = matrix[i][j];
		}
	}
	
	for(i=0; i<dim; i++){
		for(j=0; j<dim; j++){
			gridAux[i][j] = mOriginal[i][j];
		}
	}
	
	gridAux[linha][coluna]=num;
	
	eliminateRow(linha, coluna, gridAux, matrixAux, dim);
	eliminateColumn(linha, coluna, gridAux, matrixAux, dim);
	
	 for(i = 0; i < tam; i = i+4){
			
			lin1 = v[i];
			col1 = v[i+1];
			lin2 = v[i+2];
			col2 = v[i+3];
				
			if(lin1 == linha && col1 == coluna){
				if(mOriginal[lin2][col2] == 0)
					eliminateRestriction(lin1, col1, lin2, col2, gridAux, matrixAux, dim);
			}
				
			else if(lin2 == linha && col2 == coluna){
				if(mOriginal[lin1][col1] == 0)
					eliminateRestriction(lin1, col1, lin2, col2, gridAux, matrixAux, dim);
			}
		}
		
		if(!MajorMinorSat(mOriginal, v, linha, coluna, num, dim, tam) && 
			verifyRow(linha, matrixAux, dim) &&
			verifyColumn(coluna, matrixAux, dim)){
				return false;
		}
	return true;
}
 
/*Retorna um valor booleano que indica se a entrada de num em dada linha, coluna 
será valida*/
bool isSafe(int grid[N][N], int *v, int row, int col, int num, int dim, int tam){

    return !UsedInRow(grid, row, num, dim) &&
           !UsedInCol(grid, col, num, dim) &&
           !MajorMinorSat(grid, v, row, col, num, dim, tam);
}
 
/*Retorna um valor booleano que indica se a entrada de num em dada linha, coluna 
será valida*/
bool isSafe2(int grid[N][N], int matrix[N][81], int *v, int row, int col, int num, int dim, int tam){
   
    return !UsedInRow(grid, row, num, dim) &&
           !UsedInCol(grid, col, num, dim) &&
           !PossibleResult(grid, matrix, v, row, col, num, dim, tam);
}

/* Função para printar a matriz */
void printGrid(int grid[N][N], int dim){
    
	for (int row = 0; row < dim; row++)
    {
       for (int col = 0; col < dim; col++)
             printf("%2d", grid[row][col]);
        printf("\n");
    }
}

int main(){
	int matrix[9][81];
	int m[N][N];
	int i, j, k, l, dim, restr, tam, cases, aux;
	int v[300];
	int lin1, col1, lin2, col2, flagHeu;
	
	/* MODIFICAR HEURÍSTICA PELA FLAG! 
	1 para backtracking simples
	2 para backtracking com verificação adiante
	3 para backtracking com verificação adiante e MVR */
	
	flagHeu = 1;
	
	scanf("%d", &cases);
	
	for(aux = 0; aux < cases; aux++){
	
		scanf("%d %d", &dim, &restr);
		tam=4*restr;
		
		/*Para cada dimensão de matriz, lemos ela de uma forma diferente, pegando todos elementos
		da linha por vez*/
		
		if(dim==4){
			
			/*A matriz auxiliar só é utilizada com as heurísticas, então só é analisada quando usamos estas*/
			if(flagHeu != 1){
			
				for(i=0; i<dim; i++){
					for(j=0;j<4*dim;j=j+4){
						matrix[i][j]=1;
						matrix[i][j+1]=2;
						matrix[i][j+2]=3;
						matrix[i][j+3]=4;
					}
				}
		
				for(i=0;i<dim;i++){
					scanf("%d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3]);
					
					/*depois de ler, já tiramos as possibilidades em linha e coluna que foram cortadas com
					o preenchimento de alguns campos com determinado valor*/
					for(j = 0; j<dim; j++){
						if(m[i][j] != 0){
							eliminateRow(i, j, m, matrix, dim);
							eliminateColumn(i, j, m, matrix, dim);
						}
					}	
				}
			}
			else{
				for(i=0;i<dim;i++)
					scanf("%d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3]);
			}
		}	
	
		if(dim==5){
			
			/*A matriz auxiliar só é utilizada com as heurísticas, então só é analisada quando usamos estas*/
			if(flagHeu != 1){
			
				for(i=0; i<dim; i++){
					for(j=0;j<5*dim;j=j+5){
						matrix[i][j]=1;
						matrix[i][j+1]=2;
						matrix[i][j+2]=3;
						matrix[i][j+3]=4;
						matrix[i][j+4]=5;
					}
				}
			
				for(i=0;i<dim;i++){
					scanf("%d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4]);
					
					/*depois de ler, já tiramos as possibilidades em linha e coluna que foram cortadas com
					o preenchimento de alguns campos com determinado valor*/
					for(j = 0; j<dim; j++){
						if(m[i][j] != 0){
							eliminateRow(i, j, m, matrix, dim);
							eliminateColumn(i, j, m, matrix, dim);
						}	
					}
				}
			}
			else{
				for(i=0;i<dim;i++)
					scanf("%d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4]);
			}
		}
	
		if(dim==6){
			
			/*A matriz auxiliar só é utilizada com as heurísticas, então só é analisada quando usamos estas*/
			if(flagHeu != 1){
					
				for(i=0; i<dim; i++){
					for(j=0;j<6*dim;j=j+6){
						matrix[i][j]=1;
						matrix[i][j+1]=2;
						matrix[i][j+2]=3;
						matrix[i][j+3]=4;
						matrix[i][j+4]=5;
						matrix[i][j+5]=6;
					}
				}
				
				for(i=0;i<dim;i++){
					scanf("%d %d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4], &m[i][5]);
					
					/*depois de ler, já tiramos as possibilidades em linha e coluna que foram cortadas com
					o preenchimento de alguns campos com determinado valor*/
					for(j = 0; j<dim; j++){
						if(m[i][j] != 0){
							eliminateRow(i, j, m, matrix, dim);
							eliminateColumn(i, j, m, matrix, dim);
						}
					}
				}
			}
			
			else{
				for(i=0;i<dim;i++)
					scanf("%d %d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4], &m[i][5]);
			}
		}
		
		if(dim==7){
			
			/*A matriz auxiliar só é utilizada com as heurísticas, então só é analisada quando usamos estas*/
			if(flagHeu != 1){
				
				for(i=0; i<dim; i++){
					for(j=0;j<7*dim;j=j+7){
						matrix[i][j]=1;
						matrix[i][j+1]=2;
						matrix[i][j+2]=3;
						matrix[i][j+3]=4;
						matrix[i][j+4]=5;
						matrix[i][j+5]=6;
						matrix[i][j+6]=7;
					}		
				}
				
				/*depois de ler, já tiramos as possibilidades em linha e coluna que foram cortadas com
				o preenchimento de alguns campos com determinado valor*/
				for(i=0;i<dim;i++){
					scanf("%d %d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4], &m[i][5]);
					for(j = 0; j<dim; j++){
						if(m[i][j] != 0){
							eliminateRow(i, j, m, matrix, dim);
							eliminateColumn(i, j, m, matrix, dim);
						}
					}
				}	
			}
			
			else{
				for(i=0;i<dim;i++)
					scanf("%d %d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4], &m[i][5]);
			}
		}
	
		if(dim==8){
			
			/*A matriz auxiliar só é utilizada com as heurísticas, então só é analisada quando usamos estas*/
			if(flagHeu != 1){
			
				for(i=0; i<dim; i++){
					for(j=0;j<8*dim;j=j+8){
						matrix[i][j]=1;
						matrix[i][j+1]=2;
						matrix[i][j+2]=3;
						matrix[i][j+3]=4;
						matrix[i][j+4]=5;
						matrix[i][j+5]=6;
						matrix[i][j+6]=7;
						matrix[i][j+7]=8;
					}
				}
			
				for(i=0;i<dim;i++){
					scanf("%d %d %d %d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4], &m[i][5], &m[i][6], &m[i][7]);
					
					/*depois de ler, já tiramos as possibilidades em linha e coluna que foram cortadas com
					o preenchimento de alguns campos com determinado valor*/
					for(j = 0; j<dim; j++){
						if(m[i][j] != 0){
							eliminateRow(i, j, m, matrix, dim);
							eliminateColumn(i, j, m, matrix, dim);
						}
					}
				}
			}
			else{
				for(i=0;i<dim;i++)
					scanf("%d %d %d %d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4], &m[i][5], &m[i][6], &m[i][7]);
			}
		}
	
		if(dim==9){
			
			/*A matriz auxiliar só é utilizada com as heurísticas, então só é analisada quando usamos estas*/
			if(flagHeu != 1){
			
				for(i=0; i<dim; i++){
					for(j=0;j<9*dim;j=j+9){
						matrix[i][j]=1;
						matrix[i][j+1]=2;
						matrix[i][j+2]=3;
						matrix[i][j+3]=4;
						matrix[i][j+4]=5;
						matrix[i][j+5]=6;
						matrix[i][j+6]=7;
						matrix[i][j+7]=8;
						matrix[i][j+8]=9;
					}
				}
				
				/*depois de ler, já tiramos as possibilidades em linha e coluna que foram cortadas com
				o preenchimento de alguns campos com determinado valor*/
				for(i=0;i<dim;i++){
					scanf("%d %d %d %d %d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4], &m[i][5], &m[i][6], &m[i][7], &m[i][8]);
				
					for(j = 0; j<dim; j++){
						if(m[i][j] != 0){
							eliminateRow(i, j, m, matrix, dim);
							eliminateColumn(i, j, m, matrix, dim);
						}
					}
				}
			}
			else{
				for(i=0;i<dim;i++)
					scanf("%d %d %d %d %d %d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3], &m[i][4], &m[i][5], &m[i][6], &m[i][7], &m[i][8]);
			}
		}
	
		for(i=0;i<restr;i++){
			scanf("%d %d %d %d", &v[4*i], &v[(4*i)+1], &v[(4*i)+2], &v[(4*i)+3]);
			v[4*i]--; //Apenas normalizo para que as posições comecem com linhas e colunas no zero
			v[(4*i)+1]--;
			v[(4*i)+2]--;
			v[(4*i)+3]--;
		}
	
	//Agora preciso garantir que os cortes em relação as restrições de menor/maior sejam feitas
		
		if(flagHeu != 1){
			for (i = 0; i < tam; i = i+4){
				lin1 = v[i];
				col1 = v[i+1];
				lin2 = v[i+2];
				col2 = v[i+3];
			
				eliminateRestriction(lin1, col1, lin2, col2, m, matrix, dim);
			}
		}
		
		if(flagHeu == 1){
			if (SolveFutoshiki(m, v, dim, tam) == true)
          		printGrid(m, dim);
		}
		
		else if(flagHeu == 2){
			if (SolveFutoshiki2(m, matrix, v, dim, tam) == true)
          		printGrid(m, dim);
		}
		
		else if(flagHeu == 3){
			if (SolveFutoshiki3(m, matrix, v, dim, tam) == true)
          		printGrid(m, dim);
		}
			
		printf("\n");
	}
return 0;
}
