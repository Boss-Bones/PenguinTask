// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições iniciais
typedef struct Tarefa {
    char descricao[1000];
    struct Tarefa* prox;
} Tarefa;

Tarefa* head = NULL;
Tarefa* tail = NULL; // Se for para inserir na última posição, então que vá direto

int tamanho = 0; // Útil para verificar se a posição informada é a última

// Função de buscar trecho (Adicionar em outra biblioteca)
int buscar_trecho(char* trecho, char* todo) {
    // trecho -> O trecho buscado
    // todo -> A string em que o trecho será procurado
    // Usar strstr é mais robusto, seguro e eficiente.
    if (strstr(todo, trecho) != NULL) {
        return 1; // Encontrou o trecho
    }
    return 0; // Não encontrou
}

// Função de inserir
int inserir_tarefa(char* descricao, int pos) {
    // descricao -> String de qualquer tamanho
    // pos -> Posição específica
    // 0 para sucesso, 1 para posição inválida

    // Tarefa nova
    Tarefa* nova = (Tarefa*)malloc(sizeof(Tarefa));
    if (nova == NULL) {
        // Falha na alocação de memória
        return 1;
    }
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
                item_ant = item_ant->prox;
            }
            (*nova).prox = (*item_ant).prox;
            (*item_ant).prox = nova;
            tamanho++;
            return 0;
        }
    }
}

// Função de buscar
int buscar_tarefa(char* trecho, Tarefa*** lista_ret_ptr, Tarefa*** listant_ret_ptr, int** indices_ptr, int sem_listant) {
    // trecho -> Um trecho a ser procurado
    // lista_ret_ptr -> Ponteiro para o ponteiro que armazenará os resultados
    // listant_ret_ptr -> Ponteiro para o ponteiro que armazenará o item anterior de cada, necessário para remover_tarefa()
    // indices_ptr -> Ponteiro para o ponteiro que armazenará o indíce de cada
    // sem_listant -> Não armazenar no listant_ret se for diferente de 0
    // Retorna a quantidade de resultados
    
    // Inicializa ponteiros de saída para NULL
    *lista_ret_ptr = NULL;
    if (indices_ptr != NULL) *indices_ptr = NULL;
    if (listant_ret_ptr != NULL) *listant_ret_ptr = NULL;

    if (head == NULL) {
        return 0; // Lista vazia, nenhum resultado.
    }

    Tarefa** lista_ret = NULL;
    Tarefa** listant_ret = NULL;
    int* indices = NULL;
    int tam_alocado = 0;
    int quant_encontrados = 0;

    Tarefa* atual = head;
    Tarefa* anterior = NULL;
    int indice_atual = 0;

    // Percorrendo a lista
    while (atual != NULL) {
        if (buscar_trecho(trecho, atual->descricao)) {
            // Se a capacidade atual for insuficiente, realocar (dobrando o tamanho)
            if (quant_encontrados == tam_alocado) {
                tam_alocado = (tam_alocado == 0) ? 2 : tam_alocado * 2;

                // Realoca o array de resultados
                Tarefa** temp_lista = realloc(lista_ret, tam_alocado * sizeof(Tarefa*));
                if (temp_lista == NULL) { /* Erro de alocação */ free(lista_ret); free(listant_ret); free(indices); return -1; }
                lista_ret = temp_lista;

                // Realoca o array de índices
                int* temp_indices = realloc(indices, tam_alocado * sizeof(int));
                if (temp_indices == NULL) { /* Erro de alocação */ free(lista_ret); free(listant_ret); free(indices); return -1; }
                indices = temp_indices;

                // Realoca o array de nós anteriores (se necessário)
                if (!sem_listant) {
                    Tarefa** temp_listant = realloc(listant_ret, tam_alocado * sizeof(Tarefa*));
                    if (temp_listant == NULL) { /* Erro de alocação */ free(lista_ret); free(listant_ret); free(indices); return -1; }
                    listant_ret = temp_listant;
                }
            }

            lista_ret[quant_encontrados] = atual;
            indices[quant_encontrados] = indice_atual;
            if (!sem_listant) {
                listant_ret[quant_encontrados] = anterior;
            }
            quant_encontrados++;
        }
        anterior = atual;
        atual = atual->prox;
        indice_atual++;
    }

    // Atribui os ponteiros alocados aos ponteiros de saída passados por referência
    *lista_ret_ptr = lista_ret;
    if (indices_ptr != NULL) *indices_ptr = indices;
    if (!sem_listant && listant_ret_ptr != NULL) {
        *listant_ret_ptr = listant_ret;
    }

    return quant_encontrados;
}

