#include <pthread.h>
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include "files_util.h"
#include <arpa/inet.h>
#include "proto.h"

char * opt_echo(char * filename, char * filepath){
    // determina-i extensia
    char * tempfname = malloc(sizeof(filename) + 1);
    strcpy(tempfname, filename);
    char * p = strtok(filename, ".");
    p = strtok(NULL, ".");
    char * logfname = malloc(500);
    strcpy(logfname, "./client_files/INET/");
    strcat(logfname, tempfname);
    strcat(logfname, ".txt");
    logfname[strlen(logfname)] = 0;
    fprintf(stderr, "log: %s\n", logfname);
    char * command = malloc(200);
    unlink(logfname); // if exists
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
            close(logfname);
                execl("/usr/bin/python3", "/usr/bin/python3", command, NULL);
                perror("execl()");
            }
            else if(strcmp(p, "c") == 0){
                strcpy(command, "-o ");
                strcat(command, tempfname);
                strcat(command, "{,.c} && ./");
                strcat(command, tempfname);
                execl("/usr/bin/gcc", "/usr/bin/gcc", command, NULL);
                perror("execl()");
            }
            else if(strcmp(p, "java") == 0){
             //   strcpy(command, "javac ");
                strcpy(command, tempfname);
                execl("/bin/javac", "/bin/javac", command, NULL);
                perror("execl()");
            }
            break;
        default:
            wait(NULL); // asteapta dupa fiu
    }
    int fd = open(logfname, S_IRUSR, 0666);
    if(fd < 0){
        perror("open");
    }
    char * buf = malloc(10000);
    read(fd, buf, 10000);
    unlink(logfname);
    return buf;
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

void *inet_main (void *args) {
  int port = *((int *)args) ;
  int sock ;
  size_t size;
  fd_set active_fd_set, read_fd_set;
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
               if (readSingleInt (i, &m)<0) {
                 // Cannot read from client. This is impossible :) Close connection!
                 close (i) ; FD_CLR (i, &active_fd_set) ;
               }
               if (writeSingleInt (i, h, newID) < 0) {
                 // Cannot write to client. Close connection!
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

              switch (operation) {
                 case OPR_ECHO: 
                      {
                      	 msgStringType str, str2, str3 ;
	                 if (readSingleString (i, &str) < 0) {
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
                          }
                          //// asta-i continutul fisierului
                          // 	fprintf (stderr, "An echo value was received: %s\n", str.msg) ;
                 ///// asta-i numele fisierului
                     fprintf(stderr, "Filename is %s%s\n", str3.msg, str2.msg);
                  // calea fisierului este str3.msg
                        char * logcontent = opt_echo(str2.msg, str3.msg);
                        strcpy(str.msg, logcontent);
                          if (writeSingleString (i, h, str.msg) < 0) {
                              // Cannot write to client. Close connection!
                              close (i) ; FD_CLR (i, &active_fd_set) ;
                          }
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
                 case OPR_CONC: 
                      break ;
                 case OPR_ADD: 
                      {
                      	msgIntType m1, m2, m ;
                      	if (readMultiInt (i, &m1, &m2)<0) {
                      		// Cannot read from client. This is impossible :) Close connection!
                      	  close (i) ; FD_CLR (i, &active_fd_set) ;
                      	}
                      	// Complex implementation for negative value :D
                      	m.msg = m1.msg+m2.msg ;
                      	if (writeSingleInt (i, h, m.msg) < 0) {
                 		// Cannot write to client. Close connection!
                 	  close (i) ; FD_CLR (i, &active_fd_set) ;
                 	}
                 	fprintf (stderr, "An adder value was sent back: %d\n", m.msg) ;
                      } 
                      break ;
                 case OPR_NEG: 
                      {
                      	msgIntType m ;
                      	if (readSingleInt (i, &m)<0) {
                      		// Cannot read from client. This is impossible :) Close connection!
                      	  close (i) ; FD_CLR (i, &active_fd_set) ;
                      	}
                      	// Complex implementation for negative value :D
                      	m.msg = -m.msg ;
                      	if (writeSingleInt (i, h, m.msg) < 0) {
                 		// Cannot write to client. Close connection!
                 	  close (i) ; FD_CLR (i, &active_fd_set) ;
                 	}
                 	fprintf (stderr, "A negative value was sent back: %d\n", m.msg) ;
                      } 
                      break ;
                 case OPR_BYE: 
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

