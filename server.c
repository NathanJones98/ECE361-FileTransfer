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
#include <stdbool.h>

//Networking include statements
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "packet_format.h"

//Maximum packet size
#define BUFFER_SIZE 4096


/****************************************Function for parsing packets********************************************/
bool Packet_Parcing (const char * Packet, struct packet_format *packet_data, FILE *binary_file)
{
	char * delimiter = ":";

	//Total Fragments
	int n_packets =  atoi(strtok(Packet, delimiter));
	printf("Total Frag\n");
	if (n_packets== NULL)
		return false;
	
	//Init server side recieving packet_format struct
	else if (packet_data->total_frag == 1)
		packet_data->total_frag = n_packets;
	printf("Total Frag: %d control: %d\n", packet_data->total_frag, n_packets);
	//Total Frag check
	if (packet_data->total_frag != n_packets)
		return false;
	printf("Total Frag\n");
	
	//Frag Number
	packet_data->frag_no = atoi(strtok(NULL, delimiter));
	if (packet_data->frag_no == NULL)
		return false;
	printf("Frag #\n");
	
	//Size of packet
	packet_data->size = atoi(strtok(NULL, delimiter));
	if (packet_data->size == NULL)
		return false;
	
	printf("Size\n");
	//Filename
	packet_data->filename = strtok(NULL, delimiter);
	if (packet_data->filename == NULL)
		return false;
	printf("FileName\n");
	
	//Data
	strcpy(packet_data->filedata , strtok(NULL, delimiter));
	if (packet_data->filedata == NULL)
		return false;
	printf("Data\n");
	
	//Opens a file to write to with the file name
	char FilePath [263];
	strcpy(FilePath, "./server_/" );
	strcat(FilePath, packet_data->filename);

	binary_file = fopen(FilePath, "a");
	//printf("Size of file data: %d\n", strlen(packet_data->filedata));
	//printf("File Data: %s\n", packet_data->filedata);
	
	fwrite(packet_data->filedata, packet_data->size, packet_data->size, binary_file);
	//fwrite(fragment_binary, 1, strlen(fragment_binary), binary_file);
	//remove(packet_data->filename)

	printf("Write\n");
	fclose(binary_file);
	return true;
}

/****************************************Main Loop********************************************/
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

	struct packet_format packet_data; 
	packet_data.total_frag = 1;
	packet_data.frag_no = 0;
	packet_data.size = 0;

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
		} else {
			//Print return buffer
			printf("Client : %s\n", mssg);
		}

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
			exit(1);
		} else {
			printf("Message: '%s' sent\n", response);

			/****************************************File Transfer started********************************************/
			//Packet.filename 
			FILE *binary_file;
			memset(&binary_file, 0, sizeof(binary_file)); 
			while (packet_data.frag_no<packet_data.total_frag){
				
				//Init message
				bzero(mssg, BUFFER_SIZE);

				//Receive message 
				if (recvfrom(sockfd, mssg, BUFFER_SIZE, 0, (struct sockaddr*)&cliaddr, &cli_len) < 0) {
					printf("Message was not recieved\n");
					exit(1);
				}
				printf("\n\nClient message: %s\n\n", mssg);

				//parse and validate
				char *response_;
				if(Packet_Parcing(mssg, &packet_data, binary_file)){
					response_ = "yes"; 
				} else {
					response_ = "no"; 
				}
				printf("%s\n",response_);
				if(sendto(sockfd, (const char *)response_, strlen(response_), MSG_CONFIRM, (struct sockaddr*)&cliaddr, cli_len) < 0){
					printf("Message was not sent\n");
				} else {
					printf("Message: '%s' sent\n", response_);
				}
			}

			//done
			printf("files transfer completed\n");
			
			/*for (;;) {

				bzero(mssg, BUFFER_SIZE);
				if (recvfrom(sockfd, mssg, BUFFER_SIZE, 0, (struct sockaddr*)&cliaddr, &cli_len) < 0) {
					printf("Message was not recieved\n");
					exit(1);
				} else {
					//Print return buffer
					printf("Client : %s\n", mssg);

					if(sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (struct sockaddr*)&cliaddr, cli_len) < 0){
						printf("Message was not sent\n");
						exit(1);
					} else {
						printf("Message: '%s' sent\n", response);
					}
				}
			}*/
		}

	}
	return;
}



