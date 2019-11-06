#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <stdbool.h>

#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include "multicast.h"

unsigned long** createAverageArray(cv::Mat image, long numberOfElements, long & sizeOfString);

unsigned long calculateAverageOfSubet(long newRowIndex, long newColIndex, cv::Mat image, long numberOfRowsPerAvgCells, long numberOfColumnsPerAvgCells);

void sendPackets(unsigned long **avgMatrix, long numberOfEntries, long sizeOfString);

unsigned long convertRGBToSingleValue(cv::Mat image, long row, long col);

static int DisplayImage_numberOfDigits(long number);

int main(int argc, char** argv )
{


	if ( argc != 2 )
	{
		printf("usage: DisplayImage.out <Image_Path>\n");
		return -1;
	}

	cv::Mat image;
	image = cv::imread( argv[1], 1 );

	if ( !image.data )
	{
		printf("No image data \n");
		return -1;
	}
	std::cout<<"image.rows : " << image.rows << std::endl;
	std::cout<<"image.cols : " << image.cols << std::endl;
	long numberOfValues = ((long)image.rows) * image.cols;
	std::cout<<"numberOfValues : " << numberOfValues << std::endl;

	long numberOfElements = 4;
	long sizeOfString = 0;
	unsigned long** avgMatrix = createAverageArray(image, numberOfElements, sizeOfString);

	sendPackets(avgMatrix, numberOfElements, sizeOfString);

	return 0;
}

unsigned long** createAverageArray(cv::Mat image, long numberOfElements, long & sizeOfString){
	long numberOfRowsPerAvgCells = image.rows / 2;
	long numberOfColumnsPerAvgCells = image.cols / 2;
	unsigned long**  avgArray = new unsigned long *[numberOfElements/2];
	for(long rowIndex = 0; rowIndex < 2; rowIndex++){
		sizeOfString+=1;
		avgArray[rowIndex] = new unsigned long[numberOfElements/2];
		for (long colIndex = 0; colIndex < 2; colIndex++){
			avgArray[rowIndex][colIndex] =  calculateAverageOfSubet(rowIndex, colIndex, image, numberOfRowsPerAvgCells, numberOfColumnsPerAvgCells);
			sizeOfString+=DisplayImage_numberOfDigits(avgArray[rowIndex][colIndex])+2;
		}
		sizeOfString+=1;

	}
	sizeOfString+=1;

	return avgArray;
}

unsigned long calculateAverageOfSubet(long newRowIndex, long newColIndex, cv::Mat image, long numberOfRowsPerAvgCells, long numberOfColumnsPerAvgCells){

	unsigned long sum = 0;
	int numOfEntries=0;
	for (long originalMatrixRow = ( ( newRowIndex * numberOfRowsPerAvgCells) + 1); originalMatrixRow <= ( ( newRowIndex * numberOfRowsPerAvgCells) + numberOfRowsPerAvgCells); originalMatrixRow++){
		for (long originalMatrixCol = ( ( newColIndex * numberOfColumnsPerAvgCells ) + 1); originalMatrixCol <= ( ( newColIndex * numberOfColumnsPerAvgCells ) + numberOfColumnsPerAvgCells ) ; originalMatrixCol++){
			unsigned long product = convertRGBToSingleValue(image, originalMatrixRow, originalMatrixCol);
			product = product * product;
			sum+=product;
			numOfEntries++;
		}
	}
	return sqrt(sum / numOfEntries);

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

static int DisplayImage_numberOfDigits(long number){
	int numberOfDigits=0;
	while (number / 10 != 0){
		numberOfDigits++;
		number /= 10;
	}
	return ++numberOfDigits;
}
