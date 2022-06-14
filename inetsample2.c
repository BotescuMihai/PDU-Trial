#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/dirent.h>
#include <sys/un.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "proto.h"
#include "files_util.h"
#define BUF_SIZE 1000000
#define PORT            18081
#define SERVERHOST      "127.0.0.1" 
#define MSGLEN	128

static char * options[] = {
        "Trimite fisier sursa spre executie",
        "Creeaza un fisier nou pe server",
        "Modifica numele unui fisier deja existent pe server",
        "Sterge un fisier existent de pe server",
        "Vizualizeaza toate log-urile de la nivel de client",
        "Vizualizeaza toate fisierele de la nivel de server",
        "Vizualizeaza programele scrise intr-un anumit limbaj de programare de la nivel de server",
        "Afiseaza marimea totala a tuturor fisierelor stocate pe server (INET) [GB]",
        "Afiseaza informatii legate de cel mai MARE fisier din toate fisierele stocate pe server (INET)",
        "Afiseaza informatii legate de cel mai MIC fisier din toate fisierele stocate pe server (INET)",
        "Statistica (limbaj de programare -- numar fisiere)"
};

static char * avail_programming_languages[] ={
        "c",
        "cpp",
        "java",
        "py",
        NULL
};

void avail_languages_menu(){
    int i;
    for(i=0;avail_programming_languages[i] != NULL;i++){
        fprintf(stderr, "[%d]\t%s\n", i + 1, avail_programming_languages[i]);
    }
    fprintf(stderr, "Alegeti optiunea...>");
}

void menu(){
    int i;
    for(i=0;i<sizeof(options)/sizeof(char*);i++){
        fprintf(stderr, "[%d]\t%s\n", i + 1, options[i]);
    }
    fprintf(stderr, "Alegeti optiunea...>");
}



void init_sockaddr (struct sockaddr_in *name,
  const char *hostname,
  uint16_t port)
  {
  struct hostent *hostinfo;
  
  name->sin_family = AF_INET;
  name->sin_port = htons (port);
  hostinfo = gethostbyname (hostname);
  if (hostinfo == NULL)
  {
  fprintf (stderr, "Unknown host %s.\n", hostname);
  exit (EXIT_FAILURE);
  }
  name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
  }

void write_server (int fd, char *TMESSAGE) {
  int nbytes;
  
  nbytes = send (fd, TMESSAGE, strlen (TMESSAGE) + 1, 0);
  if (nbytes < 0) {
    perror ("write");
    exit (EXIT_FAILURE);
  }
}


