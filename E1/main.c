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
 * 
 */
int main(int argc, char** argv) {
    int nb_client;
    int port;
    char **addr;
    
    if (argc !=4 ) {
        printf("Use : %s nb_client my_ip port",argv[0]);
        return(1);
    } else {
        nb_client = atoi(argv[1]);
        port = atoi(argv[3]);
        addr = p2p_ping(nb_client,argv[2],port);
        for (int i =0 ; i < nb_client ; i++ ) {
            printf("%s\r\n",addr[i]);
        }
    }
    return (EXIT_SUCCESS);
}

