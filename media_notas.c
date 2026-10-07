#include <stdio.h>

#define QUANTIDADE_NOTAS 3

int main(void) {
    char nome[80];
    double nota, soma = 0.0;

    printf("Nome do aluno: ");
    if (scanf("%79s", nome) != 1) {
        fprintf(stderr, "Nao foi possivel ler o nome.\n");
        return 1;
    }

    for (int i = 0; i < QUANTIDADE_NOTAS; i++) {
        printf("Digite a nota %d (0 a 10): ", i + 1);
        if (scanf("%lf", &nota) != 1 || nota < 0.0 || nota > 10.0) {
            fprintf(stderr, "Nota invalida. Informe um valor de 0 a 10.\n");
            return 1;
        }
        soma += nota;
    }

    double media = soma / QUANTIDADE_NOTAS;
    printf("Media de %s: %.2f\n", nome, media);
    if (media >= 7.0) {
        printf("Situacao: aprovado\n");
    } else if (media >= 5.0) {
        printf("Situacao: recuperacao\n");
    } else {
        printf("Situacao: reprovado\n");
    }

    return 0;
}