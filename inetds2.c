#include <pthread.h>
#include "headerr.h"
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stddef.h>
#include <sys/socket.h>
#include <math.h>
#include <sys/un.h>
#include <dirent.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include "files_util.h"
#include <arpa/inet.h>
#include "proto.h"
extern int client_UIDS[10000];
extern int client_no;
#define BUF_SIZE 1000000



char * opt_echo(int sock, msgHeaderType h, char * filename, char * filepath){
    // determina-i extensia
    //fprintf(stderr,"am ajusn in opt echo\n");
    char * tempfname = malloc(sizeof(filename) + 1);
    strcpy(tempfname, filename);
    char * p = strtok(filename, ".");
    p = strtok(NULL, ".");
    char * logfname = malloc(500);
    strcpy(logfname, "./client_files/INET/");
    strcat(logfname, tempfname);
    // strcat(logfname, ".txt");
    logfname[strlen(logfname)] = 0;
    fprintf(stderr, "log: %s\n", logfname);
    char * command = malloc(200);
    // unlink(logfname); // if exists
    int fdlog = open(logfname, O_CREAT | O_RDWR | S_IRUSR | S_IWUSR | S_IXUSR, 0666);
    if(fdlog < 0){
        perror("open() -- fdlog");
        return NULL;
    }
    if(p == NULL){
        write(fdlog, "Eroare! Fisierul n-are extensie, nu-l pot compila.",
              strlen("Eroare! Fisierul n-are extensie, nu-l pot compila.")
        );
        return NULL;
    }
    pid_t pr;
    switch(fork()) {
        // vad care-i extensia
        case -1:
            perror("eroare fork()");
            return NULL;
        case 0:
            dup2(fdlog, STDOUT_FILENO);
            int fdlog2 = dup(fdlog);
            dup2(fdlog2, STDERR_FILENO);
            if (strcmp(p, "py") == 0) {
                //     strcpy(command, "ipython ");
                strcpy(command, filepath);
                strcat(command, "/");
                strcat(command, tempfname);
                //   fprintf(stderr, "%s\n", command);
                //      strcat(command, " > ");
                //    strcat(command, logfname);
                close(logfname); // inchide fisierul ca sa functioneze dup2 in exec
                execl("/usr/bin/python3", "/usr/bin/python3", command, NULL);
                //   perror("execl()");
            }
            else if(strcmp(p, "c") == 0){
                //    strcpy(command, "-o ");
                char * tempfname2 = malloc(sizeof(tempfname) + 1);
                strcpy(tempfname2, tempfname);
                char * p = strtok(tempfname, "."); // outfile
                strcpy(command, "gcc -o ");
                //    strcat(command, filepath);
                //  strcat(command, "/");
                strcat(command, p); // gcc -o signal.c signal
                strcat(command, " ");
                strcat(command, filepath);
                strcat(command, "/");
                strcat(command, tempfname2);
                strcat(command, " && ./");
                strcat(command, p);
                command[strlen(command)] = 0;
                fprintf(stderr,"Command: %s\n", command); //asta-i numele si calea fisierului de executat
                //  strcat(command, tempfname);
                system(command);
                //    execlp("gcc", "-o", p, command, NULL);
                //   perror("execl()");
            }
            else if(strcmp(p, "java") == 0){
                //   strcpy(command, "javac ");
                strcpy(command, tempfname);
                execl("/bin/javac", "/bin/javac", command, NULL);
                //     perror("execl()");
            }
            break;
        default:
            wait(NULL); // asteapta dupa fiu
    }
    int fd = open(logfname, O_RDWR | O_CREAT);
    if(fd < 0){
        perror("open");
    }
    msgStringType msg;
    long msgSize;
    struct stat sb;
    if(stat(logfname, &sb) == -1){ // nr total de octeti
        perror("on stat() -- log file");
        return NULL;
    }
    //  perror("stat -- opr echo");
    // trimite-i numele fisierului inapoi
    //msg.msg = malloc(strlen(getFileName(logfname)) + 1);
    //strcpy(msg.msg, getFileName(logfname));
    //  writeSingleString(sock, * h, msg.msg);


    msgSize = (long) sb.st_size;
    /*
    while(msgSize > 0){
        if(msgSize < BUF_SIZE){
            //   bzero(msg.msg, BUF_SIZE);
            msg.msg = malloc(msgSize + 1);
            h->msgSize = read(fd, msg.msg, msgSize);
            msg.transfer = 0; // am incheiat transfer
            writeSingleString(sock, * h, msg.msg);
            msgSize = 0; // ultima iteratie
        }
        else{
            //   bzero(msg.msg, sizeof(msg.msg));
            msg.msg = malloc(BUF_SIZE  + 1);
            h->msgSize = read(fd, msg.msg, BUF_SIZE);
            msg.transfer = 1; // transfer in progress
            writeSingleString(sock, * h, msg.msg);
            msgSize -= BUF_SIZE;
        }
    } */
    msg.msg = malloc(msgSize);
    read(fd, msg.msg, msgSize);
    writeSingleString(sock, h, msg.msg);
    unlink(logfname);
    system("chmod u+r ./serv_files/INET/*");
    // return buf;
}

