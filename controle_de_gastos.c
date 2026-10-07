#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GASTOS 100
#define TAM_DESCRICAO 80

struct Gasto {
    char descricao[TAM_DESCRICAO];
    int categoria;
    double valor;
};

static const char *categorias[] = {
    "Alimentacao", "Transporte", "Moradia", "Lazer", "Outros"
};
#define TOTAL_CATEGORIAS ((int)(sizeof(categorias) / sizeof(categorias[0])))

static int ler_linha(const char *prompt, char *texto, size_t tamanho) {
    printf("%s", prompt);
    if (fgets(texto, (int)tamanho, stdin) == NULL) {
        return 0;
    }

    size_t comprimento = strlen(texto);
    if (comprimento > 0 && texto[comprimento - 1] == '\n') {
        texto[comprimento - 1] = '\0';
    } else {
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
            /* Descarta o restante da linha quando ela excede o buffer. */
        }
    }
    return 1;
}

static int ler_inteiro(const char *prompt, int minimo, int maximo, int *resultado) {
    char linha[100];

    for (;;) {
        if (!ler_linha(prompt, linha, sizeof(linha))) {
            return 0;
        }

        errno = 0;
        char *fim;
        long valor = strtol(linha, &fim, 10);
        while (isspace((unsigned char)*fim)) {
            fim++;
        }

        if (linha[0] != '\0' && *fim == '\0' && errno == 0 &&
            valor >= minimo && valor <= maximo) {
            *resultado = (int)valor;
            return 1;
        }
        printf("Entrada invalida. Digite um numero entre %d e %d.\n", minimo, maximo);
    }
}

static int ler_valor(const char *prompt, double *resultado) {
    char linha[100];

    for (;;) {
        if (!ler_linha(prompt, linha, sizeof(linha))) {
            return 0;
        }

        errno = 0;
        char *fim;
        double valor = strtod(linha, &fim);
        while (isspace((unsigned char)*fim)) {
            fim++;
        }

        if (linha[0] != '\0' && *fim == '\0' && errno == 0 && valor >= 0.0) {
            *resultado = valor;
            return 1;
        }
        printf("Valor invalido. Digite um numero nao negativo.\n");
    }
}

static void listar_gastos(const struct Gasto gastos[], int quantidade) {
    if (quantidade == 0) {
        printf("\nNenhum gasto cadastrado ainda.\n");
        return;
    }

    printf("\n%-4s %-30s %-16s %12s\n", "Nº", "Descricao", "Categoria", "Valor (R$)");
    printf("---------------------------------------------------------------------\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%-4d %-30s %-16s %12.2f\n",
               i + 1, gastos[i].descricao, categorias[gastos[i].categoria], gastos[i].valor);
    }
}

static void mostrar_resumo(const struct Gasto gastos[], int quantidade, double orcamento) {
    double total = 0.0;
    double totais_categoria[TOTAL_CATEGORIAS] = {0};

    for (int i = 0; i < quantidade; i++) {
        total += gastos[i].valor;
        totais_categoria[gastos[i].categoria] += gastos[i].valor;
    }

    printf("\nResumo financeiro\n");
    printf("Orcamento:       R$ %.2f\n", orcamento);
    printf("Total gasto:     R$ %.2f\n", total);
    printf("Saldo:           R$ %.2f\n", orcamento - total);
    if (quantidade > 0) {
        printf("Media por gasto: R$ %.2f\n", total / quantidade);
    } else {
        printf("Media por gasto: R$ 0.00\n");
    }

    printf("\nGastos por categoria:\n");
    for (int i = 0; i < TOTAL_CATEGORIAS; i++) {
        printf("%-16s R$ %.2f\n", categorias[i], totais_categoria[i]);
    }

    if (total > orcamento) {
        printf("\nAtencao: o total gasto ultrapassou o orcamento.\n");
    }
}

int main(void) {
    struct Gasto gastos[MAX_GASTOS];
    int quantidade = 0;
    double orcamento;

    printf("=== Controle de gastos ===\n");
    if (!ler_valor("Informe o orcamento disponivel (R$): ", &orcamento)) {
        fprintf(stderr, "Entrada encerrada.\n");
        return 1;
    }

    for (;;) {
        int opcao;
        printf("\n1. Adicionar gasto\n");
        printf("2. Listar gastos\n");
        printf("3. Mostrar resumo\n");
        printf("0. Sair\n");

        if (!ler_inteiro("Escolha uma opcao: ", 0, 3, &opcao)) {
            printf("\nEntrada encerrada.\n");
            break;
        }

        if (opcao == 0) {
            break;
        }

        if (opcao == 1) {
            if (quantidade >= MAX_GASTOS) {
                printf("Limite de %d gastos atingido.\n", MAX_GASTOS);
                continue;
            }

            struct Gasto novo_gasto;
            if (!ler_linha("Descricao: ", novo_gasto.descricao, sizeof(novo_gasto.descricao))) {
                printf("\nEntrada encerrada.\n");
                break;
            }
            if (novo_gasto.descricao[0] == '\0') {
                strcpy(novo_gasto.descricao, "Sem descricao");
            }

            printf("Categorias:\n");
            for (int i = 0; i < TOTAL_CATEGORIAS; i++) {
                printf("%d. %s\n", i + 1, categorias[i]);
            }
            int categoria_escolhida;
            if (!ler_inteiro("Categoria: ", 1, TOTAL_CATEGORIAS, &categoria_escolhida) ||
                !ler_valor("Valor do gasto (R$): ", &novo_gasto.valor)) {
                printf("\nEntrada encerrada.\n");
                break;
            }

            novo_gasto.categoria = categoria_escolhida - 1;
            gastos[quantidade++] = novo_gasto;
            printf("Gasto registrado.\n");
        } else if (opcao == 2) {
            listar_gastos(gastos, quantidade);
        } else {
            mostrar_resumo(gastos, quantidade, orcamento);
        }
    }

    printf("\nControle de gastos encerrado.\n");
    return 0;
}
