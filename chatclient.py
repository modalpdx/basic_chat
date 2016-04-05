#
# *****************************************************************************
# 
# Author:    Erik Ratcliffe
# Date:      July 26, 2015
# Project:   Program 1 - Chat server/client
# Filename:  chatclient.py
# Class:     CS 372 (Summer 2015)
#
#
# Overview:
#    Basic chat server/client combo. Server in C, client in Python.
#
#    This file contains chat client code.
#
# *****************************************************************************
#

import socket
import errno
import sys

#
# *****************************************************************************
#
# serverConnect(serverName, serverPort)
#
#    Entry:   serverName
#                String containing server hostname
#             serverPort
#                Number of the port for the server.
#
#    Exit:    Returns a socket object for the client/server connection
#
#    Purpose: Connect to the chat server, create a socket object for the
#             connection.
#
# *****************************************************************************
#
def serverConnect(serverName, serverPort):
    # Create the socket
    #
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Connect to the server, or die trying.
    #
    try:
        clientSocket.connect((serverName, serverPort))
    except socket.error, errmsg:
        print "Connect failed: %s\nExiting..." % errmsg
        exit(1)

    return clientSocket


#
# *****************************************************************************
#
# sendMsg(chatMsg, sock)
#
#    Entry:   chatMsg
#                String containing full message to send.
#             sock
#                Socket object for the client/server connection.
#
#    Exit:    None.
#
#    Purpose: Send a chat message to the chat server.
#
# *****************************************************************************
#
def sendMsg(chatMsg, sock):
    # Send an outgoing message, or die trying.
    #
    try:
        sock.send(chatMsg)
    except socket.error, errmsg:
        print "Send failed: %s\nExiting..." % errmsg
        exit(1)
        

#
# *****************************************************************************
#
# recvMsg(sock)
#
#    Entry:   sock
#                Socket object for the client/server connection.
#
#    Exit:    Returns a socket object for the client/server connection
#
#    Purpose: Connect to the chat server, create a socket object for the
#             connection.
#
# *****************************************************************************
#
def recvMsg(sock):
    # Receive an incoming message, or die trying.
    #
    try:
        inChatMsg = sock.recv(1024) # Read the incoming message into inChatMsg
    except socket.error, errmsg:
        print "Read failed: %s\nExiting..." % errmsg
        exit(1)

    return inChatMsg  # The length of the message string will be used by
                      # the calling function to determine if the connection 
                      # was closed by the server. We must return it, not
                      # process or display it here.


# *****************************************************************************
#
# End of function definitions
#
# *****************************************************************************


# Check for insufficient or incorrect arguments.
# If there is a problem, display usage and exit.
#
if len(sys.argv) < 3 or int(sys.argv[2]) < 1 or int(sys.argv[2]) > 65535:
    print 'Usage: chatclient.py [hostname] [port number]'
    print '       (hostname = string, port number 0-65534)'
    exit()

# Get the user's handle, limited to 10 characters
#
while True:
    handle = raw_input('Enter a handle (10 characters max): ')
    if len(handle) > 0 and len(handle) <= 10:
        break
    else:
        print 'Incorrect number of characters!'

# Concatenate the prompt character and a space. We use this as a prompt
# and it is sent along with each outgoing chat messages, so it makes sense
# to just have it all in one string (rather than build it over and over again).
#
handle += '> '

# Set up server host and port from command line arguments.
#
serverName = sys.argv[1]
serverPort = int(sys.argv[2]) 

# Connect to the server, assign connection to clientSocket
#
clientSocket = serverConnect(serverName, serverPort)

try:
    while True:
    
        # Get a chat message.
        outChatMsg = raw_input(handle)
    
        # Make sure the message length does not exceed 500 characters.
        if len(outChatMsg) > 500:
            print 'Message length is limited to 500 characters.'
            continue
    
        # If '\quit' was entered, close the connection.
        #
        if outChatMsg == '\quit':
            break
    
        # Concatenate the outgoing message with the client handle.
        #
        fullChatMsg = handle + outChatMsg
        
        # Send the chat message.
        #
        sendMsg(fullChatMsg, clientSocket)

        # Read a chat message from the server and display it.
        #
        inChatMsg = recvMsg(clientSocket)

        # If we received 0 bytes, the server has disconnected.
        #
        if len(inChatMsg) == 0:
            break

        # If we're still here (not disconnected), print the incoming message.
        #
        print inChatMsg
    
finally:
    # Close the connection.
    #
    clientSocket.close()

