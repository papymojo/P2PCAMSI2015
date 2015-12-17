/* 
 * File:   main.c
 * Author: benjaminbercy
 *
 * Created on 10 décembre 2015, 11:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "net_interface_udp.h"
#include "net_interface_tcp.h"
#include "tcp_c.h"

#define NB_PEERS 2
#define PORT_NUM 1910
#define TIMEOUT 10

/* processus serveur */
void server_process (int port_num) {
    tcps_file(port_num);
}

/** Client procesus
 * adresses: the adress array must be terminated with a NULL.
 */
void client_process (int port_num, char** adresses) {
    int servers_sockets[NB_PEERS];
    
    /* récupération des serveurs possédant le fichier */
    p2p_scan_for_servers(servers_sockets, 2, "image.png", adresses);
    
    /* récupération du fichier */
    p2p_get_file("image.png", servers_sockets, NB_PEERS);
}


int main(int argc, char** argv) {
    
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s <port_num> <ip_addr>\n", argv[0]);
        exit(-1);
    }
    
    char* my_ip = argv[2];
    char** adresses;

    /* formation du groupe */
    adresses = p2p_ping(NB_PEERS, my_ip, PORT_NUM, TIMEOUT);
    
    int pid = fork();
    if (pid == 0) { // process fils (serveur)
        server_process(atoi(argv[1]));
    }
    else if (pid > 0) { // process père (client)
        sleep(1);
        client_process(atoi(argv[1]), adresses);
    }
    else { // error
        fprintf(stderr, "erreur lors de la creation du process fils.\n");
        exit(-1);
    }
    
    return (EXIT_SUCCESS);
}
