/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tcp_c.h
 * Author: maignial
 *
 * Created on 19 novembre 2015, 14:21
 */

#ifndef TCP_C_H
#define TCP_C_H

#ifdef __cplusplus
extern "C" {
#endif

#define DATA_BLOCK 4096
#define SCAN_FOR_FILE "SCAN"
#define HAVE_FILE "OK"
#define NO_FILE "NO_FILE"
#define END_FILE "EXIT"
    
typedef struct block_s {
    char data[DATA_BLOCK];
    int block_size;
} block_t;
    
int port_number;
    
int p2p_tcp_connect (char* addr);
void p2p_scan_for_servers (int* servers_sockets, int nb_servers, char* filename, char** adresses);
int tcps_sendblock(char * file,int block_num);
void p2p_get_file(char* filename, int* servers, int nb_servers);
int tcps_file(int port);

#ifdef __cplusplus
}
#endif

#endif /* TCP_C_H */

