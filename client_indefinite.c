#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>  // For inet_pton() and inet_ntoa()
#include <stdlib.h>  // For exit()
#include <string.h>  // For memset() (instead of bzero)
#include <unistd.h>  // For read() and write()

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    printf("Connected to server. Type 'exit' to disconnect.\n");

    while (1) {
        printf("Please enter the message: ");
        bzero(buffer, 256); // Clear buffer

        // Get input from user
        fgets(buffer, 255, stdin);

        // Check for exit command
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Disconnecting from server...\n");
            break;  // Exit the loop and close the connection
        }

        // Send the message to the server
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");

        // Clear buffer for reading response
        bzero(buffer, 256);

        // Read the server's response
        n = read(sockfd, buffer, 255);
        if (n < 0) 
            error("ERROR reading from socket");

        // Display server's response
        printf("Server: %s\n", buffer);
    }

    // Close the connection after exiting the loop
    close(sockfd);
    return 0;
}

