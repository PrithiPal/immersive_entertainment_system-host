#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <stdbool.h>

#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include "multicast.h"
/*
Useful Links:
https://www.tutorialspoint.com/stringstream-in-cplusplus-for-decimal-to-hexadecimal-and-back
https://stackoverflow.com/questions/20034297/decimal-to-hex-conversion-c-built-in-function
https://www.rapidtables.com/convert/number/hex-to-decimal.html
https://www.w3schools.com/colors/colors_rgb.asp
https://www.checkyourmath.com/convert/color/decimal_rgb.php
https://medium.com/@kevinsimper/how-to-average-rgb-colors-together-6cd3ef1ff1e5
https://stackoverflow.com/questions/8932893/accessing-certain-pixel-rgb-value-in-opencv
https://docs.opencv.org/2.4/doc/tutorials/core/mat_the_basic_image_container/mat_the_basic_image_container.html
*/

void calculateIndexToWriteAt(long packetPage, long timeStamp, char *& messageToSend, int & indexToWriteAt, long sizeOfString, long stringSizeSoFar);

long getReductionFactor(long numberOfRows);

unsigned long** createAverageArray(cv::Mat image, long avgMatrixRowSize, long avgMatrixColSize, long & numberOfElements, long & sizeOfString);

unsigned long calculateAverageOfSubet(long newRowIndex, long newColIndex, cv::Mat image, long numberOfRowsPerAvgCells, long numberOfColumnsPerAvgCells);

unsigned long convertRGBToSingleValue(cv::Mat image, long row, long col);

void sendOffMultiplePacket(unsigned long** avgMatrix, long row_size, long col_size, long numberOfElements, long sizeOfString);

static int numberOfDigits(long number);

static _Bool displayImage_makeNewString(long indexToWriteAt, long nextval);

static int DisplayImage_numberOfDigits(long number);

void addHeader(long packetPage, long timeStamp, char *& messageToSend, int & indexToWriteAt, long sizeOfString, long stringSizeSoFar);

char * printSubString(char *& message, long firstIndex, long endIndex);

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

	long avgMatrixRowSize = getReductionFactor(image.rows);
	long avgMatrixColSize = getReductionFactor(image.cols);
	long numberOfElements = 0;
	long sizeOfString = 0;
	unsigned long** avgMatrix = createAverageArray(image, avgMatrixRowSize, avgMatrixColSize, numberOfElements, sizeOfString);

	sendOffMultiplePacket(avgMatrix,avgMatrixRowSize, avgMatrixColSize, numberOfElements, sizeOfString);

	return 0;
}

void sendOffMultiplePacket(unsigned long** avgMatrix, long row_size, long col_size, long numberOfElements, long sizeOfString){
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

	long packetPage = 1;
	long timeStamp = (unsigned long)time(NULL);
	long stringSizeSoFar = 0;

	for (int row = 0; row < row_size; row++){
		for (int col = 0; col < col_size; col++){

			//the message that the host has been constructing will be too big
			//if the next value gets add onto it so it sends off the currently constructed packet
			//to the multicast then clear the message in preparation for
			//the next array values
			if (displayImage_makeNewString(indexToWriteAt, avgMatrix[row][col])){
				messageToSend[UDP_MESSAGE_SIZE-1]= 0;
				std::cout<<"sending a packet" << std::endl;
				addHeader(packetPage, timeStamp, messageToSend, indexToWriteAt, sizeOfString, stringSizeSoFar);
				ssize_t res = sendto(sd, messageToSend, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, addrlen);
				packetPage++;
				delete []messageToSend;
				messageToSend = new char[UDP_MESSAGE_SIZE];
				indexToWriteAt = 0;
			}

			if (indexToWriteAt == 0){
				//addHeader(packetPage, timeStamp, messageToSend, indexToWriteAt, sizeOfString, stringSizeSoFar);
				calculateIndexToWriteAt(packetPage, timeStamp, messageToSend, indexToWriteAt, sizeOfString, stringSizeSoFar);
				//used to clear the array just in case there is any junk value in there
				for (int k = 0; k < UDP_MESSAGE_SIZE; k++){
					messageToSend[k] = ' ';
				}
			}

			sprintf(messageToSend+indexToWriteAt, "%ld, ", avgMatrix[row][col]);
			indexToWriteAt+=DisplayImage_numberOfDigits(avgMatrix[row][col]);
			indexToWriteAt+=2;
			stringSizeSoFar+=DisplayImage_numberOfDigits(avgMatrix[row][col])+2;
			messageToSend[indexToWriteAt+1] = 0;
		}
	}

	messageToSend[UDP_MESSAGE_SIZE-1]= 0;
	std::cout<<"sending a packet" << std::endl;
	addHeader(packetPage, timeStamp, messageToSend, indexToWriteAt, sizeOfString, stringSizeSoFar);
	ssize_t res = sendto(sd, messageToSend, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, addrlen);
}

