#ifndef _SEND_AVERAGE_SCREENSHOT_H_
#define _SEND_AVERAGE_SCREENSHOT_H_

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <stdbool.h>

#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include "multicast.h"
#include "packetSender.h"
#include "rgbCombiner.h"

unsigned long** createAverageArray(cv::Mat image, long numberOfElements);

unsigned long calculateAverageOfSubet(long newRowIndex, long newColIndex, cv::Mat image, long numberOfRowsPerAvgCells, long numberOfColumnsPerAvgCells);

unsigned long** createAverageArray(cv::Mat image, long numberOfElements);

unsigned long createAverageOfScreen(cv::Mat image);

unsigned long calculateAverageOfSubet(long newRowIndex, long newColIndex, cv::Mat image, long numberOfRowsPerAvgCells, long numberOfColumnsPerAvgCells);

#endif
