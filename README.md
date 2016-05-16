Trabalho Disciplina de IPPD - CMP134

Dieison Silveira

Multiplicação de dois vetores de tamanho idêntico
Tamanho do vetor é de 250 milhões de posições de floats

O tempo de execucao pode ser medido pela função "MPI_Wtime()", esse tempo eh medido com inicio antes das alocações dos vetores e fim antes da função "MPI_Finalize()"

Compilar:
$ cd DieisonSilveira/
$ make

Executar com a função "MPI_Wtime()" habilitada:
$ mpirun -np <#nodos> ./exec -time

Executar sem a função "MPI_Wtime()":
$ mpirun -np <#nodos> ./exec
