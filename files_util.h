//
// Created by alex on 31.05.2022.
//

#ifndef PDU_TRIAL_FILES_UTIL_H
#define PDU_TRIAL_FILES_UTIL_H
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <libgen.h>
#define MAX 10000
int client_UIDS[10000];
int client_no;


char * getTimestamp(){
    time_t ltime;
    ltime = time(NULL);
    return asctime(localtime(&ltime));
}

char * getFileUID(char * type){
    char * UID = malloc(100);
    strcpy(UID, "./client_files/");
    strcat(UID, type);
    strcat(UID, "/log_[");
    strcat(UID, getTimestamp());
    strcat(UID, "].txt");
    UID[strlen(UID)] = 0;
    return UID;
}

char * getFileName(char * name){
    char* ts2 = strdup(name);
    return basename(ts2);
}
char * getFilePath(char * name){
    return dirname(strdup(name));
}
char * getContent(int fd){
    if(fd < 0) return NULL;
    char * content = malloc(MAX);
    switch(fork()){
        case -1: perror("fork"); return NULL;
        case 0: /* child */
            read(fd, content, MAX);
        default:
            wait(NULL);
            break;
    }
    content[strlen(content)] = 0;
    return content;
}

#endif //PDU_TRIAL_FILES_UTIL_H
