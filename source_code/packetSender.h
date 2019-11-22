
#ifndef _PACKET_SENDER_HEADER_
#define _PACKET_SENDER_HEADER_

// being defined because of the following issue
// https://stackoverflow.com/questions/5582211/what-does-define-gnu-source-imply
// https://stackoverflow.com/questions/16927613/error-when-compiling-a-multicast-listener/42236615#42236615
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <netdb.h>				// for creating sin
#include <iostream>

#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>				// for srand
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


#include <stdlib.h>
#include <string.h>

struct UDP_ConnectionInformation{
	struct sockaddr_in sinRemote;
	int socketDescriptor;
};

void sendDominantColor(unsigned long dominantColor);

void sendAverageColors(unsigned long **avgMatrix);

void sendAverageColor(unsigned long averageColor);

struct UDP_ConnectionInformation * UDP_setupConnection();

char* UDP_receiveMessage(struct UDP_ConnectionInformation*info);

void sendCombinedColors(unsigned long dominantColor, unsigned long **avgMatrix, unsigned long AveragePixelOfWholeScreen);
#endif
