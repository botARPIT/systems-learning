// This file demonstrates how to create a single TCP HTTP server

#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main(){

    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // Create server fd

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
    bind(
        server_fd,
        (struct sockaddr*)&server_addr,
        sizeof(server_addr)
    );

    // listen on socket
    listen(
        server_fd,
        5
    );

    printf("Server listening on the port: 8080 \n");

    while(1){

        int client_fd = accept(server_fd, NULL, NULL); // create separate fd for client

        printf("Client connected with fd: %d\n", client_fd);

        char buffer[1024]; // buffer to receive message

        int bytes = read(client_fd, buffer, sizeof(buffer));

        buffer[bytes] = '\0';

        printf("Received message: %s\n", buffer);

        write(client_fd, "Hello from server \n", 19);
        close(client_fd);

    }
    return 0;

}