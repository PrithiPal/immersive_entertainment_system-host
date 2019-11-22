#include <ctype.h>

#include "packetSender.h"

#include "mode1.h"
#include "mode2.h"
#include "mode3.h"
#include <signal.h>

#define AVERAGE_COMMAND "avg"
#define DOMINANT_COMMAND "dominant"
#define QUAD_COMMAND "quad"
#define STOP_COMMAND "stop"

pthread_t pidToKill;

void KillThread();

static void Menu_processCommand(struct UDP_ConnectionInformation*udpInfo, char * message);

int main(){

	struct UDP_ConnectionInformation *udpInfo = UDP_setupConnection();

	//used to make sure that the audioMixer does not start playing music until all modules are up and running
	pidToKill = -1;

	while(1){
		printf("waiting for a message\n");
		char * message = UDP_receiveMessage(udpInfo);
		if (strlen(message) != 0 && isalpha(message[0])){
			Menu_processCommand(udpInfo, message);
		}
		free(message);
		message = NULL;
	}
	return 0;
}


static void Menu_processCommand(struct UDP_ConnectionInformation*udpInfo,  char * message){
	if ( strcmp(message, AVERAGE_COMMAND) == 0){
		KillThread();
		pthread_t screenCapturePID;
		pthread_create(&screenCapturePID, NULL, &mode1_main, NULL);
		pidToKill = screenCapturePID;
	}else if ( strcmp(message, DOMINANT_COMMAND) == 0){
		KillThread();
		pthread_t screenCapturePID;
		pthread_create(&screenCapturePID, NULL, &mode2_main, NULL);
		pidToKill = screenCapturePID;

	}else if ( strcmp(message, QUAD_COMMAND) == 0){
		KillThread();
		pthread_t screenCapturePID;
		pthread_create(&screenCapturePID, NULL, &mode3_main, NULL);
		pidToKill = screenCapturePID;

	}
}

void KillThread(){
	long unsigned int val = -1;
	if (pidToKill != val){
		kill(pidToKill, SIGKILL);
	}
}
