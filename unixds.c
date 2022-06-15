#include <stddef.h>
#include "headerr.h"
#include <errno.h>
#include <pthread.h>
// #include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <dirent.h>
#include <time.h>
#include<signal.h>
#define MAX 1024
#define SOCK_PATH  "tpf_unix_sock.server"
#define UNIXSOCKET "tpf_unix_sock.client"
/*
extern WINDOW * mainwnd ;
extern pthread_mutex_t curmtx ;
*/

clock_t end;
double total_time = 0.0;
int server_sock, client_sock, len, rc;

char * my_ls(char * path){
    struct dirent *d;
    DIR * dh = opendir(path);
    if(!dh){
        perror("opendir");
        return NULL;
    }
    char * res = malloc(10000);
    sprintf(res, "\t[%s]\n", path);
    while((d = readdir(dh)) != NULL){
        char * tempent = malloc(1000);
        //  sprintf(tempent, "%d\t%s\t%d\t%d\t%d\t%d\n", d->d_ino, d->d_name, d->d_namlen, d->d_reclen, d->d_seekoff, d->d_type);
        sprintf(tempent,"%s\n", d->d_name);
        strcat(res, tempent);
    }
    return res;
}

int fd_temp;
char * procesare(long opt){
    fprintf(stderr,"Opt %ld\n", opt);
    char * rs1;
    int salvout, fdlog, fdlog2;
    char tmp[300];
    char buf[256];
    pid_t ppid;
    int counter = 0;
    switch(opt){
        case 1:
            salvout = dup(STDOUT_FILENO);
            fdlog = open("tempfileforfork.txt", O_CREAT | O_RDWR | S_IRUSR | S_IWUSR | S_IXUSR, 0666);
            dup2(fdlog, STDOUT_FILENO);
            fdlog2 = dup(fdlog);
            ppid = getpid();
            sprintf(&tmp, "ps -p %d -o etime", ppid);
            system(tmp);
            dup2(salvout, STDOUT_FILENO);
            close(fdlog);
            fdlog = open("tempfileforfork.txt", O_RDWR | O_CREAT);
            rs1 = (char*)malloc(256);
            read(fdlog, rs1, 256);
            close(fdlog);
            unlink("tempfileforfork.txt");
            break;
        case 2:
            system("killall serverds");
            break;
        case 3:
            for (int i = 0; i < FD_SETSIZE; ++i){
                if (FD_ISSET (i, &active_fd_set)){
                    close (i); FD_CLR (i, &active_fd_set);
                }
            }
            rs1 = (char*)malloc(100);
            strcpy(rs1,"Am executat\n");
            rs1[strlen(rs1)] = 0;
            break;
        case 4:
            memset(buf, 0, 256);
            int bytes_rec = recv(client_sock, buf, sizeof(buf), 0);
            if (bytes_rec == -1) {
                perror("recv");
                //   printf("RECV ERROR: %d\n", sock_errno());
                close(server_sock);
                close(client_sock);
                exit(1);
            } else {
                printf("DATA RECEIVED = %s\n", buf);
            }
            //strcpy(rs1, buf);


            salvout = dup(STDOUT_FILENO);
            fdlog = open("tempfileforfork2.txt", O_CREAT | O_RDWR | S_IRUSR | S_IWUSR | S_IXUSR, 0666);
            dup2(fdlog, STDOUT_FILENO);
            fdlog2 = dup(fdlog);
            sprintf(&tmp, "ls ./serv_files/INET/%s", buf);
            system(tmp);
            dup2(salvout, STDOUT_FILENO);
            close(fdlog);
            fdlog = open("tempfileforfork2.txt", O_RDWR | O_CREAT);
            rs1 = (char*)malloc(256);
            read(fdlog, rs1, 256);
            close(fdlog);
            unlink("tempfileforfork2.txt");
            if(strlen(rs1)!=0){
                salvout = dup(STDOUT_FILENO);
                fdlog = open("tempfileforfork2.txt", O_CREAT | O_RDWR | S_IRUSR | S_IWUSR | S_IXUSR, 0666);
                dup2(fdlog, STDOUT_FILENO);
                fdlog2 = dup(fdlog);
                sprintf(&tmp, "rm ./serv_files/INET/%s", buf);
                system(tmp);
                dup2(salvout, STDOUT_FILENO);
                close(fdlog);
                fdlog = open("tempfileforfork2.txt", O_RDWR | O_CREAT);
                rs1 = (char*)malloc(256);
                read(fdlog, rs1, 256);
                close(fdlog);
                unlink("tempfileforfork2.txt");
                strcpy(rs1,"Fisier sters!");
                rs1[strlen(rs1)] = 0;
            }
            else{
                strcpy(rs1,"Acest fisier nu exista!");
            }

            break;
        case 5:
            for (int i = 0; i < FD_SETSIZE; ++i){
                if (FD_ISSET (i, &active_fd_set) && i != STDIN_FILENO){
                    counter ++;
                }
            }
            rs1 = (char*)malloc(100);
            sprintf(rs1,"%d clienti", counter-1);
            rs1[strlen(rs1)] = 0;
            break;
        default:
            rs1 = NULL;
            break;
    }
    /*
    fd_temp = open("temp", O_RDWR | O_CREAT, O_TRUNC);
    char * buf = (char*) malloc(10000);
    read(fd_temp, buf, 10000);
    buf[strlen(buf)] = 0;
    return buf; */
    return rs1;
}
int conn;
void *unix_main (void *args) {
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];
    int backlog = 10;
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, 256);

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1){
        perror("socket");
        //      printf("SOCKET ERROR: %d\n", sock_errno());
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
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCK_PATH);
    len = sizeof(server_sockaddr);

    unlink(SOCK_PATH);
    rc = bind(server_sock, (struct sockaddr *) &server_sockaddr, len);
    if (rc == -1){
        perror("bind");
        // printf("BIND ERROR: %d\n", sock_errno());
        close(server_sock);
        exit(1);
    }

    /*********************************/
    /* Listen for any client sockets */
    /*********************************/
    rc = listen(server_sock, backlog);
    if (rc == -1){
        perror("listen");
        //printf("LISTEN ERROR: %d\n", sock_errno());
        close(server_sock);
        exit(1);
    }
    printf("socket listening...\n");
    char opt[10];
    /*********************************/
    /* Accept an incoming connection */
    /*********************************/
    if(conn == 0) {
        client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr, &len);
        if (client_sock == -1) {
            perror("accept");
            //   printf("ACCEPT ERROR: %d\n", sock_errno());
            close(server_sock);
            close(client_sock);
            exit(1);
        }
        conn = 1;
        /****************************************/
        /* Get the name of the connected socket */
        /****************************************/
        len = sizeof(client_sockaddr);
        rc = getpeername(client_sock, (struct sockaddr *) &client_sockaddr, &len);
        if (rc == -1) {
            perror("getpeername");
            //  printf("GETPEERNAME ERROR: %d\n", sock_errno());
            close(server_sock);
            close(client_sock);
            exit(1);
        } else {
            printf("Client socket filepath: %s\n", client_sockaddr.sun_path);
        }
        char * buff;
        /************************************/
        /* Read and print the data          */
        /* incoming on the connected socket */
        /************************************/
        while (1) {
            printf("waiting to read...\n");
            bytes_rec = recv(client_sock, buf, sizeof(buf), 0);
            if (bytes_rec == -1) {
                perror("recv");
                //   printf("RECV ERROR: %d\n", sock_errno());
                close(server_sock);
                close(client_sock);
                exit(1);
            } else {
                printf("DATA RECEIVED = %s\n", buf);
                strcpy(opt, buf);
            }
            long opt = strtol(buf, NULL, 10);
            buff = procesare(opt);
            /******************************************/
            /* Send data back to the connected socket */
            /******************************************/
            // sprintf(buf, "Mi-ai trimis optiunea %s", buf);
            if(buff != NULL) {
                //memset(buf, 0, 256);
                //  sprintf(buf, "Mi-ai trimis optiunea %s", opt);
                //  buf[strlen(buf)] = 0;
                printf("Sending data...\n");
                rc = send(client_sock, buff, strlen(buff), 0);
                if (rc == -1) {
                    perror("send");
                    //printf("SEND ERROR: %d", sock_errno());
                    close(server_sock);
                    close(client_sock);
                    exit(1);
                } else {
                    printf("Data sent!\n");
                }
            }

            /******************************/
            /* Close the sockets and exit */
            /******************************/
        }
    }
    else{
        /*
        printf("Sending data...\n");
        buff = malloc(100);
        strcpy(buff, "Nu pot accepta alte conexiuni\n");
        rc = send(client_sock, buff, strlen(buff), 0);
        if (rc == -1) {
            perror("send");
            //printf("SEND ERROR: %d", sock_errno());
            close(server_sock);
            close(client_sock);
            exit(1);
        } else {
            printf("Data sent!\n");
            close(client_sock);
        } */
        fprintf(stderr, "Nu pot accepta alte conexiuni\n");
    }
    close(server_sock);
    close(client_sock);
    pthread_exit(NULL);
}


