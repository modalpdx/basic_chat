//
// *****************************************************************************
// 
// Author:    Erik Ratcliffe
// Date:      July 26, 2015
// Project:   Program 1 - Chat server/client
// Filename:  chat_func.c
// Class:     CS 372 (Summer 2015)
//
//
// Overview:
//    Basic chat server/client combo. Server in C, client in Python.
//
//    This file contains functions that are called by main(). They are
//    here to keep main() tidy.
//
// *****************************************************************************
//

#include "chat.h"  // Specifications for functions are in this header


// *****************************************************************************
// 
// int setupServer(int port)
//
// Purpose: Setup the server connection and listen for clients.
//
// *****************************************************************************
//
int setupServer(int port)
{
    int    sock;               // Socket descriptor for server
    int    optval;             // Holds option values from setsockopt()
    struct sockaddr_in myServ; // Info describing server sockets

    // Set up the socket: AF_INET for Internet domain, SOCK_STREAM for
    // TCP, 0 for default Internet protocol. If this was a UDP socket,
    // SOCK_STREAM would be SOCK_DGRAM instead.
    //
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    { 
        perror("Socket failed");
        exit(1);
    }

    // This is rumored to help with the "address in use" errors that
    // sometimes pop up when trying to start a server on a busy socket. I
    // have found it works sometimes but not always. It is possible my
    // syntax is incorrect but based on everything I have read it is not.
    // I am leaving it here for future experimentation.
    //
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("Setsockopt failed");
        exit(1);
    }

    // Initialize socket structure with zeroes (once handled by bzero()
    // which has been deprecated).
    // 
    memset((char *)&myServ, 0, sizeof(myServ));

    // Use the server hostent information to help populate the sockaddr_in
    // struct that holds server connection data. Set the domain to
    // Internet (AF_INET), convert the port passed on the command line
    // from host to network byte ordering and assign to the sockaddr_in
    // port, and indicate that connections from any address on our network
    // are okay (INADDR_ANY).
    //
    myServ.sin_family      = AF_INET;
    myServ.sin_port        = htons(port); // host to network endian conversion
    myServ.sin_addr.s_addr = htonl(INADDR_ANY);

    // Associate the address assocated with myServ with the socket for
    // this server.
    //
    if(bind(sock, (struct sockaddr *)&myServ, sizeof(myServ)) == -1)
    {
        perror("Bind failed");
        exit(1);
    }

    // Listen for connections.
    //
    if(listen(sock, 1) == -1)
    {
        perror("Listen failed");
        exit(1);
    }

    return sock;
}


// *****************************************************************************
// 
// int setupClient(int sock)
//
// Purpose: Setup the client connection to the server.
//
// *****************************************************************************
//
int setupClient(int sock)
{
    int       cli;                // Socket descriptor for the client
    socklen_t myCliLen;           // Holds size of client socket info
    struct    sockaddr_in myCli;  // Info describing client socket

    myCliLen = sizeof(myCli); // Grab the size of the myCli sockaddr_in struct 

    // Wait for a client connection. If one is requested, accept it and 
    // return a socket descriptor for the new connection. If it fails,
    // exit with an error. 
    //
    cli = accept(sock, (struct sockaddr *)&myCli, &myCliLen);
    if(cli == -1)
    {
        perror("Accept failed");
        exit(1);
    }

    return cli;
}


