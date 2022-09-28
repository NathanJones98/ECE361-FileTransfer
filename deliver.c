/* Client Program (deliver.c)
deliver <server address> <server port number>
After executing the server, the client should:
1. Ask the user to input a message as follows:
ftp <file name>
2. Check the existence of the file:
a. if exist, send a message “ftp” to the server
b. else, exit
3. Receive a message from the server:
a. if the message is “yes”, print out “A file transfer can start.”
b. else, exit*/