#include <stdio.h>

int main(void) {
    double temperatura;
    char escala;

    printf("Conversor de temperatura (C para F ou F para C)\n");
    printf("Digite a temperatura e a escala (ex.: 25 C): ");
    if (scanf("%lf %c", &temperatura, &escala) != 2) {
        fprintf(stderr, "Entrada invalida.\n");
        return 1;
    }

    if (escala == 'C' || escala == 'c') {
        printf("%.2f C = %.2f F\n", temperatura, temperatura * 9.0 / 5.0 + 32.0);
    } else if (escala == 'F' || escala == 'f') {
        printf("%.2f F = %.2f C\n", temperatura, (temperatura - 32.0) * 5.0 / 9.0);
    } else {
        fprintf(stderr, "Escala desconhecida. Use C ou F.\n");
        return 1;
    }

    return 0;
}