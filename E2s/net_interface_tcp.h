/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   net_interface_udp.h
 * Author: Benjamin
 *
 * Created on 10 novembre 2015, 13:57
 */

#ifndef NET_INTERFACE_UDP_H
#define NET_INTERFACE_UDP_H

#define _XOPEN_SOURCE >= 500
#define BLOCK 4096
#define SEPARATOR '!'
#define UNCONNECT "EXIT!"
#ifdef __cplusplus
extern "C" {
#endif
#define MAX_LINE 10   
int tcps_file(int port);

int tcps_send(char* buffer, int size);

int tcps_recv(char* buffer, int size);

#ifdef __cplusplus
}
#endif

#endif /* NET_INTERFACE_UDP_H */

