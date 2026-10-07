#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_SENSORES 3
#define MAX_LEITURAS 20

enum TipoSensor {
    PH,
    TURBIDEZ,
    TEMPERATURA
};

struct Leitura {
    float valor;
    struct tm data_hora;
};

struct Sensor {
    const char *nome;
    enum TipoSensor tipo;
    struct Leitura leituras[MAX_LEITURAS];
    int total_leituras;
};

static float simular_leitura(enum TipoSensor tipo) {
    switch (tipo) {
        case PH:
            return 6.0f + (rand() % 201) / 100.0f;       // 6.00 a 8.00
        case TURBIDEZ:
            return 0.5f + (rand() % 296) / 10.0f;         // 0.5 a 30.0 NTU
        case TEMPERATURA:
            return 10.0f + (rand() % 251) / 10.0f;        // 10.0 a 35.0 °C
        default:
            return 0.0f;
    }
}

static void inicializar_sensores(struct Sensor sensores[]) {
    sensores[0] = (struct Sensor){.nome = "pH", .tipo = PH};
    sensores[1] = (struct Sensor){.nome = "Turbidez", .tipo = TURBIDEZ};
    sensores[2] = (struct Sensor){.nome = "Temperatura", .tipo = TEMPERATURA};

    for (int i = 0; i < TOTAL_SENSORES; i++) {
        sensores[i].total_leituras = 0;
    }
}

static int registrar_leitura(struct Sensor *sensor) {
    if (sensor->total_leituras >= MAX_LEITURAS) {
        return 0;
    }

    time_t agora = time(NULL);
    struct tm *horario = localtime(&agora);
    if (horario == NULL) {
        fprintf(stderr, "Erro ao obter a data e hora.\n");
        return 0;
    }

    struct Leitura *leitura = &sensor->leituras[sensor->total_leituras];
    leitura->valor = simular_leitura(sensor->tipo);
    leitura->data_hora = *horario;
    sensor->total_leituras++;

    return 1;
}

static void mostrar_ultima_leitura(const struct Sensor *sensor) {
    if (sensor->total_leituras == 0) {
        return;
    }

    char buffer[30];
    int idx = sensor->total_leituras - 1;

    if (strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S",
                 &sensor->leituras[idx].data_hora) == 0) {
        fprintf(stderr, "Erro ao formatar a data e hora.\n");
        return;
    }

    printf("Última leitura de %s: %.2f | Data/hora: %s\n",
           sensor->nome, sensor->leituras[idx].valor, buffer);
}

static float calcular_media(const struct Sensor *sensor) {
    if (sensor->total_leituras == 0) {
        return 0.0f;
    }

    float soma = 0.0f;
    for (int i = 0; i < sensor->total_leituras; i++) {
        soma += sensor->leituras[i].valor;
    }

    return soma / sensor->total_leituras;
}

static int salvar_em_csv(const struct Sensor sensores[]) {
    FILE *arquivo = fopen("qualidade_agua.csv", "w");
    if (arquivo == NULL) {
        perror("Não foi possível criar qualidade_agua.csv");
        return 0;
    }

    fprintf(arquivo, "Sensor,Valor,DataHora\n");

    for (int i = 0; i < TOTAL_SENSORES; i++) {
        for (int j = 0; j < sensores[i].total_leituras; j++) {
            char buffer[30];

            if (strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S",
                         &sensores[i].leituras[j].data_hora) == 0) {
                fprintf(stderr, "Erro ao formatar uma data para o CSV.\n");
                fclose(arquivo);
                return 0;
            }

            fprintf(arquivo, "%s,%.2f,%s\n",
                    sensores[i].nome,
                    sensores[i].leituras[j].valor,
                    buffer);
        }
    }

    if (fclose(arquivo) != 0) {
        perror("Erro ao fechar o arquivo CSV");
        return 0;
    }

    return 1;
}

int main(void) {
    srand((unsigned)time(NULL));

    struct Sensor sensores[TOTAL_SENSORES];
    inicializar_sensores(sensores);

    char continuar = 's';
    int rodada = 1;

    while (continuar == 's' || continuar == 'S') {
        printf("\nRodada %d de medições:\n", rodada);

        for (int i = 0; i < TOTAL_SENSORES; i++) {
            if (registrar_leitura(&sensores[i])) {
                mostrar_ultima_leitura(&sensores[i]);
            } else {
                printf("Limite de %d leituras atingido.\n", MAX_LEITURAS);
                continuar = 'n';
                break;
            }
        }

        if (continuar == 'n') {
            break;
        }

        printf("\nDeseja registrar uma nova rodada? (s/n): ");
        if (scanf(" %c", &continuar) != 1) {
            fprintf(stderr, "\nNão foi possível ler a resposta.\n");
            break;
        }

        rodada++;
    }

    printf("\nMédias por sensor:\n");
    for (int i = 0; i < TOTAL_SENSORES; i++) {
        printf("%s: %.2f\n", sensores[i].nome, calcular_media(&sensores[i]));
    }

    if (salvar_em_csv(sensores)) {
        printf("\nLeituras salvas em 'qualidade_agua.csv'.\n");
    }

    return 0;
}