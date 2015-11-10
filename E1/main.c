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
    int i;
    char **addr;
    if (argc !=3 ) {
        printf("Use : %s nb_client port",argv[0]);
        return(1);
    } else {
        nb_client = atoi(argv[1]);
        addr = p2p_ping(nb_client,port);
        for (i =0 ; i < nb_client ; i++ ) {
            printf("%s\r\n",addr[i]);
        }
    }
    return (EXIT_SUCCESS);
}

