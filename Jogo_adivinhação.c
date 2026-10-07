#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    const int max_tentativas = 7;
    int segredo, palpite;

    srand((unsigned)time(NULL));
    segredo = rand() % 100 + 1;

    printf("Jogo de adivinhacao: descubra o numero de 1 a 100.\n");

    for (int tentativa = 1; tentativa <= max_tentativas; tentativa++) {
        printf("Tentativa %d/%d: ", tentativa, max_tentativas);
        if (scanf("%d", &palpite) != 1) {
            fprintf(stderr, "Digite um numero inteiro.\n");
            return 1;
        }
        if (palpite < 1 || palpite > 100) {
            printf("Escolha um numero entre 1 e 100.\n");
            tentativa--;
            continue;
        }
        if (palpite == segredo) {
            printf("Acertou! O numero era %d.\n", segredo);
            return 0;
        }
        printf("Muito %s!\n", palpite < segredo ? "baixo" : "alto");
    }

    printf("As tentativas acabaram. O numero era %d.\n", segredo);
    return 0;
}
