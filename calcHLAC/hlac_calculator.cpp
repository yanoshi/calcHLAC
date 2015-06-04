#include "hlac_calculator.h"

using namespace std;
using namespace cv;



HLACCalculator::HLACCalculator(std::string input_path, std::shared_ptr<std::vector<cv::Rect>> calc_area)
{
	this->calc_area = calc_area;
	this->input_path = input_path;
}


std::vector<int> HLACCalculator::get_result()
{


}


std::string HLACCalculator::get_result_string()
{

}



void HLACCalculator::get_result_file(std::string filepath)
{
	
}