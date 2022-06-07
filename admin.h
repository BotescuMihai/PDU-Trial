//
// Created by alex on 03.06.2022.
//

#ifndef PDU_TRIAL_ADMIN_H
#define PDU_TRIAL_ADMIN_H
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#define SOCK_PATH "/tmp/unixds"
#define CLIENT_PATH "tpf_unix_sock.client"
#define SERVER_PATH "tpf_unix_sock.server"


void send_option(int opt){
    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("socket");
        exit(1);
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, CLIENT_PATH);
    len = sizeof(client_sockaddr);

    unlink(CLIENT_PATH);
    rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
    if (rc == -1){
        perror("bind");
        close(client_sock);
        exit(1);
    }

    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* for the server socket and connect   */
    /* to it.                              */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
    if(rc == -1){
        perror("connect");
        close(client_sock);
        exit(1);
    }
    /************************************/
    /* Copy the data to the buffer and  */
    /* send it to the server socket.    */
    /************************************/
    char * DATA = malloc(5);
    sprintf(DATA, "%d", opt);
    strcpy(buf, DATA);
    printf("Sending data...\n");
    rc = send(client_sock, buf, strlen(buf), 0);
    if (rc == -1) {
        perror("send");
        close(client_sock);
        exit(1);
    }
    else {
        printf("Data sent!\n");
    }

}

static char * options[]={
        "0. Oprire client ADMIN", //client OBV
        "1. Permitere conexiuni server", // server
        "2. Deconectare (fortata) a altor clienti de la server", // server
        "3. Logging", // server
        "4. Oprire (fortata) a server-ului" // client
};

void opt4(){
    /* transmite semnal SIGTERM catre server */
    system("killall serverds");
}
void opt0(){
    fprintf(stdout, "Client Admin> Shut down\n");
    unlink(SOCK_PATH);
    exit(EXIT_SUCCESS);
}
#endif //PDU_TRIAL_ADMIN_H