int remover_tarefa(int pos) {
    // 0 = sucesso, 1 = erro

    if(pos < 0 || pos >= tamanho) {
        return 1; // posição inválida
    }

    Tarefa* removido;

    // remover o primeiro
    if(pos == 0) {
        removido = head;
        head = head->prox;

        // se só tinha um
        if(tamanho == 1) {
            tail = NULL;
        }

        free(removido);
        tamanho--;
        return 0;
    }

    // achar o anterior
    Tarefa* anterior = head;
    for(int i = 1; i < pos; i++) {
        anterior = anterior->prox;
    }

    removido = anterior->prox;
    anterior->prox = removido->prox;

    // se removeu o último
    if(removido == tail) {
        tail = anterior;
    }

    free(removido);
    tamanho--;

    return 0;
}

// Principal
int main() {
    int entr;

    while(1) {
        system("clear");

        printf("1- Listar\n2- Buscar\n3- Adicionar\n4- Remover\n> ");
        scanf("%d", &entr);

        switch(entr) {
            case 1:
                // Listar

                Tarefa* imprimir = head;
                if(tamanho == 0) {
                    printf("Vazio\n");
                    break;
                }
                for(int i = 0; i < tamanho; i++) {
                    printf("[%d] %s\n", i, (*imprimir).descricao);
                    imprimir = (*imprimir).prox;
                }
                break;
            case 2:
                // Buscar
                
                char pes[100];
                Tarefa** results = NULL; // IMPORTANTE: Inicializar com NULL
                int* is = NULL;          // IMPORTANTE: Inicializar com NULL
                printf("Pesquisar: ");
                while (getchar() != '\n'); // Limpa o buffer do teclado
                fgets(pes, sizeof(pes), stdin);
                pes[strcspn(pes, "\n")] = 0; // Remove o '\n' que o fgets adiciona

                // Passa o ENDEREÇO dos ponteiros para que a função possa modificá-los
                int quant = buscar_tarefa(pes, &results, NULL, &is, 1);

                if (quant == -1) {
                    printf("Erro de memória durante a busca.\n");
                } else if (quant == 0) {
                    printf("Nenhum resultado encontrado para \"%s\".\n", pes);
                } else {
                    printf("Resultados encontrados:\n");
                    for(int i = 0; i < quant; i++) {
                        // Agora 'results' e 'is' apontam para a memória alocada
                        printf("[%d] %s\n", is[i], results[i]->descricao);
                    }
                }

                // Liberar a memória que foi alocada DENTRO de buscar_tarefa
                free(results);
                free(is);

                break;
            case 3:
                int p;
                char tp[50];
                char s[1000];
                printf("Tipo de posição (I-início/F-fim/E-específica): ");
                while (getchar() != '\n'); // Limpa o buffer do teclado
                scanf("%s", &tp);
                switch(tp[0]) {
                    case 'i':
                    case 'I':
                        p = 0;
                        break;
                    case 'f':
                    case 'F':
                        p = tamanho;
                        break;
                    case 'e':
                    case 'E':
                        printf("Posição: "); scanf("%d", &p);
                        break;
                    default:
                        printf("Opção inválida. Assumindo fim da lista.\n");
                        p = tamanho;
                        break;
                }
                printf("Descrição: ");
                while (getchar() != '\n'); // Limpa o buffer do teclado
                fgets(s, sizeof(s), stdin);
                s[strcspn(s, "\n")] = 0; // Remove o '\n' que o fgets adiciona

                int resul = inserir_tarefa(s, p);

                if(resul) {
                    printf("Posição inválida\n");
                    break;
                }
                printf("Inserido com sucesso\n");
                break;
            case 4:
                // Remover
                int pos_remover;
                printf("Posição da tarefa a remover: ");
                // Limpa o buffer antes de ler o inteiro
                while (getchar() != '\n');
                scanf("%d", &pos_remover);

                int remove_res = remover_tarefa(pos_remover);
                if (remove_res == 0) {
                    printf("Tarefa removida com sucesso.\n");
                } else {
                    printf("Erro: Posição inválida ou tarefa não encontrada.\n");
                }
                break;
            default:
                printf("Comando desconhecido\n");
                break;
        }

        printf("\n---\n\nQualquer tecla para continuar...");
        while (getchar() != '\n'); // Limpa o buffer do teclado
        getchar(); // Espera o usuário pressionar Enter
    }
}