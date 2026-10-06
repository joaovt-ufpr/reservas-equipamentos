/*
 * equipamentos.h
 * Funcionalidades do sistema (leitura de dados, regras e exibição).
 */
#ifndef EQUIPAMENTOS_H
#define EQUIPAMENTOS_H

#include "lista.h"

void cadastrar_equipamento(ListaTipos *l);
void reservar_equipamento(ListaTipos *l);
void listar_reservas_por_tipo(ListaTipos *l);
void consultar_reservas_por_data(ListaTipos *l);
void cancelar_reserva(ListaTipos *l);
void exibir_historico_por_tipo(ListaTipos *l);

#endif
