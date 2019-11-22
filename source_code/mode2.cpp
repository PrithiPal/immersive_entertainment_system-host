

#include "mode2.h"
#include "packetSender.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>


void* mode2_main(void * args){


	mode2_takePic();
	return NULL;
}

void mode2_takePic(){
	while(1){
		screenCapture screen(0,0,1920,1080);
		cv::Mat screenshotToSave;
		screen(screenshotToSave);
		std::cout<<"screenshotToSave.rows : " << screenshotToSave.rows << std::endl;
		std::cout<<"screenshotToSave.cols : " << screenshotToSave.cols << std::endl;
		//cv::imshow("screenshotToSave", screenshotToSave);
		cv::imwrite("screen.jpg", screenshotToSave);

		cv::Mat screenshotToProcess;
		screenshotToProcess = cv::imread( "screen.jpg", 1 );
		/////////////////////////////////////////////////////////////////

		std::cout<<"screenshotToProcess.rows : " << screenshotToProcess.rows << std::endl;
		std::cout<<"screenshotToProcess.cols : " << screenshotToProcess.cols << std::endl;
		long numberOfValues = ((long)screenshotToProcess.rows) * screenshotToProcess.cols;
		std::cout<<"numberOfValues : " << numberOfValues << std::endl;



		cv::Mat reduced;

		reduceColor_kmeans(screenshotToProcess, reduced);

		std::map<cv::Vec3b, int, lessVec3b> palette = getPalette(reduced);
		int area = screenshotToProcess.rows * screenshotToProcess.cols;
		float largestArea = 0;
		cv::Vec<unsigned char, 3>  dominantColor;
		for (auto color : palette)
		{
			if (float(color.second) / float(area) > largestArea){
				largestArea = float(color.second) / float(area);
				dominantColor = color.first ;
			}
		}

		sendDominantColor(convertPixelRGBToSingleValue(dominantColor));
	}
}
