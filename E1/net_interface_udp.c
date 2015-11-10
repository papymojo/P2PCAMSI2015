/*
 * File: net_interface_udp.c
 * Author: Olivier
 * 
 * Created on 10 novembre 2015, 13:50
 */

#include <stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

char** p2p_ping(int nb_client) {
    return NULL;
}

int send(char* buffer, int size) {
    struct sockaddr_in si_me, si_other;
    
    int sock;
    
    /* create an UDP socket */
    if (-1 == (sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))) {
        perror("Fail on socket creation");
        exit(-1);
    }
}

int recieve(void* buffer, int size) {
    
}

