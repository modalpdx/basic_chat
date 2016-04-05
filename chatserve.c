//
// *****************************************************************************
// 
// Author:    Erik Ratcliffe
// Date:      July 26, 2015
// Project:   Program 1 - Chat server/client
// Filename:  chatserve.c
// Class:     CS 372 (Summer 2015)
//
//
// Overview:
//    Basic chat server/client combo. Server in C, client in Python.
//
//    This file contains the main() code for the chat server.
//
// *****************************************************************************
//

#include "chat.h"


int main(int argc, char **argv)
{
    int sock, cli;  // Socket descriptors for server and client

    // If we did not get two items on our command line, vital information
    // is missing. Display a usage message and exit.
    //
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    // The server does not exit when clients disconnect; instead, SIGINT is
    // required. Use a signal handler to clean up whatever needs to be 
    // cleaned up (if anything) when the server recieves a SIGINT.
    //
    if(signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        perror("could not catch SIGINT");
        exit(1);
    }

    // Start the server and listen for connections.
    //
    fprintf(stderr, "Setting up a chat server on port %s.\n", argv[1]);
    sock = setupServer(atoi(argv[1]));

    // Only SIGINT will cleanly kill this server. Without it, keep running.
    //
    while(1)
    {
        fprintf(stderr, "Listening for a new connection...\n");

        // Setup a client connection.
        //
        cli = setupClient(sock);

        // Do a chat...
        //
        chatAction(cli);

        // Close the client 
        //
        fprintf(stderr, "Closing the connection...\n");
        close(cli);
        cli = -1;
    }

    // Close the server socket
    //
    close(sock);
    sock = -1;

    return 0;
}
