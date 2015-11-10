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

char** p2p_ping(int nb_client) {
    char** addr;
    
    addr = calloc(nb_client, sizeof(char*));
    
    for (int i=0 ; i < nb_client ; i++ ) {
        addr[i] = calloc(19, sizeof(char));
        strncpy(addr[i], "127.0.0.1", 19);
    }
    
    return addr;
}

int p2p_send(char* buffer, int size) {
    struct sockaddr_in si_me, si_other;
    
    int sock;
    
    /* create an UDP socket */
    if (-1 == (sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))) {
        perror("Fail on socket creation");
        exit(-1);
    }
    
    /* init structure socket */
    //memset();
    return 0;
}

int recieve(void* buffer, int size) {
    
}

