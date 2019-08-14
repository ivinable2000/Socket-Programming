#include <arpa/inet.h>
#include<stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    //convert port number into machine order
    address.sin_port = htons(atoi(argv[2]));
    //Convert IP address from dot notation to 32-bit network byte order
    //Put it in adress struct
    if (inet_pton(AF_INET, argv[1], &address.sin_addr.s_addr) == 0){
        perror("address incorrect");
        exit(1);
    }

    if(connect(sock, (struct sockaddr *) &address, sizeof(address)) == -1){
        perror("Connect failed");
        exit(1);
    }

    int n;
    char buf[1024];
    bzero(buf, 1024);
    //Send filename to server
    if((n = write(sock, argv[3], strlen(argv[3]))) < 0){
        perror("Writing filename failed");
        exit(1);
    }  

    if((n = read(sock, buf, sizeof(char))) < 0){
        perror("Reading failed");
        exit(1);
    }

    //If server sends a "n" file cannot be read, close client
    if(strcmp(buf, "n") == 0){
        perror("Server could not send file");
        close(sock);
        exit(1);
    }
    
    int f = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(f == -1){
        perror("File was not created");
        exit(1);
    }
    
    bzero(buf, 1024);
    while(0 < (n = read(sock, buf, 1024))){
        int w = write(f, buf, n);
        if (w < n)
        {
            perror("write failing");
        }
        bzero(buf, 1024);
    }
    close(f);
    close(sock);
    return 0;
}