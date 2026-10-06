/*
 * main.c
 * Menu e controle geral do Sistema de Reservas de Equipamentos Tecnologicos.
 * Autor: João Victor Timoteo
 */
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "equipamentos.h"

int main(void) {
    ListaTipos lista;
    lista_inicializar(&lista);

    int opcao = -1;
    char buf[16];
    while (opcao != 0) {
        printf("\n===== RESERVA DE EQUIPAMENTOS =====\n");
        printf("1 - Cadastrar equipamento\n");
        printf("2 - Reservar equipamento\n");
        printf("3 - Listar reservas por tipo\n");
        printf("4 - Consultar reservas por data\n");
        printf("5 - Cancelar reserva\n");
        printf("6 - Exibir historico por tipo\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (fgets(buf, sizeof buf, stdin) == NULL)
            break;
        opcao = atoi(buf);

        switch (opcao) {
            case 1: cadastrar_equipamento(&lista);       break;
            case 2: reservar_equipamento(&lista);        break;
            case 3: listar_reservas_por_tipo(&lista);    break;
            case 4: consultar_reservas_por_data(&lista); break;
            case 5: cancelar_reserva(&lista);            break;
            case 6: exibir_historico_por_tipo(&lista);   break;
            case 0: printf("Encerrando...\n");           break;
            default: printf("Opcao invalida.\n");
        }
    }

    lista_liberar(&lista);
    return 0;
}
