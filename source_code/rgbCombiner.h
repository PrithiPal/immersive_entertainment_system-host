
#ifndef _RGB_COMBINER_
#define _RGB_COMBINER_

unsigned long convertPixelRGBToSingleValue(cv::Vec<unsigned char, 3>  dominantColor){

	long r = dominantColor[2];
	long g = dominantColor[1];
	long b = dominantColor[0];
	//printf("[rgbCombiner.h convertPixelRGBToSingleValue()] (r,g,b) => (%ld,%ld,%ld)\n", r, g, b);

	std::stringstream r_ss;
	r_ss<< std::hex << r; // int decimal_value
	std::string r_hex ( r_ss.str() );
	//printf("[rgbCombiner.h convertPixelRGBToSingleValue()] r_hex => %s\n", r_hex.c_str());

	std::stringstream g_ss;
	g_ss<< std::hex << g; // int decimal_value
	std::string g_hex ( g_ss.str() );
	//printf("[rgbCombiner.h convertPixelRGBToSingleValue()] g_hex => %s\n", g_hex.c_str());

	std::stringstream b_ss;
	b_ss<< std::hex << b; // int decimal_value
	std::string b_hex ( b_ss.str() );
	//printf("[rgbCombiner.h convertPixelRGBToSingleValue()] b_hex => %s\n", b_hex.c_str());

	std::string concatenatedHex = r_hex + g_hex + b_hex;
	//printf("[rgbCombiner.h convertPixelRGBToSingleValue()] concatenatedHex => %s\n", concatenatedHex.c_str());

	unsigned long decimalvalueOfColor;
	std::stringstream hex_stream;
	hex_stream << std::hex << concatenatedHex;
	hex_stream >> decimalvalueOfColor;
	//printf("[rgbCombiner.h convertPixelRGBToSingleValue()] decimalvalueOfColor => %ld\n", decimalvalueOfColor);
	return decimalvalueOfColor;

	//std::cout<<"("<<r<<", "<<r_hex<<"), ("<<g<<", "<<g_hex<<"), ("<<b<<", "<<b_hex<<") = ("<<concatenatedHex<<", "<<decimalvalueOfColor<<")"<<std::endl;
}

#endif
