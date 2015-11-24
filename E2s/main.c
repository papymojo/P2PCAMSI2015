/* 
 * File:   main.c
 * Author: benjaminbercy
 *
 * Created on 18 novembre 2015, 14:24
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Use : %s <port>",argv[0]);
        exit(1);
    }
    tcps_file(atoi(argv[1]));
    return (EXIT_SUCCESS);
}

