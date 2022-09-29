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

//Maximum packet size
#define BUFFER_SIZE 4096

void main(int argc, char const * argv[]){
	
	/****************************************Server Input Args********************************************/
	//Check program usage
	if (argc != 2) {
		printf("Incorrect usage.\nUsage: server <server port num>\n");
		exit(1);
  	}

	//Read port argument
	int port = 0;
	printf("Port: %d \n", atoi(argv[1]));
	port = atoi(argv[1]);

	/****************************************Socket Creation********************************************/
	//Create IPV4, UDP socket
	int sockfd = 0; 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	//Check if socket creation was succesful
	if (sockfd < 0){
		perror("socket creation failed"); 
        exit(1);
    } 

	/****************************************Server / Client Address********************************************/
	//Init the socket addresses
	struct sockaddr_in servaddr, cliaddr;
	memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 

	//Server settings
	servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(port);

	// Bind the socket w/ addresses
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
        perror("Socket bind failed"); 
        exit(1); 
    } 

	/****************************************Main Server Loop********************************************/
	//Begin loop to listen at port
	while(1){
		printf("Listening at port: %d\n", port);

		/****************************************Recieve message from cient********************************************/
		//Length of client address
		socklen_t cli_len = sizeof(cliaddr);

		//return buffer -> recive message
		char mssg[BUFFER_SIZE];
		bzero(mssg, BUFFER_SIZE);

		//Receive message 
		if (recvfrom(sockfd, mssg, BUFFER_SIZE, 0, (struct sockaddr*)&cliaddr, &cli_len) < 0) {
			printf("Message was not recieved\n");
			exit(1);
		}

		//Print return buffer
		printf("Client : %s\n", mssg);

		/****************************************Send response to client********************************************/
		//Check message from client
		char *response;
		if(strcmp(mssg, "ftp") == 0){
			response = "yes"; 
		} else {
			response = "no"; 
		}

		if(sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (struct sockaddr*)&cliaddr, cli_len) < 0){
			printf("Message was not sent\n");
		} else {
			printf("Message: '%s' sent\n", response);
		}
	}

	printf("Hello World");
	return;
}
