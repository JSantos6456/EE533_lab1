#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>     // For exit()
#include <string.h>     // For bzero() or memset()
#include <unistd.h>     // For read(), write(), and close()
#include <signal.h>     // For signal handling

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void handle_client(int sockfd, struct sockaddr_in *cli_addr, socklen_t clilen)
{
    char buffer[256];
    int n;

    // Clear the buffer
    bzero(buffer, 256);

    // Receive message from client
    n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)cli_addr, &clilen);
    if (n < 0) error("ERROR in recvfrom");

    printf("Here is the message: %s\n", buffer);

    // Send response to client
    n = sendto(sockfd, "I got your message", 18, 0, (struct sockaddr *)cli_addr, clilen);
    if (n < 0) error("ERROR in sendto");
}

int main(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Initialize socket structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    printf("Server is listening on port %d...\n", portno);

    clilen = sizeof(cli_addr);

    while (1) {
        // Handle incoming client requests
        handle_client(sockfd, &cli_addr, clilen);
    }

    close(sockfd);
    return 0;
}

