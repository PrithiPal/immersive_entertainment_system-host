#include "sendAverageScreenshot.h"
#include "rgbCombiner.h"

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

unsigned long createAverageOfScreen(cv::Mat image){
	unsigned long sum = 0;
	int numOfEntries=0;
	for (long originalMatrixRow =  0; originalMatrixRow < image.rows; originalMatrixRow++){
		for (long originalMatrixCol = 0; originalMatrixCol < image.cols ; originalMatrixCol++){
			unsigned long product = convertPixelRGBToSingleValue(image.at<cv::Vec3b>(originalMatrixRow,originalMatrixCol));
			product = product * product;
			sum+=product;
			numOfEntries++;
		}
	}
	return sqrt(sum / numOfEntries);
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
