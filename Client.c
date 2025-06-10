#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int client_fd, n;
    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[1024];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(1);
    }

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        error("ERROR opening socket");
    }


    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }


    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);


    int port = atoi(argv[2]);
    if (port <= 0) {
        fprintf(stderr, "ERROR, invalid port number\n");
        exit(1);
    }
    server_addr.sin_port = htons(port);

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("ERROR connecting");
    }

    printf("Connected to the server...\n");

    // Communicate with server
    while (1) {
        bzero(buffer, sizeof(buffer));
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);

        n = send(client_fd, buffer, strlen(buffer), 0);
        if (n < 0) {
            error("ERROR writing to socket");
        }
        
        if (strncmp(buffer, "Bye", 3) == 0) {
            printf("Exiting Client...\n");
            break;
        } 

        bzero(buffer, sizeof(buffer));
        n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            error("ERROR reading from socket");
        }
        printf("Server: %s\n", buffer);

    }
    
    // Close the socket
    close(client_fd);
    
    return 0;
}
