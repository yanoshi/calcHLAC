//OpenCV用インクルード文
//インクルードファイル指定
#include <opencv2/opencv.hpp>
//静的リンクライブラリの指定
#include <opencv2/opencv_lib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<string>
#include<memory>

class HLACCalculator
{
public:
	HLACCalculator(std::string input_path, std::shared_ptr<std::vector<cv::Rect>> calc_area, int step_size);

	std::shared_ptr<std::vector<std::pair<cv::Rect, std::vector<int>>>> get_result();
	std::string get_result_string();
	
	//面倒なので、実装やめた
	//void get_result_file(std::string filepath);

private:
	cv::Mat input_mat;
	std::shared_ptr<std::vector<cv::Rect>> calc_area;

	uchar get_pixel(int x, int y);

	std::vector<int> calc_features(int start_x, int start_y, int end_x, int end_y, int step);

	cv::Mat get_mat_from_file(std::string filename);

	int step_size;
};