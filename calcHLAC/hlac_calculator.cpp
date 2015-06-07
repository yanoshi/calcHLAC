#include "hlac_calculator.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <string>

using namespace std;
using namespace cv;



HLACCalculator::HLACCalculator(std::string input_path, std::shared_ptr<std::vector<cv::Rect>> calc_area,int step_size)
{
	Mat origin_img = get_mat_from_file(input_path);
	origin_img.convertTo(origin_img, CV_8UC1);

	this->calc_area = calc_area;
	this->input_mat = Mat(origin_img.size().width, origin_img.size().height, CV_8UC1);
	cv::threshold(origin_img, this->input_mat, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	this->step_size = step_size;
}

uchar HLACCalculator::get_pixel(int x, int y)
{
	return input_mat.at<unsigned char>(y, x);
}

std::vector<int> HLACCalculator::calc_features(int start_x, int start_y, int end_x, int end_y, int step)
{
	vector<int> hlac_values(25);

	for (int i = 0; i < 25; i++)
		hlac_values[i] = 0;

#pragma omp parallel for
	for (int iy = start_y; iy <= end_y; iy+=step)
	{
		for (int ix = start_x; ix <= end_x; ix+=step)
		{
			uchar p5 = get_pixel(ix, iy);

			if (p5 != 0)
			{
				uchar p1 = get_pixel(ix - 1, iy - 1);
				uchar p2 = get_pixel(ix, iy - 1);
				uchar p3 = get_pixel(ix + 1, iy - 1);

				uchar p4 = get_pixel(ix - 1, iy);
				uchar p6 = get_pixel(ix + 1, iy);

				uchar p7 = get_pixel(ix - 1, iy + 1);
				uchar p8 = get_pixel(ix, iy + 1);
				uchar p9 = get_pixel(ix + 1, iy + 1);

				hlac_values[0]++;


				if (p1 != 0)
				{
					hlac_values[1]++;
					if (p8 != 0) hlac_values[13]++;
					if (p3 != 0) hlac_values[21]++;
					if (p7 != 0) hlac_values[22]++;
				}
				if (p2 != 0)
				{
					hlac_values[2]++;
					if (p8 != 0) hlac_values[7]++;
					if (p7 != 0) hlac_values[11]++;
					if (p9 != 0) hlac_values[12]++;
					if (p6 != 0) hlac_values[17]++;
				}
				if (p3 != 0)
				{
					hlac_values[3]++;
					if (p7 != 0) hlac_values[6]++;
					if (p4 != 0) hlac_values[9]++;
					if (p8 != 0) hlac_values[14]++;
				}
				if (p4 != 0)
				{
					hlac_values[4]++;
					if (p6 != 0) hlac_values[5]++;
					if (p9 != 0) hlac_values[10]++;
					if (p2 != 0) hlac_values[18]++;
					if (p8 != 0) hlac_values[19]++;
				}
				if (p6 != 0)
				{
					if (p7 != 0) hlac_values[15]++;
					if (p1 != 0) hlac_values[16]++;
					if (p8 != 0) hlac_values[20]++;
				}
				if (p9 != 0)
				{
					if (p1 != 0) hlac_values[8]++;
					if (p7 != 0) hlac_values[23]++;
					if (p3 != 0) hlac_values[24]++;
				}
			}

		}
	}

	return hlac_values;
}


std::shared_ptr<std::vector<std::pair<cv::Rect,std::vector<int>>>> HLACCalculator::get_result()
{
	std::shared_ptr<vector<pair<Rect, std::vector<int>>>> features(new vector<pair<Rect, std::vector<int>>>());

	for (int i = 0; i < calc_area->size(); i++)
	{
		//探索開始点
		int start_x = this->calc_area->at(i).x + 1;
		int start_y = this->calc_area->at(i).y + 1;

		//探索終了点
		int end_x = this->calc_area->at(i).x + this->calc_area->at(i).width - 2;
		int end_y = this->calc_area->at(i).y + this->calc_area->at(i).height - 2;

		features->push_back(
			pair<Rect, std::vector<int>>(
				calc_area->at(i),
				calc_features(start_x, start_y, end_x, end_y, step_size)));
	}

	return features;
}


std::string HLACCalculator::get_result_string()
{
	using boost::adaptors::transformed;

	std::shared_ptr<vector<pair<Rect, std::vector<int>>>> features = get_result();
	std::string str = "";

	for (int i = 0; i < features->size(); i++)
	{
		std::stringstream sout;
		sout << features->at(i).first.x << "-" << features->at(i).first.y << "-" << features->at(i).first.width << "-" << features->at(i).first.height << ",";
		sout << boost::algorithm::join(features->at(i).second |
			transformed(static_cast<std::string(*)(int)>(std::to_string))
			, ",") << endl;
		str += sout.str();
	}

	return str;
}


/*
void HLACCalculator::get_result_file(std::string filepath)
{
	
}
*/


Mat HLACCalculator::get_mat_from_file(std::string filename)
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