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

#define _XOPEN_SOURCE >= 500

#ifdef __cplusplus
extern "C" {
#endif
#define MAX_LINE           (10)    
int tcps_file(char * file,char * ip,int port);

int tcps_send(char* buffer, int size);

int tcps_send(char* buffer, int size);


#ifdef __cplusplus
}
#endif

#endif /* NET_INTERFACE_UDP_H */
