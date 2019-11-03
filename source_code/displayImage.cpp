#include <stdio.h>
#include <opencv2/opencv.hpp>

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
long getReductionFactor(long numberOfRows);

unsigned long** createAverageArray(cv::Mat image, long rowReductionFactor, long colReductionFactor);

unsigned long calculateAverageOfSubet(long newRowIndex, long newColIndex, cv::Mat image, long numberOfRowsPerAvgCells, long numberOfColumnsPerAvgCells);

unsigned long convertRGBToSingleValue(cv::Mat image, long row, long col);

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

	long rowReductionFactor = getReductionFactor(image.rows);
	long colReductionFactor = getReductionFactor(image.cols);
	unsigned long** avgMatrix = createAverageArray(image, rowReductionFactor, colReductionFactor);

	std::cout<<" avgMatrix : ";
	for (long row = 0; row < rowReductionFactor; row++ ){
		for (long col = 0; col < colReductionFactor; col++){
			std::cout<<avgMatrix[row][col]<<", ";
		}
		std::cout<<std::endl;
	}

	return 0;
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

long getReductionFactor(long numberOfEntries){
	for (float numberofNewEntries = 2; numberofNewEntries <= numberOfEntries - 1; numberofNewEntries++){
		float numberOfEntriesPerAverage = numberOfEntries / numberofNewEntries;
		double fraction = numberOfEntriesPerAverage - ((long)numberOfEntriesPerAverage);
		if (fraction == 0){
			return numberOfEntriesPerAverage;
		}
	}
}

unsigned long** createAverageArray(cv::Mat image, long rowReductionFactor, long colReductionFactor){
	long numberOfRowsPerAvgCells = image.rows / rowReductionFactor;
	long numberOfColumnsPerAvgCells = image.cols / colReductionFactor;

	unsigned long** a = new unsigned long*[rowReductionFactor];
	for(long row = 0; row < rowReductionFactor; row++){
		a[row] = new unsigned long[colReductionFactor];
		for (long col = 0; col < colReductionFactor; col++){
			a[row][col] = calculateAverageOfSubet(row, col, image, numberOfRowsPerAvgCells, numberOfColumnsPerAvgCells);
		}
	}
	return a;
}

unsigned long calculateAverageOfSubet(long newRowIndex, long newColIndex, cv::Mat image, long numberOfRowsPerAvgCells, long numberOfColumnsPerAvgCells){

	unsigned long sum = 0;
	int numOfEntries=0;
	for (long originalMatrixRow = ( ( newRowIndex * numberOfRowsPerAvgCells) + 1); originalMatrixRow <= ( ( newRowIndex * numberOfRowsPerAvgCells) + numberOfRowsPerAvgCells); originalMatrixRow++){
		for (long originalMatrixCol = ( ( newColIndex * numberOfColumnsPerAvgCells ) + 1); originalMatrixCol <= ( ( newColIndex * numberOfColumnsPerAvgCells ) + numberOfColumnsPerAvgCells ) ; originalMatrixCol++){
			sum+=convertRGBToSingleValue(image, originalMatrixRow, originalMatrixCol);
			numOfEntries++;
		}
	}
	return (sum / numOfEntries);

}
