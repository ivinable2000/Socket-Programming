#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    int sfd;
    struct sockaddr_in address;

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sfd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) == -1){
        perror("bind failed");
        exit(1);
    }

    if(listen(sfd, 2) == -1){
        perror("listen failed");
        exit(1);
    }

    for(;;)
    {
        int cfd;
        int b;
        struct sockaddr_in caddress;
        socklen_t sinlen = sizeof(struct sockaddr_in);
        char buf[1024];
        int f;

        cfd = accept(sfd, (struct sockaddr *)&caddress, &sinlen);
        
        //Read filename from client and open file
        if ((b = read(cfd, buf, 1024)) < 0){
            perror("Could not read filename from client");
        }
        else{
            f = open(buf, O_RDONLY);

            //Send a "n" to the client if file cannot be opened
            if(f == -1){
                perror("file returned NULL");
                write(cfd, "n", strlen("n"));
            }
            //Send a "y" to the client if file is opened and then send contents
            else{
                write(cfd, "y", sizeof(char));
                while((b = read(f, buf, 1024)) > 0){
                    write(cfd, buf, b);
                    bzero(buf, b);
                }
            }
        }

        close(f);
        close(cfd);
    }
    close(sfd);
    return 0;
}