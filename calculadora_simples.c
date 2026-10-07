#include <stdio.h>

int main(void) {
    char continuar = 's';

    printf("Calculadora simples (+, -, *, /)\n");

    while (continuar == 's' || continuar == 'S') {
        double a, b;
        char operador;

        printf("\nDigite uma operacao (ex.: 8 * 3): ");
        if (scanf("%lf %c %lf", &a, &operador, &b) != 3) {
            fprintf(stderr, "Entrada invalida.\n");
            return 1;
        }

        switch (operador) {
            case '+': printf("Resultado: %.2f\n", a + b); break;
            case '-': printf("Resultado: %.2f\n", a - b); break;
            case '*': printf("Resultado: %.2f\n", a * b); break;
            case '/':
                if (b == 0.0) {
                    fprintf(stderr, "Nao e possivel dividir por zero.\n");
                    return 1;
                }
                printf("Resultado: %.2f\n", a / b);
                break;
            default:
                fprintf(stderr, "Operador desconhecido.\n");
                return 1;
        }

        printf("Deseja fazer outra operacao? (s/n): ");
        if (scanf(" %c", &continuar) != 1) {
            fprintf(stderr, "Nao foi possivel ler a resposta.\n");
            return 1;
        }

        if (continuar != 's' && continuar != 'S' &&
            continuar != 'n' && continuar != 'N') {
            printf("Resposta invalida. A calculadora sera encerrada.\n");
            continuar = 'n';
        }
    }

    printf("Calculadora encerrada.\n");

    return 0;
}