#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

void initializeWinsock();
SOCKET createSocket();
void bindSocket(SOCKET serverSocket);
void listenForConnections(SOCKET serverSocket);
void handleClient(SOCKET clientSocket);
void generateUsername(char *username, int length);
void generatePassword(char *password, int length);

int main() {
    WSADATA wsaData;
    SOCKET serverSocket = INVALID_SOCKET;
    SOCKET clientSocket;

    initializeWinsock();
    serverSocket = createSocket();
    bindSocket(serverSocket);
    listenForConnections(serverSocket);

    printf("Servidor en ejecución. Esperando conexiones...\n");

    while (clientSocket = accept(serverSocket, NULL, NULL)) {
        if (clientSocket == INVALID_SOCKET) {
            perror("Error al aceptar la conexión");
            continue;
        }
        printf("Cliente conectado.\n");
        handleClient(clientSocket);
        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

void initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        fprintf(stderr, "WSAStartup falló: %d\n", result);
        exit(EXIT_FAILURE);
    }
}

SOCKET createSocket() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        perror("Error al crear el socket");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return serverSocket;
}

void bindSocket(SOCKET serverSocket) {
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        perror("Error al enlazar el socket");
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void listenForConnections(SOCKET serverSocket) {
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        perror("Error al escuchar en el socket");
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    int recvResult;
    int length;
    char response[BUFFER_SIZE];

    // Recibir el tipo de solicitud
    recvResult = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (recvResult > 0) {
        buffer[recvResult] = '\0';
        int requestType = atoi(buffer); // Se espera que el cliente envíe un número indicando el tipo de solicitud

        // Recibir la longitud deseada
        recvResult = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (recvResult > 0) {
            buffer[recvResult] = '\0';
            length = atoi(buffer);

            // Procesar solicitud y generar respuesta
            if (requestType == 1) { // Generar nombre de usuario
                if (length < 5 || length > 15) {
                    strcpy(response, "Longitud no válida");
                } else {
                    generateUsername(response, length);
                }
            } else if (requestType == 2) { // Generar contraseña
                if (length < 8 || length >= 50) {
                    strcpy(response, "Longitud no válida");
                } else {
                    generatePassword(response, length);
                }
            } else {
                strcpy(response, "Tipo de solicitud no válido");
            }

            // Enviar la respuesta al cliente
            send(clientSocket, response, strlen(response), 0);
        } else {
            strcpy(response, "Error al recibir longitud");
            send(clientSocket, response, strlen(response), 0);
        }
    } else if (recvResult == 0) {
        printf("Conexión cerrada\n");
    } else {
        perror("Error al recibir datos");
    }
}

void generateUsername(char *username, int length) {
    static const char vowels[] = "aeiou";
    static const char consonants[] = "bcdfghjklmnpqrstvwxyz";
    char *vowelPtr = (char *)vowels;
    char *consonantPtr = (char *)consonants;
    int i;

    srand((unsigned int)time(NULL));
    int startWithVowel = rand() % 2;

    for (i = 0; i < length; i++) {
        if ((i % 2 == 0 && startWithVowel) || (i % 2 != 0 && !startWithVowel)) {
            username[i] = vowelPtr[rand() % (sizeof(vowels) - 1)];
        } else {
            username[i] = consonantPtr[rand() % (sizeof(consonants) - 1)];
        }
    }
    username[length] = '\0';
}

void generatePassword(char *password, int length) {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int i;

    srand((unsigned int)time(NULL));
    for (i = 0; i < length; i++) {
        password[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    password[length] = '\0';
}
