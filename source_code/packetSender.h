
#ifndef _PACKET_SENDER_HEADER_
#define _PACKET_SENDER_HEADER_

static int DisplayImage_numberOfDigits(long number);

#define BEAGLE_IP "192.168.7.2"
void sendAverageColors(unsigned long **avgMatrix){
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	printf("[server-host] L20 sd: %d\n", sd);
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(MULTICAST_PORT);
	//addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
	addr.sin_addr.s_addr = inet_addr(BEAGLE_IP);

	unsigned int addrlen = sizeof(addr);

	char * messageToSend = new char[UDP_MESSAGE_SIZE];
	int indexToWriteAt = 0;

	for (int row = 0; row < 2; row++){
		for (int col = 0; col < 2; col++){
			sprintf(messageToSend+indexToWriteAt, "%ld, ", avgMatrix[row][col]);
			indexToWriteAt+=DisplayImage_numberOfDigits(avgMatrix[row][col]);
			indexToWriteAt+=2;
		}
	}
	messageToSend[UDP_MESSAGE_SIZE-1]= 0;
	std::cout<<"sending a packet[" << messageToSend << "]" << std::endl;
	/*ssize_t res =*/ sendto(sd, messageToSend, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, addrlen);
}


void sendDominantColor(unsigned long dominantColor){
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	printf("[server-host] L20 sd: %d\n", sd);
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(MULTICAST_PORT);
	addr.sin_addr.s_addr = inet_addr(BEAGLE_IP);
	//addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
	unsigned int addrlen = sizeof(addr);

	char * messageToSend = new char[UDP_MESSAGE_SIZE];
	for (int i = 0; i < UDP_MESSAGE_SIZE; i++){
		messageToSend[i]= ' ';
	}

	sprintf(messageToSend, "%ld",dominantColor);
	messageToSend[UDP_MESSAGE_SIZE-1]= 0;
	std::cout<<"sending a packet[" << messageToSend << "]" << std::endl;
	/*ssize_t res =*/ sendto(sd, messageToSend, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, addrlen);
}



static int DisplayImage_numberOfDigits(long number){
	int numberOfDigits=0;
	while (number / 10 != 0){
		numberOfDigits++;
		number /= 10;
	}
	return ++numberOfDigits;
}

#endif
