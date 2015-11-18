/*
 * File: net_interface_udp.c
 * Author: Olivier
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
int fdl;
int fdc;
int f;

int tcps_file(char * file,char * ip,int port) {
    if ( (fdl = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "socket : \n");
	exit(EXIT_FAILURE);
    }
    memset(&datas, 0, sizeof(struct sockaddr_in));
    datas.sin_family      = AF_INET;
    datas.sin_addr.s_addr = htonl(INADDR_ANY);
    datas.sin_port        = htons(port);
    
    if ( bind(fdl, (struct sockaddr *) &datas, sizeof(datas)) < 0 ) {
	fprintf(stderr, "bind : \n");
	exit(EXIT_FAILURE);
    }
    if ( listen(fdl,MAX_LINE) < 0 ) {
	fprintf(stderr, "listen : \n");
	exit(EXIT_FAILURE);
    }
    for (;;) {
        if ((fdc = accept(fdl, NULL, NULL)) < 0) {
            fprintf(stderr,"accept : \n");
        }
        int pid = fork();
        switch(pid){
            case 0 :
                if ( close(fdc) < 0 ) {
                    fprintf(stderr, "ECHOSERV: Error calling close()\n");
                    exit(EXIT_FAILURE);
                }
                    exit(0);
            default:
                break;
        }
    }

}

int tcps_send(char* buffer, int size) {
    recv(fdc, (void *) buffer, size, 0);
    return 0;
}

int tcps_send(char* buffer, int size) {
    send(fdc, (const void *)buffer,size, 0);
    return 0;
}

