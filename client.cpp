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

	std::cout << "fill successful\n";
	
	// Connect to the server socket
	int con = connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	if (con != -1) std::cout << "connection successful\n";

	// Set socket timeout time
	struct timeval  timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	int set = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));
	if (set != -1) std::cout << "set socket option successful\n";

	// Write Message into Buffer
	strcpy(buffer, "ping");

	for (int i = 0; i < 10; i++){
		//Get beginning time
		clock_t ping = clock();

		//Send packet to server
		int sen = sendto(sockfd, (const char *)buffer, strlen(buffer), 
			   MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

		if (sen != -1) std::cout << "sending packet successful\n";
		
		//Receive the packet from server
		int rec = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 
			MSG_WAITALL, ( struct sockaddr *) &servaddr, &len);
		
		if (rec < 0){
			//Print Error Message
			printf("receiving packet failed :,(");
			if (errno == EAGAIN) printf(" EAGAIN ");
			if (errno == EWOULDBLOCK) printf(" EWOULDBLOCK ");
			if (errno == EBADF) printf(" EBADF ");
			if (errno == ECONNREFUSED) printf(" ECONNREFUSED ");
			if (errno == EFAULT) printf(" EFAULT ");
			if (errno == EINTR) printf(" EINTR ");
			if (errno == EINVAL) printf(" EINVAL ");
			printf("\n");
		}
		else{
			//Get end time
			clock_t pong = clock();
			//Print RTT Time
			printf("receiving packet successful ");
			printf("RTT: %fs\n", (double)(pong - ping) / CLOCKS_PER_SEC);
		}
		printf("\n");
	}
	return 0; 
}
