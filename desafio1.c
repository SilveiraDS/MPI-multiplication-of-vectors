/**Trabalho Disciplina de IPPD - CMP134
 * 
 * Dieison Silveira
 * 
 * Multiplicação de dois vetores de tamanho idêntico
 * Tamanho do vetor é de 250 milhões de posições de floats
 * TAMANHO = 953.67431640625 MBytes cada vetor
 * Algoritmo
 *   1. malloc do TAMANHO
 *   2. random() retornando entre [0,1) multiplica por =MAX_INT=
 *   - repete por TAMANHO vezes, preenche o primeiro vetor
 *   3. Faz cópia do vetor com memcopy considerando TAMANHO
 *   4. Faz o programa em MPI * 

 * O tempo de execucao pode ser medido pela função "MPI_Wtime()"
 * para isso basta passar o parametro "-time" na execucao
 */


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define TAG 1
#define TAM 250000000
 
int main(int argc, char *argv[]) {
	float *A, *B;
	double startTime, endTime;
	int numElements, offset, stripSize, rank, nodos, i, j;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nodos);

	if (rank == 0) {
		/**start timer*/ 
		if (argc == 2){
			startTime = MPI_Wtime();
		}
		
		if ((A = (float *) malloc (sizeof(float) * TAM)) == NULL){
			fprintf(stderr, "Nao foi possivel alocar memoria!\n");
			exit(1);
		}

		if ((B = (float *) malloc (sizeof(float) * TAM)) == NULL){
			fprintf(stderr, "Nao foi possivel alocar memoria!\n");
			exit(1);
		}
	}else{
		if ((A = (float *) malloc (sizeof(float) * TAM/nodos)) == NULL){
			fprintf(stderr, "Nao foi possivel alocar memoria!\n");
			exit(1);
		}

		if ((B = (float *) malloc (sizeof(float) * TAM/nodos)) == NULL){
			fprintf(stderr, "Nao foi possivel alocar memoria!\n");
			exit(1);
		}
	}


/**porçao do vetor que cada processo terá */
	stripSize = TAM/nodos;

/**envia as fatias do vetores A e B para os processos filhos e parte inicial fica com o pai*/
	if (rank == 0) {
		offset = stripSize;
		numElements = stripSize;
		for (i=1; i<nodos; i++) {
			MPI_Send(&A[offset], numElements, MPI_FLOAT, i, TAG, MPI_COMM_WORLD);
			MPI_Send(&B[offset], numElements, MPI_FLOAT, i, TAG, MPI_COMM_WORLD);
			offset += stripSize;
		}
	}
	else {	// recebe a parte do processo pai referente aos vetores A e B
		MPI_Recv(&A[0], stripSize, MPI_FLOAT, 0, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&B[0], stripSize, MPI_FLOAT, 0, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	

/** o calculo do produto eh realizado por todos os processos*/
	//semente para o random
	srand(time(NULL));
	for (i=0; i < stripSize; i++){
		A[i] = ((float)rand() / (float) RAND_MAX) * INT_MAX; //esse calculo interfere de forma significativa no tempo de execucao.
	}
	//copia o vetor A para o vetor B
	memcpy(&B, &A, sizeof(B));

	for (i=0; i<stripSize; i++) { 
		B[i] = B[i] * A[i];
	}


/**processo pai recebe o resultado de cada filho*/
	if (rank == 0) {
		offset = stripSize;
		numElements = stripSize;
		for (i=1; i<nodos; i++) {
			MPI_Recv(&B[offset], numElements, MPI_FLOAT, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			offset += stripSize;
		}
	}else{ /**os processos filhos enviam o resultado calculado para o pai*/
		numElements = stripSize;
		MPI_Send(&B[0], numElements, MPI_FLOAT, 0, TAG, MPI_COMM_WORLD);
	}

/**stop timer*/
	if (rank == 0) {
		if (argc == 2){
			endTime = MPI_Wtime();
			printf("Time is %f\n", endTime-startTime);
		}
	}

/**imprime o vetor resultante
	if (rank == 0) {
		for (i=0; i<TAM; i++) {
			printf("%.2f ",B[i]);
		}
		printf("\n");
	}
	//*/
	MPI_Finalize();
	return 0;
}
