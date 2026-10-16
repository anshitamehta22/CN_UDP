
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);
    char buffer[1024];
    char response[1024];

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
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("UDP Server is running...\n");

    while (1) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&client_addr, &client_len);

        if (n == SOCKET_ERROR)
            break;

        buffer[n] = '\0';
        printf("Received: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            strcpy(response, "Server disconnected.");
        } else {
            snprintf(response, sizeof(response), "%s", buffer);
        }

        printf("Sent: %s\n", response);

        sendto(sockfd, response, (int)strlen(response), 0,
               (struct sockaddr *)&client_addr, client_len);

        if (strcmp(buffer, "exit") == 0)
            break;
    }

    closesocket(sockfd);
    WSACleanup();

    printf("UDP Server closed.\n");
    return 0;
}
