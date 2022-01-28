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

#ifdef __cplusplus
extern "C" {
#endif

#define csocket_open(F,T,P) socket(F,T,P)

#ifdef _MSC_VER
    typedef SOCKET csocket_t;
#define csocket_close(S) closesocket(S)
#else
    typedef int csocket_t;
#define csocket_close(S) close(S)
#endif

#ifdef __cplusplus
}
#endif

#endif