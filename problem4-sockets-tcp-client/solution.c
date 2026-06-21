#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        close(sock);
        return 1;
    }

    char buffer[1024];
    int num;
    while (scanf("%d", &num) == 1) {
        snprintf(buffer, sizeof(buffer), "%d\n", num);
        if (send(sock, buffer, strlen(buffer), 0) == -1) {
            perror("send");
            break;
        }
    }

    shutdown(sock, SHUT_WR);

    ssize_t bytes;
    while ((bytes = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }

    close(sock);
    return 0;
}
