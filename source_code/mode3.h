

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
#include "pipe.h"
#include "packetSender.h"

#ifndef _MODE_3_H_
#define _MODE_3_H_

void* mode3_main(void * args);

void mode3_takePic();
#endif
