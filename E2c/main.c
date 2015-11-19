/* 
 * File:   main.c
 * Author: benjaminbercy
 *
 * Created on 18 novembre 2015, 14:23
 */

#include <stdio.h>
#include <stdlib.h>

#include "tcp_c.h"

/*
 * 
 */
int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <addresse> <port_num>\n", argv[0]);
        exit(-1);
    }
    
    p2p_tcp_connect(argv[1], atoi(argv[2]));
    
    return (EXIT_SUCCESS);
}
