/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   net_interface_udp.h
 * Author: maignial
 *
 * Created on 10 novembre 2015, 13:57
 */

#ifndef NET_INTERFACE_UDP_H
#define NET_INTERFACE_UDP_H

#ifdef __cplusplus
extern "C" {
#endif

    
char** p2p_ping(int nb_client, int port);

int send(void* buffer, int size);

int recieve(void* buffer, int size);


#ifdef __cplusplus
}
#endif

#endif /* NET_INTERFACE_UDP_H */

