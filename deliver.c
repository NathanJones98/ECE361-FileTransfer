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
#include <stdbool.h>
//Networking include statements
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include "packet_format.h"
//Maximum packet size
#define BUFFER_SIZE 4096
#define FRAGMENT_SIZE 1000
//Temp Main
void main(int argc, char const * argv[]){

	if (argc != 3) {
		printf("Incorrect usage.\nUsage: deliver <server address> <server port num>\n");
		return;
  	}
	
	char FileName[255];
	char * mssg;
    FILE *binary_file;

    printf("Enter file name: ");
    scanf("%s", FileName);
    printf("\n Filename: ++%s++\n",FileName);


	char FilePath [263];
	strcpy(FilePath, "./deliver_/" );
	strcat(FilePath,FileName);
	printf("\n FilePath: ++%s++",FilePath);


	if (binary_file = fopen(FilePath,"r"))
    {
        mssg = "ftp"; 
		printf("\nFile exists");
    }
    else 
	{
		printf("\nFile does not exists");
        exit(1);
	}

	binary_file = fopen(FilePath,"rb");
	fseek(binary_file, 0L, SEEK_END);
  
    long int size = ftell(binary_file);
	rewind(binary_file);
	int num_packets = (size + FRAGMENT_SIZE-1) / FRAGMENT_SIZE;

	int offset = size%FRAGMENT_SIZE;
	if (offset = 0)
		offset = FRAGMENT_SIZE;

	unsigned char fragment[FRAGMENT_SIZE];

	char output [2000];

	struct packet_format Packet;
	//memset(&Packet, 0, sizeof(Packet)); 
	Packet.total_frag = num_packets;
	Packet.frag_no = 0;
	Packet.size = 0;
	Packet.filename = FileName;
	//strcpy(Packet.filename,FileName);

	



	//Read server address
	//char * s_addr = 0;
	printf("\ns_addr: %d \n", argv[1]);
	//s_addr = argv[1];

	//Read port argument
	int port = 0;
	printf("Port: %d \n", atoi(argv[2]));
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
    char mssg_IN[20];
	bzero(mssg_IN, 20);
	
    


	
	sendto(sockfd, (const char *)mssg, strlen(mssg), 
            MSG_CONFIRM, servinfo->ai_addr,  
            servinfo->ai_addrlen);
            socklen_t servlen = sizeof(servaddr);      
    printf("FTP message sent\n");

    if (recvfrom(sockfd, mssg_IN, 20, 0, (struct sockaddr*)&servaddr, &servlen) < 0) {
			printf("Message was not recieved\n");
			exit(1);
	}
	else 
	    printf("received: %s\n",mssg_IN);
    if (strcmp("yes", mssg_IN)==0) 
        printf("A file transfer can start.\n"); 
    else 
        exit (1);

	bool re_started = false;
	int i = 0;
	printf("loop begins%d\n",i);
 
	while ( i<num_packets ){	
	
	printf("loop itteration: %d", i); 
	
		if (!re_started)
		{
			printf("loop itteration");
			if (i < num_packets-1)
			{
				fread(fragment,sizeof(fragment),1,binary_file);
				sprintf(output,"%d:%d:%d:%s:%u",num_packets,i+1,FRAGMENT_SIZE,FileName,fragment);
			}

			else 
			{
				fread(fragment,offset,1,binary_file);
				sprintf(output,"%d:%d:%d:%s:%u",num_packets,i+1,offset,FileName,fragment);
			}
			printf("message changed\n");
		}
		
		printf("sending...\n");
		sendto(sockfd, (const char *)output, strlen(output), 
            MSG_CONFIRM, servinfo->ai_addr,  
            servinfo->ai_addrlen);
		printf("message sent\n");

		if (recvfrom(sockfd, mssg_IN, 20, 0, (struct sockaddr*)&servaddr, &servlen) < 0) 
		{
			printf("Message was not recieved\n");
			re_started = true;
		}
		else
		{
			if (strcmp("yes", mssg_IN)==0) 
			{
        		printf("A packet transfer was confirmed."); 
				i++;
				re_started = false;
			}
    		else 
			{
				printf("A packet transfer was not confirmed.");
				re_started = true;
			}
		}


	}

	printf("Hello World 2");
	return;
}
