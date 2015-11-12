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
#include "net_interface_udp.h"

struct sockaddr_in datac;
struct sockaddr_in datas;
int fdc;
int fds;

char** p2p_ping(int nb_client,char* ip,int port) {
    char** addr;
    char* buffer;
    
    addr = calloc(nb_client, sizeof(char*));
    buffer = calloc(19,sizeof(char));
    
    //open udp client
    if ((fdc = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    
    //enable use of broadcast
    int broadcastenable = 1;
    if (setsockopt(fdc,SOL_SOCKET,SO_BROADCAST,&broadcastenable,sizeof(broadcastenable)) < 0) {
        perror("setsockopt");
        exit(1);
    }
    
    memset(&datac,0,sizeof(datac));
    datac.sin_family = AF_INET;
    datac.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    datac.sin_port=htons(port);
    
    //open udp server
    if ((fds = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    int reuseaddr = 1;
    if(setsockopt(fds, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    datas.sin_addr.s_addr = htonl(INADDR_ANY);
    datas.sin_family = AF_INET;
    datas.sin_port = htons(port);
    if(bind (fds, &datas, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind");
        exit(errno);
    }
    
    //communication (envoi de l'addresse dans le reseau)
    addr[0] = calloc(19, sizeof(char));
    strncpy(addr[0],ip,19);
    p2p_send(ip,19);
    for (int i=1 ; i < nb_client ; i++ ) {
        int r =0;
        p2p_recieve(buffer,19);
        for (int j=0 ; j < i;j++) {
            if (!strcmp(buffer,addr[j])) {
                r++;
            }
        }
        if (r = 0) {
            addr[i] = calloc(19, sizeof(char));
            strncpy(addr[i], buffer, 19);
        }
        p2p_send(ip,19);
    }
    
    return addr;
}

int p2p_send(char* buffer, int size) {
            if (sendto(fdc, buffer, size, 0,(struct sockaddr *) &datac, sizeof(struct sockaddr_in)) < 0)
        {
            perror("sendto");
            exit(1);
        }
    return 0;
}

int p2p_recieve(void* buffer, int size) {
    socklen_t datasize = (socklen_t) sizeof(struct sockaddr_in);
    if ( recvfrom( fds, buffer, size, 0, (struct sockaddr *) &datac, &datasize) < 0) {
        perror("recevievefrom");
        exit(1);
    }
}

