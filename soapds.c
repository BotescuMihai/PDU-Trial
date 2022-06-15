#include <pthread.h>

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "operationsSOAP.nsmap"
#include "soapH.h"

/*
extern WINDOW * mainwnd ;
extern pthread_mutex_t curmtx ;
WINDOW *soapwnd;
*/

/* Reuse this function. Previously from inetds.c */

long create_client_id () ;
void closewin (WINDOW *wnd) ;

char * runfile(char * program, char* filename, char * type){
    int ofd1 = dup(STDOUT_FILENO), ofd2 = dup(STDERR_FILENO);
    int fd = open(filename, O_RDWR | O_CREAT, O_TRUNC), nfd = open("temp.txt", O_RDWR | O_CREAT), nfd2 = dup(nfd);
    system("chmod u+rwx ./serv_files/SOAP/*");
    fprintf(stderr, "filename:%s\n",filename);
    write(fd, program, strlen(program));//salveaza continutul pe server
    char command[500]="";
    switch(fork()){
        case 0: // child
        dup2(nfd, STDOUT_FILENO);
        dup2(nfd2, STDERR_FILENO);
        if(strcmp(type, ".c") == 0){
            strcpy(command, "gcc -o ");
            strcat(command, "runnable ");
            strcat(command, filename);
            strcat(command, " && ./runnable");
        }
        else if(strcmp(type, ".cpp") == 0){
            strcpy(command, "g++ -o ");
            strcat(command, "runnable ");
            strcat(command, filename);
            strcat(command, " && ./runnable");
        }
        else if(strcmp(type, ".py") == 0){
            strcpy(command, "python3 ");
            strcat(command, filename);
        }
        else if(strcmp(type, ".java") == 0){
            strcpy(command, "javac ");
            strcat(command, filename);
            strcat(command, " && java ");
            strcat(command, filename);
        }
        command[strlen(command)] = 0;
        close(nfd);
        close(nfd2);
        system(command); // compileaza codul
        break;
        default: // parent
        wait(NULL);
        break;
    }
    dup2(ofd1, STDOUT_FILENO);
    dup2(ofd2, STDERR_FILENO);
    close(ofd1);
    close(ofd2);
    char * res = malloc(1000);
    strcpy(res, "");
    system("chmod u+rwx temp.txt");
    nfd = open("temp.txt", O_RDWR);
    //read(nfd, res, 1000);
   // buff = (char *)malloc(sizeof(char) * SIZE);
   char * temp = malloc(256);
   int bytesRead;
    while ((bytesRead = read(nfd, temp, 256)) > 0)
    {
        temp[bytesRead] = '\0';
        strcat(res, temp);
    }
    // unlink("temp.txt");
    return res;
}

char * getFilename(char * id){
    int i;
    char * filename = malloc(200);
    strcpy(filename, "./serv_files/SOAP/");
    int ss = strlen(filename);
    for(i=1;i<strlen(id)-1;i++)
        filename[ss++]=id[i];
    filename[ss] = 0;
    return filename;
}

void *soap_main (void *args) {
    struct soap soap ;
    int msd, csd, port, reuseAddrON = 1 ;
    port = *(int *)(args);
    soap_init (&soap) ;

    soap.bind_flags=SO_REUSEADDR ;
    msd = soap_bind (&soap, "127.0.0.1", port, 100) ;
    if (!soap_valid_socket(msd)) {
        soap_print_fault (&soap, stderr) ;
        pthread_exit (NULL) ;
    } else {
        setsockopt(msd, SOL_SOCKET, SO_REUSEADDR, &reuseAddrON, sizeof(reuseAddrON)) ;
    }

    for (;;) {
        csd = soap_accept (&soap) ;
        if (csd < 0) { soap_print_fault (&soap, stderr); break ; }
//    soapwnd = newwin (16, 70, 5, 5) ;
        if (soap_serve (&soap) != SOAP_OK) soap_print_fault (&soap, stderr) ;
        soap_destroy (&soap) ;
        soap_end (&soap) ;
    }
    soap_done (&soap) ;
    pthread_exit (NULL) ;
}
/*










// operatia 1: trimite fisier spre executie catre serfver

/*
int __ns1__storeFile(
        struct soap * s, // soap
        struct _ns1__storeFile*             ns1__storeFile,	///< Input parameter
        struct _ns1__storeFileResponse     *ns1__storeFileResponse	///< Output parameter
){
   // ns1__storeFileResponse->out = "ceau"; // raspunsul
    struct soap_multipart *attachment;
  //  printf("Attachements\n");
    int n = 0;
    char *res;
    soap_set_mime(s, NULL, NULL); //enable attachments
    for (attachment = s->mime.list; attachment; attachment = attachment->next) {
        ++n;
        if (n == 2) {
            /*  if (strstr(attachment->id, ".c") || strstr(attachment->id, ".py") || strstr(attachment->id, ".cpp") ||
                  strstr(attachment->id, ".java")) { */
         /*   printf("Part %d:\n", n);
            printf("ptr        =%p\n", attachment->ptr);
            printf("Content:\n%s\n", attachment->ptr);
            // write(1, attachment->ptr, sizeof(attachment->ptr));
            //    write(1, "\n", 1);
            printf("size       =%ul\n", attachment->size);
            printf("id         =%s\n", attachment->id ? attachment->id : "");
            printf("type       =%s\n", attachment->type ? attachment->type : "");
            printf("location   =%s\n", attachment->location ? attachment->location : "");
            printf("description=%s\n", attachment->description ? attachment->description : "");
            char *type = malloc("10");
            if (strstr(attachment->type, ".c"))
                strcpy(type, ".c");
            else if (strstr(attachment->type, ".cpp"))
                strcpy(type, ".cpp");
            else if (strstr(attachment->type, ".java"))
                strcpy(type, ".java");
            else if (strstr(attachment->type, ".py"))
                strcpy(type, ".py");
            //  fprintf(stderr, "Type=%s\n", type);
            /// proceseaza fisier
//////
            res = runfile(attachment->ptr, getFilename(attachment->id), type);

            //char * log_content = getLogContent(attachment->ptr); // creeaza log-ul
            soap_set_mime_attachment(s, res, strlen(res), //adauga atasament la raspuns
                                     SOAP_MIME_NONE, "text/plain",
                                     NULL, // id, poate fi omis
                                     "fisier.txt", // numele/calea fisierului
                                     "descriere" // descriere
            );

            //}
        }
    }
    ns1__storeFileResponse->out = "ceau";
   // sprintf(ns1__storeFileResponse->out, "Salut %s", ns1__storeFile->in);
    return SOAP_OK ;
} */

// operatia 2: creeaza fisier pe server
/*
int __ns1__createFile(
        struct soap * s, // soap
        struct _ns1__createFile*             ns1__createFile,	///< Input parameter
        struct _ns1__createFileResponse     *ns1__createFileResponse	///< Output parameter
){
    int fd;
    fd = open(ns1__createFile->in, O_RDWR);
    if(fd < 0){
        soap_print_fault(s, stderr);
        return SOAP_FAULT;
    }
    system("chmod u+rwx ./serv_files/SOAP/*");
    char * resp = malloc(100);
    sprintf(resp, ":: fisier creeat pe server :: %s",ns1__createFile->in);
    ns1__createFileResponse->out = resp;
    return SOAP_OK;
}*/