void printDem(char *& messageToSend){
	printf("{%d}-{%d}-{%d}-{%d}-{%d}-",messageToSend[0], messageToSend[1], messageToSend[2], messageToSend[3], messageToSend[4]);
	printf("{%d}-{%d}-{%d}-{%d}-{%d}-",messageToSend[5], messageToSend[6], messageToSend[7], messageToSend[8], messageToSend[9]);
	printf("{%d}-{%d}-{%d}-{%d}-{%d}-",messageToSend[10], messageToSend[11], messageToSend[12], messageToSend[13], messageToSend[14]);
	printf("{%d}-{%d}-{%d}-{%d}-{%d}-",messageToSend[15], messageToSend[16], messageToSend[17], messageToSend[18], messageToSend[19]);
	printf("{%d}-{%d}-{%d}-{%d}-{%d}-",messageToSend[20], messageToSend[21], messageToSend[22], messageToSend[23], messageToSend[24]);
	printf("{%d}-{%d}-{%d}-{%d}-{%d}-",messageToSend[25], messageToSend[26], messageToSend[27], messageToSend[28], messageToSend[29]);
	printf("{%d}-{%d}-{%d}-{%d}-{%d}-\n",messageToSend[30], messageToSend[31], messageToSend[32], messageToSend[33], messageToSend[34]);
}
void calculateIndexToWriteAt(long packetPage, long timeStamp, char *& messageToSend, int & indexToWriteAt, long sizeOfString, long stringSizeSoFar){
	indexToWriteAt+=DisplayImage_numberOfDigits(timeStamp) + DisplayImage_numberOfDigits(sizeOfString) +DisplayImage_numberOfDigits(sizeOfString) + 4;
}
void addHeader(long packetPage, long timeStamp, char *& messageToSend, int & indexToWriteAt, long sizeOfString, long stringSizeSoFar){

	int additionalStrings = DisplayImage_numberOfDigits(sizeOfString);
	sprintf(messageToSend, "[%ld-%0*ld/%ld]", timeStamp, additionalStrings, stringSizeSoFar, sizeOfString);
	printDem(messageToSend);
	messageToSend[DisplayImage_numberOfDigits(timeStamp) + DisplayImage_numberOfDigits(sizeOfString) +DisplayImage_numberOfDigits(sizeOfString) + 4]=' ';
	printDem(messageToSend);
	std::cout<<" creating header {" << printSubString(messageToSend, 0, DisplayImage_numberOfDigits(timeStamp) + DisplayImage_numberOfDigits(sizeOfString) +DisplayImage_numberOfDigits(sizeOfString) + 9) <<"}"<< std::endl;
	printDem(messageToSend);
	//std::cout<<" creating header [" << timeStamp << "-" << stringSizeSoFar<<"/"<<sizeOfString <<"]" << std::endl;
	//indexToWriteAt+=DisplayImage_numberOfDigits(timeStamp) + DisplayImage_numberOfDigits(stringSizeSoFar) +DisplayImage_numberOfDigits(sizeOfString) + 4;

}

char * printSubString(char *& message, long firstIndex, long endIndex){
	char * substr = new char[endIndex - firstIndex+1];

	//std::cout<<"endIndex - firstIndex : " << endIndex - firstIndex << std::endl;
	int index = 0;
	for (long i = firstIndex; index < endIndex - firstIndex; i++){
		//std::cout<<" index : " << index << std::endl;
		substr[index++] = message[i];
	}
	substr[endIndex - firstIndex]=0;
	return substr;

}
static _Bool displayImage_makeNewString(long indexToWriteAt, long nextval){
	return ( UDP_MESSAGE_SIZE <= ( (indexToWriteAt + DisplayImage_numberOfDigits(nextval) ) + 1) );
}

long getReductionFactor(long numberOfEntries){
	for (float numberofNewEntries = 2; numberofNewEntries <= numberOfEntries - 1; numberofNewEntries++){
		float numberOfEntriesPerAverage = numberOfEntries / numberofNewEntries;
		double fraction = numberOfEntriesPerAverage - ((long)numberOfEntriesPerAverage);
		if (fraction == 0){
			return numberOfEntriesPerAverage;
		}
	}
	return -1;
}

unsigned long** createAverageArray(cv::Mat image, long avgMatrixRowSize, long avgMatrixColSize, long & numberOfElements, long & sizeOfString){
	long numberOfRowsPerAvgCells = image.rows / avgMatrixRowSize;
	long numberOfColumnsPerAvgCells = image.cols / avgMatrixColSize;
	unsigned long** a = new unsigned long*[avgMatrixRowSize];
	for(long row = 0; row < avgMatrixRowSize; row++){
		a[row] = new unsigned long[avgMatrixColSize];
		for (long col = 0; col < avgMatrixColSize; col++){
			a[row][col] = calculateAverageOfSubet(row, col, image, numberOfRowsPerAvgCells, numberOfColumnsPerAvgCells);
			sizeOfString+=DisplayImage_numberOfDigits(a[row][col])+2;
			numberOfElements++;
		}
	}

	return a;
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
