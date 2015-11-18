/* 
 * File:   main.c
 * Author: benjaminbercy
 *
 * Created on 10 novembre 2015, 13:50
 */

#include <stdio.h>
#include <stdlib.h>
#include "net_interface_udp.h"
/*
 */
int main(int argc, char** argv) {
    int nb_client, port;
    char **addr;
    
    if (argc != 5) {
        printf("Use : %s <nb_client> <my_ip> <port> <timeout>",argv[0]);
        exit(1);
    } else {
        nb_client = atoi(argv[1]);
        port = atoi(argv[3]);
        addr = p2p_ping(nb_client,argv[2],port, atoi(argv[4]));
        for (int i =0 ; i < nb_client ; i++ ) {
            printf("%s\r\n",addr[i]);
        }
    }
    
    return (EXIT_SUCCESS);
}
