/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2020
##  This code is provided under the MIT license agreement.
######################################################################
*********************************************************************/
#ifndef CSOCKET_H
#define CSOCKET_H

#ifdef _MSC_VER
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "cring.h"

#ifdef __cplusplus
extern "C" {
#endif

#define csocket_open(F,T,P) socket(F,T,P)

#ifdef _MSC_VER
    typedef SOCKET csocket_t;
#define csocket_close(S) closesocket(S)
    typedef struct sockaddr_in csocket_addr;
#else
    typedef int csocket_t;
    typedef int cevent_t;
#define csocket_close(S) close(S)
    typedef struct sockaddr_in csocket_addr;
#endif

    int    csocket_bind     (csocket_t sock, const csocket_addr* addr);
    size_t csocket_sendto   (csocket_t sock, const csocket_addr * to, const void* buf, size_t len);
    size_t csocket_recvfrom (csocket_t sock, csocket_addr* from, void* buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif