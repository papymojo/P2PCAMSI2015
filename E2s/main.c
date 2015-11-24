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
    if (argc != 3) {
        printf("Use : %s <port> <folder>",argv[0]);
        exit(1);
    }
    tcps_file(argv[2],atoi(argv[1]));
    return (EXIT_SUCCESS);
}

