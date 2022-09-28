/* Server Program (server.c)
You should implement a server program, called “server.c” in C on a UNIX system. Its
execution command should have the following structure:
server <UDP listen port>
Upon execution, the server should:
1. Open a UDP socket and listen at the specified port number
2. Receive a message from the client
a. if the message is “ftp”, reply with a message “yes” to the client.
b. else, reply with a message “no” to the client. */

//Include statements
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Networking include statements
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 

void main(int argc, char const * argv[]){
	
	//Check program usage
	if (argc != 2) {
		printf("Incorrect usage.\nUsage: server <server port num>\n");
		return;
  	}

	//Read argument
	int port = 0;
	printf("Port: %d \n", atoi(argv[1]));
	port = atoi(argv[1]);

	//Create IPV4, UDP socket
	int sockfd = 0; 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	//Bind Socket
	struct sockaddr_in servaddr, cliaddr;

	//Check if socket creation was succesful
	if (sockfd < 0){
		perror("socket creation failed"); 
        exit(1);
    } 

	//Server settings
	servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(port);

	//int bind(sockfd, const struct sockaddr *addr, socklen_t addrlen)

	printf("Hello World");
	return;
}
