#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

#include "multicast.h"

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

static int DisplayImage_numberOfDigits(long number);

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

    for (auto color : palette)
    {
        std::cout << "Color: " << color.first << " \t - Area: " << 100.f * float(color.second) / float(area) << "%" << std::endl;
    }

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

unsigned long convertRGBToSingleValue(cv::Mat image, long row, long col){
	long r = image.at<cv::Vec3b>(row,col)[2];
	long g = image.at<cv::Vec3b>(row,col)[1];
	long b = image.at<cv::Vec3b>(row,col)[0];

	std::stringstream r_ss;
	r_ss<< std::hex << r; // int decimal_value
	std::string r_hex ( r_ss.str() );

	std::stringstream g_ss;
	g_ss<< std::hex << g; // int decimal_value
	std::string g_hex ( g_ss.str() );

	std::stringstream b_ss;
	b_ss<< std::hex << b; // int decimal_value
	std::string b_hex ( b_ss.str() );

	std::string concatenatedHex = r_hex + g_hex + b_hex;

	unsigned long decimalvalueOfColor;
	std::stringstream hex_stream;
	hex_stream << std::hex << concatenatedHex;
	hex_stream >> decimalvalueOfColor;
	return decimalvalueOfColor;

	//std::cout<<"("<<r<<", "<<r_hex<<"), ("<<g<<", "<<g_hex<<"), ("<<b<<", "<<b_hex<<") = ("<<concatenatedHex<<", "<<decimalvalueOfColor<<")"<<std::endl;
}

void sendPackets(unsigned long **avgMatrix, long numberOfEntries, long sizeOfString){
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	printf("[server-host] L20 sd: %d\n", sd);
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(MULTICAST_PORT);
	addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
	unsigned int addrlen = sizeof(addr);

	char * messageToSend = new char[UDP_MESSAGE_SIZE];
	int indexToWriteAt = 0;

	for (int row = 0; row < 2; row++){
		for (int col = 0; col < 2; col++){
			sprintf(messageToSend+indexToWriteAt, "%ld, ", avgMatrix[row][col]);
			indexToWriteAt+=DisplayImage_numberOfDigits(avgMatrix[row][col]);
			indexToWriteAt+=2;
		}
	}
	messageToSend[UDP_MESSAGE_SIZE-1]= 0;
	std::cout<<"sending a packet[" << messageToSend << "]" << std::endl;
	/*ssize_t res =*/ sendto(sd, messageToSend, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, addrlen);
}


static int DisplayImage_numberOfDigits(long number){
	int numberOfDigits=0;
	while (number / 10 != 0){
		numberOfDigits++;
		number /= 10;
	}
	return ++numberOfDigits;
}
