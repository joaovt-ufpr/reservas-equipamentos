/*
 * lista.h
 * Estruturas da LISTA COM CABEÇALHO (um cabeçalho por tipo de equipamento).
 *
 *  ListaTipos ──> [Cabecalho Notebook] ──> [Cabecalho Tablet] ──> NULL
 *                      │
 *                      ├─ equipamentos ──> Eq ──> Eq ──> NULL
 *                      └─ reservas ──────> Res ──> Res ──> NULL  (histórico)
 */
#ifndef LISTA_H
#define LISTA_H

#define TAM_TIPO 30
#define TAM_DESC 80
#define TAM_NOME 50
#define TAM_DATA 11   /* DD/MM/AAAA */
#define TAM_HORA 6    /* HH:MM */

typedef struct Equipamento {
    int codigo;
    char descricao[TAM_DESC];
    struct Equipamento *prox;
} Equipamento;

typedef struct Reserva {
    int codigoEquip;
    char solicitante[TAM_NOME];
    char data[TAM_DATA];
    char hora[TAM_HORA];
    int ativa;                  /* 1 = ativa, 0 = cancelada (continua no histórico) */
    struct Reserva *prox;
} Reserva;

/* Cabeçalho: guarda o tipo, contadores e o início das duas listas do tipo */
typedef struct Cabecalho {
    char tipo[TAM_TIPO];
    int qtdEquipamentos;
    int qtdReservas;            /* total no histórico (ativas + canceladas) */
    Equipamento *equipamentos;
    Reserva *reservas;
    struct Cabecalho *prox;
} Cabecalho;

typedef struct {
    Cabecalho *inicio;
} ListaTipos;

void lista_inicializar(ListaTipos *l);
void lista_liberar(ListaTipos *l);

Cabecalho *buscar_cabecalho(ListaTipos *l, const char *tipo);
Cabecalho *criar_cabecalho(ListaTipos *l, const char *tipo);

void inserir_equipamento(Cabecalho *cab, int codigo, const char *descricao);
Equipamento *buscar_equipamento(ListaTipos *l, int codigo, Cabecalho **cabOut);

void inserir_reserva(Cabecalho *cab, int codigo, const char *nome,
                     const char *data, const char *hora);
Reserva *buscar_reserva_ativa(Cabecalho *cab, int codigo,
                              const char *data, const char *hora);

#endif