// *****************************************************************************
// 
// void chatAction(int cli)
//
// Purpose: Actual chat functionality between server and client.
//
// *****************************************************************************
//
void chatAction(int cli)
{
    char userInput[MSG_LEN + 1]; // Chat message input on server side
    int  actualRecv;             // Total chars from a string transfer

    // Strings for incoming (from client) and outgoing (from server) messages.
    // These strings will include the following:
    //
    // - user "handle"
    // - prompt characters
    // - a chat message
    // - a null terminator
    //
    char inMsg[HDL_LEN + strlen(PROMPT_CHARS) + MSG_LEN + 1];
    char outMsg[HDL_LEN + strlen(PROMPT_CHARS) + MSG_LEN + 1];

    // While the connection is not manually closed on the server side
    // and the client has not closed the connection on its side...
    //
    do
    {
        // Set the first character to a null terminator, just to be sure 
        // we're starting with fresh strings.
        //
        inMsg[0]     = '\0';
        outMsg[0]    = '\0';
        userInput[0] = '\0';

        // Get an incoming message from the client.
        //
        actualRecv = recvStream(&cli, inMsg, sizeof(*inMsg));

        // recvStream will return -1 if the client disconnects. When
        // this happens, stop pretending a connection is still up. Allow
        // the server to fully close the connection and start listening
        // again for new connection requests.
        //
        if(actualRecv >= 0)
        {
            // Add a null terminator
            //
            inMsg[actualRecv] = '\0';

            // Display the incoming message.
            //
            printf("%s\n", inMsg);

            // Prompt for and read an outgoing message.
            //
            do{
                printf("%s%s", SVR_HANDLE, PROMPT_CHARS);
                fgets(userInput, MSG_LEN, stdin);

                // If the message is too long, say so.
                //
                if(strlen(userInput) > MSG_LEN)
                {
                    printf("Message length is limited to %d characters.\n", MSG_LEN);
                }
            } while(strlen(userInput) > MSG_LEN);

            // Strip out trailing newline in userInput (eliminates an
            // annoying newline at the client side).
            //
            userInput[strlen(userInput) - 1] = '\0';

            // If the connection is not being closed on the server side,
            // build the complete outgoing message string and send it to
            // the client.
            //
            if(strncmp(userInput, "\\quit", 5) != 0)
            {
               strncat(outMsg, SVR_HANDLE, strlen(SVR_HANDLE));
               strncat(outMsg, PROMPT_CHARS, strlen(PROMPT_CHARS));
               strncat(outMsg, userInput, strlen(userInput));
 
               // Send message to client
               //
               sendStr(&cli, outMsg);
            }
        }

    } while((strncmp(userInput, "\\quit", 5) != 0) && (actualRecv != -1));
}


// *****************************************************************************
// 
// void sendStr(int *sock, char *str)
//
// Purpose: Send a string across a network connection.
//
// *****************************************************************************
//
void sendStr(int *sock, char *str)
{
    long len;     // Holds the length of the input string
    int  numSent; // Characters transferred

    // Get the input string length.
    //
    len = strlen(str);

    // Send the string, report the number of characters transferred. If
    // -1, exit with an error.
    //
    if((numSent = send(*sock, str, len, 0)) == -1)
    {
        perror("client send failed");
        exit(1);
    }
}


// *****************************************************************************
// 
// int recvStream(int *sock, char *str, long maxChars)
//
// Purpose: Receive a potentially long string from across a network connection.
//
// *****************************************************************************
//
int recvStream(int *sock, char *str, long maxChars)
{
    int  numRecv    = 0;  // Characters transferred per recv() call
    long actualRecv = 0;  // Actual accumulated characters transferred

    // Temp. buffer used by recv()
    //
    char buf[HDL_LEN + sizeof(PROMPT_CHARS) + MSG_LEN]; 

    // Start at the beginning of the string we are receiving.
    //
    str[0] = '\0';

    // Keep looping until the entire input stream is received. 
    //
    // The actualRecv var should never be -1 without being set that way
    // intentionally. Detecting and dealing with the client closing the
    // connection can be tricky. It's easiest (and works just fine) if
    // actualRecv is set to -1 when the client disconnects and then -1 is
    // detected and dealt with by the calling function.
    //
    while((actualRecv < maxChars) && (actualRecv != -1))
    {
       // Read the string, report the number of characters transferred. If
       // -1, exit with an error. If 0, we're done. Otherwise, concatenate
       // the contents of buf to the final string and add the number of
       // characters transferred to the running count (actualRecv).
       //
       if((numRecv = recv(*sock, buf, sizeof(buf), 0)) == -1)
       {
           perror("server recv failed (message)");
           exit(1);
       }
       else if(numRecv == 0)
       {
           perror("socket closed during recv (cli)");

           // Change to -1 to make it abundantly clear that the client
           // disconnected. Removes all ambiguity.
           //
           actualRecv = -1;
       }
       else 
       {
           // Concatenate the buffer content to the string collector 
           // passed in the argument list. Update the actual number of
           // characters received.
           //
           strncat(str, buf, numRecv);
           actualRecv += numRecv;
       }
    }

    return actualRecv; // Return the actual number of characters received or
                       // -1 if the client disconnected.
}


// *****************************************************************************
// 
// void sigint_handler(int signum)
//
// Purpose: Perform actions when SIGINT is received.
//
// *****************************************************************************
//
void sigint_handler(int signum)
{
    // If SIGINT is caught, just exit with a message. We're in a signal
    // handler, so printf() won't work. Use write() instead.
    //
    write(STDERR_FILENO, "SIGINT caught, exiting server...\n", 34);
    exit(1);
}

