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
    int servers_sockets[4];
    char** adresses = malloc(4*sizeof(char*));
    adresses[0] = (char*) malloc(64);
    
    if (argc != 4) {
        printf("Usage: %s <addresse> <port_num> <path>\n", argv[0]);
        exit(-1);
    }
    
    port_number = atoi(argv[2]);
    
    strcpy(adresses[0], "localhost");
    adresses[1] = (char*) NULL;
    
    p2p_scan_for_servers(servers_sockets, 2, argv[3], adresses);
    
    
    
    return (EXIT_SUCCESS);
}
