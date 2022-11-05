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
#include <time.h>

//Networking include statements
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include "packet_format.h"


 
//Maximum packet size
#define FRAGMENT_SIZE 1000

/****************************************Main Loop********************************************/



void main(int argc, char const * argv[]){

	time_t t;
	srand((unsigned) time(&t));
	if (argc != 3) {
		printf("Incorrect usage.\nUsage: deliver <server address> <server port num>\n");
		return;
  	}
	
	/****************************************Input********************************************/
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

	/****************************************Open File Inputted********************************************/
	
	//Check if file exists
	if (binary_file = fopen(FilePath,"r")){
        mssg = "ftp"; 
		printf("\nFile exists");
    } else {
		printf("\nFile does not exist\n");
        exit(1);
	}
	
	//Open file
	binary_file = fopen(FilePath,"rb");

	/****************************************File Parameters********************************************/
	//Size
	fseek(binary_file, 0L, SEEK_END);
    long int size = ftell(binary_file);
	fseek(binary_file, 0L, SEEK_SET);

	//Total Fragments
	//int num_packets = (size + FRAGMENT_SIZE-1) / FRAGMENT_SIZE;

	//Offset
	//int offset = size%FRAGMENT_SIZE;
	//if (offset == 0)
		//offset = FRAGMENT_SIZE;

	
	//char output [2000];

	//Packet struct
	struct packet_format Packet;
	int estimated_num_packets = 1 + (size /(FRAGMENT_SIZE- (4*sizeof(char)+ 3*sizeof(int) + strlen(FileName))));
	//memset(&Packet, 0, sizeof(Packet)); 
	Packet.total_frag = estimated_num_packets;
	Packet.frag_no = 0;
	Packet.size = 0;
	Packet.filename = FileName;
	//strcpy(Packet.filename,FileName);

	/****************************************Server Parameters********************************************/
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

	/****************************************DNS lookup********************************************/
	//Lookup address via dns
	int addr_info;
	if ((addr_info = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) < 0) {
		printf("Lookup via DNS failed\n");
		exit(1);
	}

	/****************************************Server Connection********************************************/
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
	
//Time at first message to server
//#ifdef TIMER
	double time = (double)(clock());
//#endif

	/****************************************Send Message to server********************************************/
	sendto(sockfd, (const char *)mssg, strlen(mssg), 
            MSG_CONFIRM, servinfo->ai_addr,  
            servinfo->ai_addrlen);
            socklen_t servlen = sizeof(servaddr);      
    printf("FTP message sent\n");

	/****************************************Recieve Message from server********************************************/
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

//Time when we recieve message from server
//#ifdef TIMER
	time = (double)(clock()) - time;
	printf("Round trip time: %.3f ms\n", ((float)time * 1000) / CLOCKS_PER_SEC);
//#endif

	/****************************************File Trasnfer Main loop********************************************/
	bool re_started = false;
	
	char pack_buf[estimated_num_packets][FRAGMENT_SIZE];
	//printf("loop begins%d\n",i);
	
    //static int timeout = TIMEOUT_MS;
    //setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));
	char fragment[FRAGMENT_SIZE];
	int bytes_sent = 0;
	int num_packets = 0;
	int i = 0;
	while ( bytes_sent<size ){	
	
		
		//printf("loop itteration: %d\n", i); 
	
		
		memset(fragment, 0, FRAGMENT_SIZE);
		//memset(pack_buf, 0, 2*FRAGMENT_SIZE);
		//printf("loop itteration\n");
			
				
		unsigned int header = sprintf(pack_buf[num_packets], "%d:%d:%d:%s:", estimated_num_packets,i+1,(FRAGMENT_SIZE- (4*sizeof(char)+ 3*sizeof(int) + strlen(FileName))),FileName);
		header = sprintf(pack_buf[num_packets], "%d:%d:%d:%s:", estimated_num_packets,i+1,FRAGMENT_SIZE - header,FileName);
		//printf("header created\n");
		int size_ = fread(fragment,sizeof(char),FRAGMENT_SIZE-header, binary_file);
		//printf("data created\n");
		if (size_<1)
			break;
				
   		//printf("header: %s\n",pack_buf);
		memcpy(pack_buf[num_packets] + header, fragment, size_ );
		num_packets++;
		bytes_sent += FRAGMENT_SIZE-header;
		i++;
			
		
	}
	float  timeout = 0;
	int  ratio = 5;
	i = 0;
	while (i < num_packets){
		//#ifdef TIMER
		timeout =((float)time * 1000) / CLOCKS_PER_SEC;
		//printf("Round trip time: %.3f ms\n", ((float)time * 1000) / CLOCKS_PER_SEC);
		//#endif
		
		printf("\n\n Sending Packet\n", i);
		//printf("sending...\n");
		sendto(sockfd, (const char *)pack_buf[i], sizeof(pack_buf[i]), 
            MSG_CONFIRM, servinfo->ai_addr,  
            servinfo->ai_addrlen);

		printf("Sent Packet\n", i);
		//printf("time: %d\n", timeout);


		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = timeout;
		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));

		printf("\n\n Waiting for Ack\n", i);
		if (recvfrom(sockfd, mssg_IN, 20, 0, (struct sockaddr*)&servaddr, &servlen) < 0) 
		{
			printf("Timeout passed, Packet: %d \n", i);
			printf("A packet transfer was not confirmed, sending again.\n");
		}
		else
		{
			if (strcmp("yes", mssg_IN)==0) 
			{
        		printf("A packet transfer was confirmed.\n"); 
				i++;
			}
    		else 
			{
				printf("A packet transfer was not confirmed.\n");
			}
		}
		//printf("end of loop\n");
		//#ifdef TIMER
  		time = (double)(clock()) - time;
		//#endif
  		//printf("Round trip time: %.3f ms\n", ((float)time * 1000) / CLOCKS_PER_SEC);
		

	}
	char * final = "finished";
	sendto(sockfd, (const char *)pack_buf[i], sizeof(pack_buf[i]), 
            MSG_CONFIRM, servinfo->ai_addr,  
            servinfo->ai_addrlen);
	printf("done, closing\n");
	return;
}
