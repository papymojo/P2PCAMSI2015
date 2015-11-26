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

/*
 * 
 */
int main(int argc, char** argv) {
    int sock_desc, file_desc, n;
    char buffer[DATA_BLOCK];
    char *request = malloc(sizeof(char) * (strlen(argv[3]) + 10));
    
    if (argc != 4) {
        printf("Usage: %s <addresse> <port_num> <path>\n", argv[0]);
        exit(-1);
    }
    
    sock_desc = p2p_tcp_connect(argv[1], atoi(argv[2]));
    file_desc = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY);
    
    int block_num = 0;
    do {
        sprintf(request, "%s!%d", argv[3], block_num);
        printf("Request: %s\n", request);
        if (send(sock_desc, request, strlen(request), 0) == -1) {
            perror("Sending request");
            exit(-1);
        }
        
        if ((n = recv(sock_desc, buffer, DATA_BLOCK, 0)) < 0) {
            perror("Recieve");
            exit(-1);
        }
        printf("Recieved.\n");
        
        if (strcmp(buffer, "EXIT!"))
            write(file_desc, buffer, n);
        else
            break;
        ++block_num;
    } while(n == DATA_BLOCK);
    
    if (send(sock_desc, "EXIT!", 5, 0) == -1) {
        perror("Sending disconnect request");
        exit(-1);
    }
    
    close(sock_desc);
    close(file_desc);
    
    return (EXIT_SUCCESS);
}
