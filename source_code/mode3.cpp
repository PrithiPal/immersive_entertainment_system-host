

#include "mode3.h"
#include "sendAverageScreenshot.h"
#include "packetSender.h"

void* mode3_main(void * args){
	mode3_takePic();
	return NULL;
}

void mode3_takePic(){
	while(1){
		screenCapture screen(0,0,1920,1080);
		cv::Mat screenshotToSave;
		screen(screenshotToSave);
		long numberOfElements = 4;
		unsigned long** avgMatrix = createAverageArray(screenshotToSave, numberOfElements);

		sendAverageColors(avgMatrix);
	}
}
