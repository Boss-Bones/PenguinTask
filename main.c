// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições iniciais
typedef struct Tarefa {
    char descricao[100];
    struct Tarefa* prox;
    int prioridade;
    int estado; // 0 para pendente e 1 para concluído
} Tarefa;

Tarefa* head = NULL;
Tarefa* tail = NULL; // Se for para inserir na última posição, então que vá direto

int tamanho = 0; // Útil para verificar se a posição informada é a última

// Remover \n
void remover_barran(char* texto) {
    int i;
    for(i = 0; texto[i] != '\n'; i++);
    texto[i] = '\0';
}

// Função de inserir
int inserir_tarefa(char* descricao, int prioridade, int pos) {
    // descricao -> String de qualquer tamanho
    // pos -> Posição específica
    // 0 para sucesso, 1 para posição inválida

    // Tarefa nova
    Tarefa* nova = (Tarefa*)malloc(sizeof(Tarefa));
    (*nova).prioridade = prioridade;
    (*nova).estado = 0;
    strcpy((*nova).descricao, descricao);

    // Adicionar em posição...
    if(pos > tamanho || pos < 0) { // Posição inválida
        return 1;
    } else {
        if(pos == tamanho) { // Última posição
            (*nova).prox = NULL; // Já que é na última posição
            if(tamanho == 0) { // Se não tem nenhum, esse item será primeiro e último
                head = nova;
                tail = nova;
            } else { // Senão, operação normal de adicionar na última posição
                (*tail).prox = nova;
                tail = nova;
            }
            tamanho++;
            return 0;
        } else { // Qualquer outra posição
            if(pos == 0) {
                (*nova).prox = head;
                head = nova;
                tamanho++;
                return 0; // Finalizado
            }

            Tarefa* item_ant = head;
            for(int i = 1; i < pos; i++) {
                item_ant = (*head).prox;
            }
            (*nova).prox = (*item_ant).prox;
            (*item_ant).prox = nova;
            tamanho++;
            return 0;
        }
    }
}

// Função de remover
int remover_tarefa(int pos) {
    // pos -> Posição específica
    // 0 para sucesso e 1 para não encontrado

    if(pos < 0 || pos >= tamanho) { // Posição inválida
        return 1;
    }

    // Diminuindo tamanho
    tamanho--;

    // Posição 0
    if(pos == 0) {
        Tarefa* remover = head;
        head = (*remover).prox;
        free(remover);

        if(tamanho == 0) { // Remover o tail se o removido for o único
            tail = NULL;
        }

        return 0;
    }

    // Posição qualquer
    Tarefa* remover_prox = head;
    Tarefa* conectar;
    for(int i = 1; i < pos; i++) {
        remover_prox = (*remover_prox).prox;
    }
    conectar = (*((*remover_prox).prox)).prox;
    free((*remover_prox).prox);
    (*remover_prox).prox = conectar;
    if(pos == tamanho) { // Se o último foi removido, atualizar tail
        tail = remover_prox;
    }
    return 0;
}

// Função de concluir
void marcar_como_concluido(int pos) {
    Tarefa* alvo = head;
    for(int i = 0; i < pos; i++) {
        alvo = (*alvo).prox;
    }
    (*alvo).estado = 1;
}

// Função de buscar
int buscar_tarefa(char* trecho) {
    // trecho -> Um trecho a ser procurado
    // Retorna a quantidade de resultados

    int tam_return = 0;

    Tarefa* imprimir = head;
    for(int i = 0; i < tamanho; i++) {
        if(strstr((*imprimir).descricao, trecho)) {
            if((*imprimir).estado) {
                printf("Concluido - %d - [%d] %s\n", (*imprimir).prioridade, i, (*imprimir).descricao);
            } else {
                printf("Pendente - %d - [%d] %s\n", (*imprimir).prioridade, i, (*imprimir).descricao);
            }
            tam_return++;
        }
        imprimir = (*imprimir).prox;
    }

    if(tam_return == 0) {
        printf("Vazio\n");
    }

    // Retornando a quantidade encontrada
    return tam_return;
}

