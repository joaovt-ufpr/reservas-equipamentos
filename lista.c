/*
 * lista.c
 * Operações da lista encadeada com cabeçalho. Aqui não há printf/scanf:
 * só a manipulação da estrutura.
 */
#include <stdlib.h>
#include <string.h>
#include "lista.h"

/*
 * lista_inicializar
 * Função: deixa a lista de cabeçalhos vazia.
 * Autor: João Victor Timoteo
 */
void lista_inicializar(ListaTipos *l) {
    l->inicio = NULL;
}

/*
 * buscar_cabecalho
 * Função: percorre a lista de cabeçalhos e devolve o do tipo informado
 * (ou NULL se o tipo ainda não existe).
 * Autor: João Victor Timoteo
 */
Cabecalho *buscar_cabecalho(ListaTipos *l, const char *tipo) {
    Cabecalho *c = l->inicio;
    while (c != NULL) {
        if (strcmp(c->tipo, tipo) == 0)
            return c;
        c = c->prox;
    }
    return NULL;
}

/*
 * criar_cabecalho
 * Função: cria o cabeçalho de um novo tipo e o coloca no fim da lista.
 * Autor: João Victor Timoteo
 */
Cabecalho *criar_cabecalho(ListaTipos *l, const char *tipo) {
    Cabecalho *novo = (Cabecalho *)calloc(1, sizeof(Cabecalho));
    if (novo == NULL)
        return NULL;
    strncpy(novo->tipo, tipo, TAM_TIPO - 1);   /* calloc já zerou o resto */

    if (l->inicio == NULL) {
        l->inicio = novo;
    } else {
        Cabecalho *c = l->inicio;
        while (c->prox != NULL)
            c = c->prox;
        c->prox = novo;
    }
    return novo;
}

/*
 * inserir_equipamento
 * Função: cria um nó de equipamento e o coloca no fim da lista de
 * equipamentos do cabeçalho, atualizando o contador.
 * Autor: João Victor Timoteo
 */
void inserir_equipamento(Cabecalho *cab, int codigo, const char *descricao) {
    Equipamento *novo = (Equipamento *)calloc(1, sizeof(Equipamento));
    if (novo == NULL)
        return;
    novo->codigo = codigo;
    strncpy(novo->descricao, descricao, TAM_DESC - 1);

    if (cab->equipamentos == NULL) {
        cab->equipamentos = novo;
    } else {
        Equipamento *e = cab->equipamentos;
        while (e->prox != NULL)
            e = e->prox;
        e->prox = novo;
    }
    cab->qtdEquipamentos++;
}

/*
 * buscar_equipamento
 * Função: procura o equipamento pelo código em todos os tipos. Se achar,
 * devolve também o cabeçalho a que ele pertence (em *cabOut).
 * Autor: João Victor Timoteo
 */
Equipamento *buscar_equipamento(ListaTipos *l, int codigo, Cabecalho **cabOut) {
    Cabecalho *c = l->inicio;
    while (c != NULL) {
        Equipamento *e = c->equipamentos;
        while (e != NULL) {
            if (e->codigo == codigo) {
                if (cabOut != NULL)
                    *cabOut = c;
                return e;
            }
            e = e->prox;
        }
        c = c->prox;
    }
    return NULL;
}

/*
 * inserir_reserva
 * Função: cria uma reserva (ativa) e a coloca no fim da lista de reservas
 * (histórico) do cabeçalho, atualizando o contador.
 * Autor: João Victor Timoteo
 */
void inserir_reserva(Cabecalho *cab, int codigo, const char *nome,
                     const char *data, const char *hora) {
    Reserva *nova = (Reserva *)calloc(1, sizeof(Reserva));
    if (nova == NULL)
        return;
    nova->codigoEquip = codigo;
    strncpy(nova->solicitante, nome, TAM_NOME - 1);
    strncpy(nova->data, data, TAM_DATA - 1);
    strncpy(nova->hora, hora, TAM_HORA - 1);
    nova->ativa = 1;

    if (cab->reservas == NULL) {
        cab->reservas = nova;
    } else {
        Reserva *r = cab->reservas;
        while (r->prox != NULL)
            r = r->prox;
        r->prox = nova;
    }
    cab->qtdReservas++;
}

/*
 * buscar_reserva_ativa
 * Função: procura, no histórico do tipo, uma reserva ATIVA do equipamento na
 * data e hora informadas. Serve para detectar conflito (ao reservar) e para
 * encontrar a reserva a cancelar.
 * Autor: João Victor Timoteo
 */
Reserva *buscar_reserva_ativa(Cabecalho *cab, int codigo,
                              const char *data, const char *hora) {
    Reserva *r = cab->reservas;
    while (r != NULL) {
        if (r->ativa && r->codigoEquip == codigo &&
            strcmp(r->data, data) == 0 && strcmp(r->hora, hora) == 0)
            return r;
        r = r->prox;
    }
    return NULL;
}

/*
 * lista_liberar
 * Função: libera toda a memória alocada (reservas, equipamentos e cabeçalhos).
 * Autor: João Victor Timoteo
 */
void lista_liberar(ListaTipos *l) {
    Cabecalho *c = l->inicio;
    while (c != NULL) {
        Reserva *r = c->reservas;
        while (r != NULL) {
            Reserva *tmp = r;
            r = r->prox;
            free(tmp);
        }
        Equipamento *e = c->equipamentos;
        while (e != NULL) {
            Equipamento *tmp = e;
            e = e->prox;
            free(tmp);
        }
        Cabecalho *tmpC = c;
        c = c->prox;
        free(tmpC);
    }
    l->inicio = NULL;
}
