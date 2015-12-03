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
    
int port_number;
    
int p2p_tcp_connect (char* addr);
int p2p_get_bloc (int server_sock, int block_num, char* filename);
void p2p_scan_for_servers (int* servers_sockets, int nb_servers, char* filename, char** adresses);

#ifdef __cplusplus
}
#endif

#endif /* TCP_C_H */

