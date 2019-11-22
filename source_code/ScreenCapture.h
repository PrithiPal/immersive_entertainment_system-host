#ifndef _SCREEN_CAPTURE_H_
#define _SCREEN_CAPTURE_H_

struct screenCapture{
	Display* display;
	Window root;
	int startingPosition_X,startingPosition_Y,windowWidth,windowHeight;
	XImage* image;
	bool created;
	//constructor
	screenCapture(int startingPosition_X, int startingPosition_Y, int windowWidth, int windowHeight):
	startingPosition_X(startingPosition_X),
	startingPosition_Y(startingPosition_Y),
	windowWidth(windowWidth),
	windowHeight(windowHeight)
	{
		//connect to X server
		display = XOpenDisplay(NULL);
		// returns the current window to display
		root = DefaultRootWindow(display);
		//window is created successfully
		created = true;
	}

	void operator() (cv::Mat& openCVImage){
		if(created){
			created = false;
		}
		else{
			XDestroyImage(image);
		}
		//XImage structure with specified window width, height, starting X & Y position
		image = XGetImage(display, root, startingPosition_X, startingPosition_Y, windowWidth, windowHeight, AllPlanes, ZPixmap);
		//creates a Mat object
		openCVImage = cv::Mat(windowHeight, windowWidth, CV_8UC4, image->data);
	}

	//deconstructor
	~screenCapture(){
		if(!created){
			//deallocates the memory
			XDestroyImage(image);
		}
		//closes the connection to the x server
		XCloseDisplay(display);
	}


};

#endif
