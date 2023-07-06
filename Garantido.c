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
    int tempos[20];
    int prioridades[20];
    int tamanho;
} FilaGarantido;

// Função de comparação para o qsort
int compararProcessos(const void *a, const void *b) {
    const Processo *processoA = (const Processo *)a;
    const Processo *processoB = (const Processo *)b;

    // Comparar pela prioridade (maior prioridade primeiro)
    if (processoA->prioridade < processoB->prioridade) {
        return 1;
    } else if (processoA->prioridade > processoB->prioridade) {
        return -1;
    } else {
        // Em caso de prioridade igual, manter a ordem original
        return 0;
    }
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

// Função para adicionar um processo à fila Garantido
void adicionarProcessoGarantido(FilaGarantido *fila, Processo processo) {
    if (fila->tamanho < 20) {
        fila->processos[fila->tamanho] = processo;
        fila->tempos[fila->tamanho] = 0;
        fila->prioridades[fila->tamanho] = processo.prioridade;
        fila->tamanho++;
        printf("Processo '%d' adicionado à fila Garantido. Duração: %d, Prioridade: %d\n", processo.id, processo.duracao, processo.prioridade);
    } else {
        printf("A fila Garantido está cheia. Não foi possível adicionar o processo '%d'.\n", processo.id);
    }
}

// Função para executar a fila Garantido
void executarGarantido(FilaGarantido *fila) {
    if (fila->tamanho == 0) {
        printf("Não há processos na fila Garantido.\n");
        return;
    }

    // Ordenar a fila pela prioridade (maior prioridade primeiro)
    qsort(fila->processos, fila->tamanho, sizeof(Processo), compararProcessos);

    // Executar os processos da fila Garantido e incrementar o tempo
    int tempo = 0;
    for (int i = 0; i < fila->tamanho; i++) {
        Processo *processoExecutado = &fila->processos[i];
        printf("Executando o processo '%d' da fila Garantido. Tempo: %d. Duração: %d, Prioridade: %d\n", processoExecutado->id, tempo, processoExecutado->duracao, processoExecutado->prioridade);
        for (int t = 0; t < processoExecutado->duracao; t++) {
            tempo += 1;
            printf("Tempo: %d\n", tempo);
        }

        // Remover o processo da fila Garantido
        for (int j = i + 1; j < fila->tamanho; j++) {
            fila->processos[j - 1] = fila->processos[j];
            fila->tempos[j - 1] = fila->tempos[j];
            fila->prioridades[j - 1] = fila->prioridades[j];
        }
        fila->tamanho--;
        i--; // Decrementar o índice após remover um processo
    }
}

int main() {
    Buffer buffer;
    buffer.tamanho = 0;

    FilaGarantido filaGarantido;
    filaGarantido.tamanho = 0;

    srand(time(NULL)); // Inicializar a semente para geração de números aleatórios

    // Adicionar processos ao buffer e transferi-los para a fila Garantido
    for (int i = 1; i <= 20; i++) {
        Processo processo = {i, rand() % 10 + 1, rand() % 5 + 1};

        // Adicionar processo ao buffer
        adicionarProcessoBuffer(&buffer, processo);

        // Transferir processos do buffer para a fila Garantido quando o buffer estiver cheio ou todos os processos tiverem sido adicionados
        if (buffer.tamanho == 10 || i == 20) {
            for (int j = 0; j < buffer.tamanho; j++) {
                adicionarProcessoGarantido(&filaGarantido, buffer.processos[j]);
            }
            buffer.tamanho = 0;
        }
    }

    // Executar a fila Garantido somente quando todos os processos estiverem na fila
    if (filaGarantido.tamanho == 20) {
        printf("Execução da fila Garantido:\n");
        executarGarantido(&filaGarantido);
    }

    return 0;
}
