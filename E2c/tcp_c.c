/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "tcp_c.h"

int str_startwith (const char* str1, const char* str2) {
    int i=0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i])
            return 0;
        ++i;
    }
    
    return 1;
}

/* return the socket descriptor */
int p2p_tcp_connect (char* addr) {
    int sock_fd;
    struct sockaddr_in sin = {0};
    
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sock_fd) {
        perror("Creation du socket");
        exit(-1);
    }
    
    inet_pton(AF_INET, addr, &(sin.sin_addr));
    sin.sin_port = htons(port_number);
    sin.sin_family = AF_INET;
    printf("connection en cours...\n");
    if (connect(sock_fd, (struct sockaddr*) &sin, sizeof(struct sockaddr)) == -1) {
        perror("Connection" );
        exit(-1);
    }
    printf("connected!\n");
    return sock_fd;
}

int p2p_get_bloc (int server_sock, int block_num, char* filename) {
    char request[256];
    char buffer[DATA_BLOCK];
    
    strcpy(request, filename);
    sprintf(request+strlen(request), "!%d", block_num);
    
    /* envoie de la reqûete au serveur*/
    printf("send request   : %s\n", request);
    if (send(server_sock, request, strlen(request), 0) == -1) {
        perror("Sending request");
        exit(-1);
    }

    /* réponse du serveur */
    if (recv(server_sock, buffer, DATA_BLOCK, 0) < 0) {
        perror("Recieve");
        exit(-1);
    }
}

void p2p_scan_for_servers (int* servers_sockets, int nb_servers, char* filename, char** adresses) {
    const int buffer_size = 256;
    int sock, filename_start, num_server=0, i=0;
    char request[256] = SCAN_FOR_FILE;
    char buffer[256];
    strcat(request, "!");

    filename_start = strlen(request);
    servers_sockets = (int*) malloc(nb_servers);
    
    /* pour chaque serveur */
    while  (adresses[i] != NULL) {
        printf("scan server n° %d\n", i);
        sock = p2p_tcp_connect (adresses[i]);
        strcpy(request+filename_start, filename);
        
        /* envoie de la reqûete au serveur*/
        printf("send request   : %s\n", request);
        printf("Request length : %d\n", (int)strlen(request));
        if (send(sock, request, strlen(request), 0) == -1) {
            perror("Sending request");
            exit(-1);
        }
        
        /* réponse du serveur */
        if (recv(sock, buffer, buffer_size, 0) < 0) {
            perror("Recieve");
            exit(-1);
        }
        printf("recieved %s\n", buffer);
        
        /* parsing de la réponse */
        if (str_startwith(buffer, HAVE_FILE)) {//le serveur a le fichier
            servers_sockets[num_server ++] = sock;
            printf("OK!\n");
        }
        else if(str_startwith(buffer, NO_FILE)) {//le serveur n'a pas le fichier
            close(sock);
            printf("KO :(\n");
        }
        else {//réponse inconnue
            fprintf(stderr, "ERROR: unknown server response\n");
        }
        
        ++i;
    }
    
    servers_sockets[num_server] = -1;
}

void p2p_save_block (FILE f, char* block) {
    
}