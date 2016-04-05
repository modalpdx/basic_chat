//
// *****************************************************************************
// 
// Author:    Erik Ratcliffe
// Date:      July 26, 2015
// Project:   Program 1 - Chat server/client
// Filename:  chat.h
// Class:     CS 372 (Summer 2015)
//
//
// Overview:
//    Basic chat server/client combo. Server in C, client in Python.
//
//    This file contains variable definitions and function prototypes.
//
// *****************************************************************************
//

#ifndef CHAT_H
#define CHAT_H


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>


#define HDL_LEN 10             // Maximum handle length
#define PROMPT_CHARS "> "      // Prompt + space after handle
#define MSG_LEN 500            // Maximum message length
#define SVR_HANDLE "chatserve" // Handle on server-side. Must be no more
                               //    than HDL_LEN characters long!


// *****************************************************************************
// 
// int setupServer(int port)
//
//    Entry:   int port
//                Number of the port for the server
//
//    Exit:    Integer containing the socket descriptor.
//
//    Purpose: Setup the server connection and listen for clients.
//
// *****************************************************************************
//
int setupServer(int port);


// *****************************************************************************
// 
// int setupClient(int sock)
//
//    Entry:   int sock
//                Descriptor for the server socket.
//
//    Exit:    Integer containing the client descriptor.
//
//    Purpose: Setup the client connection to the server.
//
// *****************************************************************************
//
int setupClient(int sock);


// *****************************************************************************
// 
// void chatAction(int cli)
//
//    Entry:   int cli
//                Descriptor for the client.
//
//    Exit:    None.
//
//    Purpose: Actual chat functionality between server and client.
//
// *****************************************************************************
//
void chatAction(int cli);


// *****************************************************************************
// 
// void sendStr(int *sock, char *str)
//
//    Entry:   int *sock
//                Socket for the current network connection
//             char *str
//                String to send across the connection
//
//    Exit:    None.
//
//    Purpose: Send a string across a network connection.
//
// *****************************************************************************
//
void sendStr(int *sock, char *str);


// *****************************************************************************
// 
// int recvStream(int *sock, char *str, long maxChars)
//
//    Entry:   int *sock
//                Socket for the current network connection
//             char *str
//                Long string to receive from across the connection (see
//                recvStr() for receiving short strings).
//             long maxChars
//                Long integer containing the  number of characters expected
//                to arrive in the string.
//
//    Exit:    An integer containing the actual number of characters received,
//             or -1 if the client disconnected.
//
//    Purpose: Receive a long string from across a network connection.
//
// *****************************************************************************
//
int recvStream(int *sock, char *str, long maxChars);


// *****************************************************************************
// 
// void sigint_handler(int signum)
//
//    Entry:   int signum
//                Number representing the signal that was received (not used)
//
//    Exit:    None
//
//    Purpose: Perform actions when SIGINT is received.
//
// *****************************************************************************
//
void sigint_handler(int signum);


#endif
