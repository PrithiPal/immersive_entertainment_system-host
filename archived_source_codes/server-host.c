#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include "multicast.h"

static int numberOfDigits(long number){
	int numberOfDigits=0;
	while (number / 10 != 0){
		numberOfDigits++;
		number /= 10;
	}
	return ++numberOfDigits;
}

int main(){
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	printf("[server-host] L20 sd: %d\n", sd);
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(MULTICAST_PORT);
	addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
	unsigned int addrlen = sizeof(addr);

	char * message;
	int messageIndex = 0;
	message = malloc(sizeof(*message) * PACKET_SIZE);
	for (int i = 0; i < PACKET_SIZE; i++){
		message[messageIndex] = i;

		//adds the next array element to the mesage as well as adding a newline
		//if it is the 10th element added to the current line
		if ( ( i + 1 ) % NUMBER_OF_ARRAY_ELEMENTS_PER_LINE == 0 && i != 0){
			sprintf(message+messageIndex, "%d", i);
			messageIndex+=numberOfDigits(i);
			message[messageIndex] = '\n';
			message[messageIndex+1] = 0;
		}else if (messageIndex + numberOfDigits(i)+2 < PACKET_SIZE){
			//adding next value to the same line of the message
			sprintf(message+messageIndex, "%d", i);
			message[messageIndex+numberOfDigits(i)]=',';
			message[messageIndex+numberOfDigits(i)+1]=' ';
			message[messageIndex+numberOfDigits(i)+2] = 0;
			messageIndex+=numberOfDigits(i)+2;

		}

		//if the current value is the last elementin the array, adds the newline
		//and the NULL terminating character in preparation for when it gets sent
		//down the pipe
		if ( ( i + 1 ) % NUMBER_OF_ARRAY_ELEMENTS_PER_LINE == 0 && i != 0){
			message[messageIndex] = '\n';
			messageIndex++;
			message[messageIndex] = 0;
		}
	}

	message[PACKET_SIZE-1] = 0;


	ssize_t res = sendto(sd, message, PACKET_SIZE, 0, (struct sockaddr *) &addr, addrlen);

	printf("[server-host] L71 res: %ld\n", res);
	printf("[server-host] message sent : \n{%s}\n", message);
	return 0;
}
