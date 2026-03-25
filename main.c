// Bibliotecas
#include <stdio.h>
#include <stdlib.h>

// Definições iniciais
typedef struct Tarefa {
    char* descricao;
    struct Tarefa* prox;
} Tarefa;

Tarefa* head = NULL;
Tarefa* tail = NULL; // Se for para inserir na última posição, então que vá direto

int tamanho = 0; // Útil para verificar se a posição informada é a última

// Função de buscar trecho (Adicionar em outra biblioteca)
int buscar_trecho(char* trecho, char* todo) {
    // trecho -> O trecho buscado
    // todo -> A string em que o trecho será procurado
    // 1 para contém, 0 para não contém
    int j = 0;
    for(int i = 0; todo[i] != '\0' || todo[i] != '\n'; i++) {
        if(trecho[j] == '\0' || trecho[j] == '\n') {
            return 0; // A comparação chegou ao final do trecho, então tem esse trecho
        } else {
            if(todo[i] == trecho[j]) {
                j++;
            } else {
                j = 0;
            }
        }
    }
    if(trecho[j] == '\0' || trecho[j] == '\n') {
        return 0;
    }
    return 1;
}

// Função de inserir
int inserir_tarefa(char* descricao, int pos) {
    // descricao -> String de qualquer tamanho
    // pos -> Posição específica
    // 0 para sucesso, 1 para posição inválida

    // Tarefa nova
    Tarefa* nova;
    (*nova).descricao = descricao;

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

// Função de buscar
int buscar_tarefa(char* trecho, Tarefa** lista_ret, Tarefa** listant_ret, int* indices, int sem_listant) {
    // trecho -> Um trecho a ser procurado
    // lista_ret -> Ponteiro para armazenar os resultados
    // listant_ret -> Ponteiro para armazenar o item anterior de cada, necessário para remover_tarefa()
    // indices -> Ponteiro para armazenar o indíce de cada
    // sem_listant -> Não armazenar no listant_ret se for diferente de 0
    // Retorna a quantidade de resultados

    // Armazenando
    Tarefa* item_ant = head;
    
    // Outras variáveis
    int tam_lista = 0; // Alocado
    int tam_return = 0; // Lá dentro

    // Buscando
    if(sem_listant) {
        // Se tiver o termo no primeiro item
        if(buscar_trecho(trecho, (*head).descricao)) {
            lista_ret = (Tarefa**)malloc(2*sizeof(Tarefa*));
            indices = (int*)malloc(2*sizeof(int));
            tam_lista = 2;

            lista_ret[0] = head;
            indices[0] = 0;
            tam_return = 1;
        }
        // O restante
        for(int i = 1; i < tamanho; i++) {
            if(buscar_trecho(trecho, (*((*item_ant).prox)).descricao)) {
                if(tam_lista == tam_return) {
                    lista_ret = (Tarefa**)realloc(lista_ret, (tam_lista*2)*sizeof(Tarefa*));
                    indices = (int*)realloc(indices, (tam_lista*2)*sizeof(int));
                    tam_lista = (tam_lista*2);
                }

                lista_ret[tam_return] = (*item_ant).prox;
                indices[tam_return] = i;
                tam_return++;
            }
        }
    } else {
        // Se tiver o termo no primeiro item
        if(buscar_trecho(trecho, (*head).descricao)) {
            lista_ret = (Tarefa**)malloc(2*sizeof(Tarefa*));
            listant_ret = (Tarefa**)malloc(2*sizeof(Tarefa*));
            indices = (int*)malloc(2*sizeof(int));
            tam_lista = 2;

            lista_ret[0] = head;
            listant_ret[0] = NULL;
            indices[0] = 0;
            tam_return = 1;
        }
        // O restante
        for(int i = 1; i < tamanho; i++) {
            if(buscar_trecho(trecho, (*((*item_ant).prox)).descricao)) {
                if(tam_lista == tam_return) {
                    lista_ret = (Tarefa**)realloc(lista_ret, (tam_lista*2)*sizeof(Tarefa*));
                    listant_ret = (Tarefa**)realloc(listant_ret, (tam_lista*2)*sizeof(Tarefa*));
                    indices = (int*)realloc(indices, (tam_lista*2)*sizeof(int));
                    tam_lista = (tam_lista*2);
                }

                lista_ret[tam_return] = (*item_ant).prox;
                listant_ret[tam_return] = item_ant;
                indices[tam_return] = i;
                tam_return++;
            }
        }
    }

    // Retornando a quantidade encontrada
    return tam_return;
}

// Principal
int main() {
    int entr;

    while(1) {
        system("clear");

        printf("1- Listar\n2- Buscar\n3- Adicionar\n> ");
        scanf("%d", &entr);

        switch(entr) {
            case 1:
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
                printf("Buscar em breve\n");
                break;
            case 3:
                int p;
                char s[100];
                printf("Posição: "); scanf("%d", &p);
                printf("Descrição: "); scanf("%s", s);

                int resul = inserir_tarefa(s, p);

                if(resul) {
                    printf("Posição inválida\n");
                    break;
                }
                printf("Inserido com sucesso\n");
                break;
            default:
                printf("Comando desconhecido\n");
                break;
        }

        printf("\n---\n\nQualquer tecla para continuar...");
        getchar();
        getchar();
    }
}