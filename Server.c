#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg); // Free the dynamically allocated memory
    char buffer[BUFFER_SIZE];

    printf("Client handler started for client: %d\n", client_socket);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            printf("Client disconnected: %d\n", client_socket);
            close(client_socket);

            pthread_mutex_lock(&mutex);
            for (int i = 0; i < client_count; i++) {
                if (client_sockets[i] == client_socket) {
                    for (int j = i; j < client_count - 1; j++) {
                        client_sockets[j] = client_sockets[j + 1];
                    }
                    client_count--;
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Message from client %d: %s", client_socket, buffer);

        if (strncmp(buffer, "Bye", 3) == 0) {
            printf("Client %d exiting.\n", client_socket);
            break;
        }

        // Send message to all clients except the sender
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < client_count; i++) {
            if (client_sockets[i] != client_socket) {
                char server_response[BUFFER_SIZE];
                snprintf(server_response, sizeof(server_response), "Server: %s", buffer);
                send(client_sockets[i], server_response, strlen(server_response), 0);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        error("ERROR opening socket");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("ERROR binding socket");
    }

    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        error("ERROR listening");
    }
    printf("Server running on port %d...\n", PORT);

    while (1) {
        printf("Waiting for clients...\n");
        int new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (new_socket < 0) {
            perror("ERROR accepting connection");
            continue;
        }

        printf("New client connected: %d\n", new_socket);

        pthread_mutex_lock(&mutex);
        client_sockets[client_count++] = new_socket;
        pthread_mutex_unlock(&mutex);

        pthread_t tid;
        int *new_sock_ptr = malloc(sizeof(int));  // Dynamically allocate memory for the socket
        if (new_sock_ptr == NULL) {
            perror("ERROR allocating memory for socket");
            close(new_socket);
            continue;
        }

        *new_sock_ptr = new_socket;

        if (pthread_create(&tid, NULL, handle_client, (void *)new_sock_ptr) != 0) {
            perror("ERROR creating thread");
            close(new_socket);
            free(new_sock_ptr);
            continue;
        }

        // Detach the thread so resources are freed when it's done
        pthread_detach(tid);
    }

    // Close the server socket (although the server will run indefinitely)
    close(server_socket);
    return 0;
}
