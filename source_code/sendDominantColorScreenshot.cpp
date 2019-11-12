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


int main(int argc, char** argv )
{


	if ( argc != 2 )
	{
		printf("usage: DisplayImage.out <Image_Path>\n");
		return -1;
	}

	cv::Mat src;
	src = cv::imread( argv[1], 1 );

	cv::Mat reduced;

	if ( !src.data )
	{
		printf("No src data \n");
		return -1;
	}
	std::cout<<"src.rows : " << src.rows << std::endl;
	std::cout<<"src.cols : " << src.cols << std::endl;
	long numberOfValues = ((long)src.rows) * src.cols;
	std::cout<<"numberOfValues : " << numberOfValues << std::endl;

	reduceColor_kmeans(src, reduced);

	std::map<cv::Vec3b, int, lessVec3b> palette = getPalette(reduced);
	int area = src.rows * src.cols;
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

	return 0;
}


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