void remover_buscar_tarefa(char* trecho) {
    // trecho -> Um trecho a ser procurado
    // Retorna a quantidade de resultados

    Tarefa* imprimir = head;
    for(int i = 0; i < tamanho; i++) {
        if(strstr((*imprimir).descricao, trecho)) {
            remover_tarefa(i);
            printf("Removido com sucesso!\n");
            return;
        }
        imprimir = (*imprimir).prox;
    }
    printf("Não encontrado");
}

// Principal
int main() {
    int entr, jump = 0;

    while(1) {
        system("clear");

        printf("1- Listar\n2- Buscar\n3- Adicionar\n4- Remover\n5- Marcar como concluído\n6- Sair\n> ");
        scanf("%d", &entr);

        switch(entr) {
            case 1:
                Tarefa* imprimir = head;
                if(tamanho == 0) {
                    printf("Vazio\n");
                    break;
                }
                for(int i = 0; i < tamanho; i++) {
                    if((*imprimir).estado) {
                        printf("Concluido - %d - [%d] %s\n", (*imprimir).prioridade, i, (*imprimir).descricao);
                    } else {
                        printf("Pendente - %d - [%d] %s\n", (*imprimir).prioridade, i, (*imprimir).descricao);
                    }
                    imprimir = (*imprimir).prox;
                }
                break;
            case 2:
                char st[100];
                printf("Pesquisar: "); scanf("%s", st);

                buscar_tarefa(st);
                break;
            case 3:
                int p;
                int prio;
                char entr_pos;
                char s[100];

                printf("(i) Inicio, (f) fim ou (e) específico: "); getchar(); scanf("%c", &entr_pos);
                if(entr_pos == 'i') {
                    p = 0;
                } else {
                    if(entr_pos == 'f') {
                        p = tamanho;
                    } else {
                        printf("Posição: "); scanf("%d", &p);
                    }
                }
                printf("Prioridade: "); scanf("%d", &prio);
                printf("Descrição: "); getchar(); fgets(s, 100, stdin); remover_barran(s);

                int resul = inserir_tarefa(s, prio, p);

                if(resul) {
                    printf("Posição inválida\n");
                    break;
                }
                printf("Inserido com sucesso\n"); 
                jump++; // fgets não deixa \n no buffer
                break;
            case 4:
                char entr_qual;
                printf("Remover por (p) posição ou (c) palavra-chave? "); getchar(); scanf("%c", &entr_qual);
                if(entr_qual == 'p') {
                    int p2;
                    printf("Posição: "); scanf("%d", &p2);

                    int resul2 = remover_tarefa(p);

                    if(resul2) {
                        printf("Posição inválida\n");
                        break;
                    }
                    printf("Removido com sucesso\n");
                    break;
                } else {
                    char st2[100];
                    printf("Pesquisar: "); scanf("%s", st);

                    remover_buscar_tarefa(st);
                    break;
                }
            case 5:
                int p3;
                printf("Posição: "); scanf("%d", &p3);

                marcar_como_concluido(p3);

                printf("Marcado com sucesso\n");
                break;
            case 6:
                Tarefa* liberar = head;
                Tarefa* prox = NULL;

                for(int i = 0; i < tamanho; i++) {
                    if((*liberar).prox != NULL) {
                        prox = (*liberar).prox;   
                    }

                    printf("Liberando o %d\n", i);
                    free(liberar);

                    liberar = prox;
                }
                return 0;
            default:
                printf("Comando desconhecido\n");
                break;
        }

        printf("\n---\n\nQualquer tecla para continuar...");
        if(jump) { // Não pega o \n se não tiver
            jump--;
        } else {
            getchar();
        }
        getchar();
    }
}