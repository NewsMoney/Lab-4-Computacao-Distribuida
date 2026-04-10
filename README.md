# Atividade 1: Soma de Quadrados com MPI (`MPI_Scatter` e `MPI_Reduce`)

## Descrição

Nesta atividade, será implementado um programa em **C utilizando MPI (Message Passing Interface)** para calcular a **soma dos quadrados dos números de 1 a 40** de forma paralela.

O programa distribui os dados entre múltiplos processos, realiza cálculos locais e utiliza comunicação coletiva para obter o resultado global, validando-o com uma abordagem sequencial.

---

## Objetivo

Aplicar conceitos fundamentais de programação paralela com MPI, incluindo:

* Distribuição de dados com `MPI_Scatter`
* Processamento paralelo
* Redução de resultados com `MPI_Reduce`
* Validação de resultados paralelos

---

## Funcionamento

1. O processo **root (rank 0)** cria um vetor contendo os inteiros de `1` a `N`, onde `N = 40`.

2. O vetor é dividido igualmente entre os processos utilizando `MPI_Scatter`.

3. Cada processo:

   * Recebe uma parte do vetor
   * Calcula a **soma dos quadrados local**

4. As somas locais são enviadas ao processo root utilizando `MPI_Reduce`.

5. O processo root:

   * Calcula a soma sequencial dos quadrados
   * Compara com o resultado paralelo
   * Valida se os resultados coincidem

---

## Regras e Restrições

* O valor de `N` é fixo em 40
* O número de processos deve dividir `N` exatamente
* Utilizar apenas:

  * `MPI_Scatter`
  * `MPI_Reduce`
* Não utilizar:

  * `MPI_Gather`
  * `MPI_Probe`
  * `MPI_Cancel`
  * `MPI_Allgather`

---

## Validação

A validação pode ser feita utilizando a fórmula matemática:

```math
\sum_{i=1}^{N} i^2 = \frac{N(N+1)(2N+1)}{6}
```

Para `N = 40`, o resultado esperado é **22140**.

---

## Compilação

```bash
mpicc -o soma_quadrados soma_quadrados.c
```

---

## Execução

```bash
mpirun -np 4 ./soma_quadrados
```

---

## Exemplo de Saída Esperada

```
Processo 0 recebeu: 1 2 3 4 5 6 7 8 9 10
Processo 1 recebeu: 11 12 13 14 15 16 17 18 19 20
Processo 2 recebeu: 21 22 23 24 25 26 27 28 29 30
Processo 3 recebeu: 31 32 33 34 35 36 37 38 39 40

Processo 0: soma local dos quadrados = 385
Processo 1: soma local dos quadrados = 1540
Processo 2: soma local dos quadrados = 3555
Processo 3: soma local dos quadrados = 9660

Processo 0: soma paralela dos quadrados = 22140
Processo 0: soma sequencial esperada    = 22140

✅ Os valores conferem!
```

---

## Testes Automatizados

Os testes do projeto são executados automaticamente utilizando **GitHub Actions**, garantindo que:

* O código compile corretamente
* O programa execute sem erros
* O resultado paralelo seja validado corretamente

---

## Envio da Atividade

A entrega deve conter um **PDF organizado** com:

* Prints da execução
* Explicação do funcionamento
* Código-fonte (ou link para repositório)
* Evidência dos testes automatizados (GitHub Actions)
* Nome de todos os integrantes

⚠️ Mantenha o material organizado e bem estruturado.
