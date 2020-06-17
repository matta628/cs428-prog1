// UDP Pinger

#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h>

#define PORT	 12000

int main(int argc, char ** argv) { 
	int sockfd;
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr; 

	// Check for correct format
	if (argc != 2 ) return -1;
	
	// Create a UDP socket
	// Notice the use of SOCK_DGRAM for UDP packets
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&servaddr, 0, sizeof(servaddr)); 

	// Fill server information 
	servaddr.sin_family = AF_INET; // IPv4
	struct hostent * host = gethostbyname(argv[1]);
	servaddr.sin_addr = *(struct in_addr*)(host->h_addr);
	servaddr.sin_port = htons(PORT); // port number
	
	// Connect to the server socket
	connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

	// Set socket timeout time
	struct timeval  timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));

	// Write Message into Buffer
	strcpy(buffer, "ping");

	for (int i = 0; i < 10; i++){
		//Get beginning time
		clock_t ping = clock();

		//Send packet to server
		sendto(sockfd, (const char *)buffer, strlen(buffer), 
			   MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		
		//Receive the packet from server
		int rec = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 
			MSG_WAITALL, ( struct sockaddr *) &servaddr, &len);
		
		if (rec < 0){
			//Print Error Message
			printf("Packet Lost\n");
		}
		else{
			//Get end time
			clock_t pong = clock();
			//Print RTT Time
			printf("RTT: %.3f ms\n", (double)(pong - ping) * 1000 / CLOCKS_PER_SEC);
		}
	}
	return 0; 
}
