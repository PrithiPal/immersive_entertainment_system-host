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
	unsigned long** avgMatrix = createAverageArray(image, numberOfElements);

	sendAverageColors(avgMatrix);

	return 0;
}


unsigned long** createAverageArray(cv::Mat image, long numberOfElements){
	long numberOfRowsPerAvgCells = image.rows / 2;
	long numberOfColumnsPerAvgCells = image.cols / 2;
	unsigned long**  avgArray = new unsigned long *[numberOfElements/2];
	for(long rowIndex = 0; rowIndex < 2; rowIndex++){
		avgArray[rowIndex] = new unsigned long[numberOfElements/2];
		for (long colIndex = 0; colIndex < 2; colIndex++){
			avgArray[rowIndex][colIndex] =  calculateAverageOfSubet(rowIndex, colIndex, image, numberOfRowsPerAvgCells, numberOfColumnsPerAvgCells);
		}

	}

	return avgArray;
}

unsigned long calculateAverageOfSubet(long newRowIndex, long newColIndex, cv::Mat image, long numberOfRowsPerAvgCells, long numberOfColumnsPerAvgCells){

	unsigned long sum = 0;
	int numOfEntries=0;
	for (long originalMatrixRow = ( ( newRowIndex * numberOfRowsPerAvgCells) + 1); originalMatrixRow < ( ( newRowIndex * numberOfRowsPerAvgCells) + numberOfRowsPerAvgCells); originalMatrixRow++){
		for (long originalMatrixCol = ( ( newColIndex * numberOfColumnsPerAvgCells ) + 1); originalMatrixCol < ( ( newColIndex * numberOfColumnsPerAvgCells ) + numberOfColumnsPerAvgCells ) ; originalMatrixCol++){
			unsigned long product = convertPixelRGBToSingleValue(image.at<cv::Vec3b>(originalMatrixRow,originalMatrixCol));
			product = product * product;
			sum+=product;
			numOfEntries++;
		}
	}
	return sqrt(sum / numOfEntries);

}
