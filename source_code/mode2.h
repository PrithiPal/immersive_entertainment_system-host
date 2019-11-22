


#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>

#include <stdio.h>
#include <string.h>

#include "sendAverageScreenshot.h"
#include "sendDominantColorScreenshot.h"
#include "ScreenCapture.h"
#include "packetSender.h"

#ifndef _MODE_2_H_
#define _MODE_2_H_

void* mode2_main(void * args);

void mode2_takePic();

#endif
