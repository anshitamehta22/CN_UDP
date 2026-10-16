
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in server_addr;
    int server_len = sizeof(server_addr);
    char buffer[1024];

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("Enter a string (exit to quit): ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            break;

        buffer[strcspn(buffer, "\n")] = '\0';

        if (sendto(sockfd, buffer, (int)strlen(buffer), 0,
                   (struct sockaddr *)&server_addr,
                   server_len) == SOCKET_ERROR) {
            printf("Send failed.\n");
            break;
        }

        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         NULL, NULL);

        if (n == SOCKET_ERROR) {
            printf("Receive failed.\n");
            break;
        }

        buffer[n] = '\0';
        printf("Server response: %s\n", buffer);

        if (strcmp(buffer, "Server disconnected.") == 0)
            break;
    }

    closesocket(sockfd);
    WSACleanup();

    printf("Client socket closed.\n");
    return 0;
}
