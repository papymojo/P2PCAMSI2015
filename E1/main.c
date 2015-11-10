/* 
 * File:   main.c
 * Author: benjaminbercy
 *
 * Created on 10 novembre 2015, 13:50
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
int main(int argc, char** argv) {
    int nb_client;
    int port;
    char ** addr;
    if (argc <!=3 ) {
        printf("Use : %s nb_client port",argv[0]);
        return(1);
    } else {
        nb_client = atoi(argv[1]);
        addr = calloc(nb_client,19*sizeof(char));
        addr = ping(nb_client,port);
        for (int i =0 ; i < 3 ; i++ ) {
            printf("%s\r\n",nb_client[i]);
        }
    }
    return (EXIT_SUCCESS);
}

