#include "hlac_calculator.h"

using namespace std;
using namespace cv;



HLACCalculator::HLACCalculator(std::string input_path, std::shared_ptr<std::vector<cv::Rect>> calc_area)
{
	this->calc_area = calc_area;
	this->input_mat = get_mat_from_file(input_path);
}


std::vector<std::pair<cv::Rect,std::vector<int>>> HLACCalculator::get_result()
{
	vector<pair<Rect,std::vector<int>>> features(calc_area->size());
}


std::string HLACCalculator::get_result_string()
{

}



void HLACCalculator::get_result_file(std::string filepath)
{
	
}

Mat HLACCalculator::get_mat_from_file(string filename)
{
	Mat img_real = imread(filename, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	//-1はおまじない。
	// http://stackoverflow.com/questions/10969585/opencv-reading-a-16-bit-grayscale-image コチラいわく


	/*
	//表示
	namedWindow("Display window", WINDOW_AUTOSIZE);		// Create a window for display.
	imshow("Display window", new_image);                // Show our image inside it.

	waitKey(0);
	//*/

	return img_real;
}