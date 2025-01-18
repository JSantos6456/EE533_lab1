/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
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

void handle_client(int newsockfd)
{
    char buffer[256];
    int n;

    // Clear the buffer
    bzero(buffer, 256);

    // Read client message
    n = read(newsockfd, buffer, 255);
    if (n < 0) error("ERROR reading from socket");

    printf("Here is the message: %s\n", buffer);

    // Write response to client
    n = write(newsockfd, "I got your message", 18);
    if (n < 0) error("ERROR writing to socket");

    // Close client socket
    close(newsockfd);
}


int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    pid_t pid;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Setup SIGCHLD handler to prevent zombies
    signal(SIGCHLD, SIG_IGN);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    // Start listening for connections
    listen(sockfd, 5);
    printf("Server is listening on port %d...\n", portno);

    clilen = sizeof(cli_addr);

    while (1) {
        // Accept a new client connection
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        // Fork a new process to handle the client
        pid = fork();
        if (pid < 0) {
            error("ERROR on fork");
        }

        if (pid == 0) { // Child process
            close(sockfd); // Child does not need the listening socket
            handle_client(newsockfd); // Handle the client connection
            exit(0); // Terminate child process after handling client
        } else { // Parent process
            close(newsockfd); // Parent does not need the client socket
        }
    }

    close(sockfd);
    return 0;
}

