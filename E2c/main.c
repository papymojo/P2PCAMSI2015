/* 
 * File:   main.c
 * Author: benjaminbercy
 *
 * Created on 18 novembre 2015, 14:23
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "tcp_c.h"

#define MAX_SEGMENT_SIZE 4096

/*
 * 
 */
int main(int argc, char** argv) {
    int sock_desc, file_desc, n;
    char buffer[MAX_SEGMENT_SIZE];
    
    if (argc != 4) {
        printf("Usage: %s <addresse> <port_num> <path>\n", argv[0]);
        exit(-1);
    }
    
    sock_desc = p2p_tcp_connect(argv[1], atoi(argv[2]));
    file_desc = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY);
    
    if (send(sock_desc, argv[3], strlen(argv[3]), 0) == -1) {
        perror("Sending request");
        exit(-1);
    }
    
    do {
        if ((n = recv(sock_desc, buffer, MAX_SEGMENT_SIZE, 0)) < 0) {
            perror("Recieve");
            exit(-1);
        }
        write(file_desc, buffer, n);
    } while(n != 0);
    
    return (EXIT_SUCCESS);
}
