
#ifndef _RGB_COMBINER_
#define _RGB_COMBINER_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>

unsigned long convertPixelRGBToSingleValue(cv::Vec<unsigned char, 3>  dominantColor);

#endif
