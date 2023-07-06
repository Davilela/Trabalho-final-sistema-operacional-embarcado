#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das estruturas de dados
typedef struct {
    int id;
    int duracao;
    int prioridade;
    int contagem;
} Processo;


typedef struct {
    Processo processos[10];
    int tamanho;
} Buffer;


typedef struct {
    Processo processos[20];
    int tempos[20];
    int prioridades[20];
    int contagens[20];
    int tamanho;
} FilaFairShare;

// Função de comparação para o qsort
int compararProcessos(const void *a, const void *b) {
    const Processo *processoA = (const Processo *)a;
    const Processo *processoB = (const Processo *)b;

    // Comparar pela contagem (menor contagem primeiro)
    if (processoA->contagem < processoB->contagem) {
        return -1;
    } else if (processoA->contagem > processoB->contagem) {
        return 1;
    } else {
        // Em caso de contagem igual, comparar pela prioridade (maior prioridade primeiro)
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

// Função para adicionar um processo ao buffer
void adicionarProcessoBuffer(Buffer *buffer, Processo processo) {
    if (buffer->tamanho < 10) {
        buffer->processos[buffer->tamanho++] = processo;
        printf("Processo '%d' adicionado ao buffer. Duração: %d, Prioridade: %d\n", processo.id, processo.duracao, processo.prioridade);
    } else {
        printf("O buffer está cheio. Não foi possível adicionar o processo '%d'.\n", processo.id);
    }
}
// Função para adicionar um processo à fila Fair-Share
void adicionarProcesso(FilaFairShare *fila, Processo processo) {
    if (fila->tamanho < 20) {
        fila->processos[fila->tamanho] = processo;
        fila->tempos[fila->tamanho] = 0;
        fila->prioridades[fila->tamanho] = processo.prioridade;
        fila->contagens[fila->tamanho] = processo.contagem;
        fila->tamanho++;
        printf("Processo '%d' adicionado à fila Fair-Share. Duração: %d, Prioridade: %d, Contagem: %d\n", processo.id, processo.duracao, processo.prioridade, processo.contagem);
    } else {
        printf("A fila Fair-Share está cheia. Não foi possível adicionar o processo '%d'.\n", processo.id);
    }
}

// Função para executar a fila Fair-Share
void executarFairShare(FilaFairShare *fila) {
    if (fila->tamanho == 0) {
        printf("Não há processos na fila Fair-Share.\n");
        return;
    }

    // Ordenar a fila pela contagem (menor contagem primeiro) e prioridade (maior prioridade primeiro)
    qsort(fila->processos, fila->tamanho, sizeof(Processo), compararProcessos);

    // Calcular o total de contagens para determinar a fatia de tempo de cada processo
    int totalContagens = 0;
    for (int i = 0; i < fila->tamanho; i++) {
        totalContagens += fila->contagens[i];
    }

    // Executar os processos da fila Fair-Share e incrementar o tempo
    int tempo = 0;
    for (int i = 0; i < fila->tamanho; i++) {
        Processo *processoExecutado = &fila->processos[i];
        printf("Executando o processo '%d' da fila Fair-Share. Tempo: %d. Duração: %d, Prioridade: %d, Contagem: %d\n", processoExecutado->id, tempo, processoExecutado->duracao, processoExecutado->prioridade, processoExecutado->contagem);
        
        int fatiaTempo = (processoExecutado->contagem * 100) / totalContagens; // Calcula a fatia de tempo proporcional
        for (int t = 0; t < fatiaTempo; t++) {
            tempo += 1;
            printf("Tempo: %d\n", tempo);
        }

        // Atualizar a contagem do processo
        processoExecutado->contagem++;

        // Remover o processo da fila Fair-Share se a duração for alcançada
        if (processoExecutado->contagem > processoExecutado->duracao) {
            for (int j = i + 1; j < fila->tamanho; j++) {
                fila->processos[j - 1] = fila->processos[j];
                fila->tempos[j - 1] = fila->tempos[j];
                fila->prioridades[j - 1] = fila->prioridades[j];
                fila->contagens[j - 1] = fila->contagens[j];
            }
            fila->tamanho--;
            i--; // Decrementar o índice após remover um processo
        }
    }
}

int main() {
    FilaFairShare filaFairShare;
    filaFairShare.tamanho = 0;
    Buffer buffer;
    buffer.tamanho = 0;

    srand(time(NULL));   // Inicializar a semente para geração de números aleatórios
    // Adicionar processos ao buffer e transferi-los para a fila Fair Share
    for (int i = 1; i <= 20; i++) {
        Processo processo = {i, rand() % 10 + 1, rand() % 5 + 1,rand() %3 + 1};

        // Adicionar processo ao buffer
        adicionarProcessoBuffer(&buffer, processo);

        // Transferir processos do buffer para a fila Fair Share quando o buffer estiver cheio ou todos os processos tiverem sido adicionados
        if (buffer.tamanho == 10 || i == 20) {
            for (int j = 0; j < buffer.tamanho; j++) {
                adicionarProcesso(&filaFairShare, buffer.processos[j]);
            }
            buffer.tamanho= 0;
        }
    }
    

    // Executar a fila Fair-Share somente quando os 20 processos forem adicionados
    if(filaFairShare.tamanho == 20)
    {
    printf("Execução da fila Fair-Share:\n");
    executarFairShare(&filaFairShare);
    }

    return 0;
}
