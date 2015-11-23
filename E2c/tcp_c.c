/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int p2p_tcp_connect (char* addr, int port_num) {
    int sock_fd;
    struct sockaddr_in sin = {0};
    
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sock_fd) {
        perror("Creation du socket");
        exit(-1);
    }
    
    inet_pton(AF_INET, addr, &(sin.sin_addr));
    sin.sin_port = htons(port_num);
    sin.sin_family = AF_INET;
    printf("connection en cours...\n");
    if (connect(sock_fd, (struct sockaddr*) &sin, sizeof(struct sockaddr)) == -1) {
        perror("Connection" );
        exit(-1);
    }
    printf("connected!\n");
    return 0;
}
