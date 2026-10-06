# Sistema de Reservas de Equipamentos Tecnológicos

Trabalho Prático I — Estrutura de Dados I
Tecnologia em Análise e Desenvolvimento de Sistemas — UFPR

**Autor:** João Victor Timoteo (trabalho individual)
**Professor(a):** <NOME_DO_PROFESSOR>

## Sobre o projeto

Aplicação em C (modo console) para o setor de tecnologia de uma escola controlar
reservas de notebooks, tablets, câmeras e projetores. Regra principal: **um
equipamento só pode ser reservado por um usuário em determinado dia e horário**.
O sistema guarda o **histórico de reservas por tipo de equipamento**.

## Estrutura de dados: lista encadeada com cabeçalho

```
ListaTipos ──> [Cabecalho Notebook] ──> [Cabecalho Tablet] ──> NULL
                    │ tipo, qtdEquipamentos, qtdReservas
                    ├─ equipamentos ──> Eq 101 ──> Eq 102 ──> NULL
                    └─ reservas ──────> Res ──> Res ──> NULL   (histórico)
```

- Existe **um cabeçalho para cada tipo**. Ele guarda o nome do tipo, as
  quantidades e o início das duas listas daquele tipo (equipamentos e reservas).
- **Cancelar não apaga** a reserva: ela é marcada como cancelada (`ativa = 0`),
  para o histórico ficar completo. O conflito de horário só considera reservas ativas.
- **Por que lista encadeada?** Não se sabe quantos tipos, equipamentos e reservas
  vão existir; a lista cresce conforme necessário, sem tamanho fixo.
- **Por que cabeçalho?** Cada tipo tem suas próprias listas e contadores, o que
  facilita listar reservas e montar o histórico por tipo.

## Arquivos

| Arquivo | Conteúdo |
|---|---|
| `main.c` | Menu e controle geral |
| `equipamentos.c / .h` | Funcionalidades: cadastro, reserva, consultas, cancelamento, histórico |
| `lista.c / .h` | Estruturas e operações da lista com cabeçalho |
| `Makefile` | Compilação |
| `teste_entrada.txt` | Roteiro de teste automático |
| `diario_de_bordo.pdf` | Processo de desenvolvimento |

## Como compilar e executar

Requer `gcc` (Linux/macOS) ou MinGW/MSYS2 (Windows).

```bash
make
./reservas              # no Windows: .\reservas.exe
./reservas < teste_entrada.txt   # roda um roteiro de teste pronto
```

Sem `make`: `gcc -Wall -Wextra -std=c11 -o reservas main.c lista.c equipamentos.c`

## Funcionalidades

| Menu | Função | O que faz |
|---|---|---|
| 1 | `cadastrar_equipamento` | código (único), tipo e descrição; cria o tipo se for novo |
| 2 | `reservar_equipamento` | código, solicitante, data (DD/MM/AAAA) e hora (HH:MM); impede conflito |
| 3 | `listar_reservas_por_tipo` | mostra as reservas ativas de um tipo |
| 4 | `consultar_reservas_por_data` | mostra as reservas ativas de um dia |
| 5 | `cancelar_reserva` | cancela pela combinação equipamento + data + hora |
| 6 | `exibir_historico_por_tipo` | para cada cabeçalho: equipamentos e todo o histórico |

O tipo deve ser digitado sempre igual (ex.: `Notebook`), pois a comparação diferencia maiúsculas de minúsculas.

## Autoria

Todo o código e a documentação foram desenvolvidos por **João Victor Timoteo**. Cada função tem, no topo, um comentário com sua descrição e o autor.

## Limitações

- Os dados ficam só na memória (não são salvos em arquivo).
- A data é conferida apenas quanto ao formato (não verifica dias de cada mês).
