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

#define PORT	 12000

int main() { 
	int sockfd, retvalue;
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr; 
	
	// Create a UDP socket
	// Notice the use of SOCK_DGRAM for UDP packets
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Fill server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(PORT); // port number
	
	// Connect to the server socket
	connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

	// Set socket timeout time
	struct timeval * timeout;
	timeout->tv_sec = 1;
	timeout->tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, timeout, sizeof(struct timeval));
	
	// Write Message into Buffer
	strcpy(buffer, "ping");

	for (int i = 0; i < 10; i++){
		//Get beginning time
		clock_t ping = clock();

		//Send packet to server
		sendto(sockfd, (const char *)buffer, strlen(buffer), 
			   MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		
		//Receive the packet from server
		retvalue = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 
			0, ( struct sockaddr *) &servaddr, &len);

		if (retvalue < 0)
			//Print Error Message
			printf("Packet Lost :,(\n");
		else{
			//Get end time
			clock_t pong = clock();
			//Print RTT Time 
			printf("RTT: %fs\n", (double)(pong - ping) / CLOCKS_PER_SEC);
		}
	}
	return 0; 
}
