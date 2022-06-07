#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "proto.h"
#include "files_util.h"
#define BUF_SIZE 1000000
#define PORT            18081
#define SERVERHOST      "127.0.0.1" 
#define MSGLEN	128

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
    h.opID = OPR_ECHO ;
    while(1) {
        start: bzero(filename, sizeof(filename));
        h.clientID = clientID;
        h.opID = OPR_ECHO;
        fprintf(stderr, "Enter the file name...> ");
        scanf("%s", filename);
        //h.fileName = malloc(strlen(getFileName(filename)) + 1);
        //strcpy(h.fileName, getFileName(filename));
        msgStringType msg;
        msg.transfer = 0;
        // fprintf(stderr, "You entered %s", filename);
        if((fd = open(filename, O_RDONLY)) < 0){
            perror("open");
            goto start;
        }

        // char * outgoing = getContent(fd);
        //  h.msgSize = lseek(fd, 0L, SEEK_END); // marimea fisierului intreg
        ///  lseek(fd, 0L, SEEK_SET); // resetare cursor
        struct stat sb;
        stat(filename, &sb);
        h.msgSize = sb.st_size;
        fprintf(stderr,"Dimensiune fisier:%d\n", h.msgSize);
        //   writeSingleString(sock, h, h.fileName);
        //  h.fileName = getFileName(filename);
        // printf("%s\n", outgoing);
        int msgSize = h.msgSize;
        while(msgSize != 0){
            if(msgSize < BUF_SIZE){
                //   bzero(msg.msg, BUF_SIZE);
                msg.msg = malloc(msgSize + 1);
                h.msgSize = read(fd, msg.msg, msgSize);
                msg.transfer = 0; // am incheiat transfer
                writeSingleString(sock, h, msg.msg);
                msgSize = 0; // ultima iteratie
            }
            else{
                //   bzero(msg.msg, sizeof(msg.msg));
                msg.msg = malloc(BUF_SIZE  + 1);
                h.msgSize = read(fd, msg.msg, BUF_SIZE);
                msg.transfer = 1; // transfer in progress
                writeSingleString(sock, h, msg.msg);
                msgSize -= BUF_SIZE;
            }
        }

   /*
    writeSingleString(sock, h, getContent(fd)); // Just for tests, ignore the response!
    writeSingleString(sock, h, getFileName(filename));
    fprintf(stderr,"filename: %s%s\n", getFilePath(filename), getFileName(filename));
    writeSingleString(sock, h, getFilePath(filename));
    readSingleString(sock, &str);   // Just for tests, ignore the response! */
    // mai intai numele fisierului

    char fname[100];
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
/* 5. BYE */
  h.clientID = clientID ;
  h.opID = OPR_BYE ;
  fprintf (stderr, "5. An explicit BYE operation...\n") ;
  writeSingleInt (sock, h, 0) ; // Just for tests, ignore the response!

/* NO RESPONSE WAS EXPECTED */

  close (sock);
  exit (EXIT_SUCCESS);
}
