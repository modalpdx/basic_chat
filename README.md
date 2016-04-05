# Basic Chat client/server
A school project for a simple chat server/client pair. Mostly an exercise in
exchanging messages via sockets.


##About the server:

The chat server is written in C. The source code is in the following
files:

- chatserve.c
- chat_func.c
- chat.h

To build the server, just run the compileall shell script while in the
same directory as the forementioned source files:

```
  ./compileall
```

This should result in a binary called chatserve. To start the server,
run the chatserve program and pass an available port number as its first
(and only) argument. Example:

```
  ./chatserve 54321
```

Note: the chatserve program must be running on the server before running
the client.

The chatserve program only allows one client at a time, but it will cycle
itself to accept a new connection when either the client or server side of
the chat disconnects. To disconnect from the server side, enter "\quit"
(no quotes) as your chat message and hit ENTER. The client will
automatically disconnect and quit if the server breaks the connection.

To completely quit the chat server program, send a SIGINT signal either
with the UNIX "kill" command or by hitting CTRL-C.

When the server connects with a client, after the first message is received it
will be displayed and a chat prompt on the server side will be presented (the
server's handle is hardcoded as "chatserve"). From that point, you can take
turns trading chat messages (500 characters or less) with the chat client
until either side quits.


##About the client:

The client is written in Python. There are no build steps involved. 

To start the client, pass the chatclient.py script to Python and specify
the server's hostname (string, not IP address) and port number as its
first (and only) two arguments. Example:

```
  python chatclient.py hostname 54321
```

You will be prompted to enter a user "handle" (10 characters or less). After
you enter the handle, you will be presented with a chat prompt. Start the
chat by typing a message (500 characters or less) and hit ENTER.

Note: the chatserve program must be running on the server before running
the client.

To disconnect from the server, enter "\quit" (no quotes) as your chat
message and hit ENTER. When you disconnect, the client program will exit
but the server will continue to run.

##Disclaimer:

This is code from a school project. It satisfies assignment requirements
but is nowhere near as "scrubbed" as released software should be.
Security is not addressed, only functionality and no input
validation. If you use this code for anything other than satisfying your
curiosity, please keep the following in mind:

- there is no warranty of any kind (you use the code as-is)
- there is no support offered, please do not ask
- there is no guarantee it'll work, although it's not complex so it should
  work
- please do not take credit for code you did not write, especially if you
  are a student. NO CHEATING.

Thanks!
