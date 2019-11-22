
#include "multicast.h"
#include "packetSender.h"

#define BEAGLE_IP "192.168.7.2"

void sendAverageColor(unsigned long averageColor){
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

	sprintf(messageToSend, "a %ld",averageColor);
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

	sprintf(messageToSend, "d %ld",dominantColor);
	messageToSend[UDP_MESSAGE_SIZE-1]= 0;
	std::cout<<"sending a packet[" << messageToSend << "]" << std::endl;
	/*ssize_t res =*/ sendto(sd, messageToSend, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, addrlen);
}


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

	sprintf(messageToSend, "q %ld %ld %ld %ld", avgMatrix[0][0], avgMatrix[0][1], avgMatrix[1][0], avgMatrix[1][1]);
	messageToSend[UDP_MESSAGE_SIZE-1]= 0;
	std::cout<<"sending a packet[" << messageToSend << "]" << std::endl;
	/*ssize_t res =*/ sendto(sd, messageToSend, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, addrlen);
}


/*
rgb <AveragePixelOfWholeScreen> <DominantPixelOfWholeScreen> <AveragePixelOfQuad_Pixel_Left> <AveragePixelOfQuad_Pixel_Right> <AveragePixelOfQuad_Pixel_Top> <AveragePixelOfQuad_Pixel_Bottom>

*/
void sendCombinedColors(unsigned long dominantColor, unsigned long **avgMatrix, unsigned long AveragePixelOfWholeScreen){
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

	sprintf(messageToSend, "rgb %ld %ld %ld %ld %ld %ld", AveragePixelOfWholeScreen, dominantColor, avgMatrix[0][0], avgMatrix[0][1], avgMatrix[1][0], avgMatrix[1][1]);
	std::cout<<"sending a packet[" << messageToSend << "]" << std::endl;
	sendto(sd, messageToSend, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, addrlen);
}

char* UDP_receiveMessage(struct UDP_ConnectionInformation*info){
	unsigned int sin_len = sizeof(info->sinRemote);
	//char * messageRx;
	char * messageRx = (char*)malloc(UDP_MESSAGE_SIZE);
	//messageRx = malloc(sizeof(*messageRx) * UDP_MESSAGE_SIZE);
	int bytesRx = recvfrom(info->socketDescriptor,
		messageRx, UDP_MESSAGE_SIZE, 0,
		(struct sockaddr *) &info->sinRemote, &sin_len);

		// Make it null terminated (so string functions work):
		int terminateIdx = (bytesRx < UDP_MESSAGE_SIZE) ? bytesRx : UDP_MESSAGE_SIZE - 1;
		messageRx[terminateIdx-1] = 0;
		return messageRx;
}

struct UDP_ConnectionInformation * UDP_setupConnection(){

	struct UDP_ConnectionInformation * info = (struct UDP_ConnectionInformation *)malloc(1);

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(PORT);                 // Host to Network short

	// Create the socket for UDP
	info->socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port (PORT) that we specify
	bind (info->socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

	return info;
}
