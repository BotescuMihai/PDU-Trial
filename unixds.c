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
#include <sys/un.h>

/*
extern WINDOW * mainwnd ;
extern pthread_mutex_t curmtx ;
*/

int unix_socket (const char *filename) {
  struct sockaddr_un name;
  int sock; /* UNIX Socket descriptor */
  size_t size;
  
  /* Create the socket. */
  sock = socket (PF_LOCAL, SOCK_DGRAM, 0);
  if (sock < 0) {
//    perror ("unix-common: socket error");
    pthread_exit (NULL);
  }
  
  name.sun_family = AF_LOCAL; /* Set ADDRESS Family */
  strncpy (name.sun_path, filename, sizeof (name.sun_path));
  /* Create SOCKET Path info */
  name.sun_path[sizeof (name.sun_path) - 1] = '\0';
  
  size = (offsetof (struct sockaddr_un, sun_path)
	  + strlen (name.sun_path) + 1);
  /* You can use size = SUN_LEN (&name) ; instead */
  
  /* Now BIND the socket */
  if (bind (sock, (struct sockaddr *) &name, size) < 0) {
//    perror ("bind");
    pthread_exit (NULL);
  }
  
  /* And RETURN success :) */
  return sock;
}
int conn;
void *unix_main (void *args) {
  char *socket = (char *) args ;
  if(conn == 1){
      fprintf(stderr, "Nu mai pot accepta alti clienti!\n");
      return NULL;
  }
  if (unix_socket (socket)) {
      conn = 1;
 /*
    pthread_mutex_lock (&curmtx) ; // Protect CURSES usage!!!
    attron (COLOR_PAIR(1)) ;
    mvwprintw (mainwnd, LINES-4, 2, "Socket UNIX (%s) created", socket) ;
    attroff (COLOR_PAIR(1)) ;
//    wrefresh (mainwnd) ;
    pthread_mutex_unlock (&curmtx) ;
    */
  }

  pthread_exit (NULL) ;
}

