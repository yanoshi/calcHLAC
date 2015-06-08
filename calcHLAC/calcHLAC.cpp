#include <iostream>
#include <vector>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>


#include "hlac_calculator.h"

using namespace std;
using namespace boost::program_options;
using namespace cv;

int main(int argc, char* argv[])
{
	//オプション定義
	options_description inout_options("path setting");
	options_description calc_options("calculation option");

	inout_options.add_options()
		("input,i", value<string>(), "input filepath");
		//("output,o", value<string>(), "output filepath");

	calc_options.add_options()
		("calc_area,a", value<vector<string>>(), "set area for HLAC. format:[x,y,w,h x,y,w,h ...]")
		("step_size,s", value<int>()->default_value(1), "step size for HLAC");

	variables_map cmd_values;
	string input_src /*, output_src*/ ;
	std::shared_ptr<vector<Rect>> calc_area = std::shared_ptr<vector<Rect>>(new vector<Rect>());
	int step_size;

	try
	{
		options_description option_marged("");
		option_marged.add(inout_options).add(calc_options);


		store(parse_command_line(argc, argv, option_marged), cmd_values);
		notify(cmd_values);


		if (!(cmd_values.count("input")/* && cmd_values.count("output")*/))
		{
			std::cout << option_marged << endl;
			exit(0);
		}
		else
		{
			input_src = cmd_values["input"].as<string>();
			/*output_src = cmd_values["output"].as<string>();*/
		}
		
		step_size = cmd_values["step_size"].as<int>();
		
		if (cmd_values.count("calc_area"))
		{
			vector<string> temp_options(cmd_values["calc_area"].as<vector<string> >());
			for (vector<string>::iterator it = temp_options.begin(); it != temp_options.end(); ++it) {
				namespace alg = boost::algorithm;
				vector<string> splited;

				alg::split(splited, *it, alg::is_any_of(","));

				calc_area->push_back(cv::Rect(
					stoi(splited[0]),
					stoi(splited[1]),
					stoi(splited[2]),
					stoi(splited[3])
					));
			}
		}

	}
	catch (exception &e)
	{
		std::cout << e.what() << endl;
		exit(0);
	}

	HLACCalculator calculator(input_src, calc_area, step_size);
	std::cout << calculator.get_result_string() << endl;

#ifdef _DEBUG
	getchar();
#endif

	return 0;
}

