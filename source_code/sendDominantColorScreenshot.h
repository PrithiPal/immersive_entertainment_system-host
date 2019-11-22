#ifndef _SEND_DOMINANT_COLOR_SCEENSHOT_
#define _SEND_DOMINANT_COLOR_SCEENSHOT_

#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

#include "multicast.h"
#include "packetSender.h"
#include "rgbCombiner.h"

// https://code.i-harness.com/en/q/21d5f30
struct lessVec3b
{
    bool operator()(const cv::Vec3b& lhs, const cv::Vec3b& rhs) const{
        return (lhs[0] != rhs[0]) ? (lhs[0] < rhs[0]) : ((lhs[1] != rhs[1]) ? (lhs[1] < rhs[1]) : (lhs[2] < rhs[2]));
    }
};



// https://code.i-harness.com/en/q/21d5f30
// https://stackoverflow.com/a/34734939/5008845
void reduceColor_kmeans(const cv::Mat& src, cv::Mat& dst);

// https://code.i-harness.com/en/q/21d5f30
std::map<cv::Vec3b, int, lessVec3b> getPalette(const cv::Mat& src);

void reduceColor_kmeans(const cv::Mat& src, cv::Mat& dst);

std::map<cv::Vec3b, int, lessVec3b> getPalette(const cv::Mat& src);

#endif
