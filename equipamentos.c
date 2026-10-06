/*
 * equipamentos.c
 * Cadastro, reservas, consultas, cancelamento e histórico.
 * (Mensagens sem acento de propósito, para não dar problema no terminal do Windows.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipamentos.h"

/*
 * ler_texto
 * Função: mostra uma mensagem e lê uma linha inteira (aceita espaços),
 * tirando o '\n' do final.
 * Autor: João Victor Timoteo
 */
static void ler_texto(const char *msg, char *dest, int tam) {
    printf("%s", msg);
    if (fgets(dest, tam, stdin) == NULL)
        dest[0] = '\0';
    dest[strcspn(dest, "\n")] = '\0';
}

/*
 * ler_inteiro
 * Função: lê um número inteiro (devolve 0 se o texto não for número).
 * Autor: João Victor Timoteo
 */
static int ler_inteiro(const char *msg) {
    char buf[20];
    ler_texto(msg, buf, sizeof buf);
    return atoi(buf);
}

/*
 * data_valida
 * Função: confere se o texto está no formato DD/MM/AAAA.
 * Autor: João Victor Timoteo
 */
static int data_valida(const char *s) {
    int d, m, a;
    return strlen(s) == 10 && sscanf(s, "%d/%d/%d", &d, &m, &a) == 3 &&
           d >= 1 && d <= 31 && m >= 1 && m <= 12 && a >= 2000;
}

/*
 * hora_valida
 * Função: confere se o texto está no formato HH:MM.
 * Autor: João Victor Timoteo
 */
static int hora_valida(const char *s) {
    int h, m;
    return strlen(s) == 5 && sscanf(s, "%d:%d", &h, &m) == 2 &&
           h >= 0 && h <= 23 && m >= 0 && m <= 59;
}

static void mostrar_reserva(const Reserva *r) {
    printf("  equip %d | %s | %s %s | %s\n", r->codigoEquip, r->solicitante,
           r->data, r->hora, r->ativa ? "ATIVA" : "CANCELADA");
}

/*
 * cadastrar_equipamento
 * Função: lê código, tipo e descrição; não deixa repetir código; cria o
 * cabeçalho do tipo se ele ainda não existir; insere o equipamento na lista
 * desse tipo.
 * Autor: João Victor Timoteo
 */
void cadastrar_equipamento(ListaTipos *l) {
    int codigo = ler_inteiro("Codigo: ");
    if (codigo <= 0) {
        printf("Codigo invalido.\n");
        return;
    }
    if (buscar_equipamento(l, codigo, NULL) != NULL) {
        printf("Ja existe equipamento com esse codigo.\n");
        return;
    }

    char tipo[TAM_TIPO], desc[TAM_DESC];
    ler_texto("Tipo (ex: Notebook): ", tipo, sizeof tipo);
    ler_texto("Descricao: ", desc, sizeof desc);
    if (tipo[0] == '\0' || desc[0] == '\0') {
        printf("Tipo e descricao nao podem ser vazios.\n");
        return;
    }

    Cabecalho *cab = buscar_cabecalho(l, tipo);
    if (cab == NULL)
        cab = criar_cabecalho(l, tipo);
    inserir_equipamento(cab, codigo, desc);
    printf("Equipamento cadastrado no tipo %s.\n", cab->tipo);
}

/*
 * reservar_equipamento
 * Função: lê os dados da reserva, confere se o equipamento existe e se NÃO há
 * reserva ativa dele na mesma data e hora (um equipamento só pode ter um
 * usuário por horário); se estiver livre, registra a reserva.
 * Autor: João Victor Timoteo
 */
void reservar_equipamento(ListaTipos *l) {
    int codigo = ler_inteiro("Codigo do equipamento: ");
    Cabecalho *cab = NULL;
    if (buscar_equipamento(l, codigo, &cab) == NULL) {
        printf("Equipamento nao encontrado.\n");
        return;
    }

    char nome[TAM_NOME], data[TAM_DATA + 10], hora[TAM_HORA + 10];
    ler_texto("Nome do solicitante: ", nome, sizeof nome);
    ler_texto("Data (DD/MM/AAAA): ", data, sizeof data);
    ler_texto("Hora (HH:MM): ", hora, sizeof hora);
    if (nome[0] == '\0' || !data_valida(data) || !hora_valida(hora)) {
        printf("Dados invalidos (confira nome, data e hora).\n");
        return;
    }

    if (buscar_reserva_ativa(cab, codigo, data, hora) != NULL) {
        printf("Conflito: equipamento ja reservado nesse horario.\n");
        return;
    }
    inserir_reserva(cab, codigo, nome, data, hora);
    printf("Reserva realizada.\n");
}

