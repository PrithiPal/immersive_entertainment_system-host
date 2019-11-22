
#include "mode1.h"

#include "packetSender.h"

void* mode1_main(void * args){
	model1_takePic();
	return NULL;
}

void model1_takePic(){
	while(1){
		screenCapture screen(0,0,1920,1080);
		cv::Mat screenshotToSave;
		screen(screenshotToSave);
		sendAverageColor(createAverageOfScreen(screenshotToSave));
	}
}
