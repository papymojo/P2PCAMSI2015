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
char filename[BLOCK];
int fdl;
int fdc;

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
                    tcps_recv(filename,BLOCK);
                    tcps_sendfile(filename);
                    sleep(1);
                    printf("timeout\n");
                if ( shutdown(fdc) < 0 ) {
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

int tcps_sendfile(char * file) {
    char buffer[BLOCK];
    printf("envoi du fichier %s au client\n", file);
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        printf("ERROR:\n", file);
        exit(1);
    }
    bzero(buffer, BLOCK);
    int f_block_sz;
    while ((f_block_sz = fread(buffer, sizeof (char), BLOCK, fp)) > 0) {
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

