#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

void initializeWinsock();
SOCKET createSocket();
void connectToServer(SOCKET clientSocket);
void sendRequest(SOCKET clientSocket, int requestType, int length);
void receiveResponse(SOCKET clientSocket);

int main() {
    WSADATA wsaData;
    SOCKET clientSocket = INVALID_SOCKET;
    int choice;
    int length;

    initializeWinsock();
    clientSocket = createSocket();
    connectToServer(clientSocket);

    while (1) {
        printf("Menu:\n");
        printf("1. Generar nombre de usuario\n");
        printf("2. Generar contraseña\n");
        printf("3. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &choice);

        if (choice == 3) {
            break;
        }

        if (choice == 1) {
            printf("Ingrese la longitud (numero entero entre 5 y 15): ");
            scanf("%d", &length);

            // Enviar la solicitud al servidor
            sendRequest(clientSocket, choice, length);

            // Recibir la respuesta del servidor
            receiveResponse(clientSocket);
        } else if(choice == 2){
            printf("Ingrese la longitud (numero entero entre 8 y 50): ");
            scanf("%d", &length);

            // Enviar la solicitud al servidor
            sendRequest(clientSocket, choice, length);

            // Recibir la respuesta del servidor
            receiveResponse(clientSocket);
        } else {
            printf("Opcion no valida. Intente de nuevo.\n");
        }
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

void initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        fprintf(stderr, "WSAStartup fallo: %d\n", result);
        exit(EXIT_FAILURE);
    }
}

SOCKET createSocket() {
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        perror("Error al crear el socket");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return clientSocket;
}

void connectToServer(SOCKET clientSocket) {
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddress.sin_port = htons(PORT);

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        perror("Error al conectar al servidor");
        closesocket(clientSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void sendRequest(SOCKET clientSocket, int requestType, int length) {
    char buffer[BUFFER_SIZE];

    // Enviar el tipo de solicitud
    sprintf(buffer, "%d", requestType);
    if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
        perror("Error al enviar el tipo de solicitud");
        exit(EXIT_FAILURE);
    }

    // Enviar la longitud deseada
    sprintf(buffer, "%d", length);
    if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
        perror("Error al enviar la longitud");
        exit(EXIT_FAILURE);
    }
}

void receiveResponse(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    int recvResult;

    // Recibir la respuesta del servidor
    recvResult = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (recvResult > 0) {
        buffer[recvResult] = '\0';
        printf("Respuesta del servidor: %s\n", buffer);
    } else if (recvResult == 0) {
        printf("El servidor cerró la conexión.\n");
    } else {
        perror("Error al recibir datos");
    }
}