int inet_socket (uint16_t port, short reuse) {
    int sock;
    struct sockaddr_in name;

    /* Create the socket. */
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
//    perror ("socket");
        pthread_exit (NULL);
    }

    if (reuse) {
        int reuseAddrON = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseAddrON, sizeof(reuseAddrON)) < 0) {
            perror("setsockopt(SO_REUSEADDR) failed");
            pthread_exit (NULL);
        }
    }

    /* Give the socket a name. */
    name.sin_family = AF_INET;
    name.sin_port = htons (port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);
    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0) {
        perror ("bind");
        pthread_exit (NULL);
    }

    return sock;
}

int get_client_id (int sock) {
    int size ;
    long clientid ;
    size = recv (sock, &clientid, sizeof (clientid), 0) ;
    fprintf (stderr, "gcid:\tReceived %ld\n", clientid) ;

    if (size == -1) return -1 ;

    return clientid ;
}



int write_client_int (int sock, long id) {
    /* DUMMY FUNCTION FOR SENDING CLIENT ID */
    int size ;
    fprintf (stderr, "wcl:\tWriting %ld\n", id) ;
    if ((size = send (sock, &id, sizeof (id), 0)) < 0) return -1 ;
    return EXIT_SUCCESS ;
}

int write_client_id (int sock, long id) {
    return write_client_int (sock, id) ;
}

int write_client_concat (int sock, char *o1, char *o2) {
    /* DUMMY IMPLEMENTATION OF CONCATENATE FUNCTION  */
    int size, bsize ;
    char *b ;

    bsize = strlen (o1) + strlen (o2)+2 ;
    b = malloc (256) ;
    sprintf (b, "%s %s", o2, o1) ; b [bsize] = 0 ;
    if ((size = send (sock, b, bsize, 0)) < 0) return -1 ;

    free (b) ;

    return EXIT_SUCCESS ;
}

char *get_client_str (int sock,  int *dsize) {
    /* DUMMY FUNCTION FOR GETTING CLIENT DATA
      Notice that FULL CLIENT DATA could come: e.g
      OPERATION^@DATA1^@DATA2^@ */
    char buffer[256] ;
    char *result = NULL;
    int isize = 0, size, i = 0 ;
    while (((size = recv (sock, &buffer, 256, 0)) >= 0) ) {
        result = realloc (result, isize+size) ;
        memcpy (result+isize, buffer, size) ; i++ ;
        isize += size ; result [isize] = 0;
        if (size != 256) break ;
    }
    if (isize < 0) return NULL ;
    fprintf (stderr, "gcs:\tGot %s\n", result) ;
    *dsize = isize ;
    /* Notice: use smarter mem allocation here ;) */
    return result ;
}

int create_client_id () {
    /* Create some unique ID. e.g. UNIX timestamp... */
    char ctsmp [12] ;
    time_t rawtime;
    struct tm * timeinfo;
    int uuid ;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime (ctsmp, 12, "%s", timeinfo) ;

    uuid = atoi (ctsmp) ;
    /* NOTICE: YOU NEED TO STORE THIS INFORMATION AT SERVER LEVEL !*/
    return uuid ;
}

int extract_client_operation (char *data) {


    return -1 ;
}

/*
void closewin (WINDOW *wnd) {
  if (wnd != NULL) delwin (wnd) ;
  wnd = NULL ;
  refresh () ;
}
*/

fd_set active_fd_set, read_fd_set;

