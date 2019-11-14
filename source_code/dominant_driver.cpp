
#include "sendDominantColorScreenshot.cpp"


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
