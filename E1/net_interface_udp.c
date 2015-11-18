/*
 * File: net_interface_udp.c
 * Author: Olivier
 * 
 * Created on 10 novembre 2015, 13:50
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include "net_interface_udp.h"

struct sockaddr_in datac;
struct sockaddr_in datas;
int fdc;
int fds;

/* handler pour le sigalrm du timeout */
void handler (int signum) {
    return;
}

char** p2p_ping(int nb_client, char* ip, int port, unsigned int timeout) {
    char** addr;
    char* buffer;
    
    /* traitement du timeout */
    signal(SIGALRM, handler);
    
    addr = calloc(nb_client, sizeof(char*));
    for (int i=0 ; i<nb_client ; ++i) {
        addr[i] = (char *) calloc(19, sizeof(char));
    }
    buffer = calloc(19,sizeof(char));
    
    //open udp server
    if ((fds = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    int reuseaddr = 1;
    if(setsockopt(fds, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    datas.sin_addr.s_addr = htonl(INADDR_ANY);
    datas.sin_family = AF_INET;
    datas.sin_port = htons(port);
    if(bind (fds, &datas, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind");
        exit(errno);
    }
    
    //communication (envoi de l'addresse dans le reseau)
    strncpy(addr[0],ip,19);
    p2p_send(ip,19,port);
    
    /* lancement du timeur de timeout */
    alarm (timeout);
    /* attente de clients */
    int r, i=1;
    do {
        
        printf("En attente de reception ...\n");
        if (p2p_recieve(buffer,19) == EINTR) {
            return addr;
        }
        
        printf("Recieved: %s\n", buffer);
        
        /* on vérifie qu'on ne connais pas déjà ce client */
        r = 0;
        for (int j=0 ; j<i ; ++j) {
            if (!strcmp(buffer,addr[j])) {
                r++;
            }
        }
        if (r == 0) { // si on connais pas on ajoute à la liste de clients
            strncpy(addr[i], buffer, 19);
            p2p_send(ip,19,port);
            ++i;
        }
        
    } while(i < nb_client);
    
    printf("groupe complet\n");
    return addr;
}

int p2p_send(char* buffer, int size,int port) {
        //open udp client
    if ((fdc = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    
    //enable use of broadcast
    int broadcastenable = 1;
    if (setsockopt(fdc,SOL_SOCKET,SO_BROADCAST,&broadcastenable,sizeof(broadcastenable)) < 0) {
        perror("setsockopt");
        exit(1);
    }
    
    memset(&datac,0,sizeof(datac));
    datac.sin_family = AF_INET;
    datac.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    datac.sin_port=htons(port);
    if (sendto(fdc, buffer, size, 0,(struct sockaddr *) &datac, sizeof(struct sockaddr_in)) < 0)
    {
        perror("sendto");
        exit(1);
    }
    close(fdc);
    return 0;
}

int p2p_recieve(char* buffer, int size) {
    socklen_t datasize = (socklen_t) sizeof(struct sockaddr_in);
    int r = recvfrom( fds, buffer, size-1, 0, (struct sockaddr *) &datac, &datasize);
    
    if (r < 0 && r != EINTR) {
        perror("recevievefrom");
        exit(1);
    }
    buffer[size] = '\0';
    
    return r;
}

