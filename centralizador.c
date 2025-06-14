#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORTA 3000
#define BUFFER_SIZE 1024

typedef struct {
    int sock;
    struct sockaddr_in addr;
} client_data;

void *handle_client(void *arg) {
    client_data *data = (client_data *)arg;
    char buffer[BUFFER_SIZE];
    int n;

    FILE *fp = fopen("log.txt", "a");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        close(data->sock);
        free(data);
        pthread_exit(NULL);
    }

    while ((n = read(data->sock, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        printf("Recebido de %s: %s\n", inet_ntoa(data->addr.sin_addr), buffer);
        fprintf(fp, "%s\n", buffer);
        fflush(fp);
    }
    fclose(fp);
    close(data->sock);
    free(data);
    pthread_exit(NULL);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Erro ao criar socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORTA);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro no bind");
        close(server_sock);
        exit(1);
    }

    if (listen(server_sock, 10) < 0) {
        perror("Erro no listen");
        close(server_sock);
        exit(1);
    }

    printf("Centralizador ouvindo na porta %d...\n", PORTA);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("Erro no accept");
            continue;
        }

        client_data *data = malloc(sizeof(client_data));
        data->sock = client_sock;
        data->addr = client_addr;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, data);
        pthread_detach(tid);  // Limpa a thread automaticamente após terminar
    }

    close(server_sock);
    return 0;
}
