#include <stddef.h>
#include <errno.h>
#include <pthread.h>
// #include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/un.h>
#include <time.h>
#define SERVER_PATH "tpf_unix_sock.server"
#define CLIENT_PATH "tpf_unix_sock.client"
#define DATA "Hello from client"

char * opts[] ={
        "Active runtime (de cat timp este pornit server-ul)",
        "Oprire (fortata) server",
        "Deconecteaza toti clientii de INET",
        "Stergere fisier existent de la nivelul client-ului INET (UX) -- [LOCAL]",
        "Numarul de clienti conectati"
};


void meniu(){
    int i;
    for(i=0;i<sizeof(opts)/sizeof(char*);i++){
        fprintf(stderr, "[%d]\t--  %s  --\n", i+1, opts[i]);
    }
}


int main(void){

    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[10000];
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));


    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("socket");
        //      printf("SOCKET ERROR = %d\n", sock_errno());
        exit(1);
    }

    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, CLIENT_PATH);
    len = sizeof(client_sockaddr);

    unlink(CLIENT_PATH);
    rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
    if (rc == -1){
        perror("bind");
        // printf("BIND ERROR: %d\n", sock_errno());
        close(client_sock);
        exit(1);
    }

    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
    if(rc == -1){
        perror("connect");
        //    printf("CONNECT ERROR = %d\n", sock_errno());
        close(client_sock);
        exit(1);
    }
    //  strcpy(buf, DATA);
    while(1) {

        meniu();
        fprintf(stderr, "Selecteaza optiunea...>");
        scanf("%s", buf);
        long opt = strtol(buf, NULL, 10);
        rc = send(client_sock, buf, strlen(buf), 0);
        if (opt == 4){
            fprintf(stderr,"Introduceti numele fisierului...>");
            scanf("%s", buf);
            rc = send(client_sock, buf, strlen(buf),0);
        }
        printf("Sending data...\n");
        if (rc == -1) {
            perror("send");
            // printf("SEND ERROR = %d\n", sock_errno());
            close(client_sock);
            exit(1);
        } else {
            printf("Data sent!\n");
        }

        if (opt <= 5) {
            printf("Waiting to recieve data...\n");
            memset(buf, 0, 10000);
            rc = recv(client_sock, buf, sizeof(buf), 0);
            if (rc == -1) {
                perror("recv");
                //   printf("RECV ERROR = %d\n", sock_errno());
                close(client_sock);
                exit(1);
            } else {
                if(opt == 1){
                    clock_t end = clock();
                    double endd = (double)end / CLOCKS_PER_SEC;
                    printf("%s\n", buf);
                }
                else printf("%s\n", buf);
            }
        }
    }

    close(client_sock);

    return 0;
}

