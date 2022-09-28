/* Server Program (server.c)
You should implement a server program, called “server.c” in C on a UNIX system. Its
execution command should have the following structure:
server <UDP listen port>
Upon execution, the server should:
1. Open a UDP socket and listen at the specified port number
2. Receive a message from the client
a. if the message is “ftp”, reply with a message “yes” to the client.
b. else, reply with a message “no” to the client. */