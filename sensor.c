#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

#define PORTA 3000
#define HOST "127.0.0.1"

void gerar_dados(char *mensagem, int sensor_id, float lat, float lon) {

    float temperatura = 15 + ((float)rand() / RAND_MAX) * (40 - 15);
    float umidade = 20 + ((float)rand() / RAND_MAX) * (100 - 20);
    float vento = ((float)rand() / RAND_MAX) * 70;
    float psi = 950 + ((float)rand() / RAND_MAX) * (1050 - 950);
    float uv = ((float)rand() / RAND_MAX) * 11;
    float ph = 5 + ((float)rand() / RAND_MAX) * (7 - 5);


    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *tm_info = localtime(&tv.tv_sec);
    char horario[40];

    strftime(horario, sizeof(horario), "%Y-%m-%d %H:%M:%S", tm_info);
    int millisec = tv.tv_usec / 1000;

    sprintf(mensagem,
        "[%s.%03d] ID: %d | Temp: %.2f°C | Umidade: %.2f%% | Vento: %.2fkm/h | Pressão: %.2fhPa | UV: %.2f | pH: %.2f | Localização: (%.2f, %.2f)",
        horario, millisec, sensor_id, temperatura, umidade, vento, psi, uv, ph, lat, lon
    );
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <ID>\n", argv[0]);
        return 1;
    }
    int id = atoi(argv[1]);
    int freq = rand() % 10 + 1;
    float lat = rand() % 180 - 90;
    float lon = rand() % 360 - 180;

    srand(time(NULL) + id * 5);

    while (1) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Erro ao criar socket");
            sleep(freq);
            continue;
        }
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORTA);
        inet_pton(AF_INET, HOST, &server_addr.sin_addr);

        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Erro ao conectar ao centralizador");
            close(sock);
            sleep(freq);
            continue;
        }

        char mensagem[512];
        gerar_dados(mensagem, id, lat, lon);

        if (write(sock, mensagem, strlen(mensagem)) < 0) {
            perror("Erro ao enviar dados");
        }

        close(sock);
        sleep(freq);
    }
    return 0;
}
