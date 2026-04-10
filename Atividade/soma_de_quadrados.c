#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[]) {
        int rank, size;
    int N; // Agora N será lido do argumento de linha de comando
    int *global_array = NULL;
    int local_size;
    int *local_array;
    int local_sum = 0, global_sum = 0;

    MPI_Init(&argc, &argv);

    if (argc < 2) {
        if (rank == 0) {
            fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    N = atoi(argv[1]);
    if (N <= 0) {
        if (rank == 0) {
            fprintf(stderr, "Erro: N deve ser um número inteiro positivo.\n");
        }
        MPI_Finalize();
        return 1;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Garantir divisão exata
    if (N % size != 0) {
        if (rank == 0) {
            printf("Erro: N (%d) não é divisível pelo número de processos (%d)\n", N, size);
        }
        MPI_Finalize();
        return 1;
    }

    local_size = N / size;
    local_array = (int *)malloc(local_size * sizeof(int));

    // Root cria o vetor
    if (rank == 0) {
        global_array = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            global_array[i] = i + 1;
        }
    }

    // Distribuição
    MPI_Scatter(global_array, local_size, MPI_INT,
                local_array, local_size, MPI_INT,
                0, MPI_COMM_WORLD);

    // Mostrar vetor local
    printf("Processo %d recebeu: ", rank);
    for (int i = 0; i < local_size; i++) {
        printf("%d ", local_array[i]);
    }
    printf("\n");

    // Soma dos quadrados local
    for (int i = 0; i < local_size; i++) {
        local_sum += local_array[i] * local_array[i];
    }

    printf("Processo %d: soma local dos quadrados = %d\n", rank, local_sum);

    // Redução
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Root valida
    if (rank == 0) {
        int soma_sequencial = 0;

        // cálculo sequencial
        for (int i = 1; i <= N; i++) {
            soma_sequencial += i * i;
        }

        printf("\nProcesso 0: soma paralela dos quadrados = %d\n", global_sum);
        printf("Processo 0: soma sequencial esperada    = %d\n", soma_sequencial);

        if (global_sum == soma_sequencial) {
            printf("✅ Os valores conferem!\n");
        } else {
            printf("❌ Os valores NÃO conferem!\n");
        }
    }

    // Liberação
    free(local_array);
    if (rank == 0) {
        free(global_array);
    }

    MPI_Finalize();
    return 0;
}
