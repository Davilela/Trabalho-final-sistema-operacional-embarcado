#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das estruturas de dados
typedef struct {
    int id;
    int duracao;
    int prioridade;
} Processo;

typedef struct {
    Processo processos[10];
    int tamanho;
} Buffer;

typedef struct {
    Processo processos[20];
    int tamanho;
} FilaSJF;

typedef struct {
    Processo processos[20];
    int prioridades[20];
    int tamanho;
} FilaLoteria;

// Função de comparação para o qsort (para o SJF)
int compararProcessos(const void *a, const void *b) {
    const Processo *processoA = (const Processo *)a;
    const Processo *processoB = (const Processo *)b;

    // Comparar pela duração
    if (processoA->duracao < processoB->duracao) {
        return -1;
    } else if (processoA->duracao > processoB->duracao) {
        return 1;
    } else {
        // Em caso de duração igual, comparar pela prioridade (maior prioridade primeiro)
        if (processoA->prioridade < processoB->prioridade) {
            return 1;
        } else if (processoA->prioridade > processoB->prioridade) {
            return -1;
        } else {
            // Em caso de prioridade igual, manter a ordem original
            return 0;
        }
    }
}
// Função para gerar um número aleatório entre min e max (inclusivos)
int gerarNumeroAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Função para adicionar um processo ao buffer
void adicionarProcessoBuffer(Buffer *buffer, Processo processo) {
    if (buffer->tamanho < 10) {
        buffer->processos[buffer->tamanho++] = processo;
        printf("Processo '%d' adicionado ao buffer. Duração: %d, Prioridade: %d\n", processo.id, processo.duracao, processo.prioridade);
    } else {
        printf("O buffer está cheio. Não foi possível adicionar o processo '%d'.\n", processo.id);
    }
}

// Função para adicionar um processo à fila SJF
void adicionarProcessoSJF(FilaSJF *fila, Processo processo) {
    if (fila->tamanho < 20) {
        fila->processos[fila->tamanho++] = processo;
        printf("Processo '%d' adicionado à fila SJF. Duração: %d, Prioridade: %d\n", processo.id, processo.duracao, processo.prioridade);
    } else {
        printf("A fila SJF está cheia. Não foi possível adicionar o processo '%d'.\n", processo.id);
    }
}

// Função para adicionar um processo à fila Loteria
void adicionarProcessoLoteria(FilaLoteria *fila, Processo processo) {
    if (fila->tamanho < 20) {
        fila->processos[fila->tamanho] = processo;
        fila->prioridades[fila->tamanho] = processo.prioridade;
        fila->tamanho++;
        printf("Processo '%d' adicionado à fila Loteria. Duração: %d, Prioridade: %d\n", processo.id, processo.duracao, processo.prioridade);
    } else {
        printf("A fila Loteria está cheia. Não foi possível adicionar o processo '%d'.\n", processo.id);
    }
}

// Função para executar a fila SJF
void executarSJF(FilaSJF *fila) {
    if (fila->tamanho == 0) {
        printf("Não há processos na fila SJF.\n");
        return;
    }

    // Ordenar a fila pelo tempo de duração do processo e prioridade
    qsort(fila->processos, fila->tamanho, sizeof(Processo), compararProcessos);

    // Executar os processos da fila SJF e incrementar o tempo
    int tempo = 0;
    for (int i = 0; i < fila->tamanho; i++) {
        Processo processoExecutado = fila->processos[i];
        printf("Executando o processo '%d' da fila SJF. Tempo: %d. Duração: %d, Prioridade: %d\n", processoExecutado.id, tempo, processoExecutado.duracao, processoExecutado.prioridade);
        tempo += processoExecutado.duracao;
    }

    fila->tamanho = 0;
}

// Função para executar a fila Loteria
void executarLoteria(FilaLoteria *fila) {
    if (fila->tamanho == 0) {
        printf("Não há processos na fila Loteria.\n");
        return;
    }

    int tempo = 0;
    int bilheteVencedor = gerarNumeroAleatorio(1, 100);
    int totalPrioridades = 0;

    // Calcular o total de prioridades para determinar as chances de cada processo
    for (int i = 0; i < fila->tamanho; i++) {
        totalPrioridades += fila->prioridades[i];
    }

    // Executar os processos da fila Loteria e incrementar o tempo
    while (fila->tamanho > 0) {
        int bilheteSorteado = gerarNumeroAleatorio(1, totalPrioridades);
        int somaPrioridades = 0;

        for (int i = 0; i < fila->tamanho; i++) {
            somaPrioridades += fila->prioridades[i];

            if (somaPrioridades >= bilheteSorteado) {
                Processo processoExecutado = fila->processos[i];
                printf("Executando o processo '%d' da fila Loteria. Tempo: %d. Duração: %d, Prioridade: %d\n", processoExecutado.id, tempo, processoExecutado.duracao, processoExecutado.prioridade);
                tempo += processoExecutado.duracao;

                // Remover o processo da fila Loteria
                for (int j = i; j < fila->tamanho - 1; j++) {
                    fila->processos[j] = fila->processos[j + 1];
                    fila->prioridades[j] = fila->prioridades[j + 1];
                }
                fila->tamanho--;

                break;
            }
        }
    }
}

int main() {
    Buffer buffer;
    buffer.tamanho = 0;

    FilaSJF filaSJF;
    filaSJF.tamanho = 0;

    FilaLoteria filaLoteria;
    filaLoteria.tamanho = 0;

    srand(time(NULL)); // Inicializar a semente para geração de números aleatórios

    // Adicionar processos ao buffer e transferi-los para a fila SJF e Loteria
    for (int i = 1; i <= 20; i++) {
        Processo processo = {i, rand() % 10 + 1, rand() % 5 + 1};

        // Adicionar processo ao buffer
        adicionarProcessoBuffer(&buffer, processo);

        // Transferir processos do buffer para a fila SJF e Loteria quando o buffer estiver cheio ou todos os processos tiverem sido adicionados
        if (buffer.tamanho == 10 || i == 20) {
            for (int j = 0; j < buffer.tamanho; j++) {
                adicionarProcessoSJF(&filaSJF, buffer.processos[j]);
                adicionarProcessoLoteria(&filaLoteria, buffer.processos[j]);
            }
            buffer.tamanho= 0;
        }
    }

    // Executar a fila SJF somente quando todos os processos estiverem na fila
    if (filaSJF.tamanho == 20) {
        printf("Execução da fila SJF:\n");
        executarSJF(&filaSJF);
    }

    // Executar a fila Loteria somente quando todos os processos estiverem na fila
    if (filaLoteria.tamanho == 20) {
        printf("Execução da fila Loteria:\n");
        executarLoteria(&filaLoteria);
    }

    return 0;
}
