#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

#include "multicast.h"
#include "packetSender.h"
#include "rgbCombiner.h"
#include "sendDominantColorScreenshot.h"

// https://code.i-harness.com/en/q/21d5f30
// https://stackoverflow.com/a/34734939/5008845
void reduceColor_kmeans(const cv::Mat& src, cv::Mat& dst);

// https://code.i-harness.com/en/q/21d5f30
std::map<cv::Vec3b, int, lessVec3b> getPalette(const cv::Mat& src);

void reduceColor_kmeans(const cv::Mat& src, cv::Mat& dst)
{
    int K = 8;
    int n = src.rows * src.cols;
    cv::Mat data = src.reshape(1, n);
    data.convertTo(data, CV_32F);

    std::vector<int> labels;
    cv::Mat1f colors;
    kmeans(data, K, labels, cv::TermCriteria(), 1, cv::KMEANS_PP_CENTERS, colors);

    for (int i = 0; i < n; ++i)
    {
        data.at<float>(i, 0) = colors(labels[i], 0);
        data.at<float>(i, 1) = colors(labels[i], 1);
        data.at<float>(i, 2) = colors(labels[i], 2);
    }

    cv::Mat reduced = data.reshape(3, src.rows);
    reduced.convertTo(dst, CV_8U);
}

std::map<cv::Vec3b, int, lessVec3b> getPalette(const cv::Mat& src)
{
    std::map<cv::Vec3b, int, lessVec3b> palette;
    for (int r = 0; r < src.rows; ++r)
    {
        for (int c = 0; c < src.cols; ++c)
        {

            cv::Vec3b color = src.at<cv::Vec3b>(r,c);
            if (palette.count(color) == 0)
            {
                palette[color] = 1;
            }
            else
            {
                palette[color] = palette[color] + 1;
            }
        }
    }
    return palette;
}
