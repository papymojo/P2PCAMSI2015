/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <fcntl.h>

#include "tcp_c.h"

int str_startwith(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i])
            return 0;
        ++i;
    }

    return 1;
}

/* return the socket descriptor */
int p2p_tcp_connect(char* addr) {
    int sock_fd;
    struct sockaddr_in sin = {0};

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sock_fd) {
        perror("Creation du socket");
        exit(-1);
    }

    //sin.sin_addr.s_addr = inet_addr(addr);
    inet_pton(AF_INET, addr, &(sin.sin_addr));
    sin.sin_port = htons(port_number);
    sin.sin_family = AF_INET;
    printf("connection en cours...\n");
    if (connect(sock_fd, (struct sockaddr*) &sin, sizeof (struct sockaddr)) == -1) {
        perror("Connection");
        exit(-1);
    }
    printf("connected!\n");
    return sock_fd;
}

/* cherche les serveurs qui ont le fichier demandé
 * La liste des sockets de serveurs est terminée par 0.
 */
void p2p_scan_for_servers(int* servers_sockets, int nb_servers, char* filename, char** adresses) {
    const int buffer_size = 256;
    int sock, filename_start, num_server = 0, i = 0;
    char request[256] = SCAN_FOR_FILE;
    char buffer[256];
    strcat(request, "!");

    filename_start = strlen(request);
    servers_sockets = (int*) malloc(nb_servers);

    /* pour chaque serveur */
    while (adresses[i] != NULL) {
        printf("scan server n° %d, addr %s\n", i, adresses[i]);
        sock = p2p_tcp_connect(adresses[i]);
        strcpy(request + filename_start, filename);

        /* envoie de la reqûete au serveur*/
        printf("send request   : %s\n", request);
        printf("Request length : %d\n", (int) strlen(request));
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
            servers_sockets[num_server++] = sock;
            printf("OK!\n");
        } else if (str_startwith(buffer, NO_FILE)) {//le serveur n'a pas le fichier
            close(sock);
            printf("KO :(\n");
        } else {//réponse inconnue
            fprintf(stderr, "ERROR: unknown server response\n");
        }

        ++i;
    }

    servers_sockets[num_server] = -1;
}

/** récupère un bloc du fichier filename sur le serveur
 * 
 */
int p2p_get_bloc(int server_sock, int block_num, char* filename, block_t* block) {
    char request[256];

    strcpy(request, filename);
    sprintf(request + strlen(request), "!%d", block_num);

    /* envoie de la reqûete au serveur*/
    printf("send request   : %s\n", request);
    if (send(server_sock, request, strlen(request), 0) == -1) {
        perror("Sending request");
        exit(-1);
    }

    /* réponse du serveur */
    if (recv(server_sock, (char*)block, DATA_BLOCK, 0) < 0) {
        perror("Recieve");
        exit(-1);
    }
}

/**
 * Ecrit le bloc dans le fichier 
 * @param filename
 * @param block
 * @param block_num
 * @param block_size
 */
void p2p_save_block(char* filename, block_t* block, int block_num) {
    FILE* fd = fopen(filename, "r+");
    char garbage_buffer[DATA_BLOCK];
    
    fseek(fd, 0, SEEK_END);
    /* Agrandissement du fichier si nécessaire */
    if (ftell(fd) < block_num*DATA_BLOCK) {    
        while (block_num*DATA_BLOCK > ftell(fd)) {
            fwrite(garbage_buffer, DATA_BLOCK, 1, fd);
        }
    }
    else {
        fseek(fd, block_num*DATA_BLOCK, SEEK_SET); 
    }
    
    fwrite(block->data, block->block_size, 1, fd);
}

typedef struct thread_args_s {
    char* filename;
    int socket;
    int semid;
    int* block_num;
} thread_args_t;

void* p2p_client_thread(void* a) {
    thread_args_t * args = (thread_args_t*) a;
    int block_num;
    block_t block;
    
    struct sembuf param = {
        0,  // numéro de semaphore dans le set semid
        -1, // opération (P)
        0   // flags
    };
    
    /* récupération de blocs depuis le serveur associé à mon thread */
    for(;;) {
        /* récupération du numéro de bloc */
        semop(args->semid, &param, 1);
        block_num = *(args->block_num);
        *(args->block_num) ++;
        param.sem_op = 1;
        semop(args->semid, &param, 1);
        
        /* récupération du block */
        p2p_get_bloc(args->socket, block_num, args->filename, &block);
        if (str_startwith(block.data, END_FILE))
            break;
        else
            p2p_save_block(args->filename, &block, block_num);
    }

    return NULL;
}

/**
 * Récupère le fichier depuis les serveurs distants.
 * @param filename chemin du fichier sur les serveurs.
 * @param servers liste des sockets des conexions vers les serveurs.
 */
void p2p_get_file(char* filename, int* servers, int nb_servers) {
    pthread_t threads[nb_servers];
    thread_args_t args[nb_servers];
    int block_num = 0;
    key_t semkey;
    int semid;


    /* creation d'un sémaphore (exclusion de l'accès au n° de block) */
    semkey = ftok("p2p_client", 1789);
    if ((semid = semget(semkey, 1, IPC_CREAT | 0666)) == -1) {
        perror("Initialisation semaphore impossible");
        semctl(semid, 0, IPC_RMID, 0);
        exit(1);
    }
    

    /* lancement des threads */
    for (int i = 0; i < nb_servers; ++i) {
        args[i].filename = filename;
        args[i].socket = servers[i];
        args[i].block_num = &block_num;
        args[i].semid = semid;

        pthread_create(threads + i,
                NULL,
                p2p_client_thread,
                (void*) args + i);

        ++nb_servers;
    }

    /* attente de la fin des threads */
    for (int i = 0; i < nb_servers; ++i) {
        pthread_join(threads[i], NULL);
    }

}