void *inet_main (void *args) {
    int port = *((int *)args) ;
    int sock ;
    size_t size;
    struct sockaddr_in clientname;

    if ((sock = inet_socket (port, 1)) < 0) {
        pthread_exit (NULL);
    }
    if (listen (sock, 1) < 0) {
        pthread_exit (NULL);
    }
    /* Initialize the set of active sockets. */
    FD_ZERO (&active_fd_set);
    FD_SET (sock, &active_fd_set);

    while (1) {
        int i ;
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
            pthread_exit (NULL);
        }
        DIR * dptr;
        struct dirent *sdir;
        long size = 0;
        char str[100];
        /* Service all the sockets with input pending. */
        for (i = 0; i < FD_SETSIZE; ++i)
            if (FD_ISSET (i, &read_fd_set)) {
                if (i == sock) { /* Connection request on original socket. */
                    int new;
                    size = sizeof (clientname);
                    new = accept (sock, (struct sockaddr *) &clientname,
                                  (socklen_t *) &size);
                    if (new < 0) {
                        pthread_exit (NULL);
                    }

                    FD_SET (new, &active_fd_set);
                }
                else { /* Data arriving on an already-connected socket. */
                    int clientID ;
                    /* Protocolul simplu utilizat:
                        Se verifica intai header-ul.
                        Pe baza informatiei din header, se decide operatia de urmat!

                    */
                    msgHeaderType h = peekMsgHeader (i) ;
                    msgStringType filename;
                    pid_t p;
                    //sleep(5);
                    if ((clientID = h.clientID) < 0) {
                        // Protocol error: missing client ID. Close connection
                        fprintf (stderr, "There's something wrong! Negative ClientID.\t Closing connection, probably the client was terminated.\n") ;
                        close (i); FD_CLR (i, &active_fd_set);
                    } else {
                        if (clientID == 0) {
                            int newID ;
                            msgIntType m ;
                            newID = create_client_id () ;
                            fprintf (stderr, "\tDetected new client! New clientID: %d\n", newID ) ;
                            client_UIDS[client_no ++] = newID;
                            if (readSingleInt (i, &m)<0) {
                                // Cannot read from client. This is impossible :) Close connection!
                                //  perror("eroare readsingleint");
                                close (i) ; FD_CLR (i, &active_fd_set) ;
                            }
                            //   fprintf(stderr, "m int=%d\n", m.msg);
                            if (writeSingleInt (i, h, newID) < 0) {
                                // Cannot write to client. Close connection!
                                // perror("eroare writesingleint");
                                close (i) ; FD_CLR (i, &active_fd_set) ;
                            }
                        } else { /* Already identified. Existing client... communication continues */
                            /* YOU SHOULD CHECK IF THIS IS AN EXISTING CLIENT !!! */
                            int operation, dsize ;
                            char *opr1, *opr2;
                            int num1, num2 ;
                            char *opr ;
                            operation = h.opID ;
                            if (operation == -1) { // Protocol error: missing or incorect operation
                                close (i); FD_CLR (i, &active_fd_set);
                            }
                            char command[500];
                            /// citeste mai intai optiunea
                            msgStringType stropt;
                            readSingleString(i, & stropt);
                            long opt = strtol(stropt.msg, NULL, 10);
                            // long opt = h.opID;
                            fprintf(stderr, "Am primit opt %ld\n", opt);
                            /* if(opt == 1){
                                 operation = OPR_ECHO;
                             }
                             else if(opt == 2){

                             } */
                            int fd;
                            long maxsize = 0;
                            char maxfile[100];
                            char filecontent[500];
                            msgStringType mopt;
                            int lbs[5]={0}; // C -- 0, C++ -- 1, JAVA -- 2, PY -- 3, Oth -- 4
                            switch (opt) {
                                case 1:
                                {

                                    msgStringType str, str2, str3;
                                    // readSingleString(i, &str2);
                                    char * buf = (char*) malloc(BUF_SIZE + 1);
                                    char path[100];
                                    readSingleString(i, & str2); // calea fisierului trimisa
                                    strcpy(path, "serv_files/INET/"); //just for tests!!!!
                                    strcat(path, getFileName(str2.msg)); // aici am path-ul fisierului de salvat pe server
                                    path[strlen(path)] = 0;
                                    fprintf(stderr,"Path received:%s\n", path);
                                    //  strcat(path, str2.msg);
                                    //    path[strlen(path)] = 0;
                                    //unlink(path);
                                    fd = open(path, O_CREAT | O_RDWR);
                                    if(fd < 0){perror("open"); close(i); FD_CLR(i, & active_fd_set);
                                    }
                                    int n = readSingleString(i, &str); // continutul fisierului transferat
                                    if(n < 0){
                                        // Cannot write to client. Close connection!
                                        close (i) ; FD_CLR (i, &active_fd_set) ;
                                    }
                                    write(fd, str.msg, n);
                                    opt_echo(i, h, getFileName(path), getFilePath(path));
                                    // fprintf(stderr, "%s\n", h.fileName);
                                    /*      if (readSingleString (i, &str) < 0) {
                                                 // Cannot write to client. Close connection!
                                                 close (i) ; FD_CLR (i, &active_fd_set) ;
                                              }
                                               if (readSingleString (i, &str2) < 0) {
                                                   // Cannot write to client. Close connection!
                                                   close (i) ; FD_CLR (i, &active_fd_set) ;
                                               }
                                               if (readSingleString (i, &str3) < 0) {
                                                   // Cannot write to client. Close connection!
                                                   close (i) ; FD_CLR (i, &active_fd_set) ;
                                               } */
                                    //// asta-i continutul fisierului
                                    // 	fprintf (stderr, "An echo value was received: %s\n", str.msg) ;
                                    ///// asta-i numele fisierului
                                    //  fprintf(stderr, "Filename is %s%s\n", str3.msg, str2.msg);
                                    // calea fisierului este str3.msg
                                    //   char * logcontent = opt_echo(str2.msg, str3.msg);
                                    // strcpy(str.msg, logcontent);
                                    // if (writeSingleString (i, h, str.msg) < 0) {
                                    //   // Cannot write to client. Close connection!
                                    //  close (i) ; FD_CLR (i, &active_fd_set) ;
                                    //}
                                    //    int fd = open(log, O_RDONLY | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
                                    //  if(fd < 0){perror("open() --");}
                                    //  char bf[10000];
                                    // bzero(bf, 10000);
                                    // read(fd, bf, 10000);
                                    //bf[strlen(bf)] = 0;
                                    /*
                                    str.msg = malloc(strlen(bf)+1);
                                    strcpy(str.msg, bf);
                                    unlink(log);//sterge fisier temporar
                                      if (writeSingleString (i, h, str.msg) < 0) {
                                     // Cannot write to client. Close connection!
                                   close (i) ; FD_CLR (i, &active_fd_set) ;
                                 } */

                                    //	fprintf (stderr, "An echo value was sent back: %s\n", str.msg) ;
                                    free (str.msg) ; // Need to free, once is used!
                                }
                                    break ;
                                case 2: // creeaza pe server fisierul primit
                                    // citeste numele fisierului
                                start2:
                                    writeSingleString(i, h, "Introduceti numele fisierului...>");
                                    readSingleString(i, &filename);
                                    char * actual_path = malloc(500);
                                    strcpy(actual_path, "./serv_files/INET/");
                                    strcat(actual_path, getFileName(filename.msg));
                                    actual_path[strlen(actual_path)] = 0;
                                    int fd;
                                    if((fd = open(actual_path, O_RDWR | O_CREAT, O_TRUNC)) < 0){
                                        perror("Couldn't create new blank file on server");
                                        goto start2;
                                    }
                                    system("chmod u+rwx ./serv_files/INET/*");
                                    break ;
                                case 3: // modifica nume fisier
                                {
                                    // citeste numele fisierului
                                    start3:
                                    writeSingleString(i, h, "Introduceti numele fisierului deja existent...>");
                                    readSingleString(i, &filename);
                                    char * actual_path = malloc(500);
                                    strcpy(actual_path, "./serv_files/INET/");
                                    strcat(actual_path, getFileName(filename.msg));
                                    actual_path[strlen(actual_path)] = 0;
                                    bzero(command, 500);
                                    strcpy(command, "mv ");
                                    strcat(command, actual_path);
                                    writeSingleString(i, h, "Introduceti noua denumire a fisierului dat...>");
                                    readSingleString(i, &filename);
                                    strcat(command, " ");
                                    strcat(command, getFilePath(actual_path));
                                    strcat(command, "/");
                                    strcat(command, getFileName(filename.msg));
                                    command[strlen(command)] = 0;
                                    fprintf(stderr, "%s\n", command);
                                    system(command);
                                }
                                    break ;
                                case 4: // sterge fisier
                                {
                                    // citeste numele fisierului
                                    start4:
                                    writeSingleString(i, h, "Introduceti numele fisierului...>");
                                    readSingleString(i, &filename);
                                    char * actual_path = malloc(500);
                                    strcpy(actual_path, "./serv_files/INET/");
                                    strcat(actual_path, getFileName(filename.msg));
                                    actual_path[strlen(actual_path)] = 0;
                                    if(unlink(actual_path) == -1){
                                        perror("Couldn't delete file from server");
                                        goto start4;
                                    }
                                }
                                    break ;
                                case 5:
                                    break; // nothing todo here
                                case 6:
                                start6:
                                    unlink("temp.txt");
                                    creat("temp.txt", O_RDWR | S_IRUSR | S_IWUSR | S_IXUSR);
                                    fd = open("temp.txt", O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
                                    if(fd < 0){
                                        perror("tempfile -- ls 1 -- server");
                                        goto start6;
                                    }
                                    switch(p = fork()) {
                                        case 0: //child
                                            dup2(fd, STDOUT_FILENO);
                                            close(fd);//inchide fisierul ca sa mearga dup2 in system
                                            system("ls -lia ./serv_files/INET");
                                            break;
                                        default: //parent
                                            wait(NULL);
                                            break;
                                    }
                                    fd = open("temp.txt", O_RDONLY);
                                    if(fd < 0){
                                        perror("tempfile -- ls -- server");
                                        goto start6;
                                    }
                                    msgStringType  msg;
                                    int msgSize;
                                    struct stat sb;
                                    stat("temp.txt", &sb);
                                    msgSize = (int) sb.st_size;
                                    fprintf(stderr, "msgSize is %d\n", msgSize);
                                    while (msgSize != 0) {
                                        if (msgSize < BUF_SIZE) {
                                            //   bzero(msg.msg, BUF_SIZE);
                                            msg.msg = malloc(msgSize + 1);
                                            h.msgSize = read(fd, msg.msg, msgSize);
                                            msg.transfer = 0; // am incheiat transfer
                                            writeSingleString(i, h, msg.msg);
                                            //     fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize = 0; // ultima iteratie
                                        } else {
                                            //   bzero(msg.msg, sizeof(msg.msg));
                                            msg.msg = malloc(BUF_SIZE + 1);
                                            h.msgSize = read(fd, msg.msg, BUF_SIZE);
                                            msg.transfer = 1; // transfer in progress
                                            writeSingleString(i, h, msg.msg);
                                            // fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize -= BUF_SIZE;
                                        }
                                    }
                                    unlink("temp.txt");
                                    break;
                                case 7:
                                start7:
                                    readSingleString(i, &mopt);
                                    unlink("temp.txt");
                                    creat("temp.txt", O_RDWR | S_IRUSR | S_IWUSR | S_IXUSR);
                                    fd = open("temp.txt", O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
                                    if(fd < 0){
                                        perror("tempfile -- ls 1 -- server");
                                        goto start7;
                                    }
                                    strcpy(command, "");
                                    strcat(command, "ls -lia ./serv_files/INET/*.");
                                    strcat(command, mopt.msg);
                                    command[strlen(command)] = 0;
                                    switch(p = fork()) {
                                        case 0: //child
                                            dup2(fd, STDOUT_FILENO);
                                            close(fd);//inchide fisierul ca sa mearga dup2 in system
                                            system(command);
                                            break;
                                        default: //parent
                                            wait(NULL);
                                            break;
                                    }
                                    fd = open("temp.txt", O_RDONLY);
                                    if(fd < 0){
                                        perror("tempfile -- ls -- server");
                                        goto start6;
                                    }
                                    //   msgStringType  msg;
                                    // int msgSize;
                                    //  struct stat sb;
                                    stat("temp.txt", &sb);
                                    msgSize = (int) sb.st_size;
                                    fprintf(stderr, "msgSize is %d\n", msgSize);
                                    while (msgSize != 0) {
                                        if (msgSize < BUF_SIZE) {
                                            //   bzero(msg.msg, BUF_SIZE);
                                            msg.msg = malloc(msgSize + 1);
                                            h.msgSize = read(fd, msg.msg, msgSize);
                                            msg.transfer = 0; // am incheiat transfer
                                            writeSingleString(i, h, msg.msg);
                                            //     fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize = 0; // ultima iteratie
                                        } else {
                                            //   bzero(msg.msg, sizeof(msg.msg));
                                            msg.msg = malloc(BUF_SIZE + 1);
                                            h.msgSize = read(fd, msg.msg, BUF_SIZE);
                                            msg.transfer = 1; // transfer in progress
                                            writeSingleString(i, h, msg.msg);
                                            // fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize -= BUF_SIZE;
                                        }
                                    }
                                    if(msgSize == 0){
                                        msg.msg = (char*) malloc(100);
                                        sprintf(msg.msg, "Nu exista fisiere cu extensia .%s pe server!", mopt.msg);
                                        writeSingleString(i, h, msg.msg);
                                    }
                                    unlink("temp.txt");
                                    break;
                                case 8:
                                    // calculez suma totala a fisierelor in GB de pe tot server-ul INET
                                    //struct stat sb;
                                    dptr = opendir("./serv_files/INET");
                                    while((sdir = readdir(dptr)) != NULL){
                                        if(sdir->d_type == 4){
                                            if(sdir->d_name[0] == '.'){
                                                // . sau ..
                                                stat(sdir->d_name,&sb);
                                                //size=buf.st_size;
                                                //  pf("size=%d\n",size);
                                            }
                                        }
                                        else
                                        {
                                            strcpy(str,"./serv_files/INET");
                                            strcat(str,"/");
                                            strcat(str,sdir->d_name);
                                            fprintf(stderr, "%s\n", str);
                                            stat(str,&sb);
                                            size+=sb.st_size;
                                        }
                                    }
                                    double res = 9.31 * pow(10, -9) * size;
                                    char result[30];
                                    sprintf(result, "%.15lf", res);
                                    fprintf(stderr, "%s GB\n", result);
                                    result[strlen(result)] = 0;
                                    writeSingleString(i, h, result); // trimite-i inapoi rezultatul
                                    break;
                                case 9: // informatii legate de cel mai mare fisier, aici avem transfer de fisiere
                                    // optiunea e trimisa deja, caut cel mai mare fisier
                                    dptr = opendir("./serv_files/INET");
                                    while((sdir = readdir(dptr)) != NULL){
                                        if(sdir->d_type == 4){
                                            if(sdir->d_name[0] == '.'){
                                                // . sau ..
                                                stat(sdir->d_name,&sb);
                                                //size=buf.st_size;
                                                //  pf("size=%d\n",size);
                                            }
                                        }
                                        else
                                        {
                                            strcpy(str,"./serv_files/INET");
                                            strcat(str,"/");
                                            strcat(str,sdir->d_name);
                                            //    fprintf(stderr, "%s\n", str);
                                            stat(str,&sb); // dimensiunea fisierului
                                            if(sb.st_size > maxsize) {
                                                maxsize = sb.st_size;
                                                strcpy(maxfile, str); // ii salvez si calea
                                            }
                                        }
                                    }
                                    // acum afisez informatiile legate de fisier
                                    switch(p = fork()){
                                        case 0: // child
                                            creat("temp.txt", O_RDWR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // creeaza fisierul
                                            fd = open("temp.txt", O_RDWR);
                                            dup2(fd, STDOUT_FILENO);
                                            // close(fd); //ca sa mearga dup2 in system
                                            fprintf(stdout, "\t\t[Informatii generale legate de fisierul %s]\n", maxfile);
                                            fprintf(stdout, "\t\t");
                                            for(int i=0;i<sizeof("[Informatii generale legate de fisierul ") + sizeof(maxfile) + 1; i++)
                                                fprintf(stdout, "=");
                                            fprintf(stdout,"\n");
                                            strcpy(command, "ls -lia ");
                                            strcat(command,maxfile);
                                            system(command); // afiseaza informatii generale
                                            fprintf(stdout,"\n");
                                            fprintf(stdout, "\t\t[Informatii legate de continuturile din iNOD pentru fisierul %s]\n", maxfile);
                                            fprintf(stdout, "\t\t");
                                            for(int i=0;i<sizeof("[Informatii legate de continuturile din iNOD pentru fisierul ") + sizeof(maxfile) + 1; i++)
                                                fprintf(stdout, "=");
                                            fprintf(stdout,"\n");
                                            strcpy(command, "stat -s ");
                                            strcat(command,maxfile);
                                            system(command); // afiseaza informatii legate de continuturile din INOD
                                            fprintf(stdout,"\n");
                                            fprintf(stdout, "\t\t[Continutul fisierului %s]\n", maxfile);
                                            fprintf(stdout, "\t\t");
                                            for(int i=0;i<sizeof("[Continutul fisierului ") + sizeof(maxfile) + 1; i++)
                                                fprintf(stdout, "=");
                                            fprintf(stdout,"\n");
                                            strcpy(command, "cat -b ");
                                            strcat(command, maxfile);
                                            system(command); // afiseaza-i continutul
                                            fprintf(stdout,"\n");
                                        default:
                                            wait(NULL);
                                            break;
                                    }
                                    close(fd); // inchide fisierul
                                    // acum citeste din fisier si trimite inapoi la client
                                    fd = open("temp.txt", O_RDWR);
                                    stat("temp.txt", &sb);
                                    msgSize = (int) sb.st_size;
                                    fprintf(stderr, "msgSize is %d\n", msgSize);
                                    while (msgSize != 0) {
                                        if (msgSize < BUF_SIZE) {
                                            //   bzero(msg.msg, BUF_SIZE);
                                            msg.msg = malloc(msgSize + 1);
                                            h.msgSize = read(fd, msg.msg, msgSize);
                                            msg.transfer = 0; // am incheiat transfer
                                            writeSingleString(i, h, msg.msg);
                                            //     fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize = 0; // ultima iteratie
                                        } else {
                                            //   bzero(msg.msg, sizeof(msg.msg));
                                            msg.msg = malloc(BUF_SIZE + 1);
                                            h.msgSize = read(fd, msg.msg, BUF_SIZE);
                                            msg.transfer = 1; // transfer in progress
                                            writeSingleString(i, h, msg.msg);
                                            // fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize -= BUF_SIZE;
                                        }
                                    }
                                    if(msgSize == 0){
                                        msg.msg = (char*) malloc(100);
                                        strcpy(msg.msg, "Nu exista fisiere pe server-ul INET!");
                                        writeSingleString(i, h, msg.msg);
                                    }
                                    unlink("temp.txt");  // sterge fisierul temporar, nu mai avem nevoie de el
                                    break;
                                case 10:
                                    // optiunea e trimisa deja, caut cel mai mare fisier
                                    maxsize = 9223372036854775807; //valoarea max pt longs
                                    dptr = opendir("./serv_files/INET");
                                    while((sdir = readdir(dptr)) != NULL){
                                        if(sdir->d_type == 4){
                                            if(sdir->d_name[0] == '.'){
                                                // . sau ..
                                                stat(sdir->d_name,&sb);
                                                //size=buf.st_size;
                                                //  pf("size=%d\n",size);
                                            }
                                        }
                                        else
                                        {
                                            strcpy(str,"./serv_files/INET");
                                            strcat(str,"/");
                                            strcat(str,sdir->d_name);
                                            //    fprintf(stderr, "%s\n", str);
                                            stat(str,&sb); // dimensiunea fisierului
                                            if(sb.st_size < maxsize && sb.st_size > 0) { // sa nu fie fisier gol
                                                maxsize = sb.st_size;
                                                strcpy(maxfile, str); // ii salvez si calea
                                            }
                                        }
                                    }
                                    // acum afisez informatiile legate de fisier
                                    switch(p = fork()){
                                        case 0: // child
                                            creat("temp.txt", O_RDWR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // creeaza fisierul
                                            fd = open("temp.txt", O_RDWR);
                                            dup2(fd, STDOUT_FILENO);
                                            // close(fd); //ca sa mearga dup2 in system
                                            fprintf(stdout, "\t\t[Informatii generale legate de fisierul %s]\n", maxfile);
                                            fprintf(stdout, "\t\t");
                                            for(int i=0;i<sizeof("[Informatii generale legate de fisierul ") + sizeof(maxfile) + 1; i++)
                                                fprintf(stdout, "=");
                                            fprintf(stdout,"\n");
                                            strcpy(command, "ls -lia ");
                                            strcat(command,maxfile);
                                            system(command); // afiseaza informatii generale
                                            fprintf(stdout,"\n");
                                            fprintf(stdout, "\t\t[Informatii legate de continuturile din iNOD pentru fisierul %s]\n", maxfile);
                                            fprintf(stdout, "\t\t");
                                            for(int i=0;i<sizeof("[Informatii legate de continuturile din iNOD pentru fisierul ") + sizeof(maxfile) + 1; i++)
                                                fprintf(stdout, "=");
                                            fprintf(stdout,"\n");
                                            strcpy(command, "stat -s ");
                                            strcat(command,maxfile);
                                            system(command); // afiseaza informatii legate de continuturile din INOD
                                            fprintf(stdout,"\n");
                                            fprintf(stdout, "\t\t[Continutul fisierului %s]\n", maxfile);
                                            fprintf(stdout, "\t\t");
                                            for(int i=0;i<sizeof("[Continutul fisierului ") + sizeof(maxfile) + 1; i++)
                                                fprintf(stdout, "=");
                                            fprintf(stdout,"\n");
                                            strcpy(command, "cat -b ");
                                            strcat(command, maxfile);
                                            system(command); // afiseaza-i continutul
                                            fprintf(stdout,"\n");
                                        default:
                                            wait(NULL);
                                            break;
                                    }
                                    close(fd); // inchide fisierul
                                    // acum citeste din fisier si trimite inapoi la client
                                    fd = open("temp.txt", O_RDWR);
                                    stat("temp.txt", &sb);
                                    msgSize = (int) sb.st_size;
                                    fprintf(stderr, "msgSize is %d\n", msgSize);
                                    while (msgSize != 0) {
                                        if (msgSize < BUF_SIZE) {
                                            //   bzero(msg.msg, BUF_SIZE);
                                            msg.msg = malloc(msgSize + 1);
                                            h.msgSize = read(fd, msg.msg, msgSize);
                                            msg.transfer = 0; // am incheiat transfer
                                            writeSingleString(i, h, msg.msg);
                                            //     fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize = 0; // ultima iteratie
                                        } else {
                                            //   bzero(msg.msg, sizeof(msg.msg));
                                            msg.msg = malloc(BUF_SIZE + 1);
                                            h.msgSize = read(fd, msg.msg, BUF_SIZE);
                                            msg.transfer = 1; // transfer in progress
                                            writeSingleString(i, h, msg.msg);
                                            // fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize -= BUF_SIZE;
                                        }
                                    }
                                    if(msgSize == 0){
                                        msg.msg = (char*) malloc(100);
                                        strcpy(msg.msg, "Nu exista fisiere pe server-ul INET!");
                                        writeSingleString(i, h, msg.msg);
                                    }
                                    unlink("temp.txt");  // sterge fisierul temporar, nu mai avem nevoie de el
                                    break;
                                case 11: // statistica lb de programare -- nr de fisiere
                                    dptr = opendir("./serv_files/INET");
                                    while((sdir = readdir(dptr)) != NULL){
                                        if(sdir->d_type == 4){
                                            if(sdir->d_name[0] == '.'){
                                                // . sau ..
                                                stat(sdir->d_name,&sb);
                                                //size=buf.st_size;
                                                //  pf("size=%d\n",size);
                                            }
                                        }
                                        else
                                        {
                                            strcpy(str,"./serv_files/INET");
                                            strcat(str,"/");
                                            strcat(str,sdir->d_name);
                                            //    fprintf(stderr, "%s\n", str);
                                            if(strstr(str, ".c") && str[strlen(str)-1] == 'c'){ // .C
                                                lbs[0] ++ ;
                                            }
                                            else if(strstr(str, ".cpp")){ // .CPP
                                                lbs[1] ++ ;
                                            }
                                            else if(strstr(str, ".java")){ // .JAVA
                                                lbs[2] ++ ;
                                            }
                                            else if(strstr(str, ".py")) { // python
                                                lbs[3] ++ ;
                                            }
                                            else lbs[4] ++ ; // unknown language ?
                                        }
                                    }
                                    // acum afisez informatiile legate de fisier
                                    switch(p = fork()){
                                        case 0: // child
                                            creat("temp.txt", O_RDWR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // creeaza fisierul
                                            fd = open("temp.txt", O_RDWR);
                                            dup2(fd, STDOUT_FILENO);
                                            sprintf(filecontent, "{\n\t\t'C':%d,\n"
                                                                 "\t\t'C++':%d,\n"
                                                                 "\t\t'Java':%d,\n"
                                                                 "\t\t'Python':%d,\n"
                                                                 "\t\t'others':%d\n"
                                                                 "}",
                                                    lbs[0], lbs[1], lbs[2], lbs[3], lbs[4]);
                                            //    strcat(filecontent, "}");
                                            fprintf(stdout, "%s\n", filecontent);
                                        default:
                                            wait(NULL);
                                            break;
                                    }
                                    close(fd); // inchide fisierul
                                    // acum citeste din fisier si trimite inapoi la client
                                    fd = open("temp.txt", O_RDWR);
                                    stat("temp.txt", &sb);
                                    msgSize = (int) sb.st_size;
                                    fprintf(stderr, "msgSize is %d\n", msgSize);
                                    while (msgSize != 0) {
                                        if (msgSize < BUF_SIZE) {
                                            //   bzero(msg.msg, BUF_SIZE);
                                            msg.msg = malloc(msgSize + 1);
                                            h.msgSize = read(fd, msg.msg, msgSize);
                                            msg.transfer = 0; // am incheiat transfer
                                            writeSingleString(i, h, msg.msg);
                                            //     fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize = 0; // ultima iteratie
                                        } else {
                                            //   bzero(msg.msg, sizeof(msg.msg));
                                            msg.msg = malloc(BUF_SIZE + 1);
                                            h.msgSize = read(fd, msg.msg, BUF_SIZE);
                                            msg.transfer = 1; // transfer in progress
                                            writeSingleString(i, h, msg.msg);
                                            // fprintf(stderr, "%s\n", msg.msg); de test
                                            msgSize -= BUF_SIZE;
                                        }
                                    }
                                    if(msgSize == 0){
                                        msg.msg = (char*) malloc(100);
                                        strcpy(msg.msg, "Nu exista fisiere pe server-ul INET!");
                                        writeSingleString(i, h, msg.msg);
                                    }
                                    unlink("temp.txt");  // sterge fisierul temporar, nu mai avem nevoie de el
                                    break;
                                default:
                                    close (i) ; FD_CLR (i, &active_fd_set) ;
                                    break ;
                            }
                        }
                    }
                }
            }
    }

    pthread_exit (NULL) ;
}

