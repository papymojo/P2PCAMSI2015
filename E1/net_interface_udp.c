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
#include <arpa/inet.h>
#include <sys/socket.h>

struct sockaddr_in data;
int fd;
char** p2p_ping(int nb_client,int port) {
    char** addr;
    
    addr = calloc(nb_client, sizeof(char*));
    
    //open udp
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    memset(&addr,0,sizeof(addr));
    data.sin_family = AF_INET;
    data.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    data.sin_port=htons(port);
    for (int i=0 ; i < nb_client ; i++ ) {
        
        addr[i] = calloc(19, sizeof(char));
        strncpy(addr[i], "127.0.0.1", 19);
    }
    
    return addr;
}

int p2p_send(char* buffer, int size, int port) {
            if (sendto(fd, buffer, size, 0,(struct sockaddr *) &data, sizeof(data)) < 0)
        {
            perror("sendto");
            exit(1);
        }
    return 0;
}

int recieve(void* buffer, int size) {
    
}