/*
 * listar_reservas_por_tipo
 * Função: pede um tipo, acha o cabeçalho dele e percorre a lista de reservas
 * mostrando as ativas.
 * Autor: João Victor Timoteo
 */
void listar_reservas_por_tipo(ListaTipos *l) {
    char tipo[TAM_TIPO];
    ler_texto("Tipo: ", tipo, sizeof tipo);
    Cabecalho *cab = buscar_cabecalho(l, tipo);
    if (cab == NULL) {
        printf("Tipo nao encontrado.\n");
        return;
    }
    int achou = 0;
    printf("Reservas ativas de %s:\n", cab->tipo);
    for (Reserva *r = cab->reservas; r != NULL; r = r->prox) {
        if (r->ativa) {
            mostrar_reserva(r);
            achou = 1;
        }
    }
    if (!achou)
        printf("  (nenhuma)\n");
}

/*
 * consultar_reservas_por_data
 * Função: pede uma data e percorre TODOS os cabeçalhos mostrando as reservas
 * ativas daquele dia.
 * Autor: João Victor Timoteo
 */
void consultar_reservas_por_data(ListaTipos *l) {
    char data[TAM_DATA + 10];
    ler_texto("Data (DD/MM/AAAA): ", data, sizeof data);
    if (!data_valida(data)) {
        printf("Data invalida.\n");
        return;
    }
    int achou = 0;
    for (Cabecalho *c = l->inicio; c != NULL; c = c->prox) {
        for (Reserva *r = c->reservas; r != NULL; r = r->prox) {
            if (r->ativa && strcmp(r->data, data) == 0) {
                printf("[%s]", c->tipo);
                mostrar_reserva(r);
                achou = 1;
            }
        }
    }
    if (!achou)
        printf("Nenhuma reserva nessa data.\n");
}

/*
 * cancelar_reserva
 * Função: cancela uma reserva específica (identificada por equipamento, data
 * e hora). O nó NÃO é removido: apenas marcado como cancelado (ativa = 0),
 * para o histórico por tipo continuar completo.
 * Autor: João Victor Timoteo
 */
void cancelar_reserva(ListaTipos *l) {
    int codigo = ler_inteiro("Codigo do equipamento: ");
    Cabecalho *cab = NULL;
    if (buscar_equipamento(l, codigo, &cab) == NULL) {
        printf("Equipamento nao encontrado.\n");
        return;
    }
    char data[TAM_DATA + 10], hora[TAM_HORA + 10];
    ler_texto("Data (DD/MM/AAAA): ", data, sizeof data);
    ler_texto("Hora (HH:MM): ", hora, sizeof hora);

    Reserva *r = buscar_reserva_ativa(cab, codigo, data, hora);
    if (r == NULL) {
        printf("Nao existe reserva ativa com esses dados.\n");
        return;
    }
    r->ativa = 0;
    printf("Reserva cancelada.\n");
}

/*
 * exibir_historico_por_tipo
 * Função: percorre a lista de cabeçalhos e, para cada tipo, mostra as
 * informações do cabeçalho (nome, quantidades), seus equipamentos e todo o
 * histórico de reservas (ativas e canceladas).
 * Autor: João Victor Timoteo
 */
void exibir_historico_por_tipo(ListaTipos *l) {
    if (l->inicio == NULL) {
        printf("Nenhum tipo cadastrado.\n");
        return;
    }
    for (Cabecalho *c = l->inicio; c != NULL; c = c->prox) {
        printf("\n=== %s === (%d equipamentos, %d reservas no historico)\n",
               c->tipo, c->qtdEquipamentos, c->qtdReservas);
        printf(" Equipamentos:\n");
        for (Equipamento *e = c->equipamentos; e != NULL; e = e->prox)
            printf("  %d - %s\n", e->codigo, e->descricao);
        printf(" Historico:\n");
        if (c->reservas == NULL)
            printf("  (vazio)\n");
        for (Reserva *r = c->reservas; r != NULL; r = r->prox)
            mostrar_reserva(r);
    }
}
