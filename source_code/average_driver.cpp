#include "sendAverageScreenshot.cpp"


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