int main (void) {
  int sock; 
  struct sockaddr_in servername;

  
  /* Create the socket. */
  sock = socket (PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror ("socket (client)");
    exit (EXIT_FAILURE);
  }
  
  /* Connect to the server. */
  init_sockaddr (&servername, SERVERHOST, PORT);
  if (0 > connect (sock,
		   (struct sockaddr *) &servername,
		   sizeof (servername))) {
    perror ("connect (client)");
    exit (EXIT_FAILURE);
  }
  
/* 0. Send a connect request!. */
  msgHeaderType h ;
  msgIntType m ;
  msgStringType str ;
  int clientID = 0 ;
  int fd;
  h.clientID = 0 ;
    char filename[100];
    h.fileName = malloc(100);
    strcpy(h.fileName, "/serv_files/INET/lorem_transfered.txt");
 writeSingleInt (sock, h, 0) ; // Just for tests, ignore the response!
  readSingleInt (sock,  &m) ;   // Just for tests, ignore the response!
  clientID = m.msg ;
  h.clientID = clientID; // salvez ID-ul
  fprintf (stderr, "Got a clientID: %d\n", clientID) ;
/* 1. Now do some stupid math: negative of a number.
  h.clientID = clientID ;
  h.opID = OPR_NEG ;
  fprintf (stderr, "1. Do some OPR_NEG calls...\n") ;
  for (int i =0; i < 20; i++) {
    writeSingleInt (sock, h, -100+i*15) ; // Just for tests, ignore the response!
    readSingleInt (sock,  &m) ;   // Just for tests, ignore the response!
    fprintf (stderr, "Got the negative (hopefully) version of %d ==> %d\n", -100+i*15, m.msg) ;
  } */
/* 2. More stupid math: add two numbers :)
  h.clientID = clientID ;
  h.opID = OPR_ADD ;
  fprintf (stderr, "2. Do some OPR_ADD calls...\n") ;
  for (int i =0; i < 120; i++) {
    writeMultiInt (sock, h, -100+i*15, 100-i*22) ; // Just for tests, ignore the response!
    readSingleInt (sock,  &m) ;   // Just for tests, ignore the response!
    fprintf (stderr, "Got the sum of %d %d ==> %d\n", -100+i*15, 100-i*22, m.msg) ;
  } */
/* 3. ECHO */
//int fd;
int id = 0;
   // h.opID = OPR_ECHO ;
char opt[2];
    while(1) {
        menu();
        bzero(opt, 2);
        scanf("%s", opt);
        writeSingleString(sock, h, opt);
        long optiune = strtol(opt, NULL, 10);
        h.opID = optiune;
        if (optiune == 1) {
            start:
            bzero(filename, sizeof(filename));
            h.clientID = clientID;
            h.opID = 1;
            fprintf(stderr, "Enter the file name...> ");
            scanf("%s", filename);
            msgStringType Filename;
            Filename.msg = malloc(strlen(filename) + 1);
            strcpy(Filename.msg, filename);
            writeSingleString(sock, h, Filename.msg); // trimite-i mai intai numele fisierului
            //h.fileName = malloc(strlen(getFileName(filename)) + 1);
            //strcpy(h.fileName, getFileName(filename));
            msgStringType msg;
            msg.transfer = 0;
            // fprintf(stderr, "You entered %s", filename);
            if ((fd = open(filename, O_RDONLY)) < 0) {
                perror("open");
                goto start;
            }

            // char * outgoing = getContent(fd);
            //  h.msgSize = lseek(fd, 0L, SEEK_END); // marimea fisierului intreg
            ///  lseek(fd, 0L, SEEK_SET); // resetare cursor
            struct stat sb;
            stat(filename, &sb);
            h.msgSize = sb.st_size;
            fprintf(stderr, "Dimensiune fisier:%d\n", h.msgSize);
            //   writeSingleString(sock, h, h.fileName);
            //  h.fileName = getFileName(filename);
            // printf("%s\n", outgoing);
            int msgSize = h.msgSize;
            while (msgSize != 0) {
                if (msgSize < BUF_SIZE) {
                    //   bzero(msg.msg, BUF_SIZE);
                    msg.msg = malloc(msgSize + 1);
                    h.msgSize = read(fd, msg.msg, msgSize);
                    msg.transfer = 0; // am incheiat transfer
                    writeSingleString(sock, h, msg.msg);
                    msgSize = 0; // ultima iteratie
                } else {
                    //   bzero(msg.msg, sizeof(msg.msg));
                    msg.msg = malloc(BUF_SIZE + 1);
                    h.msgSize = read(fd, msg.msg, BUF_SIZE);
                    msg.transfer = 1; // transfer in progress
                    writeSingleString(sock, h, msg.msg);
                    msgSize -= BUF_SIZE;
                }
            }
            close(fd); // inchide fisierul!
            msgStringType msg3;
            // readSingleString(sock, &msg3);
            msgSize = h.msgSize;
            char fname[100]; // salvez local fisierul
            strcpy(fname, getFileUID("INET"));
            //fname[strlen(fname)] = 0;
            fprintf(stderr, "Send file back: filename -- %s\n", fname);
            fd = open(fname, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
            if (fd < 0) {
                perror("on open() -- send file back");
                return 0;
            }
            // scrie in fisier
            int n;
            msgStringType str4;
            /*
            while ((n = readSingleString(sock, &str4)) > 0) {
                write(fd, str4.msg, n);
            } */
            n = readSingleString(sock, &str4);
            if(n > 0){
                write(fd, str4.msg, n);
            }
            system("chmod u+r ./client_files/INET/*");
            /*
             writeSingleString(sock, h, getContent(fd)); // Just for tests, ignore the response!
             writeSingleString(sock, h, getFileName(filename));
             fprintf(stderr,"filename: %s%s\n", getFilePath(filename), getFileName(filename));
             writeSingleString(sock, h, getFilePath(filename));
             readSingleString(sock, &str);   // Just for tests, ignore the response! */
            // mai intai numele fisierului

            //   strcpy(fname, "./client_files/INET/");
            /*
               strcpy(fname, str.msg);
               fname[strlen(fname)] = 0;
               readSingleString(sock, &str);
               fprintf(stderr, "filename and path: %s\n", fname);
               sprintf(fname, "./client_files/INET/log_[%s].txt", getTimestamp());
               int fd = open(fname, S_IRUSR | S_IWUSR | S_IXUSR | O_RDWR | O_CREAT, 0666);
               write(fd, str.msg, strlen(str.msg));
               //   fprintf(stderr, "Got the echo of %s  ==> %s\n", outgoing, str.msg);
               free(str.msg); // Need to free, once it's used! */
        }
        else if(optiune == 2){
            msgStringType  msgopt;
            readSingleString(sock, &msgopt);
            fprintf(stderr, "%s",msgopt.msg);
            scanf("%s", filename);
            h.clientID = clientID;
            h.opID = 2;
            writeSingleString(sock, h, filename);
        }
        else if(optiune == 3){
            msgStringType  msgopt;
            readSingleString(sock, &msgopt);
            fprintf(stderr, "%s",msgopt.msg);
            scanf("%s", filename);
            h.clientID = clientID;
            h.opID = 3;
            writeSingleString(sock, h, filename);
            readSingleString(sock, &msgopt);
            fprintf(stderr, "%s", msgopt.msg); // nume fisier nou!!!!
            scanf("%s", filename);
            writeSingleString(sock, h, filename);
        }
        else if(optiune == 4){
            msgStringType  msgopt;
            readSingleString(sock, &msgopt);
            fprintf(stderr, "%s",msgopt.msg);
            scanf("%s", filename);
            h.clientID = clientID;
            h.opID = 4;
            writeSingleString(sock, h, filename);
        }
        else if(optiune == 5){
            system("ls -lia ./client_files/INET/*");
        }
        else if(optiune == 6){
            msgStringType  msgopt;
            /*readSingleString(sock, &msgopt);
            fprintf(stderr, "%s",msgopt.msg);
            scanf("%s", filename); */
            h.clientID = clientID;
            h.opID = 6;
          //  writeSingleString(sock, h, filename);
            // afisez ce primesc de la server
            int n;
            /*
            while((n = readSingleString(sock, & msgopt)) > 0){
                write(STDOUT_FILENO, msgopt.msg, strlen(msgopt.msg));
            }*/
            readSingleString(sock, & msgopt);
            char serv_files_filename[100];
            strcpy(serv_files_filename, "./client_files/INET/");
            strcat(serv_files_filename, "[");
            strcat(serv_files_filename, getTimestamp());
            strcat(serv_files_filename, "]server_INET_files.txt");
            serv_files_filename[strlen(serv_files_filename)] = 0;
            fd = open(serv_files_filename, O_RDWR | O_CREAT | O_TRUNC);
            write(fd, msgopt.msg, strlen(msgopt.msg));
            close(fd);
            system("chmod u+rwx ./client_files/INET/*");
           // fprintf(stderr, "%s\n", msgopt.msg);
        }
        else if(optiune == 7){
            msgStringType  msgopt;
            avail_languages_menu();
            char opt[2];
            bzero(opt, 2);
            scanf("%s", opt);
            long opts = strtol(opt, NULL, 10);
            writeSingleString(sock, h, avail_programming_languages[opts-1]); // indexarea incepe de la 0
            /*readSingleString(sock, &msgopt);
            fprintf(stderr, "%s",msgopt.msg);
            scanf("%s", filename); */
            h.clientID = clientID;
            h.opID = 7;
            //  writeSingleString(sock, h, filename);
            // afisez ce primesc de la server
            int n;
            /*
            while((n = readSingleString(sock, & msgopt)) > 0){
                write(STDOUT_FILENO, msgopt.msg, strlen(msgopt.msg));
            }*/
            readSingleString(sock, & msgopt);
            char serv_files_filename[100];
            strcpy(serv_files_filename, "./client_files/INET/");
            strcat(serv_files_filename, "[");
            strcat(serv_files_filename, getTimestamp());
            strcat(serv_files_filename, "]server_INET_");
            strcat(serv_files_filename, avail_programming_languages[opts-1]);
            strcat(serv_files_filename, "_files.txt");
            serv_files_filename[strlen(serv_files_filename)] = 0;
            fd = open(serv_files_filename, O_RDWR | O_CREAT | O_TRUNC);
            write(fd, msgopt.msg, strlen(msgopt.msg));
            close(fd);
            system("chmod u+rwx ./client_files/INET/*");
            // fprintf(stderr, "%s\n", msgopt.msg);
        }
        else if(optiune == 8){
           /* msgStringType msgopt;
            msgopt.msg = malloc(3);
            strcpy(msgopt.msg, "8");
            writeSingleString(sock, h, msgopt.msg); // trimite optiunea -- ii trimisa mai sus*/
            msgStringType msgrecv;
            readSingleString(sock, & msgrecv);
            fprintf(stderr, "Toate fisierele ocupa in total %s GB pe server-ul de INET\n", msgrecv.msg);
        }
        else if(optiune == 9){
            // optiunea e trimisa deja
            msgStringType  msg;
            readSingleString(sock, &msg);
            int fd = open("./client_files/INET/largest_INET_sv_file_info.txt", O_RDWR | O_CREAT, O_TRUNC);
            write(fd, msg.msg, strlen(msg.msg));
            close(fd);
            system("chmod u+rwx ./client_files/INET/*");
        }
        else if(optiune == 10){
            // optiunea e trimisa deja
            msgStringType  msg;
            readSingleString(sock, &msg);
            int fd = open("./client_files/INET/smallest_INET_sv_file_info.txt", O_RDWR | O_CREAT, O_TRUNC);
            write(fd, msg.msg, strlen(msg.msg));
            close(fd);
            system("chmod u+rwx ./client_files/INET/*");
        }
        else if(optiune == 11){
            // optiunea e trimisa deja
            msgStringType  msg;
            readSingleString(sock, &msg);
            unlink("./client_files/INET/INET_server_programming_languages_statistics.json");
            int fd = open("./client_files/INET/INET_server_programming_languages_statistics.json", O_RDWR | O_CREAT, O_TRUNC);
            write(fd, msg.msg, strlen(msg.msg)+1);
            close(fd);
            system("chmod u+rwx ./client_files/INET/*");
        }
    }
/* 5. BYE */
  h.clientID = clientID ;
  h.opID = OPR_BYE ;
  fprintf (stderr, "5. An explicit BYE operation...\n") ;
  writeSingleInt (sock, h, 0) ; // Just for tests, ignore the response!

/* NO RESPONSE WAS EXPECTED */


/***
 *
 * "6. Vizualizeaza toate fisierele de la nivel de server",
      7.  "Vizualizeaza programele scrise intr-un anumit limbaj de programare de la nivel de server",
        "8. Afiseaza marimea totala a tuturor fisierelor stocate pe server (INET) [GB]",
        "9. Afiseaza informatii legate de cel mai MARE fisier din toate fisierele stocate pe server (INET)",
        "10. Afiseaza informatii legate de cel mai MIC fisier din toate fisierele stocate pe server (INET)",
        "11. Statistica (limbaj de programare -- numar fisiere)"
 */
  close (sock);
  exit (EXIT_SUCCESS);
}
