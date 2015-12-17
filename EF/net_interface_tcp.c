/*
 * File: net_interface_tcp.c
 * Author: Benjamin
 * 
 * Created on 10 novembre 2015, 13:50
 */

#define  _POSIX_C_SOURCE  1
#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#include <string.h>
#include <strings.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <pthread.h>

#include "tcp_c.h"
#include "net_interface_tcp.h"

struct sockaddr_in datas;
char queryrecv[DATA_BLOCK];
int fdl;
int fdc;

char * getfilename(char * query) {
     char *token = malloc(strlen(query));
     strcpy(token, query);

     char* end = strchr(token, SEPARATOR);
     if (end != NULL)
         *end = '\0';
     
     return token;
}

int getblocknumber(char * query) {
    return atoi(strchr(query, SEPARATOR)+1);
}

int tcps_scan(char * filename) {
    struct stat info;
    char * answer;
    answer = calloc(128,sizeof(char));
    int n;
    if((n=open(filename, O_RDONLY)) >= 0){
        fstat(n, &info);
        sprintf(answer,"OK!%d",(int)info.st_size);
        tcps_send(answer,128);
    } else {
        tcps_send(NOFILE,128);
    }
    
    free(answer);
    return(0);
}

int tcps_file(int port) {
    if ( (fdl = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "socket : \n");
	exit(EXIT_FAILURE);
    }
    memset(&datas, 0, sizeof(struct sockaddr_in));
    datas.sin_family      = AF_INET;
    datas.sin_addr.s_addr = htonl(INADDR_ANY);
    datas.sin_port        = htons(port);
    
    if ( bind(fdl, (struct sockaddr *) &datas, sizeof(struct sockaddr)) < 0 ) {
	perror( "bind : \n");
	exit(EXIT_FAILURE);
    }
    if ( listen(fdl,MAX_LINE) < 0 ) {
	fprintf(stderr, "listen : \n");
	exit(EXIT_FAILURE);
    }
    printf("en écoute sur port %d.\n", port);
    for (;;) {
        if ((fdc = accept(fdl, NULL, 0)) < 0) {
            fprintf(stderr,"accept : \n");
        }
        int pid = fork();
        switch(pid){
            case 0 :
                    printf("connecté\n");
                    do {
                        tcps_recv(queryrecv,DATA_BLOCK);
                        printf("querry recieved! \"%s\"\n", queryrecv);
                        if (strcmp(getfilename(queryrecv),SCAN)==0) {
                            printf("Scanning for file %s\n", queryrecv+5);
                            tcps_scan(getfilename(queryrecv+5));
                        } else if (strcmp(getfilename(queryrecv),UNCONNECT)!=0) {
                            printf("ask for block number : %d of the file %s\n",getblocknumber(queryrecv),getfilename(queryrecv));
                            tcps_sendblock(getfilename(queryrecv),getblocknumber(queryrecv));
                            usleep(10);
                        } else {
                            printf("GoodBye!");
                        }
                    } while (strcmp(getfilename(queryrecv),UNCONNECT)!=0);
                if ( shutdown(fdc,SHUT_RDWR) < 0 ) {
                    fprintf(stderr, "shutdown : \n");
                    exit(EXIT_FAILURE);
                }
                if ( close(fdc) < 0 ) {
                    fprintf(stderr, "close : \n");
                    exit(EXIT_FAILURE);
                }
                    exit(0);
                    break;
            default:
                break;
        }
    }
}

int tcps_sendblock(char * file,int block_num) {
    block_t b;
    
    struct stat info;
    FILE *fp = fopen(file, "r+");
    fstat(fileno(fp), &info);
    if (fp == NULL) {
        printf("ERROR:%s\n", file);
        exit(1);
    }
    bzero(b.data, DATA_BLOCK);
    int f_block_sz;
    if (block_num*DATA_BLOCK >= info.st_size) {
        tcps_send("EXIT!", 6);
        printf("EndOfFile\n");
    } else {
        fseek(fp,block_num*DATA_BLOCK,SEEK_SET);
        if ((b.block_size = fread(b.data, sizeof (char), DATA_BLOCK, fp)) > 0) {
            tcps_send((char*)&b, b.block_size+sizeof(b.block_size));
        }
    }
    printf("Fin d'envoi\n");
}

int tcps_send(char* buffer, int size) {
    send(fdc, (void *) buffer, size, 0);
    return 0;
}

int tcps_recv(char* buffer, int size) {
    recv(fdc, (void *)buffer,size, 0);
    return 0;
}

