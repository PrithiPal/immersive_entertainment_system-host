//g++ ScreenCapture.cpp -lX11 `pkg-config --cflags opencv` `pkg-config --libs opencv`

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>

#include "sendAverageScreenshot.cpp"
#include "sendDominantColorScreenshot.cpp"
#include "ScreenCapture.cpp"



int main(int argc, char** argv ){

	if ( argc != 2 ){
		printf("usage: ./moodSetter <mode>\n");
		printf("modes:\t0 - average color (return 4 numbers)\n");
		printf("modes:\t1 - dominant color (returns 1 number)\n");
		return -1;
	}

	int mode = atoi(argv[1]);

	while(1){
		screenCapture screen(0,0,1280,768);
		cv::Mat screenshotToSave;
		// output the current frame to a output file.
		// may slow down but for the testing purposes.
		screen(screenshotToSave);

		if (mode == 0){
			long numberOfElements = 4;
			unsigned long** avgMatrix = createAverageArray(screenshotToSave, numberOfElements);

			sendAverageColors(avgMatrix);
		}

		if (mode == 1){

			//hack done as the code for determining dominant color seems unable to work with the original image file
			cv::imshow("screenshotToSave", screenshotToSave);
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
		cv::waitKey(1);
	}

}
