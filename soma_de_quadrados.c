#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Definimos N como 40 por padrão, mas o código aceita N via argumento de linha de comando
#define DEFAULT_N 40

int main(int argc, char *argv[]) {
    int rank = 0, size = 0; // Inicialização explícita para evitar alertas de linters
    int *global_array = NULL;
    int local_size;
    int *local_array;
    int local_sum = 0, global_sum = 0;
    int n_value = DEFAULT_N;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Se um argumento for passado, usamos como o valor de N
    if (argc > 1) {
        n_value = atoi(argv[1]);
    }

    // Garantir divisão exata
    if (n_value % size != 0) {
        if (rank == 0) {
            printf("Erro: N (%d) não é divisível pelo número de processos (%d)\n", n_value, size);
        }
        MPI_Finalize();
        return 1;
    }

    local_size = n_value / size;
    local_array = (int *)malloc(local_size * sizeof(int));

    // Root cria o vetor
    if (rank == 0) {
        global_array = (int *)malloc(n_value * sizeof(int));
        for (int i = 0; i < n_value; i++) {
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
        long long soma_sequencial = 0; // Usar long long para evitar overflow em N grandes

        // cálculo sequencial
        for (int i = 1; i <= n_value; i++) {
            soma_sequencial += (long long)i * i;
        }

        printf("\nProcesso 0: soma paralela dos quadrados = %d\n", global_sum);
        printf("Processo 0: soma sequencial esperada    = %lld\n", soma_sequencial);

        if ((long long)global_sum == soma_sequencial) {
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
