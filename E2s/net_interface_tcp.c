/*
 * File: net_interface_tcp.c
 * Author: Benjamin
 * 
 * Created on 10 novembre 2015, 13:50
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "net_interface_tcp.h"

struct sockaddr_in datas;
char queryrecv[BLOCK];
int fdl;
int fdc;

char * getfilename(char * query) {
     char *token;
     const char s[2] = SEPARATOR;
     token = strtok(query, s);
     return token;
}
int getblocknumber(char * query) {
     char *token;
     const char s[2] = SEPARATOR;
     strtok(query, s);
     token = strtok(NULL, s);
     return atoi(token);
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
	fprintf(stderr, "bind : \n");
	exit(EXIT_FAILURE);
    }
    if ( listen(fdl,MAX_LINE) < 0 ) {
	fprintf(stderr, "listen : \n");
	exit(EXIT_FAILURE);
    }
    printf("en écoute \n");
    for (;;) {
        if ((fdc = accept(fdl, NULL, 0)) < 0) {
            fprintf(stderr,"accept : \n");
        }
        int pid = fork();
        switch(pid){
            case 0 :
                    printf("connecté\n");
                    do {
                        tcps_recv(queryrecv,BLOCK);
                        printf("ask for block number : %d of the file %s\n",getblocknumber(queryrecv),getfilename(queryrecv));
                        tcps_sendblock(getfilename(queryrecv),getblocknumber(queryrecv));
                        usleep(10);
                        printf("timeout\n");
                    } while (strcmp(queryrecv,UNCONNECT)!=0);
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

int tcps_sendblock(char * file,int block) {
    char buffer[BLOCK];
    printf("envoi du fichier %s au client\n", file);
    FILE *fp = fopen(file, "r+");
    if (fp == NULL) {
        printf("ERROR:%s\n", file);
        exit(1);
    }
    bzero(buffer, BLOCK);
    int f_block_sz;
    fseek(fp,block*BLOCK,SEEK_SET);
    if ((f_block_sz = fread(buffer, sizeof (char), BLOCK, fp)) > 0) {
        tcps_send(buffer, f_block_sz);
        bzero(buffer, BLOCK);
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

