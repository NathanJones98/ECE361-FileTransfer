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

//Include statements
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Networking include statements
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h>

//Maximum packet size
#define BUFFER_SIZE 4096

//Temp Main
void main(int argc, char const * argv[]){

	if (argc != 3) {
		printf("Incorrect usage.\nUsage: deliver <server address> <server port num>\n");
		return;
  	}
	
	//Read server address
	int s_addr = 0;
	printf("s_addr: %d \n", atoi(argv[1]));
	s_addr = atoi(argv[1]);

	//Read port argument
	int port = 0;
	printf("Port: %d \n", atoi(argv[1]));
	port = atoi(argv[2]);

	//Init the socket addresses
	struct sockaddr_in servaddr;
	struct addrinfo *servinfo;
	struct addrinfo hints;

	memset(&servaddr, 0, sizeof(servaddr));  
	memset(&servinfo, 0, sizeof(servinfo)); 
	memset(&hints, 0 , sizeof (hints));

	//Server settings
	servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(port);

	//Hints
	hints.ai_family = AF_INET;
  	hints.ai_socktype = SOCK_DGRAM;

	//Lookup address via dns
	int addr_info;
	if ((addr_info = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) < 0) {
		printf("Lookup via DNS failed\n");
		exit(1);
	}

	//Length of client address
	int sockfd = 0; 
	socklen_t ser_len = sizeof(servaddr);
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	//Check if socket creation was succesful
	if (sockfd < 0){
		perror("socket creation failed"); 
        exit(1);
    } 

	//Send message
	char *hello = "Hello from client"; 
	sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr));

	printf("Hello World 2");
	return;
}