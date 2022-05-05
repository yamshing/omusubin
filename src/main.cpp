#include "omusubin.h"
#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
	Omusubin omusubin;
	 
	if (argc == 3) {
		std::string type = std::string(argv[1]);
		if (type == "insert") {
			 
			std::string conf_path = std::string(argv[2]);
			omusubin.InsertFromConf(conf_path);
			 
		}else if (type == "extract") {
			
			std::string bin_path = std::string(argv[2]);
			omusubin.Load(bin_path);
			 
			std::unordered_map<std::string, std::vector<unsigned char>> data_map{};
			 
			omusubin.GetBinDataMap(data_map);
			 
			for (auto & [key, data] : data_map) {
				 
				unsigned char* data_char_arr = const_cast<unsigned char*>(data.data());
				std::string file_extension;
				std::string data_key = key;
				omusubin.GetFileExtensionByKey(data_key, file_extension);
				 
				std::cout << "data.size() (in main.cpp) " << data.size() << std::endl;
				std::cout << "data_char_arr (in main.cpp) " << data_char_arr << std::endl;
				std::cout << "file_extension (in main.cpp) " << file_extension << std::endl;
				 
				std::stringstream ss_file_name;
				ss_file_name << key << "." << file_extension;
				 
				FILE *f1 = fopen(ss_file_name.str().c_str(), "wb");
				if (f1) {
					size_t r1 = fwrite(data_char_arr, sizeof data_char_arr[0], data.size(), f1);
					fclose(f1);
				}
				 
				 
			}
			 
			 
		}
		 
		 
	}
	return 0;
}
 
